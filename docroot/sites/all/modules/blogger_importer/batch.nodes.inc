<?php

/**
 * batch to create nodes from imported Blogger posts
 */

/**
 * #submit hook from trigger button
 */
function blogger_importer_trigger_nodes_batch($form, &$form_state) {
  $results = blogger_importer_results();

  if (empty($results)) {
    drupal_set_message(t("There is no imported Blogger data yet. Run the Process XML batch to populate the data."), 'error');
    drupal_goto('admin/content/blogger_importer');
  }

  // make sure user is allowed to create these nodes
  $node_type = variable_get('blogger_importer_node_type', '');
  if (empty($node_type)) {
    return drupal_set_message(t("No node type set for importing."), 'error');
  }
  if (! user_access('create ' . $node_type . ' content')) {
    return drupal_set_message(t("User does not have permission to create @type nodes.", array('@type' => $node_type)), 'error');
  }
  // Make sure that the tags field is valid.
  $field_name = variable_get('blogger_importer_node_tag_field', '');
  $instance = field_info_instance('node', $field_name, variable_get('blogger_importer_node_type', ''));
  if (module_exists('taxonomy') && $field_name && !$instance) {
    return drupal_set_message(t("Invalid tag fields selected for node type."), 'error');
  }

  $input_format = filter_format_load(variable_get('blogger_importer_format', filter_fallback_format()));
  if (empty($input_format) || ! filter_access($input_format)) {
    $format_info = filter_formats($input_format);
    return drupal_set_message(t("User does not have permission to use the input format <pre>@format : @inf</pre>, or no format is set.",
        array('@format' => print_r($format_info, TRUE), '@inf' => print_r(filter_list_format($input_format),TRUE))), 'error');
  }

  // make sure the node type allows for anonymous comments
  if (!user_access('post comments', drupal_anonymous_user())) {
    return drupal_set_message(t("Anonymoys user does not have permissions to post comments. Please !link to import comments.",
      array('!link' => l('correct this', 'admin/people/permissions') )
      ), 'error');
  }
  // make sure the node type allows for anonymous comments w/o fullcontact info
  // (can be '_MAY_NOT' or '_MAY_CONTACT' but not '_MAYNOT_CONTACT')
  $import_comments = variable_get('blogger_importer_comment_per_node', 0);
  if ($import_comments && variable_get('comment_anonymous_'. $node_type, COMMENT_ANONYMOUS_MAYNOT_CONTACT) == COMMENT_ANONYMOUS_MAYNOT_CONTACT) {
    return drupal_set_message(t("The content type %node_type does not allow for anonymous comments without contact info. Please !link to import comments.",
        array('%node_type' => $node_type, '!link' => l('correct this', 'admin/structure/types/manage/' . $node_type) )
        ), 'error');
  }

  // run the batch
  // pass all entries (not just 'posts')
  blogger_importer_batch_nodes($results, $import_comments);
}


// -- see blogger_importer.batch.inc for batch function notes --


/**
 * batch caller
 * @param $results cached imported entries
 */
function blogger_importer_batch_nodes($results, $import_comments) {
  $batch = array(
    'operations' => array(
      array('blogger_importer_batch_nodes_process', array($results, $import_comments)),
    ),
    'finished' => 'blogger_importer_batch_nodes_finished',
    'title' => t('Creating nodes from Blogger posts'),
    'init_message' => t('Beginning node import from Blogger posts'),
    'progress_message' => t('Importing posts...'),
    'error_message' => t('Blogger import has encountered an error.'),
    'file' => drupal_get_path('module', 'blogger_importer') . '/batch.nodes.inc',
  );

  batch_set($batch);

  // redirect when done
  batch_process('admin/content/blogger_importer/results');
}


/**
 * Batch Operation Callback for Node Import
 *
 * @param $initial_entries all initial entries to populate the sandbox and results (all types)
 * @param $context persistent batch info
 */
function blogger_importer_batch_nodes_process($initial_entries, $import_comments, &$context) {

  // initialize the sandbox
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;

    // pass back ALL entries as results, (new node info will be added)
    $context['results'] = $initial_entries;

    // keep 'post' entries in the sandbox until they're all processed
    $context['sandbox']['posts'] = $initial_entries['post'];
  }

  $results = &$context['results'];
  $posts = &$context['sandbox']['posts'];

  // track original # of entries (needed for progress bar)
  if (! isset($context['sandbox']['max'])) $context['sandbox']['max'] = count($posts);

  $limit = variable_get('blogger_importer_limit', 10);

  // message while batch is running
  $context['message'] = t('Processed @count out of @total posts, @remaining remaining (@limit at a time).',
    array('@count' => $context['sandbox']['progress'],
            '@remaining' => count($posts),
            '@total' => $context['sandbox']['max'],
            '@limit' => $limit,
          ));
  $entry_count = 0;

  // Load this up right at first.
  $vid = NULL;
  $field_name = variable_get('blogger_importer_node_tag_field', '');
  $instance = field_info_instance('node', $field_name, variable_get('blogger_importer_node_type', ''));
  if (module_exists('taxonomy') && $field_name && $instance) {
    $vid = variable_get('blogger_importer_tags_vid', 0);
  }

  // loop thru all the remaining entries
  foreach($posts as $id => $post) {

    $entry_count++;

    // is there already a node logged for this post?
    if (isset($post['nid']) && !empty($post['nid'])) {
      if ($node = node_load($post['nid']) ) {
        drupal_set_message(t("Post %title is already saved to node #@nid (!link), not duplicating. (Clear the cached entries to reset.)",
            array(
              '%title' => _blogger_importer_post_title($post),
              '@nid' => $post['nid'],
              '!link' => l($node->title, 'node/' . $node->nid, array('attributes' => array('target' => '_blank'))),
            )),
            'warning'
          );

        // remove from results (same as if created)
        unset($posts[ $post['id'] ]);
      }
      else {
        // nid saved but node doesn't exist!
        drupal_set_message(t("Post %title already has nid @nid logged, but node does not exist. Creating again.",
            array(
              '%title' => _blogger_importer_post_title($post),
              '@nid' => $post['nid'],
            ) ), 'warning' );

        unset($post['nid']);
      }
    }


    // if node does not yet exist
    if (! isset($post['nid']) || empty($post['nid'])) {
      // create a node!
      $node = new stdClass;

      // Find the author by email.
      $creator = user_load_by_mail($post['author_email']);
      if (!empty($post['author_email']) && $creator) {
        $node->uid = $creator->uid;
      }

      $node->type = variable_get('blogger_importer_node_type', '');

      // use title, date if no title, or 'Untitled' if no date
      // html_entity_decode needed?
      $node->title = _blogger_importer_post_title($post);

      $node->created = empty($post['time']) ? NULL: $post['time'];
      // Sometime in the future,user should choose language too
      $node->language = LANGUAGE_NONE;

      $node->comment = $import_comments;

      // Had to add it cause it kills import on non-utf8 values
      $post['value'] = decode_entities($post['value']);

      $node->body[LANGUAGE_NONE][0]['value'] = $post['value'];

      $node->body[LANGUAGE_NONE][0]['format'] = variable_get('blogger_importer_format', filter_fallback_format());

      if ($vid) {
        foreach ($post['tags'] as $tag) {
          $term = _blogger_importer_taxonomy_get_term_by_name($tag, $vid);
          if (!$term) {
            $term = (object) array(
              'vid' => $vid,
              'name' => $tag,
            );
            taxonomy_term_save($term);
          }
          $node->{$field_name}[LANGUAGE_NONE][] = array('tid' => $term->tid);
        }
      }

      // Download images.
      $directory = variable_get('blogger_importer_image_path', 'blogger_importer');
      if (!empty($directory)) {
        // Search for blogspot image references (thumbnails and linked images).
        if (preg_match_all('/(href|src)="(http:\/\/.*?\.blogspot\.com\/(.*?))"/', $node->body[LANGUAGE_NONE][0]['value'], $matches)
          || preg_match_all('/(href|src)="(http:\/\/.*?\.blogger\.com\/(.*?))"/', $node->body[LANGUAGE_NONE][0]['value'], $matches)) {
          // Set the destination directory.
          $directory = file_default_scheme() . '://' . $directory . '/';
          $replace = array();
          // Loop through image matches.
          foreach ($matches[2] as $key => $url) {
            $filename = pathinfo($matches[3][$key], PATHINFO_BASENAME);
            $dir_name = pathinfo($matches[3][$key], PATHINFO_DIRNAME);
            $folder = basename($dir_name);
            $filepath = $directory . $folder . '/' . $filename;
            // Image was already downloaded.
            if (file_exists($filepath)) {
              $replace[$url] = file_create_url($filepath);
            }
            // Image hasn't been download.
            else {
              // Create directory for new file.
              file_prepare_directory(dirname($filepath), FILE_CREATE_DIRECTORY);
              // Download the image.
              $response = drupal_http_request($url);
              // Image downloaded successfully.
              if ($response->code == 200) {
                // Save it.
                file_save_data($response->data, $filepath, FILE_EXISTS_REPLACE);
                $replace[$url] = file_create_url($filepath);
              }
            }
          }
          // Replace image URLs in node body.
          $node->body[LANGUAGE_NONE][0]['value'] = str_replace(array_keys($replace), array_values($replace),$node->body[LANGUAGE_NONE][0]['value']);
        }
      }

      // DEFINE A HOOK FOR OTHER MODULES TO ADD/MODIFY THE NEW NODE CONTENTS OR OTHERWISE ACT ON THEM
      // invoke hook as HOOK_blogger_importer_node_alter(&$node, $post, $op)
      // - only $op implemented now is 'presave' but might want more later
      // - ignores return value
      // example usage: map the original URL of each post to a redirect handler
      $op = 'presave';
      drupal_alter('blogger_importer_node', $node, $post, $op);

      // create/save the node
      node_save($node);

      if (is_numeric($node->nid)) {
        drupal_set_message(t("Saved node #@nid: !title from @post_id",
          array(
            '@nid' => $node->nid,
            '!title' => l($node->title, 'node/' . $node->nid),
            'post_id' => $post['id']) ),
          'status');

        // save nid in the results
        $context['results']['post'][ $post['id'] ]['nid'] = $node->nid;
      }
      else {
        drupal_set_message(t("No nid found for new post - unable to save %title", array('%title' => $post['title'])), 'error');
      }

      // remove processed post from sandbox (even if error)
      unset($posts[ $post['id'] ]);

    }

    if ($entry_count >= $limit ) {
      break;
    }

  }


  // Update our progress information.

  $context['sandbox']['progress'] = $context['sandbox']['max'] - count($posts);

  // [not sure if necessary]
  if (count($posts) == 0) $context['sandbox']['progress'] = $context['sandbox']['max'];

  $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  if ($context['finished'] > 1) {
    // just in case
    $context['finished'] = 1;
  }

  // in case progress calculations aren't right...
  if ($context['finished'] == 1 && count($posts) > 0) {
    drupal_set_message(t("Import ended prematurely! Still @size left to go but finished.",
        array('@size' => count($posts)) ), 'error');
    $context['success'] = FALSE;
  }
  if ($context['finished'] != 1 && count($posts) == 0
    ||  $context['sandbox']['progress'] > $context['sandbox']['max']) {
    drupal_set_message(t("Import should have ended! 0 left to go but batch still going."), 'error');
    $context['success'] = FALSE;
    $context['finished'] = 1;
  }

}



/**
 * Node batch 'finished' callback
 */
function blogger_importer_batch_nodes_finished($success, $results, $operations) {
  // save the new results
  if (!empty($results)) {
    blogger_importer_results($results);
  }

  if ($success) {

    // count the number of posts in $results w/ 'nid' set!
    // note: this doesn't neces mean they were all created now -- includes nodes previously created!

    $created_node_count = 0;
    $list = '';

    foreach($results['post'] as $post) {
      if (is_numeric($post['nid'])) {
        $created_node_count++;
      }
    }

    drupal_set_message( t('@count posts saved as nodes (some may have existed prior):',
        array('@count' => $created_node_count)));

  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $output = t('An error occurred while processing %error_operation with arguments: @arguments',
        array(
          '%error_operation' => $error_operation[0],
          '@arguments' => print_r($error_operation[1], TRUE)
      ));

    drupal_set_message($output, 'error');
  }

  // batch gets redirected to results page (from batch_process() above)
}

/**
 * Get a term by name for the given vocabulary.
 */
function _blogger_importer_taxonomy_get_term_by_name($name, $vid) {
  return reset(taxonomy_term_load_multiple(array(), array('name' => trim($name), 'vid' => $vid)));
}

<?php

/**
 * batch to create nodes from imported Blogger posts
 */


/**
 * #submit hook from trigger button
 */
function blogger_importer_trigger_comments_batch($form, &$form_state) {
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

  // make sure the node type allows for anonymous comments
  if (!user_access('post comments', drupal_anonymous_user())) {
    return drupal_set_message(t("Anonymoys user does not have permissions to post comments. Please !link to import comments.",
      array('!link' => l('correct this', 'admin/people/permissions') )
      ), 'error');
  }
  // make sure the node type allows for anonymous comments w/o fullcontact info
  // (can be '_MAY_NOT' or '_MAY_CONTACT' but not '_MAYNOT_CONTACT')
  if (variable_get('comment_anonymous_'. $node_type, COMMENT_ANONYMOUS_MAYNOT_CONTACT) == COMMENT_ANONYMOUS_MAYNOT_CONTACT) {
    return drupal_set_message(t("The content type %node_type does not allow for anonymous comments without contact info. Please !link to import comments.",
        array('%node_type' => $node_type, '!link' => l('correct this', 'admin/structure/types/manage/' . $node_type) )
        ), 'error');
  }

  // run the batch
  // pass all entries (not just 'posts')
  blogger_importer_batch_comments($results);
}



/**
 * batch caller
 * @param $results cached imported entries
 */
function blogger_importer_batch_comments($results) {
  $batch = array(
    'operations' => array(
      array('blogger_importer_batch_comments_process', array($results)),
      ),
    'finished' => 'blogger_importer_batch_comments_finished',
    'title' => t('Creating comments from Blogger comments'),
    'init_message' => t('Beginning comment import from Blogger comments'),
    'progress_message' => t('Importing comments...'),
    'error_message' => t('Blogger import has encountered an error.'),
    'file' => drupal_get_path('module', 'blogger_importer') . '/batch.comments.inc',
  );

  batch_set($batch);

  // redirect when done
  batch_process('admin/content/blogger_importer/results');
}




/**
 * Batch Operation Callback for Comment Import
 *
 * @param $initial_entries all initial entries to populate the sandbox and results (all types)
 * @param $context persistent batch info
 */
function blogger_importer_batch_comments_process($initial_entries, &$context) {

  // initialize the sandbox
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;    // should persist

    // Sort by the date of the comment so that comment.thread is correct.
    uasort($initial_entries['comment'], '_blogger_importer_comments_sort_date');

    // pass back ALL entries as results, (new comment info will be added)
    $context['results'] = $initial_entries;

    // keep 'comment' entries in the sandbox until they're all processed
    $context['sandbox']['comments'] = $initial_entries['comment'];
  }

  $results = &$context['results'];
  $comments = &$context['sandbox']['comments'];

  // track original # of entries (needed for progress bar)
  if (! isset($context['sandbox']['max'])) $context['sandbox']['max'] = count($comments);

  $limit = variable_get('blogger_importer_limit', 10);    // same limit as xml import

  // message while batch is running
  $context['message'] = t('Processed @count out of @total comments, @remaining remaining (@limit at a time).',
      array('@count' => $context['sandbox']['progress'],
            '@remaining' => count($comments),
            '@total' => $context['sandbox']['max'],
            '@limit' => $limit,
          ));

  $entry_count = 0;

  // loop thru all the remaining entries
  foreach($comments as $id => $comment) {

    $entry_count++;

    // is there already a comment id logged for this comment?
    if (isset($comment['cid']) && !empty($comment['cid'])) {
      if ($saved_comment = comment_load($comment['cid']) ) {

        drupal_set_message(t("Comment %title is already saved as #@cid (!link), not duplicating. (Clear the cached entries to reset.)",
            array(
              '%title' => $comment['title'],
              '@cid' => $comment['cid'],
              '!link' => '@todo finish porting', //l($node->title, 'node/' . $node->cid, array('attributes' => array('target' => '_blank'))),
            )),
            'warning'
          );

        // remove from results (same as if created)
        unset($comments[ $comment['id'] ]);
      }
      else {    // cid saved but comment doesn't exist!
        drupal_set_message(t("Comment %title already has cid @cid logged, but unable to load comment. Creating again.",
            array(
              '%title' => $comment['title'],
              '@cid' => $comment['cid'],
            ) ), 'warning' );

        unset($comment['cid']);   // so it's created fresh
      }
    }

    // if comment does not yet exist
    if (! isset($comment['cid']) || empty($comment['cid'])) {

      // need a nid to save a comment (attached to node)
      $comment_post_title = _blogger_importer_comment_post($comment['id'], $results, 'title');
      $comment_nid = _blogger_importer_comment_post($comment['id'], $results, 'nid');

      if (empty($comment_nid) || !is_numeric($comment_nid)) {
        drupal_set_message(t("No node saved for post %post_title, can't save comment %comment_title",
          array('%post_title' => $comment_post_title, '%comment_title' => $comment['title']) ), 'error');
      }
      else {
        // need to save a comment, and node exists to do so

        $comment_values = array(     // form values for comment_form
          'comment' => decode_entities($comment['value']),   // need to unescape html
          'subject' => decode_entities($comment['title']),
          'nid' => $comment_nid,
          'created' => empty($comment['time']) ? NULL: $comment['time'],
          'name' => isset($comment['author_name']) ? $comment['author_name'] : '',
          'uid' => 0 , //anonymous
          'language'=> LANGUAGE_NONE, //undefined language
          'mail' => isset($comment['author_email']) ? $comment['author_email'] : '',
          'homepage' => isset($comment['author_uri']) ? $comment['author_uri'] : '',
          'format' => variable_get('blogger_importer_format',filter_fallback_format()),    // ok to use same?
        );
        $comment_values['changed'] = $comment_values['created'];

        // @todo define a comment alter hook here similar to node above?

        $cid = _blogger_importer_create_comment($comment_values);

        // created?
        if (is_numeric($cid)) {
          drupal_set_message(t("Saved comment #@cid: %title for node @nid",
            array('@cid' => $cid, '%title' => $comment['title'], '@nid' => $comment_nid) ),
            'status');

          // save nid in the results
          $context['results']['comment'][ $comment['id'] ]['cid'] = $cid;

        }
        else {
          drupal_set_message(t("No cid found for new comment - unable to save %title",
              array('%title' => $comment['title'])), 'error');
          // [keeps going even if error]
        }

      }

    }


    // remove comment from sandbox (even if error or nothing done)
    unset($comments[ $comment['id'] ]);

    if ($entry_count >= $limit) {
      break;
    }

  }


  // Update our progress information.

  $context['sandbox']['progress'] = $context['sandbox']['max'] - count($comments);

  $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  if ($context['finished'] > 1) {
    // just in case
    $context['finished'] = 1;
  }
  // in case progress calculations aren't right...
  if ($context['finished'] == 1 && count($comments) > 0) {
    drupal_set_message(t("Import ended prematurely! Still @size left to go but finished.",
        array('@size' => count($comments)) ), 'error');
    $context['success'] = FALSE;
  }
  if ($context['finished'] != 1 && count($comments) == 0
    ||  $context['sandbox']['progress'] > $context['sandbox']['max']) {
    drupal_set_message(t("Import should have ended! 0 left to go but batch still going."), 'error');
    $context['success'] = FALSE;
    $context['finished'] = 1;
  }

}





/**
 * create a COMMENT given comment form values
 */
function _blogger_importer_create_comment($values) {
  if (! $nid = $values['nid']) {
    drupal_set_message(t("Missing nid for new comment %title",
        array('%title' => $values['subject'])), 'error');
    return FALSE;
  }
  if (! $node = node_load($nid)) {
    drupal_set_message(t("Node #@nid for new comment %title does not exist",
        array('@nid' => $nid, '%title' => $values['subject'])), 'error');
    return FALSE;
  }

  $values = array_merge(
    array(
      'status' => COMMENT_PUBLISHED,
    ), (array) $values);

  $comment = (object) $values;

  $comment->comment_body[LANGUAGE_NONE][0]['value'] = $comment->comment;

  $comment->comment_body[LANGUAGE_NONE][0]['format'] = $comment->format;

  comment_save($comment);

  if (!isset($comment->cid)) {
    drupal_set_message(t("Unable to save comment %title to node #@nid", array('%title' => $values['subject'], '@nid' => $nid)));
  }

  return $comment->cid;
}



/**
 * Comments batch 'finished' callback
 */
function blogger_importer_batch_comments_finished($success, $results, $operations) {

  // save the new results
  if (!empty($results)) blogger_importer_results($results);

  if ($success) {

    // count the number of posts in $results w/ 'cid' set!
    // note: this doesn't neces mean they were all created now -- includes comments previously created!

    $created_comment_count = 0;
    $list = '';

    foreach($results['comment'] as $comment) {
      if (is_numeric($comment['cid'])) {
        $created_comment_count++;

        $comment_title = _blogger_importer_post_title($comment);

        $list .= '<li>'
          . l($comment_title, 'comment/' . $comment['cid'], array('fragment' => 'comment-' . $comment['cid']))
          . ' - ' . format_date($comment['time']) . '</li>';
      }
    }

    drupal_set_message( t('@count comments saved to their matching nodes (some may have existed prior):',
        array('@count' => $created_comment_count ) ) );
    drupal_set_message( '<ul>' . $list . '</ul>' );

  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $output = t('An error occurred while processing %error_operation with arguments: @arguments',
        array(
          '%error_operation' => $error_operation[0],
          '@arguments' => print_r($error_operation[1], TRUE),
        ));

    drupal_set_message($output, 'error');
  }

  // batch gets redirected to results page (from batch_process() above)
}

/**
 * Sort comment by date.
 */
function _blogger_importer_comments_sort_date($comment, $comment2) {
  return ($comment['time'] > $comment2['time']) ? 1 : -1;
}

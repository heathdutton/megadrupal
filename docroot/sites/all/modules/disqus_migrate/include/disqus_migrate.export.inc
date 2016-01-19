<?php

/**
 * @file
 * Provides export functions
 */

/**
 * Menu callback for the export settings
 */
function disqus_migrate_admin_export_settings() {
  $form = array();

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Export Settings'),
    '#description' => t('Select the following options to effect what comments are exported.'),
  );
  $status_options = array(
    '--' => t('No Change'),
    0 => t('Disabled'),
    1 => t('Read Only'),
  );
  $form['settings']['disqus_migrate_export_status_alter'] = array(
    '#type' => 'select',
    '#default_value' => variable_get('disqus_migrate_export_status_alter', '--'),
    '#title' => t('Node comment status change'),
    '#description' => t('After comments for a node have been exported, the commenting status can be altered. Useful for disabling Drupal commenting once comments have been exported.'),
    '#options' => $status_options,
  );

  $form['settings']['disqus_migrate_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Override base url.'),
  );

  return system_settings_form($form);
}

/**
 * Menu callback for the export actions
 */
function disqus_migrate_admin_export() {
  $form = array();

  $form['export'] = array(
    '#type' => 'fieldset',
    '#title' => t('Export Now'),
  );

  $subquery = db_select('disqus_migrate', 'dm')->fields('dm', array('cid'));
  $pending_comments = db_select('comment', 'c')->condition('c.cid', $subquery, 'NOT IN')->countQuery()->execute()->fetchField();

  $form['export']['pending'] = array(
    '#prefix' => '<p>',
    '#markup' => t('There are @pending comments awaiting export.', array('@pending' => $pending_comments)),
    '#suffix' => '</p>',
  );

  $form['export']['export_xml'] = array(
    '#type' => 'submit',
    '#value' => t('Export all comments to XML file'),
    '#submit' => array('_disqus_migrate_export_wxr'),
  );

  return $form;
}


/**
 * Calls function to gather comments to export, then builds the appropriate XML
 * file and presents it to the user for download.
 */
function _disqus_migrate_export_wxr($form, &$form_state) {

  // Gather ALL of the comments.
  $thread_data = _disqus_migrate_export_gather(TRUE);

  $status_change = variable_get('disqus_migrate_export_status_alter', '--');

  if (!empty($thread_data)) {
    // Pass the data to a function that generates the XML.
    $output = '';

    // Print header.
    $output .= '<?xml version="1.0" encoding="UTF-8"?>' . "\n";
    $output .= '<rss version="2.0"' . "\n";
    $output .= '  xmlns:content="http://purl.org/rss/1.0/modules/content/"' . "\n";
    $output .= '  xmlns:dsq="http://www.disqus.com/"' . "\n";
    $output .= '  xmlns:dc="http://purl.org/dc/elements/1.1/"' . "\n";
    $output .= '  xmlns:wp="http://wordpress.org/export/1.0/"' . "\n";
    $output .= '>' . "\n";
    $output .= '  <channel>' . "\n";

    foreach ($thread_data as $nid => $thread) {
      /* Skip thread if there were no comments attached to it. This would only
       * happen if an export is created and there are no published comments on
       * a node (also depends on what user selects in checkbox).
       */
      if (empty($thread['comments'])) {
        continue;
      }

      // The link must have a scheme
      $parsed_url = parse_url($thread['link']);
      if (empty($parsed_url['scheme'])) {
        $thread['link'] = 'http://' . $thread['link'];
      }

      $output .= '<item>' . "\n";
      $output .= '<title>' . _disqus_migrate_cleanse_xml($thread['title']) . '</title>' . "\n";
      $output .= '<link>' . $thread['link'] . '</link>' . "\n";
      $output .= '<content:encoded></content:encoded>' . "\n";
      $output .= '<dsq:thread_identifier>' . $thread['identifier'] . '</dsq:thread_identifier>' . "\n";
      $output .= '<wp:post_date_gmt>' . $thread['post_date_gmt'] . '</wp:post_date_gmt>' . "\n";
      $output .= '<wp:comment_status>open</wp:comment_status>' . "\n";

      foreach ($thread['comments'] as $comment) {

        $output .= '<wp:comment>' . "\n";

        $output .= '<wp:comment_id>' . $comment['id'] . '</wp:comment_id>' . "\n";
        $output .= '<wp:comment_author><![CDATA[' . $comment['author'] . ']]></wp:comment_author>' . "\n";
        $output .= '<wp:comment_author_email>' . $comment['author_email'] . '</wp:comment_author_email>' . "\n";
        $output .= '<wp:comment_author_url><![CDATA[' . $comment['author_url'] . ']]></wp:comment_author_url>' . "\n";
        $output .= '<wp:comment_author_IP>' . $comment['author_IP'] . '</wp:comment_author_IP>' . "\n";
        $output .= '<wp:comment_date_gmt>' . $comment['date_gmt'] . '</wp:comment_date_gmt>' . "\n";
        $output .= '<wp:comment_content><![CDATA[' . disqus_migrate_filter($comment['content']) . ']]></wp:comment_content>' . "\n";
        $output .= '<wp:comment_approved>' . $comment['approved'] . '</wp:comment_approved>' . "\n";
        $output .= '<wp:comment_parent>' . $comment['parent'] . '</wp:comment_parent>' . "\n";

        $output .= '</wp:comment>' . "\n";
      }

      $output .= '</item>' . "\n";

      // Should change the comment status for the node?
      if ($status_change != '--') {
        // $update = db_query("UPDATE {node} SET comment = %d WHERE nid = %d", $status_change, $nid);
      }
    }

    // Footer.
    $output .= '  </channel>' . "\n";
    $output .= '</rss>' . "\n";

    // Output the XML file.
    header("Content-disposition: attachment; filename=drupalcomments.xml");
    header("Content-Type: text/xml; charset=utf-8");

    print $output;

    exit();
  }
  else {
    drupal_set_message(t('No comments to export.'), 'error');
  }

}

/**
 * Helper function to gather comments to export. The boolean parameter
 * determines whether or not to select all the comments or all comments since
 * last export
 */
function _disqus_migrate_export_gather($select_all = FALSE) {
  // Select all the nodes with comments.
  $thread_data = array();

  // Get max amount of comments to gather.
  $export_threshold = variable_get('disqus_migrate_export_api_limit', 100);
  $comments_gathered = 0;

  // Gather distinct node IDs from comments.
  if ($select_all) {
    $node_results = db_select('comment', 'c')
      ->fields('c', array('nid'))
      ->distinct()
      ->execute();
  }
  else {
    // Find all comments that haven't already been imported/exported via this
    // module. Let's also limit the result to prevent
    // any gathering an unnecessary amount of comments.
    $node_results = db_select('comment', 'c')
      ->distinct()
      ->fields('c', array('nid'))
      ->leftJoin('disqus_migrate', 'dm', 'c.cid = dm.cid')
      ->where('dm.cid IS NULL')
      ->range(0, $export_threshold)
      ->execute();
  }

  $max_comment_id = 0;

  foreach ($node_results as $result) {
    $nid = $result->nid;

    // Get some bits of info from the node. This is much more efficient
    // than doing a node_load.
    // $node_data_query = db_query("SELECT title, created, status FROM {node} WHERE nid = %d", $nid);
    $node_data = db_select('node', 'n')->fields('n', array('title', 'created', 'status'))->condition('n.nid', $nid)->execute()->fetchAssoc();
    $disqus_migrate_base_url = variable_get('disqus_migrate_base_url', $_SERVER['HTTP_HOST']);
    // Load up the thread data array for this node
    $thread_data[$nid] = array(
      'title' => $node_data['title'],
      // Use the aliased version.
      'link' => $disqus_migrate_base_url . url("node/" . $nid),
      'identifier' => 'node/' . $nid,
      'post_date_gmt' => date("Y-m-d H:i:s", $node_data['created']),
      'post_date_gmt_unix' => $node_data['created'],
    );

    // Find all comments attached to this node
    if ($select_all) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'comment')->propertyCondition('nid', $nid);
      $result = $query->execute();
      $cids = array();
      foreach ($result['comment'] as $record) {
        $cids[] = $record->cid;
      }
      $comments = entity_load('comment', $cids);
    }
    else {
      $comment_query = "SELECT c.* FROM {comments} c LEFT JOIN {disqus_migrate} dm ON c.cid = dm.cid WHERE dm.cid IS NULL AND c.nid = %d ORDER BY c.cid ASC";
    }


    foreach ($comments as $comment) {
      // Set max comment ID.
      $max_comment_id = ($comment->cid > $max_comment_id) ? $comment->cid : $max_comment_id;
      // Randomize emails for anonymous comments - otherwise all the anonymous
      // comments would appear to come from the same account.
      $random = rand(1000, 100000);
      if ($comment->mail) {
        $comment_mail = $comment->mail;
      }
      else if (!empty($comment->uid)) {
        $author = entity_load('user', array($comment->uid));
        $comment_mail = $author[$comment->uid]->mail;
      }
      else {
        $comment_mail = "anon" . $random . "@anonymous.com";
      }

      // Use name "Anonymous" for anonymous users.
      if ($comment->name) {
        $comment_name = $comment->name;
      }
      else {
        $comment_name = "Anonymous";
      }

      // Load up the comment data for this comment.
      $thread_data[$nid]['comments'][$comment->cid] = array(
        'id' => $comment->cid,
        'author' => $comment_name,
        'author_email' => $comment_mail,
        'author_url' => $comment->homepage,
        'author_IP' => $comment->hostname,
        'date_gmt' => date("Y-m-d H:i:s", $comment->created),
        'date_gmt_unix' => $comment->created,
        'content' => $comment->comment_body['und'][0]['value'],
        'approved' => 1,
        'parent' => $comment->pid,
      );

      $comments_gathered++;
      // Check to see if we reached limit of comments we shoudl export.
      if ($comments_gathered >= $export_threshold && !$select_all) {
        break 2;
      }
    }
  }

  return $thread_data;
}

/**
 * Helper function cleanse a string for placement in an XML file.
 */
function _disqus_migrate_cleanse_xml($string) {
  $find = array(
    '>',
    '<',
    '&',
    '"',
    "'",
  );

  $replace = array(
    '&gt;',
    '&lt;',
    '&amp;',
    '&apos;',
    '&quot;',
  );

  return str_replace($find, $replace, $string);
}

/**
 * Scan input and make sure that all HTML tags are properly closed and nested.
 */
function disqus_migrate_filter($text) {
  $text = str_replace("\r", '', $text);
  $text = filter_dom_serialize(filter_dom_load($text));
  return str_replace(']]>', ']]]]><![CDATA[>', $text);
}


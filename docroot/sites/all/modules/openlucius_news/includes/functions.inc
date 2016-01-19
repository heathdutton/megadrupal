<?php
/**
 * @file
 * This file contains functions for the news.
 */

// Define openlucius status updates limit.
define('OPENLUCIUS_NEWS_INTRO_TEXT_WORDS', 15);
define('OPENLUCIUS_PROMOTED_NEWS_LIMIT', 1);

/**
 * Function to start a minimal query.
 */
function openlucius_news_basic_query() {

  // Basic select query from heartbeat activity.
  $query = db_select('heartbeat_activity', 'h');

  // Join on the node table.
  $query->leftJoin('node', 'n', 'n.nid = h.nid');

  // Join on the data body table.
  $query->leftJoin('field_data_body', 'b', 'b.entity_id = h.nid');

  // Fetch fields from the heartbeat activity table.
  // Fetch fields from the node table.
  // Only fetch published items.
  // Filter on node type ol news.
  // Respect grants.
  $query->fields('h', array(
    'uid',
    'nid',
    'message_id',
    'timestamp',
  ))
        ->fields('n', array('title', 'type'))
        ->condition('n.status', NODE_PUBLISHED, '=')
        ->condition('n.type', 'ol_news', '=')
        ->addTag('node_access');

  // Allow other modules to changes the query.
  drupal_alter('openlucius_news_basic_query', $query);

  // Return.
  return $query;
}

/**
 * Get the latest news items and order by heartbeat activity id.
 */
function openlucius_news_get_latest_news_items() {

  // Unique pager id for ajax.
  $pager_id = openlucius_core_pager_counter('news');

  // Get the basic query to build on.
  $query = openlucius_news_basic_query();

  // Fields from the data body table.
  $query->fields('b', array('body_value'));

  // Select not the first promoted item, but wel the others.
  $latest = openlucius_news_get_latest_promoted_news_item();

  // Condition to show all items but that last promoted item.
  $query->condition('n.nid', $latest, '<>');

  // Group items by node id.
  $query->groupBy('n.nid');

  // Extend for the page.
  $query = $query->extend('PagerDefault');

  // Add the element.
  $query->element($pager_id);

  // Get the news limit.
  $limit = variable_get('openlucius_news_news_limit', 20);

  // Limit the output.
  $query->limit($limit);

  // Order by heartbeat id descending.
  $query->orderby('uaid', 'desc');

  // Allow other modules to changes the query.
  drupal_alter('openlucius_news_get_latest_news_items', $query);

  // Return.
  return $query->execute()->fetchAll();
}

/**
 * Get the latest news items and order by promoted.
 */
function openlucius_news_get_promoted_news_items() {

  // Get the basic query to build on.
  $query = openlucius_news_basic_query();

  // Add a join to the shared files.
  $query->addJoin('left', 'field_data_field_shared_files', 'f', 'f.entity_id = n.nid');

  // Add join to check the file type.
  $query->addJoin('left', 'file_managed', 'm', 'f.entity_id = m.fid');

  // Get the file id.
  $query->fields('f', array('field_shared_files_fid'));

  // Fields from the data body table.
  $query->fields('b', array('body_value'));

  // Order by heartbeat id descending.
  $query->condition('n.promote', NODE_PROMOTED, '=');

  // Order by heartbeat id descending.
  $query->orderby('uaid', 'desc');

  // Order by heartbeat id descending.
  $query->orderby('n.promote', 'desc');

  // Limit to OPENLUCIUS_PROMOTED_NEWS_LIMIT.
  $query->range(0, OPENLUCIUS_PROMOTED_NEWS_LIMIT);

  // Allow other modules to changes the query.
  drupal_alter('openlucius_news_get_promoted_news_items', $query);

  // Return.
  return $query->execute()->fetchAll();
}

/**
 * Get the latest news items and order by promoted.
 */
function openlucius_news_get_latest_promoted_news_item() {

  // Select the last promoted news item.
  $query = db_select('node', 'n');

  // Select just the node id.
  $query->fields('n', array('nid'));

  // Make sure it is promoted.
  $query->condition('promote', NODE_PROMOTED, '=');

  // Order by changed timestamp descending.
  $query->orderby('changed', 'desc');

  // Limit one.
  $query->range(0, 1);

  // Return the field.
  return $query->execute()->fetchField();
}

/**
 * Function to generate the output for the regular news items timeline.
 * @return string
 *   Returns a string of output.
 */
function openlucius_news_build_news_items_display() {

  // Initialize the output.
  $output = '';

  // // Add news item link if the user has access.
  if (user_access('create ol_news content')) {
    $output .= '<div class="add-news-button">' . l(t('Add news item'), 'node/add/ol-news', array(
        'attributes' => array('class' => array('btn btn-xs btn-default add-ol-news')),
      )) . '</div>';
  }

  // Get the promoted news items.
  $news_items = openlucius_news_get_latest_news_items();

  // Initialize news items variable.
  $vars['news_items'] = '';

  // Initialize build.
  $build = array();

  // Loop through the results.
  foreach ($news_items as $news_item) {

    // Initialize item array.
    $item = array();

    $item['nid']          = $news_item->nid;
    $item['title']        = check_plain($news_item->title);
    $item['time_ago']     = format_interval(($_SERVER['REQUEST_TIME'] - $news_item->timestamp), 2);
    $item['between_text'] = t('posted');
    // Get the body.
    $body = check_markup($news_item->body_value);
    // Remove <a></a> tags, they will break our big-link block.
    $body         = preg_replace('/<a[^>]*>/i', '', $body);
    $item['body'] = preg_replace('/<\/a>/i', '', $body);

    // The link to the detail page of the news item.
    $item['link'] = url('node/' . $item['nid']);

    // Do a user load.
    $user             = user_load($news_item->uid);
    $item['username'] = check_plain($user->name);

    // Empty it, else a user without image will get image from another user.
    $item['picture'] = '';

    // Create an image styled image.
    if (isset($user->picture->uri)) {
      $item['picture'] = theme('image_style', array(
        'style_name' => 'ol_50x50',
        'path'       => $user->picture->uri,
      ));
    }

    // Get the node like flag.
    $item['like_flag'] = openlucius_core_social_create_like_node_flag($news_item->nid);

    // Get the likes on the news item.
    $item['likes'] = '<span id="like-node-' . $news_item->nid . '">' . openlucius_core_social_get_like_count($news_item->nid, 'openlucius_core_social_like_node') . '</span>';

    // Get the attachments.
    $attachments = db_select('field_data_field_shared_files', 'f')
      ->fields('f', array('field_shared_files_fid'))
      ->condition('f.field_shared_files_display', 1, '=')
      ->condition('entity_id', $news_item->nid, '=')
      ->execute()
      ->fetchAll();

    // Process attachments.
    _openlucius_core_process_recent_stuff_attachments($attachments, $item);

    // Build rows.
    $vars['news_items'] .= theme('openlucius_news_news_item', array('vars' => $item));
  }

  // Stuff rows in wrapper.
  $content = theme('openlucius_news_news_items', array('vars' => $vars));

  // Add the content.
  $build['content'] = array('#markup' => $content);

  // Get the news limit.
  $limit = variable_get('openlucius_news_news_limit', 20);

  $build['pager'] = array(
    '#markup' => theme('pager', array(
      'element'  => $GLOBALS['pager_id'],
      'quantity' => $limit,
    )),
  );

  // Build the output.
  $output .= drupal_render($build);

  return $output;
}

/**
 * Function to generate the output for the promoted ne$limitms timeline.
 * @return string
 *   Returns a string of output.
 */
function openlucius_news_build_promoted_news_items_display() {

  // Get the promoted news items.
  $promoted_items = openlucius_news_get_promoted_news_items();

  // Initialize promoted news items variable.
  $output = '';

  // Loop through the results.
  foreach ($promoted_items as $item) {
    // Add the promoted news item link to the output.
    $output .= openlucius_news_build_promoted_news_item($item);
  }

  // Return the build.
  return $output;
}

/**
 * Function to format a link for promoted news.
 *
 * @param \stdClass $item
 *   The item to format the link for.
 *
 * @return string
 *   Returns the link as html string.
 */
function openlucius_news_build_promoted_news_item($item) {

  // Initialize item array.
  $vars = array();

  // Set the variables for templating.
  $vars['nid']      = $item->nid;
  $vars['title']    = $item->title;
  $vars['time_ago'] = format_interval(($_SERVER['REQUEST_TIME'] - $item->timestamp), 2);

  // Get the intro text.
  $vars['intro'] = text_summary($item->body_value);

  // Start the attachments query.
  $attachment_query = db_select('field_data_field_shared_files', 'f')
    ->fields('f', array('field_shared_files_fid'))
    ->condition('f.field_shared_files_display', 1, '=')
    ->condition('entity_type', 'node', '=')
    ->condition('entity_id', $item->nid, '=');

  // Fetch all attachment file ids.
  $attachments = $attachment_query->execute()->fetchAll();

  // Initialize empty image.
  $vars['image'] = '';

  // Loop through the files.
  foreach ($attachments as $attachment) {

    // Load the file.
    $file = file_load($attachment->field_shared_files_fid);

    // Check if the file exists.
    if (file_exists($file->uri)) {

      // Check if the file is an image.
      if (stristr($file->filemime, 'image/')) {

        // Create images array.
        $images[] = $file;

        // Check images array.
        if (is_array($images)) {

          // Create image from the first image.
          $vars['image'] = theme('image_style', array(
            'style_name' => 'ol_100x100',
            'path'       => $images[0]->uri,
          ));
        }
      }
    }
  }

  // Create the output;
  $output = theme('openlucius_news_promoted_news_item', array('vars' => $vars));

  // Return the output.
  return $output;
}

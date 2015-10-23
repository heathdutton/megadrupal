<?php

/**
 * @file
 * Functions for handling pings notifying us of Ooyala backlot updates.
 */

/**
 * Menu callback; Respond to Ooyala pings when a video has finished processing.
 * This function does not actually process the ping requeust, but instead
 * inserts an embed code to the ooyala_ping queue to be processed.
 *
 * This callback responds to the URL "ooyala/ping" within the Drupal site.
 * Typically this URL needs to be configured to receive pings from Ooyala.com
 * when a video has finished processing and is ready to be displayed. The
 * configuration for setting up a ping URL is at
 * https://www.ooyala.com/backlot/web, underneath the Account => Developers
 * tab. Enter a URL in the API Ping URL such as http://example.com/ooyala/ping,
 * if Drupal is installed at the base of the domain.
 *
 */
function ooyala_ping_insert() {
  // Disable the page cache for this request.
  drupal_page_is_cacheable(FALSE);

  $verbose = variable_get('ooyala_ping_reporting_verbose', FALSE);

  if (isset($_GET['embedCode'])) {
    $data = array(
      'embed_code' => $_GET['embedCode'],
      'fetch_attempts' => 0,
    );

    if ($verbose) {
      watchdog('ooyala_ping', 'Received ping from Ooyala Backlot for embedCode %embed_code with request <pre>@request</pre>.', array('%embed_code' => $_GET['embedCode'], '@request' => print_r($_REQUEST, TRUE)));
    }

    $queue = ooyala_ping_queue();
    $queue->createItem($data);
  }
  else {
    if ($verbose) {
      watchdog('ooyala_ping', 'Received embed_code-less ping from Ooyala Backlot with request <pre>@request</pre>.', array('@request' => print_r($_REQUEST, TRUE)));
    }
    else {
      watchdog('ooyala_ping', 'Recieved ping without an embed code. No action has been taken.', array(), WATCHDOG_ERROR);
    }
  }
}

/**
 * Return the Ooyala Ping queue, creating it if needed.
 *
 * @return
 *   An instance of the default queuing mechanism used for storing Ooyala pings
 *   to process.
 */
function ooyala_ping_queue() {
  $queue = DrupalQueue::get('ooyala_ping');
  $queue->createQueue();
  return $queue;
}

/**
 * Process items in the Ooyala ping queue.
 *
 * @param $limit
 *   Optional parameter indicating the number of items to process at once.
 */
function ooyala_ping_queue_process($limit = NULL) {
  $queue = ooyala_ping_queue();

  if (!$limit) {
    $limit = 10;
  }

  // Limit to at most the number of items in the queue. This prevents us from
  // repeatedly fetching the same embed code during the same processing phase
  // if the embed code is encountering errors processing.
  $limit = min($limit, $queue->numberOfItems());

  // Keep an array of all embed codes processed during this request. We use
  // this to prevent multiple API calls where the same embed code has been
  // pinged in beteween queue runs. We don't worry about uniqueness across
  // multiple requests to save us from having to handle any potential race
  // conditions where another API call is actually needed.
  $processed = array();

  while (count($processed) < $limit && $item = $queue->claimItem()) {
    $data = $item->data;
    $embed_code = $data['embed_code'];

    if (!in_array($embed_code, $processed)) {
      $processed[] = $embed_code;

      if (!ooyala_ping_process($data['embed_code'])) {
        $data['fetch_attempts']++;
        if ($data['fetch_attempts'] < variable_get('ooyala_ping_fetch_attempts', 10)) {
          $queue->createItem($data);
        }
        else {
          watchdog('ooyala_ping', 'Unable to process ping request for embed code %embed_code after !count attempts. Discarding the embed code from the queue.', array('%embed_code' => $data['embed_code'], '!count' => $data['fetch_attempts']), WATCHDOG_ERROR);
        }
      }
    }
    $queue->deleteItem($item);
  }
}

/**
 * Process an embed code that has been queued.
 *
 * @param $embed_code
 *   The embed code to process by syncing data such as video metadata and node
 *   statuses.
 *
 * @return
 *   TRUE if the embed code was processed successfully, or FALSE otherwise.
 */
function ooyala_ping_process($embed_code) {
  module_load_include('inc', 'ooyala', 'includes/ooyala.api');
  $verbose = variable_get('ooyala_ping_reporting_verbose', FALSE);

  $embed_args = array('%embed_code' => $embed_code);

  if ($video = ooyala_api_video_load($embed_code)) {
    // If we can't fetch an image for a video, odds are the rest of the
    // request will fail too. So, we bail here and don't publish the node so
    // anything dependent on the thumbnail image (such as views) won't break.
    if (!_ooyala_ping_fetch_image($embed_code, $embed_args)) {
      return FALSE;
    }

    if ($nodes = ooyala_load_nodes($embed_code)) {
      foreach ($nodes as $node) {
        _ooyala_ping_autopublish($node, $embed_args);
        _ooyala_ping_metadata($node, $video, $embed_code, $embed_args);
      }
    }
    else {
      watchdog('ooyala_ping', 'Content matching the embed code %embed_code was not found. No action has been taken.', $embed_args, WATCHDOG_WARNING);
      return FALSE;
    }
  }
  else {
    watchdog('ooyala_ping', 'A matching video for the %embed_code could not be loaded from Ooyala.', $embed_args, WATCHDOG_ERROR);
    return FALSE;
  }

  return TRUE;
}

/**
 * Fetch an image in response to an Ooyala ping.
 *
 * @param $embed_code
 *   The embed code to fetch the image for.
 * @param $embed_args
 *   An array of arguments for substition into translated watchdog messages.
 *
 * @return
 *   The image path of the fetched image, or FALSE if one could not be loaded.
 */
function _ooyala_ping_fetch_image($embed_code, $embed_args) {
  $image_path = NULL;
  if ($image_path = ooyala_api_fetch_image($embed_code)) {
    if (variable_get('ooyala_ping_reporting_verbose', FALSE)) {
      watchdog('ooyala_ping', 'The video image for %embed_code has been downloaded to <a href="!url">@filename</a>.', $embed_args + array('!url' => file_create_url($image_path), '@filename' => basename($image_path)));
    }
  }
  else {
    watchdog('ooyala_ping', 'There was an error downloading the video image for %embed_code.', $embed_args, WATCHDOG_ERROR);
    return FALSE;
  }

  return $image_path;
}

/**
 * Mark a node as published if the video has been published.
 *
 * @param &$node
 *   The node object to mark as published.
 * @param $embed_args
 *   An array of arguments for substition into translated watchdog messages.
 */
function _ooyala_ping_autopublish(&$node, $embed_args) {
  $verbose = variable_get('ooyala_ping_reporting_verbose', FALSE);
  if (variable_get('ooyala_autopublish', 0) && !$node->status) {
    $node->status = 1;
    node_save($node);
    watchdog('ooyala_ping', 'The content <a href="!url">%title</a> has been published by a ping for %embed_code.', $embed_args + array('!url' => url('node/' . $node->nid), '%title' => $node->title));
  }
  elseif (!variable_get('ooyala_autopublish', 0)) {
    if ($verbose) {
      watchdog('ooyala_ping', 'The content <a href="!url">%title</a> with embed code %embed_code was not published as automatic publishing is disabled.', $embed_args + array('!url' => url('node/' . $node->nid), '%title' => $node->title));
    }
  }
  else {
    if ($verbose) {
      watchdog('ooyala_ping', 'The content <a href="!url">%title</a> with embed code %embed_code was already published.', $embed_args + array('!url' => url('node/' . $node->nid), '%title' => $node->title), WATCHDOG_WARNING);
    }
  }
}

/**
 * Update the saved metadata in a node's video field.
 *
 * @param &$node
 *   The node object to mark as published.
 * @param $video
 *   The video array as loaded from the Ooyala API.
 * @param $embed_code
 *   The embed code to fetch the image for.
 * @param $embed_args
 *   An array of arguments for substition into translated watchdog messages.
 */
function _ooyala_ping_metadata(&$node, $video, $embed_code, $embed_args) {
  $verbose = variable_get('ooyala_ping_reporting_verbose', FALSE);
  foreach ($node->ooyala_field_names as $field_name) {
    // We explicitly only allow a delta of 1 for fields, so we can hardcode the
    // array index.
    foreach ($node->{$field_name}['und'][0] as $key => $value) {
      if ($value['value'] == $embed_code) {
        // Update the meta data in the node if changed.
        if (empty($value['status']) || $value['status'] != $video['status']) {
          $node->{$field_name}[$key]['status'] = $video['status'];
          $node->pulled_from_ooyala = TRUE;
        }
        if (empty($value['length']) || $value['length'] != $video['length']) {
          $node->{$field_name}[$key]['length'] = $video['length'];
          $node->pulled_from_ooyala = TRUE;
        }
      }
    }
  }
  if (!empty($node->pulled_from_ooyala)) {
    if ($verbose) {
      watchdog('ooyala_ping', 'Updated video %embed_code status (@status) and length (@length) values.', $embed_args + array('@status' => $video['status'], '@length' => $video['length']));
    }
    node_save($node);
  }
  else {
    if ($verbose) {
      watchdog('ooyala_ping', 'No changes necessary to video status or length for embed code %embed_code.', $embed_args);
    }
  }
}


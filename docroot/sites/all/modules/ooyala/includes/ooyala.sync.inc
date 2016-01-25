<?php

/**
 * @file
 * Code responsible for checking and synchronizing general data with Ooyala.
 */

/**
 * Main syncing function to pull information from Ooyala for certain videos.
 *
 * @param $videos
 *   A list of video items returned from ooyala_api_video_load() or
 *   ooyala_api_video_query(). This video data will be compared against local
 *   nodes and only changed data will be updated locally.
 */
function ooyala_sync_pull_videos($videos) {
  // Build a map of all nodes and embed codes.
  $nodes = array();
  $node_map = array(
    'nids' => array(),
    'embed_codes' => array(),
  );
  foreach ($videos as $embed_code => $video) {
    $embed_code_nodes = ooyala_load_nodes($embed_code);
    if (empty($embed_code_nodes)) {
      unset($videos[$embed_code]);
    }
    else {
      foreach ($embed_code_nodes as $nid => $node) {
        $nodes[$nid] = $node;
        $node_map['nids'][$nid][$embed_code] = $embed_code; // Lookup by NID.
        $node_map['embed_codes'][$embed_code][$nid] = $nid; // Lookup by embed code.
      }
    }
  }

  // Allow each module to handle what information needs to be pulled out.
  module_load_include('inc', 'ooyala', 'includes/ooyala.sync');
  foreach (module_implements('ooyala_sync_pull_videos') as $module) {
    $function = $module . '_ooyala_sync_pull_videos';
    $function($nodes, $videos, $node_map);
  }

  // Save all nodes that have been modified.
  foreach ($nodes as $nid => $node) {
    if (isset($node->pulled_from_ooyala) && $node->pulled_from_ooyala) {
      watchdog('ooyala', 'Updated !title @type to match changes from Ooyala.', array('!title' => l($node->title, 'node/' . $node->nid), '@type' => $node->type), WATCHDOG_NOTICE);
      node_save($node);
    }
  }
}

/**
 * Update ALL the videos on the site from Ooyala.
 *
 * This function sets a batch operation. It may be used within update.php calls
 * or in any submit handler to automatically initiate a batch process.
 *
 * @param $videos_per_batch
 *   The number of videos to process per page load.
 */
function ooyala_sync_pull_videos_batch($videos_per_batch = 50) {
  // Build a batch that migrates the data in each filefield
  $batch = array(
    'title' => t('Updating video data from Ooyala'),
    'operations' => array(
      array('ooyala_sync_pull_videos_batch_process', array($videos_per_batch)),
    ),
    'file' => drupal_get_path('module', 'ooyala') .'/includes/ooyala.sync.inc',
  );
  batch_set($batch);
}

/**
 * Batch API callback to update ALL videos on the site, pulling from Ooyala.
 *
 * @param $videos_per_batch
 *   The number of videos to process per page load.
 * @param $context
 *   The Batch API variable containing information on the videos processed.
 */
function ooyala_sync_pull_videos_batch_process($videos_per_batch, &$context) {
  module_load_include('inc', 'ooyala', 'includes/ooyala.api');

  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['page_number'] = 0;
  }

  // Query Ooyala for a set of videos and process on the Drupal side.
  $details = array(); // Passed by reference.
  $options = array(
    'pageID' => $context['sandbox']['page_number'],
    'limit' => $videos_per_batch,
    'orderBy' => 'uploadedAt,asc',
    'fields' => 'labels',
  );
  $videos = ooyala_api_video_query($options, $details);
  ooyala_sync_pull_videos($videos);

  // Update progress information.
  $context['sandbox']['total'] = $details['total'];
  $context['sandbox']['progress'] += count($videos);
  // TODO: This line should not be necessary. Ooyala always returns the "limit"
  // number of videos, even when there are less or no results.
  $context['sandbox']['progress'] = min($context['sandbox']['progress'], $context['sandbox']['total']);
  $context['sandbox']['page_number']++;
  $context['message'] = t('Proessing Ooyala videos... @count of @total', array('@count' => $context['sandbox']['progress'], '@total' => $context['sandbox']['total']));

  if ($context['sandbox']['progress'] != $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['total'];
  }
}

/**
 * Implementation of hook_ooyala_sync_pull_videos().
 *
 * General syncing function to check if meta-data needs updating in nodes.
 *
 * @param $nodes
 *   An array of all nodes that may need to be updated because they contain
 *   videos that have been updated.
 * @param $videos
 *   An array of all videos that have been updated.
 * @param $node_map
 *   An array containing two separate keys: nids and embed_codes. Each key
 *   contains a list of usages keyed by their corresponding parent key.
 */
function ooyala_ooyala_sync_pull_videos(&$nodes, $videos, $node_map) {
  foreach ($nodes as $nid => &$node) {
    // Check each Ooyala field separately for matching embed codes.
    foreach ($node->ooyala_field_names as $field_name) {
       foreach ($node->{$field_name} as $langcode => $field_values) {
         foreach ($field_values as $delta => $field_value) {
           $embed_code = isset($field_value['value']) ? $field_value['value'] : FALSE;
           if (in_array($embed_code, $node_map['nids'][$nid]) && isset($videos[$embed_code])) {
             $video = $videos[$embed_code];

             // Update the meta data in the node if changed.
             if (empty($field_value['status']) || $field_value['status'] != $video['status']) {
               $node->{$field_name}[$langcode][$delta]['status'] = $video['status'];
               $node->pulled_from_ooyala = TRUE;
             }
             if (empty($field_value['length']) || $field_value['length'] != $video['duration']) {
               $node->{$field_name}[$langcode][$delta]['length'] = $video['duration'];
               $node->pulled_from_ooyala = TRUE;
             }
           }
         }
      }
    }
  }
}

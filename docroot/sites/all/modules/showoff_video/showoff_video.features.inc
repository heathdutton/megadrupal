<?php
/**
 * @file
 * showoff_video.features.inc
 */

/**
 * Implements hook_node_info().
 */
function showoff_video_node_info() {
  $items = array(
    'video' => array(
      'name' => t('Video'),
      'base' => 'node_content',
      'description' => t('Allows a video to be played in selected feeds.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

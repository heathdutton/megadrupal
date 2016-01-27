<?php
/**
 * @file
 * voipnumber_feature.features.inc
 */

/**
 * Implements hook_views_api().
 */
function voipnumber_feature_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function voipnumber_feature_node_info() {
  $items = array(
    'phone_number' => array(
      'name' => t('Phone number'),
      'base' => 'node_content',
      'description' => t('Stores subscribed phone numbers.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

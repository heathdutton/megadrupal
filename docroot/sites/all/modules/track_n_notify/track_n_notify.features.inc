<?php
/**
 * @file
 * track_n_notify.features.inc
 */

/**
 * Implements hook_views_api().
 */
function track_n_notify_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function track_n_notify_node_info() {
  $items = array(
    'track' => array(
      'name' => t('Track'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implements hook_default_rules_link().
 */
function track_n_notify_default_rules_link() {
  $items = array();
  $items['track_button'] = entity_import('rules_link', '{
    "settings" : {
      "text" : "track",
      "link_type" : "javascript",
      "bundles" : [],
      "entity_link" : 1
    },
    "name" : "track_button",
    "label" : "Track Button",
    "path" : "track_button",
    "entity_type" : "node"
  }');
  return $items;
}

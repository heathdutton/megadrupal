<?php
/**
 * @file
 * open_badges.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function open_badges_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function open_badges_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function open_badges_node_info() {
  $items = array(
    'award' => array(
      'name' => t('Award'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'badge' => array(
      'name' => t('Badge'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'issuer' => array(
      'name' => t('Issuer'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Organization name'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

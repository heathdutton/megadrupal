<?php
/**
 * @file
 * showoff_unit.features.inc
 */

/**
 * Implements hook_node_info().
 */
function showoff_unit_node_info() {
  $items = array(
    'unit' => array(
      'name' => t('Unit'),
      'base' => 'node_content',
      'description' => t('A unit is any object that will display a feed (typically a TV or a screen).'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implements hook_views_api().
 */
function showoff_unit_views_api() {
  return array("api" => "3.0");
}

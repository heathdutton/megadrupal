<?php
/**
 * @file
 * logo_doodle.features.inc
 */

/**
 * Implements hook_node_info().
 */
function logo_doodle_node_info() {
  $items = array(
    'logo_doodle' => array(
      'name' => t('Logo Doodle'),
      'base' => 'node_content',
      'description' => t('Allows you to define an alternate logo that will be shown for a specific time period.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

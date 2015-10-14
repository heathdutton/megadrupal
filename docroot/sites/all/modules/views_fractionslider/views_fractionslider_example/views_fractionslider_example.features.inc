<?php
/**
 * @file
 * views_fractionslider_example.features.inc
 */

/**
 * Implements hook_views_api().
 */
function views_fractionslider_example_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function views_fractionslider_example_node_info() {
  $items = array(
    'views_fractionslider' => array(
      'name' => t('Views FractionSlider'),
      'base' => 'node_content',
      'description' => t('Example content type for Views FractionSlider to work with views.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

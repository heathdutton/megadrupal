<?php
/**
 * @file
 * feature_slideshow.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function feature_slideshow_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function feature_slideshow_node_info() {
  $items = array(
    'slide' => array(
      'name' => t('Slide'),
      'base' => 'node_content',
      'description' => t('Use <em>slides</em> to create content to place inside <em>slideshows</em>.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'slideshow' => array(
      'name' => t('Slideshow'),
      'base' => 'node_content',
      'description' => t('Use <em>slideshow</em> to display groups of <em>slides</em> in a slideshow.'),
      'has_title' => '1',
      'title_label' => t('Slideshow Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

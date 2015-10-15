<?php
/**
 * @file
 * nm_testimonials.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function nm_testimonials_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function nm_testimonials_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function nm_testimonials_node_info() {
  $items = array(
    'nm_testimonial' => array(
      'name' => t('Testimonial'),
      'base' => 'node_content',
      'description' => t('User-submitted review of your company/brand.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

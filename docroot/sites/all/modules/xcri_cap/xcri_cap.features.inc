<?php
/**
 * @file
 * xcri_cap.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function xcri_cap_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function xcri_cap_node_info() {
  $items = array(
    'course_presentation' => array(
      'name' => t('Course presentation'),
      'base' => 'node_content',
      'description' => t('Holds the data relating to an instance of the learning opportunity.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

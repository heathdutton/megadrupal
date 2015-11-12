<?php
/**
 * @file
 * iati_feature.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function iati_feature_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "cer" && $api == "default_cer_presets") {
    return array("version" => "1");
  }
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  if ($module == "relation" && $api == "relation_type_default") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function iati_feature_node_info() {
  $items = array(
    'iati_activity' => array(
      'name' => t('IATI Activity'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'iati_organisation' => array(
      'name' => t('IATI Organisation'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Organisation Name'),
      'help' => '',
    ),
  );
  return $items;
}

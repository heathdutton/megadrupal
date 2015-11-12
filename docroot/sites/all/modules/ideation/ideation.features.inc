<?php
/**
 * @file
 * ideation.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ideation_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function ideation_node_info() {
  $items = array(
    'idea_site' => array(
      'name' => t('Idea'),
      'base' => 'node_content',
      'description' => t('A submission of an idea that can be posted, voted on, and commented upon.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

<?php
/**
 * @file
 * user_priority_ranking_example.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function user_priority_ranking_example_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function user_priority_ranking_example_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_flag_default_flags().
 */
function user_priority_ranking_example_flag_default_flags() {
  $flags = array();
  // Exported flag: "Things I want".
  $flags['wants'] = array(
    'content_type' => 'node',
    'title' => 'Things I want',
    'global' => '0',
    'types' => array(
      0 => 'toys_user_priority_ranking',
    ),
    'flag_short' => 'I want this',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'I do not want this anymore',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => '2',
      ),
      'unflag' => array(
        0 => '2',
      ),
    ),
    'show_on_page' => 1,
    'show_on_teaser' => 1,
    'show_on_form' => 0,
    'access_author' => '',
    'i18n' => 0,
    'module' => 'user_priority_ranking_example',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;
}

/**
 * Implements hook_node_info().
 */
function user_priority_ranking_example_node_info() {
  $items = array(
    'toys_user_priority_ranking' => array(
      'name' => t('Toys'),
      'base' => 'node_content',
      'description' => t('Content type to demostrate the User Priority Example module'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  return $items;
}

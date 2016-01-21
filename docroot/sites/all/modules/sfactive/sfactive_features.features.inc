<?php
/**
 * @file
 * sfactive_features.features.inc
 */

/**
 * Implementation of hook_ctools_plugin_api().
 */
function sfactive_features_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implementation of hook_views_api().
 */
function sfactive_features_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implementation of hook_flag_default_flags().
 */
function sfactive_features_flag_default_flags() {
  $flags = array();
  // Exported flag: "Global".
  $flags['global'] = array(
    'content_type' => 'node',
    'title' => 'Global',
    'global' => '1',
    'types' => array(
      0 => 'article',
    ),
    'flag_short' => 'Classify global',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Unclassify global',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => '3',
      ),
      'unflag' => array(
        0 => '3',
      ),
    ),
    'show_on_page' => 1,
    'show_on_teaser' => 1,
    'show_on_form' => 1,
    'access_author' => '',
    'i18n' => 0,
    'module' => 'sfactive_features',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  // Exported flag: "Local".
  $flags['local'] = array(
    'content_type' => 'node',
    'title' => 'Local',
    'global' => '1',
    'types' => array(
      0 => 'article',
    ),
    'flag_short' => 'Classify local',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Unclassify local',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => '3',
      ),
      'unflag' => array(
        0 => '3',
      ),
    ),
    'show_on_page' => 1,
    'show_on_teaser' => 1,
    'show_on_form' => 1,
    'access_author' => '',
    'i18n' => 0,
    'module' => 'sfactive_features',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;
}

/**
 * Implementation of hook_node_info().
 */
function sfactive_features_node_info() {
  $items = array(
    'article' => array(
      'name' => t('Article'),
      'base' => 'node_content',
      'description' => t('Use <em>articles</em> for time-sensitive content like news, press releases or blog posts.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'event' => array(
      'name' => t('Event'),
      'base' => 'node_content',
      'description' => t('Calendar events.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'feature' => array(
      'name' => t('Feature'),
      'base' => 'node_content',
      'description' => t('Feature stories.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'page' => array(
      'name' => t('Basic page'),
      'base' => 'node_content',
      'description' => t('Use <em>basic pages</em> for your static content, such as an \'About us\' page.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

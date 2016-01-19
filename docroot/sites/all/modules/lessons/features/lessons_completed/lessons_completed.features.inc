<?php
/**
 * @file
 * lessons_completed.features.inc
 */

/**
 * Implements hook_views_api().
 */
function lessons_completed_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_flag_default_flags().
 */
function lessons_completed_flag_default_flags() {
  $flags = array();
  // Exported flag: "Mark lessons completed".
  $flags['lessons_complete'] = array(
    'content_type' => 'node',
    'title' => 'Mark lessons completed',
    'global' => '0',
    'types' => array(
      0 => 'lesson',
    ),
    'flag_short' => 'Not completed',
    'flag_long' => 'Not completed',
    'flag_message' => '',
    'unflag_short' => 'Completed',
    'unflag_long' => 'Completed',
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
    'module' => 'lessons_completed',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;
}

<?php
/**
 * @file
 * lessons_bookmarks.features.inc
 */

/**
 * Implements hook_views_api().
 */
function lessons_bookmarks_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_flag_default_flags().
 */
function lessons_bookmarks_flag_default_flags() {
  $flags = array();
  // Exported flag: "Bookmarks lessons and ladders".
  $flags['lessons_bookmark'] = array(
    'content_type' => 'node',
    'title' => 'Bookmarks lessons and ladders',
    'global' => '0',
    'types' => array(
      0 => 'ladder',
      1 => 'lesson',
    ),
    'flag_short' => 'Not bookmarked',
    'flag_long' => 'Not bookmarked',
    'flag_message' => '',
    'unflag_short' => 'Bookmarked',
    'unflag_long' => 'Bookmarked',
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
    'module' => 'lessons_bookmarks',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;
}

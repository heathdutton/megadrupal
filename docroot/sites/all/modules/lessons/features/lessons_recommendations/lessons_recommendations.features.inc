<?php
/**
 * @file
 * lessons_recommendations.features.inc
 */

/**
 * Implements hook_views_api().
 */
function lessons_recommendations_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_flag_default_flags().
 */
function lessons_recommendations_flag_default_flags() {
  $flags = array();
  // Exported flag: "Recommend lessons and ladders".
  $flags['lessons_recommend'] = array(
    'content_type' => 'node',
    'title' => 'Recommend lessons and ladders',
    'global' => '0',
    'types' => array(
      0 => 'lesson',
      1 => 'ladder',
    ),
    'flag_short' => 'Not recommended',
    'flag_long' => 'Not recommended',
    'flag_message' => '',
    'unflag_short' => '',
    'unflag_long' => 'Recommended',
    'unflag_message' => 'Recommended',
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
    'module' => 'lessons_recommendations',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;
}

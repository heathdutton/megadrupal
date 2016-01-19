<?php
/**
 * @file
 * lessons_provisional.features.inc
 */

/**
 * Implements hook_flag_default_flags().
 */
function lessons_provisional_flag_default_flags() {
  $flags = array();
  // Exported flag: "Provisional".
  $flags['lessons_provisional'] = array(
    'content_type' => 'node',
    'title' => 'Provisional',
    'global' => '1',
    'types' => array(
      0 => 'lesson',
    ),
    'flag_short' => 'Provisional',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Stable',
    'unflag_long' => 'Stable',
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
    'weight' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_on_page' => 1,
    'show_on_teaser' => 1,
    'show_contextual_link' => 0,
    'i18n' => 0,
    'module' => 'lessons_provisional',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;
}

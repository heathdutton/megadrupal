<?php
/**
 * @file
 * instagram_feeds_moderation.features.inc
 */

/**
 * Implements hook_views_api().
 */
function instagram_feeds_moderation_views_api() {
  $path = drupal_get_path('module', 'instagram_feeds_moderation');
  return array(
    'version' => '3.0',
    'path' => $path,
    'template path' => $path . '/templates',
  );
}

/**
 * Implements hook_flag_default_flags().
 */
function instagram_feeds_moderation_flag_default_flags() {
  $flags = array();
  // Exported flag: "Moderation images".
  $flags['image_archived'] = array(
    'entity_type' => 'node',
    'title' => 'Moderation images',
    'global' => '1',
    'types' => array(
      0 => 'instagram_media_item',
    ),
    'errors' => array(),
    'flag_short' => 'Archive',
    'flag_long' => 'Approve and flag as appropriate',
    'flag_message' => 'Archived and flagged as appropriate.',
    'unflag_short' => 'Archived',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => 'You can\'t change the image moderation status from FRONTEND',
    'link_type' => 'toggle',
    'weight' => '-6',
    'show_in_links' => array(
      'teaser' => TRUE,
      'full' => TRUE,
    ),
    'show_as_field' => FALSE,
    'show_on_form' => 1,
    'access_author' => '',
    'show_contextual_link' => 1,
    'i18n' => 0,
    'api_version' => 3,
    'module' => 'instagram_feeds_moderation',
    'locked' => array(
      0 => 'name',
    ),
  );
  return $flags;

}

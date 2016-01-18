<?php

/**
 * Implementation of hook_flag_default_flags().
 */
function drupal_planet_flag_default_flags() {
  $flags = array();
  // Exported flag: "Publish to Drupal Planet?".
  $flags['planet_posts'] = array(
    'content_type' => 'node',
    'title' => 'Publish to Drupal Planet?',
    'global' => '1',
    'types' => array(
      '0' => 'article',
    ),
    'flag_short' => 'Publish this post to Drupal Planet',
    'flag_long' => '',
    'flag_message' => 'This post has been published to Drupal Planet',
    'unflag_short' => 'Unpublish this post from Drupal Planet',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => 'This post has already been published to Drupal Planet. It cannot be undone.',
    'link_type' => 'normal',
    'roles' => array(
      'flag' => array(
        '0' => '3',
      ),
      'unflag' => array(),
    ),
    'show_on_page' => 0,
    'show_on_teaser' => 0,
    'show_on_form' => 1,
    'access_author' => '',
    'i18n' => 0,
    'api_version' => 2,
    'module' => 'drupal_planet',
    'locked' => array(
      '0' => 'name',
    ),
  );
  return $flags;
}

/**
 * Implementation of hook_views_api().
 */
function drupal_planet_views_api() {
  return array(
    'api' => '3.0-alpha1',
  );
}

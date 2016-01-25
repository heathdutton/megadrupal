<?php

/**
 * @file
 * Instagram social feed translatable variables.
 */

/**
 * Implements hook_variable_info().
 */
function instagram_social_feed_variable_info() {
  $variables['instagram_social_feed_more_uri'] = array(
    'type' => 'string',
    'title' => t('Instagram social feed - More link URI'),
    'description' => t('URI of the general link to display in the display below the photos.'),
    'default' => '',
  );
  $variables['instagram_social_feed_more_text'] = array(
    'type' => 'string',
    'title' => t('Instagram social feed - More link Text'),
    'description' => t('Text of the general link to display in the display below the photos.'),
    'default' => '',
  );

  return $variables;
}

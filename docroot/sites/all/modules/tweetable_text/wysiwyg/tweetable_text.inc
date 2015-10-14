<?php

/**
 * Implements hook_INCLUDE_plugin().
 */
function tweetable_text_tweetable_text_plugin() {
  $plugins = array();
  $plugins['tweetable_text'] = array(
    'title' => t('Tweetable text'),
    'vendor url' => 'http://drupal.org/project/tweetable_text',
    'icon file' => 'tweet.gif',
    'icon title' => t('Mark specific phrases or sentences for one-click tweeting.'),
    'settings' => array(),
  );
  return $plugins;
}

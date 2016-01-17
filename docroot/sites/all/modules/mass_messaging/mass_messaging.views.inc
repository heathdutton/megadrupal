<?php

/**
 * @file
 * Implementaion of the views hooks.
 */

/**
 * Implementation of hook_views_plugins().
 */
function mass_messaging_views_plugins() {
  $path = drupal_get_path('module', 'mass_messaging');
  $theme_path = $path . '/theme';
  return array(
    'module' => 'mass_messaging',
    'display' => array(
      'message' => array(
        'title' => t('Message List'),
        'help' => t('Outputs the view as a messages list. Use this for mass messaging.'),
        'handler' => 'mass_messaging_plugin_display',
        'uses hook menu' => FALSE,
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'use_more' => FALSE,
        'accept attachments' => FALSE,
        'admin' => t('Mail'),
      ),
    ),
    'style' => array(
      'message_style' => array(
        'title' => t('Message List Style'),
        'help' => t('Generates a list of messages.'),
        'handler' => 'mass_messaging_plugin_style',
        'parent' => 'default',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'mail',
      ),
    ),

  );
}
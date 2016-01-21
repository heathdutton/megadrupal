<?php
/**
 * @file
 * Code for Theme functions for OpenAtrium Contextual Help widgets
 */

/**
 * Implements hook_theme()
 */
function atrium_contextual_help_theme() {
  return array(
    'atrium_contextual_help_button' => array(
      'template'  => 'atrium-contextual-help-button',
      'arguments' => array(
        'public' => NULL,
        'links'  => array(),
      ),
      'path' => drupal_get_path('module', 'atrium_contextual_help') . '/templates',
    ),
  );
}

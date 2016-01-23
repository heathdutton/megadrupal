<?php

/**
 * @file
 *
 * Implement Wysiwyg module hooks.
 */

/**
 * Implements hook_wysiwyg_plugin().
 */
function insert_popup_inline_image_button_plugin() {
  $plugins['inline_image_button'] = array(
    'title' => t('Inline image insert popup'),
    'vendor url' => 'http://drupal.org/project/insert_popup', // @todo
    'icon title' => t('Inline image insert popup'),
    'settings' => array(),
  );

  return $plugins;
}

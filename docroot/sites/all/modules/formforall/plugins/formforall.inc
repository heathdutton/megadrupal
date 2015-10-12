<?php

/**
 * @file
 * Wysiwyg API integration.
 */

/**
 * Implements hook_wysiwyg_plugin().
 */
function formforall_formforall_plugin() {
  $plugins['formforall'] = array(
    'title' => t('Insert FormForAll'),
    'vendor url' => 'http://drupal.org/project/formforall',
    'icon file' => 'formforall.png',
    'icon title' => t('Insert a FormForAll form'),
    'settings' => array(),
  );
  return $plugins;
}

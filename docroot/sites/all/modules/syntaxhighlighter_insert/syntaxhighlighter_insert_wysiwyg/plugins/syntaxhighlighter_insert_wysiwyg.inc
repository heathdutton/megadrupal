<?php

/**
 * @file
 * Wysiwyg API integration.
 */

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function syntaxhighlighter_insert_wysiwyg_syntaxhighlighter_insert_wysiwyg_plugin() {
  $plugins['syntaxhighlighter_insert_wysiwyg'] = array(
    'title' => t('Insert syntaxhighlighter tag'),
    'vendor url' => 'http://drupal.org/project/syntaxhighlighter_insert',
    'icon file' => 'insert.gif',
    'icon title' => t('Insert syntaxhighlighter tag'),
    'settings' => array(),
  );
  return $plugins;
}


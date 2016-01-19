<?php

/**
 * @file
 * Provides Media: WYSIWYG integration for CKEditor.
 */

/**
 * Implements hook_ckeditor_plugin_alter().
 */
function media_ckeditor_widget_ckeditor_plugin_alter(&$plugins) {
  // Override the default CKEditor Media plugin.
  $plugins['media_widget'] = array(
    'name' => 'media',
    'desc' => t('Plugin for embedding files using Media CKEditor Widget'),
    'path' => '%base_path%' . drupal_get_path('module', 'media_ckeditor_widget') . '/js/plugins/media/',
    'buttons' => array(
      'Media' => array(
        'icon' => 'images/icon.png',
        'label' => 'Add media',
      ),
    ),
    'default' => 't',
  );
}


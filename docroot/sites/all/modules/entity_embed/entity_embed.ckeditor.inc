<?php

/**
 * @file
 * CKEditor integration for the entity_embed module.
 */

/**
 * Implements hook_ckeditor_plugin().
 */
function entity_embed_ckeditor_plugin() {
  $plugins = array(
    'drupalentity' => array(
      'name' => 'drupalentity',
      'desc' => t('Plugin for embedding entities'),
      'path' => base_path() . drupal_get_path('module', 'entity_embed') . '/js/plugins/drupalentity/',
      'default' => 'f',
    ),
  );

  // Add user-defined buttons.
  $embed_buttons = entity_embed_embed_button_load_all();

  $buttons = array();

  foreach ($embed_buttons as $embed_button) {
    // CKEditor only supports icons relative to the plugin path.
    // @todo: Figure out how to support custom icons.
    $buttons[$embed_button->name] = array(
      'label' => check_plain($embed_button->button_label),
      'icon' => 'entity.png',
    );
  }

  $plugins['drupalentity']['buttons'] = $buttons;

  return $plugins;
}

/**
 * Implements hook_ckeditor_settings_alter().
 */
function entity_embed_ckeditor_settings_alter(&$settings, $conf) {
  // Add theme-independent styles for aligning content.
  $settings['contentsCss'][] = base_path() . drupal_get_path('module', 'entity_embed') . '/css/entity_embed.css';

  // Add base styles for accessibility, clearing, etc.
  $settings['contentsCss'][] = base_path() . drupal_get_path('module', 'system') . '/system.base.css';

  // Add styles for handling contextual links.
  if (module_exists('contextual')) {
    $settings['contentsCss'][] = base_path() . drupal_get_path('module', 'contextual') . '/contextual.css';
  }
}

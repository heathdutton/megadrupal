<?php

/**
 * Implements hook_form_system_theme_settings_alter().
 */
function maps_admin_form_system_theme_settings_alter(&$form, $form_state) {
  $values = variable_get('theme_maps_admin_settings', array());
  
  $form['fonts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fonts'),
  );
  
  $form['fonts']['font_source'] = array(
    '#type' => 'select',
    '#title' => t('Font source'),
    '#options' => array(
      'local' => t('Local'),
      'remote' => t('Remote'),
    ),
    '#default_value' => theme_get_setting('font_source', 'maps_admin'),
    '#description' => 'Define if we have to load theme fonts locally or remotely',
  );
}

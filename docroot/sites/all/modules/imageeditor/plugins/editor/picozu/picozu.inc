<?php
/**
 * @file
 * Picozu editor for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('Picozu'),
  'description' => t('Advanced HTML5 editor'),
  'class' => 'picozu',
  'site' => 'https://www.picozu.com',
  'api_key' => TRUE,
  'api_key_codename' => 'imageeditor_picozu_api_key',
  'image_creation' => TRUE,
  'launch_type' => 'overlay',
  'image_url_param' => 'i',
  'options' => array(
    'launch_url' => 'https://www.picozu.com/editor/',
    'key' => variable_get('imageeditor_picozu_api_key', ''),
    'theme' => variable_get('imageeditor_picozu_theme', 'default'),
    'workspace' => variable_get('imageeditor_picozu_workspace', 1),
  ),
  'parameters' => array(
    'saveurl' => url(imageeditor_ajax_save_path() . '/picozu', array('absolute' => TRUE)),
  ),
  //'initialize_callback' => 'imageeditor_picozu_initialize_callback',
  //'save_callback' => 'imageeditor_picozu_save_callback',
  'settings_form_callback' => 'imageeditor_picozu_settings_form_callback',
  'js' => 'picozu.js',
  'css' => 'picozu.css',
);

function imageeditor_picozu_settings_form_callback() {
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('Picozu'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['imageeditor_picozu_theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#description' => t('Choose color theme for Picozu editor.'),
    '#options' => array(
      'default' => 'Default',
      'galaxy' => 'Galaxy',
      'kids' => 'Kids',
    ),
    '#default_value' => variable_get('imageeditor_picozu_theme', 'default'),
    '#weight' => 5,
  );
  $form['imageeditor_picozu_workspace'] = array(
    '#type' => 'select',
    '#title' => t('Workspace'),
    '#description' => t('Choose workspace for Picozu editor.'),
    '#options' => array(
      0 => 'Minimal',
      1 => 'Default',
      2 => 'Educational',
      3 => 'Kids',
    ),
    '#default_value' => variable_get('imageeditor_picozu_workspace', 1),
    '#weight' => 10,
  );
  $link = 'https://www.picozu.com/developers/applications/';
  $form['imageeditor_picozu_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#description' => l($link, $link, array('attributes' => array('target' => '_blank'))),
    '#default_value' => variable_get('imageeditor_picozu_api_key', ''),
    '#size' => 50,
    '#maxlength' => 100,
    '#required' => FALSE,
    '#weight' => 15,
  );

  return $form;
}

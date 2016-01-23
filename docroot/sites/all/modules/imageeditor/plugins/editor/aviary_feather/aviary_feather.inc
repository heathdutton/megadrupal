<?php
/**
 * @file
 * Aviary Feather editor for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('Aviary Feather'),
  'description' => t('Simple photo editor - only edit images, no create new image option'),
  'class' => 'aviary-feather',
  'site' => 'http://www.aviary.com/',
  'api_key' => TRUE,
  'api_key_codename' => 'imageeditor_aviary_api_key',
  'image_creation' => FALSE,
  'options' => array(
    'apiKey' => variable_get('imageeditor_aviary_api_key', ''),
    'apiVersion' => 3,
    'theme' => variable_get('imageeditor_aviary_feather_theme', 'dark'),
    //'noCloseButton' => 'true',
    'launchDelay' => variable_get('imageeditor_aviary_feather_launch_delay', 1000),
    'closeDelay' => variable_get('imageeditor_aviary_feather_close_delay', 1000),
    'tools' => variable_get('imageeditor_aviary_feather_tools', 'all'),
    'displayImageSize' => TRUE,
  ),
  'parameters' => array(
    'closeonsave' => variable_get('imageeditor_aviary_feather_closeonsave', 1),
  ),
  'initialize_callback' => 'imageeditor_aviary_feather_initialize_callback',
  //'save_callback' => 'imageeditor_aviary_feather_save_callback',
  'settings_form_callback' => 'imageeditor_aviary_feather_settings_form_callback',
  'js' => 'aviary_feather.js',
  'css' => 'aviary_feather.css',
);

function imageeditor_aviary_feather_initialize_callback(&$editor) {
  global $user, $language, $is_https;
  if ($is_https) {
    drupal_add_js('https://dme0ih8comzn4.cloudfront.net/js/feather.js', 'external');
  }
  else {
    drupal_add_js('http://feather.aviary.com/js/feather.js', 'external');
  }
  $editor['options']['language'] = property_exists($user, 'language') ? $user->language : $language->language;
  // Some magic for highres images - doesn't work.
  /*if (variable_get('imageeditor_aviary_feather_secret', '')) {
    $signature = md5(variable_get('imageeditor_aviary_api_key', '') . variable_get('imageeditor_aviary_feather_secret', '') . REQUEST_TIME);
    $editor['options']['timestamp'] = REQUEST_TIME;
    $editor['options']['signature'] = $signature;
  }*/
}

function imageeditor_aviary_feather_save_callback() {
}

function imageeditor_aviary_feather_settings_form_callback() {
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('Aviary Feather'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['imageeditor_aviary_feather_theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#description' => t('Choose color theme for Aviary Feather editor.'),
    '#options' => array(
      'dark' => 'dark',
      'light' => 'light',
      'minimum' => 'minimum',
    ),
    '#default_value' => variable_get('imageeditor_aviary_feather_theme', 'dark'),
    '#weight' => 5,
  );
  $form['imageeditor_aviary_feather_launch_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Launch delay'),
    '#description' => t('Set the launch fade-in animation time in milliseconds.'),
    '#default_value' => variable_get('imageeditor_aviary_feather_launch_delay', 1000),
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => TRUE,
    '#weight' => 10,
  );
  $form['imageeditor_aviary_feather_close_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Close delay'),
    '#description' => t('Set the close fade-out animation time in milliseconds.'),
    '#default_value' => variable_get('imageeditor_aviary_feather_close_delay', 1000),
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => TRUE,
    '#weight' => 11,
  );
  $form['imageeditor_aviary_feather_tools'] = array(
    '#type' => 'textfield',
    '#title' => t('Tools'),
    '#description' => t('Specify comma-separated string of tools. See !link.',
      array('!link' => l(t('Aviary documentation'), 'http://developers.aviary.com/',
        array('external' => TRUE, 'fragment' => 'constructor-config-tools', 'attributes' => array('target' => '_blank'))))),
    '#default_value' => variable_get('imageeditor_aviary_feather_tools', 'all'),
    '#size' => 100,
    '#maxlength' => 150,
    '#required' => TRUE,
    '#weight' => 15,
  );
  $form['imageeditor_aviary_feather_closeonsave'] = array(
    '#type' => 'checkbox',
    '#title' => t('Close on save.'),
    '#description' => t('Close Aviary Feather editor after clicking the Save button.'),
    '#default_value' => variable_get('imageeditor_aviary_feather_closeonsave', 1),
    '#weight' => 20,
  );
  $link = 'http://developers.aviary.com/getfeatherkey';
  $form['imageeditor_aviary_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#description' => l($link, $link, array('attributes' => array('target' => '_blank'))),
    '#default_value' => variable_get('imageeditor_aviary_api_key', ''),
    '#size' => 50,
    '#maxlength' => 100,
    '#required' => FALSE,
    '#weight' => 25,
  );
  $form['imageeditor_aviary_feather_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret API key'),
    '#description' => t('Secret API key is needed to edit high resolution images.'),
    '#default_value' => variable_get('imageeditor_aviary_feather_secret', ''),
    '#size' => 50,
    '#maxlength' => 100,
    '#required' => FALSE,
    '#weight' => 30,
  );

  return $form;
}

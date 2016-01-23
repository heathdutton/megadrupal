<?php
/**
 * @file
 * Adobe Creative SDK Image Editing Component editor for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('Adobe Creative SDK'),
  'description' => t('Simple photo editor - only edit images, no create new image option'),
  'class' => 'adobe-creative-sdk',
  'site' => 'https://creativesdk.adobe.com/docs/web',
  'api_key' => TRUE,
  'api_key_codename' => 'imageeditor_adobe_creative_sdk_api_key',
  'image_creation' => FALSE,
  'options' => array(
    'apiKey' => variable_get('imageeditor_adobe_creative_sdk_api_key', ''),
    'theme' => variable_get('imageeditor_adobe_creative_sdk_theme', 'dark'),
    //'noCloseButton' => 'true',
    'launchDelay' => variable_get('imageeditor_adobe_creative_sdk_launch_delay', 1000),
    'closeDelay' => variable_get('imageeditor_adobe_creative_sdk_close_delay', 1000),
    'tools' => variable_get('imageeditor_adobe_creative_sdk_tools', 'all'),
    'displayImageSize' => TRUE,
  ),
  'parameters' => array(
    'closeonsave' => variable_get('imageeditor_adobe_creative_sdk_closeonsave', 1),
  ),
  'initialize_callback' => 'imageeditor_adobe_creative_sdk_initialize_callback',
  //'save_callback' => 'imageeditor_adobe_creative_sdk_save_callback',
  'settings_form_callback' => 'imageeditor_adobe_creative_sdk_settings_form_callback',
  'js' => 'adobe_creative_sdk.js',
  'css' => 'adobe_creative_sdk.css',
);

function imageeditor_adobe_creative_sdk_initialize_callback(&$editor) {
  global $user, $language, $is_https;
  if ($is_https) {
    drupal_add_js('https://dme0ih8comzn4.cloudfront.net/imaging/v1/editor.js', 'external');
  }
  else {
    drupal_add_js('http://feather.aviary.com/imaging/v1/editor.js', 'external');
  }
  $editor['options']['language'] = property_exists($user, 'language') ? $user->language : $language->language;
  // Some magic for highres images - doesn't work.
  /*if (variable_get('imageeditor_adobe_creative_sdk_secret', '')) {
    $signature = md5(variable_get('imageeditor_adobe_creative_sdk_api_key', '') . variable_get('imageeditor_adobe_creative_sdk_secret', '') . REQUEST_TIME);
    $editor['options']['timestamp'] = REQUEST_TIME;
    $editor['options']['signature'] = $signature;
  }*/
}

function imageeditor_adobe_creative_sdk_save_callback() {
}

function imageeditor_adobe_creative_sdk_settings_form_callback() {
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('Adobe Creative SDK'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['imageeditor_adobe_creative_sdk_theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#description' => t('Choose color theme for Adobe Creative SDK editor.'),
    '#options' => array(
      'dark' => 'dark',
      'light' => 'light',
      'minimum' => 'minimum',
    ),
    '#default_value' => variable_get('imageeditor_adobe_creative_sdk_theme', 'dark'),
    '#weight' => 5,
  );
  $form['imageeditor_adobe_creative_sdk_launch_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Launch delay'),
    '#description' => t('Set the launch fade-in animation time in milliseconds.'),
    '#default_value' => variable_get('imageeditor_adobe_creative_sdk_launch_delay', 1000),
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => TRUE,
    '#weight' => 10,
  );
  $form['imageeditor_adobe_creative_sdk_close_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Close delay'),
    '#description' => t('Set the close fade-out animation time in milliseconds.'),
    '#default_value' => variable_get('imageeditor_adobe_creative_sdk_close_delay', 1000),
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => TRUE,
    '#weight' => 11,
  );
  $form['imageeditor_adobe_creative_sdk_tools'] = array(
    '#type' => 'textfield',
    '#title' => t('Tools'),
    '#description' => t('Specify comma-separated string of tools. See !link.',
      array('!link' => l(t('Adobe documentation'), 'https://creativesdk.adobe.com/docs/web/',
        array('external' => TRUE, 'fragment' => '/articles/gettingstarted/index.html', 'attributes' => array('target' => '_blank'))))),
    '#default_value' => variable_get('imageeditor_adobe_creative_sdk_tools', 'all'),
    '#size' => 100,
    '#maxlength' => 150,
    '#required' => TRUE,
    '#weight' => 15,
  );
  $form['imageeditor_adobe_creative_sdk_closeonsave'] = array(
    '#type' => 'checkbox',
    '#title' => t('Close on save.'),
    '#description' => t('Close Adobe Creative SDK editor after clicking the Save button.'),
    '#default_value' => variable_get('imageeditor_adobe_creative_sdk_closeonsave', 1),
    '#weight' => 20,
  );
  $link = 'https://creativesdk.adobe.com/myapps.html';
  $form['imageeditor_adobe_creative_sdk_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#description' => l($link, $link, array('attributes' => array('target' => '_blank'))),
    '#default_value' => variable_get('imageeditor_adobe_creative_sdk_api_key', ''),
    '#size' => 50,
    '#maxlength' => 100,
    '#required' => FALSE,
    '#weight' => 25,
  );
  $form['imageeditor_adobe_creative_sdk_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Client secret'),
    '#description' => t('Client secret is needed to edit high resolution images.'),
    '#default_value' => variable_get('imageeditor_adobe_creative_sdk_secret', ''),
    '#size' => 50,
    '#maxlength' => 100,
    '#required' => FALSE,
    '#weight' => 30,
  );

  return $form;
}

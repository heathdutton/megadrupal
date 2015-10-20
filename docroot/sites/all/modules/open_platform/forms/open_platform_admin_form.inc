<?php

/**
 * @file
 * Admin settings for The Open Platfrom module.
 */

/**
 * Implements hook_form().
 */
function open_platform_settings_form() {

  $form = array();
  $default = array();
  if(variable_get('open_platform_api_key') == '') {
      drupal_set_message(t('You must add an API key in the API key settings page before you can use this form'), 'warning');
      return system_settings_form($form);
  }


  $field_options = array(
    'trailText' => t('Trail text'),
    'thumbnail' => t('Thumbnail'),
  );

  $form['open_platform_sections'] = array(
    '#type' => 'select',
    '#title' => t('Sections'),
    '#default_value' => variable_get('open_platform_sections'),
    '#options' => array(
      open_platform_get_end_points('sections'),
    ),
  );

  $form['open_platform_search'] = array(
    '#type' => 'textfield',
    '#title' => t('Search'),
    '#default_value' => variable_get('open_platform_search'),
    '#description' => t('Enter enter keyword(s)'),
    '#size' => 60,
    '#maxlength' => 128,
  );

  $form['open_platform_fields'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Block fields'),
    '#default_value' => variable_get('open_platform_fields', $default),
    '#description' => t('Select what fields you would like to display in your blocks'),
    '#options' => $field_options,
  );


  $form['open_platform_advance_fields'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content Fields'),
    '#default_value' => variable_get('open_platform_advance_fields', $default),
    '#description' => t('Select additional fields for content'),
    '#options' => array(
      'body' => t('Body field'),
      'wordcount' => t('Word count'),
    ),
  );

  $form['advanced'] = array(
        '#type' => 'fieldset',
        '#title' => t('Advanced settings'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#description' => t('Add your API key and select extra options here'),
  );

  $form['advanced']['open_platform_block_cache'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enable block cache'),
    '#default_value' => variable_get('open_platform_block_cache', $default),
    '#description' => t('The number of queries you can make with an API key is 
        limited depending on your subscription. Displaying the block that
        this module provides means that a web service call is made to the 
        Guardian Open Platform on every page request. If your site is very busy
        you may want to consider turning this option on. This will also help to
        improve performance on pages displaying the Open Platform block.'),
    '#options' => array(
      'cache' => t('Turn on block content cache'),
    ),
  );

  return system_settings_form($form);
}


function open_platform_settings_apikey_form() {

    $form['open_platform_api_key'] = array(
        '#type' => 'textfield',
        '#title' => t('API Key'),
        '#default_value' => variable_get('open_platform_api_key'),
        '#description' => 'In order to recieve full content you will need to add your API key',
    );

    return system_settings_form($form);

}

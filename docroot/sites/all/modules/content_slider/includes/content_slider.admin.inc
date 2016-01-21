<?php

/**
 * @file
 * Content administration and module settings UI.
 */

function content_slider_admin_settings() {

  // Defines form elements for sliders content type
  $form['content_slider_source'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slider content source (based on Drupal content-type)'),
    '#description' => t('Enter the machine name of the content types you wish to use for each slider. Only one per slider, ex: article, page, image, etc.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['content_slider_source']['content_slider_source_0'] = array(
    '#type' => 'textfield',
    '#title' => t('Content type for Slider 1'),
    '#default_value' => variable_get('content_slider_source_0', 'page'),
  );

  $form['content_slider_source']['content_slider_source_1'] = array(
    '#type' => 'textfield',
    '#title' => t('Content type for Slider 2'),
    '#default_value' => variable_get('content_slider_source_1', 'article'),
  );

  $form['content_slider_source']['content_slider_source_2'] = array(
    '#type' => 'textfield',
    '#title' => t('Content type for Slider 3'),
    '#default_value' => variable_get('content_slider_source_2', 'content_slider'),
  );

  // General settings
  $form['content_slider_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['content_slider_settings']['content_slider_auto'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto rotate contents'),
    '#default_value' => variable_get('content_slider_auto', 1),
    '#description' => t("Should the rotation be automatic?"),
  );

  $form['content_slider_settings']['content_slider_speed'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#title' => t('Glide animation duration (in milliseconds)'), // TODO
    '#default_value' => variable_get('content_slider_speed', 1000),
  );
  
   return system_settings_form($form);
}

<?php

/**
 * @file
 * Administrative page for Transcript module.
 */

/**
 * Callback function for administration menu item.
 */
function transcript_admin_settings($form, &$form_state) {
  // General Options.
  $form['transcript_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Transcript Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['transcript_options']['transcript_video_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Youtube video id'),
    '#default_value' => variable_get('transcript_video_id', TRANSCRIPT_DEFAULT_VIDEO),
    '#size' => 20,
    '#maxlength' => 50,
    '#description' => t('Please provide your youtube video id alone (Not url)'),
    '#required' => TRUE,
  );

  $form['transcript_options']['transcript_lang_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Youtube language code'),
    '#default_value' => variable_get('transcript_lang_code', TRANSCRIPT_DEFAULT_LANG),
    '#size' => 20,
    '#maxlength' => 10,
    '#description' => t('Please provide your transcript language code. Ex: en or ta or fr-CA (You can find the language code from the Youtube site url)'),
    '#required' => TRUE,
  );

  $form['transcript_options']['transcript_video_auto_play'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable YouTube Video auto play'),
    '#return_value' => 1,
    '#default_value' => variable_get('transcript_video_auto_play', FALSE),
  );

  $form['transcript_options']['transcript_iframe_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width of Iframe'),
    '#default_value' => variable_get('transcript_iframe_width', '400'),
    '#size' => 20,
    '#maxlength' => 10,
    '#description' => t('Please provide Iframe width'),
  );

  $form['transcript_options']['transcript_iframe_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height of Iframe'),
    '#default_value' => variable_get('transcript_iframe_height', '350'),
    '#size' => 20,
    '#maxlength' => 10,
    '#description' => t('Please provide Iframe height'),
  );

  return system_settings_form($form);
}

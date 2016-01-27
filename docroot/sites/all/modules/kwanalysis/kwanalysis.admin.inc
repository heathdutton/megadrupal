<?php

/**
 * @file
 * Admin include file.
 */

/**
 * Displays the form for the standard settings tab.
 *
 * @return
 *   array A structured array for use with Forms API.
 */
function kwanalysis_admin_settings($form, &$form_state) {
  //Define the titles

  $form['seterms_yahoo_appid'] = array(
    '#type' => 'textfield',
    '#title' => t('Yahoo! Term Extractor Application ID'),
    '#description' => t('You will need a Application ID for Yahoo!s Term Extractor api. <a href="http://developer.yahoo.com/wsregapp/">Get your appid here</a>.</a>'),
    '#default_value' => variable_get('seterms_yahoo_appid', ''),
  );

  return system_settings_form($form);
}


<?php

/**
 * @file
 * Provide administration of PACE module.
 */

function savvior_admin_settings_form() {
  $form = array();

  $form['savvior_use_min_js'] = array(
    '#title' => t('Javascript file to use'),
    '#description' => t('Please choose whether you want the minified or the full version of the JS file.'),
    '#type' => 'radios',
    '#options' => array(
      '1' => 'Minified',
      '0' => 'Full (debug)',
    ),
    '#default_value' => variable_get('savvior_use_min_js',array()),
  );


  $form['savvior_load_default_js'] = array(
    '#title' => t('Load default JS'),
    '#description' => t('Savvior is easily configurable via JS and you should write your own in your theme.<br>If some reason you want to use the default then enable this setting.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('savvior_load_default_js',array()),
  );
  $form['savvior_load_default_css'] = array(
    '#title' => t('Load default CSS'),
    '#description' => t('Savvior comes with a small CSS configuration. Ideally you should write your own in your theme.<br>If some reason you want to use the default then enable this setting.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('savvior_load_default_css',array()),
  );

  return system_settings_form($form);
}

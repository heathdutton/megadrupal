<?php

/**
 * @file
 * Provide administration of PACE module.
 */

function salvattore_admin_settings_form() {
  $form = array();

  $form['salvattore_use_min_js'] = array(
    '#title' => t('Javascript file to use'),
    '#description' => t('Please choose whether you want the minified or the full version of the JS file.'),
    '#type' => 'radios',
    '#options' => array(
      '1' => 'Minified',
      '0' => 'Full (debug)',
    ),
    '#default_value' => variable_get('salvattore_use_min_js',array()),
  );


  $form['salvattore_load_default_css'] = array(
    '#title' => t('Load default CSS'),
    '#description' => t('Salvattore is easily configurable via CSS and you should write your own in your theme.<br>If some reason you want to use the default then enable this setting.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('salvattore_load_default_css',array()),
  );

  return system_settings_form($form);
}

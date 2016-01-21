<?php

/**
 * @file
 * Admin page callbacks for the simple_cache module.
 */

/**
 * Form builder; Configure simple_cache module settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function simple_cache_module_settings() {
  $form['modules'] = array(
    '#type' => 'fieldset',
    '#title' => t('Modules'),
    '#collapsible' => TRUE,
  );
  $check_list = array();
  if ($cached = cache_get('bootstrap_modules', 'cache_bootstrap')) {
    $bootstrap_list = $cached->data;
    foreach ($bootstrap_list as $key => $value){
      if($key != "simple_cache"){
        $check_list[$key] = $key;
      }
    }
  }
  $form['modules']['simple_cache_modules_list'] = array(
    '#title' => t('Disable modules'),
    '#description' => t('check to not load these modules while serving cached pages.'),
    '#type' => 'checkboxes',
    '#options' => $check_list,
    '#default_value' => variable_get('simple_cache_modules_list', array()),
  );
  return system_settings_form($form);
}

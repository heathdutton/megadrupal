<?php

/**
 * @file
 * Contains (most of) the form-related functions.
 */

/**
 * Actually alters the system_performance_settings form.
 *
 * Used to display the module settings under the standard performance settings.
 */
function _closure_compiler_form_system_performance_settings_alter(&$form) {
  module_load_include('inc', 'closure_compiler', 'closure_compiler.checks');
  module_load_include('inc', 'closure_compiler', 'closure_compiler.defines');
  module_load_include('inc', 'closure_compiler', 'closure_compiler.functions');

  $services = closure_compiler_get_services(TRUE);
  if (empty($services)) {
    drupal_set_message(t('No services are available, please enable a service sub-module for JS optimization to occur'), 'warning');
  }

  $form['bandwidth_optimization']['closure_compiler_js_optimization'] = array(
    '#type' => 'checkbox',
    '#title' => t("Optimize JavaScript files, using Google's Closure Compiler or another service."),
    '#default_value' => variable_get('closure_compiler_js_optimization', 0) && !empty($services),
    '#disabled' => empty($services),
  );

  $form['bandwidth_optimization']['closure_compiler'] = array(
    '#type' => 'fieldset',
    '#title' => t('JavaScript Optimization Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  if (!empty($services)) {
    $form['bandwidth_optimization']['closure_compiler']['closure_compiler_service'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Optimization service(s) to use'),
      '#options' => $services,
      '#default_value' => variable_get('closure_compiler_service', array()),
      '#description' => t('Select at least one of the services to use. If more than one service is selected, they will be tried in turn and the smallest result will be used.'),
    );
  }

  $form['bandwidth_optimization']['closure_compiler']['closure_compiler_process_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Process Limit'),
    '#default_value' => (int) variable_get('closure_compiler_process_limit', 5),
    '#description' => t('Limit the number of javascript files that will be processed during a single cron run. 0 is unlimited.'),
  );

  foreach ($services as $module => $friendly) {
    if (module_load_include('inc', $module, $module . '.forms')) {
      $function = '_' . $module . '_form_system_performance_settings_alter';
      if (function_exists($function)) {
        $form['bandwidth_optimization']['closure_compiler'][$module] = array(
          '#type' => 'fieldset',
          '#title' => t('Settings for @service', array('@service' => $friendly)),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
        );
        $form['bandwidth_optimization']['closure_compiler'][$module] += call_user_func($function);
      }
    }
  }

  // Warn the user if other JS aggregation modules are enabled.
  $js_modules = array('javascript_aggregator');
  foreach ($js_modules as $module) {
    if (module_exists($module)) {
      drupal_set_message(t('Warning: Javascript module @module is enabled. This might cause problems with the Closure Compiler module.', array('@module' => $module)), 'warning');
    }
  }

  $form['#submit'][] = 'closure_compiler_form_system_performance_settings_submit';

  drupal_add_js(drupal_get_path('module', 'closure_compiler') . '/system_performance_settings.min.js', 'file');
}

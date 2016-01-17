<?php

/**
 * @file
 * form_clerk UI functions.
 *
 * A user with the 'use form_clerk' permission can:
 *   1. Inspect information about a form
 *   2. Input form data from a file.
 *
 * The UI presents forms to enable the user to do those tasks.
 *
 */

define("FORM_CLERK_PATH", drupal_get_path('module', 'form_clerk'));
require_once(FORM_CLERK_PATH . '/form_clerk_curl.inc');
require_once(FORM_CLERK_PATH . '/form_clerk_process.inc');
require_once(FORM_CLERK_PATH . '/form_clerk_ui_fields.inc');
require_once(FORM_CLERK_PATH . '/form_clerk_custom.inc');

/**
 * Main form_clerk configuration form builder.
 *
 * @param $form
 *  The multiform form argument.
 * @param $form_state
 *  The multiform state argument.
 * @param $tab
 *  The 'tab' path argument set in form_clerk_menu().
 */
function form_clerk_form($form, &$form_state, $tab) {

  // If $form_state is not set, then we in startup. Initialize.
  if (!isset($form_state['values'])) {
    $form_state['storage'] = array();
  }

  // Set the form title.
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('Form Clerk')
  );

  if (isset($form_state['storage']['op'])) {
    if ($form_state['storage']['op'] == 'Get Form Info') {
      if (isset($form_state['storage']['form_list'])) {
        form_clerk_ui_build_form_field($form, $form_state, 'result_fieldset');
        form_clerk_ui_build_form_field($form, $form_state, 'form_list');
      }
    }
    elseif ($form_state['storage']['op'] == 'Save Configuration') {
    }
    elseif ($form_state['storage']['op'] == 'Input Form Data') {
      $report_variables = array(
                                'report_dir',
                                'cookie_dir',
                                'proxy',
                                'verbose',
                                );
      foreach ($report_variables as $report_variable) {
        $value = $form_state['storage'][$report_variable];
        $line = "FC_$report_variable = $value\n";
        $form_state['storage']['report'] .= $line;
      }

      // Finish the report and add it to the form.
      $time_result = timer_stop('Form Clerk');
      $elapsed = $time_result['time'] / 1000;
      $form_state['storage']['report'] .= sprintf("Elapsed time: %.2f seconds\n", $elapsed);
      form_clerk_ui_build_form_field($form, $form_state, 'result_fieldset');
      if ($form_state['storage']['info_only'] == TRUE) {
        form_clerk_ui_build_form_field($form, $form_state, 'form_list');
      }
      form_clerk_ui_build_form_field($form, $form_state, 'report');
    }
  }

  // Add the form fields to the form.
  if ($tab == 'get_info') {
    form_clerk_ui_build_form_field($form, $form_state, 'info_fieldset');
    form_clerk_ui_build_form_field($form, $form_state, 'info_help');
    form_clerk_ui_build_form_field($form, $form_state, 'info_form_url');
    form_clerk_ui_build_form_field($form, $form_state, 'info_login_fieldset');
    form_clerk_ui_build_form_field($form, $form_state, 'info_login_help');
    form_clerk_ui_build_form_field($form, $form_state, 'info_login_url');
    form_clerk_ui_build_form_field($form, $form_state, 'info_login_form_id');
    form_clerk_ui_build_form_field($form, $form_state, 'info_user');
    form_clerk_ui_build_form_field($form, $form_state, 'info_password');
    form_clerk_ui_build_form_field($form, $form_state, 'info_submit');
  }
  elseif ($tab == 'configuration') {
    form_clerk_ui_build_form_field($form, $form_state, 'config_fieldset');
    form_clerk_ui_build_form_field($form, $form_state, 'config_help');
    form_clerk_ui_build_form_field($form, $form_state, 'config_directory');
    form_clerk_ui_build_form_field($form, $form_state, 'config_watchdog');
    form_clerk_ui_build_form_field($form, $form_state, 'config_submit');
  }
  else {
    form_clerk_ui_build_form_field($form, $form_state, 'input_fieldset');
    form_clerk_ui_build_form_field($form, $form_state, 'input_help');
    form_clerk_ui_build_form_field($form, $form_state, 'input_file');
    form_clerk_ui_build_form_field($form, $form_state, 'input_password');
    form_clerk_ui_build_form_field($form, $form_state, 'input_submit');
  }
  return $form;

}

/**
 * Validate handler for form ID 'form_clerk_form'.
 *
 * @param $form
 *  The multiform form argument.
 * @param $form_state
 *  The multiform state argument.
 */
function form_clerk_form_validate($form, &$form_state) {
      // Let 'submit' handle validation.
}

/**
 * Submit handler for form ID 'form_clerk_form'.
 * @param $form
 *  The form argument.
 * @param $form_state
 *  The form state argument.
 */
function form_clerk_form_submit($form, &$form_state) {

  $form_state['storage']['report_number'] = 1;
  $form_state['storage']['report'] = "Form Clerk Report\n";
  timer_start('Form Clerk');
  form_clerk_set_defaults($form_state);

  // Save inputs. This also cleans out existing data, if any.
  $values = $form_state['values'];
  foreach ($values as $key => $value) {
    $form_state['storage'][$key] = $value;
  }
  if ($form_state['storage']['op'] == 'Get Form Info') {
    form_clerk_process_form_info($form_state);
  }
  elseif ($form_state['storage']['op'] == 'Save Configuration') {
    form_clerk_process_save_config($form_state);
  }
  elseif ($form_state['storage']['op'] == 'Input Form Data') {
    if (variable_get('form_clerk_watchdog', TRUE)) {
      form_clerk_watchdog($form_state);
    }
    form_clerk_process_input($form_state);
  }
  // Setting $form_state['rebuild'] = TRUE saves the state for now.
  $form_state['rebuild'] = TRUE;
  return;
}

/**
 * Write watchdog message for this run.
 * @param $form_state
 *  The form state argument.
 */
function form_clerk_watchdog($form_state) {
  return;

  $inputs = $form_state['storage']['input']['input_file'];
  $message = 'Form Clerk run. Input files: @inputs';
  $variables = array('@inputs' => $inputs,);
  watchdog('form_clerk', $message, $variables, WATCHDOG_NOTICE);
  return;
}

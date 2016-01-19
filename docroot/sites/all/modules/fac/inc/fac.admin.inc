<?php

/**
 * @file
 * This file contains the settings forms of the Fast Autocomplete module.
 */

/**
 * Fast AutoComplete settings form.
 */
function fac_settings_form($form, $form_state) {
  $form = array();

  $form['backend'] = array(
    '#type' => 'fieldset',
    '#title' => t('Backend'),
  );

  // Get all available backend service options.
  $service_options = array();
  $service_infos = fac_get_service_info();
  foreach ($service_infos as $service_info) {
    $service_options[$service_info['class']] = $service_info['name'];
  }

  $form['backend']['fac_backend_service'] = array(
    '#type' => 'select',
    '#title' => t('The backend service to use'),
    '#description' => t('Select the backend service that is used by Fast Autocomplete. Note that if a service is dependent on another module (for example: the Search API backend service needs the Search API module to be enabled) it only shows up if that module is enabled.'),
    '#options' => $service_options,
    '#default_value' => variable_get('fac_backend_service', ''),
    '#required' => TRUE,
  );

  $form['behavior'] = array(
    '#type' => 'fieldset',
    '#title' => t('Behavior'),
  );

  $form['behavior']['fac_input_selectors'] = array(
    '#type' => 'textarea',
    '#title' => t('Inputs to enable Fast Autocomplete on'),
    '#description' => t('Enter the jQuery selector(s) for text input elements to enable the Fast Autocomplete functionality on those elements. You can provide multiple selectors divided by commas.'),
    '#default_value' => variable_get('fac_input_selectors', ''),
    '#attributes' => array(
      'placeholder' => t('for example: input[name="search_block_form"], input[name="some_other_form"]'),
    ),
  );

  $form['behavior']['fac_empty_result'] = array(
    '#type' => 'textarea',
    '#title' => t('Empty result to show when the input gets focus and there is no search term.'),
    '#description' => t('Enter the HTML to show when the search input gets focus and there is no search term. Useful for "quick links" for instance.'),
    '#default_value' => variable_get('fac_empty_result', ''),
  );

  $form['behavior']['fac_key_min_length'] = array(
    '#type' => 'textfield',
    '#title' => t('The minimum key length to trigger Fast Autocomplete'),
    '#description' => t('Enter the minimum key length to trigger the Fast Autocomplete on an input field. The minimum value is 1.'),
    '#default_value' => variable_get('fac_key_min_length', 1),
    '#required' => TRUE,
    '#size' => 2,
  );

  $form['behavior']['fac_key_max_length'] = array(
    '#type' => 'textfield',
    '#title' => t('The maximum key length to trigger Fast Autocomplete'),
    '#description' => t('Enter the maximum key length to trigger the Fast Autocomplete on an input field. The minimum value is 1.'),
    '#default_value' => variable_get('fac_key_max_length', 5),
    '#required' => TRUE,
    '#size' => 2,
  );

  $form['behavior']['fac_breakpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Breakpoint'),
    '#description' => t('Enter a minimum width in pixels to disable the Fast Autocomplete behavior until this minimum width is reached. Insert 0 to always enable the Fast Autocomplete behavior.'),
    '#default_value' => variable_get('fac_breakpoint', 0),
  );

  $form['behavior']['fac_use_module_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include the css provided by the module in the output'),
    '#description' => t("Enable this to include the css provided by the Fast Autocomplete module in the output. Don't forget to clear the Drupal cache after changing this option!"),
    '#default_value' => variable_get('fac_use_module_css', TRUE),
  );

  $form['json_files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Json files'),
  );

  $form['json_files']['fac_clean_up_files'] = array(
    '#type' => 'checkbox',
    '#title' => t('Periodically clean up json files'),
    '#description' => t('Enable cleaning up json files on cron to refresh the contents of the json files that contain the autocomplete suggestions.'),
    '#default_value' => variable_get('fac_clean_up_files', TRUE),
  );

  $form['json_files']['fac_files_expire_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Expire time'),
    '#description' => t('How old do the json files have to be to be considered expired? The value for this field should contain a relative string compared to now like "-1 month" or "-1 day"'),
    '#default_value' => variable_get('fac_files_expire_time', '-1 day'),
    '#size' => 20,
    '#states' => array(
      'visible' => array(
        'input[name="fac_clean_up_files"]' => array(
          'checked' => TRUE,
        ),
      ),
      'required' => array(
        'input[name="fac_clean_up_files"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  $form['json_files']['fac_bulk_generate_json_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Bulk generate json files'),
    '#description' => t('Enable daily bulk generating and refreshing json files on cron instead of only generating json files on user request.'),
    '#default_value' => variable_get('fac_bulk_generate_json_enabled', FALSE),
  );

  $form['json_files']['fac_bulk_generate_json_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Size'),
    '#description' => t('To what size of the search key should the json files be generated? CAUTION: setting a size larger than 2 rapidly increases the number of combinations to generate json files for!'),
    '#default_value' => variable_get('fac_bulk_generate_json_size', 2),
    '#size' => 3,
    '#states' => array(
      'visible' => array(
        'input[name="fac_bulk_generate_json_enabled"]' => array(
          'checked' => TRUE,
        ),
      ),
      'required' => array(
        'input[name="fac_bulk_generate_json_enabled"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  $form['json_files']['fac_files_delete_all'] = array(
    '#type' => 'button',
    '#value' => t('Delete all json files'),
    '#limit_validation_errors' => array(),
    '#name' => 'delete',
  );

  return system_settings_form($form);
}

/**
 * Fast Autocomplete settings form validation callback.
 */
function fac_settings_form_validate(&$form, &$form_state) {
  if ($form_state['clicked_button']['#name'] == 'delete') {
    drupal_goto('admin/config/search/fac/delete');
  }

  if (!is_numeric($form_state['values']['fac_key_min_length'])) {
    form_set_error('fac_key_min_length', t('Please enter a positive number.'));
  }
  else {
    if ($form_state['values']['fac_key_min_length'] < 1) {
      form_set_error('fac_key_min_length', t('The minimum value is 1'));
    }
  }

  if (!is_numeric($form_state['values']['fac_key_max_length'])) {
    form_set_error('fac_key_max_length', t('Please enter a positive number.'));
  }
  else {
    if ($form_state['values']['fac_key_max_length'] < 1) {
      form_set_error('fac_key_max_length', t('The minimum value is 1'));
    }
  }

  if (!is_numeric($form_state['values']['fac_breakpoint'])) {
    form_set_error('fac_breakpoint', t('Please enter zero or a positive number.'));
  }
  else {
    if ($form_state['values']['fac_breakpoint'] < 0) {
      form_set_error('fac_breakpoint', t('The minimum value is 0'));
    }
  }

  if ($form_state['values']['fac_clean_up_files'] == 1 && empty($form_state['values']['fac_files_expire_time'])) {
    form_set_error('fac_files_expire_time', t('The field expire time is required!'));
  }

  if ($form_state['values']['fac_bulk_generate_json_enabled'] == 1 && empty($form_state['values']['fac_bulk_generate_json_size'])) {
    form_set_error('fac_bulk_generate_json_size', t('The field size is required!'));
  }
}

/**
 * Returns the backend service settings form.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 *
 * @return array
 *   The backend service settings form.
 */
function fac_backend_settings_form($form, $form_state) {
  $backend_service = variable_get('fac_backend_service', '');

  if (empty($backend_service)) {
    drupal_set_message(t('You have not selected a backend service yet. Please select one below.'), 'warning');
    drupal_goto('admin/config/search/fac');
  }
  else {
    $service = new $backend_service();
    $form = $service->configurationForm($form, $form_state);
  }

  return $form;
}

/**
 * Validates the backend service settings form submitted values.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 */
function fac_backend_settings_form_validate(&$form, &$form_state) {
  $backend_service = variable_get('fac_backend_service', '');

  $service = new $backend_service();
  $service->configurationFormValidate($form, $form_state['values'], $form_state);
}

/**
 * Submits the backend service settings form submitted values.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state array.
 */
function fac_backend_settings_form_submit(&$form, &$form_state) {
  $backend_service = variable_get('fac_backend_service', '');

  $service = new $backend_service();
  $service->configurationFormSubmit($form, $form_state['values'], $form_state);
}

/**
 * Fast AutoComplete delete json files form.
 */
function fac_delete_form($form, $form_state) {
  $form = array();

  return confirm_form($form, t('Delete the Fast Autocomplete json files'), 'admin/config/search/fac', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Submit handler for the fac_delete_form.
 */
function fac_delete_form_submit($form, &$form_state) {
  fac_delete_json_files();

  $form_state['redirect'] = 'admin/config/search/fac';
}

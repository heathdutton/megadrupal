<?php

/**
 * @file
 * Watchdog reporter admin for list presets.
 */

/**
 * Callback function after drupal_get_form().
 */
function watchdog_reporter_list_preset_form($form, &$form_state) {

  // Get the preset list.
  $form_state['watchdog_reporter_preset_list'] = $watchdog_presets = variable_get('watchdog_reporter_preset_list', array());

  // Table header.
  $rows = $default_value = array();
  $header = array(
    'title' => array(
      'data' => t('Title'),
      'width' => '75%',
    ),
    'cronrule' => array(
      'data' => t('Schedule'),
      'width' => '20%',
    ),
    'operations' => array(
      'data' => t('Operations'),
      'width' => '20%',
    ),
  );

  // Table rows.
  foreach ($watchdog_presets as $key => $value) {

    $operations = l(t('Edit'), "admin/config/development/watchdog_reporter/preset/$key") . ' | ' . l(t('Delete'), "admin/config/development/watchdog_reporter/preset/delete/$key");
    $rows[$key] = array(
      'title' => $value['#values']['watchdog_reporter_preset_name'],
      'cronrule' => $value['#values']['watchdog_reporter_cronrule'],
      'operations' => $operations,
    );
    $default_value[$key] = $value['#enabled'];
  }

  // The table form element.
  $form['preset_list'] = array(
    '#type' => 'tableselect',
    '#title' => t('Preset List'),
    '#header' => $header,
    '#options' => $rows,
    '#multiple' => TRUE,
    '#default_value' => $default_value,
    '#empty' => t('No presets available. !link.', array('!link' => l('Create new preset', 'admin/config/development/watchdog_reporter/preset'))),
    '#js_select' => FALSE,
    '#attributes' => array('id' => 'watchdog_reporter_preset_list'),
  );

  // Submit button.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Implements hook_preprocess_table().
 */
function watchdog_reporter_preprocess_table(&$variables) {

  // Changing the checkbox header to Status string.
  if (isset($variables['attributes']['id']) && $variables['attributes']['id'] == 'watchdog_reporter_preset_list') {
    $variables['header'][0] = t('Status');
  }
}

/**
 * Implements hook_formID_submit().
 */
function watchdog_reporter_list_preset_form_submit(&$form, &$form_state) {

  // Update the presets status.
  foreach ($form_state['values']['preset_list'] as $key => $value) {
    $form_state['watchdog_reporter_preset_list'][$key]['#enabled'] = $value;
  }

  $preset_list = $form_state['watchdog_reporter_preset_list'];

  // Save the result.
  variable_set('watchdog_reporter_preset_list', $preset_list);
  drupal_set_message(t('Presets were enabled/disabled successfully.'), 'status');
}

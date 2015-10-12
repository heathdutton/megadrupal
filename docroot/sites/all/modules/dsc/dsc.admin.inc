<?php

/**
 * @file
 * Admin page callbacks for the dsc module.
 */

/**
 * Menu callback; Provides a setting form.
 */
function dsc_admin_settings($form, &$form_state) {
  $form = array();
  module_load_include('admin.inc', 'dblog');
  $filters = dblog_filters();
  $settings = variable_get('dsc_settings', array('default' => 100));
  $form['default'] = array(
    '#type' => 'textfield',
    '#title' => t('Default number of entries to keep for each type/category pair'),
    '#size' => 6,
    '#default_value' => $settings['default'],
    '#field_suffix' => t('items'),
    '#element_validate' => array('element_validate_integer_positive'),
  );
  $form['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Detailled settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  foreach ($filters['type']['options'] as $type_name => $type_label) {
    $form['details'][$type_name] = array(
      '#type' => 'fieldset',
      '#title' => $type_name,
      '#attributes' => array('class' => array('container-inline')),
      '#collapsible' => FALSE,
    );
    foreach ($filters['severity']['options'] as $severity_name) {
      $form['details'][$type_name][$type_name . '_' . $severity_name] = array(
        '#type' => 'textfield',
        '#title' => $severity_name,
        '#size' => 6,
        '#attributes' => array('placeholder' => $settings['default']),
        '#element_validate' => array('element_validate_integer_positive'),
        '#default_value' => !empty($settings[$type_name . '_' . $severity_name]) ? $settings[$type_name . '_' . $severity_name] : '',
      );
    }
  }
  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('container-inline'),
    ),
    'reset' => array(
      '#type' => 'submit',
      '#value' => t('Reset to defaults'),
      '#name' => 'reset',
    ),
    'save' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#name' => 'save',
    ),
  );
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'dsc') . '/js/dsc.js',
  );
  return $form;
}

function dsc_admin_settings_validate($form, &$form_state) {
  form_state_values_clean($form_state);
  $form_state['dsc_settings'] = array();
  $form_state['dsc_settings']['default'] = !empty($form_state['values']['default']) ? $form_state['values']['default'] : 100;

  foreach ($form_state['values'] as $key => $val) {
    if ($val > 0 && $val !== $form_state['dsc_settings']['default']) {
      $form_state['dsc_settings'][$key] = $val;
    }
  }
}

function dsc_admin_settings_submit($form, &$form_state) {
  if ($form_state['triggering_element']['#name'] === 'reset') {
    variable_del('dsc_settings');
    return;
  }
  variable_set('dsc_settings', $form_state['dsc_settings']);
  drupal_set_message(t('Settings for DSC have been saved.'));
}

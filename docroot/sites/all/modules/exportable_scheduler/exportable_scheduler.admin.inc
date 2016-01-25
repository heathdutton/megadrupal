<?php

/**
 * Settings form.
 */
function exportable_scheduler_admin_form($form, &$form_state) {
  ctools_include('export');

  $schemas = ctools_export_get_schemas();
  $options = array();
  foreach ($schemas as $key => $val) {
    $options[$key] = $val['name'];
  }

  $form['exportable_scheduler_schemas'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Exportables'),
    '#description' => t('Select which exportables can be scheduled.'),
    '#options' => $options,
    '#default_value' => variable_get('exportable_scheduler_schemas', array()),
  );

  return system_settings_form($form);
}

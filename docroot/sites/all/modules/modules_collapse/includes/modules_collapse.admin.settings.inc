<?php
/**
 * @file
 * Modules Collapse admin settings include file.
 */

function modules_collapse_settings($form, &$form_state) {
  $form = array();

  $options_yes_no = array(
    0 => t('No'),
    1 => t('Yes'),
  );

  $form['modules_collapse_collapse_modules'] = array(
    '#type' => 'radios',
    '#title' => t('Collapse modules package groups in modules list'),
    '#default_value' => variable_get('modules_collapse_collapse_modules', 1),
    '#options' => $options_yes_no,
    '#description' => t('If set to yes, the package groups on the modules admin page will be collapsed by default.'),
  );

  return system_settings_form($form);
}

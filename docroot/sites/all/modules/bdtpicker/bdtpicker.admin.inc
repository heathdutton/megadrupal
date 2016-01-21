<?php

/**
 * @file
 * Admin functions of Bootstrap Date & Time picker module.
 */

/**
 * Settings libraries.
 */
function bdtpicker_settings_libraries_form($form, &$form_state) {
  $name = 'bdtpicker_attach_to_created__node';
  $form[$name] = array(
    '#type' => 'checkbox',
    '#title' => t('Attach Bootstrap Date & Time picker to %label field on Content create/update form', array('%label' => t('Authored on'))),
    '#default_value' => variable_get($name, TRUE),
  );

  return system_settings_form($form);
}

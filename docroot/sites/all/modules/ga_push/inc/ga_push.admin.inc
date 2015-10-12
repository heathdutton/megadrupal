<?php
/**
 * @file
 * The administration functions and forms.
 */

/**
 * Form: GA Push settings.
 */
function ga_push_settings_form($form, &$form_state) {

  $options = ga_push_get_methods_option_list(NULL, FALSE);

  $form['ga_push_default_method'] = array(
    '#type' => 'radios',
    '#title' => t('Default method'),
    '#options' => $options,
    '#default_value' => variable_get('ga_push_default_method', 'javascript'),
    '#description' => t("Client-side: the action will be sent on the next page load - Server-side: the action will be sent immediately."),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

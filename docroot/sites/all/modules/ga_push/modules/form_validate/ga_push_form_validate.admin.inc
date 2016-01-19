<?php
/**
 * @file
 * The administration functions and forms.
 */

/**
 * From: validate_settings_form.
 *
 * @return array
 *   System settings form.
 */
function ga_push_form_validate_settings_form() {

  $form['ga_push_form_validate_forms'] = array(
    '#type' => 'textarea',
    '#title' => t('Active forms'),
    '#default_value' => variable_get('ga_push_form_validate_forms', ''),
    '#rows' => 10,
    '#description' => t('Insert the form ids to be logged on validate by line. You can user regular expressions.'),
  );

  $form['ga_push_form_validate_show_form_ids'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show form ids'),
    '#default_value' => variable_get('ga_push_form_validate_show_form_ids', FALSE),
  );

  return system_settings_form($form);
}

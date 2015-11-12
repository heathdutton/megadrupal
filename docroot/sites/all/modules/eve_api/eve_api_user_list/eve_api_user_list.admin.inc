<?php
/**
 * @file
 * Functions related to allow admin's to alter the settings and options.
 */

/**
 * Form constructor for the User List form.
 *
 * @see eve_api_user_list_admin_form_validate()
 * @see eve_api_user_list_admin_form_submit()
 *
 * @ingroup forms
 */
function eve_api_user_list_admin_form($form, &$form_state) {
  $form['user_list'] = array(
    '#type' => 'fieldset',
    '#title' => t('Modify User List Settings'),
    '#weight' => 0,
  );

  $form['user_list']['private_access'] = array(
    '#type' => 'radios',
    '#title' => t('Private API Key Information'),
    '#description' => t('Which role can access the Private API Key Information. By Permission is assignable to a role by "Access EVE API Private User List".'),
    '#options' => array(
      0 => t('Director, CEO, or By Permission'),
      1 => t('CEO or By Permission'),
      2 => t('By Permission Only'),
    ),
    '#weight' => 0,
    '#default_value' => (int) variable_get('eve_api_user_list_private_access', 0),
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 10,
  );

  if (variable_get('eve_api_first_run', FALSE) && !variable_get('eve_api_enable', FALSE)) {
    drupal_set_message(t('Please wait until the cron tasks have pulled the Alliance API Info.'));
    $form['submit_button']['#disabled'] = TRUE;
  }

  $form['#validate'] = array('eve_api_user_list_admin_form_validate');
  $form['#submit'] = array('eve_api_user_list_admin_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_user_list_admin_form().
 *
 * @see eve_api_user_list_admin_form_submit()
 */
function eve_api_user_list_admin_form_validate($form, &$form_state) {
}

/**
 * Form submission handler for eve_api_user_list_admin_form().
 *
 * @see eve_api_user_list_admin_form_validate()
 */
function eve_api_user_list_admin_form_submit($form, &$form_state) {
  variable_set('eve_api_user_list_private_access', (int) $form_state['values']['private_access']);

  drupal_set_message(t('User List Settings Updated.'));
}

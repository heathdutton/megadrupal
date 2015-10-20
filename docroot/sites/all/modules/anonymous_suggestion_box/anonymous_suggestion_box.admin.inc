<?php

/**
 * @file
 * Administration forms for the Anonymous Suggestion Box module.
 */

/**
 * Page callback for admin page form.
 *
 * Default values for each variable are set on hook_install().
 */
function anonymous_suggestion_box_admin() {
  $form = array();

  $form['anonymous_suggestion_box_method_db'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do you want to save records to the Database when a suggestion is submitted?'),
    '#default_value' => variable_get('anonymous_suggestion_box_method_db'),
  );

  $form['anonymous_suggestion_box_method_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do you want to receive Emails when a suggestion is submitted?'),
    '#default_value' => variable_get('anonymous_suggestion_box_method_email'),
  );

  $form['anonymous_suggestion_box_to_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email To'),
    '#default_value' => variable_get('anonymous_suggestion_box_to_email'),
    '#description' => t('The email address that suggestions are sent to.'),
  );
  $form['anonymous_suggestion_box_from_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email From'),
    '#default_value' => variable_get('anonymous_suggestion_box_from_email'),
    '#description' => t('The email address that suggestions are sent from.'),
  );
  $form['anonymous_suggestion_box_form_intro'] = array(
    '#type' => 'textarea',
    '#title' => t('Form Instructions Text'),
    '#default_value' => variable_get('anonymous_suggestion_box_form_intro'),
    '#description' => t('The text that is displayed above the suggestion box form, input plain text only. HTML Tags will not be displayed.'),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * Validates admin form submissions.
 */
function anonymous_suggestion_box_admin_validate($form, &$form_state) {
  // Check for valid email addresses on form submit if email method is
  // selected.
  if ($form_state['values']['anonymous_suggestion_box_method_email'] == 1) {
    if (!valid_email_address($form_state['values']['anonymous_suggestion_box_to_email'])) {
      form_set_error('anonymous_suggestion_box_to_email', t('The To e-mail address is not valid.'));
    }
    if (!valid_email_address($form_state['values']['anonymous_suggestion_box_from_email'])) {
      form_set_error('anonymous_suggestion_box_from_email', t('The From e-mail address is not valid.'));
    }
  }
  else {
    // If email method is not selected, check that the to and from
    // fields are valid or empty.
    if (!valid_email_address($form_state['values']['anonymous_suggestion_box_to_email']) && $form_state['values']['anonymous_suggestion_box_to_email'] != '') {
      form_set_error('anonymous_suggestion_box_to_email', t('The To e-mail address is not valid.'));
    }
    if (!valid_email_address($form_state['values']['anonymous_suggestion_box_from_email']) && $form_state['values']['anonymous_suggestion_box_from_email'] != '') {
      form_set_error('anonymous_suggestion_box_from_email', t('The From e-mail address is not valid.'));
    }
  }
  // Check that at least one method is selected, return error message
  // if both method_db and method_email are equal to zero.
  if ($form_state['values']['anonymous_suggestion_box_method_db'] == 0 && $form_state['values']['anonymous_suggestion_box_method_email'] == 0) {
    form_set_error('anonymous_suggestion_box_method_email', t('You have selected to not have submissions saved or sent. Please select one or both of the check boxes.'));
  }
}

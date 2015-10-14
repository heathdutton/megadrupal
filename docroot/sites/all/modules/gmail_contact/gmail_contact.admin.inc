<?php

/**
 * @file
 * Page callback for gmail contact admin page.
 */

/**
 * @param $form
 * @param $form_states
 * @return array
 */
function gmail_contact_admin_form($form, $form_states) {
  $form = array();

  $form['gmail_contact_code'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gmail client code'),
    '#required' => TRUE,
    '#collapsible' => TRUE,
    '#description' => t('Get it at Google developer console "APIs & auth" => Credentials OAuth Client ID'),
  );
  $form['gmail_contact_code']['gmail_contact_client_id'] = array(
    '#type' => 'textarea',
    '#title' => t('Gmail client id'),
    '#required' => TRUE,
    '#default_value' => variable_get('gmail_contact_client_id', ''),
  );
  $form['gmail_contact_code']['gmail_contact_client_secret'] = array(
    '#type' => 'textarea',
    '#title' => t('Gmail client secret'),
    '#required' => TRUE,
    '#default_value' => variable_get('gmail_contact_client_secret', ''),
  );

  $form['gmail_contact_configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gmail client configuration'),
    '#collapsible' => TRUE,
    '#description' => t('Configuration for Gmail Contact'),
  );
  $form['gmail_contact_configuration']['gmail_contact_max_result'] = array(
    '#type' => 'textfield',
    '#title' => t('Max number of result'),
    '#description' => t('Google may return thousands of contacts back for one Gmail account. Add one max number if you want.'),
    '#default_value' => variable_get('gmail_contact_max_result', ''),
  );
  $form['gmail_contact_configuration']['gmail_contact_name_required'] = array(
    '#type' => 'checkbox',
    '#title' => t('Contact Name required'),
    '#description' => t('Gmail contacts returned from Google may contain all contacts no matter this contact has name or not.
       Check this checkbox if you only want to return contacts with name associated.'),
    '#default_value' => variable_get('gmail_contact_name_required', ''),
  );
  $form['gmail_contact_configuration']['gmail_contact_sort'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sort contacts alphabetically'),
    '#description' => t('Sort contacts alphabetically as per contact names'),
    '#default_value' => variable_get('gmail_contact_sort', ''),
  );
  $form['gmail_contact_configuration']['gmail_contact_capitalize_name'] = array(
    '#type' => 'checkbox',
    '#title' => t('Capitalize first letter of name'),
    '#description' => t('Useful if you want to sort contacts alphabetically'),
    '#default_value' => variable_get('gmail_contact_capitalize_name', ''),
  );

  $form['gmail_contact_email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gmail invite email'),
    '#collapsible' => TRUE,
    '#description' => t('Email configuration'),
  );
  if (module_exists('token')) {
    $token_types = array('');
    $form['gmail_contact_email']['token_tree'] = array(
      '#theme' => 'token_tree',
      '#token_types' => $token_types,
    );
  }

  $form['gmail_contact_email']['gmail_contact_queue_send'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send emails using drupal queue'),
    '#description' => t("Using drupal queue will provide performance
      improvement, in case user sends thousands of emails in one time."),
    '#default_value' => variable_get('gmail_contact_queue_send', ''),
  );
  $form['gmail_contact_email']['gmail_contact_email_sender'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender Name'),
    '#description' => t("The sender's name. Leave it empty to use the site wide configured name."),
    '#default_value' => variable_get('gmail_contact_email_sender', ''),
  );
  $form['gmail_contact_email']['gmail_contact_email_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender Email Address'),
    '#description' => t("The sender's address. Leave it empty to use the site wide configured address."),
    '#default_value' => variable_get('gmail_contact_email_address', ''),
  );
  $form['gmail_contact_email']['gmail_contact_email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Email subject'),
    '#description' => t("Email's subject"),
    '#default_value' => variable_get('gmail_contact_email_subject', ''),
  );
  $form['gmail_contact_email']['gmail_contact_email_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Email Message'),
    '#description' => t("Email's Message"),
    '#default_value' => variable_get('gmail_contact_email_message', ''),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function gmail_contact_admin_form_validate($form, $form_state) {
  $number = trim($form_state['values']['gmail_contact_max_result']);
  if (!is_numeric($number) || $number != (int) $number || $number <= 0) {
    form_set_error('gmail_contact_max_result', t('Max number must be an integer number over zero.'));
  }
}

function gmail_contact_admin_form_submit($form, $form_state) {
  // Exclude unnecessary elements.
  unset($form_state['values']['submit'],
    $form_state['values']['form_id'],
    $form_state['values']['op'],
    $form_state['values']['form_token'],
    $form_state['values']['form_build_id']
  );
  foreach ($form_state['values'] as $key => $value) {
    variable_set($key, trim($value));
  }

  drupal_set_message(t('Configuration saved successfully!'));
}

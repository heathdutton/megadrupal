<?php

/**
 * Callback to provide fields for the basic mail debugger interface.
 * 
 * @param array $default_values
 * @return array|boolian
 */
function mail_debugger_basic_form($default_values = array()) {
  if (!user_access('use mail debugger basic')) {
    return FALSE;
  }
  
  return array(
    'to' => array(
      '#type' => 'textfield',
      '#title' => t('To'),
      '#default_value' => $default_values['to'],
    ),
    'subject' => array(
      '#type' => 'textfield',
      '#title' => t('Subject'),
      '#default_value' => $default_values['subject'],
    ),
    'body' => array(
      '#type' => 'textarea',
      '#title' => t('Message'),
      '#default_value' => $default_values['body'],
    ),
  );
}

/**
 * Validate entered values
 * @param array $values
 * @param array $elements
 * @param array $form_state
 */
function mail_debugger_basic_form_validate($values, $elements, &$form_state) {
  if (!valid_email_address($values['to'])) {
    form_set_error($elements['to'], t('Enter a valid e-mail address'));
  }
  if (empty($values['subject'])) {
    form_set_error($elements['subject'], t('Enter a subject'));
  }
  if (empty($values['body'])) {
    form_set_error($elements['body'], t('Enter content for the mail'));
  }
}

/**
 * 
 * @param type $values
 * @param type $form_state
 */
function mail_debugger_basic_form_submit($values, &$form_state) {
  // Send the mail
  $mail = drupal_mail('mail_debugger_common', 'basic', $values['to'], NULL, array(
    'body' => $values['body'],
    'subject' => $values['subject'],
  ));

  // Notify about the result
  if ($mail && $mail['result']) {
    drupal_set_message(t('Message sent'));
  } else {
    drupal_set_message(t('Message sent with errors. Check the error log.'), 'warning');
  }
}

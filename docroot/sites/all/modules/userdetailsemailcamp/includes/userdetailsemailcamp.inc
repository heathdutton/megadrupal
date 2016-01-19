<?php
/**
 * @file
 * Send mails to admin about the user details and their status.
 */

/**
 * Implements form to send the mail.
 */
function userdetailsemailcamp_mailsend() {
  $form['userdetailsemailcamp_setting_emailcamp'] = array(
    '#type' => 'select',
    '#title' => t('Select a value to send mail to admin'),
    '#empty_option' => t('Select a  value'),
    '#options' => array(
      '0' => t('daily'),
      '1' => t('monthly'),
    ),
    '#default_value' => variable_get('userdetailsemailcamp_setting_emailcamp', ''),
    '#description' => t('if daily is selected then mail will send to admin daily. if monthly is selected then mail will send to admin monthly'),
    '#required' => 1,
  );
  $form['userdetailsemailcamp_from_email_setting'] = array(
    '#type' => 'textfield',
    '#title' => t('From Email Address'),
    '#size' => 60,
    '#maxlength' => 255,
    '#description' => t('Provide From Address to send mails.if empty system will take site email address as "from address". Eg:noreply@gmail.com,example@gmail.com,test@gmail.com,.....'),
    '#default_value' => variable_get('userdetailsemailcamp_from_email_setting', ''),
    '#element_validate' => array('userdetailsemailcamp_custom_email_validate'),
  );
  $form['userdetailsemailcamp_to_email_setting'] = array(
    '#type' => 'textfield',
    '#title' => t('To Email Address'),
    '#size' => 60,
    '#maxlength' => 255,
    '#description' => t('Provide To Address to send mails. Please provide the admin email address suggessted by client.Eg:admin@admin.com,example@gmail.com,test@gmail.com,...'),
    '#default_value' => variable_get('userdetailsemailcamp_to_email_setting', ''),
    '#required' => 1,
    '#element_validate' => array('userdetailsemailcamp_custom_email_validate'),
  );
  return system_settings_form($form);
}

/**
 * @file
 * Function to validate the email address.
 */
function userdetailsemailcamp_custom_email_validate($element, &$form_state, $form) {
  $value = $element['#value'];
  $emailvalidates = (explode(",", $value));
  foreach ($emailvalidates as $emailvalidate) {
    if (!empty($emailvalidate)) {
      if (!valid_email_address($emailvalidate)) {
        form_error($element, t('Please enter a valid email address.'));
        break;
      }
    }
  }
}

<?php
/**
 * @file
 * Sendinblue module home admin settings.
 */

/**
 * Display Home page of module.
 *
 * @return array
 *   Render array.
 */
function sendinblue_home_page() {
  $page_html = '<div id="wrap" class="box-border-box container-fluid"><div id="wrap-left" class="box-border-box col-md-9">';
  if (SendinblueManager::isLoggedInState()) {
    $page_html .= SendinblueManager::generateHomeLogin();
  }
  else {
    $page_html .= SendinblueManager::generateHomeLogout();
  }
  $page_html .= '</div><div id="wrap-right-side" class="box-border-box  col-md-3">';
  $page_html .= SendinblueManager::generateSidebar();
  $page_html .= '</div></div>';
  $output = array(
    'main_wrapper' => array(
      '#type' => 'markup',
      '#markup' => $page_html,
    ),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'sendinblue') . '/css/admin-setting.css'),
    ),
  );
  return $output;
}

/**
 * Login form of Home page.
 */
function sendinblue_login_form($form, &$form_state) {
  $form = array();
  $form['access_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#required' => TRUE,
    '#attributes' => array(
      'placeholder' => t('API Key'),
    ),
    '#size' => 50,
    '#maxlenght' => 100,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Login'),
  );
  return $form;
}

/**
 * Validation Handler of Login form.
 */
function sendinblue_login_form_validate($form, &$form_state) {
  $access_key = $form_state['values']['access_key'];

  $mailin = new SendinblueMailin(SendinblueManager::API_URL, $access_key);
  $response = $mailin->getAccount();
  if (is_array($response) && ($response['code'] == 'success')) {
    $account_data = $response['data'];
    $count = count($account_data);
    $account_email = $account_data[$count - 1]['email'];
    $account_user_name = $account_data[$count - 1]['first_name'] . ' ' . $account_data[$count - 1]['last_name'];
    variable_set(SendinblueManager::ACCESS_KEY, $access_key);
    variable_set(SendinblueManager::ACCOUNT_EMAIL, $account_email);
    variable_set(SendinblueManager::ACCOUNT_USERNAME, $account_user_name);
    variable_set(SendinblueManager::ACCOUNT_DATA, $account_data);

    $smtp_details = SendinblueManager::updateSmtpDetails();

    if (($smtp_details == FALSE) || ($smtp_details['relay'] == FALSE)) {
      variable_set('sendinblue_on', 0);
    }
    else {
      variable_set('sendinblue_on', 1);
    }

    $mailin->partnerDrupal();
  }
  else {
    form_set_error('access_key');
    if (!empty($access_key)) {
      drupal_set_message(t('API key is invalid'), 'error');
    }
  }
}

/**
 * Form to send email of Home page.
 */
function sendinblue_send_email_form($form, &$form_state) {
  $smtp_details = variable_get(SendinblueManager::SMTP_DETAILS, FALSE);
  if ($smtp_details == FALSE) {
    $smtp_details = SendinblueManager::updateSmtpDetails();
  }
  if (($smtp_details == FALSE) || ($smtp_details['relay'] == FALSE)) {
    variable_set('sendinblue_on', 0);
    $smtp_available = FALSE;
  }
  else {
    $smtp_available = TRUE;
  }

  $form = array();
  if ($smtp_available == FALSE) {
    $form['sendinblue_alert'] = array(
      '#type' => 'markup',
      '#prefix' => '<div id="sendinblue_alert_area" style="padding: 10px;background-color: #fef5f1;color: #8c2e0b;border-color: #ed541d;border-width: 1px;border-style: solid;">',
      '#markup' => t('Current you can not use SendinBlue SMTP. Please confirm at <a href="@smtp-sendinblue" target="_blank">Here</a>', array('@smtp-sendinblue' => 'https://mysmtp.sendinblue.com/?utm_source=drupal_plugin&utm_medium=plugin&utm_campaign=module_link')),
      '#suffix' => '</div>',
      '#tree' => TRUE,
    );
  }

  $form['sendinblue_on'] = array(
    '#type'          => 'radios',
    '#title'         => t('Send emails through SendinBlue SMTP'),
    '#default_value' => variable_get('sendinblue_on', 0),
    '#description'   => t('Choose "Yes" if you want to use SendinBlue SMTP to send transactional emails.'),
    '#options'       => array(1 => t('Yes'), 0 => t('No')),
    '#disabled'      => ($smtp_available == TRUE) ? FALSE : TRUE,
  );

  $form['sendinblue_to_email'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Enter email to send a test'),
    '#description'   => t('Select here the email address you want to send a test email to.'),
    '#disabled'      => ($smtp_available == TRUE) ? FALSE : TRUE,
    '#states' => array(
      // Hide unless needed.
      'visible' => array(
        ':input[name="sendinblue_on"]' => array('value' => 1),
      ),
      'required' => array(
        ':input[name="sendinblue_on"]' => array('value' => 1),
      ),
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
    '#disabled'      => ($smtp_available == TRUE) ? FALSE : TRUE,
  );
  return $form;
}

/**
 * Validation Handler of Form to send email of Home page.
 */
function sendinblue_send_email_form_validate($form, &$form_state) {
  $sendinblue_on = $form_state['values']['sendinblue_on'];
  $send_email = $form_state['values']['sendinblue_to_email'];
  if ($sendinblue_on == '1') {
    $smtp_details = variable_get(SendinblueManager::SMTP_DETAILS, NULL);
    if ($smtp_details == NULL) {
      $smtp_details = SendinblueManager::updateSmtpDetails();
    }
    if ($smtp_details['relay'] == FALSE) {
      $sendinblue_on = 0;
    }
  }
  variable_set('sendinblue_on', $sendinblue_on);
  if ($send_email != '') {
    if (!valid_email_address($send_email)) {
      form_set_error('sendinblue_to_email', t('The email address is invalid.'));
      return;
    }
  }
}

/**
 * Submit Handler of Form to send email of Home page.
 */
function sendinblue_send_email_form_submit($form, &$form_state) {
  $send_email = $form_state['values']['sendinblue_to_email'];
  SendinblueManager::sendEmail('test', $send_email, '', '');
}

<?php
/**
 * @file
 * Form renderer and submission callback functions for the endicia module's
 * admin & configuration pages.
 */

/**
 * Form for requesting refunds on printed labels.
 *
 * @ingroup forms
 */
function endicia_ui_refund_postage_form($form, &$form_state) {
  if (_endicia_in_test_mode()) {
    drupal_set_message(t('Reminder: Endicia is operating in test mode. Your account will not actually be credited.'), 'warning');
  }

  // We have some tricks that allow the URL format to be something like
  // .../refund/REFUND_ID_TYPE/REFUND_ID1/REFUND_ID2/....
  // URL trick part 1/3: get rid of $form and $form_state
  $args = func_get_args();
  array_shift($args);
  array_shift($args);

  $form['Description'] = array(
    '#markup' => 'You may process refunds for unused labels up to 5 at a time. Your account will be credited after 14 days.',
  );
  $options = array(
    'PICNumber' => 'Tracking (PIC) number',
    'CustomsID' => 'Customs ID',
    'PieceID' => 'Piece ID',
  );
  $form['Type'] = array(
    '#type' => 'radios',
    '#title' => t('Entry type'),
    '#description' => t('Labels for designated for refund can be identified by their tracking number (PIC Number),  Customs ID or Package Piece ID.'),
    '#options' => $options,
    // URL trick part 2/3: Set refund type if type passed from URL is valid
    '#default_value' => (isset($args[0]) && array_key_exists($args[0], $options)) ? $args[0] : 'PICNumber',
    '#required' => TRUE,
  );
  for ($i = 1; $i <= 5; $i++) {
    $form['Number'.$i] = array(
      '#type' => 'textfield',
      '#title' => t('Package @num', array('@num' => $i)),
      '#required' => $i == 1,
      '#maxlength' => 30,
      // URL trick part 3/3: When looping over the 5 available refund IDs, see
      // if we can extract them from the URL.
      '#default_value' => isset($args[$i]) ? $args[$i] : '',
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Request refund'),
  );

  return $form;
}

/**
 * Submit handler for postage refund form.
 *
 * @see endicia_ui_refund_postage_form().
 */
function endicia_ui_refund_postage_form_submit($form, &$form_state) {
  $account_id = variable_get('endicia_account_id', '');
  $passphrase = variable_get('endicia_passphrase', '');
  if (!$account_id) {
    drupal_set_message(t('Your account ID has not been set! Please configure your Endicia account information first.'), 'error');
    drupal_goto('admin/endicia/settings/credentials');
  }

  $valid = array();
  for ($i = 1; $i <= 5; $i++) {
    $number = $form_state['values']['Number'.$i];
    if (empty($number)) {
      continue;
    }
    $data = array(
      'AccountID' => $account_id,
      'Password' => $passphrase,
      'Test' => _endicia_in_test_mode() ? 'Y' : 'N',
      'RefundList' => array(
        $form_state['values']['Type'] => $number,
      ),
    );

    $response_node = 'RefundResponse';
    $response = endicia_api_request(ENDICIA_ELS_SERVICES_URI, 'RefundRequest', $data, $response_node);
    // Check for failures
    if ($response === FALSE) {
      _endicia_communication_error_redirect('admin/endicia/account/refund');
    }
    // Check for errors
    if ($error = endicia_api_request_error_check($response, $response_node)) {
      $message = 'Could not process refund for package #@identifier: %errormsg (check the site log for additional details).';
      $variables = array(
        '@identifier' => $number,
        '%errormsg' => $error['message'],
      );
      drupal_set_message(t($message, $variables), 'error');
      continue;
    }
    $valid[] = $i;
  }
  if (count($valid) > 0) {
   drupal_set_message('Your refund request(s) were processed successfully.');
  }
}

/**
 * Display the account status form, informing the user of their account balance
 * and account status (active/suspended).
 *
 * FIXME: Add caching so we don't spam the servers.
 *
 * @ingroup forms
 */
function endicia_ui_status() {
  $account_info = endicia_get_account_status();
  $variables = array();
  if ($account_info === FALSE) {
    $message = 'Your account status could not be retrieved.';
  }
  elseif ($account_info == array()) {
    $message = 'Your account is currently <strong>suspended</strong>.';
  } else {
    $message = 'Your account is currently <strong>active</strong> and has a balance of <strong>$@balance</strong>.';
    $variables = array('@balance' => $account_info['PostageBalance']);
  }
  $output = '<div>' . t($message, $variables) . '</div>';
  if (_endicia_in_test_mode()) {
    $output .= '<div>';
    $message = 'Please note that Endicia is currently in <strong>testing mode</strong> (you can change this in the !account_settings). Your account will not be billed for any transactions and the server will return preprogrammed responses that may not reflect the nature of your shipping requests.';
    $variables = array(
      '!account_settings' => l(t('account settings'), 'admin/endicia/settings/credentials'),
    );
    $output .= t($message, $variables);
    $output .= '</div>';
  }
  return $output;
}

/**
 * Generates the admin settings form.
 *
 * This form is where users are able to input their Account ID, password, and
 * switch between testing or debug mode.
 *
 * @see theme_endicia_ui_settings_credentials()
 * @ingroup forms
 */
function endicia_ui_settings_credentials() {
  drupal_add_css(drupal_get_path('module', 'endicia') . '/theme/endicia_ui.css');
  $form = array();

  $form['description_compatibility'] = array(
    '#value' => t('This module requires an Grindflow Endicia Cloud (GEC) account with Grindflow Management LLC, an Endicia partner. This module is not compatible with Endicia\'s desktop print postage services and applications.'),
  );

  $link = l(t('Click here'), 'http://www.grindflow.com/grindflow-endicia-cloud-services/sign-up', array('external' => TRUE, 'attributes' => array('target' => '_blank')));
  $form['description_plans'] = array(
    '#value' => t('!click_here to learn more about features and pricing plans.', array('!click_here' => $link)),
  );

  $form['credentials'] = array(
    '#type' => 'fieldset',
    '#title' => t('Grindflow Endicia Cloud Credentials'),
    '#tree' => FALSE,
  );

  $form['credentials']['endicia_account_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Endicia Account ID'),
    '#description' => t('Your Account ID is assigned to you by Endicia and should consist of 6 digits.'),
    '#default_value' => variable_get('endicia_account_id', ''),
  );

  $form['credentials']['endicia_passphrase'] = array(
    '#type' => 'password',
    '#title' => t('Endicia Software Passphrase'),
    '#description' => t('Enter the software passphrase for your Endicia account (maximum length 64 characters). If  this field is left blank, the existing saved passphrase will not be changed.'),
    '#maxlength' => 64,
  );

  $link = l(t('Grindflow user account settings'), 'http://grindflow.com/endicia/get-authtoken', array('external' => TRUE, 'attributes' => array('target' => '_blank')));
  $form['credentials']['endicia_gf_auth_token'] = array(
    '#type' => 'textfield',
    '#title' => t('GEC Authorization Token'),
    '#description' => t('Copy the authorization token provided in your !gf_account_settings here.', array('!gf_account_settings', $link)),
    '#default_value' => variable_get('endicia_gf_auth_token', ''),
  );

  $form['credentials']['endicia_test_mode'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Testing mode'),
    '#default_value' => variable_get('endicia_test_mode', 0),
    '#description'   => t('Use the Endicia test servers. Your account will not be billed for any transactions.'),
  );

  $form['credentials']['endicia_debug_mode'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Debugging and testing mode'),
    '#default_value' => variable_get('endicia_debug_mode', 0),
    '#description'   => t('Debugging mode enables more verbose error messages as well as forces the use of the Endicia test servers.'),
  );

  // Negative value indicates disabled status
  $form['credentials']['endicia_partner_messages'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Receive partner messages'),
    '#default_value' => variable_get('endicia_message_last_time', 0) >= 0,
    '#description'   => t("Occasionally display notifications about Endicia's new features or module updates from Grindflow Management LLC."),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form submit handler for the account credentials form.
 */
function endicia_ui_settings_credentials_submit($form, &$form_state) {
  variable_set('endicia_account_id', $form_state['values']['endicia_account_id']);
  // Only overwrite password if they actually supplied one
  if (!empty($form_state['values']['endicia_passphrase'])) {
    variable_set('endicia_passphrase', $form_state['values']['endicia_passphrase']);
  }
  variable_set('endicia_gf_auth_token', $form_state['values']['endicia_gf_auth_token']);

  variable_set('endicia_test_mode', $form_state['values']['endicia_test_mode']);
  variable_set('endicia_debug_mode', $form_state['values']['endicia_debug_mode']);

  if (!$form_state['values']['endicia_partner_messages']) {
    // Disable partner messages
    variable_set('endicia_message_last_time', -1);
  }
  else if ($form_state['values']['endicia_partner_messages'] && variable_get('endicia_message_last_time', 0) == -1) {
    // Enable partner messages if they were previously disabled
    variable_set('endicia_message_last_time', time());
  }

  // Verify account credentials
  $account_info = endicia_get_account_status();
  if ($account_info === FALSE) {
    form_error($form['credentials'], t('Your account credentials could not be verified with Endicia: incorrect account ID or password.'));
    $form_state['rebuild'] = TRUE;
    return;
  }
  else if ($account_info === NULL) {
    form_error($form['credentials'], t('Could not communicate with the GEC servers. Please verify that the configured GEC authentication token is correct and try again.'));
    $form_state['rebuild'] = TRUE;
    return;
  }
  drupal_set_message(t('Your Endicia account ID and password were verified successfully.'));

  // Notifications
  if ($account_info['PostageBalance'] == 0) {
    $link = l(t('purchase postage credit'), 'admin/endicia/account/purchase');
    drupal_set_message(t('Your account balance is empty! Please !purchase_credit before attempting to create postage labels.', array('!purchase_credit' => $link)), 'warning');
  }

  $available_wrappers = array_keys(file_get_stream_wrappers());
  if (!in_array('private', $available_wrappers)) {
    $link = l(t('Click here'), 'admin/config/media/file-system');
    drupal_set_message(t("Drupal's private file system has not been configured! It is required in order to securely store the labels images. !click_here to configure it now.", array('!click_here' => $link)), 'error');
  }
}

/**
 * Returns HTML for the admin settings form.
 *
 * @param $form
 *  Array containing the form to be themed
 *
 * @see endicia_ui_settings_credentials()
 * @ingroup themeable
 */
function theme_endicia_ui_settings_credentials($variables) {
  $form = $variables['form'];
  $output = '<div class="description-compatibility">' . drupal_render_children($form['description_compatibility']) . '</div>';
  $output .= '<div class="description-plans">' . drupal_render_children($form['description_plans']) . '</div>';

  $output .= drupal_render_children($form);

  $img_path = drupal_get_path('module', 'endicia_ui') . '/theme/logo_grindflow_endicia.jpg';
  $logo = theme('image', array('path' => $img_path, 'alt' => t('Logos'), 'title' => t('Logos')));

  $output .= '<div class="logo-images">';
  $output .= $logo;
  $output .= '</div>';

  return $output;
}

function endicia_ui_settings_config() {
  $form = array();

  $form['defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shipping defaults'),
    '#tree' => FALSE,
  );

  $options = _endicia_pkg_types();
  $form['defaults']['endicia_pkg_type_visibility'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available Mailpiece Shapes'),
    '#description' => t('You may choose to disable mailpiece shapes that you do not use to prevent them from appearing during the creation of postage labels.'),
    '#options' => $options,
    '#default_value' => variable_get('endicia_pkg_type_visibility', array_keys($options)),
  );

  $form['defaults']['endicia_service_type_domestic'] = array(
    '#type' => 'select',
    '#title' => t('Shipping Class (Domestic)'),
    '#description' => t('Parcels & envelopes shipping domestically will use this shipping class.'),
    '#options' => _endicia_mail_classes(),
    '#default_value' => variable_get('endicia_service_type_domestic', 'Express'),
  );

  $form['defaults']['endicia_service_type_international'] = array(
    '#type' => 'select',
    '#title' => t('Shipping Class (International)'),
    '#description' => t('Parcels & envelopes shipping internationally will use this shipping class.'),
    '#options' => _endicia_mail_classes('International'),
    '#default_value' => variable_get('endicia_service_type_international', 'Express'),
  );

  $form['labels'] = array(
    '#type' => 'fieldset',
    '#title' => t('Label Settings'),
    '#tree' => FALSE,
  );

  // Form to select how long labels stay on server
  $period = drupal_map_assoc(array(86400, 302400, 604800, 1209600, 2419200, 0), 'format_interval');
  $period[0] = t('Forever');
  $form['labels']['endicia_label_lifetime'] = array(
    '#type'          => 'select',
    '#title'         => t('Label lifetime'),
    '#default_value' => variable_get('endicia_label_lifetime', 1209600),
    '#options'       => $period,
    '#description'   => t('Controls how long labels are stored on the server before being automatically deleted. Cron must be enabled for automatic deletion.'),
  );

  $formats = array(
    //'PNG' => 'PNG (default)',
    'GIF' => 'GIF (default)',
    'JPEG' => 'JPEG',
    'PDF' => 'PDF',
    'ZPLII' => 'ZPLII',
    'EPL2' => 'EPL2'
  );
  $form['labels']['endicia_label_image_format'] = array(
    '#type'          => 'select',
    '#title'         => t('Label image format'),
    '#default_value' => variable_get('endicia_label_image_format', 'PNG'),
    '#options'       => $formats,
  );

  $form = system_settings_form($form);
  array_unshift($form['#submit'], 'endicia_ui_settings_config_submit');
  $form['#theme'] = 'endicia_ui_settings_config';

  return $form;
}

/**
 * Submit handler function for the module configuration form
 */
function endicia_ui_settings_config_submit($form, &$form_state) {
  if (in_array($form_state['values']['endicia_label_image_format'], array('ZPLII', 'EPL2')) && !module_exists('jzebra')) {
    drupal_set_message(t('The jZebra module is required in order to print ZPL labels. Please install it before printing any ZPL/EPL bar-code labels.'), 'warning');
  }
}

/**
 * Displays the change passphrase form, allowing users to change their account's
 * passphrase.
 *
 * @ingroup forms
 */
function endicia_ui_change_passphrase_form() {
  $account_id = variable_get('endicia_account_id', '');
  if (!$account_id) {
    drupal_set_message(t('Your account ID has not been set! Please configure your Endicia account information first.'), 'error');
    drupal_goto('admin/endicia/settings/credentials');
  }

  if (_endicia_in_test_mode()) {
    drupal_set_message(t('Reminder: Endicia is operating in test mode. Your software passphrase will not actually be modified.'), 'warning');
  }

  $form['Description'] = array(
    '#markup' => t('Passphrase change request for account #@account. For security purposes, you are required to re-enter your current Endicia software passphrase below. This is different than your Internet passphrase (used to manage your Endicia account from their web portal).', array('@account' => $account_id)),
  );

  $form['Password'] = array(
    '#type' => 'password',
    '#title' => t('Current Software Passphrase'),
    '#maxlength' => 64,
    '#required' => TRUE,
  );

  $form['NewPassword'] = array(
    '#type' => 'password',
    '#title' => t('New Software Passphrase'),
    '#maxlength' => 64,
    '#required' => TRUE,
  );
  $form['NewPassword_Confirm'] = array(
    '#type' => 'password',
    '#title' => t('Confirm New Software Passphrase'),
    '#maxlength' => 64,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Change passphrase'),
  );

  return $form;
}

/**
 * Validation handler for the password change form.
 *
 * @see endicia_ui_change_passphrase_form()
 */
function endicia_ui_change_passphrase_form_validate($form, &$form_state) {
  if ($form_state['values']['NewPassword'] != $form_state['values']['NewPassword_Confirm']) {
    form_set_error('NewPassPhrase', t('The new passphrase and its confirmation must match.'));
  }
}

/**
 * Submit handler for the password change form.
 *
 * @see endicia_ui_change_passphrase_form()
 */
function endicia_ui_change_passphrase_form_submit($form, &$form_state) {
  $data = array(
    'AccountID' => variable_get('endicia_account_id', ''),
    'Password' => $form_state['values']['Password'],
    'NewPassword' => $form_state['values']['NewPassword'],

  );
  $uri = _endicia_in_test_mode() ? ENDICIA_TEST_URI : ENDICIA_PRODUCTION_URI;
  $response_node = 'ChangePassPhraseRequestResponse';
  $response = endicia_api_request($uri, 'ChangePassPhrase', $data, $response_node);
  // Check for failures
  if ($response === FALSE) {
    _endicia_communication_error_redirect('admin/endicia/account/passphrase');
  }

  // Check for errors
  if ($error = endicia_api_request_error_check($response, $response_node)) {
    $message = 'Could not change your software passphrase. Error #@errornum: %errormsg (check the site log for additional details).';
    $variables = array(
      '@errornum' => $error['status'],
      '%errormsg' => $error['message'],
    );
    drupal_set_message(t($message, $variables), 'error');
  }
  if (_endicia_in_test_mode()) {
    drupal_set_message(t('Your passphrase change request was processed successfully. Note: your actual Endicia software passphrase has not been changed because Endicia is in test mode.'));
  }
  else {
    variable_set('endicia_passphrase', $form_state['values']['NewPassword']);
    drupal_set_message(t('Your software passphrase was changed successfully. Your Endicia account settings have automatically been updated.'));
  }
  $form_state['redirect'] = 'admin/endicia';
}

/**
 * Displays the purchase postage form, allowing users to credit their account.
 *
 * @ingroup forms
 */
function endicia_ui_buy_postage_form() {
  $account_id = variable_get('endicia_account_id', '');
  if (!$account_id) {
    drupal_set_message(t('Your account ID has not been set! Please configure your Endicia account information first.'), 'error');
    drupal_goto('admin/endicia/settings/credentials');
  }

  if (_endicia_in_test_mode()) {
    drupal_set_message(t('Reminder: Endicia is operating in test mode. Your account balance will not be changed.'), 'warning');
  }

  $message = t('This transaction will credit account #@account_id. Please observe the following restrictions:', array('@account_id' => variable_get('endicia_account_id', '')));

  $restrictions = array(
    t('The minimum required USPS postage purchase amount is $10.'),
    t('You may make an unlimited number of USPS postage purchases in a calendar month, free of sales tax.'),
    t('<strong>Customers using credit card payment:</strong> You may not exceed a $500 postage balance at any given time. In the event your combined monthly postage purchases exceed $500, you are required to establish ACH (direct debit) as payment for future postage purchases within the first 10 days of the following month.'),
    t('<strong>Customers using ACH payment:</strong> Your USPS postage balance is restricted to a $500 balance during the initial setup period. After this period, there is no postage purchase limitation.'),
  );

  $form = array();

  $form['Description'] = array(
    '#markup' => $message . theme('item_list', array('items' => $restrictions)),
  );

  $form['RecreditAmount'] = array(
    '#type' => 'textfield',
    '#title' => t('Postage credit amount'),
    '#description' => t('Enter an amount in US dollars in order to credit USPS postage to your account. This amount will be billed according to the payment method you have on file with Endicia.'),
    '#maxlength' => 6,
    '#size' => 7,
    '#required' => TRUE,
    '#field_prefix' => '$',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Continue'),
  );

  return $form;
}

/**
 * Validation handler for the purchase postage form.
 *
 * @see endicia_ui_buy_postage_form()
 */
function endicia_ui_buy_postage_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['RecreditAmount']) ||
      $form_state['values']['RecreditAmount'] < 10 ||
      $form_state['values']['RecreditAmount'] > 99999) {
    form_set_error('RecreditAmount', t('The Credit Amount field must be a valid number between $10 and $99,999.'));
  }
}


/**
 * Submit handler for the purchase postage form.
 *
 * @see endicia_ui_buy_postage_form()
 */
function endicia_ui_buy_postage_form_submit($form, &$form_state) {
  $_SESSION['endicia_recredit_amount'] = $form_state['values']['RecreditAmount'];
  $form_state['redirect'] = 'admin/endicia/account/purchase/confirm';
}

/**
 * Confirmation form for purchasing postage.
 *
 * @ingroup forms
 */
function endicia_ui_buy_postage_confirm_form() {
  // Session consistency check
  if (!isset($_SESSION['endicia_recredit_amount']) || !is_numeric($_SESSION['endicia_recredit_amount'])) {
    drupal_set_message(t('Could not determine the credit amount from your session (this may occur if you have refreshed the page). Please try again.'), 'error');
    drupal_goto('admin/endicia/account/purchase');
  }

  $form['amount'] = array(
    '#type' => 'hidden',
    '#value' => $_SESSION['endicia_recredit_amount'],
  );
  $question = t('Are you sure you would like to purchase $@amount of postage?', array('@amount' => $_SESSION['endicia_recredit_amount']));
  $path = 'admin/endicia/account/purchase';
  $description = t('You will be billed this amount immediately via the payment method setup for this account.');
  return confirm_form($form, $question, $path, $description, t('Buy postage'), t('Cancel'));
}

/**
 * Submit handler for confirmation form.
 *
 * @see endicia_ui_buy_postage_confirm_form()
 */
function endicia_ui_buy_postage_confirm_form_submit($form, &$form_state) {
  unset($_SESSION['endicia_recredit_amount']);

  $data = array(
    'AccountID' => variable_get('endicia_account_id', ''),
    'Password' => variable_get('endicia_passphrase', ''),
    'Amount' => $form_state['values']['amount']
  );

  $uri = _endicia_in_test_mode() ? ENDICIA_TEST_URI : ENDICIA_PRODUCTION_URI;
  $response_node = 'RecreditRequestResponse';
  $response = endicia_api_request($uri, 'BuyPostage', $data, $response_node);
  // Check for failures
  if ($response === FALSE) {
    _endicia_communication_error_redirect('admin/endicia/account/passphrase');
  }
  // Check for errors
  if ($error = endicia_api_request_error_check($response, $response_node)) {
    $message = 'Could not buy postage. Error #@errornum: %errormsg (check the site log for additional details).';
    $variables = array(
      '@errornum' => $error['status'],
      '%errormsg' => $error['message'],
    );
    drupal_set_message(t($message, $variables), 'error');
  }
  $balance = $response->RecreditRequestResponse->CertifiedIntermediary->PostageBalance;
  drupal_set_message(t('Your account has been credited successfully. Your updated account balance is $@balance', array('@balance' => $balance)));
  $form_state['redirect'] = 'admin/endicia';
}

/**
 * Lists all available presets in a table.
 */
function endicia_ui_label_preset_list() {
  $header = array(
    t('Preset name'),
    t('Mailpiece shape'),
    t('Class (dom.)'),
    t('Class (intl.)'),
    t('Operations'),
  );
  $rows = array();
  $presets = endicia_label_preset_get_all();
  $classes = _endicia_mail_classes();
  $shapes = _endicia_pkg_types('enabled');
  if ($presets) {
    foreach ($presets as $preset) {
      $edit_url = 'admin/endicia/label/presets/' . $preset['preset_id'] . '/edit';
      $reset_url = 'admin/endicia/label/presets/' . $preset['preset_id'] . '/delete';
      $linklist = array(
        'edit' => array('title' => t('Edit'), 'href' => $edit_url),
        'delete' => array('title' => t('Delete'), 'href' => $reset_url),
      );

      // Create the row array and add it to the list of all rows
      $row = array();
      $row[] = check_plain($preset['name']);
      $row[] = check_plain($shapes[$preset['mail_shape']]);
      $row[] = check_plain($classes[$preset['mail_class_domestic']]);
      $row[] = check_plain($classes[$preset['mail_class_international']]);
      $row[] = theme('links', array('links' => $linklist));
      $rows[] = $row;
    }
  }
  else {
    $rows[] = array(array('data' => t('No presets have been created.'), 'colspan' => count($header)));
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Builds the preset edit form. If $preset_id is set, then the form's initial
 * values are loaded from the existing definition. If it is set to NULL, then
 * the default values are used.
 */
function endicia_ui_label_preset_form($form, &$form_state, $preset = NULL) {
  if (is_null($preset)) {
    $preset = endicia_label_preset_new();
  }

  $form = array();

  $form['action'] = array(
    '#type' => 'hidden',
    '#value' => $preset['preset_id'] == -1 ? 'add' : 'update',
  );
  $form['preset_id'] = array(
    '#type' => 'hidden',
    '#value' => $preset['preset_id'],
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Preset name'),
    '#description' => t('Enter a name to identify this preset by.'),
    '#default_value' => $preset['name'],
    '#required' => TRUE,
  );
  $form['mail_shape'] = array(
    '#type' => 'select',
    '#title' => t('Mailpiece Shape'),
    '#description' => t('Select the mailpiece shape to use for this shipment.'),
    '#options' => _endicia_pkg_types('enabled'),
    '#required' => TRUE,
    '#default_value' => isset($preset['mail_shape']) ? $preset['mail_shape'] : 0,
  );
  $form['mail_class_domestic'] = array(
    '#type' => 'select',
    '#title' => t('Mail Class'),
    '#default_value' => isset($preset['mail_class_domestic']) ? $preset['mail_class_domestic'] : 0,
    '#options' => _endicia_mail_classes('Domestic'),
    '#required' => TRUE,
    '#description' => t('Select the mail class to use to ship packages with for domestic shipments. Keep in mind that package verification (e.g. by weight or size) is performed as you ship a package and the selected class may not necessarily be available for the package being shipped.'),
  );
  $form['mail_class_international'] = array(
    '#type' => 'select',
    '#title' => t('Mail Class'),
    '#default_value' => isset($preset['mail_class_international']) ? $preset['mail_class_international'] : 0,
    '#options' => _endicia_mail_classes('International'),
    '#required' => TRUE,
    '#description' => t('Select the mail class to use to ship packages with for international shipments. Keep in mind that package verification (e.g. by weight or size) is performed as you ship a package and the selected class may not necessarily be available for the package being shipped.'),
  );
  $form['dimensions'] = array(
    '#tree' => FALSE,
  );
  $form['dimensions']['length'] = array(
    '#type' => 'textfield',
    '#title' => t('Dimensions'),
    '#default_value' => $preset['length'],
    '#size' => 4,
    '#field_suffix' => 'x',
  );
  $form['dimensions']['width'] = array(
    '#type' => 'textfield',
    '#default_value' => $preset['width'],
    '#size' => 4,
    '#field_suffix' => 'x',
  );
  $form['dimensions']['height'] = array(
    '#type' => 'textfield',
    '#description' => t("Enter the package's dimensions (L x W x H) in inches."),
    '#default_value' => $preset['height'],
    '#size' => 4,
    '#field_suffix' => 'in.',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );

  return $form;
}

/**
 * Submit handler for endicia_label_preset_form().
 */
function endicia_ui_label_preset_form_submit($form, &$form_state) {
  // Create or load the definition depending on the current action
  if ($form_state['values']['action'] == 'add') {
    $preset = endicia_label_preset_new();
  }
  else {
    $preset = endicia_label_preset_load($form_state['values']['preset_id']);
  }

  $preset['preset_id'] = $form_state['values']['preset_id'];
  $preset['name'] = $form_state['values']['name'];
  $preset['mail_shape'] = $form_state['values']['mail_shape'];
  $preset['mail_class_domestic'] = $form_state['values']['mail_class_domestic'];
  $preset['mail_class_international'] = $form_state['values']['mail_class_international'];
  $preset['length'] = $form_state['values']['length'];
  $preset['width'] = $form_state['values']['width'];
  $preset['height'] = $form_state['values']['height'];
  drupal_alter('endicia_label_preset', $preset, $form_state['values']);

  // Save changes and redirect back to the definitions page
  endicia_label_preset_save($preset);
  $form_state['redirect'] = 'admin/endicia/label/presets';
}

/**
 * Build the reset confirmation form.
 *
 * A simple wrapper around Drupal's core confirm_form() function. Adds a value
 * field to store the ID of the role whose display settings are being reset.
 */
function endicia_ui_label_preset_delete_form($form, &$form_state, $preset) {
  $form['preset_id'] = array(
    '#type' => 'value',
    '#value' => $preset['preset_id'],
  );

  return confirm_form($form,
    t('Are you sure you want to delete the preset <em>@name</em>?', array('@name' => $preset['name'])),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/endicia/label/presets',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for endicia_label_preset_delete_form().
 */
function endicia_ui_label_preset_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $preset_id = $form_state['values']['preset_id'];
    $preset = endicia_label_preset_load($preset_id);
    endicia_label_preset_delete($preset_id);
    drupal_set_message(t('Label preset <em>@name</em> has been deleted.', array('@name' => $preset['name'])));
  }
  $form_state['redirect'] = 'admin/endicia/label/presets';
}

/**
 * Outputs the table of any previously saved labels
 */
function endicia_ui_label_archive_list() {
  drupal_add_css(drupal_get_path('module', 'endicia') . '/theme/endicia.css');
  $header = array(
    array('data' => t('Tracking number')),
    array('data' => t('Generation date'), 'field' => 'timestamp', 'sort' => 'desc'),
    array('data' => t('Module')),
    array('data' => t('Operations')),
  );
  $rows = array();
  $page_limit = 15;
  $labels = db_select('endicia_label_archive', 'la')
    ->fields('la')
    ->extend('TableSort')->orderByHeader($header)
    ->extend('PagerDefault')->limit($page_limit)
    ->execute()
    ->fetchAll();
  if ($labels) {
    foreach ($labels as $label) {
      $module_info = system_get_info('module', $label->module);
      $links = array();
      $file = NULL;
      if ($label->fid) {
        $file = file_load($label->fid);
        $links['print'] = array(
          'title' => t('Print'),
          'href' => 'endicia/print/' . file_uri_target($file->uri),
          'weight' => 0,
        );
      }
      $links['refund'] = array(
        'title' => t('Refund'),
        'href' => 'admin/endicia/account/refund/PICNumber/' . $label->tracking_num,
        'weight' => 1,
      );
      drupal_alter('endicia_label_archive_links', $links, $label, $file);
      uasort($links, 'drupal_sort_weight');

      if ($label->part_num == 0) {
        $title = $label->tracking_num;
      }
      else {
        $title = t('@tracking_num - Part @part_num', array('@tracking_num' => $label->tracking_num, '@part_num' => $label->part_num));
      }

      // Create the row array and add it to the list of all rows
      $row = array();
      $row[] = check_plain($title);
      $row[] = check_plain(format_date($label->timestamp));
      $row[] = check_plain(isset($module_info['name']) ? $module_info['name'] : $label->module);
      $row[] = theme('links', array('links' => $links));
      $rows[] = $row;
    }
  }
  else {
    $rows[] = array(array('data' => t('No labels have been printed yet.'), 'colspan' => count($header)));
  }
  $variables = array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(
      'class' => array('endicia-label-archive-list'),
    ),
  );
  $output = theme('table', $variables);

  $variables = array(
    'tags' => array(),
  );
  $output .= theme('pager', $variables);

  return $output;
}

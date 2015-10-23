<?php

/**
 * @file
 * Defines the administration UI for ideal connector.
 */

/**
 * Form constructor for the ideal advanced tests page.
 *
 * When a user returns from the ideal environment, the transaction status will
 * be shown.
 *
 * @see ideal_advanced_test_form_validate()
 * @see ideal_advanced_test_form_submit()
 *
 * @ingroup forms
 */
function ideal_advanced_test_form($form, &$form_state) {
  // Show the returned status of an ideal transaction when the entrance code
  // and the config_id session variable has been set.
  if (isset($_GET['ec'], $_SESSION['ideal_advanced_config_id'])) {
    $config_id = $_SESSION['ideal_advanced_config_id'];
    $connector = new IdealAdvancedConnectorWrapper($config_id);

    ideal_advanced_test_form_response($connector, check_plain($_GET['ec']));
  }

  $form['help'] = array(
    '#markup' => t("This form allows you to send simple test payments to your bank, which may be needed to activate iDEAL. Don't forget to switch the server URL setting between test/live, if necessary."),
  );
  $form['configuration'] = array(
    '#type' => 'select',
    '#title' => t('Select the iDEAL configuration'),
    '#required' => TRUE,
    '#options' => ideal_advanced_configurations(),
    '#ajax' => array(
      'callback' => 'ideal_advanced_test_form_callback',
      'wrapper' => 'ideal-configuration',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  $form['configuration_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('iDEAL configuration'),
    '#prefix' => '<div id="ideal-configuration">',
    '#suffix' => '</div>',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // Only show the setting fields when an ideal configuration
  // has been selected and the configuration is valid.
  // This is done with an Ajax request.
  if (isset($form_state['values']['configuration'])) {
    $_SESSION['ideal_advanced_config_id'] = $form_state['values']['configuration'];
    $config_handler = new IdealAdvancedConfigHandler();

    if ($config_handler->configValidate($form_state['values']['configuration'])) {
      $connector = new IdealAdvancedConnectorWrapper($form_state['values']['configuration']);

      $form['configuration_fieldset']['bank_details'] = array(
        '#type' => 'select',
        '#title' => t('Select your bank'),
        '#required' => TRUE,
        '#options' => $connector->getIssuers(),
      );
      $form['configuration_fieldset']['amount'] = array(
        '#type' => 'textfield',
        '#title' => t('Amount to send in cents'),
        '#required' => TRUE,
      );
      $form['configuration_fieldset']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Send'),
      );
    }
  }

  return $form;
}

/**
 * Form validation handler for ideal_advanced_test_form().
 *
 * @see ideal_advanced_test_form_submit()
 */
function ideal_advanced_test_form_validate($form, &$form_state) {
  if (isset($form_state['values']['amount']) && !is_numeric($form_state['values']['amount'])) {
    form_error($form['configuration_fieldset']['amount'], t('Amount must be a decimal number.'));
  }
}

/**
 * Ajax callback::show the configuration fieldset.
 *
 * @return array
 *   Configuration fieldset.
 */
function ideal_advanced_test_form_callback($form, $form_state) {
  return $form['configuration_fieldset'];
}

/**
 * Form submission handler for ideal_advanced_test_form().
 *
 * @see ideal_advanced_test_form_validate()
 */
function ideal_advanced_test_form_submit($form, &$form_state) {
  $config_handler = new IdealAdvancedConfigHandler();

  if ($config_handler->configValidate($form_state['values']['configuration'])) {
    $connector = new IdealAdvancedConnectorWrapper($form_state['values']['configuration']);
    $connector->setReturnUrl(url(current_path(), array('absolute' => TRUE)));
    $connector->setAmount($form_state['values']['amount']);
    $connector->setIssuerId($form_state['values']['bank_details']);
    $connector->setPurchaseId($form_state['values']['amount']);
    $connector->setEntranceCode(user_password(40));
    $connector->setDescription('test ' . $form_state['values']['amount']);

    $response = $connector->startTransaction();

    if ($response) {
      drupal_goto($response->getIssuerAuthenticationURL());
    }
    else {
      drupal_set_message(t('Could not connect to the Ideal issuer. See the <a href="/admin/reports/dblog">system log</a> for more details.'), 'error');
    }
  }
}

/**
 * Display the iDEAL transaction status to the site admin.
 *
 * @param IdealAdvancedConnectorWrapper $connector
 *   The ideal advanced connector class.
 * @param string $entrance_code
 *   Password for returning to site.
 */
function ideal_advanced_test_form_response(IdealAdvancedConnectorWrapper $connector, $entrance_code) {
  $query = db_query('SELECT ideal_id FROM {ideal_advanced_transaction} WHERE entrance_code = :entrance_code', array(':entrance_code' => $entrance_code));
  $result = $query->fetch();
  if (!empty($result)) {
    $transaction = ideal_advanced_transaction_load($result->ideal_id);
  }

  if (isset($transaction)) {
    $status = $connector->updateTransactionStatus($transaction->ideal_id);
    drupal_set_message(t('iDEAL returned the status: <strong>@status</strong> for purchase id: @purchase_id', array('@status' => $status->status, '@purchase_id' => $status->purchase_id)), 'status', FALSE);
  }
  else {
    drupal_set_message(t('The wrong entrance code is supplied'), 'error');
  }
}

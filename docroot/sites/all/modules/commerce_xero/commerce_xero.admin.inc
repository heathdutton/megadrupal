<?php
/**
 * @file
 *   Commerce Xero Admin page callabcks.
 */


/**
 * Commerce Xero Account Form.
 *
 * @param $form
 * @param &$form_state
 * @param $bank
 *   The Xero Bank Account object if it exists.
 * @return array
 */
function commerce_xero_account_form($form, &$form_state, $bank = NULL) {

  $bank_options = commerce_xero_bank_account_options();
  $rev_options = commerce_xero_revenue_account_options();
  $method_options = commerce_payment_method_options_list();
  $form_state['tracking_categories'] = commerce_xero_tracking_categories_options_list();

  if (empty($bank_options)) {
    drupal_set_message(t('Could not find any valid bank accounts associated with your Xero company. A bank account must have an account code in order to be used with bank transactions.'), 'error');
    return;
  }
  elseif (empty($rev_options)) {
    drupal_set_message(t('Could not find any revenue accounts associated with your Xero company.'), 'error');
    return;
  }
  elseif (empty($method_options)) {
    drupal_set_message(t('Could not find any Drupal Commerce payment methods.'), 'error');
    return;
  }

  $form['payment_method'] = array(
    '#type' => 'select',
    '#title' => t('Commerce payment method'),
    '#description' => t('Choose a payment method. You may only associate a payment method once.'),
    '#options' => $method_options,
    '#default_value' => isset($bank) ? $bank->payment_method : '',
    '#required' => TRUE,
    '#disabled' => isset($bank) ? TRUE : FALSE,
  );

  $form['bank_account'] = array(
    '#type' => 'select',
    '#title' => t('Xero bank account'),
    '#description' => t('Choose the bank account to associate to a payment method.'),
    '#options' => $bank_options,
    '#default_value' => isset($bank) ? $bank->bank_account : '',
    '#required' => TRUE,
  );

  $form['revenue_code'] = array(
    '#type' => 'select',
    '#title' => t('Xero account code'),
    '#description' => t('Choose the account code to post transactions to.'),
    '#options' => $rev_options,
    '#default_value' => isset($bank) ? $bank->revenue_code : '',
    '#required' => TRUE,
  );

  $form['transaction_type'] = array(
    '#type' => 'select',
    '#title' => t('Xero transaction type'),
    '#description' => t('Choose the type of transaction to make; either a bank transaction or invoice payments.'),
    '#options' => array('BankTransaction' => t('Bank transaction'), 'Payment' => t('Invoice payment')),
    '#default_value' => isset($bank) ? $bank->transaction_type : '',
  );

  $form['tracking_option'] = array(
    '#type' => 'select',
    '#title' => t('Xero tracking category'),
    '#description' => t('Choose the tracking category the transaction should be listed under.'),
    '#options' => $form_state['tracking_categories'],
    '#element_validate' => array('commerce_xero_account_tracking_option_validate'),
    '#default_value' => isset($bank) ? _commerce_xero_account_extract_tracking_value($form_state['tracking_categories'], $bank->tracking_option) : '',
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );

  if ($bank) {
    $form_state['account'] = $bank;

    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('commerce_xero_account_form_delete'),
      '#weight' => 42,
    );
  }

  return $form;
}

/**
 * Helper function to extract tracking category option key from tracking
 * categories array based on the the label.
 *
 * @param $categories
 *   Tracking categories.
 * @param $option
 *   The tracking option.
 * @return integer
 *   The key in the array.
 */
function _commerce_xero_account_extract_tracking_value($categories, $option) {
  return array_reduce($categories, function(&$result, $item) use ($option) {
    if (!empty($result) || !is_array($item)) {
      return $result;
    }

    $values = array_flip($item);
    if (isset($values[$option])) {
      $result = $values[$option];
    }

    return $result;
  }, '');
}

/**
 * Commerce Xero Account Tracking Category/Option Validate
 */
function commerce_xero_account_tracking_option_validate($element, &$form_state) {
  // Set the value for the tracking category by its respective tracking option.
  if (!empty($form_state['values']['tracking_option']) && $form_state['values']['tracking_option'] <> '_none') {
    $opt_value = $form_state['values']['tracking_option'];
    foreach ($form_state['tracking_categories'] as $category => $options) {
      if (is_array($options) && isset($options[$opt_value])) {
        form_set_value(array('#parents' => array('tracking_category')), $category, $form_state);
        form_set_value($element, $options[$opt_value], $form_state);
        break;
      }
    }
  }
}

/**
 * Commerce Xero Account Form Validate
 */
function commerce_xero_account_form_validate($form, &$form_state) {
  if (!isset($form_state['account'])) {
    $query = db_select('commerce_xero_bankaccount', 'xba');
    $query->condition('payment_method', $form_state['values']['payment_method']);
    $num = $query->countQuery()->execute()->fetchField();

    if ($num) {
      form_set_error('payment_method', t('This payment method is already associated with a bank account.'));
    }
  }
}

/**
 * Commerce Xero Account Form Delete
 */
function commerce_xero_account_form_delete($form, &$form_state) {
  $form_state['redirect'] = 'admin/commerce/config/xero/account/' . $form_state['account']->id . '/delete';
}

/**
 * Commerce Xero Account Form Submit.
 */
function commerce_xero_account_form_submit($form, &$form_state) {

  if (isset($form_state['account'])) {
    $bank = $form_state['account'];
  }
  else {
    $bank = (object) array(
      'payment_method' => '',
      'revenue_code' => '',
      'bank_account' => '',
      'transaction_type' => '',
      'tracking_category' => '',
      'tracking_option' => '',
    );
  }

  $bank->payment_method = $form_state['values']['payment_method'];
  $bank->revenue_code = $form_state['values']['revenue_code'];
  $bank->bank_account = $form_state['values']['bank_account'];
  $bank->transaction_type = $form_state['values']['transaction_type'];
  $bank->tracking_category = $form_state['values']['tracking_category'];
  $bank->tracking_option = $form_state['values']['tracking_option'];

  if (isset($bank->id)) {
    $ret = drupal_write_record('commerce_xero_bankaccount', $bank, array('id'));
  }
  else {
    $ret = drupal_write_record('commerce_xero_bankaccount', $bank);

    // Create field instance.
    $instance_info = array(
      'field_name' => 'field_xero_transaction',
      'label' => t('Xero transaction'),
      'entity_type' => 'commerce_payment_transaction',
      'bundle' => $bank->payment_method,
      'widget' => array(
        'type' => 'xero_textfield',
        'settings' => array(
          'xero_type' => array('BankTransaction', 'Payment'),
        ),
      ),
      'display' => array(
        'default' => array(
          'type' => 'xero_reference',
          'label' => 'inline',
          'settings' => array(
            'display' => array('type', 'guid'),
          ),
        ),
      ),
    );

    try {
      $instance = field_create_instance($instance_info);
    }
    catch (FieldException $e) {
      drupal_set_message(t('An error occurred creating a field on the Commerce Payment entity type. You may need to remove the bank acount and try again.'), 'error');
      watchdog_exception('commerce_xero', $e, 'An error occurred creating a Xero Reference field instance.');
    }
  }

  if ($ret) {
    drupal_set_message(t('Xero bank account %name successfully saved.', array('%name' => $bank->bank_account)));
    $form_state['redirect'] = 'admin/commerce/config/xero/account/' . $bank->id;
  }
  else {
    drupal_set_message(t('Failed to create Xero bank account.'), 'error');
  }

}

/**
 * Commerce Xero Bank Account Delete Form.
 */
function commerce_xero_account_delete_form($form, &$form_state, $bank) {

  $form['help'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('messages', 'warning'),
    ),
    'message' => array(
      '#markup' => t('Are you sure you want to permanently delete %name? This will not remove any created transactions.', array('%name' => $bank->bank_account)),
    ),
  );

  $form_state['account'] = $bank;

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Permanently remove'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/commerce/config/xero'),
  );

  return $form;
}

/**
 * Commerce Xero Bank Account Delete Form submit callback.
 */
function commerce_xero_account_delete_form_submit($form, &$form_state) {

  $instance = array(
    'field_name' => 'field_xero_transaction',
    'entity_type' => 'commerce_payment_transaction',
    'bundle' => $form_state['account']->bank_account,
  );
  field_delete_instance($instance);

  $num = db_delete('commerce_xero_bankaccount')->condition('id', $form_state['account']->id)->execute();

  if ($num) {
    drupal_set_message(t('Successfully deleted %name.', array('%name' => $form_state['account']->bank_account)));
    $form_state['redirect'] = 'admin/commerce/config/xero';
  }
  else {
    drupal_set_message(t('Unable to delete bank account!'), 'error');
  }
}

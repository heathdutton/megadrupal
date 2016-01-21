<?php

/**
 * @file
 * Admin callbacks for Manual payments module.
 */

/**
 * Return list of manual payments
 */
function pg_manual_payment_list($form, &$form_state) {

  $accounts = db_query('SELECT * FROM {pg_manual_payment_accounts} ORDER BY name')->fetchAllAssoc('paid', PDO::FETCH_ASSOC);

  $destination = drupal_get_destination();
  foreach ($accounts as $paid => $val) {
    $accounts[$paid]['ops'] = l(t('Edit'), 'admin/config/pgapi/manual_payment/edit/' . $paid, array('query' => $destination));
  }
  $header = array(
    'name'  => t('Account Name'),
    'purse' => t('Account Number'),
    'rate'  => t('Rate'),
    'ops'   => t('Operations'),
  );
  $form['accounts'] = array(
    '#type'    => 'tableselect',
    '#header'  => $header,
    '#options' => $accounts,
    '#empty'   => t('No accounts available.'),
  );

  // Add the buttons.
  $form['actions'] = array('#type' => 'actions');
  if (!empty($accounts)) {
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Delete checked Accounts'),
    );
  }
  return $form;
}

/**
 * Submit function for manual payment list
 *
 * @todo Add confirm form.
 */
function pg_manual_payment_list_submit($form, &$form_state) {
  $accounts = array_filter($form_state['values']['accounts']);
  if ($accounts) {
    db_delete('pg_manual_payment_accounts')
      ->condition('paid', array_keys($accounts), 'IN')
      ->execute();
    drupal_set_message(t('Accounts has been deleted.'));
  }
}


/**
 * Manual payment edit form
 */
function pg_manual_payment_edit($form, &$form_state, $paid) {
  $pursed = db_query('SELECT * FROM {pg_manual_payment_accounts} WHERE paid = :paid', array(':paid' => $paid))->fetchObject();
  $form['paid'] = array(
    '#type' => 'value',
    '#value' => $paid,
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $pursed->name,
    '#description' => t("Please, enter Payment system name."),
    '#required' => TRUE,
  );
  $form['purse'] = array(
    '#type' => 'textfield',
    '#title' => t('Account Number'),
    '#default_value' => $pursed->purse,
    '#description' => t("Please, enter Payment system account number."),
    '#required' => TRUE,
  );
  $form['symbol'] = array(
    '#type' => 'textfield',
    '#title' => t('Currency Symbol'),
    '#default_value' => $pursed->symbol,
    '#description' => t("Please, enter Payment system Currency symbol."),
    '#required' => TRUE,
  );
  $form['rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Currency rate'),
    '#default_value' => $pursed->rate,
    '#description' => t("Please, enter Payment system Currency rate."),
    '#required' => TRUE,
    '#element_validate' => array('pg_manual_payment_account_validate'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Rate element validator.
 */
function pg_manual_payment_account_validate($element, &$form_state) {
  if (!is_numeric($element['#value'])) {
    form_error($element, t('Numberic value required.'));
  }
}

/**
 * Submit function for manual payment edit form
 */
function pg_manual_payment_edit_submit($form, &$form_state) {
  drupal_write_record('pg_manual_payment_accounts', $form_state['values'], 'paid');
  drupal_set_message(t('Account %name has been modified.', array('%name' => $form_state['values']['name'])));
}

/**
 * Manual payment form for additing new payment
 */
function pg_manual_payment_add($form, &$form_state) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => '',
    '#description' => t("Please, enter Payment system name."),
    '#required' => TRUE,
  );
  $form['purse'] = array(
    '#type' => 'textfield',
    '#title' => t('Account Number'),
    '#default_value' => '',
    '#description' => t("Please, enter Payment system account number."),
    '#required' => TRUE,
  );
  $form['symbol'] = array(
    '#type' => 'textfield',
    '#title' => t('Currency Symbol'),
    '#default_value' => '',
    '#description' => t("Please, enter Payment system Currency symbol."),
    '#required' => TRUE,
  );
  $form['rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Currency rate'),
    '#default_value' => '',
    '#description' => t("Please, enter Payment system Currency rate."),
    '#required' => TRUE,
    '#element_validate' => array('pg_manual_payment_account_validate'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Account'),
  );
  return $form;
}

/**
 * Submit function for additing new manual payment.
 */
function pg_manual_payment_add_submit($form, &$form_state) {
  drupal_write_record('pg_manual_payment_accounts', $form_state['values']);
  drupal_set_message(t('Account %name has been added.', array('%name' => $form_state['values']['name'])));
  $form_state['redirect'] = 'admin/config/pgapi/manual_payment';
}

/**
 * Manual payment settings form
 */
function pg_manual_payment_settings($form, &$form_state) {
  $form['pg_manual_payment_email'] = array(
    '#type' => 'textfield',
    '#title' => t("Admin email with notification emails"),
    '#default_value' => variable_get('pg_manual_payment_email', ''),
    '#description' => t("Please enter email address for receive a notify and warning messages."),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

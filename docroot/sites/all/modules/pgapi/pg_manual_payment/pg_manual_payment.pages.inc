<?php

/*
 * @file
 * pg_manual_payment.pages.inc
 * Page callbacks for Manual payment module
 */

/**
 * Return manual payment form
 */
function pg_manual_payment_payform($form, &$form_state, $transaction) {
  global $user;
  if ($user->uid != 1 && $user->uid != $transaction->uid) {
    drupal_access_denied();
  }

  $sitename = variable_get('site_name', 'Drupal');
  $desc = t('Purchase for site !sitename. Transaction !txnid', array('!sitename' => $sitename, '!txnid' => $transaction->txnid));
  $account = db_query('SELECT * FROM {pg_manual_payment_accounts} WHERE paid = :paid', array(':paid' => $transaction->extra['wallet']))->fetchObject();

  $form['txnid'] = array(
    '#type' => 'value',
    '#value' => $transaction->txnid,
  );
  $form['payment'] = array(
    '#type' => 'fieldset',
    '#title' => t('Manual Payment'),
    '#description' => t('Please make a payment and click submit'),
  );
  $form['payment']['account'] = array(
    '#type' => 'textfield',
    '#title' => $account->name,
    '#value' => $account->purse,
    '#description' => t('Please send your payment on this wallet'),
  );
  $form['payment']['ammount'] = array(
    '#type' => 'textfield',
    '#title' => t('Ammount'),
    '#value' => $transaction->extra['prices'][$transaction->extra['wallet']],
    '#description' => t('Please send your payment on this wallet'),
  );
  $form['payment']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Payment description'),
    '#value' => $desc,
    '#description' => t('Please put this description'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Submit function for manual payment form
 */
function pg_manual_payment_payform_submit($form, &$form_state) {
  $txnid = $form_state['values']['txnid'];
  if ($txnid) {
    // Load transaction and user account
    $transaction = pgapi_transaction_load($txnid);
    $user        = user_load($transaction->uid);
    $account     = db_query('SELECT * FROM {pg_manual_payment_accounts} WHERE paid = :paid', array(':paid' => $transaction->extra['wallet']))->fetchObject();

    // Build mail params
    $params                   = array();
    $params['transaction']    = $transaction;
    $params['payment_method'] = $account;

    // Send mail to user
    drupal_mail('pg_manual_payment', 'notice', $transaction->email, user_preferred_language($user), $params);

    // Send mail to admin
    $admin_mail = trim(variable_get('pg_manual_payment_email', ''));
    if ($admin_mail) {
      drupal_mail('pg_manual_payment', 'notice_admin', $admin_mail, user_preferred_language($user), $params);
    }
    drupal_set_message(t('Your payment will be checked soon. You will receive email notification.'));
  }
}

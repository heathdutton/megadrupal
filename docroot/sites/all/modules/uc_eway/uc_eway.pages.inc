<?php
// $Id$

/**
 * @file
 * Includes page callbacks for user specific recurring fee operation pages.
 */

/**
 * Displays a form for customers to update their CC info.
 */
function uc_eway_token_update_form($form, &$form_state, $rfid) {
  global $user;
  $form = array();

  $fee = uc_recurring_fee_user_load($rfid);
  $order = uc_order_load($fee->order_id);

  $form['rfid'] = array(
    '#type' => 'value',
    '#value' => $rfid,
  );
  $form['cc_data'] = array(
    '#type' => 'fieldset',
    '#title' => t('Credit card details'),
    '#theme' => 'uc_payment_method_credit_form',
    '#tree' => TRUE,
  );
  $form['cc_data'] += uc_payment_method_credit_form(array(), $form_state, $order);
  unset($form['cc_data']['cc_policy']);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#suffix' => l(t('Cancel'), 'user/' . $user->uid),
  );

  return $form;
}

/**
 * Submit handler for the token update form.
 */
function uc_eway_token_update_form_submit($form, &$form_state) {
  $fee = uc_recurring_fee_user_load($form_state['values']['rfid']);
  $order = uc_order_load($fee->order_id);
  $order->payment_details = $form_state['values']['cc_data'];

  $result = uc_eway_token_update($order, $fee);

  // If the update was successful.
  if ($result) {
    drupal_set_message(t('The payment details for that recurring fee have been updated.'));
  }
  else {
    drupal_set_message(t('An error has occurred while updating your payment details. Please try again and contact us if you are unable to perform the update.'), 'error');
  }
}

/**
 *
 */
function uc_eway_customer_profile_form($form, &$form_state, $rfid) {
  $fee = uc_recurring_fee_user_load($rfid);
  //$order = uc_order_load($fee->order_id);
  $data = array(
    'managedCustomerID' => $fee->data['ManagedCustomerID'],
  );
  $response = _uc_eway_recurring_soap_call('QueryCustomer', $data);

  if (!empty($response['QueryCustomerResult'])) {
    $header = array();
    $row = array();
    foreach($response['QueryCustomerResult'] as $title => $item) {
      $header[] = $title;
      $row[] = $item;
    }
    $rows[] = $row;
    $form['customer'] = array(
      '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
    );
  } 
  $response = _uc_eway_recurring_soap_call('QueryPayment', $data);
  if (!empty($response['QueryPaymentResult'])) {
    $header = array();
    $rows = array();
    foreach($response['QueryPaymentResult']['ManagedTransaction'] as $index => $transaction) {
      $row = array();
      foreach($transaction as $title => $item) {
        $header[$title] = $title;
        $row[$title] = $item;
      }
      $rows[] = $row;
    }
    $form['payments'] = array(
      '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
    );
  } 

  return $form;
}


<?php

/**
 * @file
 * Definition of pay_method_custom_tripletex.
 */

/**
 * Payment method class that inherits pay_method_direct class defined by the Pay module
 * Methods available are:
 *
 * @author sten
 *
 */
class pay_method_custom_tripletex extends pay_method_custom {
  // Inherited properties
  // $pmid;      // Payment ID?
  // $title;     // Title (of payment=?)
  // $description; // Description
  // $min_amount;  // Minimum payment amount
  // $max_amount;  // Maximum payment amount
  // $pay_form_action = 'complete';// Default action is complete
  // $total;     // Total amount
  // $status = 1;// Payment Status.
  // $first_name;// Payers first name
  // $last_name; // Payers last name
  // $mail;      // Payers e-mail address
  // $billto = array();// Billing address
  // $table = 'pay_method';// Payment method table
  // $key = 'pmid';//

  // Local properties

  // Always assume this is a 'invoice' payment, since we won't get more specific.
  var $payment_type = 'invoice';

  /**
   * A list of all currencies supported by Tripletex.
   */
  function available_currencies() {
    return tripletex_currencies();
  }

  // Called when invoice is to be generated and submitted
  function authorize_action($variables) {
    dpm($variables, 'variables-authorize');
  }

  function complete_action($variables) {
    dpm($variables, 'variables-complete');
  }

  function valid_action($action, $transaction, $history) {
    // Subclasses can implement this method in order to qualify its result.
    dpm($action, 'valid_action-action');
    dpm($transaction, 'valid_action-transaction');
    dpm($history, 'valid_action-history');
    return TRUE;
  }

  function form_submit($form, &$form_state) {
    dpm($form, 'form_submit->form');
    dpm($form_state, 'form_submit->form_state');
  }

  // Add extra settings options when adding the payment method
  function settings_form(&$form, &$form_state) {
    parent::settings_form($form, $form_state);
    $group = $this->handler();

    // Add pending invoice payment option
    $form[$group]['pay_form_action']['#options']['authorize'] = t('Collect payment information and await payment status update from Tripletex.  Set the transaction to "Pending".');

    $form[$group]['pp']['#type'] = 'fieldset';
    $form[$group]['pp']['#collapsible'] = FALSE;
    $form[$group]['pp']['#title'] = t('Tripletex Standard settings');
    $form[$group]['pp']['#group'] = $group;

    $form[$group]['pp']['tripletex_account'] = array(
      '#type' => 'textfield',
      '#title' => t('Tripletex Account e-mail address'),
      '#default_value' => variable_get('tripletex_user', 'json@test.no'),
      '#required' => TRUE,
      '#parents' => array($group, 'tripletex_account'),
    );
    $form[$group]['pp']['tripletex_password'] = array(
      '#type' => 'password',
      '#title' => t('Tripletex password'),
      '#default_value' => variable_get('tripletex_pass', 'pwd'),
      '#required' => TRUE,
      '#parents' => array($group, 'tripletex_password'),
    );
    // TODO: Add password and standard settings
    /*
     $form[$group]['pp']['tripletex_sandbox'] = array(
     '#type' => 'checkbox',
     '#title' => t('This is a sandbox test account'),
     '#description' => t('You can obtain a developer test account from https://www.x.com (The PayPal Develper Network) and use those credentials for development and testing. If the above values belong to a developer test account and not a real merchant account, check this box.'),
     '#default_value' => $this->tripletex_sandbox,
     '#parents' => array($group, 'tripletex_sandbox'),
     );*/
  }
}

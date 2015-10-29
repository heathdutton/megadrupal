<?php

/**
 * @file
 *   Provides page callbacks for menu
 */

/**
 * Page callback for payment/% page
 *
 * Return form with payment methods
 *
 * @param  $form_state
 *   State of form
 * @param  $transaction
 *   Current transaction
 * @return form
 */
function pgapi_payment($form, &$form_state, $transaction) {
  global $user;

  // If someone tries to view not own page before transaction status is pending or workflow is recieved - show access denied page
  if (($user->uid != 1 && $user->uid != $transaction->uid) || ($transaction->status != PG_PENDING) || ($transaction->workflow != PG_WORKFLOW_RECEIVED)) {
    drupal_access_denied();
  }

  // Get array with enabled payment systems
  $pgapi_gw = pgapi_get_enabled_payment_systems();
  // Default gateway could be chosen withing ajax callback.
  $default  = isset($form_state['values']['gateway']) ? $form_state['values']['gateway'] : '';
  $options  = array();

  foreach ($pgapi_gw as $module => $checked) {
    if ($checked) {
      // Get data about payment gateway from hook_pgapi_gw()
      $payment_data = module_invoke($module, 'pgapi_gw', 'payment gateway info', $transaction->amount, $transaction);
      // Should contain name and price keys.
      if ($payment_data) {
        // Choose default gateway.
        if (empty($default)) {
          if ($transaction->gateway == $module) {
            $default = $transaction->gateway;
          }
          else {
            $default = $module;
          }
        }
        $form[$module]['modulename'] = array(
          '#markup' => $payment_data['name'],
        );
        $options[$module] = '';

        $form[$module]['price'] = array(
          '#markup' => theme('pgapi_gateway_price', array('prices' => $payment_data['price'], 'service' => $transaction->service)),
        );
      }
    }

    $form['gateway'] = array(
      '#type' => 'radios',
      '#options' => $options,
      '#default_value' => $default,
      '#ajax' => array(
        'callback' => 'pgapi_payment_ajax',
        'wrapper' => 'pgapi-wrapper-form',
        'method' => 'replace',
        'effect' => 'slide',
      ),
    );
  }

  $form['pgapi_wrapper'] = array(
    '#prefix' => '<div id="pgapi-wrapper-form">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );

  // If checked default payment system - load special form for it
  if ($default) {
    $extra_form = module_invoke($default, 'pgapi_gw', 'get form', $transaction, $form_state);
    if (is_array($extra_form)) {
      $form['pgapi_wrapper'] += $extra_form;
    }
  }

  $form['#tree'] = TRUE;

  $form['txnid'] = array(
    '#type' => 'value',
    '#value' => $transaction->txnid,
  );

  // Add the buttons.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Process purchase'),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel purchase'),
  );

  return $form;
}

/**
 * Returns subform elements for the gateway.
 */
function pgapi_payment_ajax($form, $form_state) {
  return $form['pgapi_wrapper'];
}

/**
 * Submit function for payment form
 * Process transaction and redirect to payment system page
 */
function pgapi_payment_submit($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['txnid']) {
    // Load transaction by ID
    $transaction = pgapi_transaction_load($values['txnid']);
    if ($values['op'] == $values['actions']['submit']) {
      // If user clicked "Submit" button - process transcaction
      $transaction = module_invoke($values['gateway'], 'pgapi_gw', 'process form', $transaction, $values['pgapi_wrapper']);
      $form_state['redirect'] = pgapi_get_redirect_url($transaction);
    }
    elseif ($values['op'] == $values['actions']['cancel']) {
      // If user clicked "Cancel" button - cancel transaction
      $transaction->status = PG_CANCELED;
      $transaction->workflow = PG_WORKFLOW_CANCELED;
      $transaction->description = t('User has canceled payment');
      pgapi_transaction_save($transaction);
      $form_state['redirect'] = pgapi_get_redirect_url($transaction);
    }
  }
}

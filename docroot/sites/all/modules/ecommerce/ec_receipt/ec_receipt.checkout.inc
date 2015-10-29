<?php


/**
 * @file
 * Provides checkout implementation for ec_receipt.
 */

/**
 * Implements hook_checkout_init().
 */
function ec_receipt_checkout_init(&$txn) {
  $txn->currency = variable_get('ec_default_currency', NULL);

  // since we are running from ec_checkout_create_transaction() we do not need to do any receipting
  if (isset($txn->no_interface) && $txn->no_interface) {
    return;
  }

  if (empty($txn->receipts['rtypes'])) {
    $default_filter = array(
      'currencies_supported' => $txn->currency,
      'allow_payments' => TRUE,
    );

    if (isset($txn->receipts['payment_filter'])) {
      $filter = $txn->receipts['payment_filter'];
      $filter += $default_filter;
    }
    else {
      $filter = $default_filter;
    }
    $txn->receipts['rtypes'] = ec_receipt_type_filter($filter, 'transaction', $txn);
  }

  if (isset($_REQUEST['payment_method']) && isset($txn->receipts['rtypes'][$_REQUEST['payment_method']])) {
    $txn->payment_method = $_REQUEST['payment_method'];
  }
  else {
    $types = array_keys($txn->receipts['rtypes']);
    $txn->payment_method = empty($types) ? '' : $types[0];
  }

  if ($function = ec_receipt_get_invoke_function($txn->payment_method, 'init')) {
    $function('transaction', $txn);
  }
}

/**
 * Implements hook_checkout_form().
 */
function ec_receipt_checkout_form(&$form, &$form_state) {
  $txn = & $form_state['txn'];

  $form['ec_receipt'] = array(
    '#prefix' => '<div class="receipt-review">',
    '#suffix' => '</div>',
    '#theme' => 'ec_receipt_review_form',
  );

  $form['ec_receipt']['receipt_no'] = array(
    '#type' => 'value',
    '#value' => '',
  );

  if (ec_store_transaction_calc_gross($txn)) {
    if (count($txn->receipts['rtypes']) > 1) {
      $options = array();

      foreach ($txn->receipts['rtypes'] as $type => $info) {
        $options[$type] = $info->name;
      }
      
      // This maybe called by the select list for the which ec_receipt_checkout_submit_rtype_js() will not get called. So we need to do this here.
      if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#name'] = 'select_payment_type') {
        $txn->payment_method = $form_state['values']['select_payment_type'];
      }

      $form['ec_receipt']['buttons'] = array(
        '#prefix' => '<div id="payment-buttons" class="ec-inline-form">',
        '#suffix' => '</div>',
        '#weight' => -9,
      );
      $form['ec_receipt']['buttons']['select_payment_type'] = array(
        '#type' => 'select',
        '#title' => t('Select payment method'),
        '#default_value' => $txn->payment_method,
        '#options' => $options,
        '#access' => count($options) >= 1,
        '#ajax' => array(
          'callback' => 'ec_receipt_checkout_submit_rtype_js',
          'wrapper' => 'ec-receipt-form',
          'method' => 'replace',
        ),
      );
      $form['ec_receipt']['buttons']['receipt_submit'] = array(
        '#type' => 'submit',
        '#value' => t('Select payment method'),
        '#access' => count($options) >= 1,
        '#submit' => array('ec_receipt_checkout_alter_rtype'),
        '#ajax' => array(
          'callback' => 'ec_receipt_checkout_submit_rtype_js',
          'wrapper' => 'ec-receipt-form',
          'method' => 'replace',
        ),
      );
    }

    $info = ec_receipt_get_types('type', $txn->payment_method);
    $form['ec_receipt']['ec_receipt_name'] = array(
      '#markup' => ec_receipt_checkout_types($info),
      '#weight' => -8,
      '#prefix' => '<div class="rtype-' . str_replace(array('][', '_', ' '), '-', $txn->payment_method) . '">',
      '#suffix' => '</div>',
    );
    if (isset($info->attached)) {
      $form['ec_receipt']['ec_receipt_name']['#attached'] = $info->attached;
    }
    if ($pform = ec_receipt_invoke($txn->payment_method, 'payment_form', 'transaction', $txn)) {
      $form['ec_receipt']['payment_form'] = array(
        '#tree' => TRUE,
      );
      $form['ec_receipt']['payment_form'] += $pform;
    }
    $form['ec_receipt']['#theme'] = 'ec_receipt_review_form';
  }
}

/**
 * Implements hook_checkout_update().
 */
function ec_receipt_checkout_update(&$form, &$form_state) {
  $txn = & $form_state['txn'];

  $txn->payment_data = isset($form_state['values']['payment_form']) ? $form_state['values']['payment_form'] : array();
}

/**
 * Implements hook_checkout_post_update().
 */
function ec_receipt_checkout_post_update(&$form, &$form_state) {
  $txn = & $form_state['txn'];
  if (isset($form_state['clicked_button']['#create_txn']) && $form_state['clicked_button']['#create_txn'] && !form_get_errors() && ec_store_transaction_calc_gross($txn)) {
    $txn->receipt_no = ec_receipt_payment_process('transaction', $txn);
  }
}


/**
 * Handle changing of receipt type.
 */
function ec_receipt_checkout_alter_rtype(&$form, &$form_state) {
  ctools_include('object-cache');
  
  $txn = & $form_state['txn'];
  $txn->payment_method = $form_state['values']['select_payment_type'];
  $form_state['rebuild'] = TRUE;
  
  ctools_object_cache_set('transaction', 'ec_checkout', $txn);
}

/**
 * AHAH callback to change the payment method.
 */
function ec_receipt_checkout_submit_rtype_js(&$form, &$form_state) {
  return $form['ec_receipt'];
}

/**
 * Implements hook_checkout_post_submit().
 */
function ec_receipt_checkout_post_submit(&$txn, &$form_state) {
  $receipt = NULL;

  if (ec_store_transaction_calc_gross($txn)) {
    $alloc[] = array(
      'type' => 'transaction',
      'id' => $txn->txnid,
    );
    $info = ec_receipt_get_types('type', $txn->payment_method);
    if (empty($txn->receipt_no) || (isset($info->shared_gateway) && $info->shared_gateway)) {
      return ec_receipt_payment_goto('transaction', $txn, $alloc, $txn->receipt_no);
    }
    else {
      if ($receipt = ec_receipt_load($txn->receipt_no)) {
        ec_receipt_allocate($receipt, $alloc);
      }
    }
    if (empty($form_state['redirect'])) {
      return ec_receipt_return_page($receipt);
    }
  }
}

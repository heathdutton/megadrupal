<?php

/**
 * @file
 * All allocation functions for ec_store.
 */

/**
 * Get the email from the invoice.
 */
function ec_store_alloc_get_mail($txn) {
  return isset($txn->mail) ? $txn->mail : ec_customer_get_email($txn->ecid);
}

/**
 * Get the billing or shipping address from a transaction.
 *
 * @param &$txn
 *   Object. The transaction object.
 * @param $atype
 *   String (Optional). What type of address is required:
 *   billing (default) or shipping.
 * @return
 *   Array. The address object converted to an array.
 */
function ec_store_alloc_get_address($txn, $atype = 'billing') {
  if (!empty($txn->address[$atype])) {
    return (array) $txn->address[$atype];
  }
}

/**
 * Get all customer data from a transaction.
 *
 * @param &$txn
 *   Object. The transaction object.
 * @return
 *   Object. The customer object.
 */
function ec_store_alloc_get_customer($txn) {
  return $txn->customer;
}

/**
 * Get the customer name from a given transaction.
 * If it was not previously defined into first and last names, it must be
 * extracted thru Regex.
 *
 * @param &$txn
 *   Object. The transaction object.
 * @param $atype
 *   String (Optional). What type of address is required:
 *   billing (default) or shipping.
 * @return
 *   Array. fname for First Name and lname for Last Name.
 */
function ec_store_alloc_get_customer_names($txn, $atype = 'billing') {
  if ($address = (array)$txn->address[$atype]) {
    $names = array();
    if (!empty($address['firstname']) or !empty($address['lastname'])) {
      $names['fname'] = $address['firstname'];
      $names['lname'] = $address['lastname'];
    }

    if (empty($names['fname']) && empty($name['lname']) && !empty($address['fullname'])) {
      $names = ec_common_split_name($address['fullname']);
    }
    return !empty($names) ? $names : NULL;
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_alloc_get_currency($txn) {
  return $txn->currency;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_alloc_get_invoice_no($txn) {
  return $txn->txnid;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_alloc_get_items($txn) {
  if (!empty($txn->items) && is_array($txn->items)) {
    ec_store_alloc_build_items($txn, 'init');
    $items = array_map('ec_store_alloc_build_items', $txn->items);
    return $items;
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_alloc_allocation($txn, $alloc, $balance) {
  $is_shippable = FALSE;

  if (!empty($txn->items)) {
    foreach ($txn->items as $item) {
      if (ec_product_is_shippable($item->vid)) {
        $is_shippable = TRUE;
        break;
      }
    }
  }

  if ($balance == 0) {
    $txn->allocation = EC_ALLOC_COMPLETED;
  }
  else {
    $txn->allocation = EC_ALLOC_PART;
  }

  $txn->allocated += $alloc['amount'];
  $txn->balance -= $alloc['amount'];

  ec_store_transaction_save($txn);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_alloc_build_items($item, $op = 'process') {
  static $txn;
  if ($op == 'init') {
    $txn = $item;
    return;
  }

  return array(
    'id' => $item->nid,
    'name' => $item->title,
    'qty' => $item->qty,
    'amount' => ec_store_adjust_misc($txn, $item),
  );
}

/**
 * Implements hook_alloc_get_payment_data().
 */
function ec_store_alloc_get_payment_data($txn) {
  return isset($txn->payment_data) ? $txn->payment_data : NULL;
}

/**
 * Implements hook_alloc_get_payment_form().
 */
function ec_store_alloc_get_payment_form($txn) {
  return isset($txn->payment_form) ? $txn->payment_form : NULL;
}

/**
 * Implements hook_alloc_get_payment_type().
 */
function ec_store_alloc_get_payment_type($txn) {
  return $txn->payment_method;
}

/**
 * Implements hook_alloc_get_total().
 */
function ec_store_alloc_get_total($txn) {
  return ec_store_transaction_calc_gross($txn);
}

/**
 * Implements hook_alloc_load().
 */
function ec_store_alloc_load($txnid) {
  return ec_store_transaction_load($txnid);
}

/**
 * Implements hook_alloc_set_payment_data().
 */
function ec_store_alloc_set_payment_data($txn, $payment_data) {
  $txn->payment_data[$txn->payment_method] = $payment_data;
}

/**
 * Implements hook_alloc_get_shipping().
 */
function ec_store_alloc_get_shipping($txn) {
	if (!empty($txn->misc)) {
    ec_store_filter_misc(array('misc_group' => 'shipping'), 'init');
    $items = array_filter($txn->misc, 'ec_store_filter_misc');
    return array_sum(array_map('ec_store_map_price', $items));
  }
  return 0;
}

/**
 * Implements hook_alloc_get_handling().
 */
function ec_store_alloc_get_handling($txn) {
  if (!empty($txn->misc)) {
    ec_store_filter_misc(array('misc_group' => 'handling'), 'init');
    $items = array_filter($txn->misc, 'ec_store_filter_misc');
    return array_sum(array_map('ec_store_map_price', $items));
  }
  return 0;
}

/**
 * Implements hook_alloc_get_handling().
 */
function ec_store_alloc_get_tax($txn) {
  if (!empty($txn->misc)) {
    ec_store_filter_misc(array('misc_group' => 'tax'), 'init');
    $items = array_filter($txn->misc, 'ec_store_filter_misc');
    return array_sum(array_map('ec_store_map_price', $items));
  }
  return 0;
}

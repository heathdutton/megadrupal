<?php
/**
 * @file
 */

/**
 * Implements hook_token_info().
 */
function ec_store_token_info() {
  dpm(__FUNCTION__);
  
  $types = array();
  $types['txn'] = array(
    'name' => t('Transaction'),
    'description' => t('Tokens related to transactions'),
    'needs-data' => 'txn',
  );
  
  $types['address'] = array(
    'name' => t('Address'),
    'description' => t('Address tokens'),
    'needs-data' => 'address',
  );
  
  $txn = array();
  $txn['id'] = array(
    'name' => t('ID'),
    'description' => t('Transaction Id.'),
  );
  $txn['subtotal'] = array(
    'name' => t('Subtotal'),
    'description' => t('Subtotal of items; excluding shipping, tax and other misc charges.'),
  );
  $txn['total'] = array(
    'name' => t('Total'),
    'description' => t('Transaction total.'),
  );
  $txn['balance'] = array(
    'name' => t('Transaction balance'),
    'description' => t('Current balance of the transaction.')
  );
  $txn['created'] = array(
    'name' => t('Order Date'),
    'description' => t('Date the order/transaction was created.'),
    'type' => 'date',
  );
  $txn['changed'] = array(
    'name' => t('Order update'),
    'description' => t('Date the transaction was last modified.'),
    'type' => 'date',
  );
  $txn['workflow'] = array(
    'name' => t('Workflow'),
    'description' => t('Current state of the transaction.')
  );
  $txn['shipping-cost'] = array(
    'name' => t('Shipping cost'),
    'description' => t('Cost of shipping.'),
  );
  $txn['handling-cost'] = array(
    'name' => t('Handling cost'),
    'description' => t('Cost of handling.'),
  );
  $txn['receipt'] = array(
    'name' => t('Receipt'),
    'description' => t('Current receipt for the transaction.'),
    'type' => 'receipt',
  );
  $txn['billing'] = array(
    'name' => t('Billing address'),
    'description' => t('Billing address details'),
    'type' => 'address',
  );
  $txn['shipping'] = array(
    'name' => t('Shipping address'),
    'description' => t('Shipping address details'),
    'type' => 'address',
  );
  $txn['items'] = array(
    'name' => t('Items'),
    'description' => t('List of purchased items.'),
  );
        
  $address['firstname'] = array(
    'name' => t('First name'),
    'description' => t('First name of the customer.'),
  );
  $address['lastname'] = array(
    'name' => t('Last name'),
    'description' => t('Surname of the customer.'),
  );
  $address['street'] = array(
    'name' => t('Street'),
    'description' => t('Street address'),
  );
  $address['additional'] = array(
    'name' => t('Additional'),
    'description' => t('Second line of the street address.')
  );
  $address['zip'] = array(
    'name' => t('Zip/Postal code'),
    'description' => t('Zip/Postal code for the address'),
  );
  $address['city'] = array(
    'name' => t('City'),
    'description' => t('City for the address'),
  );
  $address['state'] = array(
    'name' => t('State/Province'),
    'description' => t('State/Province for the address'),
  );
  $address['country'] = array(
    'name' => t('Country'),
    'description' => t('Country for the address'),
  );
    
  return array(
    'types' => $types,
    'tokens' => array(
      'txn' => $txn,
      'address' => $address,
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function ec_store_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $url_options = array('absolute' => TRUE);
  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);
  $replacements = array();
  
  if ($type == 'txn' && !empty($data['txn'])) {
    $txn = $data['txn'];
    
    $items = "";
    $subtotal = 0;
    if (!empty($txn->items)) {
      foreach ($txn->items as $p) {
        $subtotal += $p->qty * $p->price;
        $items .= t('@order of @title at @price each', array(
          '@order' => format_plural($p->qty, '1 order', '@count orders'),
          '@title' => $p->title,
          '@price' => format_currency($p->price),
        )) . "\n";
      }
    }
    
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'id':
          $replacements[$original] = $txn->txnid;
          break;
          
        case 'subtotal':
          $replacements[$original] = format_currency($subtotal, $txn->currency);
          break;
          
        case 'total':
          $replacements[$original] = format_currency($txn->gross, $txn->currency);
          break;
          
        case 'balance':
          $replacements[$original] = format_currency($txn->balance, $txn->currency);
          break;
          
        case 'created':
          $replacements[$original] = format_date($txn->created, 'medium', '', NULL, $language_code);
          break;
          
        case 'changed':
          $replacements[$original] = format_date($txn->changed, 'medium', '', NULL, $language_code);
          break;
          
        case 'workflow':
          $replacements[$original] = ec_store_transaction_get_workflow($txn->workflow);
          break;
          
        case 'shipping-cost':
          module_load_include('inc', 'ec_store', 'ec_store.alloc');
          $replacements[$original] = format_currency(ec_store_alloc_get_shipping($txn), $txn->currency);
          break;
          
        case 'handling-cost':
          module_load_include('inc', 'ec_store', 'ec_store.alloc');
          $replacements[$original] = format_currency(ec_store_alloc_get_handling($txn), $txn->currency);
          break;
        
        case 'billing':
          $replacements[$original] = theme('formatted_address', array('address' => $txn->address['billing']));
          break;

        case 'shipping':
          $replacements[$original] = theme('formatted_address', array('address' => $txn->address['shipping']));
          break;

        case 'items':
          $replacements[$original] = $items;
          break;
      }
      
      if ($created_tokens = token_find_with_prefix($tokens, 'created')) {
        $replacements += token_generate('date', $created_tokens, array('date' => $txn->created), $options);
      }
      
      if ($changed_tokens = token_find_with_prefix($tokens, 'changed')) {
        $replacements += token_generate('date', $changed_tokens, array('date' => $txn->changed), $options);
      }
      
      if ($receipt_tokens = token_find_with_prefix($tokens, 'receipt')) {
        if ($erid = db_query("SELECT erid FROM ec_receipt_allocation WHERE type = 'transaction' AND reversed = 0 AND etid = :txnid ORDER BY created ASC", array(':txnid' => $txn->txnid))->fetchColumn()) {
          $receipt = ec_receipt_load($erid);
        }
        if (!empty($receipt)) {
          $replacements += token_generate('receipt', $receipt_tokens, array('receipt' => $receipt), $options);
        }
      }
      
      if ($billing_tokens = token_find_with_prefix($tokens, 'billing')) {
        $replacements += token_generate('address', $billing_tokens, array('address' => $txn->address['billing']), $options);
      }
      
      if ($shipping_tokens = token_find_with_prefix($tokens, 'shipping')) {
        $replacements += token_generate('address', $shipping_tokens, array('address' => $txn->address['shipping']), $options);
      }
    }
  }
  
  if ($type == 'address' && !empty($data['address'])) {
    $address = $data['address'];
    
    $info = ec_store_get_region_info($address['country'], array('use_state_names', 'states'));
    
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'firstname':
          $replacements[$original] = $sanitize ? check_plain($address['firstname']) : $address['firstname'];
          break;

        case 'lastname':
          $replacements[$original] = $sanitize ? check_plain($address['lastname']) : $address['lastname'];
          break;
          
        case 'street':
          $replacements[$original] = $sanitize ? check_plain($address['street']) : $address['street'];
          break;
          
        case 'additional':
          $replacements[$original] = $sanitize ? check_plain($address['additional']) : $address['additional'];
          break;
          
        case 'zip':
          $replacements[$original] = $sanitize ? check_plain($address['zip']) : $address['zip'];
          break;
          
        case 'city':
          $replacements[$original] = $sanitize ? check_plain($address['city']) : $address['city'];
          break;
          
        case 'state':
          $replacements[$original] = ($info['use_state_names'] && isset($info['states'][$address['state']])) ? $info['states'][$address['state']] : $address['state'];
          break;
          
        case 'country':
          $replacements[$original] = ec_store_get_country($address['country']);
          break;
      }
    }
  }
  
  return $replacements;
}

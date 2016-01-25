<?php
/**
 * @file
 */

/**
 * Implements hook_token_info().
 */
function ec_receipt_token_info() {
  $types = array();
  
  $types['receipt'] = array(
    'name' => t('Receipts'),
    'description' => t('Tokens related to individual receipts'),
    'needs-data' => 'receipt',
  );
  
  $receipt = array();
  $receipt['id'] = array(
    'name' => t('Id'),
    'description' => t('Receipt id')
  );
  $receipt['type'] = array(
    'name' => t('Payment method'),
    'description' => t('Payment method of the receipt.'),
  );
  $receipt['currency'] = array(
    'name' => t('Currency'),
    'description' => t('Currency which this receipt uses.'),
  );
  $receipt['amount'] = array(
    'name' => t('Amount'),
    'description' => t('Amount of the receipt.'),
  );
  $receipt['allocated'] = array(
    'name' => t('Allocated'),
    'description' => t('Amount of the receipt which has already been allocated.'),
  );
  $receipt['balance'] = array(
    'name' => t('Balance'),
    'description' => t('Balance of the transaction.'),
  );
  $receipt['status'] = array(
    'name' => t('Status'),
    'description' => t('Status of the receipt'),
  );
  $receipt['created'] = array(
    'name' => t('Created'),
    'description' => t('Date the receipt was created.'),
  );
  $receipt['changed'] = array(
    'name' => t('Changed'),
    'description' => t('Date the receipt was last changed.')
  );
  $receipt['response'] = array(
    'name' => t('Response text'),
    'description' => t('Response text from external payment gates when the receipt was processed.'),
  );
  $receipt['txn'] = array(
    'name' => t('Transaction'),
    'description' => t('Transaction which the receipt is allocated to.'),
    'type' => 'txn',
  );
  
  return array(
    'types' => $types,
    'tokens' => array(
      'receipt' => $receipt,
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function ec_receipt_tokens($type, $tokens, array $data = array(), array $options = array()) {
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
  
  if ($type == 'receipt' && !empty($data['receipt'])) {
    $receipt = $data['receipt'];
    
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'id':
          $replacements[$original] = $receipt->erid;
          break;
          
        case 'type':
          $replacements[$original] = ec_receipt_get_types($name, $receipt->type);
          break;
          
        case 'currency':
          $replacements[$original] = $receipt->currency;
          break;
          
        case 'amount':
          $replacements[$original] = format_currency($receipt->amount, $receipt->currency);
          break;
          
        case 'allocated':
          $replacements[$original] = format_currency($receipt->allocated, $receipt->currency);
          break;
          
        case 'balance':
          $replacements[$original] = format_currency($receipt->allocated, $receipt->currency);
          break;
          
        case 'status':
          $replacements[$original] = ec_receipt_get_status($receipt->status);
          break;
          
        case 'created':
          $replacements[$original] = format_date($receipt->created, 'medium', '', NULL, $language_code);
          break;
          
        case 'changed':
          $replacements[$original] = format_date($receipt->changed, 'medium', '', NULL, $language_code);
          break;
          
        case 'response':
          $replacements[$original] = $sanitize ? check_plain($receipt->response_text) : $receipt->response_text;
          break;
      }
      
      if ($txn_tokens = token_find_with_prefix($tokens, 'txn')) {
        if (!$allocations = ec_receipt_get_allocations($receipt->erid)) {
          $allocations = $receipt->allocation;
        }
        $first_allocation = reset($allocations);
        $etid = isset($first_allocation['etid']) ? $first_allocation['etid'] : $first_allocation['id'];
        if ($first_allocation['type'] == 'transaction' && $etid) {
          $object = ec_receipt_alloc_invoke($first_allocation['type'], 'load', $etid);
        }
        if (!empty($object)) {
          $replacements += token_generate('txn', $txn_tokens, array('txn' => $object), $options);
        }
      }

      if ($created_tokens = token_find_with_prefix($tokens, 'created')) {
        $replacements += token_generate('date', $created_tokens, array('date' => $receipt->created), $options);
      }
      
      if ($created_tokens = token_find_with_prefix($tokens, 'changed')) {
        $replacements += token_generate('date', $created_tokens, array('date' => $receipt->changed), $options);
      }
    }
  }
  
  return $replacements;
}
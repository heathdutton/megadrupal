<?php

/**
 * @file
 * Rules supporting AvaTax Sales Order Processing.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_avatax_rules_action_info() {
  $parameter = array(
    'order' => array(
      'type' => 'commerce_order',
      'label' => t('Commerce Order'),
    ),
  );
  $actions = array(
    'commerce_avatax_calculate_sales_tax' => array(
      'label' => t('Calculate sales tax for order'),
      'group' => t('Commerce AvaTax'),
      'parameter' => $parameter,
    ),
    'commerce_avatax_delete_avatax_line_items' => array(
      'label' => t('Delete AvaTax line items'),
      'group' => t('Commerce AvaTax'),
      'parameter' => $parameter,
    ),
    'commerce_avatax_commit_transaction' => array(
      'label' => t('Change status of sales tax to COMMITTED in AvaTax'),
      'group' => t('Commerce AvaTax'),
      'parameter' => $parameter,
    ),
    'commerce_avatax_cancel_transaction' => array(
      'label' => t('Change status of sales tax to VOIDED in AvaTax'),
      'group' => t('Commerce AvaTax'),
      'parameter' => $parameter,
    ),
  );

  return $actions;
}

/**
 * DELETE AvaTax line item for $order.
 */
function commerce_avatax_delete_avatax_line_items($order) {
  _commerce_avatax_update($order, 'delete');
}

/**
 * COMMIT AvaTax transaction for $order.
 */
function commerce_avatax_commit_transaction($order) {
  _commerce_avatax_update($order, 'commit');
}

/**
 * VOID AvaTax transaction for $order.
 */
function commerce_avatax_cancel_transaction($order) {
  _commerce_avatax_update($order, 'cancel');
}

/**
 * Send Commit/Cancel operation to AvaTax.
 */
function _commerce_avatax_update($order, $type = 'commit') {
  // Get Company code and Company Use Mode.
  $product_version = variable_get('commerce_avatax_product_version');
  $use_mode = variable_get('commerce_avatax_use_mode');
  $company_code = variable_get('commerce_avatax_' . $product_version . '_' . $use_mode . '_company');
  $doc_code_prefix = 'dc';

  if (!commerce_avatax_check_address($order, $product_version)) {
    return;
  }
  switch ($type) {
    case 'delete':
      commerce_avatax_delete_avatax_transaction($order);
      break;

    case 'commit':
      commerce_avatax_retrieve_sales_tax($order, $product_version, TRUE);
      break;

    case 'cancel':
      $body = array(
        'Client' => 'DrupalCommerce-CommerceGuys,4.3',
        'DocCode' => $doc_code_prefix . '-' . $order->order_id,
        'CompanyCode' => $company_code,
        'DocType' => 'SalesInvoice',
        'CancelCode' => 'DocVoided',
      );
      $response = commerce_avatax_post('/tax/cancel', $body);
      if (is_array($response) && $response['body']) {
        $result = $response['body'];
        if (isset($result['CancelTaxResult']['ResultCode']) && (isset($result['CancelTaxResult']['Messages'])) && $result['CancelTaxResult']['ResultCode'] != 'Success') {
          foreach ($result['CancelTaxResult']['Messages'] as $msg) {
            drupal_set_message(t('AvaTax error: %msg - %source - %details - %summary', array(
              '%msg' => $msg['Severity'],
              '%source' => $msg['Source'],
              '%details' => $msg['Details'],
              '%summary' => $msg['Summary'],
              )), 'error');
          }
          watchdog('commerce_avatax', 'Failed to void order @id !req !resp', array(
            '@id' => $order->order_id,
            '!req' => '<pre>' . var_export($body, TRUE) . '</pre>',
            '!resp' => '<pre>' . check_plain(var_export($body, TRUE)) . '</pre>',
            ), WATCHDOG_ERROR);
          break;
        }
      }

      if (!$response) {
        drupal_set_message(t("AvaTax did not get a response."), 'error');
        watchdog('commerce_avatax', "Failed to void order @id - AvaTax did not respond.", array('@id' => $order->order_id), WATCHDOG_ERROR);
        return;
      }
      break;
  }
}

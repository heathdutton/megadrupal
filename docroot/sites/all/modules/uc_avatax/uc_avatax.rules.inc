<?php

/**
 * @file
 * Rules supporting AvaTax Sales Order Processing.
 */

/**
 * Implements hook_rules_action_info().
 */
function uc_avatax_rules_action_info() {
  $parameter = array(
    'order' => array(
      'type' => 'uc_order',
      'label' => t('Order'),
    ),
  );
  $actions = array(
    'uc_avatax_commit_transaction' => array(
      'label' => t('Change status of sales tax to COMMITTED in AvaTax'),
      'group' => t('AvaTax'),
      'parameter' => $parameter,
    ),
    'uc_avatax_cancel_transaction' => array(
      'label' => t('Change status of sales tax to CANCELLED in AvaTax'),
      'group' => t('AvaTax'),
      'parameter' => $parameter,
    ),
  );

  return $actions;
}

/**
 * COMMIT AvaTax transaction for $order.
 */
function uc_avatax_commit_transaction($order) {
  _uc_avatax_update($order, 'commit');
}

/**
 * DELETE AvaTax transaction for $order.
 */
function uc_avatax_cancel_transaction($order) {
  _uc_avatax_update($order, 'cancel');
}

/**
 * Send Commit/Cancel operation to AvaTax.
 */
function _uc_avatax_update($order, $type = 'commit') {
  // Get Company code and Company Use Mode.
  $company_code = (variable_get('uc_avatax_company_code', ''));
  $use_mode = (variable_get('uc_avatax_use_mode', ''));
  $product_version = variable_get('uc_avatax_product_version', 'trial');
  $account_no = variable_get('uc_avatax_' . $product_version . '_' . $use_mode . '_account');
  $license_key = variable_get('uc_avatax_' . $product_version . '_' . $use_mode . '_license');

  if ($use_mode == 'Development') {
    $base_url = 'https://development.avalara.net/1.0';
  }
  elseif ($use_mode == 'Production') {
    $base_url = 'https://rest.avalara.net/1.0';
  }

  $doc_code_prefix = 'uc';
  if ($product_version == 'trial') {
    $company_code = 'avatax-calc';
    $doc_code_prefix = variable_get('uc_avatax_install_time', microtime(TRUE));
  }


  $order_wrapper = entity_metadata_wrapper('uc_tax_order', $order);
  if (!uc_avatax_check_address($order)) {
    return;
  }
  switch ($type) {
    case 'commit':
      uc_avatax_retrieve_sales_tax($order, TRUE);
      break;

    case 'cancel':
      $body = array(
        'Client' => 'Ubercart3x-adTumblerInc,4.0',
        'DocCode' => $doc_code_prefix . '-' . $order->order_id,
        'CompanyCode' => $company_code,
        'DocType' => 'SalesInvoice',
        'CancelCode' => 'DocVoided',
      );
      $response = uc_avatax_post('/tax/cancel', $body);
      if (is_array($response) && $response['body']) {
        $result = $response['body'];
        if (isset($result['CancelTaxResult']['ResultCode']) && $result['CancelTaxResult']['ResultCode'] != 'Success') {
          foreach ($result['CancelTaxResult']['Messages'] as $msg) {
            drupal_set_message(t('AvaTax error: %msg - %source - %details - %summary', array(
              '%msg' => $msg['Severity'],
              '%source' => $msg['Source'],
              '%details' => $msg['Details'],
              '%summary' => $msg['Summary'],
                    )), 'error');
          }
          watchdog('uc_avatax', 'Failed to void order @id !req !resp', array(
            '@id' => $order->order_id,
            '!req' => '<pre>' . var_export($body, TRUE) . '</pre>',
            '!resp' => '<pre>' . var_export($response, TRUE) . '</pre>',
            ), WATCHDOG_ERROR);
          return;
        }
      }
      
      if (!$response) {
        drupal_set_message(t("AvaTax did not get a response."), 'error');
        watchdog('uc_avatax', "Failed to void order @id - AvaTax did not respond.", array('@id' => $order->order_id), WATCHDOG_ERROR);
        return;
      }
      break;
  }
}

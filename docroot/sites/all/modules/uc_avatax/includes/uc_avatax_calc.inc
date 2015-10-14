<?php

/**
 * @file
 * Handles request to AvaTax for sales tax values.
 */

/**
 * Makes a request to Avatax for sales tax values.
 *
 * @param object $order
 *   The current order object.
 *
 * @param array $ava_args
 *   An array containing order delivery details.
 *
 * @return bool|array
 *   FALSE if the tax calculation failed.
 *   An array containing tax amount, taxable amount, total order amount.
 */
function uc_avatax_get_tax($order, $ava_args) {
  if ($order->order_id == 0) {
    $doc_type = "SalesOrder";
  }
  else {
    $doc_type = "SalesInvoice";
  }
  
  $request_body = array(
    'Client' => 'Ubercart3x-adTumblerInc,4.0',
    'CompanyCode' => $ava_args['company_code'],
    'DetailLevel' => 'Tax',
    'Commit' => $ava_args['commit'],
    'DocType' => $doc_type,
    'DocCode' => $ava_args['doc_code_prefix'] . '-' . $order->order_id . '',
    'DocDate' => date("Y-m-d", $ava_args['doc_date']),
    'CustomerCode' => $ava_args['user_id'],
    'CustomerUsageType' => $ava_args['avatax_exemption_code'],
    'Addresses' => array(
      // Origin.
      array(
        'AddressCode' => 0,
        'Line1' => $ava_args['primary_street1'],
        'Line2' => $ava_args['primary_street2'],
        'City' => $ava_args['primary_city'],
        'Region' => $ava_args['primary_state'],
        'PostalCode' => $ava_args['primary_zip'],
      ),
      // Destination.
      array(
        'AddressCode' => 1,
        'Line1' => $ava_args['street1'],
        'Line2' => $ava_args['street2'],
        'City' => $ava_args['city'],
        'Region' => $ava_args['state'],
        'Country' => $ava_args['country'],
        'PostalCode' => $ava_args['zip'],
      ),
    ),
  );

  $i = 1;
  foreach ($order->products as $prod) {
    $tax_code = '';
    if ($ava_args['product_version'] == UC_AVATAX_PRO_VERSION) {
      if (isset($prod->field_avatax_code)) {
        $avatax_array = $prod->field_avatax_code;
        if ($avatax_array) {       
          $tid = $avatax_array['und'][0]['tid'];
          $taxonomy_term = taxonomy_term_load($tid);
          $tax_code = $taxonomy_term->name;
        }
      }
      else {
        $product = node_load($prod->nid);
        if (isset($product->field_avatax_code)) {
          $avatax_array = $product->field_avatax_code;
          if ($avatax_array) {  
            $tid = $avatax_array['und'][0]['tid'];
            $taxonomy_term = taxonomy_term_load($tid);
            $tax_code = $taxonomy_term->name;
          }
        }
      } 
    }
    $lines[] = array(
      'LineNo' => $i,
      'ItemCode' => $prod->model,
      'Description' => $prod->title,
      'TaxCode' => $tax_code,
      'Qty' => $prod->qty,
      'Amount' => $prod->qty * $prod->price,
      'Discounted' => 'false',
      'Ref1' => '',
      'Ref2' => '',
      'CustomerUsageType' => '',
      'OriginCode' => 0,
      'DestinationCode' => 1,
      );
    $i++;
  }

  foreach ($order->line_items as $key => $item) {
    if (in_array($item['type'], array('coupon'))) {
      $lines[] = array(
        'LineNo' => $i,
        'ItemCode' => $item['type'],
        'Description' => $item['title'],
        'TaxCode' => '',
        'Qty' => 1,
        'Amount' => $item['amount'],
        'Discounted' => 'false',
        'Ref1' => '',
        'Ref2' => '',
        'CustomerUsageType' => '',
        'OriginCode' => 0,
        'DestinationCode' => 1,
      );
      $i++;
    }
	
	elseif (in_array($item['type'], array('shipping'))) {
      $lines[] = array(
        'LineNo' => $i,
        'ItemCode' => $item['type'],
        'Description' => $item['title'],
        'TaxCode' => $ava_args['shipcode'],
        'Qty' => 1,
        'Amount' => $item['amount'],
        'Discounted' => 'false',
        'Ref1' => '',
        'Ref2' => '',
        'CustomerUsageType' => '',
        'OriginCode' => 0,
        'DestinationCode' => 1,
      );
      $i++;
    }
	
    elseif (in_array($item['type'], array('generic'))) {
	  $lines[] = array(
        'LineNo' => $i,
        'ItemCode' => $item['type'],
        'Description' => $item['title'],
        'TaxCode' => '',
        'Qty' => 1,
        'Amount' => $item['amount'],
        'Discounted' => 'false',
        'Ref1' => '',
        'Ref2' => '',
        'CustomerUsageType' => '',
        'OriginCode' => 0,
        'DestinationCode' => 1,
      );
      $i++;
    }
  }

  $request_body['Lines'] = $lines;
    
  $response = uc_avatax_post('/tax/get', $request_body);
  if (is_array($response) && $response['body']) {
    $result = $response['body'];
    if ($result['ResultCode'] == 'Success') {
      $tax_data = array(
        'tax_amount' => $result['TotalTax'],
        'taxable_amount' => $result['TotalTaxable'],
        'total_amount' => $result['TotalAmount'],
      );
      return $tax_data;
    }
    else {
      foreach ($result['Messages'] as $msg) {
        drupal_set_message(t('AvaTax error: %msg - %source - %summary', array(
          '%msg' => $msg['Severity'],
          '%source' => $msg['Source'],
          '%summary' => $msg['Summary'],
                )), 'error');
      }
      return FALSE;
    }
  }

  if (!$response) {
    drupal_set_message(t("AvaTax did not get any response."), 'error');
    return FALSE;
  }
}
  
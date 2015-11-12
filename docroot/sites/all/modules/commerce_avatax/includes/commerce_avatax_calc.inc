<?php

/**
 * @file
 * AvaTax GetTax amount.
 */

/**
 * Gets the tax amount for the order based on the delivery address.
 *
 * @param object $order
 *   The current order object.
 * @param object $order_wrapper
 *   The current order entitywrapper object.
 * @param array $ava_args
 *   An array containing from & to delivery details.
 *
 * @return array
 *   An array containing the AvaTax request result
 *   or FALSE if the tax calculation failed.
 */
function commerce_avatax_get_tax($order, $order_wrapper, $ava_args = array()) {
  $request_body = array(
    'Client' => 'DrupalCommerce-CommerceGuys,4.3',
    'CompanyCode' => $ava_args['company_code'],
    'DetailLevel' => 'Tax',
    'Commit' => $ava_args['commit'],
    'CurrencyCode' => $ava_args['currency_code'],
    'DocType' => 'SalesInvoice',
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
        'Country' => $ava_args['primary_country'],
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

  if (module_exists('commerce_avatax_exemptions')) {
    if (isset($order->data['commerce_avatax_exemptions_category'])
            && $order->data['commerce_avatax_exemptions_category']) {
      $request_body['CustomerUsageType'] = $order->data['commerce_avatax_exemptions_category'];
    }
  }

  $i = 1;
  foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
    $line_item = $line_item_wrapper->value();
    $tax_code = '';
    if (in_array($line_item->type, commerce_product_line_item_types())) {
      if ($ava_args['product_version'] == COMMERCE_AVATAX_PRO_VERSION) {
        $tax_field_name = 'avatax_code';
        $product_field = field_get_items('commerce_line_item', $line_item, 'commerce_product');
        $product_id = $product_field[0]['product_id'];
        $prod_data = commerce_product_load($product_id);
        $avatax_code_field_value = field_get_items('commerce_product', $prod_data, $tax_field_name);
        if ($avatax_code_field_value) {
          $tid = $avatax_code_field_value[0]['tid'];
          $taxonomy_term = taxonomy_term_load($tid);
          $tax_code = $taxonomy_term->name;
        }
      }
      $avatax_line_item_label = substr($line_item->line_item_label, 0, 49);
      $lines[] = array(
        'LineNo' => $i,
        'ItemCode' => $avatax_line_item_label,
        'Description' => $line_item_wrapper->commerce_product->title->value(),
        'TaxCode' => $tax_code,
        'Qty' => $line_item->quantity,
        'Amount' => ($line_item_wrapper->commerce_unit_price->amount->value() / 100) * $line_item->quantity,
        'Discounted' => 'true',
        'Ref1' => '',
        'Ref2' => '',
        'CustomerUsageType' => '',
        'OriginCode' => 0,
        'DestinationCode' => 1,
      );
      $i++;
    }
    elseif (in_array($line_item->type, array('shipping'))) {
      $lines[] = array(
        'LineNo' => $i,
        'ItemCode' => 'Shipping',
        'Description' => 'Shipping',
        'TaxCode' => $ava_args['shipcode'],
        'Qty' => $line_item->quantity,
        'Amount' => ($line_item_wrapper->commerce_unit_price->amount->value() / 100) * $line_item->quantity,
        'Discounted' => 'false',
        'Ref1' => '',
        'Ref2' => '',
        'CustomerUsageType' => '',
        'OriginCode' => 0,
        'DestinationCode' => 1,
      );
      $i++;
    }
    elseif (in_array($line_item->type, array('commerce_coupon'))) {
      $lines[] = array(
        'LineNo' => $i,
        'ItemCode' => 'Coupon',
        'Description' => 'Coupon Amt',
        'TaxCode' => '0D010000',
        'Qty' => $line_item->quantity,
        'Amount' => ($line_item_wrapper->commerce_unit_price->amount->value() / 100) * $line_item->quantity,
        'Discounted' => 'false',
        'Ref1' => '',
        'Ref2' => '',
        'CustomerUsageType' => '',
        'OriginCode' => 0,
        'DestinationCode' => 1,
      );
      $i++;
    }
    elseif (in_array($line_item->type, array('commerce_discount'))) {
      $request_body['Discount'] = (($line_item_wrapper->commerce_unit_price->amount->value() / 100) * $line_item->quantity) * -1;
    }
  }

  $request_body['Lines'] = $lines;

  $response = commerce_avatax_post('/tax/get', $request_body);
  if (is_array($response) && $response['body']) {
    $ava_result = $response['body'];
    if ($ava_result['ResultCode'] == 'Success') {
      return $ava_result;
    }
    else {
      foreach ($ava_result['Messages'] as $msg) {
        drupal_set_message(t('AvaTax error: %msg - %source - %summary', array(
          '%msg' => $msg['Severity'],
          '%source' => $msg['Source'],
          '%summary' => $msg['Summary'],
                )), 'error');
      }
      return FALSE;
    }
  }
  else {
    drupal_set_message(t("AvaTax did not get a response."), 'error');
    return FALSE;
  }
}

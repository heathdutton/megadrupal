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
function uc_taxrates_get_tax($order, $ava_args) {
  // Array of states that require separately identified shipping to be taxed
  $shipping_states = array('AR','CT','DC','GA','HI','IN','KS','KY','MI','MN','MS','NE','NJ','NM','NY','NC','ND','OH','PA','SC','SD','TN','TX','VT','WA','WV','WY');

  $taxrate = 0;
  $n = 0;
  if (file_exists($ava_args['module_dir'] . '/taxtables/TAXRATES_ZIP5_' . $ava_args['state'] . '' . $ava_args['tax_table_date'] . '.csv')) {
    $file = fopen($ava_args['module_dir'] . '/taxtables/TAXRATES_ZIP5_' . $ava_args['state'] . '' . $ava_args['tax_table_date'] . '.csv', 'r');
    $data = array();
    while($row = fgetcsv($file)) {
      $data[] = $row;
      if ($data[$n][1] == $ava_args['zip']) {
        $tax_rate = $data[$n][4];
        break;
      }
      $n ++;
    }
    fclose($file);
    if (!$tax_rate) {
      drupal_set_message(t('TaxRates error: %msg', array('%msg' => 'The zip code entered - ' . $ava_args['zip'] . ' - was not found in the TaxRate.com table', )), 'error');
      return FALSE;
    }
  }
  else {
    drupal_set_message(t('TaxRates error: %msg', array('%msg' => 'The module could not match - state ' . $ava_args['state'] . ' - with a valid table from TaxRate.com', )), 'error');
    return FALSE;
  } 
  
  $taxable_amount = 0;
  $total_amount = 0;   
  foreach ($order->products as $prod) {
    $line_value = $prod->qty * $prod->price;
    $taxable_amount = $taxable_amount + $line_value;
    $total_amount = $total_amount + $line_value;
  }
  
  foreach ($order->line_items as $key => $item) {
    if (in_array($item['type'], array('coupon'))) {
      $taxable_amount = $taxable_amount + $item['amount'];
      $total_amount = $total_amount + $item['amount'];
    }
	elseif (in_array($item['type'], array('shipping'))) {
      // Include shipping line in taxable amount for states that include shipping
      if (in_array($ava_args['state'], $shipping_states)) {
        $taxable_amount = $taxable_amount + $item['amount'];
      }
    $total_amount = $total_amount + $item['amount'];
    }
	elseif (in_array($item['type'], array('generic'))) {
      // Include generic line in taxable amount for states that include shipping
      $taxable_amount = $taxable_amount + $item['amount'];
      $total_amount = $total_amount + $item['amount'];
    }
  }

  // Calculate sales tax - rate * total taxable amount
  $sales_tax = $taxable_amount*$tax_rate;

  // Return sales tax data
  $tax_data = array(
    'tax_amount' => $sales_tax,
    'taxable_amount' => $taxable_amount,
    'total_amount' => $total_amount + $sales_tax,
    );
  return $tax_data;
}

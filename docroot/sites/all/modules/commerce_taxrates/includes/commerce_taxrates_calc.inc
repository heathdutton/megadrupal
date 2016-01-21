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
 *
 * @param object $order_wrapper
 *   The current order entitywrapper object.
 *
 * @param array $ava_args
 *   An array containing from & to delivery details.
 *
 * @return array
 *   An array containing tax amount, taxable amount, total order amount
 *   or FALSE if the tax calculation failed.
 */
function commerce_taxrates_get_tax($order, $order_wrapper, $ava_args) {
  // Array of states that require shipping by public carrier to be taxed
  $shipping_states = array('AR','CT','DC','GA','HI','IN','KS','KY','MI','MN','MS','NE','NJ','NM','NY','NC','ND','OH','PA','SC','SD','TN','TX','VT','WA','WV','WY');

  $tax_rate = 0;
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
    $taxrate_file = 'TAXRATES_ZIP5_' . $ava_args['state'] . '' . $ava_args['tax_table_date'] . '.csv';
    drupal_set_message(t('TaxRates error: %msg', array('%msg' => 'Could not load rates data from file - ' . $taxrate_file . ' - please configure module or install TaxRates table', )), 'error');
    return FALSE;
  } 
  
  $taxable_amount = 0;
  $total_amount = 0;
  foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
    $line_item = $line_item_wrapper->value();
    $tax_code = '';
    if (in_array($line_item->type, commerce_product_line_item_types())) {
      if ($ava_args['product_version'] == COMMERCE_TAXRATES_PRO_VERSION) {
        $tax_field_name = 'taxrates_code';
        $product_field = field_get_items('commerce_line_item', $line_item, 'commerce_product');
        $product_id = $product_field[0]['product_id'];
        $prod_data = commerce_product_load($product_id);
        $taxrates_code_field_value = field_get_items('commerce_product', $prod_data, $tax_field_name);
        if ($taxrates_code_field_value) {
          $tid = $taxrates_code_field_value[0]['tid'];
          $taxonomy_term = taxonomy_term_load($tid);
          $tax_code = $taxonomy_term->name;
        }
      }
      $line_value = ($line_item_wrapper->commerce_unit_price->amount->value() / 100) * $line_item->quantity;
      if ($tax_code == 'P0000000' || $tax_code == '') {
        $taxable_amount = $taxable_amount + $line_value;
      } 
      $total_amount = $total_amount + $line_value;
    }
    elseif (in_array($line_item->type, array('shipping'))) {
      // Include shipping line in taxable amount for states that include shipping
      $line_value = ($line_item_wrapper->commerce_unit_price->amount->value() / 100) * $line_item->quantity;
      if (in_array($ava_args['state'], $shipping_states)) {
        $taxable_amount = $taxable_amount + $line_value;
      }
      $total_amount = $total_amount + $line_value;
    }
    elseif (in_array($line_item->type, array('commerce_coupon'))) {
      $line_value = ($line_item_wrapper->commerce_unit_price->amount->value() / 100) * $line_item->quantity;
      $taxable_amount = $taxable_amount + $line_value;
      $total_amount = $total_amount + $line_value;
    }
    elseif (in_array($line_item->type, array('commerce_discount'))) {
      $line_value = ($line_item_wrapper->commerce_unit_price->amount->value() / 100) * $line_item->quantity;
      $taxable_amount = $taxable_amount + $line_value;
      $total_amount = $total_amount + $line_value;
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

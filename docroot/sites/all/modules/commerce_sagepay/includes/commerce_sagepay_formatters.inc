<?php
/**
 * @file
 * commerce_sagepay_formatters.inc
 *
 * Common functions for all payment methods to format the cart for SagePay.
 */

/**
 * Convert a Commerce Order object to XML using the V3 protocol format.
 *
 * @param commerce_order $order
 *      The order to convert to XML.
 *
 * @return DOMDocument
 *      The XML document.
 */
function _commerce_sagepay_cart_to_xml($order) {

  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Define a new XML document object to store the order data.
  $xml_doc = new DOMDocument();

  $root = $xml_doc->appendChild(
    $xml_doc->createElement("basket")
  );

  $order_lines = $order->commerce_line_items['und'];

  foreach ($order_lines as $line) {

    $line_item = commerce_line_item_load($line['line_item_id']);
    $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

    // Create an XML element for each order line.
    $linexml = $root->appendChild(
      $xml_doc->createElement('item')
    );

    $linexml->appendChild(
      $xml_doc->createElement('description', $line_item->line_item_label)
    );

    $linexml->appendChild(
      $xml_doc->createElement('quantity', number_format($line_item->quantity,
        0))
    );

    $total_items += $line_item->quantity;

    $item_value = $line_item->commerce_unit_price['und'][0]['data']['components'][0]['price']['amount'] / 100;

    // Step through components to look for tax item.
    $item_tax = 0;
    $tax_type = '';
    
    foreach ($line_item->commerce_unit_price['und'][0]['data']['components'] as $c) {
      if (strpos($c['name'], 'tax') === 0) {
        $item_tax += $c['price']['amount'] / 100;
        $tax_type = $c['price']['data']['tax_rate']['type'];

      }
    }
    if ($tax_type == 'vat') {
      $item_total = $line_item_wrapper->commerce_unit_price->amount->value() / 100;
    } else {
      $item_total = $line_item_wrapper->commerce_unit_price->amount
        ->value() / 100 + $item_tax;
    }

    $arr_line_total = commerce_price_component_total($line_item_wrapper->commerce_total->value());
    $line_total = $arr_line_total['amount'] / 100;

    $linexml->appendChild(
      $xml_doc->createElement('unitNetAmount', number_format
      ($item_value,  2, '.', ''))
    );

    $linexml->appendChild(
      $xml_doc->createElement('unitTaxAmount', number_format
      ($item_tax,  2, '.', ''))
    );

    $linexml->appendChild(
      $xml_doc->createElement('unitGrossAmount', number_format
      ($item_total, 2, '.', ''))
    );

    $linexml->appendChild(
      $xml_doc->createElement('totalGrossAmount',
        number_format($line_total, 2, '.', ''))
    );

  }

  /*
  * The remainder of this specification is industry specific.
  * Adding a hook here so the XML can be enhanced.
  */
  drupal_alter('commerce_sagepay_basket_xml', $xml_doc);

  $xml_doc->formatOutput = TRUE;

  return $xml_doc;
}

/**
 * Convert a an Order object into a string.
 *
 * In the format required by SagePay protocol 3.0.
 *
 * @param commerce_order $order
 *    the order to be converted.
 *
 * @return string
 *    the order as a string.
 */
function _commerce_sagepay_cart_to_string($order) {
  $order_string = '';

  // Load an array of line items from the order.
  $order_lines = $order->commerce_line_items['und'];

  // Determine the total number of lines - this is the first component
  // of the string we send to SagePay.
  $total_lines = count($order_lines);
  $order_string = $total_lines . ':';

  // Encode each order line and add to the string.
  for ($line = 0; $line < $total_lines; $line++) {
    $order_string .= _commerce_sagepay_line_item_to_string($order_lines[$line]['line_item_id']);
  }

  // Trim off the last excess colon.
  $order_string = substr($order_string, 0, strlen($order_string) - 1);

  return $order_string;
}

/**
 * Convert a single order line into a string in the format requried by SagePay.
 *
 * @param int $line_item_id
 *     The id of the line item to be converted.
 *
 * @return string
 *     The line item as a string.
 */
function _commerce_sagepay_line_item_to_string($line_item_id) {

  // Load the line item ID.
  $line_item = commerce_line_item_load($line_item_id);
  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

  $description = $line_item_wrapper->line_item_label->value();

  // Remove any colons from the line desc as we need this as a delimiter.
  $description = str_ireplace(':', ' ', $description);

  $quantity = $line_item_wrapper->quantity->value();

  // Get the price components for the order line.
  $item_value = $line_item_wrapper->commerce_unit_price->value();

  // Step through components to look for tax item.
  $line_data = $line_item_wrapper->commerce_unit_price->value();

  // Set default value for the tax component in case the site is not using tax.
  $item_tax = array('amount' => 0, 'currency_code' => $item_value['currency_code']);
  
  $tax_type = '';
  
  foreach ($line_data['data']['components'] as $c) {
    if (strpos($c['name'], 'tax') === 0) {
      $item_tax = commerce_price_component_total($item_value, $c['name']);
      $tax_type = $c['price']['data']['tax_rate']['type'];
    }
  }

  $total = $line_item_wrapper->commerce_total->value();

  $fl_item_total = commerce_currency_amount_to_decimal($item_value['amount'], $item_value['currency_code']);
  $fl_tax_value = commerce_currency_amount_to_decimal($item_tax['amount'], $item_tax['currency_code']);
  if ($tax_type == 'vat') {
    $fl_item_value = $fl_item_total - $fl_tax_value;
    $fl_total_value = $fl_item_total * $quantity;
  } else {
    $fl_item_value = $fl_item_total;
    $fl_item_total += $fl_tax_value;
    $fl_total_value = $fl_item_total * $quantity;
  }

  $line_as_text = $description . ':' . $quantity . ':' . number_format($fl_item_value, 2) . ':' . number_format($fl_tax_value, 2) . ':' . number_format($fl_item_total, 2) . ':' . number_format($fl_total_value, 2) . ':';

  return $line_as_text;
}

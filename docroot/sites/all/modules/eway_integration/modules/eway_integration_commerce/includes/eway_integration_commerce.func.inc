<?php

/**
 * @file
 * Provide functions for module.
 */

/**
 * Prepare payments array.
 * @param $payment_method
 * @param $pane_values
 * @param $order
 * @param $charge
 * @return array
 */
function eway_integration_commerce_prepare_payments($pane_values, $order, $charge) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  // Prepare the billing address for use in the request.
  if (isset($order_wrapper->commerce_customer_billing->commerce_customer_address)) {
    $billing_address = $order_wrapper->commerce_customer_billing->commerce_customer_address->value();
  }
  else {
    $billing_address = array(
      'name_line' => '',
      'first_name' => '',
      'last_name' => '',
      'organisation_name' => '',
      'thoroughfare' => '',
      'premise' => '',
      'locality' => '',
      'administrative_area' => '',
      'postal_code' => '',
      'country' => 'au',
    );
  }

  // Prepare the shipping address for use in the request.
  if (isset($order_wrapper->commerce_customer_shipping)) {
    $shipping_address = $order_wrapper->commerce_customer_shipping->commerce_customer_address->value();
  }
  else {
    $shipping_address = $billing_address;
  }

  // Allow modules to alter parameters of the API request.
  $context = array(
    'order_wrapper' => $order_wrapper,
  );
  drupal_alter('eway_integration_billing_address', $billing_address, $context);
  // Allow modules to alter parameters of the API request.
  drupal_alter('eway_integration_shipping_address', $shipping_address, $context);

  // Build a description for the order.
  $description = array();
  $items = array();
  foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
    if (in_array($line_item_wrapper->type->value(), commerce_product_line_item_types())) {
      $unit_price = $line_item_wrapper->commerce_unit_price->value();
      $items[] = array(
        "SKU" => $line_item_wrapper->commerce_product->sku->value(),
        "Description" => $line_item_wrapper->line_item_label->value(),
        "Quantity" => round($line_item_wrapper->quantity->value(), 2),
        "UnitCost" => $unit_price['amount'],
        "Tax" => '',
        "Total" => round(round($line_item_wrapper->quantity->value(), 2) * $unit_price['amount'], 2),
      );
      $description[] = round($line_item_wrapper->quantity->value(), 2) . 'x ' . $line_item_wrapper->line_item_label->value();
    }
  }

  $payment = array(
    "Customer" => array(
      "Reference" => $order->uid,
      "Title" => '',
      "FirstName" => $billing_address['first_name'],
      "LastName" => $billing_address['last_name'],
      "CompanyName" => isset($billing_address['organisation_name']) ? $billing_address['organisation_name'] : '',
      "JobDescription" => '',
      "Street1" => $billing_address['thoroughfare'],
      "Street2" => $billing_address['premise'],
      "City" => $billing_address['locality'],
      "State" => $billing_address['administrative_area'],
      "PostalCode" => $billing_address['postal_code'],
      "Country" => strtolower($billing_address['country']),
      "Email" => $order->mail,
      "Phone" => '',
      "Mobile" => '',
      "Comments" => substr(implode(', ', $description), 0, 256),
      "Fax" => '',
      "Url" => '',
      "CardDetails" => array(
        "Name" => isset($pane_values['credit_card']['owner']) ? $pane_values['credit_card']['owner'] : $billing_address['name_line'],
        "Number" => isset($pane_values['credit_card']['eway_encrypt_number']) ? $pane_values['credit_card']['eway_encrypt_number'] : $pane_values['credit_card']['number'],
        "ExpiryMonth" => $pane_values['credit_card']['exp_month'],
        "ExpiryYear" => substr($pane_values['credit_card']['exp_year'], -2),
        "StartMonth" => "",
        "StartYear" => "",
        "IssueNumber" => "",
        "CVN" => '',
      ),
    ),
    "ShippingAddress" => array(
      "FirstName" => $shipping_address['first_name'],
      "LastName" => $shipping_address['last_name'],
      "Street1" => $shipping_address['thoroughfare'],
      "Street2" => $shipping_address['premise'],
      "City" => $shipping_address['locality'],
      "State" => $shipping_address['administrative_area'],
      "Country" => strtolower($shipping_address['country']),
      "PostalCode" => $shipping_address['postal_code'],
      "Phone" => '',
    ),
    "Items" => $items,
    "Payment" => array(
      "TotalAmount" => intval($charge['amount']),
      "InvoiceNumber" => $order->order_id,
      "InvoiceDescription" => '',
      "InvoiceReference" => $order->order_id,
      "CurrencyCode" => $charge['currency_code'],
    ),
    "Options" => array(
      "ShippingMethod" => 'Unknown',
      'DeviceID' => 'drupal_eway_integration-7.x-1.x',
      "CustomerIP" => ip_address(),
      'PartnerID' => '',
      "TransactionType" => "Purchase",
      "Method" => "ProcessPayment",
    ),
  );

  // Add the start date and issue number if processing a Maestro or Solo card.
  if (isset($pane_values['credit_card']['type']) && in_array($pane_values['credit_card']['type'], array(
      'maestro',
      'solo'
    ))
  ) {
    if (!empty($pane_values['credit_card']['StartMonth']) && !empty($pane_values['credit_card']['StartYear'])) {
      $payment['Customer']['CardDetails']['StartMonth'] = $pane_values['credit_card']['StartMonth'];
      $payment['Customer']['CardDetails']['StartYear'] = substr($pane_values['credit_card']['StartYear'], -2);
    }

    if (!empty($pane_values['credit_card']['issue'])) {
      $payment['Customer']['CardDetails']['IssueNumber'] = $pane_values['credit_card']['issue'];
    }
  }

  // Add the CVV if entered on the form.
  if (isset($pane_values['credit_card']['code']) && !empty($pane_values['credit_card']['code'])) {
    $payment['Customer']['CardDetails']['CVN'] = isset($pane_values['credit_card']['eway_encrypt_code']) ? $pane_values['credit_card']['eway_encrypt_code'] : $pane_values['credit_card']['code'];
  }
  else {
    if (isset($pane_values['eway_encrypt_code']) || isset($pane_values['code'])) {
      $payment['Customer']['CardDetails']['CVN'] = isset($pane_values['eway_encrypt_code']) ? $pane_values['eway_encrypt_code'] : $pane_values['code'];
    }
  }

  // Allow modules to alter parameters of the API request.
  $context = array(
    'order' => $order,
  );
  drupal_alter('eway_integration_payment', $payment, $context);


  return $payment;
}

/**
 * @param $payment
 * @return mixed
 */
function eway_integration_commerce_create_token_request($payment) {
  $request = $payment;
  $request['Payment']['TotalAmount'] = 0;
  $request['Options']['Method'] = "CreateTokenCustomer";
  return $request;
}

/**
 * @param $payment
 * @return mixed
 */
function eway_integration_commerce_create_token_user_request($card_data) {
  if (isset($card_data->card_name)) {
    $name = $card_data->card_name;
    $parts = explode(" ", $name);
    $last_name = array_pop($parts);
    $first_name = implode(" ", $parts);
  }

  $request = array(
    "Customer" => array(
      "Reference" => '',
      "Title" => '',
      "FirstName" => $first_name,
      "LastName" => $last_name,
      "Country" => isset($card_data->country) ? $card_data->country : 'au',
      "Email" => '',
      "Phone" => '',
      "Mobile" => '',
      "Comments" => '',
      "Fax" => '',
      "Url" => '',
      "CardDetails" => array(
        "Name" => isset($card_data->card_name) ? $card_data->card_name : '',
        "Number" => isset($card_data->cardno) ? $card_data->cardno : '',
        "ExpiryMonth" => isset($card_data->card_exp_month) ? sprintf('%02d', $card_data->card_exp_month) : '',
        "ExpiryYear" => isset($card_data->card_exp_year) ? substr($card_data->card_exp_year, -2) : '',
        "StartMonth" => '',
        "StartYear" => '',
        "IssueNumber" => '',
        "CVN" => '',
      ),
    ),
    "ShippingAddress" => array(),
    "Items" => array(),
    "Payment" => array(),
    "Options" => array(
      "ShippingMethod" => 'Unknown',
      'DeviceID' => '',
      "CustomerIP" => ip_address(),
      'PartnerID' => '',
      "TransactionType" => "Purchase",
    ),
  );

  $request['Payment']['TotalAmount'] = 0;

  if ($card_data->is_new == 1) {
    $request['Options']['Method'] = "CreateTokenCustomer";
  }

  return $request;
}

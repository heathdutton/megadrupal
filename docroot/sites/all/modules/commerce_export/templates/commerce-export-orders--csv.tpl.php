<?php

/**
 * @file
 * Default commerce orders export template.
 *
 * @ingroup themeable
 */

/* Delete this line to print a list of available variables and their values.
echo check_plain(print_r($variables, 1));
// */

$delimiter = ',';
$headers = array(
  'line_item__quantity',
  'line_item__commerce_unit_price',
  'line_item__commerce_total',
  'line_item__commerce_product__sku',
  'line_item__commerce_product__title',
  'line_item__commerce_product__commerce_price',
  'order__commerce_order_total',
  'billing_address__country',
  'billing_address__administrative_area',
  'billing_address__sub_administrative_area',
  'billing_address__locality',
  'billing_address__dependent_locality',
  'billing_address__postal_code',
  'billing_address__thoroughfare',
  'billing_address__premise',
  'billing_address__organisation_name',
  'billing_address__name_line',
  'billing_address__first_name',
  'billing_address__last_name',
  'order__order_number',
  'order__status',
  'order__state',
  'order__created',
  'order__changed',
  'order__hostname',
  'owner__mail',
  'order__mail',
);

echo implode($delimiter, $headers) . "\n";

foreach ($orders as $order) {
  $order_total = $order->commerce_order_total->value();
  foreach ($order->commerce_line_items as $line_item) {
    if ($line_item->type->value() == 'product') {
      $product_line_items[] = $line_item;
    }
  }
  $billing_address = $order->commerce_customer_billing->get('commerce_customer_address');
  $owner = $order->get('owner');

  foreach ($product_line_items as $line_item) {
    $line_item_unit_price = $line_item->commerce_unit_price->value();
    $line_item_total = $line_item->commerce_total->value();
    $line_item_product_price = $line_item->commerce_product->commerce_price->value();
    $values = array();
    $values[] = $line_item->quantity->value();
    $values[] = commerce_currency_amount_to_decimal($line_item_unit_price['amount'], $line_item_unit_price['currency_code']);
    $values[] = commerce_currency_amount_to_decimal($line_item_total['amount'], $line_item_total['currency_code']);
    $values[] = $line_item->commerce_product->sku->value();
    $values[] = $line_item->commerce_product->title->value();
    $values[] = commerce_currency_amount_to_decimal($line_item_product_price['amount'], $line_item_product_price['currency_code']);
    $values[] = commerce_currency_amount_to_decimal($order_total['amount'], $order_total['currency_code']);
    $values[] = $billing_address->country->value();
    $values[] = $billing_address->administrative_area->value();
    $values[] = $billing_address->sub_administrative_area->value();
    $values[] = $billing_address->locality->value();
    $values[] = $billing_address->dependent_locality->value();
    $values[] = $billing_address->postal_code->value();
    $values[] = $billing_address->thoroughfare->value();
    $values[] = $billing_address->premise->value();
    $values[] = $billing_address->organisation_name->value();
    $values[] = $billing_address->name_line->value();
    $values[] = $billing_address->first_name->value();
    $values[] = $billing_address->last_name->value();
    $values[] = $order->order_number->value();
    $values[] = $order->status->value();
    $values[] = $order->state->value();
    $values[] = $order->created->value();
    $values[] = $order->changed->value();
    $values[] = $order->hostname->value();
    $values[] = $owner->mail->value();
    $values[] = $order->mail->value();

    echo implode($delimiter, $values) . "\n";
  }
}

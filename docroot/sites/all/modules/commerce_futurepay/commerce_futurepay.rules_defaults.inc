<?php

/**
 * @file
 * Default rules configurations for FuturePay.
 */

/**
 * Implements hook_default_rules_configuration_alter().
 *
 * Check the FuturePay business rules in order to ensure that the customer
 * can access to that payment gateway.
 */
function commerce_futurepay_default_rules_configuration_alter(&$configs) {
  // A customer must select United States as their country selected value (in
  // the billing information).
  if (isset($configs['commerce_payment_futurepay'])) {
    $configs['commerce_payment_futurepay']->condition(
      rules_condition(
        'commerce_order_compare_address',
        array(
             'address_field' => 'commerce_customer_billing|commerce_customer_address',
             'address_component' => 'country',
             'operator' => 'equals',
             'value' => 'US',
        )
      )
    );
    // FuturePay will only accept a Maximum Order of $500.00.
    $configs['commerce_payment_futurepay']->condition(
      rules_condition(
        'commerce_price_compare_price',
        array(
             'first_price:select' => 'commerce-order:commerce-order-total',
             'operator' => '<=',
             'second_price' => array(
               "amount" => '50000', "currency_code" => "USD"
             ),
        )
      )
    );
    // Only zip codes that are not a PO box or Military address will be accepted.
    $configs['commerce_payment_futurepay']->condition(
      rules_condition(
        'commerce_futurepay_military_address',
        array(
             'address_field' => 'commerce_customer_billing|commerce_customer_address',
        )
      )->negate()
    );
  }
}

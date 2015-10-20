<?php

/**
 * @file
 * Hooks provided by Taxamo for Commerce module.
 */


/**
 * Defines the VAT number (if any) to be applied to an order. Below you can see
 * the exact hook implementation done by Taxamo for Commerce module.
 * @param  object $order      The order that is being processed.
 * @param  string $vat_number (by reference) The VAT number.
 */
function hook_commerce_taxamo_get_vat_number($order, &$vat_number) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  $vat_number_field = commerce_taxamo_get_var('commerce_taxamo_vat_number_field');

  if (!empty($vat_number_field)) {
    $vat_number_field_info = explode('||', $vat_number_field);

    if (count($vat_number_field_info) == 3) {
      // Check if field is in user entity and user bundle.
      if ($vat_number_field_info[0] == 'user' && $vat_number_field_info[1] == 'user') {
        if (!empty($order->uid)) {
          $account = user_load($order->uid);
          $field_data = field_get_items('user', $account, $vat_number_field_info[2]);
          if (!empty($field_data)) {
            $vat_number = $field_data[0]['value'];
          }
        }
      }

      // Check if field is in commerce_customer_profile entity and billing bundle.
      if ($vat_number_field_info[0] == 'commerce_customer_profile' && $vat_number_field_info[1] == 'billing') {
        $billing_profile_id = $order_wrapper->commerce_customer_billing->profile_id->value();
        if (!empty($billing_profile_id)) {
          $billing_profile = commerce_customer_profile_load($billing_profile_id);
          if (!empty($billing_profile)) {
            $field_data = field_get_items('commerce_customer_profile', $billing_profile, $vat_number_field_info[2]);
            if (!empty($field_data)) {
              $vat_number = $field_data[0]['value'];
            }
          }
        }
      }
    }

    $vat_number = check_plain($vat_number);
  }
}

/**
 * This hook is called just before to send the $transaction object to Taxamo to
 * create a Transaction.
 *
 * Here you can modify anything in the $transaction object. This is not intended
 * to be used to modify the $order object but nothing will stop you to do it.
 *
 * @param  object $order       The order entity to which the transaction will be
 * created.
 * @param  object $transaction The transaction object that will be sent to
 * Taxamo.
 */
function hook_commerce_taxamo_transaction_pre_create($order, $transaction) {
  $transaction->custom_id = "MyCustomID-" . REQUEST_TIME . "-" . rand(1000000, 9999999);
}

/**
 * This hook is called once a transaction is successfully created. You can use
 * the Transaction information to update something in the order or anything else.
 * @param  object $order       The order entity to which the transaction was
 * created.
 * @param  object $transaction The transaction object created in Taxamo.
 */
function commerce_taxamo_commerce_taxamo_transaction_post_create($order, $transaction) {
  // No example.
}


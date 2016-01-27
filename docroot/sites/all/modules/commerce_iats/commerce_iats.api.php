<?php

/**
 * @file
 * Commerce iATS hook definitions.
 */

/**
 * Alter an order's invoice number before it is sent to iATS Payments.
 *
 * @param int $invoice_number
 *   The invoice number. Defaults to the Commerce order ID.
 * @param object $order
 *   The order object.
 */
function hook_commerce_iats_invoice_number_alter(&$invoice_number, $order) {

}

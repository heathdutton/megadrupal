<?php

/**
 * @file
 * Documents hooks provided by the MoIP modules.
 */

/**
 * Lets modules perform additional processing on validated NASPs.
 *
 *
 * @param $order
 *   The order that initiated the payment associated with the NASP.
 * @param $payment_method
 *   The payment method instance used to create the payment and perform initial
 *   processing on the NASP.
 * @param $nasp
 *   The NASP array received from MoIP after it has been saved.
 *
 * @see commerce_moip_html_moip_nasp_process(()
 */
function hook_commerce_moip_nasp_process($order, $payment_method, $nasp) {
  // No example.
}

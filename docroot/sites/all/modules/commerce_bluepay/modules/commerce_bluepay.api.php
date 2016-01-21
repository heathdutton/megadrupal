<?php

/**
 * @file
 * API documentation for Commerce BluePay.
 */

/**
 * Allow modules to alter the request before being sent.
 *
 * @param CommerceBluePay20Post $request
 *   The request being modified.
 *
 * @param object $order
 *   The Commerce Order object for this request.
 *
 * @param array $payment_method
 *   Payment method information.
 */
function commerce_bluepay_level3_commerce_bluepay_request_alter($request, $order, $payment_method) {
  // No example provided.
}

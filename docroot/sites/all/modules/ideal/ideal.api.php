<?php

/**
 * @file
 * Hook documentation.
 */

/**
 * Alters a request.
 *
 * @param string $type
 *   One of the iDEALPaymentMethodController::REQUEST_* constants.
 * @param DOMDocument $dom
 *   The DOM that represents the request, without the signature.
 * @param PaymentMethod $payment_method
 */
function hook_ideal_request_alter($type, DOMDocument $dom, PaymentMethod $payment_method) {
  /** @var iDEALPaymentMethodController $controller */
  $controller = $payment_method->controller;
  if ($type == $controller::REQUEST_TRANSACTION) {
    $xpath = new DOMXPath($dom);
    $xpath->registerNamespace('iDEAL', $controller::IDEAL_XML_NAMESPACE);
    $xpath->query('/iDEAL:AcquirerTrxReq/iDEAL:Transaction/iDEAL:purchaseID')->item(0)->nodeValue .= '#';
  }
}

/**
 * Alters a response.
 *
 * @param string $type
 *   One of the iDEALPaymentMethodController::RESPONSE_* constants.
 * @param DOMDocument $dom
 *   The DOM that represents the response.
 * @param PaymentMethod $payment_method
 */
function hook_ideal_response_alter($type, DOMDocument $dom, PaymentMethod $payment_method) {
}

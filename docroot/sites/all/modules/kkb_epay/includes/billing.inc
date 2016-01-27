<?php

/**
 * @file
 * Main set of tools to work with the processing gateway.
 *
 * Copyright (C) 2012, Victor Nikulshin
 *
 * This file is part of KKB Epay.
 *
 * KKB Epay is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * KKB Epay is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KKB Epay.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Loads current private key with the default KkbEpay_DrupalKeyLoader.
 */
function kkb_epay_load_default_key() {
  static $key = NULL;
  if (!isset($key)) {
    $loader = new KkbEpay_DrupalKeyLoader();
    $loader->setDebug(kkb_epay_debug_status());
    $key = $loader->getKey();
  }
  return $key;
}

/**
 * Signs a message with the default key.
 *
 * @return string|NULL
 *   Returns NULL if any errors happen. Otherwise, returns base64-encoded
 *   signature.
 */
function kkb_epay_sign_message($message) {
  $loader = new KkbEpay_DrupalKeyLoader();
  $loader->setDebug(kkb_epay_debug_status());

  if (!$loader->validateKey()) {
    drupal_set_message(t('Private key could not be loaded. Message will not be signed.'), 'error');
    return NULL;
  }

  $sign = new KkbEpay_Sign($loader->getKey());
  try {
    return $sign->sign64($message);
  }
  catch (KkbEpay_Exception $ex) {
    drupal_set_message(t('There is a problem with the key, message was not signed. Check logs for details.'), 'error');
    watchdog('kkb_epay', $ex->getMessage(), array(), WATCHDOG_ERROR);
    if ($previous = $ex->getPrevious()) {
      watchdog('kkb_epay', $previous->getMessage(), array(), WATCHDOG_ERROR);
    }
    return NULL;
  }
}

/**
 * Checks a message signature with the default certificate.
 *
 * @param text $message
 *   Message whose signature must be verified.
 *
 * @param text $signature
 *   Base64-encoded signature of the message.
 *
 * @return boolean
 *   TRUE if signature was verified without errors.
 *   FALSE otherwise.
 */
function kkb_epay_check_signature($message, $signature) {
  $certificate = new KkbEpay_DefaultBankCertificate();
  try {
    $checker = new KkbEpay_Checker($certificate);
    return $checker->check64($message, $signature);
  }
  catch (KkbEpay_Exception $ex) {
    drupal_set_message(t('There is a problem with the certificate. Check logs for details.'), 'error');
    watchdog('kkb_epay', $ex->getMessage(), array(), WATCHDOG_ERROR);
    if ($previous = $ex->getPrevious()) {
      watchdog('kkb_epay', $previous->getMessage(), array(), WATCHDOG_ERROR);
    }
    return FALSE;
  }
}

/**
 * Creates a complete signed order document.
 *
 * The created docuemnt can be sent directly to the processing center.
 *
 * @return string
 *   Order document as a string. The document is not base64 encoded.
 */
function kkb_epay_create_signed_order(KkbEpay_Order $order) {
  $dom = new DOMDocument('1.0', 'UTF-8');
  $key = kkb_epay_load_default_key();

  $document   = $dom->createElement('document');
  $merchant   = $dom->createElement('merchant');
  $order_elem = $dom->createElement('order');
  $department = $dom->createElement('department');

  $merchant->setAttribute('cert_id', $key->getCertificateId());
  $merchant->setAttribute('name', $key->getMerchantName());
  $department->setAttribute('merchant_id', $key->getMerchantId());
  $department->setAttribute('amount', $order->getAmount());
  $order_elem->setAttribute('amount', $order->getAmount());
  $order_elem->setAttribute('currency', $order->getCurrency());
  $order_elem->setAttribute('order_id', $order->getId());
  $department->setAttribute('abonent_id', '');
  $department->setAttribute('terminal', '');

  $dom->appendChild($document);
  $document->appendChild($merchant);
  $merchant->appendChild($order_elem);
  $order_elem->appendChild($department);

  $sign = $dom->createElement(
    'merchant_sign',
    kkb_epay_sign_message($dom->saveXML($merchant))
  );
  $sign->setAttribute('type', 'RSA');
  $document->appendChild($sign);

  return $dom->saveXML($document);
}

/**
 * Returns URL of the gateway that will be used to process payment.
 *
 * Returned value is different when debugging mode is on or off.
 */
function kkb_epay_processing_gateway_url() {
  if (kkb_epay_debug_status()) {
    // URL of the sandbox gateway.
    return 'http://3dsecure.kkb.kz/jsp/process/logon.jsp';
  }
  else {
    // URL of the live processing gateway.
    return 'https://epay.kkb.kz/jsp/process/logon.jsp';
  }
}

/**
 * Returns an appendix document with a list of items in an order.
 */
function kkb_epay_create_order_appendix(KkbEpay_Order $order) {
  $dom = new DOMDocument('1.0', 'UTF-8');

  $document = $dom->createElement('document');
  foreach ($order->getItems() as $i) {
    $item = $dom->createElement('item');
    $item->setAttribute('number', $i->getNumber());
    $item->setAttribute('name', $i->getName());
    $item->setAttribute('quantity', $i->getQuantity());
    $item->setAttribute('amount', $i->getAmount());
    $document->appendChild($item);
  }

  return $dom->saveXML($document);
}

/**
 * Validates response document received from the processing center.
 *
 * Document must have correct structure and valid signature.
 *
 * @param string $document
 *   Response received from the center.
 *
 * @return boolean
 *   TRUE if document is valid, FALSE othewise.
 */
function kkb_epay_bank_response_document_validate($document) {
  if (substr($document, 0, 10) != '<document>') {
    return FALSE;
  }
  if (substr($document, -11) != '</document>') {
    return FALSE;
  }
  if (strpos($document, '</bank>') === FALSE) {
    return FALSE;
  }

  libxml_use_internal_errors(TRUE);
  $xml = @simplexml_load_string($document);
  if (!$xml) {
    return FALSE;
  }

  $body = substr($document, 10, strlen($document) - 21);
  $bank = substr($body, 0, strpos($body, '</bank>') + 7);

  if (!$xml->bank_sign || !strval($xml->bank_sign)) {
    return FALSE;
  }

  return kkb_epay_check_signature($bank, strval($xml->bank_sign));
}

/**
 * Extracts information about payment from response document.
 */
function kkb_epay_bank_response_document_parse($document) {
  $xml       = @simplexml_load_string($document);
  $order_id  = 0;
  $timestamp = REQUEST_TIME;
  $amount    = 0.0;
  $reference = NULL;
  $approval  = NULL;

  $xpath = $xml->xpath('//order/@order_id');
  if ($elem = array_shift($xpath)) {
    $order_id = (string) $elem->order_id;
  }

  $xpath = $xml->xpath('//results/@timestamp');
  if ($elem = array_shift($xpath)) {
    $timestamp_parsed = strtotime((string) $elem->order_id);
    if ($timestamp_parsed) {
      $timestamp = $timestamp_parsed;
    }
  }

  $xpath = $xml->xpath('//payment/@amount');
  if ($elem = array_shift($xpath)) {
    $amount = (string) $elem->amount;
  }

  $xpath = $xml->xpath('//payment/@reference');
  if ($elem = array_shift($xpath)) {
    $reference = (string) $elem->reference;
  }

  $xpath = $xml->xpath('//payment/@approval_code');
  if ($elem = array_shift($xpath)) {
    $approval = (string) $elem->approval_code;
  }

  return new KkbEpay_PaymentNotification(array(
    'amount'        => $amount,
    'approval_code' => $approval,
    'order_id'      => $order_id,
    'reference'     => $reference,
    'timestamp'     => $timestamp,
  ));
}

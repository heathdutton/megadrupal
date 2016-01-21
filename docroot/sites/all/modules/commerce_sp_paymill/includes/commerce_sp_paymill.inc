<?php

/**
 * @file
 * Contains all necessary API functions for connecting to Paymill.
 */


/**
 * Get Paymill private key.
 */
function commerce_sp_paymill_api_get_private_key() {
  $mode = variable_get('commerce_sp_paymill_mode', 'test');
  $key = variable_get('commerce_sp_paymill_' . $mode . '_private_key', '');
  return $key;
}

/**
 * Get Paymill public key.
 */
function commerce_sp_paymill_api_get_public_key() {
  $mode = variable_get('commerce_sp_paymill_mode', 'test');
  $key = variable_get('commerce_sp_paymill_' . $mode . '_public_key', '');
  return $key;
}


/**
 * Registers a new webhook at Paymill.
 *
 * @param $url
 *   An absolute URL where Paymill should send webhook notifications.
 * @param array $event_types
 *   An array of the webhook events to trigger on.
 * @param $mode
 *   Either live or test, indicating on which environment the webhook should be
 *   created.
 *
 * @return array
 *   A Paymill webhook array.
 */
function commerce_sp_paymill_api_webhooks_create($url, $event_types, $mode = 'live') {
  libraries_load('paymill');
  $key = variable_get('commerce_sp_paymill_' . $mode . '_private_key', '');
  $object = new Services_Paymill_Webhooks($key, COMMERCE_SP_PAYMILL_SERVER);
  $params = array(
    'url' => $url,
    'event_types' => $event_types,
  );
  $result = $object->create($params);
  return $result;
}

/**
 * Deletes a webhook at Paymill.
 *
 * @param $id
 *   The webhook id.
 * @param $mode
 *   Either live or test, indicating on which environment the webhook should be
 *   deleted.
 *
 * @return array
 */
function commerce_sp_paymill_api_webhook_delete($id, $mode = 'live') {
  libraries_load('paymill');
  $key = variable_get('commerce_sp_paymill_' . $mode . '_private_key', '');
  $object = new Services_Paymill_Webhooks($key, COMMERCE_SP_PAYMILL_SERVER);
  $result = $object->delete($id);
  return $result;
}

/**
 * Loads existing offer from Paymill
 *
 * @param $offer_id
 *   The Paymill offer id.
 *
 * @return array
 *   A Paymill offer array.
 */
function commerce_sp_paymill_api_offer_load($offer_id) {
  libraries_load('paymill');
  $object = new Services_Paymill_Offers(commerce_sp_paymill_api_get_private_key(), COMMERCE_SP_PAYMILL_SERVER);
  $result = $object->getOne($offer_id);
  return $result;
}


/**
 * Creates a new offer at Paymill.
 *
 * @param array $params
 *   An array of parameters for this offer, containing:
 *     - name: The name of the offer
 *     - amount: The amount to charge
 *     - currency: The currency code
 *     - interval: eihter DAY, WEEK, MONTH or YEAR
 *
 * @return array
 *   A Paymill offer array.
 */
function commerce_sp_paymill_api_offer_create($params = array()) {
  libraries_load('paymill');
  $object = new Services_Paymill_Offers(commerce_sp_paymill_api_get_private_key(), COMMERCE_SP_PAYMILL_SERVER);
  $result = $object->create($params);
  return $result;
}


/**
 * Updates an existing offer at Paymill.
 *
 * Only the name can be changed all other attributes cannot be edited.
 *
 * @param array $params
 *   An array of parameters for this offer, containing:
 *     - id: The Paymill id for this offer
 *     - name: The name of the offer
 *
 * @return array
 *   A Paymill offer array.
 */
function commerce_sp_paymill_api_offer_update($params = array()) {
  libraries_load('paymill');
  $object = new Services_Paymill_Offers(commerce_sp_paymill_api_get_private_key(), COMMERCE_SP_PAYMILL_SERVER);
  $result = $object->update($params);
  return $result;
}

/**
 * Deletes an existing offer at Paymill.
 *
 * You only can delete an offer if no client is subscribed to this offer.
 *
 * @param $id
 *   The Paymill offer id.
 *
 * @return array
 */
function commerce_sp_paymill_api_offer_delete($id) {
  libraries_load('paymill');
  $object = new Services_Paymill_Offers(commerce_sp_paymill_api_get_private_key(), COMMERCE_SP_PAYMILL_SERVER);
  $result = $object->delete($id);
  return $result;
}

/**
 * Creates a new subscription at Paymill.
 *
 * Once the subscription has successfully been created, Paymill initializes a
 * new transaction for the given client and payment.
 *
 * @param array $params
 *   An array of parameters for this offer, containing:
 *     - offer: The Paymill offer id.
 *     - client: The Paymill client id.
 *     - payment: The Paymill payment id.
 *
 * @return array
 *   A Paymill subscription array.
 */
function commerce_sp_paymill_api_subscription_create($params = array()) {
  libraries_load('paymill');
  $object = new Services_Paymill_Subscriptions(commerce_sp_paymill_api_get_private_key(), COMMERCE_SP_PAYMILL_SERVER);
  $result = $object->create($params);
  return $result;
}


/**
 * Updates an existing subscription at Paymill.
 *
 * @param array $params
 *   An array of parameters for this offer, containing:
 *     - id: The Paymill subscription id.
 *     - offer: The Paymill offer id.
 *     - payment: The Paymill payment id.
 *     - cancel_at_period_end: boolean, cancel this subscription immediately or
 *       at the end of the current period?
 *
 * @return array
 *   A Paymill subscription array.
 */
function commerce_sp_paymill_api_subscription_update($params = array()) {
  libraries_load('paymill');
  $object = new Services_Paymill_Subscriptions(commerce_sp_paymill_api_get_private_key(), COMMERCE_SP_PAYMILL_SERVER);
  $result = $object->update($params);
  return $result;
}

/**
 * Deletes an existing subscription at Paymill.
 *
 * @param $id
 *   The Paymill subscription id.
 * @return array
 */
function commerce_sp_paymill_api_subscription_delete($id) {
  libraries_load('paymill');
  $object = new Services_Paymill_Subscriptions(commerce_sp_paymill_api_get_private_key(), COMMERCE_SP_PAYMILL_SERVER);
  $result = $object->delete($id);
  return $result;
}

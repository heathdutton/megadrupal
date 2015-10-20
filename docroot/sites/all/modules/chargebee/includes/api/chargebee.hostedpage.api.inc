<?php
/**
 * @file
 * Hostedpage ChargeBee API functions.
 */

/**
 * Get hostedpage for checkout new subscription.
 *
 * @param array $params
 *   Subscription, customer, card and addons params.
 *
 * @return array
 *   Hostedpage with or without content.
 */
function _chargebee_hostedpage_checkout_new($params) {
  $result = ChargeBee_HostedPage::checkoutNew($params);
  $hostedpage = $result->hostedPage();
  return _chargebee_hostedpage_to_array($hostedpage);
}

/**
 * Get hostedpage for checkout existing subscription.
 *
 * @param array $params
 *   Subscription, customer, card and addons params.
 *
 * @return array
 *   Hostedpage with or without content.
 */
function _chargebee_hostedpage_checkout_existing($params) {
  $result = ChargeBee_HostedPage::checkoutExisting($params);
  $hostedpage = $result->hostedPage();
  return _chargebee_hostedpage_to_array($hostedpage);
}

/**
 * Get hostedpage for update customer card information.
 *
 * @param string $customer_id
 *   Customer identifier.
 *
 * @return array
 *   Hostedpage array.
 */
function _chargebee_hostedpage_update_card($customer_id) {
  $params = array(
    'customer' => array(
      'id' => $customer_id,
    ),
  );
  $result = ChargeBee_HostedPage::updateCard($params);
  $hostedpage = $result->hostedPage();
  return _chargebee_hostedpage_to_array($hostedpage);
}

/**
 * Retrieve hostedpage by ID.
 *
 * @param string $hostedpage_id
 *   Hostedpage identifier.
 *
 * @return array
 *   Hostedpage with or without content.
 */
function _chargebee_hostedpage_retrieve($hostedpage_id) {
  $result = ChargeBee_HostedPage::retrieve($hostedpage_id);
  $hostedpage = $result->hostedPage();
  return _chargebee_hostedpage_to_array($hostedpage);
}

/**
 * Convert ChargeBee hostedpage object to array.
 *
 * @param object $hostedpage
 *   ChargeBee hostedpage object.
 *
 * @return array
 *   Hostedpage array.
 */
function _chargebee_hostedpage_to_array($hostedpage) {
  $attributes = array(
    'id' => 'id',
    'type' => 'type',
    'url' => 'url',
    'state' => 'state',
    'failure_reason' => 'failureReason',
    'pass_thru_content' => 'passThruContent',
    'embed' => 'embed',
    'created_at' => 'createdAt',
    'expires_at' => 'expiresAt',
  );
  $hostedpage_array = _chargebee_object_to_array_convert($hostedpage, $attributes);
  $content = $hostedpage->content();
  $hostedpage_array['content'] = array(
    'subscription' => method_exists($content, 'subscription') ? _chargebee_subscription_to_array($content->subscription()) : NULL,
    'customer' => method_exists($content, 'customer') ? _chargebee_customer_to_array($content->customer()) : NULL,
    'card' => method_exists($content, 'card') ? _chargebee_card_to_array($content->card()) : NULL,
    'invoice' => method_exists($content, 'invoice') ? _chargebee_invoice_to_array($content->invoice()) : NULL,
  );
  return $hostedpage_array;
}

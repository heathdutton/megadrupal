<?php

/**
 * @file
 * Paymill webhooks processsing.
 */

/**
 * Menu callback for processing a Paymill webhook event.
 */
function commerce_sp_paymill_webhook_process() {
  $data = file_get_contents('php://input');

  // Exit now if $_POST is empty.
  if (empty($data)) {
    watchdog('commerce_sp_paymill_sub', 'Paymill webhook accessed with no data submitted.', array(), WATCHDOG_WARNING);
    return FALSE;
  }

  // Decode json request and transform it to a PHP array.
  $decoded = json_decode($data, TRUE);
  $event = isset($decoded['event']) ? $decoded['event'] : array();
  //watchdog('commerce_sp_paymill_sub', 'Event: <pre>' . print_r($event, TRUE) . '</pre>', array(), WATCHDOG_WARNING);

  // Check if all necessary Paymill properties are set.
  if (!isset($event['event_type']) || !isset($event['event_resource']) || !isset($event['created_at'])) {
    watchdog('commerce_sp_paymill_sub', 'Paymill webhook accessed with incomplete information.', array(), WATCHDOG_WARNING);
    return FALSE;
  }

  // Check if the event type is valid.
  if (!in_array($event['event_type'], array_keys(commerce_sp_paymill_webhook_event_types()))) {
    watchdog('commerce_sp_paymill_sub', 'Paymill webhook with invalid event type: %type.', array('%type' => $event['event_type']), WATCHDOG_ERROR);
    return FALSE;
  }

  // The webhooks contain the resource in different ways.
  if ($event['event_type'] == 'subscription.succeeded' || $event['event_type'] == 'subscription.failed') {
    $subscription_resource = isset($event['event_resource']['subscription']) ? $event['event_resource']['subscription'] : array();
  }
  else {
    $subscription_resource = isset($event['event_resource']) ? $event['event_resource'] : array();
  }

  // Check if the subscription object is included.
  if (!isset($subscription_resource['id'])) {
    watchdog('commerce_sp_paymill_sub', 'Paymill webhook event %type with missing subscription object.', array('%type' => $event['event_type']), WATCHDOG_ERROR);
    return FALSE;
  }

  // Check if we have an according subscription.
  $subscription = commerce_sp_paymill_sub_load_by_subscription_id($subscription_resource['id']);
  if (!$subscription) {
    watchdog('commerce_sp_paymill_sub', 'Paymill webhook event %type missing subscription id %id', array('%type' => $event['event_type'], '%id' => $event['event_resource']['subscription']['id']), WATCHDOG_ERROR);
    return FALSE;
  }

  commerce_sp_paymill_log_webhook($event, $subscription);

  if ($event['event_type'] == 'subscription.succeeded') {

    // Renew subscription.
    // Check that the transaction does not already exist in our database.
    $condition = array(
      'remote_id' => $event['event_resource']['transaction']['id'],
      'payment_method' => 'commerce_paymill',
    );
    $transaction = commerce_payment_transaction_load_multiple(FALSE, $condition);
    if ($transaction) {
      watchdog('commerce_sp_paymill_sub', 'Transaction for Paymill %id already exists, skip renewal.', array('%id' => $event['event_resource']['transaction']['id']), WATCHDOG_INFO);
      return FALSE;
    }
    $account = $subscription->user();
    // Only renew subscription if we have a user account, otherwise stop it.
    if ($account) {
      // Activate the subscription if it is inactive. This should never happen,
      // but we want to be robust just in case.
      if (!$subscription->status) {
        commerce_sp_paymill_activate_subscription($subscription, $event['event_resource']['transaction']);
      }
      commerce_sp_paymill_renew_subscription($subscription, $event['event_resource']['transaction']);
    }
    else {
      commerce_sp_paymill_api_subscription_delete($subscription->subscription_id);
      commerce_sp_paymill_stop_subscription($subscription);
    }
  }
  // Stop subscription.
  elseif ($event['event_type'] == 'subscription.failed') {
    commerce_sp_paymill_stop_subscription($subscription);
  }

  module_invoke_all('commerce_sp_paymill_webhook_event', $subscription, $event);

  rules_invoke_event('commerce_sp_paymill_webhook_event', $subscription, $event['event_type']);

  // ?
  header('HTTP/1.1 200 OK');
}

/**
 * Logs IPN to the system using the message module.
 */
function commerce_sp_paymill_log_webhook($event, $subscription) {
  $message = message_create('commerce_sp_paymill_webhook_log', array('uid' => $subscription->uid));
  $wrapper = entity_metadata_wrapper('message', $message);
  $wrapper->field_commerce_sp_paymill_sub->set($subscription->id);
  $wrapper->field_commerce_sp_paymill_event->set($event['event_type']);
  $wrapper->field_commerce_sp_paymill_cont->set(print_r($event, TRUE));
  $wrapper->save();
}

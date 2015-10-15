<?php
/**
 * @file
 * Webhook callback.
 */

/**
 * Callback for ChargeBee webhook events.
 */
function chargebee_webhook_callback() {
  $content = file_get_contents('php://input');
  $event = chargebee_event_deserialize($content);
  if (!$event || $event->source == 'api') {
    return FALSE;
  }
  $event_content = $event->content();
  $subscription_array = _chargebee_subscription_to_array($event_content->subscription());
  $customer = _chargebee_customer_to_array($event_content->customer());
  $user = user_load_by_mail($customer['email']);
  if ($user) {
    if ($event->eventType == 'subscription_cancelled') {
      chargebee_subscription_cancel($subscription_array['subscription_id']);
    }
    $subscription = chargebee_subscription_load($user->uid);
    chargebee_subscription_update($subscription, $subscription_array);
  }
  else {
    watchdog('ChargeBee', 'Webhook for nonexistent user (@email)', array('@email' => $customer['email']));
  }
  return TRUE;
}

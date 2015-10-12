<?php
/**
 * @file
 * Subscriptions ChargeBee API functions.
 */

/**
 * Create subscription in ChargeBee service.
 *
 * @param array $params
 *   Subscription params
 *
 * @return array
 *   Subscription array with invoice.
 */
function _chargebee_subscription_create($params) {
  $result = ChargeBee_Subscription::create($params);
  $subscription = $result->subscription();
  $customer = $result->customer();
  $card = $result->card();
  $invoice = $result->invoice();
  return array(
    'subscription' => _chargebee_subscription_to_array($subscription),
    'customer' => _chargebee_customer_to_array($customer),
    'card' => _chargebee_card_to_array($card),
    'invoice' => _chargebee_invoice_to_array($invoice),
  );
}

/**
 * Retrieve subscription from ChargeBee.
 *
 * @param string $subscription_id
 *   ChargeBee subscription identifier.
 *
 * @return array
 *   Subscription array.
 */
function _chargebee_subscription_retrieve($subscription_id) {
  $result = ChargeBee_Subscription::retrieve($subscription_id);
  $subscription = $result->subscription();
  $customer = $result->customer();
  $card = $result->card();
  return array(
    'subscription' => _chargebee_subscription_to_array($subscription),
    'customer' => _chargebee_customer_to_array($customer),
    'card' => _chargebee_card_to_array($card),
  );
}

/**
 * Update subscription plan.
 *
 * @param string $subscription_id
 *   ChargeBee subscription identifier.
 * @param string $plan_id
 *   New plan identifier.
 *
 * @return array
 *   Subscription array.
 */
function _chargebee_subscription_update_plan($subscription_id, $plan_id) {
  $result = ChargeBee_Subscription::update($subscription_id, array('planId' => $plan_id));
  $subscription = $result->subscription();
  $customer = $result->customer();
  $card = $result->card();
  $invoice = $result->invoice();
  return array(
    'subscription' => _chargebee_subscription_to_array($subscription),
    'customer' => _chargebee_customer_to_array($customer),
    'card' => _chargebee_card_to_array($card),
    'invoice' => _chargebee_invoice_to_array($invoice),
  );
}

/**
 * Cancel subscription.
 *
 * @param string $subscription_id
 *   ChargeBee subscription identifier.
 *
 * @return array
 *   Subscription array.
 */
function _chargebee_subscription_cancel($subscription_id) {
  $result = ChargeBee_Subscription::cancel($subscription_id);
  $subscription = $result->subscription();
  $customer = $result->customer();
  $card = $result->card();
  $invoice = $result->invoice();
  return array(
    'subscription' => _chargebee_subscription_to_array($subscription),
    'customer' => _chargebee_customer_to_array($customer),
    'card' => _chargebee_card_to_array($card),
    'invoice' => _chargebee_invoice_to_array($invoice),
  );
}

/**
 * Conversion data functions.
 */

/**
 * Convert ChargeBee subscription object to array.
 *
 * @param object $subscription
 *   ChargeBee subscription object.
 *
 * @return array
 *   Subscription array.
 */
function _chargebee_subscription_to_array($subscription) {
  $subscription_array = array();
  if ($subscription) {
    $attributes = array(
      'subscription_id' => 'id',
      'plan_id' => 'planId',
      'plan_quantity' => 'planQuantity',
      'status' => 'status',
      'start_date' => 'startDate',
      'trial_start' => 'trialStart',
      'trial_end' => 'trialEnd',
      'current_term_start' => 'currentTermStart',
      'current_term_end' => 'currentTermEnd',
      'remaining_billing_cycles' => 'remainingBillingCycles',
      'created_at' => 'createdAt',
      'started_at' => 'startedAt',
      'activated_at' => 'activatedAt',
      'cancelled_at' => 'cancelledAt',
      'cancel_reason' => 'cancelReason',
      'due_invoice_count' => 'dueInvoicesCount',
      'due_since' => 'dueSince',
      'total_dues' => 'totalDues',
    );
    $addons = isset($subscription->addons) ? $subscription->addons : NULL;
    $coupons = isset($subscription->coupons) ? $subscription->coupons : NULL;
    $shipping_address = isset($subscription->shippingAddress) ? $subscription->shippingAddress : NULL;
    $subscription_array = _chargebee_object_to_array_convert($subscription, $attributes);
    if ($subscription_array['status'] == 'in_trial') {
      if ($subscription_array['current_term_start'] == NULL) {
        $subscription_array['current_term_start'] = $subscription_array['trial_start'];
      }
      if ($subscription_array['current_term_end'] == NULL) {
        $subscription_array['current_term_end'] = $subscription_array['trial_end'];
      }
    }
    $subscription_array['addons'] = _chargebee_subscription_addons_to_array($addons);
    $subscription_array['coupons'] = _chargebee_subscription_coupons_to_array($coupons);
    $subscription_array['shipping_address'] = _chargebee_subscription_shipping_address_to_array($shipping_address);
  }
  return $subscription_array;
}

/**
 * Convert ChargeBee subscription addons objects to array.
 *
 * @param array $addons
 *   Subscription addons objects array.
 *
 * @return array
 *   Addons array.
 */
function _chargebee_subscription_addons_to_array($addons) {
  $addons_array = array();
  if ($addons) {
    foreach ($addons as $addon) {
      $attributes = array(
        'id' => 'id',
        'quantity' => 'quantity',
      );
      $addons_array[] = _chargebee_object_to_array_convert($addon, $attributes);
    }
  }
  return $addons_array;
}

/**
 * Convert ChargeBee subscription coupons objects to array.
 *
 * @param array $coupons
 *   Subscription addons objects array.
 *
 * @return array
 *   Addons array.
 */
function _chargebee_subscription_coupons_to_array($coupons) {
  $coupons_array = array();
  if ($coupons) {
    foreach ($coupons as $coupon) {
      $attributes = array(
        'id' => 'id',
        'quantity' => 'quantity',
      );
      $coupons_array[] = _chargebee_object_to_array_convert($coupon, $attributes);
    }
  }
  return $coupons_array;
}

/**
 * Convert ChargeBee subscription shipping address to array.
 *
 * @param object $shipping_address
 *   Subscription shipping address object.
 *
 * @return array
 *   Converted array.
 */
function _chargebee_subscription_shipping_address_to_array($shipping_address) {
  $attributes = array(
    'first_name' => 'firstName',
    'last_name' => 'lastName',
    'email' => 'email',
    'company' => 'company',
    'phone' => 'phone',
    'line1' => 'line1',
    'line2' => 'line2',
    'line3' => 'line3',
    'city' => 'city',
    'state' => 'state',
    'country' => 'country',
    'zip' => 'zip',
  );
  $shipping_address_array = _chargebee_object_to_array_convert($shipping_address, $attributes);
  return $shipping_address_array;
}

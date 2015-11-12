<?php
/**
 * @file
 * Plans ChargeBee API functions.
 */

/**
 * Get all plans from ChargeBee.
 */
function _chargebee_plan_get_all() {
  $plans_array = array();
  $plans = ChargeBee_Plan::all();
  foreach ($plans as $entry) {
    $plan = $entry->plan();
    $plans_array[$plan->id] = _chargebee_plan_to_array($plan);
  }
  return $plans_array;
}

/**
 * Retrieve plan from ChargeBee.
 */
function _chargebee_plan_retrieve($plan_id) {
  $result = ChargeBee_Plan::retrieve($plan_id);
  $plan = $result->plan();
  return _chargebee_plan_to_array($plan);
}

/**
 * Convert ChargeBee plan object to array.
 */
function _chargebee_plan_to_array($plan_object) {
  $plan_array = array(
    'plan_id' => $plan_object->id,
    'name' => $plan_object->name,
    'period' => $plan_object->period,
    'period_unit' => $plan_object->periodUnit,
    'invoice_name' => $plan_object->invoiceName,
    'price' => $plan_object->price,
    'trial_period' => $plan_object->trialPeriod,
    'trial_period_unit' => $plan_object->trialPeriodUnit,
    'free_quantity' => $plan_object->freeQuantity,
    'setup_cost' => $plan_object->setupCost,
    'downgrade_penalty' => $plan_object->downgradePenalty,
    'status' => $plan_object->status,
    'archived_at' => $plan_object->archivedAt,
    'billing_cycles' => $plan_object->billingCycles,
    'redirect_url' => $plan_object->redirectUrl,
  );
  return $plan_array;
}

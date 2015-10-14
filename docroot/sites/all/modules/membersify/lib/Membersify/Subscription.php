<?php

/**
 * Membersify_Subscription class
 */
class Membersify_Subscription extends Membersify_ApiResource {
  protected static $object = 'subscription';

  public $id = '';
  public $user_id = 0;
  public $plan_id = '';
  public $payment_profile_id = '';
  public $display = '';
  public $custom = '';
  public $expiration = 0;
  public $start_date = 0;
  public $current_payments = 0;
  public $current_period_start = 0;
  public $next_payment = 0;
  public $total_occurrences = 0;
  public $failed_payments = 0;
  public $payment_plan = array();
  public $livemode = FALSE;
  public $status = '';

  /**
   * Cancels the subscription via the API.
   *
   * @return mixed
   */
  public function cancel() {
    return $this->request('subscription/cancel', array('id' => $this->id));
  }

  /**
   * Renews the subscription via the API.
   *
   * @return mixed
   */
  public function renew() {
    return $this->request('subscription/renew', array('id' => $this->id));
  }

  /**
   * Modifies the subscription to the new plan.
   *
   * @param string $plan_id
   *   The new plan id.
   * @param string $profile_id
   *   The payment profile id.
   *
   * @return mixed
   */
  public function modify($plan_id, $profile_id = NULL) {
    return $this->request('subscription/modify', array('id' => $this->id, 'plan_id' => $plan_id, 'profile_id' => $profile_id));
  }

  /**
   * Changes the payment profile for the subscription.
   *
   * @param string $profile_id
   *   The new profile id.
   *
   * @return mixed
   */
  public function changeBilling($profile_id) {
    return $this->request('subscription/change_billing', array('id' => $this->id, 'profile_id' => $profile_id));
  }
}

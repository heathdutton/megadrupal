<?php

/**
 * Membersify_Membership class
 */
class Membersify_Membership extends Membersify_ApiResource {
  protected static $object = 'membership';

  public $id = '';
  public $user_id = 0;
  public $plan_id = '';
  public $payment_profile_id = '';
  public $username = '';
  public $expiration = 0;
  public $start_date = 0;
  public $current_payments = 0;
  public $next_payment = 0;
  public $total_occurrences = 0;
  public $failed_payments = 0;
  public $payment_plan = array();
  public $livemode = FALSE;
  public $status = '';

  /**
   * Cancels the membership via the API.
   *
   * @return mixed
   */
  public function cancel() {
    return $this->request('membership/cancel', array('id' => $this->id));
  }

  /**
   * Renews the membership via the API.
   *
   * @return mixed
   */
  public function renew() {
    return $this->request('membership/renew', array('id' => $this->id));
  }

  /**
   * Modifies the membership to the new plan.
   *
   * @param string $plan_id
   *   The new plan id.
   *
   * @return mixed
   */
  public function modify($plan_id) {
    return $this->request('membership/modify', array('id' => $this->id, 'plan_id' => $plan_id));
  }

  /**
   * Changes the payment profile for the membership.
   *
   * @param string $profile_id
   *   The new profile id.
   *
   * @return mixed
   */
  public function changeBilling($profile_id) {
    return $this->request('membership/change_billing', array('id' => $this->id, 'profile_id' => $profile_id));
  }
}

<?php

/**
 * The class used for Commerce SP Paymill Subscription entities.
 */
class CommerceSPPaymillSub extends Entity {

  /**
   * The entity id.
   *
   * @var int
   */
  public $id;

  /**
   * The user id of the profile owner.
   *
   * @var int
   */
  public $uid;

  /**
   * The commerce order id
   *
   * @var int
   */
  public $order_id;

  /**
   * The Unix timestamp when the profile was created.
   *
   * @var int
   */
  public $created;

  /**
   * The Unix timestamp when the profile was most recently saved.
   *
   * @var int
   */
  public $changed;

  /**
   * Activation status of the subscription. 0 or 1.
   *
   * @var int
   */
  public $status;

  /**
   * The number of payment that have been made for this subscription.
   *
   * @var int
   */
  public $payment_count;

  /**
   * The Paymill subscription id.
   *
   * @var string
   */
  public $subscription_id;


  public function __construct($values) {
    if (isset($values['user'])) {
      $this->setUser($values['user']);
      unset($values['user']);
    }
    parent::__construct($values, 'commerce_sp_paymill_sub');
  }

  /**
   * Returns the user owning this subscription.
   */
  public function user() {
    return user_load($this->uid);
  }

  /**
   * Sets a new user owning this subscription.
   *
   * @param $account
   *   The user account object or the user account id (uid).
   */
  public function setUser($account) {
    $this->uid = is_object($account) ? $account->uid : $account;
  }

  /**
   * Returns the commerce order related to this subscription.
   */
  public function order() {
    return commerce_order_load($this->order_id);
  }

  public function save() {
    // Care about setting created and changed values. But do not automatically
    // set a created values for already existing entities.
    if (empty($this->created) && (!empty($this->is_new) || !$this->id)) {
      $this->created = REQUEST_TIME;
    }
    $this->changed = REQUEST_TIME;

    parent::save();
  }
}

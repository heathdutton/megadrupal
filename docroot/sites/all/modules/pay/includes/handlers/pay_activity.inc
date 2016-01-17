<?php

/**
 * @file
 * A base class for payment activities.
 */

class pay_activity extends pay {
  var $paid;
  var $pxid;
  var $pmid;
  var $uid;
  var $result;
  var $action;
  var $payment_type;
  var $total;
  var $transaction_total;
  var $timestamp;
  var $hostname;
  var $data = array();
  var $identifier;

  var $table = 'pay_activity';
  var $key = 'paid';

  function set_pay_method($pay_method) {
    $this->pay_method = $pay_method;
    $this->pmid = $pay_method->pmid;
    $this->payment_type = $this->pay_method()->payment_type;
  }

  function set_transaction($transaction) {
    $this->pay_transaction = $transaction;
    $this->pxid = $transaction->pxid;
  }

  function set_action($action) {
    $this->action = check_plain($action);
  }

  function set_transaction_total($val = 0) {
    $this->transaction_total = (float) $val;
  }

  /**
   * Set the $data value, which is stored as a serialized value in the database.
   */
  function set_data($value = NULL) {
    if (!empty($value) && is_scalar($value)) {
      $this->data = unserialize($value);
    }
    else {
      $this->data = $value;
    }
  }

  function currency() {
    return $this->pay_transaction()->currency();
  }

  /**
   * Effect a payment action using the currently-selected payment method.
   *
   * The action must be defined in valid_actions() and the that action's
   * callback must exist for this payment method. Empty actions use a pseudo-
   * action of 'pending'.
   */
  function do_activity($action = NULL, $values = array()) {
    // No action defined: Ensure that this transaction gets logged as 'pending'
    if (!$action || $action == 'pending') {
      $action = 'pending';
      $func = 'pending_action';
    }
    else {
      $info = $this->pay_transaction()->valid_actions($action);
      $func = $info['callback'];
    }
    $this->set_action($action);

    // If the payment method has a function to handle this action, call it!
    if (method_exists($this->pay_method(), $func)) {
      $this->pay_method()->activity = $this;
      $state = $this->pay_method()->$func($values);
    }

    // Save any new/changed data for this activity.
    $this->save();

    // Update this activity's transaction.
    $this->pay_transaction()->update_status($state, $this->timestamp);

    // Return boolean result.
    return $this->result;
  }

  /**
   * The transaction balance as of this payment's completion.
   */
  function balance() {
    return (float) db_query("SELECT t.total - SUM(a.transaction_total)
      FROM {pay_activity} a
      INNER JOIN {pay_transaction} t USING (pxid)
      WHERE t.pxid = :t.pxid AND a.paid <= :a.paid", array(':t.pxid' => $this->pxid, ':a.paid' => $this->paid))->fetchField();
  }

  /**
   * Return the payment method related to this activity.
   */
  function pay_method() {
    if (!isset($this->pay_method)) {
      $this->pay_method = pay_method_load($this->pmid);
    }
    return $this->pay_method;
  }

  /**
   * Return the transaction related to this activity.
   */
  function pay_transaction() {
    if (!isset($this->pay_transaction)) {
      $this->pay_transaction = pay_transaction_load($this->pxid);
    }
    return $this->pay_transaction;
  }

  /**
   * Return a history of all payment activities related to this activity.
   * This is any activity for the current transaction that's using the same
   * payment method.
   */
  function history() {
    if (!isset($this->history)) {
      $history = $this->pay_transaction()->pay_method_activities($this->pmid);
      $this->history = is_array($history) ? $history : array();
    }
    return $this->history;
  }
}

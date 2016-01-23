<?php

/**
 * @file
 * A base class for payment transactions.
 */

class pay_transaction extends pay {
  var $pxid;
  var $pfid;
  var $state;
  var $uid;
  var $created;
  var $completed;
  var $total;
  var $total_paid;
  var $currency;
  var $notes;
  var $notes_format = NULL;
  var $mail;
  var $title;

  var $key = 'pxid';
  var $table = 'pay_transaction';

  /**
   * A list of possible payment states for a transaction.
   *
   * This short list of possible states apply to all transactions, but the
   * underlying activities, controlled by pay_methods, may contain more details.
   */
  function states($state = NULL) {
    $states = array(
      'pending' => array(
        'title' => t('Pending'),
        'description' => t('The transaction has been created, but no successful or unsuccessful payment activities have been applied.'),
      ),
      'active' => array(
        'title' => t('Active'),
        'description' => t('Payment activities are still in progress.'),
      ),
      'complete' => array(
        'title' => t('Complete'),
        'description' => t('The transaction has been completed successfully.'),
      ),
      'canceled' => array(
        'title' => t('Canceled'),
        'description' => t('The transaction has been cancelled.'),
      ),
      'refunded' => array(
        'title' => t('Refunded'),
        'description' => t('The transaction has been refunded.'),
      ),
    );
    if ($state) {
      return $states[$state];
    }
    return $states;
  }

  /**
   * Set the state of this transaction.
   */
  function set_state($state = 'pending') {
    if (array_key_exists($state, $this->states())) {
      $this->state = $state;
    }
  }

  function set_notes_format() {
    $this->notes_format = filter_fallback_format();
    return $this->notes_format;
  }

  /**
   * Return the current state of this transaction.
   */
  function state($title = FALSE) {
    if (!$this->state) {
      $this->set_state();
    }

    if ($title) {
      $info = $this->states($this->state);
      return $info['title'];
    }
    else {
      return $this->state;
    }
  }

  /**
   * Return an array of valid actions for this payment transaction.  This list
   * describes every type of general activity that might be possible, based on
   * this transaction's payment form, and in turn, that form's payment methods.
   *
   * @param $name
   *  The name of an action.  If this is defined, only info for that action
   *  wil be returned.
   */
  function valid_actions($name = NULL) {
    // Create a default list of potential payment actions.
    $actions = array(
      'authorize' => array(
        'title' => t('Authorize'),
        'callback' => 'authorize_action',
        'valid states' => array('pending'),
      ),
      'complete' => array(
        'title' => t('Complete'),
        'message' => t('Transaction completed'),
        'callback' => 'complete_action',
        'valid states' => array('pending', 'active'),
      ),
      'cancel' => array(
        'title' => t('Cancel'),
        'message' => t('Transaction cancelled'),
        'callback' => 'cancel_action',
        'valid states' => array('pending', 'active'),
      ),
      'refund' => array(
        'title' => t('Refund'),
        'message' => t('Transaction has been refunded'),
        'callback' => 'refund_action',
        'valid states' => array('complete'),
      ),
      'delete' => array(
        'title' => t('Delete'),
        'message' => t('Transaction has been deleted'),
      ),
    );

    // Allow my pay_form to alter or augment this list of actions.
    // The pay_form handler also calls set_valid_actions for each pay_method.
    $this->pay_form()->set_valid_actions($this, $actions);

    if ($name) {
      return $actions[$name];
    }
    return $actions;
  }

  /**
   * Return an array of actions that are available for this transaction in its
   * current state.
   *
   * @return array from valid_actions(), filtered by applicability.
   */
  function available_actions() {
    $actions = $this->valid_actions();
    foreach ($actions as $action => $info) {
      if (!$this->valid_action($action)) {
        unset($actions[$action]);
      }
    }
    return $actions;
  }

  /**
   * Determine whether an action is valid and appropriate for this transaction.
   */
  function valid_action($action) {
    $valid = TRUE;

    // Special-case 'delete' pseudo-action.
    if ($action == 'delete') {
      return user_access('administer pay');
    }

    // Is it defined in our list of valid_actions?
    if (!$info = $this->valid_actions($action)) {
      return FALSE;
    }

    // Is the current workflow state compatible with this action?
    if (!in_array($this->state(), $info['valid states'])) {
      return FALSE;
    }

    // Query each payment method in this transaction's history for validity.
    foreach ($this->pay_method_activities() as $pmid => $history) {
      $pay_method = pay_method_load($pmid);
      if (!method_exists($pay_method, $info['callback'])) {
        $valid = FALSE;
      }
      $valid = $valid && $pay_method->valid_action($action, $this, $history);
    }

    return $valid;
  }

  /**
   * Perform an action on this transaction.
   *
   * Actions might include anything returned by $this->available_actions().
   */
  function do_action($action, $context = array()) {
    $result = TRUE;
    if ($action == 'delete') {
      $result = $this->delete();
    }
    elseif ($this->valid_action($action)) {
      // Inflict this action against each payment method that has been used.
      foreach ($this->pay_method_activities() as $pmid => $history) {
        $pay_method = pay_method_load($pmid);

        // Add a new activity with this payment method and execute it.
        if ($pay_method->valid_action($action, $this, $history)) {
          $result = $result && $this->add_activity($pay_method)->do_activity($action);
        }
      }
    }
    return $result;
  }

  function set_title($value = NULL) {
    // This is likely a dummy transaction.
    if (!$this->pay_form()) {
      return;
    }

    if (!$value) {
      $values = array(
        '@form' => $this->pay_form()->title(),
        '!name' => theme('username', array('account' => $this->user()->name)),
        '!date' => format_date($this->created, 'short'),
      );
      $value = t('Payment for "@form" by !name on !date', $values);
    }
    $this->title = filter_xss($value);
  }

  /**
   * Set the transaction currency, based on available options for its pay_form.
   */
  function set_currency($value = NULL) {
    // Use the default currency provided by its pay_form.
    if ($this->pay_form()) {
      if (!$value) {
        $value = $this->pay_form()->currency();
      }
      elseif (!array_key_exists($value, $this->pay_form()->currency_list())) {
        $value = NULL;
      }
    }
    $this->currency = $value;
  }

  function currency() {
    if (!($this->currency)) {
      $this->set_currency();
    }
    return $this->currency;
  }

  function add_activity($pay_method) {
    $pay_activity = pay_activity_load();
    $pay_activity->set_transaction($this);
    $pay_activity->set_pay_method($pay_method);
    $pay_activity->set_total($this->total);
    $pay_activity->save();

    return $pay_activity;
  }

  function update_status($state = NULL, $timestamp = NULL) {
    if (!$timestamp) {
      $timestamp = REQUEST_TIME;
    }

    $this->total_paid = db_query("SELECT SUM(transaction_total)
      FROM {pay_activity} WHERE pxid = :pxid", array(':pxid' => $this->pxid))->fetchField();

    if ($state == 'complete') {
      // Payment has in fact been completed.
      if ($this->total == $this->total_paid) {
        $this->set_completed($timestamp);
        $this->drupal_invoke('pay_transaction_complete');
      }
      // Set state to 'active', since payment is not fully-captured.
      else {
        $state = 'active';
      }
    }

    // Set the new transaction state if it has changed and is valid.
    if ($state != $this->state() && array_key_exists($state, $this->states())) {
      $this->set_state($state);
    }

    $this->save();
  }

  function activity() {
    if (!isset($this->activity)) {
      $this->activity = array();
      $res = db_query("SELECT paid FROM {pay_activity} WHERE pxid = :pxid
        ORDER BY paid", array(':pxid' => $this->pxid));
      while ($paid = $res->fetchField()) {
        $this->activity[$paid] = pay_load_object('pay_activity', $paid);
      }
    }
    return $this->activity;
  }

  /**
   * Delete this transaction.
   */
  function delete() {
    // Call any actions that might want to know about this.
    $this->drupal_invoke('pay_transaction_delete');

    // Delete any activities associated with this transaction.
    db_delete('pay_activity')
      ->condition('pxid', $this->pxid)
      ->execute();

    // Delete this transaction from the database.
    db_delete('pay_transaction')
      ->condition('pxid', $this->pxid)
      ->execute();

    return TRUE;
  }

  /**
   * Return a list of all payment activities in this transaction's history,
   * grouped by payment method.
   */
  function pay_method_activities($pmid = NULL) {
    $activities = array();
    foreach ($this->activity() as $activity) {
      $activities[$activity->pmid][$activity->paid] = $activity;
    }
    if ($pmid) {
      return $activities[$pmid];
    }
    return $activities;
  }
}

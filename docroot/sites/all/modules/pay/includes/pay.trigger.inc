<?php

/**
 * @file
 * Implements hooks that are pertinent to the trigger module.
 */

/**
 * Implementation of hook_trigger_info().
 */
function pay_trigger_info() {
  $hooks = array('pay' => array());

  foreach (pay_handlers('pay_form') as $name => $info) {
    $replace = array('@type' => $info['title']);

    $hooks['pay'][$name . '_transaction_create'] = array(
      'label' => t('@type: A new payment transaction is created but no payment activites have been attempted', $replace),
    );
    $hooks['pay'][$name . '_activity_create'] = array(
      'label' => t('@type: A payment activity has occurred, but the transaction is not yet complete', $replace),
    );
    $hooks['pay'][$name . '_transaction_complete'] = array(
      'label' => t('@type: A payment transaction has been completed', $replace),
    );
    $hooks['pay'][$name . '_goal'] = array(
      'label' => t('@type: The form has a goal amount and that goal has been reached', $replace),
    );
  }
  return $hooks;
}

/**
 * Implementation of hook_pay_transaction_create().
 */
function pay_pay_transaction_create(&$transaction) {
  $pay_form = $transaction->pay_form();
  pay_actions_do($pay_form->handler() . '_transaction_create', $transaction);
}

/**
 * Implementation of hook_pay_activity_create().
 */
function pay_pay_activity_create(&$activity) {
  // To respond by form, we must get the pfid of the transaction and then load
  $transaction = $activity->pay_transaction();
  $pay_form = $transaction->pay_form();
  pay_actions_do($pay_form->handler() . '_activity_create', $transaction);
}

/**
 * Implementation of hook_pay_transaction_complete().
 */
function pay_pay_transaction_complete(&$transaction) {
  $pay_form = $transaction->pay_form();
  pay_actions_do($pay_form->handler() . '_transaction_complete', $transaction);
}

/**
 * Helper function to call 'actions_do'
 */
function pay_actions_do($hook, &$object) {
  if (module_exists('trigger')) {
    $context = array();
    foreach (trigger_get_assigned_actions('pay') as $aid => $action_info) {
      if ($aid) {
        actions_do($aid, $object, $context);
      }
    }
  }
}

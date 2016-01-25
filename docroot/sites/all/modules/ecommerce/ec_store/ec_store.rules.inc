<?php

/**
 * @file
 * Implementation of rules functionality for ec_store.
 */

/**
 * Implements hook_rules_event_info().
 */
function ec_store_rules_event_info() {
  return array(
    'ec_store_event_transactions_bef_save' => array(
		  'group' => t('transaction'),
      'label' => t('Before transaction saved'),
      'module' => 'ec_store',
      'variables' => array(
        'txn' => array(
          'type' => 'ec_transaction',
          'label' => t('Updated transaction'),
        ),
        'orig_txn' => array(
          'type' => 'ec_transaction',
          'label' => t('Unchanged transaction'),
        ),
      ),
    ),
    'ec_store_event_transactions_save' => array(
		  'group' => t('transaction'),
      'label' => t('After transaction saved'),
      'module' => 'ec_store',
      'variables' => array(
        'txn' => array(
          'type' => 'ec_transaction',
          'label' => t('Updated transaction'),
        ),
        'orig_txn' => array(
          'type' => 'ec_transaction',
          'label' => t('Unchanged transaction'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 */
function ec_store_rules_condition_info() {
  return array(
    'ec_store_condition_gross' => array(
      'group' => t('transaction'),
      'label' => t('Transaction gross amount'),
      'parameter' => array(
        'txn' => array(
          'type' => 'ec_transaction',
          'label' => t('Transaction'),
        ),
        'condition' => array(
          'type' => 'list<text>',
          'label' => t('Condition'),
          'options list' => 'ec_store_condition_gross_list',
          'restriction' => 'input',
        ),
        'amount' => array(
          'type' => 'decimal',
          'label' => t('Amount'),
          'restriction' => 'input',
        ),
      ),
      'module' => 'ec_store',
    ),
    'ec_store_condition_workflow_status' => array(
      'group' => t('transaction'),
      'label' => t('Transaction workflow status'),
      'parameter' => array(
        'txn' => array(
          'type' => 'ec_transaction',
          'label' => t('Transaction'),
        ),
        'status' => array(
          'type' => 'list<text>',
          'label' => t('Workflow status'),
          'options list' => 'ec_store_rules_transaction_workflow',
          'description' => t('Select the workflow statuses which the transaction is set to.'),
          'restriction' => 'input',
        ),
      ),
      'module' => 'ec_store',
    ),
    'ec_store_condition_workflow_status_changed' => array(
      'group' => t('transaction'),
      'label' => t('Transaction workflow status changed'),
      'parameter' => array(
        'txn' => array(
          'type' => 'ec_transaction',
          'label' => t('Transaction'),
        ),
        'orig_txn' => array(
          'type' => 'ec_transaction',
          'label' => t('Transaction'),
        ),
      ),
      'module' => 'ec_store',
    ),
    'ec_store_condition_allocation_status' => array(
      'group' => t('transaction'),
      'label' => t('Transaction allocation status'),
      'parameter' => array(
        'txn' => array(
          'type' => 'ec_transaction',
          'label' => t('Transaction'),
        ),
        'status' => array(
          'type' => 'list<text>',
          'label' => t('Allocation status'),
          'options list' => 'ec_store_transaction_allocation',
          'description' => t('Select the allocation statuses which the transaction is set to.'),
          'restriction' => 'input',
        ),
      ),
      'module' => 'ec_store',
    ),
    'ec_store_condition_allocation_status_changed' => array(
      'group' => t('transaction'),
      'label' => t('Transaction allocation status changed'),
      'parameter' => array(
        'txn' => array(
          'type' => 'ec_transaction',
          'label' => t('Transaction'),
        ),
        'orig_txn' => array(
          'type' => 'ec_transaction',
          'label' => t('Transaction'),
        ),
      ),
      'module' => 'ec_store',
    ),
    'ec_store_condition_is_shippable' => array(
      'group' => t('transaction'),
      'label' => t('Transaction is shippable'),
      'parameter' => array(
        'txn' => array(
          'type' => 'ec_transaction',
          'label' => t('Transaction'),
        ),
      ),
      'module' => 'ec_store',
    ),
  );
}

function ec_store_condition_gross_list() {
  return array(
    '=' => t('Equal'),
    '<' => t('Less than'),
    '<=' => t('Less than equal'),
    '>' => t('Greater than'),
    '>=' => t('Greater than equal'),
  );
}

function ec_store_condition_gross_form_alter(&$form, &$form_state, $options) {
  $form['parameter']['condition']['settings']['condition']['#multiple'] = FALSE;
}

/**
 * Condition: Check the gross amount of the transaction
 */
function ec_store_condition_gross($txn, $condition, $amount) {
  $gross = ec_store_transaction_calc_gross($txn);

  switch ($condition[0]) {
    case '=':
      return $amount == $gross;

    case '<':
      return $amount < $gross;

    case '<=':
      return $amount <= $gross;

    case '>':
      return $amount > $gross;

    case '>=':
      return $amount >= $gross;
  }
}

/**
 * Condition: Check transaction status.
 */
function ec_store_condition_workflow_status($txn, $status) {
  if (empty($txn->workflow)) {
    $txn->workflow = 0;
  }
  return in_array($txn->workflow, $status);
}

/**
 * Condition: Transaction workflow status has changed.
 */
function ec_store_condition_workflow_status_changed($txn, $orig_txn) {
  return empty($orig_txn) || !isset($txn->workflow) && !isset($orig_txn->workflow) || isset($txn->workflow) && isset($orig_txn->workflow) && $txn->workflow != $orig_txn->workflow;
}

/**
 * Condition: Check transaction status.
 */
function ec_store_condition_allocation_status($txn, $status) {
  return isset($txn->allocation) && in_array($txn->allocation, $status);
}

/**
 * Condition: Transaction allocation status has changed.
 */
function ec_store_condition_allocation_status_changed($txn, $orig_txn) {
  return (!$orig_txn || !isset($txn->allocation) || !isset($orig_txn->allocation) || $txn->allocation != $orig_txn->allocation);
}

/**
 * Condition: Transaction is shippable.
 */
function ec_store_condition_is_shippable($txn) {
  return isset($txn->shippable) && $txn->shippable ? TRUE : FALSE;
}

/**
 * Implements hook_rules_action_info().
 */
function ec_store_rules_action_info() {
  return array(
    'ec_store_rules_action_set_workflow' => array(
      'group' => t('transaction'),
      'label' => t('Set transaction workflow'),
      'parameter' => array(
        'txn' => array(
          'type' => 'ec_transaction',
          'label' => t('Transaction'),
        ),
        'status' => array(
          'type' => 'list<text>',
          'label' => t('Workflow status'),
          'options list' => 'ec_store_rules_transaction_workflow',
          'description' => t('Select the workflow statuses which the transaction is set to.'),
          'restriction' => 'input',
        ),
      ),
      'module' => 'ec_store',
	  'base' => 'ec_store_rules_action_set_workflow',
    ),
    'ec_store_rules_action_set_allocation' => array(
      'group' => t('transaction'),
      'label' => t('Set transaction allocation'),
      'parameter' => array(
        'txn' => array(
          'type' => 'ec_transaction',
          'label' => t('Transaction'),
        ),
        'status' => array(
          'type' => 'list<text>',
          'label' => t('Allocation status'),
          'options list' => 'ec_store_transaction_allocation',
          'description' => t('Select the allocation statuses which the transaction is set to.'),
          'restriction' => 'input',
        ),
      ),
      'module' => 'ec_store',
	    'base' => 'ec_store_rules_action_set_allocation',
    ),
  );
}

function ec_store_rules_transaction_workflow() {
  return ec_store_transaction_workflow();
}

/**
 * Action: Set transaction workflow.
 */
function ec_store_rules_action_set_workflow($txn, $status) {
  $txn->workflow = reset($status);
}

/**
 * Action: Set transaction allocation.
 */
function ec_store_rules_action_set_allocation($txn, $status) {
  $txn->allocation = reset($status);
}

/**
 * Implements hook_rules_action_info_alter().
 */
function ec_store_rules_action_info_alter(&$actions) {
  unset($actions['rules_core_ec_store_action_set_allocation']);
  unset($actions['rules_core_ec_store_action_set_workflow']);
}

/**
 * Implements hook_rules_data_type_info().
 */
function ec_store_rules_data_info() {
  return array(
    'ec_transaction' => array(
      'label' => t('Transaction'),
      'wrap' => TRUE,
      'property info' => array(),
    ),
  );
}

/*
 * FIXME: 
class ec_store_rules_data_type_transaction extends rules_data_type {
  function save() {
    $txn = & $this->get();
    if (!isset($txn->pass_by_ref)) {
      ec_store_transaction_save($txn);
    }
    return TRUE;
  }

  function load($txnid) {
    return ec_store_transaction_load($txnid);
  }

  function get_identifier() {
    $txn = & $this->get();
    return $txn->txnid;
  }
}
 */

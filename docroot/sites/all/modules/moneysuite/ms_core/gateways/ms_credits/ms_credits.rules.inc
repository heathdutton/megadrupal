<?php
/**
 * @file
 */
// ======================================
// CONDITIONS
// ======================================
/**
 * Conditions:
 * + Check if the balance is greater than X
 */

/**
 * Implements hook_rules_condition_info().
 */
function ms_credits_rules_condition_info() {
  return array(
    'ms_credits_condition_balance_greater_than' => array(
      'label' => t('Check if credits balance is greater than X.'),
      'parameter' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('User')
        ),
        'credits' => array(
          'type' => 'integer',
          'label' => t('Credits')
        )
      ),
      'group' => 'MS Credits',
    ),
  );
}

/**
 * Condition: Check if the balance is greater than X
 */
function ms_credits_condition_balance_greater_than($account, $amount) {
  return $account->ms_credits_balance > $amount;
}

// ======================================
// ACTIONS
// ======================================

/**
 * Actions:
 * + Add an amount to the balance
 */

/**
 * Implements hook_rules_action_info().
 */
function ms_credits_rules_action_info() {
  return array(
    'ms_credits_action_add_balance' => array(
      'label' => t('Add or subtract an amount to the balance'),
      'parameter' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('User'),
          'description' => t("The user to perform this on."),
        ),
        'credits' => array(
          'type' => 'integer',
          'label' => t('Credits'),
          'description' => t("How many credits to add or subtract. Use negative numbers to subtract."),
        ),
        'note' => array(
          'type' => 'text',
          'label' => t('Description'),
          'description' => t("Enter a description that will appear in the transaction."),
        ),
        'created' => array(
          'type' => 'text',
          'label' => t('Created'),
          'description' => t("Enter when the points should be marked as created. Examples: 'Nov 12, 2015', '10-06-18', '+5 months'.
            The string will be passed through strtotime(). Enter 0 to use the current time."),
        ),
        'expiration' => array(
          'type' => 'text',
          'label' => t('Expiration'),
          'description' => t("Enter when the points should expire. Examples: 'Nov 12, 2015', '10-06-18', '+5 months'.
            The string will be passed through strtotime(). Enter 0 for no expiration.
            Relative dates are relative to the created time."),
        ),
      ),
      'group' => 'MS Credits',
    ),
  );
}

/**
 * Action: Add an amount to the balance
 */
function ms_credits_action_add_balance($account, $credits, $note, $created, $expiration) {
  if ($created) {
    $created = strtotime($created, REQUEST_TIME);
  }
  else {
    $created = REQUEST_TIME;
  }
  if ($expiration) {
    $expiration = strtotime($expiration, $created);
  }
  else {
    $expiration = 0;
  }
  return ms_credits_transaction($account->uid, $credits, $note, 0, $created, $expiration);
}

// ======================================
// TRIGGERS
// ======================================

// ======================================
// DATA TYPES
// ======================================


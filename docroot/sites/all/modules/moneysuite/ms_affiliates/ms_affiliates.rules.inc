<?php

/**
 * @file
 * Rules functions for the Affiliates Suite module.
 */

// ======================================
// CONDITIONS
// ======================================
/*
 * Conditions:
 * + Check if the balance is greater than X
 */

/**
 * Implements hook_rules_condition_info().
 */
function ms_affiliates_rules_condition_info() {
  return array(
    'ms_affiliates_condition_balance_greater_than' => array(
      'label' => t('Check if affiliate balance is greater than X.'),
      'parameter' => array(
        'account' => array('type' => 'user', 'label' => t('User')),
        'amount' => array('type' => 'decimal', 'label' => t('Amount'))
      ),
      'group' => 'Affiliates Suite',
    ),
  );
}

/**
 * Condition: Check if the balance is greater than X
 */
function ms_affiliates_condition_balance_greater_than($account, $amount) {
  return $account->af_balance > $amount;
}

// ======================================
// ACTIONS
// ======================================

/*
 * Actions:
 * + Add an amount to the balance
 * + Set the balance to an amount
 * + Set or change the affiliate of a user
 * + Get the affiliate of a user
 */

/**
 * Implements hook_rules_action_info().
 * @ingroup rules
 */
function ms_affiliates_rules_action_info() {
  return array(
    'ms_affiliates_action_add_balance' => array(
      'label' => t('Add or subtract an amount to the balance'),
      'parameter' => array(
        'account' => array('type' => 'user', 'label' => t('User')),
        'amount' => array('type' => 'decimal', 'label' => t('Amount')),
        ),
      'group' => 'Affiliates Suite',
    ),
    'ms_affiliates_action_get_affiliate' => array(
      'label' => t('Load the affiliate of a user'),
      'parameter' => array(
        'account' => array('type' => 'user', 'label' => t('User')),
      ),
      'provides' => array(
        'affiliate' => array(
          'type' => 'user',
          'label' => t('Affiliate'),
        ),
      ),
      'group' => 'Affiliates Suite',
    ),
    'ms_affiliates_action_set_balance' => array(
      'label' => t('Set the affiliate balance'),
      'parameter' => array(
        'account' => array('type' => 'user', 'label' => t('User')),
        'amount' => array('type' => 'decimal', 'label' => t('Amount')),
        ),
      'group' => 'Affiliates Suite',
    ),
    'ms_affiliates_action_set_affiliate' => array(
      'label' => t('Set the affiliate for a user'),
      'parameter' => array(
        'account' => array('type' => 'user', 'label' => t('User')),
        'affiliate' => array('type' => 'integer', 'label' => t('Affiliate')),
        ),
      'group' => 'Affiliates Suite',
    ),
  );
}

/**
* Action: Get the affiliate for a user
*/
function ms_affiliates_action_get_affiliate($account) {
  $affiliate = user_load(ms_affiliates_get_affiliate($account->uid));
  return array('affiliate' => $affiliate);
}

/**
 * Action: Add an amount to the balance
 */
function ms_affiliates_action_add_balance($account, $amount) {
  return ms_affiliates_update_balance($account->uid, $amount);
}

/**
 * Action: Set the balance to an amount
 */
function ms_affiliates_action_set_balance($account, $amount) {
  return ms_affiliates_update_balance($account->uid, $amount, TRUE);
}

/**
 * Action: Set the affiliate of a user
 */
function ms_affiliates_action_set_affiliate($account, $affiliate) {
  ms_affiliates_set_affiliate($account, $affiliate);
}

// ======================================
// TRIGGERS
// ======================================

/*
 * Triggers:
 * + Commission has been granted to a user
 * + User gets a new referral signup
 */

/**
 * Implements hook_rules_event_info()
 */
function ms_affiliates_rules_event_info() {
  $items = array(
    'ms_affiliates_event_commission_granted' => array(
      'label' => t('Commission Granted'),
      'group' => 'Affiliates Suite',
      'help' => t("Act when commission is granted to a user"),
      'variables' => array(
        'commission' => array('type' => 'decimal', 'label' => t('Commission Amount')),
        'level' => array('type' => 'integer', 'label' => t('Level')),
        'amount' => array('type' => 'decimal', 'label' => t('Order Amount')),
        'account' => array('type' => 'user', 'label' => t('Account')),
      ),
    ),
    'ms_affiliates_event_referral_signup' => array(
      'label' => t('Referral Signup'),
      'group' => 'Affiliates Suite',
      'help' => t("Act when a referral signs up under an affiliate"),
      'variables' => array(
        'referral' => array('type' => 'user', 'label' => t('Referral')),
        'affiliate' => array('type' => 'user', 'label' => t('Affiliate')),
      ),
    ),
  );

  return $items;
}


// ======================================
// DATA TYPES
// ======================================


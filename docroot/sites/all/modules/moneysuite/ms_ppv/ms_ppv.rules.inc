<?php
// ======================================
// CONDITIONS
// ======================================

/*
 * Conditions:
 * + Check if a user has access to a node
 */

/**
 * Implements hook_rules_condition_info().
 * @ingroup rules
 */
function ms_ppv_rules_condition_info() {
  return array(
    'ms_ppv_condition_user_has_access' => array(
      'label' => t('Check if a user has access to a node'),
      'parameter' => array(
        'node' => array( 'type' => 'node', 'label' => t('Node')),
        'account' => array('type' => 'user', 'label' => t('User')),
      ),
      'group' => 'Pay Per View',
    ),
  );
}

/**
 * Condition: Check if a user has access to the node
 */
function ms_ppv_condition_user_has_access($node, $account) {
  return ms_ppv_has_access($account->uid, $node->nid);
}

// ======================================
// ACTIONS
// ======================================

/*
 * Actions:
 * + Add access to a node for a user
 * + Remove access to a node from a user
 */

/**
 * Implements hook_rules_action_info().
 * @ingroup rules
 */
function ms_ppv_rules_action_info() {
  return array(
    'ms_ppv_action_add_access' => array(
      'label' => t('Add Pay Per View access to a node for a user'),
      'parameter' => array(
        'node' => array('type' => 'node', 'label' => t('Node')),
        'account' => array('type' => 'user', 'label' => t('User')),
        'expiration' => array('type' => 'date', 'label' => t('Expiration Date')),
      ),
      'group' => 'Pay Per View',
    ),
    'ms_ppv_action_remove_access' => array(
      'label' => t('Remove Pay Per View access to a node from a user'),
      'parameter' => array(
        'node' => array('type' => 'node', 'label' => t('Node')),
        'account' => array('type' => 'user', 'label' => t('User')),
      ),
      'group' => 'Pay Per View',
    ),
  );
}

/**
 * Action: Add access to a node for a user
 */
function ms_ppv_action_add_access($node, $account, $expiration) {
  ms_ppv_insert_access($account->uid, $node->nid, 0, (int) $expiration);
}

/**
 * Action: Remove access to a node for a user
 */
function ms_ppv_action_remove_access($node, $account) {
  ms_ppv_delete_access($account->uid, $node->nid);
}

// ======================================
// TRIGGERS
// ======================================

/*
 * Triggers:
 * + The user is granted access to a node
 * + The user is removed from access to a node
 */

/**
 * Implements hook_rules_event_info()
 */
function ms_ppv_rules_event_info() {
  $items = array(
    'ms_ppv_event_access_granted' => array(
      'label' => t('Pay Per View Node Access Granted'),
      'group' => 'Pay Per View',
      'help' => t("Act when a user is granted access to a Pay Per View Node."),
      'variables' => array(
        'node' => array('type' => 'node', 'label' => t('Node')),
        'order' => array('type' => 'ms_order', 'label' => t('Order', array(), array('context' => 'ms_core'))),
        'expiration' => array('type' => 'decimal', 'label' => t('Expiration Timestamp')),
        'account' => array('type' => 'user', 'label' => t('User Account')),
      ),
    ),
    'ms_ppv_event_access_removed' => array(
      'label' => t('Pay Per View Node Access Removed'),
      'group' => 'Pay Per View',
      'help' => t("Act when a user is removed access to a Pay Per View Node."),
      'variables' => array(
        'node' => array('type' => 'node', 'label' => t('Node')),
        'account' => array('type' => 'user', 'label' => t('User Account')),
      ),
    ),
    'ms_ppv_event_field_access_granted' => array(
      'label' => t('Pay Per View Field Access Granted'),
      'group' => 'Pay Per View',
      'help' => t("Act when a user is granted access to a Pay Per View Field."),
      'variables' => array(
        'entity_type' => array('type' => 'text', 'label' => t('Entity Type')),
        'entity_id' => array('type' => 'text', 'label' => t('Entity ID')),
        'field_name' => array('type' => 'text', 'label' => t('Field Name')),
        'order' => array('type' => 'ms_order', 'label' => t('Order', array(), array('context' => 'ms_core'))),
        'expiration' => array('type' => 'decimal', 'label' => t('Expiration Timestamp')),
        'account' => array('type' => 'user', 'label' => t('User Account')),
      ),
    ),
    'ms_ppv_event_field_access_removed' => array(
      'label' => t('Pay Per View Field Access Removed'),
      'group' => 'Pay Per View',
      'help' => t("Act when a user loses access to a Pay Per View Field."),
      'variables' => array(
        'entity_type' => array('type' => 'text', 'label' => t('Entity Type')),
        'entity_id' => array('type' => 'text', 'label' => t('Entity ID')),
        'field_name' => array('type' => 'text', 'label' => t('Field Name')),
        'account' => array('type' => 'user', 'label' => t('User Account')),
      ),
    ),
  );

  return $items;
}

// ======================================
// DATA TYPES
// ======================================



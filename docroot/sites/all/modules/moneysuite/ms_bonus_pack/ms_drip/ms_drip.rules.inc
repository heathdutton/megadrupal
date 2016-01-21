<?php
// ======================================
// CONDITIONS
// ======================================

/*
 * Conditions:
 * + Check if user has an active set
 */

/**
 * Implements hook_rules_condition_info().
 * @ingroup rules
 */
function ms_drip_rules_condition_info() {
  return array(
    'ms_drip_condition_user_has_set' => array(
      'label' => t('Check if a user has an active Drip Set'),
      'parameter' => array(
        'set_id' => array('type' => 'integer', 'label' => t('Drip Set'), 'options list' => 'ms_drip_get_sets_list'),
        'account' => array('type' => 'user', 'label' => t('User')),
      ),
      'group' => 'MS Drip',
    ),
  );
}

/**
 * Condition: Check if a user has the set
 */
function ms_drip_condition_user_has_set($set_id, $account) {
  if (ms_drip_user_has_set($set_id, $account->uid)) {
    return TRUE;
  }
  return FALSE;
}

// ======================================
// ACTIONS
// ======================================


/*
 * Actions:
 * + Start Set for User
 * + End Set for User
 */

/**
 * Implements hook_rules_action_info().
 * @ingroup rules
 */
function ms_drip_rules_action_info() {
  return array(
    'ms_drip_action_start_drip_set' => array(
      'label' => t('Start a Drip Set for a User'),
      'parameter' => array(
        'set_id' => array('type' => 'integer', 'label' => t('Drip Set'), 'options list' => 'ms_drip_get_sets_list'),
        'account' => array('type' => 'user', 'label' => t('User')),
      ),
      'group' => 'MS Drip',
    ),
    'ms_drip_action_schedule_drip_set' => array(
      'label' => t('Schedule a Drip Set for a User'),
      'parameter' => array(
        'set_id' => array('type' => 'integer', 'label' => t('Drip Set'), 'options list' => 'ms_drip_get_sets_list'),
        'account' => array('type' => 'user', 'label' => t('User')),
        'start' => array('type' => 'date', 'label' => t('Start Date')),
      ),
      'group' => 'MS Drip',
    ),
    'ms_drip_action_end_drip_set' => array(
      'label' => t('End a Drip Set for a User'),
      'parameter' => array(
        'set_id' => array('type' => 'integer', 'label' => t('Drip Set'), 'options list' => 'ms_drip_get_sets_list'),
        'account' => array('type' => 'user', 'label' => t('User')),
      ),
      'group' => 'MS Drip',
    ),
  );
}

/**
* Action: Schedule a set for a User
*/
function ms_drip_action_schedule_drip_set($set_id, $account, $start) {
  // Schedule the set for this user
  ms_drip_start_set($set_id, $account->uid, $start);
}

/**
 * Action: Start a set for a User
 */
function ms_drip_action_start_drip_set($set_id, $account) {
  // Start the set for this user
  ms_drip_start_set($set_id, $account->uid);
}

/**
 * Action: Start a set for a User
 */
function ms_drip_action_end_drip_set($set_id, $account) {
  // End the set for this user
  ms_drip_end_set($set_id, $account->uid);
}

// ======================================
// TRIGGERS
// ======================================


/**
 * Implements hook_rules_event_info()
 */
function ms_drip_rules_event_info() {
  $events = array();

  $events['ms_drip_item_event'] = array(
    'label' => t('MS Drip Item Trigger'),
    'group' => 'MS Drip',
    'help' => t("Triggers on the correct day of the subscription."),
    'variables' => array(
      'account' => array('type' => 'user', 'label' => t('Subscriber')),
    ),
  );

  /*
  $sets = ms_drip_get_sets();
  foreach ($sets as $set) {
    $items = ms_drip_get_set_items($set->id);

    foreach ($items as $item) {
      $events['ms_drip_event_' . $set->id . '_' . $item->id] = array(
        'label' => ms_drip_get_item_rule_name($set, $item),
        'group' => 'MS Drip Content',
        'help' => t("Triggers on day @day of a subscription.", array('@day' => $item->day)),
        'variables' => array(
          'account' => array(
            'type' => 'user',
            'label' => t('User Account'),
          ),
        ),
      );
    }

  }
 */

  return $events;
}

// ======================================
// DATA TYPES
// ======================================

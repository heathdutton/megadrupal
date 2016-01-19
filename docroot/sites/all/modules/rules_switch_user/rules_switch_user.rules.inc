<?php

/**
 * @file
 * Example Rules code: actions, conditions and events.
 */

/**
 * Implements hook_rules_action_info().
 *
 * Declares any meta-data about actions for Rules in a big, associative, nested
 * array. See also hook_rules_action_info in the rules.api.php file, and the
 * online documentation at http://drupal.org/node/878928.
 */
function rules_switch_user_rules_action_info() {
  $actions = array(
    'rules_switch_user_action_switch_to' => array(
      'label' => t('Switch to another user'),
      'group' => t('User switching'),
      'parameter' => array(
        // Each parameter is keyed with an internally used machine name.
        'account' => array(
          'type' => 'user',
          'label' => t('User to switch to'),
          // If we were to make changes to the parameter object, whe should set
          // the 'save' property to TRUE. This makes Rules handle the saving in
          // a more optimized way than if each action saved the objects.
          // 'save' => TRUE,
        ),
      ),
    ),
    'rules_switch_user_action_switch_back' => array(
      'label' => t('Switch back from another user'), // Name displayed to admins
      'group' => t('User switching'), // Used for grouping actions in select lists
    ),
  );

  return $actions;
}


/**
 * The action function for the 'rules_example_action_hello_user'.
 *
 * The $accout parameter is the user object sent into this action, selected in
 * the Rules configuration interface.
 * @param <object> $account
 *   The user object to work with. Selected by the site administrator when the
 *   action is configured.
 */
function rules_switch_user_action_switch_to($account) {
  _rules_switch_user_engine($account);
}

function rules_switch_user_action_switch_back() {
  _rules_switch_user_engine();
}
///TODO error reporting
function _rules_switch_user_engine($account = NULL) {
  global $user;
  static $_original_user, $_old_state;

  if (is_null($account)) {
    ///TODO check $_original_user, $_old_state are set
    $user = $_original_user;
    drupal_save_session($_old_state);
    $_original_user = $_old_state = NULL;
  } else {
    ///TODO check $_original_user, $_old_state are NOT set
    $_original_user = $user;
    $_old_state = drupal_save_session();
    drupal_save_session(FALSE);
    $user = user_load($account->uid);
  }
  return true;
}

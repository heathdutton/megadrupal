<?php
/**
 * @file Rules integration for the Oempro module.
 */

/**
 * Implements hook_rules_action_info().
 */
function oempro_lists_rules_condition_info() {

  $items['oempro_lists_mail_is_subscribed'] = array(
    'label' => t('E-Mail is subscribed to a Oempro list'),
    'parameter' => array(
      'mail' => array(
        'type' => 'mail',
        'label' => t('E-Mail address'),
        'description' => t('The e-mail address for which to check whether it is subscribed to a list.'),
        'default mode' => 'selector',
      ),
      'list' => array(
        'type' => 'oempro_list',
        'label' => t('Oempro list'),
        'default mode' => 'input',
      ),
    ),
    'group' => t('Oempro'),
    'access callback' => 'oempro_lists_rules_access_callback',
    'base' => 'oempro_lists_rules_condition_mail_is_subscribed',
  );
  return $items;
}

/**
 * Condition callback: User is subscribed to a list.
 */
function oempro_lists_rules_condition_mail_is_subscribed($mail, $list) {
  return oempro_is_subscribed($list->oempro_list_id, $mail);
}

/**
 * Implements hook_rules_action_info().
 */
function oempro_lists_rules_action_info() {

  $items['oempro_lists_user_subscribe_list'] = array(
    'label' => t('Subscribe user to a oempro list'),
    'parameter' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('User'),
        'description' => t('The user to subscribe to a list.'),
      ),
      'list' => array(
        'type' => 'oempro_list',
        'label' => t('Oempro list'),
        'default mode' => 'input',
      ),
    ),
    'group' => t('Oempro'),
    'access callback' => 'oempro_lists_rules_access_callback',
    'base' => 'oempro_lists_rules_action_user_subscribe_list',
  );
  $items['oempro_lists_unsubscribe_list'] = array(
    'label' => t('Unsubscribe mail address from a oempro list'),
    'parameter' => array(
      'mail' => array(
        'type' => 'text',
        'label' => t('E-Mail address'),
        'description' => t('The mail address to unsubscribe from the given list.'),
        'default mode' => 'selector',
      ),
      'list' => array(
        'type' => 'oempro_list',
        'label' => t('Oempro list'),
        'default mode' => 'input',
      ),
    ),
    'group' => t('Oempro'),
    'access callback' => 'oempro_lists_rules_access_callback',
    'base' => 'oempro_lists_rules_action_unsubscribe_list',
  );
  return $items;
}

/**
 * Action callback: Subscribe a user to a list.
 */
function oempro_lists_rules_action_user_subscribe_list($account, $list) {
  $merge_variables = oempro_lists_load_user_mergevars($account, $list);
  oempro_subscribe_user($list, $account->mail, $merge_variables, FALSE);
}

/**
 * Action callback: Unsubscribe a user to a list.
 */
function oempro_lists_rules_action_unsubscribe_list($mail, $list) {
  oempro_unsubscribe_user($list, $mail, FALSE);
}

/**
 * Access callback for the rules integration.
 */
function oempro_lists_rules_access_callback() {
  return user_access('administer oempro');
}

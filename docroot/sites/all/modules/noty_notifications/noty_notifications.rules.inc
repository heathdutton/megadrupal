<?php

/**
 * @file
 * Rules code: actions, conditions and events.
 */

/**
 * Implements hook_rules_action_info().
 */
function noty_notifications_rules_action_info() {
  $actions = array(
    'noty_notifications_add_notification_action' => array(
      'label' => t('Add new notification to notifications queue for user'),
      'group' => t('Noty Notifications'),
      'parameter' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('User for which is a notification addressed'),
          // 'save' => TRUE,
        ),
        'noty_message' => array(
          'type' => 'text',
          'label' => t('Notification message'),
          'restriction' => 'input',
        ),
        'noty_type' => array(
          'type' => 'list<text>',
          'label' => t('Type of notification'),
          'restriction' => 'input',
          'options list' => 'noty_notifications_get_types',
          'default value' => NOTY_NOTIFICATIONS_TYPE_INFORMATION,
        ),
        'removable' => array(
          'type' => 'boolean',
          'label' => t('Removable'),
          'restriction' => 'input',
          'default value' => TRUE,
        ),
      ),
      'base' => 'noty_notifications_add_notification_action',
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
function noty_notifications_add_notification_action($account, $noty_message, $noty_type, $removable) {
  noty_notifications_add_notification_to_database($account->uid, $noty_message, array(), $noty_type);
}
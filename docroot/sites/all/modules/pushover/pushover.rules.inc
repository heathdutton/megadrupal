<?php
/**
 * @file
 * Enable Rules support for Pushover Module
 */

/**
 * Implements hook_rules_action_info().
 */
function pushover_rules_action_info() {
  $actions = array(
    'pushover_to_role' => array(
      'label' => t('Send push notification to all users of a role'),
      'group' => t('Pushover'),
      'access callback' => 'pushover_rule_access',
      'base' => 'pushover_send_to_all_rule',
      'parameter' => array(
        'roles' => array(
          'type' => 'list<integer>',
          'label' => t('Roles'),
          'options list' => 'entity_metadata_user_roles',
          'description' => t('Select the roles whose users should receive notification, if they have entered their Pushover key.'),
        ),
        'message' => array(
          'type' => 'text',
          'label' => t('Message'),
          'description' => t("Notification Message"),
        ),
        'priority' => array(
          'type' => 'integer',
          'label' => t('Priority'),
          'options list' => 'pushover_priority_list',
          'description' => t('Select what priority the messages should get.'),
          'default value' => '0',
        ),
        'subject' => array(
          'type' => 'text',
          'label' => t('Subject'),
          'optional' => TRUE,
          'description' => t("Notification Title."),
        ),
        'url' => array(
          'type' => 'uri',
          'label' => t('Url'),
          'optional' => TRUE,
          'description' => t("A full url you want to send with the notification"),
        ),
        'url_title' => array(
          'type' => 'text',
          'label' => t('Url Title'),
          'optional' => TRUE,
          'description' => t("The title of the url, if blank the url itself will be visible."),
        ),
        'timestamp' => array(
          'type' => 'date',
          'label' => t('Timestamp'),
          'optional' => TRUE,
          'description' => t("Unix Timestamp you want to display in the notification instead of the time it's sent."),
        ),
      ),
    ),
    'pushover_to_user' => array(
      'label' => t('Send push notification to specific users.'),
      'group' => t('Pushover'),
      'access callback' => 'pushover_rule_access',
      'base' => 'pushover_send_to_user_rule',
      'parameter' => array(
        'user' => array(
          'type' => 'text',
          'label' => t('Usernames'),
          'description' => t('Fill in one username per line, if they have entered a key they will get notification.'),
        ),
        'message' => array(
          'type' => 'text',
          'label' => t('Message'),
          'description' => t("Notification Message"),
        ),
        'priority' => array(
          'type' => 'integer',
          'label' => t('Priority'),
          'options list' => 'pushover_priority_list',
          'description' => t('Select what priority the messages should get.'),
          'default value' => '0',
        ),
        'subject' => array(
          'type' => 'text',
          'label' => t('Subject'),
          'optional' => TRUE,
          'description' => t("Notification Title."),
        ),
        'url' => array(
          'type' => 'uri',
          'label' => t('Url'),
          'optional' => TRUE,
          'description' => t("A full url you want to send with the notification"),
        ),
        'url_title' => array(
          'type' => 'text',
          'label' => t('Url Title'),
          'optional' => TRUE,
          'description' => t("The title of the url, if blank the url itself will be visible."),
        ),
        'timestamp' => array(
          'type' => 'date',
          'label' => t('Timestamp'),
          'optional' => TRUE,
          'description' => t("The time and date you want to display in the notification instead of the time it's sent."),
        ),
      ),
    ),
  );
  return $actions;
}

/**
 * Limit access to user who have the correct permission.
 */
function pushover_rule_access() {
  return user_access('administer pushover');
}

/**
 * The available priority levels.
 */
function pushover_priority_list() {
  return array(
    '0'   => t('Normal'),
    '1'   => t('High'),
    '-1'  => t('Low'),
    '2'   => t('Emergency (Requires confirmation)'),
  );
}

/**
 * Send notification to every user of a group.
 */
function pushover_send_to_all_rule($roles, $message, $priority, $subject = NULL, $url = NULL, $url_title = NULL, $timestamp = NULL) {
  $users = array();

  foreach ($roles as $role) {
    $result = db_select('users_roles', 'u')
    ->fields('u', array('uid'))
    ->condition('rid', $role, '=')
    ->execute()
    ->fetchAll();
    foreach ($result as $user) {
      $uid = $user->uid;
      $users[] = $uid;
    }
  }
  $ok_users = array_unique($users);
  $push_keys = db_select('field_data_field_pushover_user_key', 'p')
    ->fields('p', array('entity_id', 'field_pushover_user_key_value'))
    ->execute()
    ->fetchAll();

  foreach ($push_keys as $user_with_key) {
    if (in_array($user_with_key->entity_id, $ok_users)) {
      $userkey = $user_with_key->field_pushover_user_key_value;
      $args = _pushover_user_field_info($user_with_key->entity_id);
      _pushover_send_notification($userkey, $subject, $message, $priority, $url, $url_title, $args['sound'], $timestamp, $args['device']);
    }
  }
}

/**
 * Send notification to specific users.
 */
function pushover_send_to_user_rule($users, $message, $priority, $subject = NULL, $url = NULL, $url_title = NULL, $timestamp = NULL) {
  if (is_string($users)) {
    if (strpos($users, chr(13) . chr(10))) {
      $user_array = explode(chr(13) . chr(10), $users);
    }
    elseif (strpos($users, chr(13))) {
      $user_array = explode(chr(13), $users);
    }
    else {
      $user_array = explode(chr(10), $users);
    }
  }
  foreach ($user_array as $user) {
    $userobj = user_load_by_name($user);
    $pushkey = field_get_items('user', $userobj, 'field_pushover_user_key');
    $userkey = $pushkey[0]['value'];
    $args = _pushover_user_field_info($userobj->uid);

    _pushover_send_notification($userkey, $subject, $message, $priority, $url, $url_title, $args['sound'], $timestamp, $args['device']);
  }
}

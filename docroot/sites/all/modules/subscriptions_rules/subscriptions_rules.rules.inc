<?php

/**
 * @file rules integration for the subscriptions_rules module
 */

/**
 * Implements hook_rules_action_info() on behalf of the node module.
 */
function subscriptions_rules_rules_action_info() {
  $base = array(
    'type' => 'subscriptions',
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t('Content')),
      'user' => array('type' => 'user', 'label' => t('User')),
      'type' => array(
        'type' => 'text',
        'label' => t('Subscription type'),
        'optional' => TRUE,
        'options list' => 'subscriptions_rules_rules_type_list',
        'default value' => 'node',
      ),
      'send_interval' => array(
        'type' => 'textfield',
        'label' => t('Send Interval'),
        'optional' => TRUE,
        'description' => t('Leave blank to use user\'s setting. This will be in calculated in seconds.'),
      ),
    ),
    'group' => t('Subscriptions'),
    'access callback' => 'subscriptions_rules_add_user_content_access',
  );

  $type_base = array(
    'type' => 'subscriptions',
    'parameter' => array(
      'type' => array(
        'type' => 'list<text>',
        'label' => t('Content types'),
        'options list' => 'node_type_get_names',
        'description' => t('The content type(s) to check for.'),
        'restriction' => 'input',
      ),
      'user' => array('type' => 'user', 'label' => t('User')),
      'send_interval' => array(
        'type' => 'textfield',
        'label' => t('Send Interval'),
        'optional' => TRUE,
        'description' => t('Leave blank to use user\'s setting. This will be in calculated in seconds.'),
      ),
    ),
    'group' => t('Subscriptions'),
    'access callback' => 'subscriptions_rules_add_user_content_access',
  );

  // Action subsribe a user to a content
  $items['subscriptions_rules_add_user_content'] = array(
    'label' => t('Subscribe user to node'),
    'base' => 'subscriptions_rules_add_user_content_callback',
  ) + $base;

  // Action unsubsribe a user to a content
  $items['subscriptions_rules_remove_user_content'] = array(
    'label' => t('Unsubscribe user from node'),
    'base' => 'subscriptions_rules_remove_user_content_callback',
  ) + $base;

  // Action subsribe a user to a content type
  $items['subscriptions_rules_add_user_content_type'] = array(
    'label' => t('Subscribe user to node type'),
    'base' => 'subscriptions_rules_add_user_content_type_callback',
  ) + $type_base;

  // Action unsubsribe a user to a content type
  $items['subscriptions_rules_remove_user_content_type'] = array(
    'label' => t('Unsubscribe user from node type'),
    'base' => 'subscriptions_rules_remove_user_content_type_callback',
  ) + $type_base;

  $items['subscriptions_rules_stop_sending'] = array(
    'label' => t('Stop email sending'),
    'base' => 'subscriptions_rules_stop_sending_callback',
    'parameter' => array(
      'clear' => array(
        'type' => 'boolean',
        'label' => t('Clear existed mail queue'),
        'optional' => TRUE,
        'description' => t('Clear existed mail queue before stopping email sending'),
      ),
    ),
    'access callback' => 'subscriptions_rules_stop_sending_callback_access',
  ) + $base;

  $items['subscriptions_rules_start_sending'] = array(
    'label' => t('Start email sending'),
    'base' => 'subscriptions_rules_start_sending_callback',
    'parameter' => array(), // TODO: Add checkbox to clear existed queue.
    'access callback' => 'subscriptions_rules_stop_sending_callback_access',
  ) + $base;

  $items['subscriptions_rules_subscription_suspend'] = array(
    'label' => t('Suspend subscription'),
    'base' => 'subscriptions_rules_subscription_suspend_callback',
    'parameter' => array(
      'user' => array('type' => 'user', 'label' => t('User')),
    ),
    'access callback' => 'subscriptions_rules_stop_sending_callback_access',
  ) + $base;

  $items['subscriptions_rules_subscription_resume'] = array(
    'label' => t('Resume subscription'),
    'base' => 'subscriptions_rules_subscription_resume_callback',
    'parameter' => array(
      'user' => array('type' => 'user', 'label' => t('User')),
    ),
    'access callback' => 'subscriptions_rules_stop_sending_callback_access',
  ) + $base;

  return $items;
}

/**
 * Rule subscriptions_add_user_content callback
 */
function subscriptions_rules_add_user_content_callback($node, $user, $type = 'node', $send_interval = "") {
  // Get existed
  $sid = subscriptions_get_subscription($user->uid, 'node', 'nid', $node->nid);
  $args = array (
    0 => 'node',
    1 => 'nid',
    2 => $node->nid,
    3 => -1, // DEBUG $node->uid,
    4 => $user->uid,
    5 => is_numeric($send_interval) ? $send_interval : _subscriptions_get_setting('send_interval', $user),
    6 => $type == 'node',
    7 => $type == 'comment',
  );

  if ($sid) {
    // Add more value to existed subscription_content row
    $update = db_update('subscriptions')
    ->condition('module', $args[0])
    ->condition('field', $args[1])
    ->condition('value', $args[2])
    ->condition('author_uid', $args[3])
    ->condition('recipient_uid', $args[4]);

    if ($args[6]) {
      $update->fields(array(
        'send_updates' => 1,
      ));
    }

    if ($args[7]) {
      $update->fields(array(
        'send_comments' => 1,
      ));
    }

    $ret = $update->execute();
  }
  else {
    $ret = call_user_func_array('subscriptions_write_subscription', $args);
  }

  return $ret;
}

/**
 * Rule subscriptions_remove_user_content callback
 */
function subscriptions_rules_remove_user_content_callback($node, $user, $type = 'node') {
  $args = array (
    0 => 'node',
    1 => 'nid',
    2 => $node->nid,
    3 => -1, // DEBUG $node->uid,
    4 => $user->uid,
    5 => _subscriptions_get_setting('send_interval', $user),
    6 => $type == 'node',
    7 => $type == 'comment',
  );

  // Delete single rows
  $delete = db_delete('subscriptions')
    ->condition('module', $args[0])
    ->condition('field', $args[1])
    ->condition('value', $args[2])
    ->condition('author_uid', $args[3])
    ->condition('recipient_uid', $args[4]);

  // Update row that has both comments and update
  $update = db_update('subscriptions')
    ->condition('module', $args[0])
    ->condition('field', $args[1])
    ->condition('value', $args[2])
    ->condition('author_uid', $args[3])
    ->condition('recipient_uid', $args[4]);

  // Remove node rows
  if ($args[6]) {
    $delete->condition('send_updates', 1);
    $delete->condition('send_comments', 0);
    $update->fields(array(
      'send_updates' => 0
    ));
  }

  // Remove comments rows
  if ($args[7]) {
    $delete->condition('send_updates', 0);
    $delete->condition('send_comments', 1);
    $update->fields(array(
      'send_comments' => 0,
    ));
  }

  $ret = $delete->execute() || $update->execute();

  return $ret;
}

/**
 * TODO: user_access
 */
function subscriptions_rules_add_user_content_access() {
  return TRUE;
}

/**
 * Options list callback for the operation parameter of condition user has role.
 */
function subscriptions_rules_rules_type_list() {
  return array(
    'node' => t('Node'),
    'comment' => t('Comment'),
  );
}

/**
 * Options list callback for the operation parameter of condition user has role.
 */
function subscriptions_rules_rules_type_type_list() {
  return array(
    'node' => t('wiki'),
    'comment' => t('Comment'),
  );
}

/**
 * Rule subscriptions_rules_add_user_content_type callback
 */
function subscriptions_rules_add_user_content_type_callback($node_types, $user, $send_interval = "") {
  $ret = TRUE;
  // Load existed subscriptions
  foreach ($node_types as $type) {
    $sid = subscriptions_get_subscription($user->uid, 'node', 'type', $type);
    $args = array (
      0 => 'node',
      1 => 'type',
      2 => $type,
      3 => -1, // DEBUG $node->uid,
      4 => $user->uid,
      5 => is_numeric($send_interval) ? $send_interval : _subscriptions_get_setting('send_interval', $user),
      6 => TRUE,
      7 => TRUE,
    );

    // Write to subscriptions table
    if ($sid) {
      // Add more value to existed subscription_content row
      $update = db_update('subscriptions')
      ->condition('module', $args[0])
      ->condition('field', $args[1])
      ->condition('value', $args[2])
      ->condition('author_uid', $args[3])
      ->condition('recipient_uid', $args[4]);

      if ($args[6]) {
        $update->fields(array(
          'send_updates' => 1,
        ));
      }

      if ($args[7]) {
        $update->fields(array(
          'send_comments' => 1,
        ));
      }

      $ret = $update->execute() && $ret;
    }
    else {
      $ret = call_user_func_array('subscriptions_write_subscription', $args) && $ret;
    }
  }

  return $ret;
}

/**
 * Rule subscriptions_rules_remove_user_content_type callback
 */
function subscriptions_rules_remove_user_content_type_callback($node_types, $user, $send_interval = "") {
  $ret = TRUE;
  foreach ($node_types as $type) {
    $args = array (
      0 => 'node',
      1 => 'type',
      2 => $type,
      3 => -1, // DEBUG $node->uid,
      4 => $user->uid,
      5 => is_numeric($send_interval) ? $send_interval : _subscriptions_get_setting('send_interval', $user),
      6 => TRUE,
      7 => TRUE,
    );

    $delete = db_delete('subscriptions')
      ->condition('module', $args[0])
      ->condition('field', $args[1])
      ->condition('value', $args[2])
      ->condition('author_uid', $args[3])
      ->condition('recipient_uid', $args[4]);

    $ret = $delete->execute() && $ret;
  }

  return $ret;
}

function subscriptions_rules_stop_sending_callback_access() {
  // TODO: access callback
  return TRUE;
}

function subscriptions_rules_stop_sending_callback($clear) {
  $ret = TRUE;
  $existed = variable_get('subscriptions_cron_percent', 50);
  if ($existed != 0) {
    variable_set('subscriptions_cron_percent_backup', $existed);
  }
  variable_set('subscriptions_cron_percent', 0);

  // Clear the queue
  if ($clear) {
    subscriptions_purge_queue();
    drupal_get_messages('status'); // TODO: Only the message created in  subscriptions_purge_queue
  }
  return $ret;
}

function subscriptions_rules_start_sending_callback() {
  $ret = TRUE;
  $existed = variable_get('subscriptions_cron_percent_backup', 0);
  if ($existed != 0) {
    variable_set('subscriptions_cron_percent', $existed);
  }
  variable_del('subscriptions_cron_percent_backup');
  return $ret;
}

function subscriptions_rules_subscription_suspend_callback($user) {
  db_update('subscriptions_user')
      ->fields(array(
          'suspended' => 1,
        ))
      ->condition('uid', $user->uid)
      ->execute();
}

function subscriptions_rules_subscription_resume_callback($user) {
  db_update('subscriptions_user')
      ->fields(array(
          'suspended' => 0,
        ))
      ->condition('uid', $user->uid)
      ->execute();
}

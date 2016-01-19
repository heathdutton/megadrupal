<?php

/**
 * Implements hook_rules_condition_info().
 *
 * @ingroup rules
 */
function mandrill_inbound_privatemsg_rules_condition_info() {
  return array(
    'mandrill_inbound_privatemsg_user_is_involved' => array(
      'label' => t('User is involved in the thread.'),
      'parameter' => array(
        'thread_id' => array(
          'type' => 'integer',
          'label' => t('Specify the thread ID that should be checked.'),
        ),
        'account' => array(
          'type' => 'user',
          'label' => t('Specify the user that should be checked.'),
        ),
      ),
      'group' => t('Private messages'),
    ),
  );
}

/**
 * Check if a user is involved in the thread.
 *
 * @param $thread_id
 *   Which thread to check.
 * @param $account
 *   Which user should be checked.
 *
 * @return bool
 *   TRUE if the user is involved, FALSE otherwise.
 */
function mandrill_inbound_privatemsg_user_is_involved($thread_id, $account) {
  return (bool)privatemsg_message_load($thread_id, $account);
}

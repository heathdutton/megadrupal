<?php

/**
 * Rules integration for User Scheduled Delete.
 */

/**
 * Implements hook_rules_event_info().
 */
function user_scheduled_delete_rules_event_info() {
  return array(
    'user_scheduled_delete_delete_scheduled' => array(
      'label' => t('A user was scheduled for deletion.'),
      'group' => t('User Scheduled Delete'),
      'variables' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('Account that is scheduled for deletion.'),
        ),
        'scheduling_account' => array(
          'type' => 'user',
          'label' => t('Account that scheduled the blocking'),
        ),
        'deletion_date' => array(
          'type' => 'date',
          'label' => t('The date after which the account will be deleted.'),
        ),
      ),
    ),
    'user_scheduled_delete_user_deleted' => array(
      'label' => t('A user scheduled for deletion was deleted.'),
      'group' => t('User Scheduled Delete'),
      'variables' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('Account just deleted.'),
        ),
      ),
    ),
  );
}

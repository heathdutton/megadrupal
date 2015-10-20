<?php

/**
 * @file
 * Provides batch functions for bulk operations.
 */

/**
 * Batch process user passwords.
 */
function encrypt_password_batch_process($settings, &$context) {
  global $user;

  if ($settings['enabled']) {
    $rids = $settings['roles'];
  }
  else {
    $rids = NULL;
  }
  $batch_size = 20;

  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_uid'] = 0;
    // Get the total number of users to process and store it.
    $context['sandbox']['max'] = _encrypt_password_user_count('encrypted', $rids, 'OR');
  }

  $query = _encrypt_password_users_select_query('encrypted', $rids, 'OR');
  $query->condition('u.uid', $context['sandbox']['current_uid'], '>');
  $query->range(0, $batch_size);

  $uids = $query->execute()->fetchCol();
  $accounts = user_load_multiple($uids, array(), TRUE);

  foreach ($accounts as $account) {
    $edit['encrypt_password']['settings'] = $settings;

    if ($account->uid != $user->uid) {
      user_save($account, $edit);
    }

    $context['sandbox']['progress']++;
    $context['sandbox']['current_uid'] = $account->uid;
  }

  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Finished callback function for batch processing.
 */
function encrypt_password_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('User passwords were successfully updated.'));
  }
}

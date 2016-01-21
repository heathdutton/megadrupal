<?php
/**
 * @file
 * Batch Processor for User Sync
 */

function party_user_batch_user_sync($user, $options, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_user'] = 0;
    $context['sandbox']['max'] = db_query('SELECT COUNT(DISTINCT uid) FROM {users} WHERE uid <> 0')->fetchField();
  }

  party_user_create_party_for_user($user['uid'], $options);

  $context['results'][] = check_plain($user['name']);

  $context['sandbox']['progress']++;
  $context['sandbox']['current_user'] = $user['uid'];
  $context['message'] = t('Now processing %user', array('%user' => $user['name']));

  $context['finished'] = 1;
}

/**
 * Finished callback
 */
function party_user_sync_finished($success, $results, $ops) {
  if ($success) {
    $message = count($results) . ' created';
    $message .= theme('item_list', $results);
  }
  else {
    // An error occured
    $error_operation = reset($ops);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
  }

  drupal_set_message($message);
}
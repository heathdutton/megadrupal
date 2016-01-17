<?php

function services_sso_client_propagate_batch_process(&$context) {
  // For this example, we decide that we can safely process
  // some user accounts at a time without a timeout.
  $limit = variable_get('services_sso_client_propagate_batch_num', 5);

  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['offset'] = 0;
    $context['sandbox']['max'] = $limit + 1;
    $context['sandbox']['finished'] = 0;
  }

  // Create an API account session so we can use it.
  $session_data = services_sso_client_propagate_api_account();

  if (!empty($session_data)) {
    $users = TRUE;
    $accounts = array();

    // Grab some accounts from SSO server.
    if ($users = services_sso_client_propagate_get_accounts($session_data, $context['sandbox']['offset'], $limit)) {
      $accounts = array_merge($accounts, $users);
    }
  }

  if (!empty($accounts)) {
    $ignore = _services_sso_client_ignore_accounts();

    foreach ($accounts as $account) {
      if (!($local_account = user_external_load($account->uid))) {
        watchdog('local_account', 'creating...' . $account->name);
        if (!in_array($account->name, $ignore) && !empty($account->uid)) {
          // Copy the account to local.
          $local_account = services_sso_client_propagate_copy_account($session_data, $account);

          // Log if something went wrong.
          if (!$local_account) {
            watchdog('services_sso_client', t('Couldn\'t create account for SSO user: ' . $account->name));
          }

          else {
            // Store some result for post-processing in the finished callback.
            $context['results'][] = check_plain($account->mail);
          }
        }
      }

      // Update an existing account with latest account info.
      else {
        $local_account = services_sso_client_propagate_update_account($session_data, $account);

        if ($local_account) {
          $context['results'][] = t('Updated: ') . check_plain($account->mail);
        }
      }

      // Update our progress information.
      $context['sandbox']['progress']++;
      $context['message'] = t('Now processing %username', array('%username' => $account->name));

      // Inform the batch engine that we are not finished,
      // and provide an estimation of the completion level we reached.
      $power = pow(2, $context['sandbox']['progress']);
      $context['sandbox']['finished'] = $context['finished'] = $context['sandbox']['finished'] + 0.9/$power;

    }

    $context['sandbox']['offset']++;
  }

  // If we didn't manage to grab any accounts, we are done.
  else {
    // Inform the batch engine that we are not finished,
    // and provide an estimation of the completion level we reached.
    $context['finished'] = 1;
  }

  // This will destroy the API account session.
  services_sso_client_propagate_api_account($session_data);
}

/**
 * Batch 'finished' callback
 */
function services_sso_client_propagate_batch_finished($success, $results, $operations) {
  if ($success) {
    // Here we do something meaningful with the results.
    $message = count($results) .' processed.';
    if (count($results) < 50) {
      $message .= theme('item_list', array('items' => $results));
    }
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
  }
  drupal_set_message($message);
}
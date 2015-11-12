<?php
/**
 * @file
 * Batch implementation.
 */

function services_sso_client_profile_flag_batch_process(&$context) {
  services_sso_client_profile_flag_load_includes();

  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['offset'] = 0;
    $context['sandbox']['max'] = db_query('SELECT COUNT(DISTINCT fid) FROM {services_sso_client_profile_flag}')->fetchField();
  }

  // For this example, we decide that we can safely process
  // some flags at a time without a timeout.
  $limit = variable_get('services_sso_client_profile_flag_batch_num', 20);

  // With each pass through the callback, retrieve the next group of nids.
  $result = db_query_range("SELECT fid FROM {services_sso_client_profile_flag} WHERE 1 ORDER BY weight DESC", $context['sandbox']['offset'], $limit);

  foreach ($result as $row) {
    // Here we actually perform our processing on the current node.
    $flags = ProfileFlag::loadByID($row->fid);
    foreach ($flags as $flag) {
      watchdog('fid', $flag->string);
      $flag->process();

      // Store some result for post-processing in the finished callback.
      $context['results'][] = check_plain($flag->title);

      // Update our progress information.
      $context['sandbox']['progress']++;
      $context['sandbox']['offset']++;
      $context['message'] = t('Now processing %flag', array('%flag' => $flag->fid . ': ' . $flag->title));
    }
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Batch 'finished' callback
 */
function services_sso_client_profile_flag_batch_finished($success, $results, $operations) {
  if ($success) {
    // Here we do something meaningful with the results.
    $message = count($results) . ' processed.';
    if (count($results) < 10) {
      $message .= theme('item_list', $results);
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
<?php
/**
 * @file
 * Queue functions.
 */

function qd_fitbit_check_queues($account) {
  $features = variable_get('qd_fitbit_features');

  // Only fill queues for data points that have been turned on.
  if ($features[QD_FITBIT_FEATURE_WEIGHT] != '0') {
    qd_fitbit_fill_queue($account, QD_FITBIT_WEIGHT_ENDPOINT);
  }

  if ($features[QD_FITBIT_FEATURE_STEPS] != '0') {
    qd_fitbit_fill_queue($account, QD_FITBIT_STEP_ENDPOINT);
  }

  if ($features[QD_FITBIT_FEATURE_DISTANCE] != '0') {
    qd_fitbit_fill_queue($account, QD_FITBIT_DISTANCE_ENDPOINT);
  }

  if ($features[QD_FITBIT_FEATURE_ELEVATION] != '0') {
    qd_fitbit_fill_queue($account, QD_FITBIT_ELEVATION_ENDPOINT);
  }

  if ($features[QD_FITBIT_FEATURE_SEDENTARY] != '0') {
    qd_fitbit_fill_queue($account, QD_FITBIT_MINSEDENTARY_ENDPOINT);
  }
}


function qd_fitbit_fill_queue($account, $endpoint) {
  $queue_name = $endpoint . '_' . $account->uid;
  $queue = DrupalQueue::get($queue_name);
  // There is no harm in trying to recreate existing.
  $queue->createQueue();
  $count = $queue->numberOfItems();

  // Only process if queue is empty.
  if ($count == 0) {
    // Add queue entries, 30 days at a time.
    $start_date = qd_fitbit_get_default_sync_start($account, $endpoint);
    $today = date('U');
    $thirty_days = 60 * 60 * 24 * 30;
    $break = FALSE;
    do {
      $end_date = $start_date + $thirty_days;

      if ($end_date > $today) {
        $end_date = $today;
        $break = TRUE;
      }

      // Add to queue.
      $data = array(
        'start_date' => $start_date,
        'end_date' => $end_date,
        'endpoint' => $endpoint,
        'uid' => $account->uid,
      );
      $queue->createItem($data);

      $start_date = $end_date;
    } while ($break == FALSE);
  }
}


function qd_fitbit_process_all_queues() {
  $accounts = qd_fitbit_get_all_connected_accounts();
  $features = variable_get('qd_fitbit_features');

  // Run process for each user, on each queue.
  foreach ($accounts as $account) {
    // Just in case - make sure queues have something to process.
    qd_fitbit_check_queues($account);

    if ($features[QD_FITBIT_FEATURE_WEIGHT] != '0') {
      qd_fitbit_process_queue(QD_FITBIT_WEIGHT_ENDPOINT . '_' . $account->uid);
    }

    if ($features[QD_FITBIT_FEATURE_STEPS] != '0') {
      qd_fitbit_process_queue(QD_FITBIT_STEP_ENDPOINT . '_' . $account->uid);
    }

    if ($features[QD_FITBIT_FEATURE_DISTANCE] != '0') {
      qd_fitbit_process_queue(QD_FITBIT_DISTANCE_ENDPOINT . '_' . $account->uid);
    }

    if ($features[QD_FITBIT_FEATURE_ELEVATION] != '0') {
      qd_fitbit_process_queue(QD_FITBIT_ELEVATION_ENDPOINT . '_' . $account->uid);
    }

    if ($features[QD_FITBIT_FEATURE_SEDENTARY] != '0') {
      qd_fitbit_process_queue(QD_FITBIT_MINSEDENTARY_ENDPOINT . '_' . $account->uid);
    }
  }
}


function qd_fitbit_process_queue($queue_name) {
  $queue = DrupalQueue::get($queue_name);
  $item = $queue->claimItem();

  $account = user_load($item->data['uid']);
  $details = qd_fitbit_get_properties_for_endpoint($item->data['endpoint']);

  $entities = qd_fitbit_retrieve_data(
    $account,
    $item->data['endpoint'],
    $details['endpoint_property'],
    $details['entity_type'],
    $item->data['start_date'],
    $item->data['end_date']
  );

  $success = qd_fitbit_save_entities($entities, $details['entity_type']);

  if ($success) {
    $queue->deleteItem($item);
  }
}

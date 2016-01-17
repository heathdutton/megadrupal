<?php

/**
 * @file
 * Batch callbacks for the media_theplatform_mpx module.
 */

function media_theplatform_mpx_batch_ingest_info(array $accounts) {
  $batch = array(
    'title' => t('Running ingestion'),
    'operations' => array(),
    'finished' => 'media_theplatform_mpx_batch_finished',
    'file' => drupal_get_path('module', 'media_theplatform_mpx') . '/media_theplatform_mpx.batch.inc',
  );
  foreach ($accounts as $account) {
    $batch['operations'][] = array('media_theplatform_mpx_batch_ingest', array($account, array('method' => 'batch')));
    $queues = media_theplatform_mpx_cron_queue_info($account->id);
    foreach ($queues as $queue_name => $queue_info) {
      $batch['operations'][] = array('media_thplatform_mpx_batch_queue_run', array($queue_name, $queue_info));
    }
  }
  return $batch;
}

function media_theplatform_mpx_batch_ingest(MpxAccount $account, array $options, array &$context) {
  try {
    drupal_set_time_limit(240);
    $summary = $account->ingestVideos($options);
    $context['results'][] = array(format_string($summary['message'], $summary['args']));
    $context['message'] = t('Processed ingestion for @account.', array('@account' => (string) $account));
  }
  catch (Exception $exception) {
    watchdog_exception('media_theplatform_mpx', $exception);
    $context['results'][] = array($exception->getMessage(), 'error');
  }
}

function media_thplatform_mpx_batch_queue_run($queue_name, array $queue_info, array &$context) {
  /** @var DrupalReliableQueueInterface $queue */
  $queue = DrupalQueue::get($queue_name, TRUE);

  if (empty($context['sandbox'])) {
    $context['sandbox']['count'] = 0;

    // Attempt to release items from the queue.
    if (method_exists($queue, 'resetItems')) {
      $queue->resetItems();
    }
    else {
      db_update('queue')
        ->fields(array(
          'expire' => 0,
        ))
        ->condition('expire', 0, '<>')
        ->condition('expire', time(), '<')
        ->condition('name', $queue_name)
        ->execute();
    }
  }

  $end = time() + 60;
  while (time() < $end && $item = $queue->claimItem()) {
    try {
      $context['message'] = t("Processing %queue queue item @id", array('%queue' => $queue_name, '@id' => $item->item_id));
      call_user_func($queue_info['worker callback'], $item->data);
      $queue->deleteItem($item);
      $context['sandbox']['count']++;
    }
    catch (Exception $exception) {
      watchdog_exception('media_theplatform_mpx', $exception);
      $context['results'][] = array($exception->getMessage(), 'error');
    }
  }

  if (db_query('SELECT COUNT(item_id) FROM {queue} WHERE name = :name AND expire = 0', array(':name' => $queue_name))->fetchField()) {
    $context['finished'] = 0;
  }
  else {
    $context['finished'] = 1;
    $context['results'][] = array(t('Processed @count tasks from the %queue queue. There are @remaining tasks remaining.', array('@count' => $context['sandbox']['count'], '%queue' => $queue_name, '@remaining' => $queue->numberOfItems())));
  }
}

function media_theplatform_mpx_batch_finished($success, $results, $operations) {
  foreach ($results as $result) {
    call_user_func_array('drupal_set_message', $result);
  }
}

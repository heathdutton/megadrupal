<?php

/**
 * @file
 * MPX integration for the media_theplatform_mpx module.
 */
/**
 * Implements hook_media_theplatform_mpx_account_status().
 */
function media_theplatform_mpx_media_theplatform_mpx_account_status(MpxAccount $account) {
  $status = array();

  // Report the number of ingested players.
  $video_count = db_select('mpx_video', 'mpxv')
    ->condition('account', $account->import_account)
    ->countQuery()->execute()->fetchField();
  $status['video_count'] = array(
    'title' => t('Ingested videos'),
    'value' => $video_count,
    'severity' => $video_count ? REQUIREMENT_OK : REQUIREMENT_WARNING,
  );

  // Report the number of ingested players.
  $player_count = db_select('mpx_player', 'mpxp')
    ->condition('account', $account->import_account)
    ->countQuery()->execute()->fetchField();
  $status['player_count'] = array(
    'title' => t('Ingested players'),
    'value' => $player_count,
    'severity' => $player_count ? REQUIREMENT_OK : REQUIREMENT_WARNING,
  );

  // Report if there is a batch still being processed.
  if ($account->getDataValue('proprocessing_batch_url')) {
    $status['current_batch'] = array(
      'title' => t('Current batch'),
      'value' => ($account->getDataValue('proprocessing_batch_item_count') - $account->getDataValue('proprocessing_batch_current_item') + 1) . ' items remaining',
    );
  }

  $ingest_url = url('admin/config/media/theplatform/accounts/' . $account->id . '/ingest', array(
    'query' => array(
      'token' => drupal_get_token('admin/config/media/theplatform/accounts/' . $account->id . '/ingest'),
    ) + drupal_get_destination())
  );

  $request_queue = MpxRequestQueue::get($account);
  $status['request_queue'] = array(
    'title' => t('Queued requests to process'),
    'value' => $request_queue->numberOfItems(),
  );
  if ($status['request_queue']['value']) {
    $status['request_queue']['description'] = t('<a href="@ingest-url">Run ingestion now</a> to process items from the queue.', array('@ingest-url' => $ingest_url));
  }
  else {
    // Do not show the status of the request queue if it doesn't contain any items.
    unset($status['request_queue']);
  }

  // Report if there are any videos to be processed in the queue.
  $cron_queue_count = db_select('queue', 'q')
    ->condition('name', 'media_theplatform_mpx_video_cron_queue')
    ->condition('data', '%' . db_like('s:10:"account_id";s:' . strlen($account->id) . ':"' . $account->id . '";') . '%', 'LIKE')
    ->countQuery()->execute()->fetchField();
  $status['video_queue'] = array(
    'title' => t('Queued videos to process'),
    'value' => $cron_queue_count,
  );
  if ($status['video_queue']['value']) {
    $status['video_queue']['description'] = t('<a href="@ingest-url">Run ingestion now</a> to process items from the queue.', array('@ingest-url' => $ingest_url));
  }

  // Report if the account has a current and valid API token.
  $status['token'] = array(
    'title' => t('API token'),
  );
  try {
    $token = $account->acquireToken();
    $status['token']['value'] = $token->value;
    $status['token']['description'] = t('Expires in !interval', array('!interval' => format_interval($token->expire - REQUEST_TIME)));
  }
  catch (Exception $exception) {
    $status['token']['value'] = filter_xss($exception->getMessage());
    $status['token']['severity'] = REQUIREMENT_ERROR;
    // Anything beyond this point requires an API token, so just return.
    return $status;
  }

  // Report on the last notification sequence ID that has been processed.
  $status['current_notification_id'] = array(
    'title' => t('Last notification ID processed'),
    'description' => t('Notification sequence IDs are only stored by thePlatform for 7 days, after which they are invalid and a complete re-ingestion of all videos will be automatically performed.')
  );
  if ($last_notification_id = $account->getDataValue('last_notification')) {
    try {
      $status['current_notification_id']['value'] = $last_notification_id;
      // Verify that the current sequence ID will return data without error.
      MpxApi::authenticatedRequest(
        $account,
        'https://read.data.media.theplatform.com/media/notify',
        array(
          'account' => $account->import_account,
          'filter' => 'Media',
          'clientId' => 'drupal_media_theplatform_mpx_' . $account->account_pid,
          'since' => $last_notification_id,
          'size' => 1,
          'block' => 'false',
        ),
        array(
          'timeout' => variable_get('media_theplatform_mpx__cron_videos_timeout', 180),
        )
      );
    }
    catch (Exception $exception) {
      $status['current_notification_id']['value'] = $exception->getMessage();
      $status['current_notification_id']['severity'] = REQUIREMENT_ERROR;
      // Skip the remaining notifications status check.
      $last_notification_id = FALSE;
    }
  }
  else {
    $status['current_notification_id']['value'] = t('Not yet available');
    $status['current_notification_id']['severity'] = REQUIREMENT_WARNING;
  }

  // Report on the remaining notifications from the API.
  if ($last_notification_id) {
    $status['remaining_notifications'] = array(
      'title' => t('Notifications yet to be processed from thePlatform'),
    );
    try {
      // Check to see how far behind in notifications we are.
      $data = MpxApi::authenticatedRequest(
        $account,
        'https://read.data.media.theplatform.com/media/notify',
        array(
          'account' => $account->import_account,
          'filter' => 'Media',
          'clientId' => 'drupal_media_theplatform_mpx_' . $account->account_pid,
          'history' => $last_notification_id . '-',
        ),
        array(
          'timeout' => variable_get('media_theplatform_mpx__cron_videos_timeout', 180),
        )
      );

      // Only warn if the number of notifications is greater than the amount
      // we can process in one run.
      $remaining_count = count($data);
      $limit = variable_get('media_theplatform_mpx_notification_size', 500);
      $status['remaining_notifications']['description'] = t('Each ingestion run can only process up to @count notifications.', array('@count' => $limit));
      if ($remaining_count >= $limit) {
        $status['remaining_notifications']['severity'] = REQUIREMENT_WARNING;
        $status['remaining_notifications']['value'] = t('At least @count', array('@count' => $remaining_count));
      }
      else {
        $status['remaining_notifications']['value'] = $remaining_count;
      }
    }
    catch (Exception $exception) {
      $status['remaining_notifications']['value'] = $exception->getMessage();
      $status['remaining_notifications']['severity'] = REQUIREMENT_ERROR;
    }
  }

  return $status;
}

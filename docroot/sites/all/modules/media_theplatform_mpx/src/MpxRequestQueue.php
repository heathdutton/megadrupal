<?php

class MpxRequestQueue {

  /**
   * @param MpxAccount $account
   *   The account to use
   *
   * @return DrupalReliableQueueInterface
   */
  public static function get(MpxAccount $account) {
    $name = 'media_theplatform_mpx_request_' . $account->id;
    return DrupalQueue::get($name, TRUE);
  }

  /**
   * Queue worker callback for a request queue item.
   *
   * @param array $queue_data
   *   The queue item data.
   *
   * @return mixed
   *   The result of $data['callback'].
   */
  public static function processItem(array $queue_data) {
    return call_user_func($queue_data['callback'], $queue_data);
  }

  /**
   * Callback for processing batch requests.
   *
   * @param array $queue_data
   *   The queue item data as added in MpxRequestQueue::populateBatchItems().
   *
   * @return int
   *   The number of additional video queue tasks created from the request.
   */
  public static function processBatchItem(array $queue_data) {
    $account = MpxAccount::load($queue_data['account_id']);
    $url = $queue_data['url'];
    $queue_data += array('params' => array(), 'options' => array());

    $queue_data['options'] += array(
      'timeout' => variable_get('media_theplatform_mpx__cron_videos_timeout', 180),
    );

    $request_data = MpxApi::authenticatedRequest($account, $url, $queue_data['params'], $queue_data['options']);
    return _media_theplatform_mpx_process_video_import_feed_data($request_data, NULL, $account);
  }

  /**
   * Populate the request queue with the rest of an account's current batch.
   *
   * @param MpxAccount $account
   *   The mpx account.
   * @param int $limit
   *   The number of items to fetch in each request.
   *
   * @return int
   *   The number of request queue tasks that were added.
   *
   * @todo Add specific $url $start $count parameters instead of using proprocesing data.
   */
  public static function populateBatchItems(MpxAccount $account, $limit = NULL) {
    $batch_url = $account->getDataValue('proprocessing_batch_url');

    if (!$batch_url) {
      // Nothing to batch.
      return 0;
    }

    $queue = MpxRequestQueue::get($account);
    $queue->createQueue();

    $batch_item_count = $account->getDataValue('proprocessing_batch_item_count');
    $current_batch_item = $account->getDataValue('proprocessing_batch_current_item');
    $limit = $limit ? $limit : variable_get('media_theplatform_mpx__cron_videos_per_run', 100);

    $count = 0;
    while ($current_batch_item <= $batch_item_count) {
      $data = array();
      $data['callback'] = get_called_class() . '::' . 'processBatchItem';
      $data['account_id'] = $account->id;
      $data['url'] = $batch_url;
      $data['params'] = array(
        'range' => $current_batch_item . '-' . ($current_batch_item + $limit - 1),
      );
      $queue->createItem($data);
      $count++;
      $current_batch_item += $limit;
    }

    $account->deleteMultipleDataValues(array(
      'proprocessing_batch_url',
      'proprocessing_batch_item_count',
      'proprocessing_batch_current_item',
    ));

    // Ensure a last_notification value is set now.
    if (!$account->getDataValue('last_notification')) {
      $account->setDataValue('last_notification', media_theplatform_mpx_get_last_notification($account));
    }

    return $count;
  }

}

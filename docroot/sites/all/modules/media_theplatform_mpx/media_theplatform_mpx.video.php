<?php
/**
 * @file
 * functions for Videos.
 */


/**
 * Query thePlatform Media Notify service to get Media id's that have changed.
 *
 * @param MpxAccount $account
 *   The mpx account.
 * @param bool $all
 *   If TRUE will keep making requests for changed IDs until no data is
 *   returned. If FALSE will only make one request for changed IDs.
 *
 * @return array
 *
 * @throws MpxApiException
 */
function media_theplatform_mpx_get_changed_ids(MpxAccount $account, $all = FALSE) {
  $results = array(
    'actives' => array(),
    'deletes' => array(),
    'last_notification' => NULL,
    'count' => 0,
  );

  $params = array(
    'account' => $account->import_account,
    'block' => 'false',
    'filter' => 'Media',
    'clientId' => 'drupal_media_theplatform_mpx_' . $account->account_pid,
    'since' => $account->getDataValue('last_notification'),
    'size' => variable_get('media_theplatform_mpx_notification_size', 500),
  );

  do {
    try {
      $data = MpxApi::authenticatedRequest(
        $account,
        'https://read.data.media.theplatform.com/media/notify',
        $params,
        array(
          'timeout' => variable_get('media_theplatform_mpx__cron_videos_timeout', 180),
        )
      );
    }
    catch (MpxApiException $exception) {
      // A 404 response means the notification ID that we have is now older than
      // 7 days, and now we have to start ingesting from the beginning again.
      if ($exception->getException()->responseCode == 404) {
        $exception->setMessage("The last notification ID {$params['since']} for {$account} is older than 7 days and is too old to fetch notifications. The last notification ID has been reset to re-start ingestion of all videos.");
        drupal_register_shutdown_function(array($account, 'resetIngestion'));
      }
      throw $exception;
    }

    foreach ($data as $changed) {
      // Store last notification.
      $results['last_notification'] = $changed['id'];

      // If no method and entry ID present, skip to the next notification.
      if (empty($changed['method']) || empty($changed['entry']['id'])) {
        continue;
      }

      // Grab the last component of the URL.
      $media_id = basename($changed['entry']['id']);
      if ($changed['method'] !== 'delete') {
        $results['actives'][] = $media_id;
      }
      else {
        // Only add to deletes array if this mpxMedia already exists.
        // @todo Could we not care if the local video exists yet? Why not just silently fail the queue task instead?
        $video_exists = media_theplatform_mpx_get_mpx_video_by_field('id', $media_id);
        if ($video_exists) {
          $results['deletes'][] = $media_id;
        }
      }
    }

    // Add the total number of notifications processed.
    $results['count'] += count($data);

    // If this loops back, ensure the next request for notifications uses the
    // sequence ID we just retrieved.
    if (!empty($results['last_notification'])) {
      $params['since'] = $results['last_notification'];
    }

  } while ($all && count($data) == $params['size']);

  // Reduce duplicate results.
  $results['actives'] = array_unique($results['actives']);
  $results['deletes'] = array_unique($results['deletes']);

  watchdog('media_theplatform_mpx',
    "Results while fetching changed media IDs for @account: <br/>
<br /> Changed Media IDs: @actives
<br /> Deleted Media IDs: @deletes
<br /> Last Notification: @last_notification
<br /> Total notification count: @count",
    array(
      '@account' => (string) $account,
      '@actives' => implode(', ', $results['actives']),
      '@deletes' => implode(', ', $results['deletes']),
      '@last_notification' => $results['last_notification'],
      '@count' => $results['count'],
    ),
    WATCHDOG_NOTICE);

  return $results;
}

/**
 * Video queue item worker callback.
 */
function process_media_theplatform_mpx_video_cron_queue_item($item) {

  $start_microtime = microtime(TRUE);

  switch ($item['queue_operation']) {

    case 'publish':
      // Import/Update the video.
      if (!empty($item['video'])) {

        /** @var MpxAccount $account */
        $account = NULL;
        if (!empty($item['account'])) {
          $account = $item['account'];
        }
        elseif (!empty($item['account_id'])) {
          $account = MpxAccount::load($item['account_id']);
        }
        else {
          // If an account wasn't stored, it was queued in a previous version of
          // this module.  Assume account 1 in these cases.
          $accounts = MpxAccount::loadAll();
          $account = reset($accounts);
        }
        if (empty($account)) {
          throw new Exception("Unable to load mpx account for item.");
        }

        // Check if player sync has been turned off for this account.  If so,
        // do not import/update this video.
        if (!variable_get('media_theplatform_mpx__account_' . $account->id . '_cron_video_sync', 1)) {
          return;
        }
        $video = $item['video'];
        // Prepare the video item for import/update.

        // Sometimes MPX will send us invalid URLs like:
        // "{ssl:https:http}://mpxstatic-nbcmpx.nbcuni.com/...jpg". We need to
        // fix the protocol for the ingestion to work.
        // When the root cause is fixed this code may go away.
        $thumbnail_url = $video['plmedia$defaultThumbnailUrl'];
        $thumbnail_url = preg_replace('/(\{.*\})\:\/\/(.*)/', "http://$2", $thumbnail_url);

        $video_item = array(
          'id' => basename($video['id']),
          'guid' => $video['guid'],
          'title' => $video['title'],
          'description' => $video['description'],
          'thumbnail_url' => $thumbnail_url,
          // Additional default mpx fields.
          'released_file_pids' => serialize(_media_theplatform_mpx_flatten_array(_media_theplatform_mpx_get_media_item_data('media$content/plfile$releases/plrelease$pid', $video))),
          'default_released_file_pid' => _media_theplatform_mpx_get_default_released_file_pid($video),
          'categories' => serialize(_media_theplatform_mpx_get_media_item_data('media$categories/media$name', $video)),
          'author' => _media_theplatform_mpx_get_media_item_data('plmedia$provider', $video),
          'airdate' => _media_theplatform_mpx_get_media_item_data('pubDate', $video) / 1000,
          'available_date' => _media_theplatform_mpx_get_media_item_data('media$availableDate', $video) / 1000,
          'expiration_date' => _media_theplatform_mpx_get_media_item_data('media$expirationDate', $video) / 1000,
          'keywords' => _media_theplatform_mpx_get_media_item_data('media$keywords', $video),
          'copyright' => _media_theplatform_mpx_get_media_item_data('media$copyright', $video),
          'related_link' => _media_theplatform_mpx_get_media_item_data('link', $video),
          'fab_rating' => _media_theplatform_mpx_get_rating_media_item_data('fab', $video),
          'fab_sub_ratings' => serialize(_media_theplatform_mpx_get_subrating_media_item_data('fab', $video)),
          'mpaa_rating' => _media_theplatform_mpx_get_rating_media_item_data('mpaa', $video),
          'mpaa_sub_ratings' => serialize(_media_theplatform_mpx_get_subrating_media_item_data('mpaa', $video)),
          'vchip_rating' => _media_theplatform_mpx_get_rating_media_item_data('v-chip', $video),
          'vchip_sub_ratings' => serialize(_media_theplatform_mpx_get_subrating_media_item_data('v-chip', $video)),
          'exclude_countries' => (int)_media_theplatform_mpx_get_media_item_data('media$excludeCountries', $video),
          'countries' => serialize(_media_theplatform_mpx_get_media_item_data('media$countries', $video)),
          // An internal flag that can be used to prevent this video from being imported.
          'ignore' => FALSE,
        );
        // Allow modules to alter the video item for pulling in custom metadata.
        drupal_alter('media_theplatform_mpx_media_import_item', $video_item, $video, $account);

        // Perform the import/update.
        if (empty($video_item['ignore'])) {
          media_theplatform_mpx_import_video($video_item, $account);
        }
      }
      break;

    case 'unpublish':
      if (!empty($item['unpublish_id'])) {
        media_theplatform_mpx_set_mpx_video_inactive($item['unpublish_id'], 'unpublished');
      }
      break;

    case 'delete':
      if (!empty($item['delete_id'])) {
        media_theplatform_mpx_set_mpx_video_inactive($item['delete_id'], 'deleted');
      }
      break;

    default:
      watchdog('media_theplatform_mpx', 'Video not processed.  Unable to determine queue operation for cron queue item: @item',
        array(
          '@item' => str_replace("\n", '\n', print_r($item, TRUE)),
        ));
  }

  // @todo: Do this per account?  By type of operation?
  $current_total_videos_processed = variable_get('media_theplatform_mpx__running_total_videos_processed', 0);
  $current_total_video_processing_time = variable_get('media_theplatform_mpx__running_total_video_processing_time', 0);
  $processing_time = microtime(TRUE) - $start_microtime;
  variable_set('media_theplatform_mpx__running_total_videos_processed', 1 + $current_total_videos_processed);
  variable_set('media_theplatform_mpx__running_total_video_processing_time', $current_total_video_processing_time + $processing_time);
}

/**
 * Helper that retrieves and returns data from an mpx feed URL.
 */
function _media_theplatform_mpx_process_video_import_feed_data($result_data, $media_to_update = NULL, MpxAccount $account) {
  // Initalize arrays to store mpxMedia data.
  $queue_items = array();
  $published_ids = array();
  $unpublished_ids = array();
  $deleted_ids = array();

  // Keys entryCount and entries are only set when there is more than 1 entry.
  // If only one row, result_data holds all the mpxMedia data (if its published).
  // If responseCode is returned, it means 1 ID in $ids has been unpublished and
  // would return no data.
  $entries = array();
  if (isset($result_data['entryCount'])) {
    $entries = $result_data['entries'];
  }
  elseif (!isset($result_data['responseCode'])) {
    $entries = array($result_data);
  }

  foreach ($entries as $video) {
    if (empty($video)) {
      continue;
    }
    $published_ids[] = basename($video['id']);
    // Add item to video queue.
    $item = array(
      'queue_operation' => 'publish',
      'video' => $video,
      'account_id' => $account->id,
    );
    $queue_items[] = $item;
  }

  // Collect any ids that have been unpublished.
  if (isset($media_to_update)) {
    $unpublished_ids = array_diff($media_to_update['actives'], $published_ids);
    // Filter out any ids which haven't been imported.
    foreach ($unpublished_ids as $key => $unpublished_id) {
      $video_exists = media_theplatform_mpx_get_mpx_video_by_field('id', $unpublished_id);
      if (!$video_exists) {
        unset($unpublished_ids[$key]);
      }
      else {
        // Add to queue.
        $item = array(
          'queue_operation' => 'unpublish',
          'unpublish_id' => $unpublished_id,
        );
        $queue_items[] = $item;
      }
    }
  }

  // Collect any deleted mpxMedia.
  if (isset($media_to_update) && count($media_to_update['deletes']) > 0) {
    foreach ($media_to_update['deletes'] as $delete_id) {
      $deleted_ids[] = $delete_id;
      // Add to queue.
      $item = array(
        'queue_operation' => 'delete',
        'delete_id' => $delete_id,
      );
      $queue_items[] = $item;
    }
  }

  // Add the queue items to the cron queue.
  /** @var DrupalReliableQueueInterface $queue */
  $count = 0;
  $queue = DrupalQueue::get('media_theplatform_mpx_video_cron_queue', TRUE);
  foreach ($queue_items as $queue_item) {
    $queue->createItem($queue_item);
    $count++;
  }

  watchdog('media_theplatform_mpx', 'Video IDs queued to be processed on cron:'
    . '  @published_count new mpxMedia queued to be created or updated' . (count($published_ids) ? '(@published_ids)' : '.')
    . '  @unpublished_count new mpxMedia queued to be unpublished' . (count($unpublished_ids) ? '(@unpublished_ids)' : '.')
    . '  @deleted_count new mpxMedia queued to be deleted' . (count($deleted_ids) ? '(@deleted_ids)' : '.'),
    array(
      '@published_count' => count($published_ids),
      '@published_ids' => implode(', ', $published_ids),
      '@unpublished_count' => count($unpublished_ids),
      '@unpublished_ids' => implode(', ', $unpublished_ids),
      '@deleted_count' => count($deleted_ids),
      '@deleted_ids' => implode(', ', $deleted_ids),
    ),
    WATCHDOG_INFO);

  return $count;
}

/**
 * Processes a batch import/update.
 */
function _media_theplatform_mpx_process_batch_video_import(MpxAccount $account, array $options = array()) {
  $options += array(
    'method' => 'manually',
    'limit' => variable_get('media_theplatform_mpx__cron_videos_per_run', 100),
  );

  // Get the parts for the batch url and construct it.
  $batch_url = $account->getDataValue('proprocessing_batch_url');
  $batch_item_count = $account->getDataValue('proprocessing_batch_item_count');
  $current_batch_item = $account->getDataValue('proprocessing_batch_current_item');

  $result_data = MpxApi::authenticatedRequest(
    $account,
    $batch_url,
    array(
      'range' => $current_batch_item . '-' . ($current_batch_item + $options['limit'] - 1),
    ),
    array(
      'timeout' => variable_get('media_theplatform_mpx__cron_videos_timeout', 180),
    )
  );

  if (!$result_data) {
    watchdog('media_theplatform_mpx', 'Aborting batch video import @method.  No video data returned from thePlatform.',
      array(
        '@method' => $options['method'],
        '@account' => _media_theplatform_mpx_account_log_string($account),
      ),
      WATCHDOG_ERROR);

    return FALSE;
  }

  $processesing_success = _media_theplatform_mpx_process_video_import_feed_data($result_data, NULL, $account);
  if (!$processesing_success) {
    watchdog('media_theplatform_mpx', 'Aborting batch video import @method for @account.  Error occured while processing video data, refer to previous log messages',
      array(
        '@method' => $options['method'],
        '@account' => _media_theplatform_mpx_account_log_string($account),
      ),
      WATCHDOG_ERROR);

    return FALSE;
  }

  $current_batch_item += $options['limit'];
  if ($current_batch_item <= $batch_item_count) {
    $account->setDataValue('proprocessing_batch_current_item', $current_batch_item);
  }
  else {
    // Reset the batch system variables.
    $account->setDataValue('proprocessing_batch_url', '');
    $account->setDataValue('proprocessing_batch_item_count', 0);
    $account->setDataValue('proprocessing_batch_current_item', 0);
    // In case this is the end of the initial import batch, set the last
    // notification id.
    // HERE
    if (!$account->getDataValue('last_notification')) {
      $account->setDataValue('last_notification', media_theplatform_mpx_get_last_notification($account));
    }
  }
}

/**
 * Helper that constructs a video feed url given a comma-delited list of video
 * ids or "all" for all media (used during initial import).
 *
 * @todo Convert to a better return format to be used by MpxApi::request().
 */
function _media_theplatform_mpx_get_video_feed_url($ids = NULL, $account = NULL) {

  $ids = ($ids == 'all' || !$ids) ? '' : str_replace(',', '|', $ids);

  // Note:  We sort by updated date ascending so that newly updated content is
  // fetched from the feed last.  In cases of large batches, e.g. an initial
  // import, media can be edited before the batch is complete.  This ensures
  // these edited media will be fetched toward the end of the batch.
  $url = 'https://read.data.media.theplatform.com/media/data/Media?schema=1.8.0&form=json&pretty=true' .
    '&account=' . rawurlencode($account->import_account) .
    // @todo Eventually remove the byOwnerId parameter.
    '&byOwnerId=' . $account->account_id .
    '&byApproved=true&byAvailabilityState=notYetAvailable|available|expired' .
    '&byContent=byExists%3Dtrue%26byHasReleases%3Dtrue' .
    '&sort=guid|asc';

  if ($ids) {
    $url .= '&byId=' . $ids;
  }

  return $url;
}

/**
 * Get the total item count for a given feed url.
 */
function _media_theplatform_mpx_get_feed_item_count($url, MpxAccount $account) {
  $count_result_data = MpxApi::authenticatedRequest(
    $account,
    $url,
    array(
      'count' => 'true',
      'fields' => 'guid',
      'range' => '1-1',
    ),
    array(
      'timeout' => variable_get('media_theplatform_mpx__cron_videos_timeout', 180),
    )
  );

  if (!isset($count_result_data['totalResults'])) {
    watchdog('media_theplatform_mpx', 'Failed to retrieve total feed item count.  totalResults field not found in response data.',
      array(), WATCHDOG_ERROR);
  }

  $total_result_count = $count_result_data['totalResults'];

  watchdog('media_theplatform_mpx', 'Retrieving total feed item count returned @count items.',
    array('@count' => $total_result_count), WATCHDOG_INFO);

  return $total_result_count;
}

/**
 * Processes a video update.
 */
function _media_theplatform_mpx_process_video_update(MpxAccount $account, $options = array()) {
  $options += array(
    'method' => 'manually',
    'limit' => variable_get('media_theplatform_mpx__cron_videos_per_run', 100),
  );

  // This log message may seem redundant, but it's important for detecting if an
  // ingestion process has begun and is currently in progress.
  watchdog('media_theplatform_mpx', 'Beginning video update process @method for @account.',
    array(
      '@method' => $options['method'],
      '@account' => _media_theplatform_mpx_account_log_string($account),
    ),
    WATCHDOG_INFO);

  // Get all IDs of mpxMedia that have been updated since last notification.
  $media_to_update = media_theplatform_mpx_get_changed_ids($account, !empty($options['all']));

  // Update the current last notification from the data.
  if (!empty($media_to_update['last_notification'])) {
    $account->setDataValue('last_notification', $media_to_update['last_notification']);
  }

  // Remove any deletes from actives, because it causes errors when updating.
  $media_to_update['actives'] = array_diff($media_to_update['actives'], $media_to_update['deletes']);

  if (count($media_to_update['actives']) > 0) {
    $ids = implode(',', $media_to_update['actives']);
  }
  elseif (count($media_to_update['deletes']) > 0) {
    $result_data = array();
    $processesing_success = _media_theplatform_mpx_process_video_import_feed_data($result_data, $media_to_update, $account);
    if (!$processesing_success) {
      return FALSE;
    }
    return TRUE;
  }
  else {
    watchdog('media_theplatform_mpx', 'There were no active mpxMedia to update for @account.',
      array('@account' => _media_theplatform_mpx_account_log_string($account)), WATCHDOG_NOTICE);

    return FALSE;
  }

  // Get the feed url.
  $batch_url = _media_theplatform_mpx_get_video_feed_url($ids, $account);

  // Get the total result count for this update.  If it is greater than the feed
  // request item limit, start a new batch.
  $total_result_count = count(explode(',', $ids));

  if ($total_result_count && $total_result_count > $options['limit']) {
    // Set starter batch system variables.
    $account->setDataValue('proprocessing_batch_url', $batch_url);
    $account->setDataValue('proprocessing_batch_item_count', $total_result_count);
    $account->setDataValue('proprocessing_batch_current_item', 1);

    if (!empty($options['all'])) {
      // Put the batch into a queue if requested.
      return MpxRequestQueue::populateBatchItems($account, $options['limit']);
    }
    else {
      // Perform the first batch operation, not the update.
      return _media_theplatform_mpx_process_batch_video_import($account, $options);
    }
  }

  $result_data = MpxApi::authenticatedRequest(
    $account,
    $batch_url,
    array(),
    array(
      'timeout' => variable_get('media_theplatform_mpx__cron_videos_timeout', 180),
    )
  );

  if (!$result_data) {
    return FALSE;
  }

  $processesing_success = _media_theplatform_mpx_process_video_import_feed_data($result_data, $media_to_update, $account);

  if (!$processesing_success) {
    return FALSE;
  }

  return TRUE;
}

/**
 * Processes a video update.
 */
function _media_theplatform_mpx_process_video_import(MpxAccount $account, array $options = array()) {
  $options += array(
    'method' => 'manually',
    'limit' => variable_get('media_theplatform_mpx__cron_videos_per_run', 100),
  );

  // This log message may seem redundant, but it's important for detecting if an
  // ingestion process has begun and is currently in progress.
  watchdog('media_theplatform_mpx', 'Running initial video import for @account @method',
    array(
      '@account' => _media_theplatform_mpx_account_log_string($account),
      '@method' => $options['method'],
    ),
    WATCHDOG_NOTICE);

  // Set the first last notification value for subsequent updates.  Setting it
  // now ensures that any updates that happen during the import are processed
  // in subsequent updates.
  $account->setDataValue('last_notification', media_theplatform_mpx_get_last_notification($account));

  // Get the feed url.
  $batch_url = _media_theplatform_mpx_get_video_feed_url('all', $account);

  // Get the total result count for this update.  If it is greater than the feed
  // request item limit, start a new batch.
  $total_result_count = _media_theplatform_mpx_get_feed_item_count($batch_url, $account);

  if ($total_result_count && $total_result_count > $options['limit']) {
    // Set starter batch system variables.
    $account->setDataValue('proprocessing_batch_url', $batch_url);
    $account->setDataValue('proprocessing_batch_item_count', $total_result_count);
    $account->setDataValue('proprocessing_batch_current_item', 1);

    if (!empty($options['all'])) {
      // Put the batch into a queue if requested.
      return MpxRequestQueue::populateBatchItems($account, $options['limit']);
    }
    else {
      // Perform the first batch operation, not the update.
      return _media_theplatform_mpx_process_batch_video_import($account, $options);
    }
  }

  watchdog('media_theplatform_mpx', 'Retrieving all media data from thePlatform for @account.',
    array('@account' => _media_theplatform_mpx_account_log_string($account)), WATCHDOG_INFO);

  $result_data = MpxApi::authenticatedRequest(
    $account,
    $batch_url,
    array(),
    array(
      'timeout' => variable_get('media_theplatform_mpx__cron_videos_timeout', 180),
    )
  );

  if (empty($result_data)) {
    watchdog('media_theplatform_mpx', 'Failed to retrieve all media data from thePlatform for @account.  Halting the import process.',
      array('@account' => _media_theplatform_mpx_account_log_string($account)), WATCHDOG_ERROR);

    return FALSE;
  }

  $processesing_success = _media_theplatform_mpx_process_video_import_feed_data($result_data, NULL, $account);

  if (!$processesing_success) {
    return FALSE;
  }

  return TRUE;
}

/**
 * Imports all Videos into Media Library.
 *
 * @param String $type
 *   Import type. Possible values 'via cron' or 'manually', for sync.
 *
 * @return Array
 *   $data['total'] - # of videos retrieved
 *   $data['num_inserts'] - # of videos added to mpx_video table
 *   $data['num_updates'] - # of videos updated
 *   $data['num_inactives'] - # of videos changed from active to inactive
 */
function media_theplatform_mpx_import_all_videos($type) {
  // This log message may seem redundant, but it's important for detecting if an
  // ingestion process has begun and is currently in progress.
  watchdog('media_theplatform_mpx', 'Beginning video import/update process @method for all accounts.', array('@method' => $type), WATCHDOG_NOTICE);

  foreach (MpxAccount::loadAll() as $account) {
    // Check if video sync has been turned off for this account.
    if (!variable_get('media_theplatform_mpx__account_' . $account->id . '_cron_video_sync', 1)) {
      continue;
    }

    try {
      $account->ingestVideos(array('method' => $type));
    }
    catch (Exception $e) {
      // Log the error and move on to the next account.
      watchdog_exception('media_theplatform_mpx', $e);
    }
  }

  watchdog('media_theplatform_mpx', 'Processed video import/update @method for all accounts.', array('@method' => $type), WATCHDOG_NOTICE);
}

/**
 * Helper that updates a file entity URI for an mpx video.
 */
function _media_theplatform_mpx_update_file_uri($fid, $new_file_uri, $table = 'file_managed') {
  $file_update_result = db_update($table)
    ->fields(array('uri' => $new_file_uri))
    ->condition('fid', $fid, '=')
    ->execute();

  if ($file_update_result) {
    watchdog('media_theplatform_mpx', 'Successfully updated URI to "@new_uri" for file @fid in the @table table.',
      array(
        '@new_uri' => $new_file_uri,
        '@fid' => $fid,
      '@table' => $table,
      ),
      WATCHDOG_NOTICE);
  }
  else {
    watchdog('media_theplatform_mpx', 'Failed to update URI to "@new_uri" for file @fid in the @table table.',
      array(
        '@new_uri' => $new_file_uri,
        '@fid' => $fid,
      '@table' => $table,
      ),
      WATCHDOG_ERROR);
  }
}

/**
 * Updates or inserts given Video within Media Library.
 *
 * @param array $video
 *   Record of Video data requested from thePlatform Import Feed
 *
 * @return String
 *   Returns output of media_theplatform_mpx_update_video() or media_theplatform_mpx_insert_video()
 */
function media_theplatform_mpx_import_video($video, MpxAccount $account) {
  media_theplatform_mpx_debug($video, "Importing/Updating video " . basename($video['id']) . " for {$account}");

  $op = '';

  // Check if the file exists in files_managed table.
  $files = media_theplatform_mpx_get_files_by_guid($video['guid'], $account);

  // If a file exists:
  if (!empty($files)) {
    foreach ($files as $file) {
      // Check if any mpx_video records exist for the file fid.
      $imported_videos = db_select('mpx_video', 'v')
        ->fields('v', array('fid'))
        ->condition('guid', $video['guid'], '=')
        ->condition('parent_account', $account->id, '=')
        ->execute()
        ->fetchField();
      // If mpx_video record exists, then update record.
      if (!empty($imported_videos)) {
        // If there are multiple existing mpx_video records, log an error message.
        if (count($imported_videos) > 1) {
          watchdog('media_theplatform_mpx', 'Multiple mpx_video records found for file fid "@fid" when importing mpx video "@video_title" for @account.',
            array(
              '@fid' => $file->fid,
              '@video_title' => $video['title'],
              '@account' => _media_theplatform_mpx_account_log_string($account),
            ),
            WATCHDOG_NOTICE);
        }
        $op = media_theplatform_mpx_update_video($video, $file->fid, $account);
      }
      // Else insert new mpx_video record with existing $fid.
      else {
        $op = media_theplatform_mpx_insert_video($video, $file->fid, $account);
      }
    }
  }
  // Else insert new file/record:
  else {

    $imported_videos = media_theplatform_mpx_get_videos_by_id($video['id'], $account);
    $imported_video = (array)reset($imported_videos);

    // If there are multiple existing mpx_video records, log an error message.
    if (count($imported_videos) > 1) {
      watchdog('media_theplatform_mpx', 'Multiple mpx_video records found for video id "@id" when importing mpx video "@video_title" for @account.  Using video record with video_id "@video_id".',
        array(
          '@id' => $video['id'],
          '@video_title' => $video['title'],
          '@account' => _media_theplatform_mpx_account_log_string($account),
          '@video_id' => $imported_video->video_id,
        ),
        WATCHDOG_NOTICE);
    }

    // Perform operations for the old GUID if there is an existing video.
    if (!empty($imported_video['fid']) && !empty($imported_video['guid']) && $imported_video['guid'] != $video['guid']) {
      // Delete thumbnail from files_*/media-mpx directory and from all styles.
      _media_theplatform_mpx_delete_video_images($imported_video);
      // Change the file URI to use the new GUID.
      $new_file_uri = _media_theplatform_mpx_get_video_file_uri($video, $account);
      if ($new_file_uri) {
        _media_theplatform_mpx_update_file_uri($imported_video['fid'], $new_file_uri);
      }
      else {
        watchdog('media_theplatform_mpx', 'Failed to update URI for file @fid in the file_managed table for mpx video "@video_title" for @account.  Unable to determine new URI.',
          array(
            '@fid' => $imported_video['fid'],
            '@video_title' => $video['title'],
            '@account' => _media_theplatform_mpx_account_log_string($account),
          ),
          WATCHDOG_ERROR);
      }
    }

    if (!empty($imported_video['fid'])) {
      $op = media_theplatform_mpx_update_video($video, $imported_video['fid'], $account);
    }
    else {
      $op = media_theplatform_mpx_insert_video($video, NULL, $account);
    }
  }

  // Allow modules to perform additional media import tasks.
  module_invoke_all('media_theplatform_mpx_import_media', $op, $video, $account);

  return $op;
}

/**
 * Helper that returns player data for an mpx video.
 */
function _media_theplatform_mpx_get_video_player($video, MpxAccount $account = NULL) {

  if (!empty($video['player_id'])) {

    return media_theplatform_mpx_get_mpx_player_by_player_id($video['player_id']);
  }

  if (!empty($account->default_player)) {

    return media_theplatform_mpx_get_mpx_player_by_player_id($account->default_player);
  }

  return array();
}

/**
 * Helper that returns the file URI for an mpx video.
 */
function _media_theplatform_mpx_get_video_file_uri($video, MpxAccount $account = NULL) {

  if (!is_array($video) || empty($video['guid'])) {
    return '';
  }

  $player = _media_theplatform_mpx_get_video_player($video, $account);

  if (!is_array($player) || empty($player['fid'])) {
    return '';
  }

  $uri = 'mpx://m/' . $video['guid'] . '/p/' . $player['fid'];

  if (!empty($account->account_id)) {
    $uri .= '/a/' . basename($account->account_id);
  }

  return $uri;
}

/**
 * Helper that saves a new mpx video file entity.
 */
function _media_theplatform_mpx_create_video_file($video, MpxAccount $account = NULL) {

  // Get uri string to create file:
  //   "m" is for media.  "p" for player.  "a" for account.
  $uri = _media_theplatform_mpx_get_video_file_uri($video, $account);

  if (!$uri) {
    watchdog('media_theplatform_mpx', 'Failed to create file for video "@title" - @id - and @account.  URI could not be determined.',
      array(
        '@title' => $video['title'],
        '@id' => $video['id'],
        '@account' => _media_theplatform_mpx_account_log_string($account),
      ),
      WATCHDOG_ERROR);

    return FALSE;
  }

  // Create the file.
  $provider = media_internet_get_provider($uri);
  $file = $provider->save($account);

  if (!is_object($file) || empty($file->fid)) {
    watchdog('media_theplatform_mpx', 'Failed to create file for video "@title" - @id - and @account.',
      array(
        '@title' => $video['title'],
        '@id' => $video['id'],
        '@account' => _media_theplatform_mpx_account_log_string($account),
      ),
      WATCHDOG_ERROR);

    return NULL;
  }

  watchdog('media_theplatform_mpx', 'Successfully created file @fid with uri "@uri" for video "@title" - @id - and @account.',
    array(
      '@fid' => $file->fid,
      '@uri' => $uri,
      '@title' => $video['title'],
      '@id' => $video['id'],
      '@account' => _media_theplatform_mpx_account_log_string($account),
    ),
    WATCHDOG_INFO);

  return $file;
}

/**
 * Inserts given Video and File into Media Library.
 *
 * @param array $video
 *   Record of Video data requested from thePlatform Import Feed
 * @param int $fid
 *   File fid of Video's File in file_managed if it already exists
 *   NULL if it doesn't exist
 *
 * @return String
 *   Returns 'insert' for counters in media_theplatform_mpx_import_all_videos()
 */
function media_theplatform_mpx_insert_video($video, $fid = NULL, MpxAccount $account = NULL) {
  // If file doesn't exist, write it to file_managed.
  if (!$fid) {
    $file = _media_theplatform_mpx_create_video_file($video, $account);
    if (!is_object($file) || empty($file->fid)) {
      return 'failed to insert';
    }
    $fid = $file->fid;
  }

  // Insert record into mpx_video.
  $insert_fields = array(
    'title' => $video['title'],
    'guid' => $video['guid'],
    'description' => $video['description'],
    'fid' => $fid,
    'player_id' => !empty($video['player_id']) ? $video['player_id'] : NULL,
    'parent_account' => $account->id,
    'account' => $account->import_account,
    'thumbnail_url' => $video['thumbnail_url'],
    'created' => REQUEST_TIME,
    'updated' => REQUEST_TIME,
    'status' => 1,
    'id' => $video['id'],
    // Additional default mpx fields.
    'released_file_pids' => $video['released_file_pids'],
    'default_released_file_pid' => $video['default_released_file_pid'],
    'categories' => $video['categories'],
    'author' => $video['author'],
    'airdate' => $video['airdate'],
    'available_date' => $video['available_date'],
    'expiration_date' => $video['expiration_date'],
    'keywords' => $video['keywords'],
    'copyright' => $video['copyright'],
    'related_link' => $video['related_link'],
    'fab_rating' => $video['fab_rating'],
    'fab_sub_ratings' => $video['fab_sub_ratings'],
    'mpaa_rating' => $video['mpaa_rating'],
    'mpaa_sub_ratings' => $video['mpaa_sub_ratings'],
    'vchip_rating' => $video['vchip_rating'],
    'vchip_sub_ratings' => $video['vchip_sub_ratings'],
    'exclude_countries' => $video['exclude_countries'],
    'countries' => $video['countries'],
  );

  _media_theplatform_mpx_enforce_db_field_limits($insert_fields, 'media_theplatform_mpx', 'mpx_video');

  $video_id = db_insert('mpx_video')
    ->fields($insert_fields)
    ->execute();

  if (!$video_id) {
    watchdog('media_theplatform_mpx', 'Failed to insert new video @id - "@title" - associated with file @fid for @account into the mpx_video table.',
      array(
        '@id' => basename($video['id']),
        '@title' => $video['title'],
        '@fid' => $fid,
        '@account' => _media_theplatform_mpx_account_log_string($account),
      ),
      WATCHDOG_ERROR);
  }

  // Update the file entity filename with the newly ingested video title.
  media_theplatform_mpx_update_video_filename($fid, $video['title']);

  module_invoke_all('media_theplatform_mpx_insert_video', $fid, $video, $account);

  watchdog('media_theplatform_mpx', 'Successfully created new video @id - "@title" - associated with file @fid for @account.',
    array(
      '@id' => basename($video['id']),
      '@title' => $video['title'],
      '@fid' => $fid,
      '@account' => _media_theplatform_mpx_account_log_string($account),
    ),
    WATCHDOG_NOTICE);

  // Return code to be used by media_theplatform_mpx_import_all_videos().
  return 'insert';
}

/**
 * Updates File $fid with given $video_title.
 *
 * @todo Replace this with EntityHelper::updateBaseTableValues()?
 */
function media_theplatform_mpx_update_video_filename($fid, $video_title) {
  $success = TRUE;
  $tables = array('file_managed');
  if (db_table_exists('file_managed_revisions')) {
    $tables[] = 'file_managed_revisions';
  }
  foreach ($tables as $table_name) {
    $success &= (bool) db_update($table_name)
      ->fields(array('filename' => substr($video_title, 0, 255)))
      ->condition('fid', $fid)
      ->execute();
  }
  return $success;
}

/**
 * Deletes thumbnail images and image styles associated with an mpx video.
 */
function _media_theplatform_mpx_delete_video_images(array $video) {
  if (empty($video['guid'])) {
    return;
  }

  $image_uri = file_build_uri('media-mpx/' . $video['guid'] . '.jpg');
  if (is_file($image_uri) && !file_unmanaged_delete($image_uri)) {
    watchdog('media_theplatform_mpx', 'Failed to delete mpx image with path: @path', array('@path' => $image_uri), WATCHDOG_ERROR);
  }

  // Delete thumbnail from all the styles.
  // Now, the next time file is loaded, MediaThePlatformMpxStreamWrapper
  // will call getOriginalThumbnail to update image.
  image_path_flush($image_uri);
}

/**
 * Updates given Video and File in Media Library.
 *
 * @param array $video
 *   Record of Video data requested from thePlatform Import Feed
 *
 * @return String
 *   Returns 'update' for counters in media_theplatform_mpx_import_all_players()
 */
function media_theplatform_mpx_update_video($video, $fid = NULL, MpxAccount $account = NULL) {

  // Update mpx_video record.
  $update_fields = array(
    'title' => $video['title'],
    'guid' => $video['guid'],
    'description' => $video['description'],
    'thumbnail_url' => $video['thumbnail_url'],
    'status' => 1,
    'updated' => REQUEST_TIME,
    'id' => $video['id'],
    'player_id' => !empty($video['player_id']) ? $video['player_id'] : NULL,
    // Additional default mpx fields.
    'released_file_pids' => $video['released_file_pids'],
    'default_released_file_pid' => $video['default_released_file_pid'],
    'categories' => $video['categories'],
    'author' => $video['author'],
    'airdate' => $video['airdate'],
    'available_date' => $video['available_date'],
    'expiration_date' => $video['expiration_date'],
    'keywords' => $video['keywords'],
    'copyright' => $video['copyright'],
    'related_link' => $video['related_link'],
    'fab_rating' => $video['fab_rating'],
    'fab_sub_ratings' => $video['fab_sub_ratings'],
    'mpaa_rating' => $video['mpaa_rating'],
    'mpaa_sub_ratings' => $video['mpaa_sub_ratings'],
    'vchip_rating' => $video['vchip_rating'],
    'vchip_sub_ratings' => $video['vchip_sub_ratings'],
    'exclude_countries' => $video['exclude_countries'],
    'countries' => $video['countries'],
  );

  _media_theplatform_mpx_enforce_db_field_limits($update_fields, 'media_theplatform_mpx', 'mpx_video');

  // Fetch video_id and status from mpx_video table for given $video.
  $mpx_video = media_theplatform_mpx_get_mpx_video_by_field('guid', $video['guid']);

  // Construct the update query.
  $update_query = db_update('mpx_video');
  $update_query->fields($update_fields);
  if ($fid) {
    $update_query->condition('fid', $fid, '=');
    $association = 'file fid "' . $fid . '"';
  }
  else {
    $update_query->condition('guid', $video['guid'], '=');
    $association = 'guid "' . $video['guid'] . '"';
  }
  if (is_object($account) && !empty($account->id)) {
    $update_query->condition('parent_account', $account->id, '=');
  }
  $update_query->execute();

  // Update the file entity filename with the newly ingested video title.
  if ($fid) {
    media_theplatform_mpx_update_video_filename($fid, $video['title']);
  }
  else {
    $tables = array('file_managed');
    if (db_table_exists('file_managed_revisions')) {
      $tables[] = 'file_managed_revisions';
    }
    foreach ($tables as $table_name) {
      $filename_update_query = db_update($table_name)
        ->fields(array('filename' => substr($video['title'], 0, 255)))
        ->condition('uri', 'mpx://m/' . $video['guid'] . '/%', 'LIKE');
      if ($account) {
        $filename_update_query->condition('uri', '%/a/' . basename($account->account_id), 'LIKE');
      }
      $filename_update_query->execute();
    }
  }

  // Delete thumbnail from files_*/media-mpx directory.
  _media_theplatform_mpx_delete_video_images($video);

  module_invoke_all('media_theplatform_mpx_update_video', $fid, $video, $account);

  watchdog('media_theplatform_mpx', 'Updated video @id - "@title" - associated with file @fid for @account.',
    array(
      '@id' => basename($video['id']),
      '@title' => $video['title'],
      '@fid' => $fid ? $fid : 'UNAVAILABLE',
      '@account' => _media_theplatform_mpx_account_log_string($account),
    ),
    WATCHDOG_INFO);

  // Return code to be used by media_theplatform_mpx_import_all_videos().
  return 'update';
}

/**
 * Returns associative array of mpx_video data for given $field and its $value.
 */
function media_theplatform_mpx_get_mpx_video_by_field($field, $value, MpxAccount $account = NULL) {

  $query = db_select('mpx_video', 'v')
    ->fields('v')
    ->condition($field, $value, '=');

  if (!empty($account->id)) {
    $query->condition('parent_account', $account->id, '=');
  }

  return $query->execute()->fetchAssoc();
}

/**
 * Returns total number of records in mpx_video table.
 */
function media_theplatform_mpx_get_mpx_video_count() {
  return db_query("SELECT count(video_id) FROM {mpx_video}")->fetchField();
}

/**
 * Returns array of all records in mpx_video given a file entity's fid.
 */
function media_theplatform_mpx_get_videos_by_fid($fid) {

  $query = db_select('mpx_video', 'f')
    ->fields('f')
    ->condition('fid', $fid, '=')
    ->orderBy('video_id', 'DESC');

  return $query->execute()->fetchAll();
}

/**
 * Returns array of all records in mpx_video given a file entity's fid.
 */
function media_theplatform_mpx_get_videos_by_id($id, MpxAccount $account = NULL) {

  $query = db_select('mpx_video', 'f')
    ->fields('f')
    ->condition('id', $id, '=')
    ->orderBy('video_id', 'DESC');

  if (!empty($account->id)) {
    $query->condition('parent_account', $account->id, '=');
  }

  return $query->execute()->fetchAll();
}

/**
 * Returns array of all records in file_managed with mpx://m/$guid/%.
 */
function media_theplatform_mpx_get_files_by_guid($guid, MpxAccount $account = NULL) {

  $query = db_select('file_managed', 'f')
    ->fields('f')
    ->condition('uri', 'mpx://m/' . $guid . '/%', 'LIKE');

  if (!empty($account->account_id)) {
    $query->condition('uri', '%/a/' . basename($account->account_id), 'LIKE');
  }

  return $query->execute()->fetchAll();
}

/**
 * Returns array of all records in file_managed with mpx://m/%/p/[player_fid].
 */
function media_theplatform_mpx_get_files_by_player_fid($fid) {

  return db_select('file_managed', 'f')
    ->fields('f')
    ->condition('uri', 'mpx://m/%', 'LIKE')
    ->condition('uri', '%/p/' . $fid . '%', 'LIKE')
    ->execute()
    ->fetchAll();
}

/**
 * Returns URL string of the thumbnail object where isDefault == 1.
 */
function media_theplatform_mpx_parse_thumbnail_url($data) {
  foreach ($data as $record) {
    if ($record['plfile$isDefault']) {
      return $record['plfile$url'];
    }
  }
}

/**
 * Returns thumbnail URL string for given guid from mpx_video table.
 */
function media_theplatform_mpx_get_thumbnail_url($guid) {
  return db_query("SELECT thumbnail_url FROM {mpx_video} WHERE guid=:guid", array(':guid' => $guid))->fetchField();
}

/**
 * Retrieves the latest notification sequence ID for an account from the API.
 *
 * @param MpxAccount $account
 *   The mpx account.
 *
 * @return string
 *   The last notification sequence ID.
 *
 * @throws UnexpectedValueException
 * @throws MpxException
 */
function media_theplatform_mpx_get_last_notification(MpxAccount $account) {
  $data = MpxApi::authenticatedRequest(
    $account,
    'https://read.data.media.theplatform.com/media/notify',
    array(
      'account' => $account->import_account,
      'filter' => 'Media',
      'clientId' => 'drupal_media_theplatform_mpx_' . $account->account_pid,
    )
  );

  if (!isset($data[0]['id'])) {
    throw new MpxException("Unable to fetch the last notification sequence ID for {$account}.");
  }
  elseif (!is_numeric($data[0]['id'])) {
    throw new UnexpectedValueException("The lsat notification sequence ID {$data[0]['id']} for {$account} was not a numeric value.");
  }
  else {
    watchdog('media_theplatform_mpx', 'Retrieved last notification sequence ID @id for @account.', array(
      '@id' => $data[0]['id'],
      '@account' => (string) $account,
    ), WATCHDOG_INFO);
    return $data[0]['id'];
  }
}

/**
 * Returns most recent notification sequence number from thePlatform.
 *
 * @deprecated
 */
function media_theplatform_mpx_set_last_notification(MpxAccount $account, $last_notification = NULL) {
  if (!isset($last_notification)) {
    $last_notification = media_theplatform_mpx_get_last_notification($account);
  }

  $account->setDataValue('last_notification', $last_notification);
  return TRUE;
}

/**
 * Query thePlatform Media service to get all published mpxMedia files.
 *
 * @param string $id
 *   Value for mpx_video.id.
 * @param string $op
 *   Valid values: 'unpublished' or 'deleted'.
 *
 * @throws Exception
 */
function media_theplatform_mpx_set_mpx_video_inactive($id, $op) {
  $result = db_query("SELECT fid, status FROM {mpx_video} WHERE id = :id", array(':id' => $id))->fetch();
  if ($result === FALSE) {
    // Check to see that the video even exists locally.
    watchdog('media_theplatform_mpx', 'Unable to disable video @id since it does not exist locally.', array('@id' => $id), WATCHDOG_ERROR);
    return;
  }
  elseif ($result->status) {
    // Set status to inactive (only if the video is currently published).
    $update_result = db_update('mpx_video')
      ->fields(array('status' => 0))
      ->condition('id', $id)
      ->execute();
    if (!$update_result) {
      throw new Exception("Failed to set the mpx_video.status column to 0 for video $id.");
    }
  }

  // Set the file published status if the file_admin module is enabled.
  if (module_exists('file_admin')) {
    $file = file_load($result->fid);
    if ($file->published == FILE_PUBLISHED) {
      $file->published = FILE_NOT_PUBLISHED;
      file_save($file);
    }
  }

  // Let other modules perform operations when videos are disabled.
  // @todo Deprecate this hook. They should rely on hook_file_update() instead.
  module_invoke_all('media_theplatform_mpx_set_video_inactive', $id, $op);
}

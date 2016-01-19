<?php
/**
 * @file
 *  wildfire_jobs.api.inc
 *
 * API for Wildfire Jobs system
 *
 * This file provides a 'Drupally' interface to the underlying
 * WildfireClientJob class
 *
 * @author Craig Jones <craig@tiger-fish.com>
 *
 * Code derived from Wildfire 1:
 * @author Chris Cohen <chris.cohen@tiger-fish.com>
 */

/**
 * Add a new job to the job queue, and uploads it to the master server.
 *
 * @param array $details
 *    The job as an array, with the following keys:
 *    - template
 *    - lid
 *    - type (eg alert, broadcast, etc)
 *    - extra (the ID of the alert, broadcast, etc)
 *    - sendtime (if omitted, default is the current time)
 *    - title (if omitted, attempts to obtain this from the related message).
 *    - mode (valid values are WILDFIRE_JOB_MODE_NORMAL, WILDFIRE_JOB_MODE_TEST)
 * @param int $mode
 *  How to create the job. Valid values are:
 *    WILDFIRE_JOB_CREATE_NORMAL
 *      Create the job and batch the creation of Job content and mails
 *    WILDFIRE_JOB_CREATE_IMMEDIATE
 *      Create the job and create the Job content and mails immediately
 *    WILDFIRE_JOB_CREATE_DEFERRED
 *      Create the job and defer creation of Job content and mails
 *
 *    Default is WILDFIRE_JOB_CREATE_NORMAL
 *
 * @return int
 *  The JID of the created Job, or FALSE or failure.
 */
function wildfire_job_add($details, $mode = WILDFIRE_JOB_CREATE_NORMAL) {

  /**
   * Create a carcass local job. This needs to be the RPC version as if a batch
   * operation is started ($mode = WILDFIRE_JOB_CREATE_NORMAL), it will expect
   * to be able to upload the job during the batch operation.
   */
  $job = new WildfireClientJobRpc();
  $job->newJob(
    $details['template'],
    $details['lid'],
    $details['type'],
    $details['extra'],
    !empty($details['sendtime']) ? $details['sendtime'] : 0,
    !empty($details['title']) ? $details['title'] : '',
    !empty($details['mode']) ? $details['mode'] : WILDFIRE_JOB_MODE_NORMAL
  );

  switch ($mode) {

    case WILDFIRE_JOB_CREATE_NORMAL:
      /**
       * Create a batch job to process the list into job user entries. We need
       * to do this at job creation time to create a "snapshot" of the state
       * that the list and the users are in now, so that if a user is later
       * deleted or altered from the local list it does not affect a job that's
       * already in progress.
       */
      $batch = array(
        'operations' => array(
          array(
            'wildfire_job_batch_process_build',
            array(
              $job
            )
          ),
          array(
            'wildfire_job_batch_process_upload',
            array(
              $job
            )
          ),
        ),
        'finished' => 'wildfire_job_batch_finished',
        'title' => t('Creating job'),
        'init_message' => t('Job creation is starting.'),
        'progress_message' => t('Processing. Please wait…'),
        'error_message' => t('Creation of the Job has encountered an error.'),
        'file' => drupal_get_path('module', 'wildfire_jobs') . '/wildfire_jobs.api.inc',
      );
      batch_set($batch);

    break;

    case WILDFIRE_JOB_CREATE_IMMEDIATE:

      // Lock the list and the users on it.
      wildfire_list_lock($job->lid);
      wildfire_list_users_lock_all($job->lid);

      // Store the snapshot of the job content and job mails right away
      $job->newContent();
      $job->newJobMails(-1);

      // Unlock the list
      wildfire_list_unlock($job->lid);

    break;

    case WILDFIRE_JOB_CREATE_DEFERRED:
    default:
      // Do nothing more than the basic job creation
    break;

  }

  return isset($job->jid) ? $job->jid : FALSE;

}

/**
 * Build stage for job creation.
 *
 * @param WildfireClientJobRpc $job
 *  The Job object being processed.
 *
 * @param array &$context
 *  Batch process context. Return values, progress, etc, will be set here by
 *  reference.
 *
 * @return NULL
 */
function wildfire_job_batch_process_build($job, &$context) {

  // Set the limit per cycle.
  $limit = 50;

  // Set to explicitly not finished so that the batch runs.
  $context['finished'] = 0;

  if (empty($context['sandbox'])) {

    module_load_include('inc', 'wildfire_lists', 'wildfire_lists.api');

    $context['sandbox']['list_position'] = 0;
    $context['sandbox']['list_total'] = wildfire_list_get_count($job->lid);
    $context['sandbox']['list_skipped'] = 0;

    // Store these IDs in results for future reference.
    $context['results']['lid'] = $job->lid;
    $context['results']['jid'] = $job->jid;

    // Lock the list.
    wildfire_list_lock($job->lid);

    // Set all user on the list to locked.
    wildfire_list_users_lock_all($job->lid);

    /**
     * Create and store the content for this job. WildfireClientJobRpc
     * implements __sleep() and __wakeup, which ensures that the jobs state
     * is the same as when created when the batch job loops.
     */
    $job->newContent();

    // Watchdog the job batch start details.
    watchdog(
      'wildfire_jobs',
      'Job build started - Job #!jid, List #!lid, !count entries.',
      array(
        '!jid' => $job->jid,
        '!lid' => $job->lid,
        '!count' => $context['sandbox']['list_total'],
      ),
      WATCHDOG_INFO
    );

  }

  // Get the next batch.
  $njm_count = $job->newJobMails(0, $limit);

  // Store the amount of addresses we've skipped so far
  $context['sandbox']['list_skipped'] += $limit - $njm_count;

  // Process the next batch.
  $context['sandbox']['list_position'] += $limit;

  $remaining = $context['sandbox']['list_total'] - $context['sandbox']['list_position'];
  if ($remaining < 0) {
    $remaining = 0;
  }

  // Set the progress message.
  $context['message'] = t(
    'Building Job: !remaining of !total to process, !skipped',
    array(
      '!remaining' => $remaining,
      '!total' => format_plural(
        $context['sandbox']['list_total'],
        '1 entry',
        '@count entries'
      ),
      '!skipped' => format_plural(
        $context['sandbox']['list_skipped'],
        '1 skipped invalid address',
        '@count skipped invalid addresses'
      ),
    )
  );

  // If we've processed all the list items, this stage is complete.
  if ($context['sandbox']['list_position'] >= $context['sandbox']['list_total']) {
    $context['finished'] = 1;

    // Watchdog the job batch finish details.
    watchdog(
      'wildfire_jobs',
      'Job build finished - Job #!jid, List #!lid, final position is !position, end was !count, added !njm_count this batch run',
      array(
        '!jid' => $job->jid,
        '!lid' => $job->lid,
        '!position' => $context['sandbox']['list_position'],
        '!count' => $context['sandbox']['list_total'],
        '!njm_count' => $njm_count,
      ),
      WATCHDOG_INFO
    );

  }

}

/**
 * Upload stage for job creation.
 *
 * @param WildfireClientJobRpc $job
 *  The Job object being processed.
 *
 * @param array &$context
 *  Batch process context. Return values, progress, etc, will be set here by
 *  reference.
 *
 * @return NULL
 */
function wildfire_job_batch_process_upload($job, &$context) {

  // Set the limit per cycle
  $limit = 1000;

  // Set to explicitly not finished so that the batch runs.
  $context['finished'] = 0;

  if (!isset($context['sandbox']['list_upload_total'])) {
    $context['sandbox']['list_upload_position'] = 0;
    $context['sandbox']['list_upload_total'] = $job->getListUsersPendingCount();
  }

  if (!isset($context['sandbox']['content_upload_total'])) {
    $context['sandbox']['content_upload_position'] = 0;
    $context['sandbox']['content_upload_total'] = $job->getContentPendingCount();
  }

  $uploaded = FALSE;
  $succeeded = FALSE;

  /**
   * The Job class will raise exceptions on failure conditions, so ensure that
   * these are correctly caught.
   */
  try {

    // Create a new remote job if there isn't one
    if (!$uploaded) {
      if (!($job->juid) && $job->status == WILDFIRE_JOB_PENDING) {
        // We don't have a remote job for this yet, so let's make one.
        $succeeded = $job->uploadJob();
        $uploaded = TRUE;

        $context['message'] = t(
          'Uploading Job to WildfireHQ server: creating remote job'
        );

      }
    }

    // Add the next batch of users to the remote job
    if (!$uploaded) {
      $count = $job->getListUsersPendingCount();
      if ($count > 0) {
        $succeeded = $job->uploadJobList($limit);
        $count = $job->getListUsersPendingCount();
        $uploaded = TRUE;

        $context['sandbox']['list_upload_position'] = ($context['sandbox']['list_upload_total'] - $count);

        $context['message'] = t(
          'Uploading Job to WildfireHQ server: !count entries remaining',
          array(
            '!count' => $count
          )
        );

      }
    }

    // Add the next batch of content to the remote job
    if (!$uploaded) {
      $count = $job->getContentPendingCount();
      if ($count > 0) {
        $succeeded = $job->uploadJobContent($limit);
        $count = $job->getContentPendingCount();
        $uploaded = TRUE;

        $context['sandbox']['content_upload_position'] = ($context['sandbox']['content_upload_total'] - $count);

        $context['message'] = t(
          'Uploading Job to WildfireHQ server: !count content chunks remaining',
          array(
            '!count' => $count
          )
        );

      }
    }

    // Mark the remote job as ready to be processed
    if (!$uploaded) {
      $succeeded = $job->processJob();
      $uploaded = TRUE;

      $context['message'] = t(
        'Uploading Job to WildfireHQ server: scheduling job for send'
      );

    }

    if ($uploaded && is_array($succeeded) && $succeeded['status'] == WILDFIRE_JOB_PREPARED) {
      /**
       * We've got notification from $job->processJob() that the job is
       * prepared. That's it for this job, log the success and carry on
       */
      $message = t(
        'Job #!jid has been uploaded to the server',
        array(
          '!jid' => $job->jid
        )
      );

      drupal_set_message(check_plain($message), 'status');
      watchdog('wildfire_rpc_jobs', check_plain($message), NULL, WATCHDOG_INFO);

      $context['finished'] = 1;
    }

  }
  catch (WildfireInsufficientQuotaException $e) {
    /**
     * The job could not complete upload as there is not sufficient quota
     * to complete the upload. Log this event, but don't outright fail the job,
     * allowing the upload part of the batch to be retried later.
     *
     * Note that even if there's enough quota to send right now, there might
     * not be by the time the server processes it; in which case, when the
     * cron runs to get the jobs current statuses, this will be reported.
     */

    $message = t(
      'Job #!jid deferred: !message',
      array(
        '!jid' => $job->jid,
        '!message' => $e->getMessage(),
      )
    );

    drupal_set_message(check_plain($message), 'warning');
    watchdog('wildfire_rpc_jobs', check_plain($message), NULL, WATCHDOG_WARNING);

    $context['results']['errors'][] = $message;
    $context['finished'] = 1;

  }
  catch (Exception $e) {
    /**
     * Something in the upload process raised an uncaught Exception.
     * Mark the job as failed, as it can't proceed after this type of Exception
     */

    $job->setStatus(WILDFIRE_JOB_FAILED);

    $message = t(
      'Job #!jid has been marked as failed, as upload to the server raised an error (!message)',
      array(
        '!jid' => $job->jid,
        '!message' => $e->getMessage(),
      )
    );

    drupal_set_message(check_plain($message), 'error');
    watchdog('wildfire_rpc_jobs', check_plain($message), NULL, WATCHDOG_ERROR);

    $context['results']['errors'][] = $message;
    $context['finished'] = 1;

  }

}

/**
 * Callback on job batch completion
 *
 * @param $success
 * @param $results
 * @param $operations
 */
function wildfire_job_batch_finished($success, $results, $operations) {

  /**
   * Now that the job is prepared, add a queue item so that the job is now
   * uploaded in the background.
   */
  if ($success) {

    if (!empty($results['errors'])) {

      $message = t(
        'Error: !errors',
        array(
          '!errors' => implode(', ', $results['errors'])
        )
      );

      drupal_set_message(check_plain($message), 'error');
      watchdog('wildfire_jobs', check_plain($message));

    }

    module_load_include('inc', 'wildfire_lists', 'wildfire_lists.api');

    // Unlock the list
    wildfire_list_unlock($results['lid']);

  }
  else {

    $message = t(
      'Job was not automatically sent to the background processor to be '
      . ' uploaded. It may need to be manually started'
    );

    drupal_set_message(check_plain($message), 'error');
    watchdog('wildfire_jobs', check_plain($message));

    // Log the full input to this callback to the watchdog.

    watchdog(
      'wildfire_jobs',
      'Job batch failed: $success = !success, $results = !results, $operations = !operations',
      array(
        '!success' => print_r($success, TRUE),
        '!results' => print_r($results, TRUE),
        '!operations' => print_r($operations, TRUE),
      )
    );

  }

}

/**
 * Cancel a job before its send has begun.
 *
 * This is only available to jobs that have not yet begun sending. Once in
 * progress, sending cannot be interrupted.
 *
 * @param int $jid
 *    The job ID of the job to be cancelled.
 */
function wildfire_job_cancel($jid) {

  $job = new WildfireClientJob($jid);
  return $job->cancelJob();

}

/**
 * Updates the status of a job.
 *
 * @param $jid int
 *    The job ID to be updated.
 * @param $status int
 *    The status code of the new job status.
 */
function wildfire_jobs_set_status($jid, $status) {

  $job = new WildfireClientJob($jid);
  $job->setStatus($status);

}

/**
 * Executes a job by mailing its recipients, given the job ID.
 *
 * Note that this function does not care whether the job is in a paused, failed,
 * cancelled or any other state so it is the caller's responsibility to be aware
 * of this.
 *
 * @param int $jid
 *  The job ID of the job to execute.
 * @param bool $test_mode
 *  If set to TRUE, will set the job to test mode. In test mode, transactions
 *  are not sent to the server but the job will behave as though it has.
 *
 * @return array
 *  Returns an array containing a 'status' key, which contains the jobs current
 *  status from the server.
 */
function wildfire_job_execute($jid, $test_mode = FALSE) {

  $job = new WildfireClientJobRpc($jid);
  if ($test_mode) {
    $job->testMode(TRUE);
  }
  return $job->processJob();

}

/**
 * Get the latest jobs.
 *
 * @param int $limit
 *    The maximum number of jobs to get. Defaults to 10.

 * @return array
 *  Array of jobs, keyed by Job ID.
 *
 */
function wildfire_jobs_get($limit = 10) {

  // Parameter checking.
  if ($limit < 1) {
    $limit = 10;
  }

  $output = array();

  $jobs = db_select('wildfire_jobs', 'cj')
    ->fields('cj', array(
      'jid',
      'template',
      'uid',
      'lid',
      'started',
      'count',
      'status',
      'scheduled',
      'completed',
      'processed_count',
      'title',
    ))
    ->orderBy('scheduled', 'DESC')
    ->range(0, $limit)
    ->execute();

  while ($row = $jobs->fetchAssoc()) {
    $output[$row['jid']] = $row;
  }

  return $output;
}

/**
 * Work out whether a job is valid or not.
 *
 * @param int $jid
 *    The job ID to check.
 * @return bool
 *    TRUE if the job exists. FALSE otherwise.
 */
function wildfire_job_valid($jid) {

  $job = new WildfireClientJob($jid);
  return !empty($job) ? TRUE : FALSE;

}

/**
 * Get a user's unsubscribe token.
 *
 * This function doesn't really fit into the job class, so for now, it's an
 * external function.
 *
 * @param $uid int
 *    The user ID.
 * @param $jid int
 *    The job ID.
 * @return string
 *    The unsubscribe token for the user/job combination, or FALSE if no
 *    token was found.
 *
 */
function wildfire_job_get_unsubscribe_token($uid = 0, $jid = 0) {

  // You need to provide both values, or tokens can't be looked up.
  if (empty($uid) || empty($jid)) {
    return FALSE;
  }

  $query = db_select('wildfire_job_mails', 'cjm')
    ->fields('cjm', array(
      'token'
    ));
  $query->condition('cjm.uid', $uid);
  $query->condition('cjm.jid', $jid);
  $result = $query->execute();

  return $result->fetchField();
}

/**
 * Returns a UID given a messages token
 *
 * @param string $token
 *  The token to look up
 *
 * @return int
 *  The UID of the user that the token relates to, else FALSE if the token
 *  does not relate to a user, or the token does not exist.
 */
function wildfire_job_get_uid_from_token($token) {

  $query = db_select('wildfire_job_mails', 'cjm')
    ->fields('cjm', array(
      'uid'
    ));
  $query->condition('token', $token, '=');
  $result = $query->execute();

  $uid = $result->fetchField();

  return !empty($uid) ? $uid : FALSE;

}

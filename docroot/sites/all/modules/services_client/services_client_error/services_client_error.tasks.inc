<?php

/**
 * @file
 * Queue Runner tasks callbacks.
 */

/**
 * Re-try to send data to remote connection on services client failure.
 *
 * @param array $data
 *   Data from queue.
 *   array('eid' = 9999);
 *
 * @return bool
 *   TRUE if updated.
 */
function services_client_error_task_retry_call($data) {
  $error = services_client_error_load($data['eid']);
  // Error could be deleted by user.
  if (empty($error)) {
    return TRUE;
  }

  // Proceed only if error haven't been processed yet.
  if ($error['status'] == SC_ERROR_UNPROCESSED) {
    $result = services_client_error_retry($error);

    // Build log message.
    $log = array(
      'message' => t('Automatic re-try via queue runner.'),
      'eid' => $error['eid'],
    );
    if (isset($result->sc_error_status)) {
      $log['status'] = $result->sc_error_status;
    }
    else {
      $log['error_code'] = $result->error_code;
      $log['error_message'] = $result->error_message;
    }
    services_client_error_log_save($log);

    return $result;
  }

  // Error has been processed, or overwritten.
  return TRUE;
}

/**
 * Implements finalize hook to determine queue item state.
 */
function services_client_error_task_finalize($data, $result) {
  // Error can't be processed, i.e. it was marked as done, overwritten.
  if ($result === TRUE) {
    return QUEUERUNNER_STATE_DONE;
  }
  else {
    // If error is recovered, don't re-try
    if ($result->success()) {
      return QUEUERUNNER_STATE_DONE;
    }
    else {
      if ($result->sc_error_retries >= $data['retries']) {
        $log = array(
          'message' => t("Automatic ret-ry failed"),
          'eid' => $data['eid'],
          'status_change' => SC_ERROR_FAILED,
        );
        services_client_error_log_save($log);
        db_query("UPDATE {services_client_error} SET status = :status WHERE eid = :eid", array(
          ':eid' => $data['eid'], ':status' => SC_ERROR_FAILED
        ));

        services_client_error_notify($result);
        return QUEUERUNNER_STATE_FAILED;
      }
    }
  }
  return QUEUERUNNER_STATE_RETRYING;
}

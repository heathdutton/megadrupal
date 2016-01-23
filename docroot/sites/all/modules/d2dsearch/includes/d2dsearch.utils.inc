<?php

/**
 * @file
 * Utility functions to validate variables, to acquire db lock, etc.
 */

/**
 * Wraps Drupal lock_acquire with a timeout.
 *
 * @param string $name
 *   A name for the lock.
 * @param int $max_wait
 *   Maximum time to keep wait for the lock.
 *
 * @return bool
 *   TRUE if the lock was acquired.
 *
 * @see lock_acquire()
 */
function _d2dsearch_lock_acquire($name, $max_wait) {
  if (lock_acquire($name)) {
    return TRUE;
  }
  $start_time = microtime(TRUE);
  while (microtime(TRUE) < $start_time + $max_wait) {
    if (!lock_wait($name, 1) && lock_acquire($name)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Validates the id of a query.
 *
 * @param string $query_id
 *   The id to validate.
 *
 * @return bool
 *   TRUE if it is a valid query id.
 */
function _d2dsearch_check_query_id($query_id) {
  return is_string($query_id) &&
    strlen($query_id) == D2DSEARCH_QUERY_ID_LENGTH &&
    d2d_is_hex_string($query_id);
}

/**
 * Validates a query string.
 *
 * @param string $query
 *   The query to validate.
 *
 * @return bool
 *   TRUE if it is a valid query.
 */
function _d2dsearch_check_query($query) {
  return is_string($query) && strlen($query) <= D2DSEARCH_QUERY_MAX_LENGTH;
}

/**
 * Validates a query type.
 *
 * @param string $type
 *   The query type to validate.
 *
 * @return bool
 *   TRUE if it is a valid query type.
 */
function _d2dsearch_check_query_type($type) {
  return is_string($type) && strlen($type) <= D2DSEARCH_QUERY_TYPE_MAX_LENGTH;
}

/**
 * Generates a random id for d2dsearch query.
 *
 * @return string
 *   The random hexadecimal string.
 */
function _d2dsearch_random_query() {
  return d2d_random_hex_string(D2DSEARCH_QUERY_ID_LENGTH);
}

/**
 * Counts the number of results for a query id.
 *
 * @param string $qid
 *   The id of the query.
 *
 * @return int
 *   The number of results in the database for the given query id.
 */
function _d2dsearch_count_results($qid) {
  $result = db_query('SELECT count(*) as result_count FROM {d2dsearch_result_cache} WHERE qid=:qid',
            array(':qid' => $qid));

  $row = $result->fetchAssoc();
  if ($row) {
    return $row['result_count'];
  }
  else {
    return 0;
  }
}

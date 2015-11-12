<?php
/**
 * @file
 *  wildfire_lists.api.inc
 *
 *  API for the lists module
 *
 * @author Craig Jones <craig@tiger-fish.com>
 *
 * Code derived from Wildfire 1:
 * @author Chris Cohen <chris.cohen@tiger-fish.com>
 */

/**
 * Batch Operation Callback
 *
 * Each batch operation callback will iterate over up to a hundred rows in the
 * CSV file.
 *
 * @param handle
 *   The handle to the CSV file from which the rows are being read.
 *
 * @param context
 *   $context is an array that will contain information about the
 *   status of the batch. The values in $context will retain their
 *   values as the batch progresses.
 *
 * @param $context['sandbox']
 *   Use the $context['sandbox'] rather than $_SESSION to store the
 *   information needed to track information between successive calls.
 *   The values in the sandbox will be stored and updated in the database
 *   between http requests until the batch finishes processing. This will
 *   avoid problems if the user navigates away from the page before the
 *   batch finishes.
 *
 * @param $context['results']
 *   The array of results gathered so far by the batch processing.
 *   The current operation can append its own.
 *
 * @param $context['message']
 *   A text message displayed in the progress page.
 *
 * @param $context['finished']
 *   A float number between 0 and 1 informing the processing engine
 *   of the completion level for the operation.
 *
 *   1 (or no value explicitly set) means the operation is finished
 *   and the batch processing can continue to the next operation.
 *
 * @return NULL
 */
function wildfire_list_batch_import_process($filename, $lid, $linecount, $values, &$context) {

  $context['finished'] = 0;

  // Open the Import file. Not much will work without it :)
  $handle = fopen($filename, "r");
  if ($handle === FALSE) {
    $context['results']['failed'][] = t('Fatal Import error: could not open CSV to import from');
    $context['finished'] = 1;
    return;
  }

  // If this is the first time around this batch, start off the line counter at
  // zero and initialise the timer.
  if (!isset($context['sandbox']['line'])) {
    $context['sandbox']['starttime'] = REQUEST_TIME;
    $context['sandbox']['line'] = 0;
    $context['sandbox']['total'] = $linecount;
  }

  $rowmsg = t(
    '@current of @total rows.',
    array(
      '@current' => $context['sandbox']['line'],
      '@total' => $context['sandbox']['total'],
    )
  );

  // Work out how long has elapsed and how many rows we've processed. Give an
  // estimation of the time remaining.
  $elapsed = REQUEST_TIME - $context['sandbox']['starttime'];

  // Calculate, unless this is the first time around.
  if ($elapsed > 0) {
    $rowtime = $elapsed / $context['sandbox']['line'];
    $secsleft = ceil($rowtime * ($context['sandbox']['total'] - $context['sandbox']['line']));

    $timemsg = t(
      '@elapsed elapsed. Approximately @timeleft remaining.',
      array(
        '@elapsed' => wildfire_human_readable_time($elapsed),
        '@timeleft' => wildfire_human_readable_time($secsleft),
      )
    );
  }
  else {
    $timemsg = t('0 seconds elapsed.');
  }

  // Because we open the file anew each time around this operation, seek back
  // to the position we finished the last run at, if it is set
  if (!empty($context['sandbox']['file_pointer'])) {
    fseek($handle, $context['sandbox']['file_pointer']);
  }

  // Process 100 imports at a time. More than this might cause a timeout.
  $limit = 50;

  while ($limit-- > 0) {

    $row = fgetcsv($handle);

    if ($row) {
      $context['sandbox']['line']++;

      // Check that the row contains a valid email address.
      $mail = '';

      if (!($mail = wildfire_row_contains_email_address($row))) {
        // Don't fail if this is the first row because this is likely to
        // contain column names and not values. There is a potential fail case
        // here when the first row contains actual values and there genuinely
        // is not a valid email address. This will not be picked up using
        // current code.
        if ($context['sandbox']['line'] != 1) {
          $context['message'] = $context['results']['failed'][$context['sandbox']['line']] = t('No valid email address could be found in this row.');
        }
        continue;
      }

      $account = array();

      foreach ($row as $key => $col) {
        // Check that a mapping exists for this column number and that it has
        // not been skipped from the import.
        if ($values[$key] && $values[$key] != 'skip') {
          $account[$values[$key]] = $col;
        }
      }

      // Make sure that the email address has been set and that it's valid.
      if (!$account['mail']) {
        $context['message'] = $context['results']['failed'][$context['sandbox']['line']] = t('No email address was associated with this account.');
        continue;
      }
      if (!wildfire_valid_email_address($account['mail'])) {
        $context['message'] = $context['results']['failed'][$context['sandbox']['line']] = t('%val is not a valid email address', array('%val' => $account['mail']));
        continue;
      }

      // Attempt to load the user, and if it exists, update the existing
      // user's details instead of creating a new one.
      $user_account = user_load_by_mail($account['mail']);

      if ($user_account !== FALSE) {

        // A user was found, and we have a valid account object at this point
        $context['results']['existing'][$context['sandbox']['line']] = $account['mail'];
        $context['message'] = t('Updated existing user %mail', array('%mail' => $account['mail']));
      }
      else {

        // No user was found with this email address. We will create one!
        $account['name'] = wildfire_generate_username($account['mail']);
        $account['pass'] = wildfire_random_password();
        $account['status'] = 1;

        // Set a flag to indicate that the usual 'add user to list on register'
        // system should be ignored.
        $account['wildfire_import'] = TRUE;

        // Create a new $user_account object, as we will need one later on...
        $user_account = new stdClass();
        $user_account->uid = '';

        $user_account = user_save($user_account, $account);

        if ($user_account === FALSE) {
          // There was a problem calling the user_save() function to actually
          // insert the user into the database.
          $context['message'] = $context['results']['failed'][$context['sandbox']['line']] = t('There was a problem saving the user %email.', array('%email' => $account['mail']));
          continue;
        }

        $context['results']['new'][$context['sandbox']['line']] = $account['mail'];
        $context['message'] = t('Added new user %mail', array('%mail' => $account['mail']));

      }

      // Add the user to the specified list. Check first to see if the user
      // is already in the list.
      $query = db_select('wildfire_lists_users', 'clu');
      $query->condition('lid', $lid, '=');
      $query->condition('uid', $user_account->uid, '=');
      $query->addExpression('COUNT(1)', 'count');
      $result = $query->execute();

      if (!$result->fetchField()) {
        wildfire_lists_user_add($user_account->uid, $lid);
      }

      // Set the percentage completion we've reached. When the last line is
      // reached, this will be 1, ie finished.
      $context['finished'] = $context['sandbox']['line'] / $context['sandbox']['total'];
    }
    else {
      // There are no more rows in the file, so end the batch.
      $context['finished'] = 1;
      break;
    }
  }

  // Update the message that will be displayed on the screen so that it includes
  // the progress in terms of rows and our time estimate.
  $context['message'] = $rowmsg . ' ' . $timemsg . ' ' . $context['message'];

  // Save our position in the import file before closing...
  $context['sandbox']['file_pointer'] = ftell($handle);

  fclose($handle);

}

/**
 * Batch 'finished' callback.
 *
 * Use the 'wildfire_batch_results' variable to store the results of the
 * batch, because the results are actually displayed in a separate page request.
 */
function wildfire_list_batch_import_finished($success, $results, $operations) {

  if ($success) {
    variable_set('wildfire_batch_results', $results);
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    variable_set(
      'wildfire_batch_results',
      t(
        'An error occurred while processing !operation with arguments: !arguments',
        array(
          '!operation' => $error_operation[0],
          '!arguments' => print_r($error_operation[0], TRUE),
        )
      )
    );
  }
}

/**
 * Gets the number of users in a specific list.
 *
 * @param $lid int
 *    The list ID of the list to count.
 * @param $subscribed_only
 *    If TRUE, obtains only those users that are actually subscribed.
 * @return mixed
 *    The number of users in the list, or FALSE if the list is not found.
 */
function wildfire_list_get_count($lid = 0, $subscribed_only = FALSE) {

  if ($lid > 0) {
    $totals = wildfire_list_get_totals($lid);

    if ($subscribed_only) {
      return $totals[WILDFIRE_STATUS_SUBSCRIBED];
    }
    else {
      return $totals['total'];
    }

  }

  return FALSE;
}

/**
 * Gets the list name given its list ID.
 *
 * This can be used to ensure that the list ID is valid, since it will return
 * false if the list does not exist. Cached to prevent excess database queries.
 *
 * @param $lid
 *    The list ID of the list to look for.
 * @return mixed
 *    A string representing the list name, or FALSE if the list does not exist.
 */
function wildfire_list_get_name($lid = 0) {
  static $wildfire_listnames;

  if (!isset($wildfire_listnames[$lid])) {

    $query = db_select('wildfire_lists', 'cl')
      ->fields('cl', array(
      'name'
    ));
    $query->condition('lid', $lid, '=');
    $result = $query->execute();
    $wildfire_listnames[$lid] = $result->fetchField();

  }

  return $wildfire_listnames[$lid];
}

/**
 * Gets the total number of users in a list, and other totals.
 *
 * @param int $lid
 *    The list ID.
 * @return array
 *    An array with the following keys:
 *    - name:         The human-readable list name.
 *    - total:        The total number of users in the list.
 *
 *    Then a number of array key/value pairs, where the key is the status code,
 *    such as WILDFIRE_STATUS_SUBSCRIBED, and the value is the number of
 *    users with that status.
 *
 *    If the specified list ID is not valid, returns an empty array.
 *
 * TODO: Optimize this into a single query.
 */
function wildfire_list_get_totals($lid = 0) {
  if (!wildfire_list_valid($lid)) {
    return array();
  }

  $output = array();

  // Get the name of the list.
  $output['name'] = wildfire_list_get_name($lid);

  // Get the total number of users in the list.
  $query = db_select('wildfire_lists_users', 'clu');
  $query->condition('lid', $lid, '=');
  $query->addExpression('COUNT(1)', 'count');
  $result = $query->execute();
  $output['total'] = $result->fetchField();

  // Get the number of users who have unsubscribed, either from the list or
  // globally unsubscribed.
  $query = db_select('wildfire_lists_users', 'clu');
  $query->join('wildfire_optouts', 'co', 'clu.uid = co.uid');
  $query->condition('clu.lid', $lid, '=');
  $query->condition('co.lid', array(0, $lid), 'IN');
  $query->addExpression('COUNT(1)', 'count');
  $query->isNotNull('co.uid');
  $result = $query->execute();

  $output[WILDFIRE_STATUS_UNSUBSCRIBED] = $result->fetchField();

  // Derive the active users by subtracting the unsubscribed from the total.
  $output[WILDFIRE_STATUS_SUBSCRIBED] = $output['total'] - $output[WILDFIRE_STATUS_UNSUBSCRIBED];

  // If the value in any of the fields can be interpreted as a positive integer
  // value, cast it to an integer, as if you perform math on the values when
  // still strings it can lead to unpredictable results.
  foreach ($output as $key => &$value) {
    if (ctype_digit((string) $value)) {
      $value = (int) $value;
    }
  }

  return $output;
}

/**
 * Gets a list of mailing lists as an associative array.
 *
 * @param string $mode
 *    'all': Obtain all lists (uses internal cache)
 *    'recent': Obtain the recently used lists
 *    Defaults to 'all'
 * @param bool $display_count
 *    If set to TRUE, the list title will have the user count appended to the
 *    end of it in brackets. Note that this is significantly slower than
 *    without counts. Defaults to FALSE
 * @param int $limit
 *    Limit of number of returned lists. Ignored for 'all' mode. Defaults to 5.
 * @param int $status
 *    Return lists that are in the given state. Defaults to
 *
 * @return array
 *    An associative array of mailing lists. The key of the array is the lid
 *    of the list and the value is its name.
 */
function wildfire_lists_get($mode = 'all', $display_count = FALSE, $limit = 5, $status = WILDFIRE_LIST_STATUS_LIVE) {
  // Cache options to reduce db queries.
  static $wildfire_lists;

  // Store context, as we will need to reset the cache if the context changes.
  $context = array($mode, $display_count, $limit, $status);
  if ($wildfire_lists['context'] == $context && !empty($wildfire_lists['data'])) {
    // Cache is OK to use as its requesting the same context
    return $wildfire_lists['data'];
  }
  else {
    // We will need to obtain new data
    $wildfire_lists['context'] = $context;
    $wildfire_lists['data'] = array();
  }

  $query = db_select('wildfire_lists', 'cl')
    ->fields('cl', array(
      'lid',
      'name'
    ));
  $query->condition('status', $status, '=');

  switch ($mode) {
    case 'recent':
      $query->orderBy('last_sent_to', 'DESC');
      $query->range(0, $limit);
    break;
    case 'all':
    default:
      $query->orderBy('name', 'ASC');
    break;
  }

  $result = $query->execute();

  while ($row = $result->fetchAssoc()) {

    if ($display_count) {
      $totals = wildfire_list_get_totals($row['lid']);
      $wildfire_lists['data'][$row['lid']] = t(
        '!name (!count)',
        array(
          '!name' => check_plain($row['name']),
          '!count' => $totals[WILDFIRE_STATUS_SUBSCRIBED],
        )
      );
    }
    else {
      $wildfire_lists['data'][$row['lid']] = check_plain($row['name']);
    }

  }

  return $wildfire_lists['data'];

}


/**
 * Returns the number of mailing lists.
 *
 * @param $reset bool
 *    Whether to forcibly reset the static cache. Set this to TRUE if you know
 *    that the list count has changed between one call to the function and the
 *    next.
 * @return int
 *    The number of mailing lists.
 */
function wildfire_lists_count($reset = FALSE) {
  static $wildfire_list_count;

  if ($reset || !isset($wildfire_list_count)) {
    // We can try to get the number of lists from the cached
    // wildfire_lists_get() function, which is better than a db query.
    // Only try this if we're not forcibly resetting.
    if (!$reset) {
      static $wildfire_lists;
    }

    if (isset($wildfire_lists)) {
      $wildfire_list_count = count($wildfire_lists);
    }
    else {
      // No luck, so we'll have to run a db query.
      $query = db_select('wildfire_lists', 'cl');
      $query->addExpression('COUNT(1)', 'count');
      $result = $query->execute();

      $wildfire_list_count = $result->fetchField();
    }
  }

  return $wildfire_list_count;
}



/**
 * Add a list with the specified name.
 *
 * @param $name
 *    The name of the list to be created.
 * @return int
 *    The list ID of the list that has just been added.
 */
function wildfire_lists_add_list($name) {
  $record['name'] = check_plain($name);
  $record['created'] = REQUEST_TIME;
  $record['status'] = WILDFIRE_LIST_STATUS_LIVE;

  /**
   * We want new lists to be promoted to the top of the recently used lists,
   * therefore we set the 'last used' date to now.
   */
  $record['last_sent_to'] = REQUEST_TIME;

  drupal_write_record('wildfire_lists', $record);

  $query = db_select('wildfire_lists', 'cl')
    ->fields('cl', array(
      'lid'
    ));
  $query->condition('name', check_plain($name), '=');
  $result = $query->execute();

  return $result->fetchField();
}

/**
 * Delete a given mailing list.
 *
 * @param $lid
 *    The lid of the list that should be deleted.
 * @return bool
 *    TRUE if the list was deleted. FALSE otherwise.
 */
function wildfire_lists_delete($lid = 0) {
  if (wildfire_list_valid($lid)) {

    // Delete the list itself.
    db_delete('wildfire_lists')
      ->condition('lid', $lid)
      ->execute();

    // Delete all user mappings for users who were in that list.
    db_delete('wildfire_lists_users')
      ->condition('lid', $lid)
      ->execute();

    return TRUE;
  }

  // The list was not valid.
  return FALSE;
}


/**
 * Export a given mailing list in CSV format.
 *
 * @param int $lid
 *    The list ID to be exported.
 * @param array $statuses
 *    The statuses of the users to be included in the export. Defaults to just
 *    WILDFIRE_STATUS_SUBSCRIBED.
 * @return string
 *    The filename of the generated CSV file.
 */
function wildfire_lists_export(
$lid,
$statuses = array(WILDFIRE_STATUS_SUBSCRIBED)
) {
  // Parameter checking.
  if (!wildfire_list_valid($lid)) {
    wildfire_error(
      'Cannot export: no list with ID "@lid" exists.',
      array('@lid' => $lid),
      'admin/wildfire/lists'
    );
  }

  // Make sure we have at least one status to export.
  if (empty($statuses) || !is_array($statuses)) {
    wildfire_error(
      'Cannot export: no statuses chosen.',
      array(),
      'admin/wildfire/lists'
    );
  }

  // Get all the users to be exported in a massive array, including profile
  // data where applicable.
  $users = wildfire_list_get_users($lid, $statuses);

  // Basic header check. You should know this already.
  if (headers_sent()) {
    wildfire_error(
      'Cannot serve export list because headers have already been sent.'
    );
    return;
  }

  // Browser Header Information.
  $filename = 'export-' . wildfire_list_get_name($lid) . '-' . format_date(REQUEST_TIME, 'custom', 'YmdHis') . '.csv';
  header('Content-Disposition: attachment; filename=' . $filename . '.csv');
  header('Content-Type: text/csv');

  // Get the headers for the first row of the CSV by examining the first element
  // in the $users array. This is not ideal, since if the first user in the
  // array has some profile fields missing, the profile fields will be left out
  // of the CSV.
  $headers = array();
  $header_labels = array();

  foreach (array_keys($users[key($users)]) as $field) {
    $headers[] = $field;
    $header_labels[] = '"' . $field . '"';
  }

  print implode(',', $header_labels) . "\n";

  foreach ($users as $user) {
    $output = array();

    foreach ($headers as $field) {
      $output[] = isset($user[$field]) ? '"' . $user[$field] . '"' : '""';
    }

    print implode(',', $output) . "\n";
  }

  // We're done, but we're not going to return anything. Exit, making sure to
  // allow other modules to do the same.
  module_invoke_all('exit');
  exit(0);
}





/**
 * Add a new optout.
 *
 * @param int $lid
 *    The list ID to be opted out of. If 0, this is a global optout.
 * @param int $uid
 *    The user ID of the user being opted out.
 * @param int $timestamp
 *    UNIX timestamp representing the time of the optout. Defaults to the
 *    current timestamp.
 * @param int $method
 *    One of the following:
 *    1: User has unsubscribed by editing the user profile.
 *    2: User has unsubscribed by using a 1-click unsubscribe link. In this
 *       case, $unsubscriber is ignored and is always set to the value of $uid.
 *
 *    This defaults to 0, which is unknown, but should never be used, and exists
 *    only to provide a safe default.
 * @param int $unsubscriber
 *    The user ID of the user doing the updating.
 * @return bool
 *    TRUE if successful. FALSE otherwise, such as if bad parameters were used.
 */
function wildfire_list_add_optout($lid = 0, $uid = 0, $timestamp = 0, $method = 0, $unsubscriber = 0) {
  $msg = FALSE;
  $params = array();

  // Parameter checking.
  if (!wildfire_list_valid($lid) && $lid != 0) {
    $msg = 'Bad list ID provided to wildfire_list_add_optout: lid was %lid';
    $params = array('%lid' => $lid);
  }
  if ($uid < 1) {
    $msg = 'Bad user ID provided to wildfire_list_add_optout: uid was %uid';
    $params = array('%uid' => $uid);
  }
  if ($timestamp == 0) {
    $timestamp = REQUEST_TIME;
  }
  if ($method < 0) {
    $method = 0;
  }
  if ($method == 2) {
    $unsubscriber = $uid;
  }
  if ($unsubscriber < 1) {
    // If the unsubscriber was not set, use the current user's UID.
    global $user;
    $unsubscriber = $user->uid;
  }
  if ($msg !== FALSE) {
    watchdog('wildfire_lists', $msg, $params, WATCHDOG_ERROR);
    return FALSE;
  }

  $query = db_select('wildfire_optouts', 'co')
    ->fields('co', array(
      'uid'
    ));
  $query->condition('lid', $lid, '=');
  $query->condition('uid', $uid, '=');
  $result = $query->execute();

  $result = $result->fetchField();

  // Make sure that the optout is not already in the database. If it is, don't
  // try to add another.
  if ($result === FALSE) {
    $record = array(
      'lid' => $lid,
      'uid' => $uid,
      'timestamp' => $timestamp,
      'method' => $method,
      'unsubscriber' => $unsubscriber,
    );
    drupal_write_record('wildfire_optouts', $record);

    watchdog(
      'wildfire_lists',
      'User ID !uid unsubscribed from List ID !lid on !timestamp via method !method',
      array(
        '!uid' => $uid,
        '!lid' => $lid,
        '!timestamp' => $timestamp,
        '!method' => $method,
      ),
      WATCHDOG_INFO
    );

    /**
     * If this is a new opt out, we need to clear out the users email address
     * from any pending jobs to prevent mails from being sent to the user after
     * the point that they unsubscribed.
     */
    $user = user_load($uid);
    wildfire_lists_user_remove_queued($user->mail);

    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Get user data including profile fields by list and add to a csv.
 *
 * @param int $lid
 *   The list ID of the list to export.
 * @param array $statuses
 *   An array where the values are the statuses to be exported, such as
 *   WILDFIRE_STATUS_SUBSCRIBED, and the values must be the integer 1.
 * @return
 *   An array of arrays containing all the user data that is available for each
 *   user, where the outer array keys are the user IDs, and the inner arrays are
 *   contain the basic user details, along with any applicable profile fields.
 */
function wildfire_list_get_users(
$lid = 0,
$statuses = array(WILDFIRE_STATUS_SUBSCRIBED)
) {
  // Parameter checking.
  if (!wildfire_list_valid($lid)) {
    wildfire_error(
      'Invalid list ID "@lid" in wildfire_list_get_users().',
      array('@lid' => $lid),
      'admin/wildfire/lists'
    );
  }

  // Get all default user data for this list. We'll filter it later.
  $query = db_select('wildfire_lists_users', 'clu')
    ->fields('u', array(
      'uid',
    ));
  $query->fields('co', array(
      'timestamp'
  ));
  $query->leftJoin('users', 'u', 'clu.uid = u.uid');
  $query->leftJoin('wildfire_optouts', 'co', 'clu.uid = co.uid');
  $query->condition('clu.lid', $lid, '=');
  $query->condition('u.uid', 0, '>');
  $query->addExpression(
    'CASE WHEN co.timestamp IS NULL THEN :sub ELSE :unsub END',
    'status',
    array(
      ':sub' => WILDFIRE_STATUS_SUBSCRIBED,
      ':unsub' => WILDFIRE_STATUS_UNSUBSCRIBED,
    )
  );
  $result = $query->execute();

  // If the 'fields' module  is enabled, ensure any fields defined on the
  // 'user' entity type are also included in the export.
  if (module_exists('field')) {
    $fields = field_info_instances('user');
  }

  $users = array();
  $statustext = wildfire_options_list_statuses();

  while ($dbrow = $result->fetchAssoc()) {
    // If this user has a status we're not interested in, skip it.
    if (!in_array($dbrow['status'], $statuses)) {
      continue;
    }

    // We need a fully-loaded user entity in order to retrieve fields from
    // it later on.
    $account = user_load($dbrow['uid']);

    // Set up basic fields for the user.
    $user = array(
      'User ID' => $dbrow['uid'],
      'Username' => $account->name,
      'Email' => $account->mail,
      'Status' => $statustext[$dbrow['status']],
      'Opted out' => $dbrow['timestamp'] > 0 ?
                      format_date($dbrow['timestamp'], 'custom', WILDFIRE_DATETIME) :
                      0,
    );

    $users[$dbrow['uid']] = $user;

    if (module_exists('field')) {
      foreach ($fields['user'] as $field) {
        $field_item = field_get_items(
          'user',
          $account,
          $field['field_name']
        );
        $users[$dbrow['uid']][$field['label']] = $field_item[0]['safe_value'];
      }
    }

  }

  return $users;
}



/**
 * Returns the lists as an associative array for use in a select form element.
 *
 * @return array
 *    All mailing lists as an associative array, whose keys are the list IDs and
 *    values are the list names. If the user has permission to add a new list,
 *    this is the first option in the array, whose key is 0.
 */
function wildfire_lists_import_get_lists() {

  $existinglists = wildfire_lists_get();
  $lists = array(0 => t('--- Create a new list ---'));

  foreach ($existinglists as $key => $value) {
    $lists[$key] = $value;
  }

  return $lists;

}



/**
 * Adds a user to a list. There is no checking for parameter validity.
 *
 * Well, ok, there is some, but it is very basic. It will also check if the user
 * is already in the list, to avoid adding in that situation.
 *
 * @param $lid int
 *    The list ID to which the user should be added.
 * @param $uid int
 *    The user ID of the user to add.
 * @return string
 *    FALSE if there was a problem. Otherwise, returns TRUE.
 */
function wildfire_lists_user_add($uid = 0, $lid = 0) {
  // Sanity check.
  if (!wildfire_valid_user($uid) || !wildfire_list_valid($lid)) {
    return FALSE;
  }

  // Check that the user exists?
  /**
   * TODO: Abstract users so that list entries only need a user account for
   * custom fields. See Codebase ticket #145
   */
  $account = user_load($uid);
  if (!$account) {
    return FALSE;
  }

  // Check that the user is not already in the list.
  $query = db_select('wildfire_lists_users', 'clu')
    ->fields('clu', array(
      'uid'
    ));
  $query->condition('uid', $uid, '=');
  $query->condition('lid', $lid, '=');
  $result = $query->execute();

  if ($result->fetchField() === FALSE) {

    /**
     * Add the user to the list. We default to WILDFIRE_LIST_USER_UNLOCKED
     * Only users that are locked will get added during list processing for send
     * then get set as unlocked as they are processed. If it's already unlocked,
     * it doesn't get processed. Simple, eh?
     */
    $record = array(
      'uid' => $uid,
      'lid' => $lid,
      'locked' => WILDFIRE_LIST_USER_UNLOCKED
    );
    drupal_write_record('wildfire_lists_users', $record);
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Removes the specified user from the specified list.
 *
 * @param int $uid
 *    The user ID.
 * @param $lid
 *    The list ID.
 * @return
 *    TRUE if the user was removed. FALSE if the user was not in the list or
 *    the parameters were not valid.
 */
function wildfire_lists_user_remove($uid = 0, $lid = 0) {
  // Parameter checking.
  if (!wildfire_valid_user($uid) || !wildfire_list_valid($lid)) {
    return FALSE;
  }

  // Everything's ok, so check whether the user is in the list or not.
  $query = db_select('wildfire_lists_users', 'clu')
    ->fields('clu', array(
      'uid'
    ));
  $query->condition('uid', $uid, '=');
  $query->condition('lid', $lid, '=');
  $result = $query->execute();

  if ($result->fetchField() !== FALSE) {

    /**
     * If the list is currently locked, mark the user for removal, but don't
     * actually perform the removal. The user will get deleted when the list
     * gets unlocked, or on cron cleanup.
     */

    if (wildfire_list_is_locked($lid)) {
      // List is presently locked
      $record = array(
        'lid' => $lid,
        'uid' => $uid,
        'locked' => WILDFIRE_LIST_USER_PENDING_REMOVAL
      );
      $result = drupal_write_record('wildfire_lists_users', $record, array('lid', 'uid'));
      return $result !== FALSE ? TRUE : FALSE;
    }
    else {
      // Delete the user as the list is unlocked.
      db_delete('wildfire_lists_users')
        ->condition('uid', $uid)
        ->condition('lid', $lid)
        ->execute();
      return TRUE;
    }

  }

  // The user was not in the list.
  return FALSE;
}

/**
 * Gets the emails of the users in the specified list.
 *
 * Omits users who have opted out.
 *
 * @param $lid int
 *    The list ID of the list to use.
 * @param $include_optouts bool
 *    Whether to include opted-out addresses in the results. Defaults to FALSE
 * @return mixed
 *    An array of users in the list, or FALSE if the list was not found. Each
 *    user is an array with the keys 'uid' and 'mail'.
 */
function wildfire_list_get($lid = 0, $include_optouts = FALSE) {
  // Check the list is valid.
  if (!wildfire_list_valid($lid)) {
    return FALSE;
  }

  $wildfire_list_contents =& drupal_static(__FUNCTION__);

  if (!isset($wildfire_list_contents[$lid][$include_optouts])) {

    $query = db_select('wildfire_lists_users', 'clu')
      ->fields('clu', array(
        'locked'
      ))
      ->fields('u', array(
        'uid',
        'mail',
      ));
    $query->addField('clu', 'mail', 'mail_from_list');
    $query->leftJoin('users', 'u', 'clu.uid = u.uid');
    $query->condition('clu.lid', array($lid, 0), 'IN');
    if (!$include_optouts) {
      $query->leftJoin('wildfire_optouts', 'co', 'clu.uid = co.uid');
      $query->isNull('co.uid');
    }
    $result = $query->execute();

    while ($dbrow = $result->fetchAssoc()) {
      /**
       * Users listed under UID 0 are anonymous users, so we should use
       * the stored email address from the list row rather than deriving this
       * from the UID as we do for all registered users.
       */
      if ($dbrow['uid'] == 0) {
        if (!empty($dbrow['mail_from_list'])) {
          $dbrow['mail'] = $dbrow['mail_from_list'];
        }
      }
      unset($dbrow['mail_from_list']);

      $wildfire_list_contents[$lid][$include_optouts][] = $dbrow;
    }
  }

  return $wildfire_list_contents[$lid][$include_optouts];
}

/**
 * Remove a given optout for a specific list and user.
 *
 * @param int $lid
 *    The list ID. 0 for a global optout.
 * @param int $uid
 *    The user ID.
 */
function wildfire_list_remove_optout($lid = 0, $uid = 0) {
  $msg = FALSE;
  $params = array();

  // Check parameters.
  if (!wildfire_list_valid($lid) && $lid != 0) {
    $msg = 'Bad list ID provided to wildfire_list_remove_optout: lid was %lid';
    $params = array('%lid' => $lid);
  }
  if ($uid < 1) {
    $msg = 'Bad user ID provided to wildfire_list_remove_optout: uid was %uid';
    $params = array('%uid' => $uid);
  }
  if ($msg !== FALSE) {
    watchdog('wildfire', $msg, $params, WATCHDOG_ERROR);
    return FALSE;
  }

  // Perform the delete.
  db_delete('wildfire_optouts')
    ->condition('lid', $lid)
    ->condition('uid', $uid)
    ->execute();

  return TRUE;
}

/**
 * Remove ALL of a user's optouts.
 *
 * @param int $uid
 *    The user ID of the user for whom the optouts should be removed.
 * @return bool
 *    TRUE if successful. FALSE otherwise, such as if parameters were invalid.
 */
function wildfire_list_remove_optouts($uid = 0) {
  $msg = FALSE;
  $params = array();

  // Check parameters.
  if ($uid < 1) {
    $msg = 'Bad user ID provided to wildfire_list_remove_optouts: uid was %uid';
    $params = array('%uid' => $uid);
  }
  if ($msg !== FALSE) {
    watchdog('wildfire', $msg, $params, WATCHDOG_ERROR);
    return FALSE;
  }

  // Perform deletes.
  db_delete('wildfire_optouts')
    ->condition('uid', $uid)
    ->execute();

  return TRUE;
}

/**
 * Remove a user from the opt-outs list, putting them back into a mailing list.
 *
 * @param int $uid
 *    The user ID.
 * @param int $lid
 *    The list ID.
 * @return bool
 *    TRUE if the user's opt-out was removed. FALSE if the user had not opted
 *    out or the parameters were not valid.
 */
function wildfire_list_resubscribe($uid = 0, $lid = 0) {
  // Check parameters.
  if (!wildfire_valid_user($uid)) {
    return FALSE;
  }
  if ($lid > 0 && !wildfire_list_valid($lid)) {
    return FALSE;
  }

  $query = db_select('wildfire_optouts', 'co')
    ->fields('co', array(
      'uid'
    ));
  $query->condition('uid', $uid, '=');
  $query->condition('lid', $lid, '=');
  $result = $query->execute();

  if ($result->fetchField() !== FALSE) {
    db_delete('wildfire_optouts')
      ->condition('uid', $uid)
      ->condition('lid', $lid)
      ->execute();
    return TRUE;
  }

  // The user was not opted out to begin with.
  return FALSE;
}

/**
 * Whether an array (such as a row from a CSV) contains an email address.
 *
 * @param $row array
 *    The array containing the values that should be checked for an email
 *    address. Note that if this parameter is not an array, the value will be
 *    searched instead.
 * @return mixed
 *    FALSE if none of the rows contain an email address, otherwise the email
 *    address itself is returned.
 */
function wildfire_row_contains_email_address($row) {
  $index = 0;

  // If not an array, just test the value itself.
  if (!is_array($row)) {
    return wildfire_valid_email_address($row);
  }

  foreach ($row as $col) {
    if (wildfire_valid_email_address($col)) {
      return $col;
    }
  }

  return FALSE;
}

/**
 * Unsubscribe the specified user from the specified list.
 *
 * In this version, the list ID is always 0 because only global unsubscribes
 * are supported.
 *
 * Checks if the user has already opted out, and will not change anything if
 * this is the case.
 *
 * @param int $uid
 *    The user ID.
 * @param int $lid
 *    The list ID. Not used. Will always revert to 0 no matter what value is
 *    passed.
 * @return bool
 *    TRUE if a new unsubscribe was created. FALSE if the unsubscribe was
 *    already there or if the parameters were invalid.
 */
function wildfire_list_unsubscribe($uid = 0, $lid = 0) {
  // Force lid to be 0. Global unsubscribes only.
  $lid = 0;

  // Check the user parameter.
  $query = db_select('users', 'u')
    ->fields('u', array(
      'uid'
    ));
  $query->condition('uid', $uid, '=');
  $result = $query->execute();

  if ($result->fetchField() === FALSE) {
    return FALSE;
  }

  // Make sure that there is not already an unsubscribe here.
  $query = db_select('wildfire_optouts', 'co')
    ->fields('co', array(
      'uid'
    ));
  $query->condition('uid', $uid, '=');
  $query->condition('lid', $lid, '=');
  $result = $query->execute();

  if ($result->fetchField() === FALSE) {
    // There is no unsubscribe yet. Make one, and return TRUE.
    wildfire_list_add_optout($lid, $uid, REQUEST_TIME, 2);
    return TRUE;
  }
  else {
    // There was already an unsubscribe. Return FALSE here.
    return FALSE;
  }
}

/**
 * Test whether a specific list exists.
 *
 * @param $lid int
 *    The list ID of the list to check.
 * @return bool
 *    TRUE if the list exists. FALSE otherwise.
 */
function wildfire_list_valid($lid = 0) {

  $query = db_select('wildfire_lists', 'cl')
    ->fields('cl', array(
      'lid'
    ));
  $query->condition('lid', $lid, '=');
  $result = $query->execute();

  if ($result->fetchField() === FALSE) {
    return FALSE;
  }
  else {
    return TRUE;
  }
}

/**
 * Obtains a list of test lists from the system
 *
 * @return array
 *    List of test lists keyed by lid.
 */
function wildfire_lists_get_test_lists() {

  $num_entries_in_title = 3;

  $test_lists = wildfire_lists_get(
    'recent',
    TRUE,
    WILDFIRE_MAX_TEST_LISTS,
    WILDFIRE_LIST_STATUS_TEST
  );

  foreach ($test_lists as $lid => $name) {

    $list = wildfire_list_get($lid, TRUE);
    if (!empty($list)) {

      $t = 0;
      $email_addresses = array();

      foreach ($list as $row) {

        $email_addresses[] = $row['mail'];

        if (++$t >= $num_entries_in_title) {
          $others = count($list) - $num_entries_in_title;
          if ($others > 0) {
            $email_addresses[] = format_plural(
              $others,
              'and 1 other',
              'and !num others',
              array(
                '!num' => $others
              )
            );
          }
          break;
        }

      }

      $test_lists[$lid] = t(
        '!addresses',
        array(
          '!addresses' => implode(', ', $email_addresses),
        )
      );

    }
    else {
      $test_lists[$lid] = t('No addresses');
    }

  }

  return $test_lists;

}

  /**
   * Takes a list of email addresses, and updates an existing
   * test list that contains them, adds a new test list, or recycles an old list
   *
   * @param $cs_addresses mixed
   *    Array: array of email addresses to store into a test list.
   *    String: comma-separated list of email addresses to store.
   *
   * @return int
   *    ID of the list that was updated/added, or FALSE on failure.
   */
function wildfire_lists_save_test_list($cs_addresses) {

  // Get the email addresses the user put in.
  if (is_string($cs_addresses)) {
    $email_addresses = preg_split(
      '/[\s,]/',
      $cs_addresses,
      //$form_state['storage']['values']['recipient_addresses'],
      NULL,
      PREG_SPLIT_NO_EMPTY
    );
    $email_addresses = array_unique($email_addresses);
  }
  else {
    if (is_array($cs_addresses)) {
      $email_addresses =& $cs_addresses;
    }
    else {
      // Bad input!
      return FALSE;
    }
  }

  sort($email_addresses);

  // Make a hash of the addresses. This makes finding an existing list
  // that contains the same set of addresses much quicker and easier.
  $addresses_hash = sha1(serialize($email_addresses));

  // Does this hash match an existing list?
  $query = db_select('wildfire_lists', 'cwl');
  $query->fields('cwl', array(
    'lid'
  ));
  $query->condition('users_hash', $addresses_hash, '=');
  $query->condition('status', WILDFIRE_LIST_STATUS_TEST, '=');
  $result = $query->execute();

  $lid = $result->fetchField();
  if ($lid !== FALSE) {
    /**
     * The hash matches an existing list. Therefore, we assume that the list is
     * correct, and only need to update the last sent to timestamp.
     */
    $query = db_update('wildfire_lists');
    $query->fields(array(
      'last_sent_to' => REQUEST_TIME
    ));
    $query->condition('lid', $lid, '=');
    $query->execute();
  }
  else {
    /**
     * We didn't find an existing list with the same hash.
     *
     * If count of test lists < WILDFIRE_MAX_TEST_LISTS: Add new list
     * If count of test lists >= WILDFIRE_MAX_TEST_LISTS: Recycle oldest list
     */
    $query = db_select('wildfire_lists', 'cwl');
    $query->addExpression('count(lid)', 'c');
    $query->condition('status', WILDFIRE_LIST_STATUS_TEST, '=');
    $result = $query->execute();

    if ($result->fetchField() >= WILDFIRE_MAX_TEST_LISTS) {
      // Count is larger than WILDFIRE_MAX_TEST_LISTS, so we will recycle the
      // oldest list
      $query = db_select('wildfire_lists', 'cwl');
      $query->fields('cwl', array(
        'lid'
      ));
      $query->range(0, 1);
      $query->orderBy('last_sent_to', 'ASC');
      $query->condition('status', WILDFIRE_LIST_STATUS_TEST, '=');
      $result = $query->execute();

      $lid = $result->fetchField();
      if ($lid !== FALSE) {
        $query = db_update('wildfire_lists');
        $query->fields(array(
          'last_sent_to' => REQUEST_TIME,
          'users_hash' => $addresses_hash,
        ));
        $query->condition('lid', $lid, '=');
        $query->execute();
      }

    }
    else {
      // We haven't hit WILDFIRE_MAX_TEST_LISTS yet, so let's add a new one
      $query = db_insert('wildfire_lists');
      $query->fields(array(
        'name' => 'TEST LIST ' . REQUEST_TIME,
        'created' => REQUEST_TIME,
        'last_sent_to' => REQUEST_TIME,
        'users_hash' => $addresses_hash,
        'status' => WILDFIRE_LIST_STATUS_TEST
      ));
      $lid = $query->execute();
    }

    // In both scenarios, delete any existing entries into the test lists, and
    // add the provided list of email addresses to it.
    if ($lid !== FALSE) {

      $query = db_delete('wildfire_lists_users');
      $query->condition('lid', $lid, '=');
      $query->execute();

      foreach ($email_addresses as $email_address) {

        // Try to load each user. If successful, record the UID too
        $account = user_load_by_mail($email_address);
        $uid = 0;
        if ($account !== FALSE) {
          $uid = $account->uid;
        }

        $query = db_insert('wildfire_lists_users');
        $query->fields(array(
          'lid' => $lid,
          'uid' => $uid,
          'mail' => $email_address,
        ));
        $query->execute();

      }

    }

  }

  return $lid;

}

/**
 * Provide all subscriber statuses for a FAPI form.
 */
function wildfire_options_list_statuses() {
  return array(
    WILDFIRE_STATUS_SUBSCRIBED => t('Subscribed'),
    WILDFIRE_STATUS_UNSUBSCRIBED => t('Unsubscribed'),
  );
}

/**
 * Removes a user entry from any queues
 *
 * @param string $mail
 *  The email address to remove.
 *
 */
function wildfire_lists_user_remove_queued($mail) {

  // Obtain a list of all of the pending jobs in this system
  $statuses = array(
    WILDFIRE_JOB_PENDING,
    WILDFIRE_JOB_PREPARING,
    WILDFIRE_JOB_PREPARED,
  );

  $query = db_select('wildfire_jobs', 'wj')
    ->fields('wj');
  $query->condition('status', $statuses, 'IN');
  $job_result = $query->execute();

  while ($row = $job_result->fetchAssoc()) {

    // Find out if the user is in the list queued to be sent
    $query = db_select('wildfire_job_mails', 'wjm')
      ->fields('wjm');
    $query->condition('mail', $mail, '=');
    $query->condition('jid', $row['jid'], '=');
    $jm_result = $query->execute()->fetchAssoc();

    if (!empty($jm_result)) {

      // User was found queued up to be sent for a job; remove the entry.
      $query = db_delete('wildfire_job_mails');
      $query->condition('mail', $jm_result['mail'], '=');
      $query->condition('jid', $row['jid'], '=');
      $query->execute();

      if ($jm_result['uploaded'] == WILDFIRE_JOB_PART_UPLOADED) {

        /**
         * If the user has already been uploaded to the server, ask to remove
         * the entry
         */
        try {

          module_load_include('inc', 'wildfire', 'wildfire.rpc');
          wildfire_rpc_send_request(
            'wildfire.lists.removeUser',
            array(
              'mail' => $mail,
            )
          );

        }
        catch (Exception $e) {

          /**
           * Something went wrong with the removal. Tell the end user
           * and log it.
           */
          drupal_set_message(
            $e->getMessage(),
            'error'
          );

          watchdog(
            'wildfire_lists',
            $e->getMessage(),
            WATCHDOG_ERROR
          );

        }

      }
      else {

        /**
         * User has yet to be uploaded, only need to amend the list count
         * to reflect the fact it should be expecting one less entry
         */
        try {

          $job = new WildfireClientJobRpc($row['jid']);

          $job->sendRequest(
            'wildfire.job.amendJob',
            array(
              'uid' => $row['juid'],
              'list_count' => $job->getListUsersCount(),
            )
          );

        }
        catch (Exception $e) {

          /**
           * Something went wrong with the amendment. Tell the end user
           * and log it.
           */
          drupal_set_message(
            $e->getMessage(),
            'error'
          );

          watchdog(
            'wildfire_lists',
            $e->getMessage(),
            WATCHDOG_ERROR
          );

        }

      }



    }

  }

}

/**
 * Locks a list
 *
 * In locked mode, any updates to the list are still made but are marked as
 * pending whilst locked. When the list is unlocked, any pending updates are
 * committed at that point.
 *
 * @param $lid
 *  The list to lock
 *
 * @return bool
 *  TRUE if the list was locked, else FALSE.
 */
function wildfire_list_lock($lid) {

  $record = array(
    'lid' => $lid,
    'locked' => WILDFIRE_LIST_LOCKED
  );

  $result = drupal_write_record('wildfire_lists', $record, 'lid');

  return $result !== FALSE ? TRUE : FALSE;

}

/**
 * Unlocks a list
 *
 * When a list gets unlocked, any updates that were made will be actioned
 * appropriately to get the list back to the normal, unlocked order.
 *
 * @param int $lid
 *  The list to unlock
 *
 * @return bool
 *  TRUE if the list was successfully unlocked, else FALSE
 */
function wildfire_list_unlock($lid) {

  $record = array(
    'lid' => $lid,
    'locked' => WILDFIRE_LIST_UNLOCKED
  );

  $result = drupal_write_record('wildfire_lists', $record, 'lid');

  /**
   * Check any entries that are not unlocked at this stage. There's two ways
   * this could be the case:
   *
   * 1/ User was marked as WILDFIRE_LIST_USER_PENDING_REMOVAL whilst the list
   *    was in use. Retry wildfire_lists_user_remove() on the user now.
   * 2/ User globally opted-out whilst the processing was being done, so the
   *    row was skipped and is still in the WILDFIRE_LIST_USER_LOCKED state.
   *    Reset the flag.
   */

  /**
   * We handle any entries specifically for case 1, then just bulk cleanup any
   * remaining locked entries thereafter.
   */
  $query = db_select('wildfire_lists_users', 'wlu')
    ->fields('wlu', array(
      'uid',
      'locked'
  ));
  $query->condition('locked', WILDFIRE_LIST_USER_PENDING_REMOVAL, '=');
  $query->condition('lid', $lid, '=');
  $list_users = $query->execute();

  while ($row = $list_users->fetchAssoc()) {
    // Remove the pending entry now
    wildfire_lists_user_remove($row['uid'], $lid);
  }

  /**
   * Now that we've handled any special cases, bulk update everything back to
   * WILDFIRE_LIST_USER_UNLOCKED
   */
  $query = db_update('wildfire_lists_users');
  $query->fields(array(
    'locked' => WILDFIRE_LIST_USER_UNLOCKED,
  ));
  $query->condition('lid', $lid, '=');
  $query->execute();

  return $result !== FALSE ? TRUE : FALSE;

}

/**
 * Determines if a list is locked
 *
 * @param int $lid
 *  The list ID to check
 *
 * @return bool
 *  TRUE if the list is locked, else FALSE
 */
function wildfire_list_is_locked($lid) {

  $query = db_select('wildfire_lists', 'wl')
    ->fields('wl', array(
      'locked'
    ));
  $query->condition('lid', $lid, '=');
  $result = $query->execute()->fetchField(0);

  return $result == WILDFIRE_LIST_UNLOCKED ? FALSE : TRUE;

}

/**
 * Menu callback to toggle the lock on a list
 *
 * @param $lid
 *  The list ID to check
 *
 * @return NULL
 */
function wildfire_lists_lock_toggle($lid) {

  $status = 'unchanged';

  if (wildfire_list_is_locked($lid)) {
    if (wildfire_list_unlock($lid)) {
      $status = 'unlocked';
    }
  }
  else {
    if (wildfire_list_lock($lid)) {
      $status = 'locked';
    }
  }

  drupal_set_message(
    t(
      'Lock state of list #!lid is !status',
      array(
        '!lid' => $lid,
        '!status' => $status,
      )
    ),
    'status'
  );

  drupal_goto('admin/wildfire/lists');

}

/**
 * Sets all list users that are currently unlocked to a locked state. It will
 * skip setting the locked flag on any users that are presently
 * WILDFIRE_LIST_USER_PENDING_REMOVAL or other locked statuses.
 *
 * Used when preparing a job to set up the list for copying to
 * wildfire_job_mails, etc.
 *
 * @param int $lid
 *  The list to set all users to locked on.
 *
 * @return bool
 *  TRUE if the users were locked, else FALSE. This function will return FALSE
 *  if the parent list is not in a locked state when the request is made.
 */
function wildfire_list_users_lock_all($lid) {

  // Only lock users if the parent list is locked at present.
  if (wildfire_list_is_locked($lid)) {

    $query = db_update('wildfire_lists_users');
    /**
     * This excludes any user that is already WILDFIRE_LIST_USER_PENDING_REMOVAL
     * - there should effectively be none, as that flag can only be set on a
     * user whilst the list is in a locked state.
     */
    $query->fields(array(
      'locked' => WILDFIRE_LIST_USER_LOCKED,
    ));
    $query->condition('lid', $lid, '=');
    $query->condition('locked', WILDFIRE_LIST_USER_UNLOCKED, '=');
    $query->execute();

    return TRUE;

  }

  return FALSE;

}

/**
 * Unlocks a user on a specific list.
 *
 * @param int $lid
 *  The list ID to unlock the user on
 * @param $uid
 *  The UID of the user to unlock
 *
 * @return bool
 *  TRUE if the users were unlocked, else FALSE. This function will return FALSE
 *  if the parent list is not in a locked state when the request is made.
 */
function wildfire_list_user_unlock($lid, $uid) {

  // Only allow unlock if the parent list is locked at present.
  if (wildfire_list_is_locked($lid)) {

    $record = array(
      'lid' => $lid,
      'uid' => $uid,
      'locked' => WILDFIRE_LIST_USER_UNLOCKED
    );

    $result = drupal_write_record('wildfire_lists_users', $record, array('lid', 'uid'));

    return $result !== FALSE ? TRUE : FALSE;

  }

  return FALSE;

}

/**
 * Check whether a user is subscribed to a particular list.
 *
 * Will also return FALSE if the user has a global opt-out.
 *
 * @param int $uid
 *    The user ID of the user.
 * @param int $lid
 *    The list ID of the list. If not specified, the function will return TRUE
 *    if the user is subscribed to ANY lists at all.
 */
function wildfire_user_is_subscribed($uid = 0, $lid = 0) {
  // Check user parameter.
  if (!wildfire_valid_user($uid)) {
    return FALSE;
  }

  // If list parameter is not valid, set it to 0 (global subscribes).
  if (!wildfire_list_valid($lid)) {
    $lid = 0;
  }

  // If the user has opted out globally, don't bother checking anything else.
  $query = db_select('wildfire_optouts', 'co')
    ->fields('co', array(
      'uid'
    ));
  $query->condition('uid', $uid, '=');
  $query->condition('lid', 0, '=');
  $result = $query->execute();

  $global_optout = $result->fetchField();

  if ($global_optout !== FALSE) {
    return FALSE;
  }

  // Work out whether we're checking for subscription to a particular list, or
  // just ANY subscription (lid == 0).
  if ($lid == 0) {
    // We're looking for ANY subscription at all. Get the number of lists that
    // the user is in.
    $query = db_select('wildfire_lists_users', 'clu');
    $query->condition('uid', $uid, '=');
    $query->addExpression('COUNT(1)', 'count');
    $result = $query->execute();

    // If the user is in at least one list, return TRUE. FALSE otherwise.
    return ($result->fetchField() > 0);
  }
  else {
    // We're looking for subscription to a particular list. Make sure the user
    // is actually mapped to the list.
    $query = db_select('wildfire_lists_users', 'clu')
      ->fields('clu', array(
        'uid'
      ));
    $query->condition('uid', $uid, '=');
    $query->condition('lid', $lid, '=');
    $result = $query->execute();

    if ($result->fetchField() === FALSE) {
      return FALSE;
    }

    // The user is mapped to the list. Test whether the user has opted out, to
    // this list or globally, we don't mind.
    $query = db_select('wildfire_optouts', 'co');
    $query->condition('uid', $uid, '=');
    $query->condition('lid', array($lid, 0), 'IN');
    $query->addExpression('COUNT(1)', 'count');
    $result = $query->execute();

    return ($result->fetchField() == 0);
  }
}

/**
 * Get all of the user's optouts.
 *
 * @param int $uid
 *    The user ID of the user whose optouts to find.
 * @return array
 *    An array where the keys are list IDs, and the values are arrays with the
 *    following keys:
 *    - lid:          The list ID, or 0 if this is a global optout.
 *    - timestamp:    The UNIX timestamp the optout was made.
 *    - method:       See schema documentation in the .install file for values.
 *    - unsubscriber: The user ID of the person who made this optout.
 */
function wildfire_user_get_optouts($uid = 0) {
  $msg = FALSE;
  $params = array();

  // Check parameters.
  if ($uid < 1) {
    $msg = 'Bad user ID provided to wildfire_user_get_optouts: uid was %uid';
    $params = array('%uid' => $uid);
  }
  if ($msg !== FALSE) {
    watchdog('wildfire', $msg, $params, WATCHDOG_ERROR);
    return FALSE;
  }

  $optouts = array();

  $result = db_select('wildfire_optouts', 'co')
    ->fields('co', array(
      'lid',
      'uid',
      'timestamp',
      'method',
      'unsubscriber',
    ))
    ->condition('uid', $uid, '=')
    ->execute();

  // Go through each optout and add it to the array we will return.
  while ($row = $result->fetchAssoc()) {
    $optouts[$row['lid']] = array(
      'lid' => $row['lid'],
      'timestamp' => $row['timestamp'],
      'method' => $row['method'],
      'unsubscriber' => $row['unsubscriber'],
    );
  }

  return $optouts;
}

/**
 * Cleans a list by removing duplicates and/or invalid entries
 *
 * This effectively creates a batch job to process the list, as large lists
 * can't be processed in one go.
 *
 * @param int $lid
 *  The List ID to clean
 * @param array $clean_options
 *  An array of options to apply to the clean. Currently, this accepts the
 *  following options:
 *    'remove-duplicates'
 *    'remove-invalid'
 *  All options default to TRUE if not specified
 *
 * @return bool
 *  TRUE if the batch add was successful, else FALSE
 */
function wildfire_list_clean($lid, $clean_options) {

  $list_options = array(
    'lid' => $lid,
    'options' => $clean_options,
  );

  $batch = array(
    'operations' => array(
      array(
        'wildfire_list_batch_clean',
        array(
          $list_options
        ),
      ),
    ),
    'finished' => 'wildfire_list_batch_clean_finished',
    'title' => t('Cleaning list'),
    'init_message' => t('Cleaning list is starting.'),
    'progress_message' => t('Processing. Please wait…'),
    'error_message' => t('Cleaning the list has encountered an error.'),
    'file' => drupal_get_path('module', 'wildfire_lists') . '/wildfire_lists.api.inc',
  );

  batch_set($batch);

  return TRUE;

}

/**
 * Batch job for list cleaning.
 *
 * @param array $list_options
 *  An array of list options, containing the following keys:
 *  'lid' => The List ID
 *  'options' => An array of options defining what to clean out of the list,
 *    passed in from the form that starts the batch.
 * @see wildfire_lists_clean_form_submit()
 *
 * @param array &$context
 *  Batch process context. Return values, progress, etc, will be set here by
 *  reference.
 *
 * @return NULL
 */
function wildfire_list_batch_clean($list_options, &$context) {

  // Set the limit per cycle.
  $limit = 100;

  // Set to explicitly not finished so that the batch runs.
  $context['finished'] = 0;

  if (empty($context['sandbox'])) {

    module_load_include('inc', 'wildfire_lists', 'wildfire_lists.api');

    $context['sandbox']['list_position'] = 0;
    $context['sandbox']['list_total'] = wildfire_list_get_count($list_options['lid']);

    // Stores the IDs for entries that need to be cleaned.
    $context['sandbox']['removed_entries'] = array();

    // Store the lid for later use in the finish callback
    $context['results'] = array(
      'removed_entries' => array(),
      'lid' => $list_options['lid'],
    );

    // Lock the list.
    wildfire_list_lock($list_options['lid']);

    // Set all user on the list to locked.
    wildfire_list_users_lock_all($list_options['lid']);

  }

  $remaining = $context['sandbox']['list_total'] - $context['sandbox']['list_position'];

  if ($remaining > 0) {
    // Check the next batch.

    // Select the next batch of users from the list.
    $query = db_select('wildfire_lists_users', 'wlu')
      ->fields('wlu', array(
      'uid',
      'mail',
    ));
    $query->fields('u', array(
      'mail'
    ));
    $query->leftJoin('users', 'u', 'u.uid=wlu.uid');
    $query->condition('wlu.lid', $list_options['lid'], '=');
    $query->range($context['sandbox']['list_position'], $limit);
    $result = $query->execute();

    while($row = $result->fetchAssoc()) {

      if ($row['uid'] !== 0) {
        // Get the users email address from their account.
        $mail = $row['u_mail'];
      }
      else {
        // User is anon, use the anon address stored in the table.
        $mail = $row['mail'];
      }

      // If mail is empty at this point, it's going to be an invalid entry.
      // Try to add it to the uid to remove list and continue
      if (empty($mail)) {
        $context['sandbox']['removed_entries']['uid'][] = $row['uid'];
        continue;
      }

      if ($list_options['options']['remove-duplicates']) {

        // Find out if the count of this email address is more than one.
        $d_query = db_select('wildfire_lists_users', 'wlu')
          ->fields('wlu', array(
          'uid',
          'mail',
        ));
        $d_query->leftJoin('users', 'u', 'u.uid=wlu.uid');

        // Ensure we get either users that have this email address, or anon
        // entries that contain the same email address.
        $or = db_or();
        $or->condition('u.mail', $mail, '=');
        $or->condition('wlu.mail', $mail, '=');
        $d_query->condition($or);
        $d_query->condition('wlu.lid', $list_options['lid'], '=');

        $count_mail = $d_query->execute()->rowCount();

        if ($count_mail > 1) {
          // There's more than one entry, which means neither should be classed
          // as valid as we don't know which user account to obtain tokens for,
          // etc, and we may end up keeping the wrong one.
          if (!empty($row['uid'])) {
            $context['sandbox']['removed_entries']['uid'][] = $row['uid'];
          }
          else {
            $context['sandbox']['removed_entries']['mail'][] = $mail;
          }

        }

      }

      if ($list_options['options']['remove-invalid']) {

        if (!wildfire_valid_email_address($mail)) {
          if (!empty($row['uid'])) {
            $context['sandbox']['removed_entries']['uid'][] = $row['uid'];
          }
          else {
            $context['sandbox']['removed_entries']['mail'][] = $mail;
          }
        }

      }

    }

    // Increment to the next batch of users in the list.
    $context['sandbox']['list_position'] += $limit;

    // Set the progress message.
    $context['message'] = t(
      'Cleaning list: !remaining of !total to process',
      array(
        '!remaining' => $remaining,
        '!total' => format_plural(
          $context['sandbox']['list_total'],
          '1 entry',
          '@count entries'
        ),
      )
    );

  }
  else {

    // Remove any bad entries found by uid
    foreach($context['sandbox']['removed_entries']['uid'] as $uid) {

      // Read the mail entry for this UID directly, to be placed into the
      // results. We read directly from the users table to save the overhead
      // of a user_load(), which may be significant if the number of dirty
      // entries is many.
      $mail_query = db_select('users', 'u')
        ->fields('u', array(
          'mail'
      ));
      $mail_query->condition('uid', $uid, '=');
      $email_address = $mail_query->execute()->fetchField(0);

      $entry = t(
        'Uid !uid, email "!mail"',
        array(
          '!uid' => $uid,
          '!mail' => $email_address,
        )
      );

      // Preform the actual remove from list.
      if (wildfire_lists_user_remove($uid, $list_options['lid'])) {
        $context['results']['removed_entries'][] = $entry;
      }
      else {
        $context['results']['failed_to_remove_entries'][] = $entry;
      }

    }

    // Remove any bad entries found by address
    foreach($context['sandbox']['removed_entries']['mail'] as $email_address) {

      // Get the UID for the email address in the list. If there isn't one,
      // try removing by email address.
      $u_query = db_select('users', 'u')
        ->fields('u', array(
          'uid'
      ));
      $u_query->condition('mail', $email_address, '=');
      $uid = $u_query->execute()->fetchField(0);

      $entry = t(
        'Uid !uid, email "!mail"',
        array(
          '!uid' => $uid,
          '!mail' => $email_address,
        )
      );

      if (!empty($uid)) {
        if (wildfire_lists_user_remove($uid, $list_options['lid'])) {
          $context['results']['removed_entries'][] = $entry;
        }
        else {
          $context['results']['failed_to_remove_entries'][] = $entry;
        }
      }
      else {
        // FIXME: As anon entries are only used for test lists at present,
        // Wildfire cannot yet remove individual anon users from lists.
        // See CB Ticket #145.
        $context['results']['failed_to_remove_entries'][] = $entry;
      }

    }

    // We are done!
    $context['finished'] = 1;

  }

}

/**
 * Callback on job batch completion
 *
 * @param bool $success
 *  TRUE if the batch succeeded, else FALSE
 * @param array $results
 *  The contents of $context['results'] inside the batch run
 * @param array $operations
 *  The remaining operations in the batch run.
 */
function wildfire_list_batch_clean_finished($success, $results, $operations) {

  if ($success) {

    // Show successful removals as status.
    if (!empty($results['removed_entries'])) {

      drupal_set_message(
        t(
          'Cleaned the following entries from list !lid:',
          array(
            '!lid' => $results['lid']
          )
        ),
        'status'
      );
      foreach ($results['removed_entries'] as $removed_entry) {
        drupal_set_message(check_plain($removed_entry), 'status');
      }

      $message = t(
        'Cleaned the following entries from list !lid: !removed_entries',
        array(
          '!removed_entries' => implode(', ', $results['removed_entries']),
          '!lid' => $results['lid'],
        )
      );
      watchdog('wildfire_lists', check_plain($message), NULL, WATCHDOG_INFO);

    }
    else {

      drupal_set_message(
        t(
          'List !lid is clean',
          array(
            '!lid' => $results['lid']
          )
        ),
       'status'
      );

    }

    // Show unsuccessful removals as errors.
    if (!empty($results['failed_to_remove_entries'])) {

      drupal_set_message(
        t(
          'List !lid could not automatically be cleaned. It will require manual cleaning by a Wildfire administrator',
          array(
            '!lid' => $results['lid']
          )
        ),
        'error'
      );

      drupal_set_message(
        t(
          'Failed to remove the following entries from list !lid:',
          array(
            '!lid' => $results['lid']
          )
        ),
        'status'
      );
      foreach ($results['failed_to_remove_entries'] as $email_address) {
        drupal_set_message(check_plain($email_address), 'error');
      }

      $message = t(
        'Failed to remove the following entries from list !lid: !failed_to_remove_entries',
        array(
          '!failed_to_remove_entries' => implode(', ', $results['failed_to_remove_entries']),
          '!lid' => $results['lid'],
        )
      );
      watchdog('wildfire_lists', check_plain($message), NULL, WATCHDOG_ERROR);

    }

    module_load_include('inc', 'wildfire_lists', 'wildfire_lists.api');

    // Unlock the list
    wildfire_list_unlock($results['lid']);

  }
  else {

    $message = t('Batch job for cleaning the list failed to complete successfully');

    drupal_set_message(check_plain($message), 'error');
    watchdog('wildfire_lists', check_plain($message));

    // Log the full input to this callback to the watchdog.

    watchdog(
      'wildfire_lists',
      'Clean list failed: $success = !success, $results = !results, $operations = !operations',
      array(
        '!success' => print_r($success, TRUE),
        '!results' => print_r($results, TRUE),
        '!operations' => print_r($operations, TRUE),
      )
    );

  }

}

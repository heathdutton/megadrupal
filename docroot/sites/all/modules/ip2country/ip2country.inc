<?php

/**
 * @file
 * Utility routines to load the IP to Country database.
 *
 * Data is obtained from one of the five Regional Internet Registries
 * (AFRINIC, ARIN , APNIC, LACNIC, or RIPE).
 *
 * Data format is described by ftp://ftp.arin.net/pub/stats/arin/README
 *
 * This code derived from "class eIp2Country" posted at
 * http://www.tellinya.com/read/2007/06/03/ip2country-translate-ip-address-to-country-code/
 */


/**
 * Updates the database.
 *
 * Truncates ip2country table then reloads from ftp servers.
 *
 * @param $registry
 *   Regional Internet Registry from which to get the data.
 *   Allowed values are afrinic, apnic, arin (default), lacnic, or ripe.
 *
 * @return
 *   FALSE if database update failed. Otherwise, returns the number of
 *   rows in the updated database.
 */
function ip2country_update_database($registry = 'arin') {
  $registry = drupal_strtolower(trim($registry));

  // FTP files.
  if ($registry == 'afrinic') {
    // afrinic only holds its own data - every other NIC has all the data.
    $ftp_urls = array(
      'ftp://ftp.ripe.net/pub/stats/afrinic/delegated-afrinic-extended-latest',
    );
  }
  else {
    // Note arin doesn't play by the file-naming rules.
    $ftp_urls = array(
      'ftp://ftp.ripe.net/pub/stats/arin/delegated-arin-extended-latest',
      'ftp://ftp.ripe.net/pub/stats/apnic/delegated-apnic-latest',
      'ftp://ftp.ripe.net/pub/stats/lacnic/delegated-lacnic-latest',
      'ftp://ftp.ripe.net/pub/stats/afrinic/delegated-afrinic-latest',
      'ftp://ftp.ripe.net/pub/stats/ripencc/delegated-ripencc-latest',
    );
  }

  // Set a run-time long enough so the script won't break.
  drupal_set_time_limit(10*60);  // 10 minutes!

  // Load all the new data into a temporary table so the module still works
  // while we're downloading and validating the new data.

  // Ensure temporary table is missing.
  db_drop_table('ip2country_temp');

  // Obtain schema for {ip2country} table.
  $schema = drupal_get_schema('ip2country', 'ip2country');

  // Create an empty table identical to the {ip2country} table.
  db_create_table('ip2country_temp', $schema);

  // Download data files from the chosen registry.
  $entries = 0;
  $summary_records = 0;
  foreach ($ftp_urls as $ftp_file) {
    // Replace Registry source with chosen registry.
    $ftp_file = str_replace('ftp.ripe', 'ftp.' . $registry, $ftp_file);

    // RipeNCC is named ripe-ncc on APNIC registry.
    if ($registry == 'apnic') {
      $ftp_file = str_replace('stats/ripencc/', 'stats/ripe-ncc/', $ftp_file);
    }

    // Fetch the FTP file using cURL.
    $txt = _ip2country_fetch_page($ftp_file);
    if ($txt == FALSE) {
      // Fetch failed.
      watchdog('ip2country', 'File empty or not found on @registry server: @ftp_file', array('@registry' => drupal_strtoupper($registry), '@ftp_file' => $ftp_file), WATCHDOG_WARNING);
      return FALSE;
    }

    if (variable_get('ip2country_md5_checksum', FALSE)) {
      // Fetch the MD5 checksum using cURL.
      $md5 = _ip2country_fetch_page($ftp_file . '.md5');
      if ($md5 == FALSE) {
        // Fetch failed.
        watchdog('ip2country', 'File not found on @registry server: @ftp_file.md5', array('@registry' => drupal_strtoupper($registry), '@ftp_file' => $ftp_file), WATCHDOG_WARNING);
        return FALSE;
      }

      // Verify MD5 checksum.
      $temp = explode(" ", $md5);
      // ARIN returns two fields, MD5 is in first field.
      // All other RIR return four fields, MD5 is in fourth field.
      $md5 = isset($temp[3]) ? trim($temp[3]) : trim($temp[0]);

      // Compare checksums.
      if ($md5 != md5($txt)) {
        // Checksums don't agree, so drop temporary table,
        // add watchdog entry, then return error.
        db_drop_table('ip2country_temp');
        watchdog('ip2country', 'Validation of database from @registry server FAILED. MD5 checksum provided for the @ftp_file registry database does not match the calculated checksum.', array('@registry' => drupal_strtoupper($registry), '@ftp_file' => $ftp_file), WATCHDOG_WARNING);
        return FALSE;
      }
    }

    // Break the FTP file into records.
    $lines = explode("\n", $txt);
    unset($txt);  // Free up memory.

    // Loop over records.
    $summary_not_found = TRUE;
    foreach ($lines as $line) {
      // Trim each line for security.
      $line = trim($line);

      // Skip comment lines and blank lines.
      if (substr($line, 0, 1) == '#' || $line == '') {
        continue;
      }

      // Split record into parts.
      $parts = explode('|', $line);

      // We're only interested in the ipv4 records.
      if ($parts[2] != 'ipv4') {
        continue;
      }

      // Save number of ipv4 records from summary line.
      if ($summary_not_found && $parts[5] == 'summary') {
        $summary_not_found = FALSE;
        $summary_records += $parts[4];
        continue;
      }

      // The registry that owns the range.
      $owner_registry = $parts[0];

      // The country code for the range.
      $country_code = $parts[1];

      // Prepare the IP data for insert.
      $ip_start     = (int) ip2long($parts[3]);
      $ip_end       = (int) ip2long($parts[3]) + ($parts[4] - 1);
      $range_length = (int) $parts[4];

      // Insert range into the temporary table.
      db_insert('ip2country_temp')->fields(array(
                'ip_range_first'  => min($ip_start, $ip_end),
                'ip_range_last'   => max($ip_start, $ip_end),
                'ip_range_length' => $range_length,
                'country'         => $country_code,
                'registry'        => $owner_registry,
        )
      )->execute();
      $entries++;
    }
    unset($lines);  // Free up memory.
  }

  // Validate temporary table.
  // Check row count matches number of rows reported in the summary record.
  if ($summary_records == $entries) {
    // Start transaction.
    $txn = db_transaction();
    try {
      // Must do this in a transaction so that both functions succeed.
      // Because if one works but the other doesn't we're in trouble.
      db_drop_table('ip2country');
      db_rename_table('ip2country_temp', 'ip2country');
    }
    catch (Exception $e) {
      // Something failed, so roll back transaction.
      // {ip2country} table will remain unchanged by this update attempt.
      $txn->rollback();
      watchdog_exception('ip2country', $e);
      return FALSE;
    }
    unset($txn); // Commit transaction.

    // Record the time of update.
    variable_set('ip2country_last_update', REQUEST_TIME);
    variable_set('ip2country_last_update_rir', drupal_strtoupper($registry));

    // Return count of records in the table.
    return $entries;
  }
  else {
    // Validation failed, so drop temporary table, add watchdog entry,
    // then return error.
    db_drop_table('ip2country_temp');
    watchdog('ip2country', 'Validation of database from @registry server FAILED. Server summary reported @summary rows available, but @entries rows were entered into the database.', array('@registry' => drupal_strtoupper($registry), '@summary' => $summary_records, '@entries' => $entries), WATCHDOG_WARNING);
    return FALSE;
  }
}


/**
 * Fetches the pages via FTP with cURL.
 *
 * @param $url
 *   The ftp URL where the file is located.
 *
 * @return
 *   FALSE if ftp fetch failed. Otherwise, a string containing the contents
 *   of the fetched file.
 */
function _ip2country_fetch_page($url) {
  $curl = curl_init();

  // Fetch requested file.
  curl_setopt($curl, CURLOPT_URL, $url);
  curl_setopt($curl, CURLOPT_TIMEOUT, 60*2);
  curl_setopt($curl, CURLOPT_USERAGENT, 'Drupal (+http://drupal.org/)');
  curl_setopt($curl, CURLOPT_HEADER, FALSE);
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($curl, CURLOPT_FOLLOWLOCATION, TRUE);

  $html = curl_exec($curl);
  curl_close($curl);

  return $html;
}


/**
 * Empties the ip2country table in the database.
 */
function ip2country_empty_database() {
  db_truncate('ip2country')->execute();
}

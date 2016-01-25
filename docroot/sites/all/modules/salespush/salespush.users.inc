<?php

/**
 * @file
 * Users pushing routines.
 */

/**
 * Helper function for sending BULK API csv data.
 *
 * @param string $environment
 *   Environment: 'production', 'sandbox' or other.
 * @param string $wsdl
 *   Full path to wsdl.xml file.
 * @param string $username
 *   Salesforce username.
 * @param string $password
 *   Salesforce password.security_token.
 * @param bool $demo
 *   Enable/disable demo data.
 *
 * @return string
 *   Return log result
 */
function _salespush_user_pusher($environment, $wsdl, $username, $password, $demo = TRUE) {

  $client_data = salespush_get_client($username, $password, $wsdl);
  $session_id = $client_data['session_id'];
  $location = $client_data['location'];

  watchdog('SF ' . $environment . ' session_id:', print_r($session_id, TRUE));
  watchdog('SF ' . $environment . ' location:', print_r($location, TRUE));

  require_once 'vendor/ryanbrainard/forceworkbench/workbench/bulkclient/BulkApiClient.php';
  $my_bulk_api_connection = new BulkApiClient($location, $session_id);
  // Optional, but using here for demo purposes.
  $my_bulk_api_connection->setLoggingEnabled(TRUE);
  // Optional, but recommended. Defaults to true.
  $my_bulk_api_connection->setCompressionEnabled(TRUE);

  // STEP 3: CREATE A NEW JOB.
  // Create in-memory representation of the job.
  $job = new JobInfo();
  $job->setObject("Contact");
  $job->setOpertion("upsert");
  // This is a unique field for data syncing (upserting).
  // Old data will be overwrited.
  // Rows without email will be skipped by salesforce.
  $job->setExternalIdFieldName('Email');
  $job->setContentType("CSV");
  // Can be also set to Serial.
  $job->setConcurrencyMode("Parallel");

  // Send the job to the Bulk API and pass back returned JobInfo to the same variable.
  $job = $my_bulk_api_connection->createJob($job);

  // STEP 4. CREATE A NEW BATCH
  // prep the data. normally this would be loaded from a file,
  // but showing in plain text for demo purposes.
  if ($demo) {
    $csv_data = "\"FirstName\",\"LastName\",\"Email\",\"Title\",\"Created_Date__c\",\"IP_Address__c\",\"Linkedin_ID__c\"\n" .
      "\"TestUserFirstName\",\"TestUserLastName\",\"demo@example.com\",\"Test User Title\",\"2014-08-11T11:05:21\",\"192.168.1.1\",\"NtUhA6OSVn\"\n";
  }
  else {
    $csv_data = salespush_get_users($environment);
  }
  $batch = $my_bulk_api_connection->createBatch($job, $csv_data);

  // STEP 5. CLOSE THE JOB.
  $my_bulk_api_connection->updateJobState($job->getId(), "Closed");

  // STEP 6: GET BATCH RESULTS.
  $batch_results = $my_bulk_api_connection->getBatchResults($job->getId(), $batch->getId());

  // LOGGING EVERYTHING THAT HAPPENED ABOVE.
  watchdog('SF ' . $environment . ' BATCH RESULTS', htmlspecialchars($batch_results));
  $result = $my_bulk_api_connection->getLogs();
  watchdog('SF ' . $environment . ' CLIENT LOGS', $result);
  // Clear log buffer.
  $my_bulk_api_connection->clearLogs();

  // Parse the rows.
  $exp = array();
  $count_success = 0;
  $count_created = 0;
  $count_error = 0;
  $data = @str_getcsv($batch_results, "\n");
  if (count($data) > 0) {
    foreach ($data as &$row) {
      $exp[] = str_getcsv($row);
    }
  }

  if (empty($exp)) {
    return htmlspecialchars_decode($batch_results);
  }

  // Removing headers from CSV response.
  unset($exp[0]);

  foreach ($exp as $key => $item) {
    if (isset($item[1]) && $item[1] == 'true') {
      $count_success++;
    }
    if (isset($item[2]) && $item[2] == 'true') {
      $count_created++;
    }
    if (isset($item[3]) && $item[3] != '') {
      $count_error++;
    }
  }
  watchdog(
    __FUNCTION__ . ':' . __LINE__,
    'Pushed users with success:' . $count_success . ' created: ' . $count_created . ' errors: ' . $count_error
  );

  if ($count_success > 0) {
    variable_set('salespush_total_pushed_contacts_' . $environment, $count_success);
  }

  return htmlspecialchars_decode($batch_results);
}

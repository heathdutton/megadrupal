<?php

/**
 * @file
 * Salespush campaings routines.
 */

/**
 * Helper function for sending BULK API submissions data.
 *
 * This function should be called by hook_cron() for automation.
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
function _salespush_submissions_pusher($environment, $wsdl, $username, $password, $demo = TRUE) {

  $client_data = salespush_get_client($username, $password, $wsdl);
  $session_id = $client_data['session_id'];
  $location = $client_data['location'];
  $client = $client_data['client'];

  watchdog('SF ' . $environment . ' session_id:', print_r($session_id, TRUE));
  watchdog('SF ' . $environment . ' location:', print_r($location, TRUE));

  // STEP 4. CREATE A NEW BATCH
  // prep the data. normally this would be loaded from a file,
  // but showing in plain text for demo purposes.
  $upsert = array();
  $mapper = array();
  if ($demo) {
    $submissions[0] = new stdClass();
    $submissions[0]->CampaignId = '701L000000075ieIAA';
    $submissions[0]->ContactId = '003L000000Mp2Lm';
    $submissions[1] = new stdClass();
    $submissions[1]->CampaignId = '701L000000075ieIAA';
    $submissions[1]->ContactId = '003L000000Mp2Lm';
    // If You want to upsert, not to create new one, uncomment this and fill with a real Id.
    // $submissions[0]->Id = '701L000000075VVIAY';
  }
  else {
    $upsert = salespush_get_submissions($environment);
    $submissions = $upsert['submissions'];
    $mapper = $upsert['mapper'];
  }

  // Salesforce limits 200 records in CampaignMember call.
  $submissions_chunks = array_chunk($submissions, 200);
  $log = '';
  foreach ($submissions_chunks as $chunk_id => $sub_submissions) {
    try {
      $result = $client->upsert('Id', $sub_submissions, 'CampaignMember');
      watchdog('SF submissions ' . $environment, is_array($result) ? count(array_keys($result)) : 0);
    }
    catch (Exception $e) {
      $result = 'Exception: ' . var_export($e, TRUE);
    }

    if (!is_array($result)) {
      $log .= 'Chunk: ' . $chunk_id . ' status error: ' . print_r($result, TRUE) . ' received.\\n';
      continue;
    }

    if (count(array_keys($result)) == 0) {
      $log .= 'Chunk: ' . $chunk_id . ' status error: result returned as empty array.\\n';
      continue;
    }
    foreach ($result as $key => $status) {
      if (isset($status->errors)) {
        foreach ($status->errors as $errorid => $errorclass) {
          watchdog(
            'SF ' . $environment . ' error',
            ' message: ' . $errorclass->message . ' statusCode: ' . $errorclass->statusCode . ' key: ' . $key
          );
        }

      }
      // @todo Save submission Ids to webform_submissions.
      // @todo Use $upsert['mapper'] for that.
    }
  }
  if ($log != '') {
    watchdog(__FUNCTION__ . ' log', $log);
  }

  return print_r($log, TRUE);
}

<?php

/**
 * @file
 * Salespush campaings routines.
 */

/**
 * Helper function for sending BULK API campaigns data.
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
function _salespush_campaigns_pusher($environment, $wsdl, $username, $password, $demo = TRUE) {

  $client_data = salespush_get_client($username, $password, $wsdl);
  $client = $client_data['client'];
  $session_id = $client_data['session_id'];
  $location = $client_data['location'];

  watchdog('SF campaigns ' . $environment . ' session_id:', print_r($session_id, TRUE));
  watchdog('SF campaigns ' . $environment . ' location:', print_r($location, TRUE));

  // STEP 4. CREATE A NEW BATCH
  // prep the data. normally this would be loaded from a file,
  // but showing in plain text for demo purposes.
  $upsert = array();
  $mapper = array();
  if ($demo) {
    $campaigns[0] = new stdClass();
    $campaigns[0]->Name = 'DemoXXXCampaign';
    $campaigns[0]->Type = 'Demo';
    // If You want to upsert, not to create new one, uncomment this and fill with a real Id.
    // $campaigns[0]->Id = '701L000000075VVIAY';
  }
  else {
    $upsert = salespush_get_campaigns($environment);
    $campaigns = $upsert['campaigns'];
    $mapper = $upsert['mapper'];
  }

  if (count($campaigns) == 0) {
    return t('No new unsynced webforms');
  }

  try {
    $result = $client->upsert('Id', $campaigns, 'Campaign');
    watchdog('SF campaigns ' . $environment, 'Created campaigns: ' . count(array_keys($result)));
  }
  catch (Exception $e) {
    $result = 'Exception: ' . var_export($e, TRUE);
  }

  if (!is_array($result)) {
    return print_r($result, TRUE);
  }

  if (empty($result)) {
    return t('Something strange happened: result returned as empty array.');
  }

  // Saving webform nids to mapper table.
  foreach ($result as $key => $status) {
    $record = array(
      'type' => isset($mapper[$key]) && isset($mapper[$key]['type']) ? $mapper[$key]['type'] : 'event',
      'getter' => isset($mapper[$key]) && isset($mapper[$key]['nid']) ? $mapper[$key]['nid'] : 'demo',
      'source' => isset($mapper[$key]) && isset($mapper[$key]['nid']) ? 'nid' : 'demo',
      'destination' => $status->id
    );

    // Removing old mapped nodes from mapping table.
    $num_deleted = db_delete('salespush_mapper')
      ->condition('source', $record['source'])
      ->condition('environment', $environment)
      ->condition('getter', $record['getter'])
      ->execute();

    // drupal_write_record('salespush_mapper', $record);
    // See http://drupal.stackexchange.com/questions/11177/how-do-i-use-ignore-in-a-database-query
    db_merge('salespush_mapper')
      ->key(
        array(
          'destination' => $record['destination'],
          'source' => $record['source'],
          'environment' => $environment,
          'getter' => $record['getter'],
          'type' => $record['type']
        )
      )
      ->insertFields(
        array(
          'destination' => $record['destination'],
          'source' => $record['source'],
          'environment' => $environment,
          'getter' => $record['getter'],
          'type' => $record['type']
        )
      )
      ->execute();

    // Removing nodes marked as new from mapping table.
    $num_deleted = db_delete('salespush_mapper')
      ->condition('source', $record['source'])
      ->condition('destination', SALESPUSH_WEBFORM_MARKED_AS_NEW)
      ->condition('environment', $environment)
      ->condition('getter', $record['getter'])
      ->execute();
  }

  return print_r($result, TRUE);
}

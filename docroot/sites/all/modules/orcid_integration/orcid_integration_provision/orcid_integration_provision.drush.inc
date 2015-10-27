<?php

/**
 * @file orcid_integration_provision.drush.inc
 *   Drush integration for provisioning new accounts.
 */

/**
 * Implements hook_drush_help().
 */
function orcid_integration_provision_drush_help($section) {
  switch ($section) {
    case 'drush:orcid-integration-provision':
        return dt('This command provisions accounts based on a csv. Filepath required.');
  }
}

/**
 * Implements hook_drush_command().
 */
function orcid_integration_provision_drush_command() {
  $items = array();

  $items['orcid-integration-provision'] = array(
    'callback' => 'orcid_integration_provision_drush_provision_users',
    'description' => dt('Provision new/existing users from csv. Also create/claim orcid if one is not already attached.'),
    'arguments' => array(
      'filepath' => dt('Required. Provide absolute path to csv file. It should contain a header with the following columns (in any order): first_name,last_name,username,email'),
    ),
    'aliases' => array('oip'),
  );

  return $items;
}

/**
 * Provisions new users.
 */
function orcid_integration_provision_drush_provision_users($filepath = '') {
  if (!file_exists($filepath)) {
    drush_die(dt('Filepath %filepath does not exist', array('%filepath' => $filepath)));
  }

  $file = file_get_contents($filepath);
  $content = new parseCSV($file);
  $data = _orcid_integration_provision_sanitize_csv($content->data);

  $batch = array(
    'operations' => array(),
    'finished' => 'orcid_integration_provision_drush_provision_users_import_finished',
    'title' => t('Import users for ORCID'),
    'init_message' => t('Importing users for ORCID'),
    'progress_message' => t('Batch @current out of @total'),
    'error_message' => t('An error occurred processing a user.'),
  );
  $operations = array();
  foreach ($data as $data_entry) {
    $batch['operations'][] = array('orcid_integration_provision_import_user', array($data_entry));
  }
  batch_set($batch);
  $batch =& batch_get();
  $batch['progressive'] = FALSE;
  drush_backend_batch_process();
}

function orcid_integration_provision_drush_provision_users_import_finished($success, $results, $operations) {
  drupal_set_message(t('Users have been imported and ORCIDs claimed.'));
  drupal_set_message(t('Below list in format for csv of claiming pre-existing ORCIDs:'));
  $content = 'username,orcid';
  foreach ($results['orcid-results']['claimed'] as $result) {
    $content .= "\n{$result['username']},{$result['orcid']}";
  }
  drupal_set_message($content);
  drupal_set_message(t('Below list in format for csv of provisioned new ORCIDs:'));
  $content = 'username,orcid';
  foreach ($results['orcid-results']['provisioned'] as $result) {
    $content .= "\n{$result['username']},{$result['orcid']}";
  }
  drupal_set_message($content);
  drupal_set_message(t('Below list in format for csv of accounts with multiple ORCIDs:'));
  $content = 'username,email';
  foreach ($results['orcid-results']['conflict'] as $result) {
    $content .= "\n{$result['username']},{$result['email']},{$result['conflicts']}";
  }
  drupal_set_message($content);
}

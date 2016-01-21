<?php

/**
 * @file
 * Drush integration for Salesforce Webforms.
 */

/**
 * Implements hook_drush_command().
 */
function salesforce_webforms_drush_command() {
  $items['sf-webforms-map'] = array(
    'description' => 'Displays Salesforce mapping info for a webform',
    'aliases' => array('sfwfm'),
    'arguments' => array(
      'nid' => 'The node ID.',
    ),
    'required-arguments' => TRUE,
  );
  $items['sf-webforms-objects'] = array(
    'description' => 'Displays Salesforce data for objects mapped from a webform submission',
    'aliases' => array('sfwfo'),
    'arguments' => array(
      'submission' => 'The Submission ID.',
    ),
    'required-arguments' => TRUE,
    'options' => array(
      'fields' => 'Display information about fields that are part of the object.',
      'field-data' => 'Display information about a specific field that is part of an object',
    ),
  );
  $items['sf-webforms-resubmit'] = array(
    'description' => 'Resubmits submission data to Salesforce',
    'aliases' => array('sfwfr'),
    'arguments' => array(
      'submission' => 'The Submission ID.',
      'update-objects' => 'Update existing objects (1) or new objects (0)',
    ),
    'required-arguments' => 1,
  );
  return $items;
}

/**
 * Dump the salesforce mapping for a given node.
 */
function drush_salesforce_webforms_sf_webforms_map($nid) {
  $maps = salesforce_webforms_get_node_maps($nid);

  drush_print(drush_format($maps));
}

/**
 * Resubmits the submission data to Salesforce.
 */
function drush_salesforce_webforms_sf_webforms_resubmit($sid, $update = FALSE) {
  $salesforce = salesforce_get_api();
  if (!$salesforce->isAuthorized()) {
    drupal_print("Salesforce cannot connect");
    drupal_exit();
  }

  // Load the mapped IDs.
  $keys = salesforce_webforms_get_submission_keys($sid);

  // Find out which node this belongs to.
  $row = db_select('webform_submissions', 'w')
    ->fields('w', array('nid'))
    ->condition('sid', $sid)
    ->execute()
    ->fetch();

  $nid = $row->nid;
  $node = webform_menu_load($nid);
  $submission = webform_menu_submission_load($sid, $nid);

  salesforce_webforms_webform_submission_insert($node, $submission, $update);

  // Now dump the data.
  drush_salesforce_webforms_sf_webforms_objects($sid);
}

/**
 * Find all of the objects mapped to a given webform submission.
 *
 * @param int $submission
 *   The submission ID.
 */
function drush_salesforce_webforms_sf_webforms_objects($submission) {
  $salesforce = salesforce_get_api();
  if (!$salesforce->isAuthorized()) {
    drupal_print("Salesforce cannot connect");
    drupal_exit();
  }

  // Load the mapped IDs.
  $keys = salesforce_webforms_get_submission_keys($submission);

  // Find out which node this belongs to.
  $row = db_select('webform_submissions', 'w')
    ->fields('w', array('nid'))
    ->condition('sid', $submission)
    ->execute()
    ->fetch();

  $nid = $row->nid;

  // Now get the mappings for that node.
  $maps = salesforce_webforms_get_node_maps($nid);

  foreach ($maps as &$map) {
    foreach ($keys as $name => $key) {
      if ($name == $map['mapname']) {
        $map['sf_key'] = $key;
        try {
          $map['object_data'] = $salesforce->objectRead($map['sf_object'], $key);
        }
        catch (SalesforceException $e) {
          $map['object_data'] = "Salesforce error: " . $e->getMessage();
        }
      }
    }
  }

  // Now pull in the objects associated with the keys.
  drush_print(drush_format($maps));
}

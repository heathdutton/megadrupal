<?php

/**
 * @file
 * UI settings for the mvct module.
 */

/**
 * Form builder function for mvct settings.
 *
 * @return array
 *   The form array to be rendered.
 */
function mvct_settings() {

  $form = array();

  $types = mvct_get_content_types();

  if (!empty($types)) {
    $form['mvct_content_types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Select the content types that will be monitored'),
      '#options' => $types,
      '#default_value' => variable_get('mvct_content_types', array()),
    );
    return system_settings_form($form);
  }
  else {
    drupal_set_message(t("You currently have no active content types."));
  }
}

/**
 * Gets the bundles in the format "content type machine name" => "human name".
 *
 * @return array
 *   Returns an array containing all active content types from the database.
 */
function mvct_get_content_types() {
  $results = db_query("SELECT type, name from {node_type} WHERE disabled = 0")->fetchAll();
  $types_array = array();
  foreach ($results as $r) {
    $types_array[$r->type] = $r->name;
  }
  return $types_array;
}

/**
 * Helper function to get the record from the database.
 */
function mvct_get_record($nid) {
  $result = db_query("SELECT * from {mvct} WHERE nid = :nid", array(':nid' => $nid));
  return $result->fetch();
}

/**
 * Helper function to save the record to the database.
 */
function mvct_set_record(stdClass $record) {
  db_merge('mvct')
    ->key(array('nid' => $record->nid))
    ->fields((array) $record)
    ->execute();
}

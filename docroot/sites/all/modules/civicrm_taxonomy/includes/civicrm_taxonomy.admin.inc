<?php

/**
 * @file
 * Administration functions for the CiviCRM Taxonomy module.
 */

/**
 * Form constructor for the settings form.
 *
 * @see civicrm_taxonomy_menu().
 */
function civicrm_taxonomy_settings_form() {
  $form = array();

  // Format all child fieldsets as vertical tabs.
  $form['settings']['#type'] = 'vertical_tabs';

  // Add a fieldset to contain a checkbox for each vocabulary.
  $form['settings']['civicrm_taxonomy_settings'] = array(
    '#title' => t('Vocabularies'),
    '#description' => t('Select which vocabularies that should be syncronised with CiviCRM groups.'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );

  // Retrieve all vocabularies from the database.
  $vocabularies = taxonomy_get_vocabularies();

  // Get the default values, if any have already been defined.
  $defaults = variable_get('civicrm_taxonomy_settings', array());

  foreach ($vocabularies as $vocabulary) {
    // Add a checkbox.
    $form['settings']['civicrm_taxonomy_settings'][$vocabulary->vid] = array(
      '#title' => check_plain($vocabulary->name),
      '#type' => 'checkbox',
      '#default_value' => $defaults[$vocabulary->vid],
    );
  }

  $form['#submit'][] = 'civicrm_taxonomy_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Form submission handler for civicrm_taxonomy_settings_form().
 *
 * @see civicrm_taxonomy_settings_form()
 */
function civicrm_taxonomy_settings_form_submit(&$form, &$form_state) {
  $old_values = variable_get('civicrm_taxonomy_settings', array());
  $values = $form_state['values']['civicrm_taxonomy_settings'];

  module_load_include('inc', 'civicrm_taxonomy', 'includes/core');

  foreach ($values as $vid => $new_value) {
    $old_value = $old_values[$vid];

    if ($old_value == 0 && $new_value == 1) {
      // Vocabulary has been enabled.
      civicrm_taxonomy_enable($vid);
    }
    elseif ($old_value == 1 && $new_value == 0) {
      // Vocabulary has been disabled.
      civicrm_taxonomy_disable($vid);
    }
  }
}

/**
 * Create or enable a new CiviCRM hierarchy based on a vocabulary.
 */
function civicrm_taxonomy_enable($vid) {
  module_load_include('inc', 'civicrm_taxonomy', 'includes/core');

  $ctg = civicrm_taxonomy_get($vid);
  $vocabulary = taxonomy_vocabulary_load($vid);
  $name = $vocabulary->name;
  $description = ($vocabulary->description) ? $vocabulary->description : NULL;

  if ($ctg['vid']) {
    $gid = $ctg['gid'];
    civicrm_taxonomy_group_write($name, $description, NULL, $gid);
  }
  else {
    $gid = civicrm_taxonomy_group_write($name, $description, NULL);
    $record = array('vid' => $vid, 'tid' => NULL, 'gid' => $gid->id);
    drupal_write_record('civicrm_taxonomy', $record);
  }

  // For each term in this vocabulary, create a CiviCRM Group if necessary
  civicrm_taxonomy_enable_tree($vid);
}

/**
 * Disable a CiviCRM hierarchy based on a vocabulary.
 */
function civicrm_taxonomy_disable($vid) {
  $result = db_query("SELECT vid, tid FROM {civicrm_taxonomy} WHERE vid = :vid", array(':vid' => $vid))->fetchAll();
  foreach ($result as $row) {
    civicrm_taxonomy_link_delete($row->vid, $row->tid, TRUE);
  }
}

/**
 * Recursive helper function.
 */
function civicrm_taxonomy_enable_tree($vid, $parent = 0) {
  $tree = taxonomy_get_tree($vid, $parent, 1, TRUE);
  $parent_link = civicrm_taxonomy_get($vid, $parent, TRUE);
  $results = array();

  foreach ($tree as $term) {
    civicrm_taxonomy_link_write($term);
    civicrm_taxonomy_enable_tree($vid, $term->tid);
  }
}

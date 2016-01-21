<?php

/**
 * @file
 * Drush integration for the flat_taxonomy module.
 */

/**
 * Implements hook_drush_command().
 */
function flat_taxonomy_drush_command() {
  $items['flat-taxonomy'] = array(
    'callback' => 'drush_flat_taxonomy',
    'description' => dt('Flatten a taxonomy.'),
    'arguments' => array(
      'machine_name' => dt('Vocabulary machine name which need to be flatten.'),
    ),
    'aliases' => array('ft'),
  );

  return $items;
}

/**
 * Command callback. Flatten a vocabulary.
 */
function drush_flat_taxonomy($vname = NULL) {
  // Try to convert machine name to a vocab ID
  if (!$vocabulary = taxonomy_vocabulary_machine_name_load($vname)) {
    return drush_set_error('FLAT_TAXONOMY_INVALID_INPUT', dt('Invalid vocabulary machine name: !vname', array('!vname' => $vname)));
  }

  _flat_taxonomy_flatten_vocabulary($vocabulary->vid);
  drush_log(dt('!name vocabulary is now flat', array('!name' => $vocabulary->name)), 'success');
}

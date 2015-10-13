<?php
/**
 * @file
 * Drush commands for Taxonomy Nested Intervals
 */

/**
 * Implements hook_drush_command().
 */
function taxonomy_nested_intervals_drush_command() {
  $items = array();

  $items['rebuild-intervals'] = array(
    'description' => "Rebuild tree.",
    'arguments' => array(
      'type' => 'Name of the vocabulary (e.g. tags)',
    ),
    'options' => array(
      'safe' => 'Use "safe" sibling enumeration',
    ),
    'examples' => array(
      'drush rebuild-intervals tags --safe' => 'Rebuild of vocabuary "tags" using safe sibling enumeration',
    ),
  );

  $items['rebuild-values'] = array(
    'description' => "Rebuild left/right values for a tree.",
    'arguments' => array(
      'type' => 'Name of the vocabulary (e.g. tags)',
    ),
    'examples' => array(
      'drush rebuild-values tags',
    ),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function taxonomy_nested_intervals_drush_help($section) {
  switch ($section) {
    case 'drush:rebuild-intervals':
      return dt("This command will rebuild the nested intervals for a vocabulary.");
  }
}

/**
 * Get vocabulary ID by name.
 *
 * @param string $name
 *   Machine name of vocabulary.
 *
 * @return int
 *   Vocabulary ID.
 */
function _drush_taxonomy_nested_intervals_get_vid($name) {
  $vocabulary = taxonomy_vocabulary_machine_name_load($name);
  if (!$vocabulary) {
    return drush_set_error(dt('Vocabulary: !name not found!.', array('!name' => $name)));
  }
  $vid = $vocabulary->vid;


  if (!taxonomy_nested_intervals_get_option($vid, 'enabled')) {
    return drush_set_error(dt('Nested intervals not enabled for vocabulary: !name.', array('!name' => $name)));
  }
  return $vid;
}

/**
 * Rebuild intervals.
 */
function drush_taxonomy_nested_intervals_rebuild_intervals($name) {
  $vid = _drush_taxonomy_nested_intervals_get_vid($name);
  if (!$vid) {
    return $vid;
  }

  module_load_include('rebuild.inc', 'taxonomy_nested_intervals');

  taxonomy_nested_intervals_rebuild_batch(array($vid), drush_get_option('safe'));
  $batch = &batch_get();
  $batch['progressive'] = FALSE;
  drush_backend_batch_process();
}

/**
 * Rebuild values.
 */
function drush_taxonomy_nested_intervals_rebuild_values($name) {
  $vid = _drush_taxonomy_nested_intervals_get_vid($name);
  if (!$vid) {
    return $vid;
  }

  module_load_include('rebuild.inc', 'taxonomy_nested_intervals');

  taxonomy_nested_intervals_rebuild_left_right_batch(array($vid), drush_get_option('safe'));
  $batch = &batch_get();
  $batch['progressive'] = FALSE;
  drush_backend_batch_process();
}

<?php
/**
 * @file
 * Drush commands for Drush 311.
 *
 * * drush 311 - Find matching drush 311 entries by keyword.
 * * drush 311-add - Add a new drush 311 entry.
 */

/**
 * Implement hook_drush_help().
 */
function drush_311_drush_help($section) {
  switch ($section) {
    case 'drush:311':
      return dt('Find matching Drush 311 entries by keyword.');
    case 'drush:311-add':
      return dt('Add a new Drush 311 entry with name, keywords and information.');
  }
}

/**
 * Implement hook_drush_command().
 */
function drush_311_drush_command() {
  $items = array();

  $items['311'] = array(
    'callback' => 'drush_311_get_entry',
    'description' => 'Find matching Drush 311 entries by keyword.',
    'arguments' => array(
      'keywords' => '',
    ),
  );
  $items['311-add'] = array(
    'callback' => 'drush_311_add_entry',
    'description' => 'Add a new Drush 311 entry with name, keywords and information.',
    'arguments' => array(
      'name' => '',
      'keywords' => '',
      'information' => '',
    ),
  );

  return $items;
}

function drush_311_get_entry($keyword) {
  $result = db_query("SELECT * FROM {drush_311} WHERE keywords LIKE :keyword", array(':keyword' => '%' . $keyword . '%'));
  foreach ($result as $entry) {
    drush_print($entry->name . ': ' . $entry->information);
  }
}

function drush_311_add_entry() {
  if (!($args = func_get_args())) {
    $args = array();
  }
  if (!count($args)) {
    $name = drush_prompt('Name for your new 311 entry');
  }
  else {
    $name = $args[0];
  }
  if (count($args) < 2) {
    $keywords = drush_prompt('Keywords to activate your new 311 entry (space separated)');
  }
  else {
    $keywords = $args[1];
  }
  if (count($args) < 3) {
    $information = drush_prompt('Information to display for your new 311 entry');
  }
  else {
    $information = $args[2];
  }
  $entry = array(
    'name' => $name,
    'keywords' => $keywords,
    'information' => $information,
  );
  drupal_write_record('drush_311', $entry);
  drush_print(dt('The Drush 311 entry !name: !information was added with keywords !keywords.', array('!name' => $name, '!information' => $information, '!keywords' => $keywords)));
}

<?php
/**
 * @file
 * Drush command to get latest schema version for a module.
 */

/**
 * Implements hook_drush_command().
 */
function schema_version_drush_command() {
  $items = array();

  $items['schema-version'] = array(
    'description' => 'Show latest schema version.',
    'arguments' => array(
      'module' => 'Module name.',
    ),
    'examples' => array(
      'drush sv views' => 'Show latest schema version of the views module.',
    ),
    'aliases' => array('sv'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
  );

  $items['set-schema-version'] = array(
    'description' => 'Set schema version.',
    'arguments' => array(
      'module' => 'Module name.',
      'version' => 'Version(e.g. 7000).',
    ),
    'examples' => array(
      'drush ssv views 7000' => 'Set schema version of the views module.',
    ),
    'aliases' => array('ssv'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function schema_version_drush_help($section) {
  switch ($section) {
    case 'drush:schema-version':
      return dt('Show latest schema version.');
    case 'drush:set-schema-version':
      return dt('Set schema version.');
  }
}

/**
 * Get schema version validate callback.
 */
function drush_schema_version_validate($module = '') {
  if (empty($module)) {
    return drush_set_error('Please enter module name.');
  }
}

/**
 * Set schema version validate callback.
 */
function drush_schema_version_set_schema_version_validate($module = '', $version = '') {
  drush_schema_version_validate($module);
  if (empty($version)) {
    return drush_set_error('Please specify schema version.');
  }

  if (!is_numeric($version) || strlen($version) != 4) {
    return drush_set_error('Please enter numeric value with 4 digit for schema version.');
  }

  $module_object = db_query("SELECT * FROM {system} WHERE name = :name", array(':name' => $module))->fetchObject();
  if (!isset($module_object->status)) {
    return drush_set_error('Module is not installed.');
  }
}

/**
 * Get schema version command callback.
 */
function drush_schema_version($module = '') {
  $module_object = db_query("SELECT * FROM {system} WHERE name = :name", array(':name' => $module))->fetchObject();
  $items[] = array(
    dt('Name'),
    dt('Version'),
    dt('Status'),
    dt('Weight'),
    dt('Bootstrap'),
  );

  $status = dt('Not installed');
  if (!empty($module_object->status)) {
    if ($module_object->status) {
      $status = dt('Enabled');
    }
    elseif (!$module_object->status && $module_object->schema_version > 0) {
      $status = dt('Disabled');
    }
  }

  $items[] = array(
    !empty($module_object->name) ? $module_object->name : $module,
    !empty($module_object->schema_version) ? $module_object->schema_version : dt('-None-'),
    $status,
    !empty($module_object->weight) ? $module_object->weight : dt('-None-'),
    !empty($module_object->bootstrap) ? $module_object->bootstrap : dt('-None-'),
  );
  drush_print_table($items, TRUE);
}

/**
 * Set schema version command callback.
 */
function drush_schema_version_set_schema_version($module = '', $version = '') {
  $query_args = array(':schema_version' => $version, ':name' => $module);
  $print_args = array('!schema_version' => $version, '!name' => $module);
  if (db_query("UPDATE {system} SET schema_version = :schema_version WHERE name = :name", $query_args)) {
    drush_print(dt('Schema version !schema_version was successfully updated for module !name.', $print_args));
  }
}

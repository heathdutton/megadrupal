<?php

/**
 * @file
 * Drush hook implementations for the Entity Scaffold module.
 */

define('ENTITY_SCAFFOLD_MODEL_MODULE_PATH', dirname(__FILE__) . '/model/');
define('ENTITY_SCAFFOLD_MODEL_FILE_EXTENSION', '.model');
define('ENTITY_SCAFFOLD_LOG_SUCCESS', 'ok');
define('ENTITY_SCAFFOLD_LOG_ERROR', 'error');
define('ENTITY_SCAFFOLD_MODULE_EXTENSION', 'module');

// Always include helper functions file.
require_once dirname(__FILE__) . '/entity_scaffold.helper.inc';

/**
 * Implements hook_drush_command().
 */
function entity_scaffold_drush_command() {
  $items['entity-scaffold'] = array(
    'description' => 'Command to generate a new basic custom entity.',
    'allow-additional-options' => TRUE,
    'aliases' => array('es'),
    'arguments' => array(
      'module_name' => 'The name of the entity/module that will be generated containing the entity implementation.',
      'module_destination' => 'The location where the generated module will be placed.',
    ),
    'examples' => array(
      'drush entity-scaffold my_entity /home/user/modules' => 'Generates a new module on the given location.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_NONE,
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function entity_scaffold_drush_help($section) {
  switch ($section) {
    case 'drush:entity-scaffold':
      return dt('Generates a module containing a basic structure of a custom entity.');
  }
}

/**
 * Drush command entity-scaffold Callback.
 */
function drush_entity_scaffold($module_name, $module_destination) {
  entity_scaffold_generate($module_name, $module_destination);
}

/**
 * Validate handler for entity-scaffold command.
 */
function drush_entity_scaffold_validate($module_name = '', $module_destination = '') {
  // Validates whether no parameters or there is any empty parameter.
  $args = func_get_args();
  if (count($args) != 2) {
    return drush_log(dt('Empty parameters are not allowed'), ENTITY_SCAFFOLD_LOG_ERROR);
  }

  // Validates module_name.
  if (!entity_scaffold_is_valid_module_name($module_name)) {
    return drush_log(dt('Invalid name given to module name parameter. Uses only: "a-z 0-9" characters and "_" separator.'), ENTITY_SCAFFOLD_LOG_ERROR);
  }
}

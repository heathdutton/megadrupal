<?php
/**
 * @file
 * Drush integration for the factorydrone module
 */

/**
 * Implments hook_drush_command().
 */
function factorydrone_drush_command() {
  $items = array();

  $items['factorydrone-create'] = array(
    'description' => dt('Create and persist a new factory object.'),
    'arguments' => array(
      'name' => dt('The name of the factory on which to base the new object.'),
      'key=value...' => dt('Any additional options you want to pass to the factory for overriding defaults.'),
    ),
    'options' => array(
      'porcelain' => dt('Print output in format intended for script consumption.'),
      'factory_path' => array(
        'description' => dt('Path to load factories from (multiple values allowed)'),
        'value' => 'required',
      ),
    ),
    'required-arguments' => 1,
    'aliases' => array('fd-create', 'pdc'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
  );

  return $items;
}

/**
 *  Command callback. Create a new object.
 */
function drush_factorydrone_create($name = NULL) {
  module_load_include('inc', 'factorydrone', 'lib/FactoryDrone');

  if ($factory_path = drush_get_option('factory_path')) {
    FactoryDrone::addFactoryPath($factory_path);
    FactoryDrone::loadDefinitions();
  }

  $object = call_user_func_array('_drush_factorydrone_create', func_get_args());

  if (drush_get_option('porcelain')) {
    drush_print(serialize($object));
  }
  else {
    drush_print("Created new object.");
    drush_print(print_r($object, TRUE));
  }
}

/**
 * Function for creating an object.
 *
 * This function is passed arbitrary additional arguments in the form key=value,
 * which are passed in by drush at command invocation
 *
 * @param string $name
 *   The name of the factory template to use
 *
 * @return object
 *   The newly created object
 */
function _drush_factorydrone_create($name = NULL) {
  $args = func_get_args();
  $factory_options = array();

  if ($args) {
    $name = array_shift($args);

    foreach ($args as $arg) {
      list($key, $value) = explode('=', $arg);
      $factory_options[$key] = $value;
    }
  }

  return FactoryDrone::create($name, $factory_options);
}

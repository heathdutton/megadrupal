<?php

/**
 * @file
 * Provide information about available blueprints.
 */

/**
 * Print a list of blueprints and the information they provide.
 */
function drush_vagrant_blueprints() {
  $blueprints = drush_vagrant_blueprint_info();
  $args = drush_get_context('DRUSH_COMMAND_ARGS');

  if (in_array('--all', $args)) {
    foreach ($blueprints as $machine_name => $info) {
      foreach ($info as $label => $value) {
        drush_print('   ' . $label . ': ' . $value);
      }
      drush_print('  -------------------------');
    }
  }
  else {
    $rows = array();
    $rows[] = array('NAME', 'MACHINENAME', 'EXTENSION', 'DESCRIPTION');
    foreach ($blueprints as $machine_name => $info) {
      $rows[] = array($info['name'], $machine_name, $info['extension'], $info['description']);
    }
    drush_print_table($rows, TRUE);
  }
}

/**
 * Poll all Drush extensions implementing hook_vagrant_blueprints().
 *
 * @return
 *   Returns an array of blueprints where the key is the name, and the value is
 *   an array of information relevant to the blueprint.
 *
 * See also: docs/drush-vagrant.api.php
 */
function drush_vagrant_blueprint_info() {
  $extensions = drush_vagrant_extension_info();
  $blueprints =  drush_command_invoke_all('vagrant_blueprints');
  foreach ($extensions as $name => $info) {
    foreach ($info['blueprints'] as $key => $value) {
      $blueprints[$value]['extension'] = $name;
      $blueprints[$value]['full_path'] = $info['full_path'] . '/' . $blueprints[$value]['path'];
    }
  }
  // Ensure 'default' is the first blueprint
  $default = $blueprints['default'];
  unset($blueprints['default']);
  krsort($blueprints);
  $blueprints['default'] = $default;
  return array_reverse($blueprints);
}

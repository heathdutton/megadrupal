<?php

/**
 * @file
 * The Drush Vagrant API.
 */

/**
 * Register this extension's blueprints with Drush Vagrant
 *
 * @return
 *   Return an array of blueprints, where the key is the blueprint name and the
 *   value is an extensible list of information relating to the blueprint.
 */
function hook_vagrant_blueprints() {
  $blueprints = array(
    'first' => array(
      'name' => 'First',
      'description' => 'The first blueprint.',
      'path' => 'blueprints/first', // req'd
      'build_callback' => 'my_custom_build_function', // optional
      '...' => '...',   // Arbitrary data can be passed to the build process
                        // & saved in .config/blueprint.inc
    ),
    'another' => array(
      'name' => 'Second',
      'description' => 'Another blueprint.',
      'path' => 'blueprints/another', // req'd
      ),
    );
  return $blueprints;
}

<?php

/**
 * Implements hook_drush_command().
 */
function color_rebuilder_drush_command() {

  $items['color-rebuild'] = array(
    'description' => 'Demonstrate how Drush commands work.',
    'aliases' => array('creb'),
    'options' => array(
      'theme' => 'The name of the theme to rebuild the color files.',
      'color' => 'The main color to set.',
    ),
  );

  return $items;
}

/**
 * Callback for the drush-demo-command command
 */
function drush_color_rebuilder_color_rebuild() {
  color_rebuilder_drush_color_rebuild();
}


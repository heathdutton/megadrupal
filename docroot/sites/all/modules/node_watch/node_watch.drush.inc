<?php

/**
 * @file
 * Node watch module drush integration.
 */

/**
 * Implements hook_drush_command().
 */
function node_watch_drush_command() {
  $items['nodewatch'] = array(
    'description' => 'Run the nodewatch command.',
    'aliases' => array('nw'),
  );
  return $items;
}

/**
 * Callback for the drush-nodewatch command.
 */
function drush_node_watch_nodewatch() {
  $screen = node_watch_create();
  drush_print('Start Node Watch Report Array');
  foreach ($screen as $line) {
    $line = str_replace('</br>', PHP_EOL, $line);
    drush_print($line);
  }
  drush_print('End Node Watch Report Array');
}

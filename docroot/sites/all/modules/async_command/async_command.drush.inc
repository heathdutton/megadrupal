<?php

/**
 * @file
 *   Async Command drush interface.
 *   To use this, please install Drush at http://drupal.org/project/drush
 */

/**
 * Implements hook_drush_command().
 */
function async_command_drush_command() {
  $items = array();
  $items['async-command'] = array(
    'description' => "Submit an async command directly from Drush",
    'aliases' => array('ac'),
    'arguments' => array(
      'app'         => 'App name of this command.',
      'command'     => 'Please write down the command. Remember to enclose spaces in double quotes.',
      'description' => 'Description of this command. Enclose spaces in double quotes.',
      'options'     => 'Vertical bar separated key-value pairs. Eg. option1=value1|option2=value2'
    ),

    //'callback' => 'async_command_create_command',
    //'callback arguments' => array('default'),
    
    'examples' => array(
      'drush async-command your_app your_command your_description "option1=value1|option2=value2"' => "Create the 'your_command' command. Remember to enclose spaces into double quotes."
    ),
    
    'bootstrap' => DRUSH_BOOTSTRAP_MAX,
  );
  $items['async-command-purge'] = array(
    'description' => 'Remove all commands where status is not null.',
    'options' => array(
        'force' => 'Controls whether to remove all commands or finished ones.'
      ),
    'examples' => array(
      'drush async-command-purge' => 'Purge all finished commands.',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function async_command_drush_help($section) {
  switch ($section) {
    case 'drush:async-command':
      return dt("Create async command directly from Drush");
    case 'drush:async-command-purge':
      return dt('Remove all commands where status is not null.');
  }
}


function drush_async_command($app, $command, $description = NULL, $options = NULL) {
  if ($options != NULL) {
    // TODO: this might have escape characters issue.
    $pairs = explode('|', $options);
    $options = array();
    foreach ($pairs as $pair) {
      $key_value = explode('=', $pair);
      $options[$key_value[0]] = $key_value[1];
    }
  }
  //var_dump($options);
  $id = async_command_create_command($app, $command, $description, $options);
  drush_print("Created command with ID: ". $id);
  //var_dump(async_command_load_command($id));
}

function drush_async_command_purge() {
  $force = drush_get_option('force', NULL);
  if (!$force) {
    db_query("DELETE FROM {async_command} WHERE status IS NOT NULL");
  }
  else {
    db_query("DELETE FROM {async_command}");
  }
  drush_print("Purge {async_command} table successful.");
}
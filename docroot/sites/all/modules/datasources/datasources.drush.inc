<?php

/**
 * Implementation of hook_drush_command().
 */
function datasources_drush_command() {
  $items = array();

  $items['datasources-import'] = array(
    'description' => "Do the import.",
    'arguments' => array(
      'input_id' => 'datasource_input_id',
    ),
    'options' => array(
      'max' => 'Max number of items to import. If 0 loop until no more items. Default is 50.',
    ),
    'examples' => array(
      'drush dimp my_datasource' => 'Import the datasource my_datasource.',
    ),
    'aliases' => array('dimp'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );


  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function datasources_drush_help($section) {
  switch ($section) {
    case 'drush:datasources-import':
      return dt("This command will import the given datasource.");
    case 'meta:datasources:title':
      return dt("Datasources commands");
    case 'meta:datasources:summary':
      return dt("Help you import datasources.");
  }
}

/**
 * Implementation of drush_hook_COMMAND().
 */
function drush_datasources_import($input_id) {
  global $user;
  if ($account = user_load(1)) {
    $user = $account;
  }
  datasources_import($input_id, drush_get_option('max', 50));
}

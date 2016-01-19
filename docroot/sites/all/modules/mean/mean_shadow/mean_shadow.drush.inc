<?php

/**
 * Implements hook_drush_help().
 */
function mean_shadow_drush_help($command) {
  switch ($command) {
    case 'drush:shadow-users':
      return dt('Shadow all the users over to mongodb.');
  }
}

/**
 * Implements hook_drush_command().
 */
function mean_shadow_drush_command() {
  $items = array();
  $items['shadow-entities'] = array(
    'description' => dt('Shadow all the entities over to the MEAN stack.'),
    'arguments'   => array(
      'entity type' => dt('An optional type to shadow, user (default) or node'),
      'bundle type' => dt('An optional bundle (node type)'),
      'id' => dt('An optional id to shadow'),
    ),
    'examples' => array(
      'Standard example (all the user entities)' => 'drush shadow-entities',
      'Type example (all the node entities)' => 'drush shadow-entities node',
      'Id example (specific user)' => 'drush shadow-entities user 42',
      'Id example 2 (specific node)' => 'drush shadow-entities node 42',
      'Bundle example (all the entites of bundle article)' => 'drush shadow-entities node article'
    ),
    'aliases' => array('shanties'),
  );

  return $items;
}

/**
 * Callback function for drush my-command.
 * Callback is called by using drush_hook_command() where
 * hook is the name of the module (MYMODULE) and command is the name of
 * the Drush command with all "-" characters converted to "_" characters (my_command)
 *
 * @param string $type
 *   An optional type of the entity to shadow, defaults to 'user'
 *
 * @param $id
 *   An optional id of the entity to shadow, defaults to NULL
 * @param null $bundle
 */
function drush_mean_shadow_shadow_entities($type = 'user', $id = NULL, $bundle = NULL) {
  drush_log('Running shadow-entities', 'ok');

  if (isset($id) && is_numeric($id)) {
    $entity = _drush_mean_shadow_get_entity($type, $id);
    if (!empty($entity)) {
      _drush_mean_shadow_shadow_request($type, $entity);
    }
  }
  else {
    // Making sense of the variables
    if (isset($id) && is_string($id)) {
      $bundle = check_plain($id);
      $id = NULL;
    }

    $types = node_type_get_types();
    $machine_types = array_keys($types);
    if (!empty($bundle) && !in_array($bundle, $machine_types)) {
      drush_log('Unknown BUNDLE argument', 'error');
      return;
    }

    $result = _drush_mean_shadow_gen_query($type, $bundle);

    if (!empty($result)) {
      foreach($result as $row) {
        $entity = _drush_mean_shadow_get_entity($type, $row->id);
        if (!empty($entity)) {
          _drush_mean_shadow_shadow_request($type, $entity);
        }
      }
    }
    else {
      drush_log('Unknown type', 'error');
    }
  }

  // Calling the map reduce to index all
  mean_shadow_invoke_all_drush($type, $id, $bundle);


  drush_log('Finished shadow-entities', 'ok');
}

/**
 * @param $type
 * @param $id
 * @return null
 */
function _drush_mean_shadow_get_entity($type, $id) {
  $entity = null;

  switch ($type) {
    case 'user':
      $entity = user_load($id);
      break;

    case 'node':
      $entity = node_load($id);
      break;
  }

  if (!empty($entity)) {
    drush_log('Got entity ' . $id, 'ok');
  }
  else {
    drush_log('Could not shadow entity ' . $id, 'warning');
  }

  return $entity;
}

/**
 * @param $type
 * @param $bundle
 * @return null
 */
function _drush_mean_shadow_gen_query($type, $bundle) {
  $q = null;

  switch ($type) {
    case 'user':
      $q = db_query('SELECT uid as id FROM {users};');
      break;

    case 'node':
      $q_str = 'SELECT nid as id FROM {node}';
      if (!empty($bundle)) {
        $q = db_query($q_str . ' WHERE type = "%s";', $bundle);
      }
      else {
        $q = db_query($q_str . ';');
      }

      break;
  }

  return $q;
}

/**
 * @param $type
 * @param $entity
 */
function _drush_mean_shadow_shadow_request($type, $entity) {
  $prop_name = '';
  switch ($type) {
    case 'user':
      $prop_name = 'uid';
      break;

    case 'node':
      $prop_name = 'nid';
      break;
  }

  mean_shadow_request('insert', $type, $prop_name, $entity);

  drush_log('Shadowed entity ' . $entity->{$prop_name}, 'ok');
}

<?php

/**
 * Implements hook_drush_command().
 */
function migrate_inspect_drush_command() {
  $commands = array();

  $commands['migrate-inspect-last'] = array(
    'callback' => 'migrate_inspect_last',
    'handle-remote-commands' => TRUE,
    'description' => 'Show information about the last imported item from a migration',
    'aliases' => array('mil'),
    'examples' => array(
      'drush migrate-inspect-last [migration]' => 'Open the last migrated item from a migration',
    ),
    'arguments' => array(
      'migration' => 'The migation machine name',
    ),
    'options' => array(
      'browser' => 'Optional value denotes which browser to use (defaults to operating system default). Set to 0 to suppress opening a browser.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_NONE,
  );

  $commands['migrate-inspect-random'] = array(
    'callback' => 'migrate_inspect_random',
    'handle-remote-commands' => TRUE,
    'description' => 'Show information about an imported item, picked at random, from a migration',
    'aliases' => array('mir'),
    'examples' => array(
      'drush migrate-inspect-random [migration]' => 'Open a random migrated item from a migration',
    ),
    'arguments' => array(
      'migration' => 'The migation machine name',
    ),
    'options' => array(
      'browser' => 'Optional value denotes which browser to use (defaults to operating system default). Set to 0 to suppress opening a browser.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_NONE,
  );

  $commands['migrate-inspect-list'] = array(
    'callback' => 'migrate_inspect_list',
    'description' => 'Show last items in the migrate map for a migration',
    'aliases' => array('mils'),
    'examples' => array(
      'drush migrate-inspect-list [migration]' => 'List the last items from the migration map',
    ),
    'arguments' => array(
      'migration' => 'The migation machine name',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  $commands['migrate-inspect-source'] = array(
    'callback' => 'migrate_inspect_source',
    'description' => 'Show the source id for a destination id',
    'aliases' => array('mis'),
    'examples' => array(
      'drush migrate-inspect-source [migration] [id]' => 'List the source id for a destination id',
    ),
    'arguments' => array(
      'migration' => 'The migation machine name',
      'id' => 'The destination ID',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  $commands['migrate-inspect-destination'] = array(
    'callback' => 'migrate_inspect_destination',
    'description' => 'Show destination id for a source id',
    'aliases' => array('mid'),
    'examples' => array(
      'drush migrate-inspect-destintation [migration] [id]' => 'List the destination id for a source id',
    ),
    'arguments' => array(
      'migration' => 'The migation machine name',
      'id' => 'The source ID',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  $commands['migrate-inspect-find-source'] = array(
    'callback' => 'migrate_inspect_find_source',
    'description' => 'Show destination ID and migration for a source id',
    'aliases' => array('mifs'),
    'examples' => array(
      'drush migrate-inspect-find-source [id]' => 'List the destination id and migration for a source id',
    ),
    'arguments' => array(
      'id' => 'The source ID',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  $commands['migrate-inspect-find-destination'] = array(
    'callback' => 'migrate_inspect_find_destination',
    'description' => 'Show destination id for a source id',
    'aliases' => array('mifd'),
    'examples' => array(
      'drush migrate-inspect-find-destintation [id]' => 'List the source id and migration for a destination id',
    ),
    'arguments' => array(
      'id' => 'The destination ID',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  return $commands;
}

/**
 * Get the source id for a migrated item
 */
function migrate_inspect_destination($migration, $id, $return = FALSE) {
  $migrations = migrate_migrations();
  if (!isset($migrations[$migration])) {
    drush_log(t('Migration !migration does not exist', array('!migration' => $migration)), 'error');
    return 1;
  }

  // get the map from the migration table
  $table = $migrations[$migration]->getMap()->getMapTable();
  $exists = db_query('SHOW TABLES LIKE :table', array(':table' => $table));
  if ($exists->rowCount() === 1) {
    $destination_id = db_query('SELECT destid1 FROM ' . $table . ' WHERE sourceid1=:id', array(':id' => $id))->fetchColumn();
    if ($destination_id) {
      if ($return) {
        return $destination_id;
      }
      return drush_log(t('Destination ID: !id', array('!id' => $destination_id)), 'ok');
    }
  }

  if ($return) {
    return FALSE;
  }
  return drush_log(dt('No migration record found for the source ID !id', array('!id' => $id)), 'error');
}

/**
 * Find the destination ID for a known source ID.
 *
 * Similar to migrate_inspect_destination() except you don't need to know the
 * migration name.
 */
function migrate_inspect_find_destination($id) {
  $migrations = migrate_migrations();
  foreach (array_keys($migrations) as $migration) {
    $destination = migrate_inspect_destination($migration, $id, TRUE);
    if ($destination) {
      return drush_log(t('Destination ID: !id; Migration: !migration', array('!id' => $destination, '!migration' => $migration)), 'ok');
    }
  }

  return drush_log(dt('No migration record found for the source ID !id', array('!id' => $id)), 'error');
}

/**
 * Find the source ID for a known destination ID.
 *
 * Similar to migrate_inspect_source() except you don't need to know the
 * migration name.
 */
function migrate_inspect_find_source($id) {
  $migrations = migrate_migrations();
  foreach (array_keys($migrations) as $migration) {
    $source = migrate_inspect_destination($migration, $id, TRUE);
    if ($source) {
      return drush_log(t('Source ID: !id; Migration: !migration', array('!id' => $source, '!migration' => $migration)), 'ok');
    }
  }

  return drush_log(dt('No migration record found for the destination ID !id', array('!id' => $id)), 'error');
}

/**
 * Show the last item migrated
 */
function migrate_inspect_last($migration) {
  $args = func_get_args();
  $alias = drush_get_context('DRUSH_TARGET_SITE_ALIAS');
  if (drush_sitealias_is_remote_site($alias)) {
    $return = drush_invoke_process($alias, 'migrate-inspect-last', $args, drush_redispatch_get_options(), array('integrate' => FALSE));
    if ($return['error_status']) {
      return drush_set_error('Unable to execute user login.');
    }
    else {
      // Prior versions of Drupal returned a string so cast to an array if
      // needed.
      $link = $return['object'];
    }
  }
  else {

    if (!drush_bootstrap(DRUSH_BOOTSTRAP_DRUPAL_FULL)) {
      // Fail gracefully if unable to bootstrap Drupal.
      // drush_bootstrap() has already logged an error.
      return FALSE;
    }

    global $base_url;

    $migrations = migrate_migrations();
    if (!isset($migrations[$migration])) {
      drush_log(t('Migration !migration does not exist', array('!migration' => $migration)), 'error');
      return 1;
    }

    // get the map from the migration table
    $table = $migrations[$migration]->getMap()->getMapTable();
    $exists = db_query('SHOW TABLES LIKE :table', array(':table' => $table));
    if ($exists->rowCount() === 1) {
      // count
      $count_query = db_select($table);
      $count_query->addExpression('COUNT(*)', 'cnt');
      $count = $count_query->execute();
      $count = $count->fetchColumn();

      // get the last ID in the table
      $last = db_select($table, 'm');
      $last->fields('m', array('destid1'));
      $last->range($count - 1, 1);
      $results = $last->execute();

      // we should probably run a count query and then a range query to get the
      // last one instead of this...
      $last_result = $results->fetchAssoc();

      if ($last_result) {
        $destination = $migrations[$migration]->getDestination();
        $entity_type = $destination->getEntityType();

        $load_function = $entity_type . '_load';
        if (function_exists($load_function)) {
          $entity = call_user_func($load_function, $last_result['destid1']);
        }
        else {
          $entity = entity_load($entity_type, array($last_result['destid1']));
        }

        if ($entity) {
          $function = $entity_type . '_uri';
          if (function_exists($function)) {
            $uri = call_user_func($function, $entity);
          }
          else {
            $uri = entity_uri($entity_type, $entity);
          }
          $link = $base_url . '/' . $uri['path'];
        }
      }
    }
  }

  drush_start_browser($link);
  drush_backend_set_result($link);
  return $link;
}

/**
 * Show the last item migrated
 */
function migrate_inspect_list($migration) {
  $migrations = migrate_migrations();
  if (!isset($migrations[$migration])) {
    drush_log(t('Migration !migration does not exist', array('!migration' => $migration)), 'error');
    return 1;
  }

  // get the map from the migration table
  $table = $migrations[$migration]->getMap()->getMapTable();
  $exists = db_query('SHOW TABLES LIKE :table', array(':table' => $table));
  if ($exists->rowCount() === 1) {
    // count
    $count_query = db_select($table);
    $count_query->addExpression('COUNT(*)', 'cnt');
    $count = $count_query->execute();
    $count = $count->fetchColumn();

    $start = $count - 10;
    $items = 10;
    if ($count < 10) {
      $start = 0;
    }

    // get the last ID in the table
    $last = db_select($table, 'm');
    $last->fields('m', array('sourceid1', 'destid1'));
    $last->range($start, $items);
    $results = $last->execute();

    $records = array();
    while ($result = $results->fetchAssoc()) {
      array_push($records, array($result['sourceid1'], $result['destid1']));
    }

    array_unshift($records, array('Source', 'Destination'));
    return drush_print_table($records, TRUE);
  }

  return drush_log(dt('No migration records exist yet'), 'error');
}

/**
 * Show the last item migrated
 */
function migrate_inspect_random($migration) {
  $args = func_get_args();
  $alias = drush_get_context('DRUSH_TARGET_SITE_ALIAS');
  if (drush_sitealias_is_remote_site($alias)) {
    $return = drush_invoke_process($alias, 'migrate-inspect-random', $args, drush_redispatch_get_options(), array('integrate' => FALSE));
    if ($return['error_status']) {
      return drush_set_error('Unable to execute user login.');
    }
    else {
      // Prior versions of Drupal returned a string so cast to an array if needed.
      $link = $return['object'];
    }
  }
  else {

    if (!drush_bootstrap(DRUSH_BOOTSTRAP_DRUPAL_FULL)) {
      // Fail gracefully if unable to bootstrap Drupal.
      // drush_bootstrap() has already logged an error.
      return FALSE;
    }

    global $base_url;

    $migrations = migrate_migrations();
    if (!isset($migrations[$migration])) {
      drush_log(t('Migration !migration does not exist', array('!migration' => $migration)), 'error');
      return 1;
    }

    // get the map from the migration table
    $table = $migrations[$migration]->getMap()->getMapTable();
    $exists = db_query('SHOW TABLES LIKE :table', array(':table' => $table));
    if ($exists->rowCount() === 1) {
      // get the last ID in the table
      $last = db_select($table, 'm');
      $last->fields('m', array('destid1'));
      $last->orderRandom();
      $results = $last->execute();

      // we should probably run a count query and then a range query to get the
      // last one instead of this...
      $position = 0;
      $last_result = $results->fetchAssoc();

      if ($last_result) {
        $destination = $migrations[$migration]->getDestination();
        $entity_type = $destination->getEntityType();

        $load_function = $entity_type . '_load';
        if (function_exists($load_function)) {
          $entity = call_user_func($load_function, $last_result['destid1']);
        }
        else {
          $entity = entity_load($entity_type, array($last_result['destid1']));
        }

        if ($entity) {
          $function = $entity_type . '_uri';
          if (function_exists($function)) {
            $uri = call_user_func($function, $entity);
          }
          else {
            $uri = entity_uri($entity_type, $entity);
          }
          $link = $base_url . '/' . $uri['path'];
        }
      }
    }
  }

  drush_start_browser($link);
  drush_backend_set_result($link);
  return $link;
}

/**
 * Get the source id for a migrated item
 */
function migrate_inspect_source($migration, $id, $return = FALSE) {
  $migrations = migrate_migrations();
  if (!isset($migrations[$migration])) {
    drush_log(t('Migration !migration does not exist', array('!migration' => $migration)), 'error');
    return 1;
  }

  // get the map from the migration table
  $table = $migrations[$migration]->getMap()->getMapTable();
  $exists = db_query('SHOW TABLES LIKE :table', array(':table' => $table));
  if ($exists->rowCount() === 1) {
    $id = db_query('SELECT sourceid1 FROM ' . $table . ' WHERE destid1=:id', array(':id' => $id))->fetchColumn();
    if ($id) {
      if ($return) {
        return $id;
      }
      return drush_log(t('Source ID: !id', array('!id' => $id)), 'ok');
    }
  }

  if ($return) {
    return FALSE;
  }
  return drush_log(dt('No migration records exist yet'), 'error');
}

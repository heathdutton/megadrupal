<?php

/**
 * @file
 * Responsible for hooking views to add each database and its tables.
 */

/**
 * Implements hook_views_data().
 */
function views_database_connector_views_data() {
  $dbs = views_database_connector_get_database_schemas();
  $data = array();
  // Iterate through each database.
  foreach ($dbs as $dname => $db) {
    // Iterate through each table.
    foreach ($db as $table) {
      // Make sure the table name hasn't been used before.
      if (!isset($data[$table[0]])) {
        // Set the title.
        $title = '[VDC] ' . check_plain($dname) . ':  ';
        $title .= check_plain($table[0]);

        // Setup the table for Views to be able to see it.
        $data[$table[0]]['table']['group'] = $table[0];
        $data[$table[0]]['table']['base'] = array(
          // Use the first column's name as the primary field.
          'field' => $table[1][0][1],
          'title' => $title,
          'database' => $dname,
          'weight' => -9001,
        );

        // Add each column to its respective table.
        foreach ($table[1] as $col) {
          if ($col[0] == 'numeric') {
            $data[$table[0]][$col[1]] = array(
              'title' => $col[1],
              'help' => $col[1],
              'field' => array(
                'handler' => 'views_handler_field_numeric',
                'click sortable' => TRUE,
              ),
              'sort' => array(
                'handler' => 'views_handler_sort',
              ),
              'filter' => array(
                'handler' => 'views_handler_filter_numeric',
              ),
              'argument' => array(
                'handler' => 'views_handler_argument_numeric',
              ),
            );
          }
          if ($col[0] == 'date') {
            $data[$table[0]][$col[1]] = array(
              'title' => $col[1],
              'help' => $col[1],
              'field' => array(
                'handler' => 'views_handler_field_date',
                'click sortable' => TRUE,
              ),
              'sort' => array(
                'handler' => 'views_handler_sort_date',
              ),
              'filter' => array(
                'handler' => 'views_handler_filter_date',
              ),
            );
          }
          if ($col[0] == 'string') {
            $data[$table[0]][$col[1]] = array(
              'title' => $col[1],
              'help' => $col[1],
              'field' => array(
                'handler' => 'views_handler_field',
                'click sortable' => TRUE,
              ),
              'sort' => array(
                'handler' => 'views_handler_sort',
              ),
              'filter' => array(
                'handler' => 'views_handler_filter_string',
              ),
              'argument' => array(
                'handler' => 'views_handler_argument_string',
              ),
            );
          }
          if ($col[0] == 'broken') {
            $data[$table[0]][$col[1]] = array(
              'title' => $col[1],
              'help' => $col[1],
              'field' => array(
                'handler' => 'views_handler_field_broken',
                'click sortable' => TRUE,
              ),
              'sort' => array(
                'handler' => 'views_handler_sort_broken',
              ),
              'filter' => array(
                'handler' => 'views_handler_filter_broken',
              ),
              'argument' => array(
                'handler' => 'views_handler_argument_broken',
              ),
            );
          }
        }
      }
    }
  }

  // Return the finished result, allowing Views to be able to see everything
  // that it needs.
  return $data;
}

/**
 * Provides organizational categories for each data type.
 */
function views_database_connector_get_data_types() {
  $types = array(
    'numeric' => array(
      'int',
      'decimal',
      'numeric',
      'float',
      'double',
      'bit',
    ),
    'date' => array(
      'date',
      'time',
      'year',
    ),
    'string' => array(
      'char',
      'binary',
      'blob',
      'text',
      'enum',
      'set',
    ),
  );

  return $types;
}

/**
 * Gathers appropriate information from each potential database driver type.
 */
function views_database_connector_get_database_schemas() {
  $dbs = array();

  global $databases;
  // Iterate through each of the database configurations.
  foreach ($databases as $key => $datab) {
    // Excluding the default database.
    if ($key != 'default') {
      if (strtolower($datab['default']['driver']) == 'mysql') {
        // Add table list to the database list.
        $dbs[$key] = views_database_connector_get_database_schema_mysql($key);
      }
      if (strtolower($datab['default']['driver']) == 'sqlite') {
        // Add table list to the database list.
        $dbs[$key] = views_database_connector_get_database_schema_sqlite($key);
      }
      if (strtolower($datab['default']['driver']) == 'pgsql') {
        // Add table list to the database list.
        $dbs[$key] = views_database_connector_get_database_schema_pgsql($key);
      }
    }
  }
  return $dbs;
}

/**
 * Gathers appropriate information from MySQL driver type databases.
 */
function views_database_connector_get_database_schema_mysql($key) {
  // Load the appropriate data type groups.
  $types = views_database_connector_get_data_types();

  // Switch to database in question.
  db_set_active($key);
  // Get a list of the tables in this database.
  $tables = db_query('SHOW TABLES;');
  // Switch back to the main database.
  db_set_active('default');
  $tablelist = array();
  // Fetch a row, each with a table name.
  while ($row = $tables->fetchAssoc()) {
    // This is the one of two database formats that can have whacky table
    // names due to using information_schema.  We have the ability to
    // check on columns without the PDO table substitution problem.
    foreach ($row as $v) {
      if (!in_array($v, array_keys(drupal_get_complete_schema()))) {
        // Switch to database in question.
        db_set_active($key);
        // Fetch column names and their data type from said table.
        $q = 'SELECT column_name, data_type FROM ';
        $q .= 'information_schema.columns WHERE table_name = :table;';
        $cols = db_query($q, array(':table' => $v));
        // Switch back to the main database.
        db_set_active('default');
        $collist = array();
        // Fetch a row, each with a column name.
        while ($r = $cols->fetchAssoc()) {
          $t = 'broken';
          // Add column to column list.
          if (isset($r['column_name'])) {
            foreach ($types as $type => $matches) {
              foreach ($matches as $match) {
                if (stristr($r['data_type'], $match)) {
                  $t = $type;
                }
              }
            }
            $collist[] = array($t, $r['column_name']);
          }
        }
        // Add table and its columns to the table list.
        $tablelist[] = array($v, $collist);
      }
    }
  }

  return $tablelist;
}

/**
 * Gathers appropriate information from SQLite driver type databases.
 */
function views_database_connector_get_database_schema_sqlite($key) {
  // Load the appropriate data type groups.
  $types = views_database_connector_get_data_types();

  // Switch to database in question.
  db_set_active($key);
  // Get a list of the tables in this database.
  $q = 'SELECT name FROM sqlite_master WHERE type=\'table\';';
  $tables = db_query($q);
  // Switch back to the main database.
  db_set_active('default');
  $tablelist = array();
  // Fetch a row, each with a table name.
  while ($row = $tables->fetchAssoc()) {
    foreach ($row as $v) {
      // Check that the table name is safe to substitute in the query.
      if ($v == db_escape_table($v)) {
        // Switch to database in question.
        db_set_active($key);
        // Fetch column names and their data type from said table.
        $q = 'PRAGMA table_info(:table);';
        $cols = db_query(str_ireplace(':table', $v, $q));
        // Switch back to the main database.
        db_set_active('default');
        $collist = array();
        // Fetch a row, each with a column name.
        while ($r = $cols->fetchAssoc()) {
          $t = 'broken';
          // Add column to column list.
          if (isset($r['name'])) {
            foreach ($types as $type => $matches) {
              foreach ($matches as $match) {
                if (stristr($r['type'], $match)) {
                  $t = $type;
                }
              }
            }
            $collist[] = array($t, $r['name']);
          }
        }
        // Add table and its columns to the table list.
        $tablelist[] = array($v, $collist);
      }
    }
  }

  return $tablelist;
}

/**
 * Gathers appropriate information from PostgreSQL driver type databases.
 */
function views_database_connector_get_database_schema_pgsql($key) {
  // Load the appropriate data type groups.
  $types = views_database_connector_get_data_types();

  // Switch to database in question.
  db_set_active($key);
  // Get a list of the tables in this database.
  $q = 'SELECT table_name FROM ';
  $q .= 'information_schema.tables WHERE table_type = \'BASE TABLE\' AND ';
  $q .= 'table_schema NOT IN (\'pg_catalog\', \'information_schema\');';
  $tables = db_query($q);
  // Switch back to the main database.
  db_set_active('default');
  $tablelist = array();
  // Fetch a row, each with a table name.
  while ($row = $tables->fetchAssoc()) {
    foreach ($row as $v) {
      // Switch to database in question.
      db_set_active($key);
      // Fetch column names and their data type from said table.
      $q = 'SELECT column_name, data_type FROM ';
      $q .= 'information_schema.columns WHERE table_name = :table;';
      $cols = db_query($q, array(':table' => $v));
      // Switch back to the main database.
      db_set_active('default');
      $collist = array();
      // Fetch a row, each with a column name.
      while ($r = $cols->fetchAssoc()) {
        $t = 'broken';
        // Add column to column list.
        if (isset($r['column_name'])) {
          foreach ($types as $type => $matches) {
            foreach ($matches as $match) {
              if (stristr($r['data_type'], $match)) {
                $t = $type;
              }
            }
          }
          $collist[] = array($t, $r['column_name']);
        }
      }
      // Add table and its columns to the table list.
      $tablelist[] = array($v, $collist);
    }
  }

  return $tablelist;
}

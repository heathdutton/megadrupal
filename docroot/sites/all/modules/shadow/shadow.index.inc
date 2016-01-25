<?php

/**
 * Index row with given primary key. Will rebuild whole table if no primary key given.
 *
 * @param int $tid Table ID
 * @param int $pkey Primary key
 */
function shadow_index_add($tid, $pkey = NULL) {
  $sql = 'SELECT tid, db_name, base_table FROM {shadow_table} WHERE tid = :tid';
  if (!$table = db_query($sql, array('tid' => $tid))->fetchObject()) {
    return FALSE;
  }
  
  _shadow_load_classes();
  
  $column_parser = new ShadowColumn();
  $filter_parser = new ShadowFilter();

  $columns = array();
  $sql = 'SELECT * FROM {shadow_column} WHERE tid = :tid';
  $res2 = db_query($sql, array(':tid' => $tid));
  foreach ($res2 as $column) {
    $columns[] = $column;
  }

  $filters = array();
  $sql = 'SELECT * FROM {shadow_filter} WHERE tid = :tid';
  $res = db_query($sql, array(':tid' => $tid));
  foreach ($res as $filter) {
    $filters[] = $filter;
  }

  $sql_fields = array();
  $sql_fieldnames = array();
  $sql_joins = array();
  $sql_joins_args = array();
  $sql_conditions = array();
  $sql_conditions_args = array();
  $sql_args = array();
  
  // Reset the argument names.
  _shadow_argument_name(TRUE);
  
  $alias = 1;
  foreach ($columns as $column) {
    $column_parser->parse($column->definition);
    $left_alias = 'b';
    $last_alias = $left_alias;
    while ($join = $column_parser->nextJoin($left_alias, "a$alias")) {
      extract($join); // Introduces $sql and $args
      $sql_args = array_merge($sql_args, $args);
      $sql_joins[] = $sql;
      $last_alias = "a$alias";
      ++$alias;
      $left_alias = "a$alias";
    }
    $sql_fieldnames[] = $column->db_name;
    $field = $column_parser->getFieldName();
    if (is_null($column->invert_base)) {
      $sql_fields[] = "$last_alias.$field as $column->db_name";
    }
    else {
      $sql_fields[] = "$column->invert_base - $last_alias.$field as $column->db_name";
    }
  }

  foreach ($filters as $filter) {
    $filter_parser->parse($filter->definition);
    $left_alias = 'b';
    $last_alias = $left_alias;
    while ($join = $filter_parser->nextJoin($left_alias, "a$alias")) {
      extract($join); // Introduces $sql and $args
      $sql_args = array_merge($sql_args, $args);
      $sql_joins[] = $sql;
      $last_alias = "a$alias";
      ++$alias;
      $left_alias = "a$alias";
    }
    $field = $filter_parser->getFieldName();
    $values = $filter_parser->getValues();
    $args = array();
    foreach ($values as $value) {
      $args[_shadow_argument_name()] = $value;
    }
    $sql_args = array_merge($sql_args, $args);
    $placeholders = implode(', ', array_keys($args));
    $sql_conditions[] = "$last_alias.$field IN ($placeholders)";
  }

  if (is_array($pkey)) {
    foreach ($pkey as $name => $value) {
      $argument_name = _shadow_argument_name();
      $sql_conditions[] = "b.$name = $argument_name";
      $sql_args[$argument_name] = $value;
    }
  }
  
  $sql_fields = implode(', ', $sql_fields);
  $sql_fieldnames = implode(', ', $sql_fieldnames);
  $sql_joins = implode(' ', $sql_joins);
  $sql_conditions = $sql_conditions ? 'WHERE ' . implode(' AND ', $sql_conditions) : '';

  if (is_array($pkey)) {
    $shadow_table = $table->db_name;
  }
  else {
    $schema = drupal_get_schema_unprocessed('shadow');
    $schema = $schema[$table->db_name];
    $shadow_table = 'shadow_' . md5(microtime());
    db_create_table($shadow_table, $schema);
  }
  
  $sql = "INSERT INTO {{$shadow_table}} ($sql_fieldnames) SELECT $sql_fields FROM {{$table->base_table}} b $sql_joins $sql_conditions";
  db_query($sql, $sql_args);

  if (!is_array($pkey)) {
    $sql = "RENAME TABLE {{$table->db_name}} TO {{$shadow_table}2}, {{$shadow_table}} TO {{$table->db_name}}";
    db_query($sql);

    $sql = "DROP TABLE {{$shadow_table}2}";
    db_query($sql);

    $sql = 'UPDATE {shadow_table} SET ready = 1 WHERE tid = :tid';
    db_query($sql, array(':tid' => $tid));
  }

  return TRUE;
}

function shadow_index_delete($tid, $pkey) {
  $table = db_select('shadow_table', 't')
           ->fields('t', array('db_name', 'base_table'))
           ->condition('t.tid', $tid)
           ->execute()
           ->fetchObject();
  if (!$table) {
    return FALSE;
  }
  
  $columns = db_select('shadow_column', 'c')
             ->fields('c', array('definition', 'db_name'))
             ->condition('c.tid', $tid)
             ->execute()
             ->fetchAll();

  $query = db_delete($table->db_name);
  foreach ($pkey as $name => $value) {
    foreach ($columns as $column) {
      if ($column->definition == "{{$table->base_table}}.$name" || $column->definition == "{$table->base_table}.$name") {
        $query->condition($column->db_name, $value);
        continue(2);
      }
    }
    // One of the primary key columns was not found.
    return FALSE;
  }
  $query->execute();
  
  return TRUE;
}

function _shadow_parse_values($values) {
  $values = (string) $values;
  $output = array();
  $length = strlen($values);
  $buffer = '';
  $prev_backslash = FALSE;
  $in_string = FALSE;
  for ($i = 0; $i < $length; ++$i) {
    switch ($values{$i}) {
      case '\'':
        if (!$prev_backslash) {
          $in_string = $in_string ? FALSE : TRUE;
        }
        break;
      case '\\':
        if ($prev_backslash) {
          $prev_backslash = FALSE;
        }
        break;
      case ',':
        if (!$in_string) {
          $output[] = $buffer;
          $buffer = '';
        }
        continue(2);
    }
    $prev_backslash = $values{$i} == '\\';
    $buffer .= $values{$i};
  }
  if ($buffer !== '') {
    $output[] = $buffer;
  }
  for ($i = 0; $i < count($output); ++$i) {
    if (strlen($output[$i]) && $output[$i]{0} == '\'') {
      $output[$i] = substr($output[$i], 1, strlen($output[$i]) - 2);
      $output[$i] = str_replace(array('\\\\', '\\\''), array('\\', '\''), $output[$i]);
    }
    else {
      $output[$i] = (float) $output[$i];
    }
  }
  return $output;
}

/**
 * Update a single row in the shadow tables.
 *
 * @param string $op 'insert', 'update' or 'delete'
 * @param string $base_table Base table of the updated row
 * @param array $pkey Primary key of the updated row
 * @param int $phase Which phase to execute:
 *   1 = Retrieve (re)index command
 *   2 = Process command in hook_exit
 *   3 = Process command in hook_cron
 */
function shadow_index($op, $base_table, $pkey, $phase = 1) {
  global $_shadow_index_queue;
  
  if (!isset($_shadow_index_queue)) {
    $_shadow_index_queue = array();
  }
  
  if ($phase == 1) {
    /**
     * We do not process this job here, as the query executed by shadow_index_add()
     * may contain JOIN's to rows which will be added later (i.e. in a
     * hook_nodeapi implementation). We will try to process these jobs in
     * hook_exit (which we call phase 2).
     */
    $_shadow_index_queue[] = array(
        'op' => $op,
        'base_table' => $base_table,
        'pkey' => $pkey,
    );
    return;
  }
  elseif ($phase == 2) {
    /**
     * SELECT queries may not always retreive the latest data. Thus the SELECT
     * query can miss the row just inserted. This is not unusual when the
     * database server has a high load. We add this operation to a queue to
     * reprocess them in cron to make sure that the shadow tables are kept
     * up-to-date.
     */
    if ($op == 'insert') {
      // Force to do a delete first, cause we can't do an insert twice.
      $op = 'update';
    }
    $rec = new stdClass();
    $rec->op = $op;
    $rec->base_table = $base_table;
    $rec->pkey = serialize($pkey);
    drupal_write_record('shadow_index_queue', $rec);
  }

  $tids = db_select('shadow_table', 't')
         ->fields('t', array('tid'))
         ->condition('t.base_table', $base_table)
         ->execute()
         ->fetchCol();
  foreach ($tids as $tid) {
    switch ($op) {
      case 'insert':
        shadow_index_add($tid, $pkey);
        break;
      case 'update':
        shadow_index_delete($tid, $pkey);
        shadow_index_add($tid, $pkey);
        break;
      case 'delete':
        shadow_index_delete($tid, $pkey);
        break;
    }
  }
}

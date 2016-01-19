<?php

function paas_metric_dbinfo_call($value) {
  global $databases;
  $dbname = $databases['default']['default']['database'];
 
  switch ($value) {
    case 'tables':
      $sql = 'SELECT TABLE_NAME as name, ENGINE as engine, CASE ROW_FORMAT WHEN \'Dynamic\' THEN 1 ELSE 0 END as dynamic, TABLE_ROWS as rows
      FROM INFORMATION_SCHEMA.TABLES
      WHERE TABLE_SCHEMA = :dbname';
      paas_dump_csv($sql, array(':dbname' => $dbname));
      break;
    case 'indexes':
      $sql = 'SELECT TABLE_NAME as table_name, INDEX_NAME as index_name, GROUP_CONCAT(COLUMN_NAME ORDER BY SEQ_IN_INDEX) as columns,
      MAX(CARDINALITY) as cardinality, GROUP_CONCAT(CARDINALITY ORDER BY SEQ_IN_INDEX) as column_cardinality, INDEX_TYPE as type
      FROM INFORMATION_SCHEMA.STATISTICS
      WHERE TABLE_SCHEMA = :dbname 
      GROUP BY TABLE_NAME, INDEX_NAME
      ORDER BY TABLE_NAME, SEQ_IN_INDEX';
      paas_dump_csv($sql, array(':dbname' => $dbname));
      break;
    default:
      $cardinality = NULL;
      if (preg_match('/^(.*)\\.(.*)$/si', $value, $match)) {
        $table = $match[1];
        $field = $match[2];
        $length = NULL;
        if ($schema = drupal_get_schema($table)) {
          if (isset($schema['fields'][$field])) {
            $type = $schema['fields'][$field]['type'];
            $sql = '';
            switch ($type) {
              case 'varchar':
                $length = $schema['fields'][$field]['length'];
                $length = min(ceil($length / 4), 8);
                $sql = "SELECT COUNT(*) FROM (SELECT DISTINCT SUBSTRING($field FROM 1 FOR $length) FROM {{$table}}) t";
                break;
              case 'serial':
              case 'int':
                $sql = "SELECT COUNT(*) FROM (SELECT DISTINCT $field FROM {{$table}}) t";
                break;
            }
            if (!empty($sql)) {
              $res = db_query($sql);
              $cardinality = $res->fetchField();
            }
          }
        }
        $cardinality = (int) $cardinality;
        $length = (int) $length;
        echo "$cardinality,$length";
      }
      break;
  }
}

<?php

/**
 * The implementation of a SQL-joinable storage.
 *
 * The query returned by this implementation is a SelectQueryInterface. Providers
 * using this implementation can join to denormalized tables.
 */
abstract class Tree_Storage_SQL implements Tree_Storage {
  /**
   * Get the database connection for this storage.
   *
   * @return DatabaseConnection
   */
  abstract public function getDatabase();

  /**
   * Get the column map for this SQL-joinable storage.
   */
  abstract public function getColumnMap();

  /**
   * Get the name of the base table.
   */
  abstract public function getTableName();

  function query() {
    return new Tree_Storage_SQL_Query($this->database, $this->tableName, $this->columnMap);
  }
}

class Tree_Storage_SQL_Item implements Tree_Storage_Item {
  public $id = NULL;
  public $parent = NULL;
  public $weight = 0;

  function __construct(array $row, array $column_map) {
    $this->row = $row;
    $this->columnMap = $column_map;

    foreach ($this->columnMap as $key => $row_key) {
      if (isset($this->row[$row_key])) {
        $this->$key = $this->row[$row_key];
      }
    }
  }

  function getRow() {
    $row = array();
    foreach ($this->columnMap as $key => $row_key) {
      $row[$row_key] = $this->$key;
    }

    return $row + $this->row;
  }
}

/**
 * An implementation of Tree_Storage_Query that does a direct query on a table.
 */
class Tree_Storage_SQL_Query implements Tree_Storage_Query {
  function __construct(DatabaseConnection $database, $table_name, array $column_map) {
    $this->query = $database->select($table_name, 't', array('fetch' => PDO::FETCH_ASSOC));
    $this->query->fields('t');
    $this->columnMap = $column_map;
  }

  function execute() {
    $results = array();
    foreach ($this->query->execute() as $row) {
      $results[] = new Tree_Storage_SQL_Item($row, $this->columnMap);
    }
    return $results;
  }

  function condition($column, $value, $operator = NULL) {
    $this->query->condition('t.' . $this->columnMap[$column], $value, $operator);
    return $this;
  }

  function orderBy($column, $direction = 'ASC') {
    $this->query->orderBy('t.' . $this->columnMap[$column], $direction);
    return $this;
  }

  function isNull($column) {
    $this->query->isNull('t.' . $this->columnMap[$column]);
    return $this;
  }

  function isNotNull($column) {
    $this->query->isNotNull('t.' . $this->columnMap[$column]);
    return $this;
  }

  function alwaysFalse() {
    $this->query->where('1 = 0');
    return $this;
  }
}

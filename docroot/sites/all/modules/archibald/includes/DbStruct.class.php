<?php

/**
 * @file
 * database abstraction class
 * @author c.ackermann@educa.ch
 */
class ArchibaldDbStruct {

  /**
   * Define constances
   */
  const T_BLOB = 'blob';
  const T_NUMERIC = 'numeric';
  const T_SERIAL = 'serial';
  const T_FLOAT = 'float';
  const T_STRING = 'varchar';
  const T_VARCHAR = 'varchar';
  const T_DECIMAL = 'decimal';
  const T_TEXT = 'text';
  const T_TINYINT = 1;
  const T_MEDIUMINT = 2;
  const T_BIGINT = 3;
  const T_SMALLINT = 4;
  const T_INT = 5;
  const T_SUBTABLE = 6;
  const ADDITIONAL_SCALE = 'scale';
  const ADDITIONAL_PRECISION = 'precision';
  const ADDITIONAL_UNSIGNED = 'unsigned';
  const ADDITIONAL_MYSQL_TYPE = 'mysql_type';
  const ADDITIONAL_NOT_NULL = 'not null';
  const ADDITIONAL_SERIALIZE = 'serialize';
  const INDEX_FOREIGN = 'foreign';
  const INDEX_INDEXES = 'indexes';

  private $struct = array();
  private $defaultValues = array();
  private $tableName = "";
  private $description = "";
  private $indexe = array("foreign" => array(), "indexes" => array());
  private $autoIncrement = "";
  private $primary = array();
  private $unique = array();
  private $mapperTables = array();

  /**
   * constructor
   *
   * @param String $table_name
   *   The sql table
   */
  public function __construct($table_name, $description) {
    $this->tableName = $table_name;
    $this->description = $description;
  }

  /**
   * getter for autoIncrement
   *
   * @return return
   *   the key as a string
   */
  public function getAutoIncrement() {
    return $this->autoIncrement;
  }

  /**
   * setter for autoIncrement
   *
   * @param String $val
   *   the db field which has auto increment
   */
  public function setAutoIncrement($val) {
    $this->autoIncrement = $val;
  }

  /**
   * return the SQL Table Referenzkey where to update or delete
   *
   * @return string
   *   The SQL Table Referenzkey as an Array
   */
  public function getReferenzKey() {
    return $this->referenzKey;
  }

  /**
   * Get the Database struct
   *
   * @return array
   *   The Database struct as an Array
   */
  public function getStruct() {
    return $this->struct;
  }

  /**
   * check if struct has db field
   *
   * @param String $field
   *   The DB Field
   *
   * @return boolean
   *   if exist return TRUE, else FALSE
   */
  public function hasField($field) {
    return isset($this->struct[$field]);
  }

  /**
   * returns the db struct field
   *
   * @param String $field
   *   The DB Field
   *
   * @return boolean
   *   if exist return the field, else FALSE
   */
  public function getField($field) {
    if (isset($this->struct[$field])) {
      return $this->struct[$field];
    }
    return FALSE;
  }

  /**
   * Adds a primary key to the struct
   *
   * @param string $column1
   *   the column
   * @param string $column2
   *   the second column
   * ...
   */
  public function addPrimaryKey() {
    $args = func_get_args();
    if (count($args) <= 0) {
      return;
    }
    $this->primary = $args;
  }

  /**
   * Adds a unique key to the struct
   *
   * @param string $column1
   *   the column
   * @param string $column2
   *   the second column
   *  ...
   */
  public function addUniqueKey() {
    $args = func_get_args();
    if (count($args) <= 0) {
      return;
    }
    $args_key = $args;
    foreach ($args_key as $k => $v) {
      if (is_array($v)) {
        $args_key[$k] = reset($v);
      }
    }
    $this->unique = array($this->getKeyKey($args_key) => $args);
  }

  /**
   * Returns the primary key field
   *
   * @return string
   */
  public function getPrimaryKey() {
    return $this->primary;
  }

  /**
   * Add indexes
   *
   * @param string $relation
   *   the relation name
   * @param string $key
   *   which column
   * @param string $type
   *   the type, use ArchibaldDbStruct::INDEX_* (optional, default = INDEX_INDEXES)
   */
  public function addIndexKey($relation, $key, $type = self::INDEX_INDEXES) {
    if (!isset($this->indexe[$type][$relation])) {
      $this->indexe[$type][$relation] = array();
    }
    $this->indexe[$type][$relation][] = $key;
  }

  /**
   *   returns the database type for that field
   *   @return the type as an integer, based on constants T_*
   */
  public function getFieldType($key) {

    if (!$this->hasField($key)) {
      return - 1;
    }
    return $this->struct[$key]['typ'];
  }

  /**
   * Add a struct field
   *
   * @param string $name
   *   The Database field
   * @param integer $type
   *   The Database field typ ( use Constant for T_STRING, T_INT, T_FLOAT....)
   * @param string $description
   *   The Description
   * @param integer $length
   *   the field length
   * @param mixed $default_value
   * @param array $additional
   *   an array with additional field information
   */
  public function addField($name, $type, $description = "", $length = NULL, $additional = array(self::ADDITIONAL_NOT_NULL => FALSE)) {

    $size = $this->getSize($type);
    if ($size != NULL) {
      $this->struct[$name]['size'] = $size;

      if (is_numeric($type)) {
        $type = 'int';
        if (!isset($additional[self::ADDITIONAL_UNSIGNED])) {
          $additional[self::ADDITIONAL_UNSIGNED] = TRUE;
        }
      }
    }

    $this->struct[$name]['description'] = $description;
    $this->struct[$name]['type'] = $type;

    if (is_numeric($length) && $length > 0) {
      $this->struct[$name]['length'] = $length;
    }

    if (!empty($additional)) {
      foreach ($additional as $k => $v) {
        $this->struct[$name][$k] = $v;
      }
    }
  }

  /**
   * Returns the size string for drupal schema for given struct type
   *
   * @param int $type
   * 	 the dbstruct type
   *
   * @return string
   * 	 returns the size mapping
   */
  private function getSize($type) {
    switch ($type) {
      case self::T_INT:
        return 'normal';

      case self::T_TINYINT:
        return 'tiny';

      case self::T_SMALLINT:
        return 'small';

      case self::T_MEDIUMINT:
        return 'medium';

      case self::T_BIGINT:
        return 'big';

      case self::T_TEXT:
        return 'medium';

      default:
        return NULL;
    }
  }

  /**
   * Get the default value for the given param
   *
   * @param String $name
   * 	 The Database field
   *
   * @return mixed
   *   The default value, if value doesnt exist or no default value exist return FALSE
   */
  public function getDefaultValue($name) {
    if (isset($this->struct[$name]) && isset($this->struct[$name]['default'])) {
      return $this->struct[$name]['default'];
    }
    else {
      return FALSE;
    }
  }

  /**
   * Returns the drupal schema for the current db struct
   *
   * @return array
   */
  public function getSchema() {
    $fields = array();

    foreach ($this->struct as $field => &$value) {
      if (isset($value['typ']) && $value['typ'] == self::T_SUBTABLE) {
        continue;
      }
      $fields[$field] = $value;
    }

    $schema = array(
      'description' => $this->description,
      'fields' => $fields,
      'primary key' => $this->primary,
      'unique keys' => $this->unique,
      'foreign keys' => $this->indexe[self::INDEX_FOREIGN],
      'indexes' => $this->indexe[self::INDEX_INDEXES],
    );

    if (empty($schema['primary key'])) {
      unset($schema['primary key']);
    }

    if (empty($schema['unique key'])) {
      unset($schema['unique key']);
    }

    if (empty($schema['foreign key'])) {
      unset($schema['foreign key']);
    }

    if (empty($schema['indexes'])) {
      unset($schema['indexes']);
    }

    return array($this->tableName => $schema);
  }

  /**
   * Get the Database table
   *
   * @return string
   *   The Database table as a String
   */
  public function getTable() {
    return $this->tableName;
  }

  /**
   * get key if primary key
   *
   * @param array $args_key
   *
   * @return string
   */
  protected function getKeyKey($args_key) {
    foreach ($args_key as $k => $v) {
      if (is_array($v)) {
        $args_key[$k] = reset($v);
      }
    }

    return implode('_', $args_key);
  }
}


<?php

namespace Drupal\doctrine\Schema;

use Doctrine\DBAL\Schema\AbstractSchemaManager;
use Doctrine\DBAL\Schema\Column;
use Doctrine\DBAL\Schema\ForeignKeyConstraint;
use Doctrine\DBAL\Schema\Index;
use Doctrine\DBAL\Schema\Table;
use Doctrine\DBAL\Types\Type;

/**
 * SchemaManager is used to inspect drupal database structure from Schema API.
 *
 * @since 7.x-1.0
 * @author Sylvain Lecoy <sylvain.lecoy@gmail.com>
 *
 */
class SchemaManager extends AbstractSchemaManager {

  /**
   * Array information about the whole database schema.
   *
   * @var array
   *
   * @see drupal_get_schema()
   */
  protected $schemas;

  /**
   * Constructs a SchemaManager.
   *
   * @param \Doctrine\DBAL\Connection $conn
   * @param array $schemas
   */
  public function __construct(\Doctrine\DBAL\Connection $conn, array $schemas) {
    $this->schemas = $schemas;
    parent::__construct($conn);
  }

  /**
   * (non-PHPdoc)
   * @see \Doctrine\DBAL\Schema\AbstractSchemaManager::listTableNames()
   */
  public function listTableNames() {
    return array_keys($this->schemas);
  }

  /**
   * (non-PHPdoc)
   * @see \Doctrine\DBAL\Schema\AbstractSchemaManager::listTableDetails()
   */
  public function listTableDetails($tableName) {
    $columns = $this->listTableColumns($tableName);
    $fkConstraints = $this->listTableForeignKeys($tableName);
    $indexes = $this->listTableIndexes($tableName);

    return new Table($tableName, $columns, $indexes, $fkConstraints, FALSE, array());
  }

  /**
   * (non-PHPdoc)
   * @see \Doctrine\DBAL\Schema\AbstractSchemaManager::listTableColumns()
   */
  public function listTableColumns($table, $database = null) {
    $list = array();

    foreach ($this->schemas[$table]['fields'] as $columnName => $column) {
      $typeName = $this->getColumnType($column);
      $options = $this->getColumnOptions($column);
      $list[$columnName] = new Column($columnName, Type::getType($typeName), $options);
    }

    return $list;
  }

  /**
   * (non-PHPdoc)
   * @see \Doctrine\DBAL\Schema\AbstractSchemaManager::listTableForeignKeys()
   */
  public function listTableForeignKeys($table, $database = null) {
    $list = array();

    if (isset($this->schemas[$table]['foreign keys'])) {
      foreach ($this->schemas[$table]['foreign keys'] as $foreignKey) {
        $localColumnNames = array_keys($foreignKey['columns']);
        $foreignTableName = $foreignKey['table'];
        $foreignColumnNames = array_values($foreignKey['columns']);
        $list[] = new ForeignKeyConstraint($localColumnNames, $foreignTableName, $foreignColumnNames);
      }
    }

    return $list;
  }

  /**
   * (non-PHPdoc)
   * @see \Doctrine\DBAL\Schema\AbstractSchemaManager::listTableIndexes()
   */
  public function listTableIndexes($table) {
    $list = array();

    if (isset($this->schemas[$table]['primary key'])) {
      $list['primary'] = new Index('primary', $this->schemas[$table]['primary key'], TRUE, TRUE);
    }

    if (isset($this->schemas[$table]['unique keys'])) {
      foreach ($this->schemas[$table]['unique keys'] as $uniqueKeyName => $columns) {
        $list[$uniqueKeyName] = new Index($uniqueKeyName, $columns, TRUE);
      }
    }

    if (isset($this->schemas[$table]['indexes'])) {
      foreach ($this->schemas[$table]['indexes'] as $indexName => $columns) {
        $list[$indexName] = new Index($indexName, $this->normalizeIndexColumns($columns));
      }
    }

    return $list;
  }

  /**
   * Removes the index size option.
   *
   * TODO: Check this feature can be kept.
   *
   * @param array $columns
   */
  private function normalizeIndexColumns($columns) {
    $list = array();

    foreach ($columns as $column) {
      $list[] = is_array($column) ? $column[0] : $column;
    }

    return $list;
  }

  /**
   * Gets Schema API column options.
   *
   * @param array $column
   *
   * @return string
   *   The Doctrine translated type.
   */
  protected function getColumnOptions($column) {
    return array(
      'length'    => isset($column['length']) ? $column['length'] : NULL,
      'unsigned'  => isset($column['unsigned']) ? $column['unsigned'] : NULL,
      'default'   => isset($column['default']) ? $column['default'] : NULL,
      'notnull'   => isset($column['not null']) ? $column['not null'] : NULL,
      'scale'     => isset($column['scale']) ? $column['scale'] : NULL,
      'precision' => isset($column['precision']) ? $column['precision'] : NULL,
      'comment'   => isset($column['description']) ? $column['description'] : NULL,
   /* 'autoincrement' => TODO */
    );
  }

  /**
   * Maps Schema API types back into Doctrine types.
   *
   * @param array $column
   *
   * @return string
   *   The Doctrine translated type.
   */
  protected function getColumnType($column) {
    // Maps schema types back into doctrine types.
    // $map does not use drupal_static as its value never changes.
    static $map = array(
        'varchar:normal'  => Type::STRING,
        'char:normal'     => Type::STRING,

        'text:tiny'       => Type::TEXT,
        'text:small'      => Type::TEXT,
        'text:medium'     => Type::TEXT,
        'text:big'        => Type::TEXT,
        'text:normal'     => Type::TEXT,

        'serial:tiny'     => Type::BOOLEAN,
        'serial:small'    => Type::SMALLINT,
        'serial:medium'   => Type::INTEGER,
        'serial:big'      => Type::BIGINT,
        'serial:normal'   => Type::INTEGER,

        'int:tiny'        => Type::BOOLEAN,
        'int:small'       => Type::SMALLINT,
        'int:medium'      => Type::INTEGER,
        'int:big'         => Type::BIGINT,
        'int:normal'      => Type::INTEGER,

        'float:tiny'      => Type::FLOAT,
        'float:small'     => Type::FLOAT,
        'float:medium'    => Type::FLOAT,
        'float:big'       => Type::FLOAT,
        'float:normal'    => Type::FLOAT,

        'numeric:normal'  => Type::DECIMAL,

        'blob:big'        => Type::BLOB,
        'blob:normal'     => Type::BLOB,
    );

    $type = $column['type'];
    $size = isset($column['size']) ? $column['size'] : 'normal';

    return $map["$type:$size"];
  }

  // Not used.
  protected function _getPortableTableColumnDefinition($tableColumn) {}
}

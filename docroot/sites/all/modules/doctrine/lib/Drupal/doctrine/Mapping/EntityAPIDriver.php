<?php

/**
 * @file
 * The SchemaDriver reads the mapping meta-data from Schema API.
 */

namespace Drupal\doctrine\Mapping;

use Doctrine\Common\Persistence\Mapping\Driver\MappingDriver;
use Doctrine\Common\Persistence\Mapping\ClassMetadata;
use Doctrine\Common\Util\Inflector;
use Doctrine\ORM\Mapping\ClassMetadataInfo;

use Drupal\doctrine\Schema\SchemaManager;

use Inflect\Inflect;
use Doctrine\DBAL\Schema\Table;

/**
 * Reads the mapping meta-data from Entity API.
 *
 * The meta-data building process depends on Entity API through this class, as
 * the driver relies on entity info. Using this feature adds a substantial
 * performance hit to schema driver as more meta-data has to be loaded into
 * memory than might actually be necessary. This may not be relevant to scenarios
 * where caching of meta-data is in place, however hits hardly in scenarios where
 * no caching is used.
 *
 * @since 7.x-1.0
 * @author Sylvain Lecoy <sylvain.lecoy@gmail.com>
 */
class EntityAPIDriver implements MappingDriver {

  /**
   * Class to table name map.
   *
   * @var array
   */
  protected $classToTableNames = array();

  /**
   * Table to class name map.
   *
   * @var array
   */
  protected $classNamesForTables = array();

  /**
   * @var \Doctrine\DBAL\Schema\Table
   */
  protected $tables = array();

  /**
   * Many to many tables.
   *
   * @var \Doctrine\DBAL\Schema\Table
   */
  protected $manyToManyTables = array();

  /**
   * Constructs a SchemaDriver.
   *
   * @param SchemaManager $schema
   * @param array $entityInfo
   */
  public function __construct(SchemaManager $schema, array $entityInfo) {
    // The entity to schema map building process depends on Entity API through
    // this constructor, as the driver relies on hook_entity_info() meta-data.
    foreach (array_values($entityInfo) as $entity) {
      if (isset($entity['entity class'])) {
        // Only map entities which declares an 'entity class' in their info
        // hook. Entity API requires the 'base table' meta data so the driver
        // can safely assumes it is existing and contains functional value.
        $this->classToTableNames[$entity['entity class']] = $entity['base table'];
        $this->classNamesForTables[$entity['base table']] = $entity['entity class'];
      }
    }

    $tables = array();
    // The schema references building process depends on Schema API through
    // this constructor, as the driver relies on hook_schema() meta-data.
    foreach ($schema->listTableNames() as $tableName) {
      $tables[$tableName] = $schema->listTableDetails($tableName);
    }

    // Categorizes each tables: they can represent an entity, or a join table in
    // a ManytoMany relationship. Other uses cases are difficult to reverse
    // engineer from a Conceptual Data Model so the driver just ignore them.
    foreach ($tables as $tableName => /* @var $table Table */ $table) {
      $foreignKeys = $table->getForeignKeys();
      $allForeignKeyColumns = $this->getAllForeignKeyColumns($foreignKeys);

      if (!$table->hasPrimaryKey()) {
        // Table has no primary key. Doctrine does not support reverse
        // engineering from tables that don't have a primary key.
        continue;
      }

      $pkColumns = $table->getPrimaryKey()->getColumns();
      sort($pkColumns);
      sort($allForeignKeyColumns);

      if ($pkColumns == $allForeignKeyColumns && count($foreignKeys) == 2) {
        // The physical data model (PDM) of ManyToMany and OneToMany
        // (unidirectional) relationship is composed of two foreign keys.
        $this->manyToManyTables[$tableName] = $table;
      }
      else if (array_key_exists($tableName, $this->classNamesForTables)) {
        // When the table name is mapped to an entity, adds it to the pool.
        $this->tables[$tableName] = $table;
      }
      // Others uses cases are complexes and under the programmer responsibility.
    }
  }

  /**
   * {@inheritDoc}
   */
  public function loadMetadataForClass($className, ClassMetadata $metadata) {

    if (empty($this->classToTableNames[$className])) {
      // If entity class has not been found in entity map, throws an unexpected
      // value exception indicating the value does not match with the entity list.
      throw new \UnexpectedValueException(sprintf('Unknown entity type: %s.', $class));
    }

    $tableName = $this->classToTableNames[$className];

    $metadata->name = $className;
    $metadata->table['name'] = $tableName;

    $columns = $this->tables[$tableName]->getColumns();
    $indexes = $this->tables[$tableName]->getIndexes();
    // An entity must have an identifier/primary key. Thus the driver assumes
    // a 'primary key' meta-data exists in the schema definition.
    $primaryKeyColumns = $this->tables[$tableName]->getPrimaryKey()->getColumns();
    // The Schema API Conceptual Data Model defines foreign key so it is assumed
    // the database platform support them and the entities generated contains
    // the relationships defined.
    $foreignKeys = $this->tables[$tableName]->getForeignKeys();
    // Sometimes a foreign key can be composed of multiple columns, by getting
    // all columns involved in a constraint, it is ensured to not map blindly
    // a field which has a greater responsibility.
    $allForeignKeyColumns = $this->getAllForeignKeyColumns($foreignKeys);

    $ids = array();
    $fieldMappings = array();
    foreach ($columns as $column) {
      $fieldMapping = array();

      // Checks if the columns is part of a foreign key constraint.
      if (in_array($column->getName(), $allForeignKeyColumns)) {
        // A foreign key in schema can represent either a OneToOne relationship
        // or a ManyToOne relationship. The relation is treated like a toOne
        // association without the Many part. Do not map the field and continue
        // the loop with the next column item.
        continue;
      }
      else if ($primaryKeyColumns && in_array($column->getName(), $primaryKeyColumns)) {
        // Adds the 'id' flag when the column is part of the primary key.
        $fieldMapping['id'] = TRUE;
      }

      $fieldMapping['fieldName'] = $this->getFieldNameForColumn($tableName, $column->getName());
      $fieldMapping['columnName'] = $column->getName();
      $fieldMapping['type'] = strtolower((string) $column->getType());

      if ($column->getType() instanceof \Doctrine\DBAL\Types\StringType) {
        $fieldMapping['length'] = $column->getLength();
        $fieldMapping['fixed'] = $column->getFixed();
      }
      else if ($column->getType() instanceof \Doctrine\DBAL\Types\IntegerType) {
        $fieldMapping['unsigned'] = $column->getUnsigned();
      }
      $fieldMapping['nullable'] = $column->getNotnull() ? FALSE : TRUE;

      if (isset($fieldMapping['id'])) {
        $ids[] = $fieldMapping;
      }
      else {
        $fieldMappings[] = $fieldMapping;
      }
    }

    if ($ids) {
      if (count($ids) == 1) {
        $metadata->setIdGeneratorType(ClassMetadataInfo::GENERATOR_TYPE_AUTO);
      }

      foreach ($ids as $id) {
        $metadata->mapField($id);
      }
    }

    foreach ($fieldMappings as $fieldMapping) {
      $metadata->mapField($fieldMapping);
    }

    foreach ($this->manyToManyTables as $manyTable) {
      // A reference in schema can represent either a OneToMany relationship if
      // the join schema is composed of a unique field referencing the foreign
      // entity or a ManyToMany relationship if not. It can also represent a
      // OneToOne (bidirectional) relationship. The relation is treated like a
      // toMany association without the One part.
      foreach ($manyTable->getForeignKeys() as $foreignKey) {
        // Foreign key maps to the table of the current entity, many to many
        // association probably exists.
        if (strtolower($tableName) == strtolower($foreignKey->getForeignTableName())) {
          $ownerFk = $foreignKey;
          $otherFk = NULL;
          foreach ($manyTable->getForeignKeys() as $foreignKey) {
            if ($foreignKey != $ownerFk) {
              $otherFk = $foreignKey;
              break;
            }
          }

          if (!$otherFk || empty($this->classNamesForTables[$otherFk->getForeignTableName()])) {
            // The definition of this many to many table does not contain enough
            // foreign key information to continue schema introspection.
            continue;
          }

          $localColumn = current($ownerFk->getColumns());
          $associationMapping = array();
          $associationMapping['fieldName'] = $this->getFieldNameForColumn($manyTable->getName(), current($otherFk->getColumns()), TRUE);
          $associationMapping['targetEntity'] = $this->classNamesForTables[$otherFk->getForeignTableName()];
          if (current($manyTable->getColumns())->getName() == $localColumn) {
            $associationMapping['inversedBy'] = $this->getFieldNameForColumn($manyTable->getName(), current($ownerFk->getColumns()), TRUE);
            $associationMapping['joinTable'] = array(
              'name' => strtolower($manyTable->getName()),
              'joinColumns' => array(),
              'inverseJoinColumns' => array(),
            );

            $fkCols = $ownerFk->getForeignColumns();
            $cols = $ownerFk->getColumns();
            for ($i = 0; $i < count($cols); $i++) {
              $associationMapping['joinTable']['joinColumns'][] = array(
                'name' => $cols[$i],
                'referencesColumnName' => $fkCols[$i],
              );
            }

            $fkCols = $otherFk->getForeignColumns();
            $cols = $otherFk->getColumns();
            for ($i = 0; $i < count($cols); $i++) {
              $associationMapping['joinTable']['inverseJoinColumns'][] = array(
                'name' => $cols[$i],
                'referencedColumnName' => $fkCols[$i],
              );
            }
          }
          else {
            $associationMapping['mappedBy'] = $this->getFieldNameForColumn($manyTable->getName(), current($ownerFk->getColumns()), TRUE);
          }
          $metadata->mapManyToMany($associationMapping);
          break;
        }
      }
    }

    foreach ($foreignKeys as $foreignKey) {
      // A foreign key in schema can represent either a OneToOne relationship
      // or a ManyToOne relationship. What is expressed is: "This entity has a
      // property that is a reference to an instance of another entity".
      $foreignTable = $foreignKey->getForeignTableName();
      $cols = $foreignKey->getColumns();
      $fkCols = $foreignKey->getForeignColumns();

      if (empty($this->classNamesForTables[$foreignTable])) {
        // Do not associate to another entity when not defined in the info array.
        continue;
      }

      $localColumn = current($cols);
      $associationMapping = array();
      $associationMapping['fieldName'] = $this->getFieldNameForColumn($tableName, $localColumn, TRUE, TRUE);
      $associationMapping['targetEntity'] = $this->classNamesForTables[$foreignTable];

      if ($primaryKeyColumns && in_array($localColumn, $primaryKeyColumns)) {
        $associationMapping['id'] = TRUE;
      }

      for ($i = 0; $i < count($cols); $i++) {
        $associationMapping['joinColumns'][] = array(
          'name' => $cols[$i],
          'referencedColumnName' => $fkCols[$i],
        );
      }

      // Checks if $cols are the same as $primaryKeyColumns.
      if (!array_diff($cols, $primaryKeyColumns)) {
        $metadata->mapOneToOne($associationMapping);
      }
      else {
        $metadata->mapManyToOne($associationMapping);
      }
    }
  }

  /**
   * Return the columns involved in a constraint.
   *
   * In the case of a composite foreign key, multiple columns can be returned.
   *
   * @param array $foreignKeys
   *
   * @return \Doctrine\DBAL\Schema\ForeignKeyConstraint
   */
  protected function getAllForeignKeyColumns($foreignKeys) {
    $allForeignKeyColumns = array();

    foreach ($foreignKeys as $foreignKey) {
      // Merges local columns with foreign key columns to support composite
      // foreign keys. Foreign keys represents constraints while columns the
      // physical relationship between the two tables.
      $allForeignKeyColumns = array_merge($allForeignKeyColumns, $foreignKey->getLocalColumns());
    }

    return $allForeignKeyColumns;
  }

  /**
   * Return camelized version of the field name for column.
   *
   * @param string $tableName
   * @param string $columnName
   * @param boolean $fk Whether the column is a foreign key or not.
   * @param boolean $toOne Whether the association is a toOne or not.
   *
   * @return string
   */
  protected function getFieldNameForColumn($tableName, $columnName, $fk = FALSE, $toOne = FALSE) {
    $columnName = strtolower($columnName);

    if ($fk) {
      // Renames if it is a foreign key column.
      foreach ($this->tables[$tableName]->getForeignKeys() as $foreignKey) {
        if (in_array($columnName, $foreignKey->getLocalColumns())) {
          // I wish I could just add an 's' to the table name but Drupal defines
          // tables with an 's' already (users for instance).
          $tableName = $foreignKey->getForeignTableName();
          $columnName = $toOne ? Inflect::singularize($tableName) : Inflect::pluralize($tableName);
          break;
        }
      }
    }

    return Inflector::camelize($columnName);
  }

  /**
   * {@inheritDoc}
   */
  public function getAllClassNames() {
    return array_keys($this->classToTableNames);
  }

  /**
   * {@inheritDoc}
   */
  public function isTransient($className) {
    return TRUE;
  }
}

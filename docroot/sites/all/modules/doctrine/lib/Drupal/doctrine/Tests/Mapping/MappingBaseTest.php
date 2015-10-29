<?php

namespace Drupal\doctrine\Tests\Mapping;

/**
 * Mapping Base Test class.
 *
 * The mapping base test class uses a Builder pattern to configure entity info
 * and schema array. It does not support compound keys as Drupal schema usually
 * does not make use of it.
 *
 * By convention, entity is lowercase in this class as it represents the schema
 * name in the physical data model.
 */
class MappingBaseTest extends \DrupalUnitTestCase {

  /**
   * Entity infos.
   *
   * @var array
   */
  protected $entityInfo = array();

  /**
   * Schema infos.
   *
   * @var array
   */
  protected $schema;

  /**
   * Current entity being built.
   *
   * @var string
   */
  protected $entity;

  protected function setUp() {
    // Registry does not exists in a Unit Test context.
    spl_autoload_unregister('drupal_autoload_class');
    spl_autoload_unregister('drupal_autoload_interface');
    drupal_load('module', 'classloader');
    classloader_init();

    $this->entityInfo = array();
    $this->schema = array();
    $this->entity = NULL;

    parent::setUp();
  }

  /**
   * Creates an entity.
   *
   * This has the effect to set the current entity.
   *
   * @param string $entity
   *   Name of entity.
   *
   * @return \Drupal\doctrine\Tests\Mapping\MappingBaseTest
   */
  public function createEntity($entity, $class) {
    $this->entity = $entity;

    $this->schema[$this->entity] = array(
      // Name is added by the Schema API.
      'name' => $this->entity,
      'description' => "Stores {$this->entity} data.",
      'fields' => array(),
    );

    $this->entityInfo[$this->entity] = array(
      'label' => ucfirst($this->entity),
      'base table' => $this->entity,
      'entity class' => $class,
    );

    return $this;
  }

  /**
   * Sets a primary key on the current entity.
   *
   * Compound primary key is not supported for this use case.
   *
   * @param string $key
   *   The primary key field.
   *
   * @return \Drupal\doctrine\Tests\Mapping\MappingBaseTest
   */
  public function setPrimaryKey($key) {
    $this->schema[$this->entity]['fields'][$key] = array(
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => TRUE,
      'description' => 'Primary Key: Unique entity ID.',
      'default' => 0,
    );
    $this->schema[$this->entity]['primary key'] = array($key);
    $this->entityInfo[$this->entity]['entity keys']['id'] = $key;

    return $this;
  }

  /**
   * Sets a field in the current schema definition.
   *
   * @param string $name
   *   The name of the field.
   * @param array $definition
   *   The definition of the field.
   *
   * @return \Drupal\doctrine\Tests\Mapping\MappingBaseTest
   */
  public function setField($name, $definition = array()) {
    if (empty($definition)) {
      $definition = array(
        'type' => 'int',
        'not null' => TRUE,
        'default' => 0,
      );
    }

    $this->schema[$this->entity]['fields'][$name] = $definition;

    return $this;
  }

  /**
   * Sets a foreign key from the current entity to the target entity candidate key.
   *
   * Compound foreign key is not supported for this use case.
   *
   * @param string $field
   *   The field on which the constraint will be applied.
   * @param string $target
   *   The target entity.
   * @param string $candidateKey
   *   The candidate key.
   *
   * @return \Drupal\doctrine\Tests\Mapping\MappingBaseTest
   */
  public function setForeignKey($field, $target, $candidateKey) {
    if (empty($this->schema[$this->entity]['fields'][$field])) {
      // Creates a default field in case it does not yet exist. This is a short
      // cut to the tester avoiding him creating a field if only the relation
      // ship and not the type matters.
      $this->setField($field);
    }

    $this->schema[$this->entity]['foreign keys'][$field] = array(
      'table' => $target,
      'columns' => array($field => $candidateKey),
    );

    return $this;
  }

  /**
   * Sets a unique constraint on the field.
   *
   * @param string $key
   *   Name of the field.
   * @param string $name
   *   Name of the constraint.
   *
   * @return \Drupal\doctrine\Tests\Mapping\MappingBaseTest
   */
  public function setUnique($key, $name = NULL) {
    if (empty($this->schema[$this->entity]['fields'][$key])) {
      throw new \RuntimeException(sprintf("Column '%s' does not exists in entity: '%s'.", $key, $this->entity));
    }

    // Ensures key is an array.
    $key = is_array($key) ? $key : array($key);
    // Uses a dummy name unless specified.
    $name = empty($name) ? current($key) : $name;

    $this->schema[$this->entity]['unique keys'] = array($name => $key);

    return $this;
  }
}

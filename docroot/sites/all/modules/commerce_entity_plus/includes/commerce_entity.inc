<?php

/**
 * @file
 * Base class for Commerce Entity+ classes
 */

/**
 * Interface CommerceEntityInterface
 */
interface CommerceEntityInterface {
  /**
   * Returns entity type.
   *
   * @return string
   */
  public function getEntityType();
}

/**
 * Class CommerceEntity
 */
abstract class CommerceEntity extends Entity implements CommerceEntityInterface {

  /**
   * The created timestamp.
   *
   * @var string
   */
  public $created;

  /**
   * The changed timestamp.
   *
   * @var string
   */
  public $changed;

  /**
   * Data array stashed in most Commerce entities.
   *
   * @var array
   */
  public $data;

  /**
   * {@inheritdoc}
   */
  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($values, $this->getEntityType());
    // Make sure to initialize wrapper immediately.
    $this->wrapper();
  }

  /**
   * Returns entity value.
   *
   * @return mixed
   */
  public function getEntity() {
    return $this->wrapper()->value();
  }

  /**
   * Returns the type of the entity.
   */
  public function entityType() {
    return $this->getEntityType();
  }

  /**
   * @return string
   */
  public function getCreated() {
    return $this->created;
  }

  /**
   * @return string
   */
  public function getChanged() {
    return $this->changed;
  }

  /**
   * @return array
   */
  public function getData($key = NULL) {
    if ($key !== NULL) {
      return (isset($this->data[$key])) ? $this->data[$key] : NULL;
    }
    return $this->data;
  }

  /**
   * Helper method to get entity key data.
   *
   * @param string $key
   *
   * @return string
   */
  protected function getEntityKey($key) {
    return $this->entityInfo['entity keys'][$key];
  }

  /**
   * Attempts to resolve object into entity data.
   *
   * @param $data
   *
   * @return \stdClass
   * @throws \CommerceException
   */
  protected function resolveEntity($data) {
    $resolvedData = NULL;
    if ($data instanceof stdClass) {
      $resolvedData = $data;
    }
    else if ($data instanceof EntityDrupalWrapper) {
      $resolvedData = $data->value();
    }
    else if ($data instanceof CommerceEntity) {
      $resolvedData = $data->getEntity();
    }
    else {
      throw new CommerceException(t('Unknown entity class type'));
    }
    return $resolvedData;
  }

  /**
   * Wraps around EntityMetadataWrapper's get method.
   *
   * Mostly because doc block in Entity module doesn't type hint.
   *
   * @param $name
   * @return EntityMetadataWrapper
   */
  protected function wrapperGet($name) {
    return $this->wrapper()->get($name);
  }

  /**
   * Return entity ID if being casted to a string.
   */
  function __toString() {
    return $this->identifier();
  }
}

<?php
/**
 * @file
 * Defines \Drupal\aws_glacier\Entity\GlacierEntity
 */

namespace Drupal\aws_glacier\Entity;

use Drupal\aws_glacier\Exception\ExistsException;

/**
 * Class GlacierEntity
 * @package Drupal\aws_glacier\Entity
 */
abstract class GlacierEntity extends \Entity {

  /**
   * Primary Id.
   * @var int
   */
  public $id = 0;

  /**
   * @var string $uniqueProperty
   * The name of the property which is like a primary key.
   *
   */
  public $uniqueProperty;

  /**
   * @var bool A flag which indicates if the entity was validated or not.
   */
  public $uniquePropertyValidated = FALSE;

  /**
   * {@inheritDoc}
   */
  public function defaultLabel() {
    $property = $this->uniqueProperty;
    if (!isset($this->$property)) {
      return $this->entityInfo['label'];
    }
    return $this->$property;
  }

  /**
   * Validates that the new/changed value of the unique property is not already
   * taken by a different entity.
   *
   * @throws ExistsException
   */
  public function validateUniqueProperty() {
    $count = $this->loadByUniqueProperty(TRUE);
    $this->uniquePropertyValidated = TRUE;
    if ($count) {
      $entity_wrapper = entity_metadata_wrapper($this->entityType, $this);
      $info = $entity_wrapper->{$this->uniqueProperty}->info();
      throw new ExistsException(t('The value @value of @field_label is already in use.',
        array('@field_label' => $info['label'], '@value' => $this->{$this->uniqueProperty})
      ));
    }
  }

  /**
   * {@inheritDoc}
   */
  public function save() {
    if (!$this->uniquePropertyValidated) {
      try{
        $this->validateUniqueProperty();
      }
      catch (\Exception $e) {
        throw $e;
      }
    }
    $saved_entity = parent::save();
    $label = entity_label($this->entityType, $this);
    if ((!empty($this->is_new)) && $saved_entity === SAVED_NEW) {
      watchdog('aws_glacier', '@entity_type: @entity was created.', array('@entity_type' => $this->entityType, '@entity' => $label));
    }
    elseif($saved_entity === SAVED_UPDATED) {
      watchdog('aws_glacier', '@entity_type: @entity was changed.', array('@entity_type' => $this->entityType, '@entity' => $label));
    }
    return $saved_entity;
  }

  /**
   * @return int | $this
   */
  public function loadByUniqueProperty($count = FALSE) {
    $query = new \EntityFieldQuery();
    $query->entityCondition('entity_type', $this->entityType);
    $query->propertyCondition($this->uniqueProperty, $this->{$this->uniqueProperty});
    if ($count) {
      $query->count();
      if ($this->id) {
        $query->propertyCondition('id', $this->id, '<>');
      }
      return $query->execute();
    }
    $results = $query->execute();
    if (!empty($results)) {
      return entity_load_single($this->entityType(), reset($results[$this->entityType])->id);
    }
    return $this;
  }
}

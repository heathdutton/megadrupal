<?php

/**
 * @file
 * Abstract class that defines a default MaPS System® entity" (object, media, link, ...).
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem;

use Drupal\maps_import\Cache\Object\Profile as CacheProfile;
use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Mapping\Mapper\Mapper;
use Drupal\maps_import\Profile\Profile;
use Drupal\maps_suite\Log\Context\Context as LogContext;

abstract class Entity implements EntityInterface {

  /**
   * The profile instance.
   *
   * @var Profile
   */
  protected $profile;

  /**
   * The entity attributes.
   *
   * @var array
   */
  protected $attributes = array();

  /**
   * The entity classes.
   *
   * @var array
   */
  protected $classes = array();

  /**
   * The related entities.
   *
   * @var array
   */
  protected $relatedEntities = array();

  /**
   * The treated values by language
   *
   * @var array
   */
  protected $rawValues = array();

  /**
   * The parent entity.
   *
   * @var Entity
   */
  protected $parentEntity;

  /**
   * The Unix timestamp when the entity was updated.
   *
   * @var int
   */
  protected $updated = NULL;

  /**
   * @inheritdoc
   */
  public function __construct(array $entity) {
    $this->id = $entity['id'];
    $this->code = !empty($entity['code']) ? $entity['id'] : NULL;

    $this->profile = CacheProfile::getInstance()->loadSingle($entity['pid']);

    foreach (array('classes', 'attributes') as $key) {
      if (isset($entity[$key])) {
        $this->{$key} = unserialize($entity[$key]);
      }
    }

    $this->addRelatedEntity($entity);
  }

  /**
   * @inheritdoc
   */
  public function getProfile() {
    return $this->profile;
  }

  /**
   * @inheritdoc
   */
  public function addRelatedEntity(array $entity) {
    if (!isset($this->relatedEntities['id'])) {
      $this->relatedEntities[$entity['id']] = $this->cleanup($entity);
    }
  }

  /**
   * @inheritdoc
   */
  public function isPublished() {
    foreach ($this->relatedEntities as $entity) {
      if (!isset($entity['status']) || (int) $this->getProfile()->getPublishedState($entity['status']) == (int) Profile::STATE_PUBLISHED) {
        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Remove useless entries from the $entity array.
   */
  protected function cleanup(array $entity) {
    foreach (array('attributes') as $key) {
      if (isset($entity[$key])) {
        unset($entity[$key]);
      }
    }

    return $entity;
  }

  /**
   * @inheritdoc
   */
  public function setRawValues(PropertyWrapperInterface $property, $options = array(), ConverterInterface $currentConverter, $log = TRUE) {
    if (!array_key_exists($property->getKey(), $this->rawValues)) {
      $this->rawValues[$property->getKey()] = array();
    }

    // Create context if log is set at TRUE.
    if ($log) {
      Mapper::log()->addContext(new LogContext($property->getKey()), 'child');
    }

    $this->rawValues[$property->getKey()] = $property->extractValues($this, $options, $currentConverter);

    // Fill context in log.
    if (!$log) {
      return;
    }

    if (empty($this->rawValues[$property->getKey()])) {
      Mapper::log()->moveToParent('properties');
      return;
    }

    foreach ($this->rawValues[$property->getKey()] as $language => $values) {
      if (empty($values)) {
        continue;
      }

      Mapper::log()
        ->addContext(new LogContext('value', array('language' => $language)), 'child');

      if (!is_array($values)) {
        $values = array($values);
      }

      foreach ($values as $value) {
        Mapper::log()->addMessage($value);
      }

      Mapper::log()->moveToParent($property->getKey());
    }

    Mapper::log()->moveToParent('properties');
  }

  /**
   * @inheritdoc
   */
  public function hasValues(PropertyWrapperInterface $property, $language_id = NULL) {
    $return = isset($this->rawValues[$property->getKey()]);
    if (is_null($language_id)) {
      return $return;
    }

    if (!$property->isTranslatable()) {
      $language_id = 0;
    }

    $return = $return && array_key_exists($language_id, $this->rawValues[$property->getKey()]);

    // Manage specific case of boolean attribute, because the empty function is
    // not very smart...
    if (get_class($property) === 'Drupal\maps_import\Mapping\Source\MapsSystem\Attribute\Bool') {
      return $return && isset($this->rawValues[$property->getKey()][$language_id]);
    }

    return $return && !empty($this->rawValues[$property->getKey()][$language_id]);
  }

  /**
   * @inheritdoc
   */
  public function getAttributes() {
    return $this->attributes;
  }

  /**
   * @inheritdoc
   */
  public function getRelatedEntities() {
    return $this->relatedEntities;
  }

  /**
   * @inheritdoc
   */
  public function getRawValues() {
    return $this->rawValues;
  }

  /**
   * Return the entity ID.
   *
   * @return int
   *   The entity ID.
   */
  public function getId() {
    return $this->id;
  }

  /**
   * Set the related entities.
   *
   * @param $relatedEntities
   *   The related entities.
   */
  public function setRelatedEntities(array $relatedEntities) {
    $this->relatedEntities = $relatedEntities;
  }

  /**
   * @inheritdoc
   */
  public function setUpdated($updated) {
    $this->updated = $updated;
  }

  /**
   * @inheritdoc
   */
  public function getUpdated() {
    return $this->updated ?: time();
  }

}

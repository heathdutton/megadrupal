<?php

/**
 * @file
 * Class that defines operation on MaPS Object's related entity.
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem\RelatedEntity;

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Mapping\Mapper\Mapper;
use Drupal\maps_import\Mapping\Source\MapsSystem\PropertyWrapper;
use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface;
use Drupal\maps_suite\Log\Context\Context as LogContext;

abstract class RelatedEntity extends PropertyWrapper {

  /**
   * The parent converter cid.
   *
   * @var int
   */
  private $parentId;

  /**
   * @inheritdoc
   */
  public function __construct(array $definition = array()) {
    parent::__construct($definition);
    if (isset($definition['parent_id'])) {
      $this->parentId = (int) $definition['parent_id'];
    }
  }

  /**
   * Return the parent id.
   *
   * @return int
   */
  public function getParentId() {
    return $this->parentId;
  }

  /**
   * @inheritdoc
   */
  public function getKey() {
    return $this->id;
  }

  /**
   * @inheritdoc
   */
  public function getGroupLabel() {
    return t('Related entity ');
  }

  /**
   * @inheritdoc
   */
  public function getTranslatedTitle() {
    return t('Related entity');
  }

  /**
   * @inheritdoc
   */
  public function extractValues(EntityInterface $entity, $options = array(), ConverterInterface $currentConverter) {
    $converter = maps_import_converter_load($options['id']);

    Mapper::log()
      ->addContext(new LogContext('related_entity'), 'child');

    // @todo  check Conditions ?
    $result = $converter->getMapping()->process($entity);

    if (!$result) {
      return array();
    }

    list(, $mapsEntity) = each($result);
    list(, $drupalEntity) = each($result);

    if (!$drupalEntity) {
      return array();
    }

    $identifiers = $drupalEntity->getIdentifiers();
    $identifier = reset($identifiers);

    Mapper::log()
      ->moveToParent('related_entity')
      ->moveToParent($this->getKey());

    return array($identifier);
  }

}

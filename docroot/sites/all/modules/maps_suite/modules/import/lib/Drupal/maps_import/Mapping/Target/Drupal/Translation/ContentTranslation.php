<?php

/**
 * @file
 * Manage "Content" translations on Drupal Entity.
 */

namespace Drupal\maps_import\Mapping\Target\Drupal\Translation;

use Drupal\maps_import\Mapping\Target\Drupal\EntityInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface as MapsEntityInterface;
use Drupal\maps_import\Mapping\Target\Drupal\Field\FieldInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\PropertyWrapperInterface;


class ContentTranslation extends Translation {

  /**
   * Store the translation ID.
   *
   * @var int
   */
  protected $translationId = 0;

  /**
   * @inheritdoc
   */
  public function __construct(EntityInterface $entity, array $existingEntities = array()) {
    $this->setEntity($entity);

    foreach (array_keys($entity->getProfile()->getLanguages()) as $languageId) {
      $info = !empty($existingEntities[$languageId]) ? $existingEntities[$languageId] : array();
      $entity->addEntity($languageId, $info);
    }
  }

  /**
   * @inheritdoc
   */
  public function initTranslation(\EntityDrupalWrapper $wrapper) {
    if (!$this->translationId && $wrapper->getIdentifier()) {
      $this->translationId = $wrapper->getIdentifier();
    }

    if ($this->translationId > 0) {
      $wrapper->value()->tnid = $this->translationId;
    }
  }

  /**
   * @inheritdoc
   */
  public function setValue(FieldInterface $field, PropertyWrapperInterface $property, MapsEntityInterface $mapsEntity, $required = FALSE) {
    // @todo Catch mapping exception.
  }


}

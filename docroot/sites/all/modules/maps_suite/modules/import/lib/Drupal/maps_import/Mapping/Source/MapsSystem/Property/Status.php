<?php

/**
 * @file
 * Define the "status" MaPS System® property.
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem\Property;

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface;

class Status extends Property {

	/**
   * @inheritdoc
   */
	protected $typeCode = 'int';

  /**
   * @inheritdoc
   */
  public function processValues($values, EntityInterface $entity, ConverterInterface $currentConverter) {
    foreach ($entity->getRelatedEntities() as $relatedEntity) {
      if ($entity->getProfile()->getPublishedState((int) $relatedEntity[$this->getId()]) === Profile::STATE_PUBLISHED) {
        return TRUE;
      }
    }

    return FALSE;
  }

}

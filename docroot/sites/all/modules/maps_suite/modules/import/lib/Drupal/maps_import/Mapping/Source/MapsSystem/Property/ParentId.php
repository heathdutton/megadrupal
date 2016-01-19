<?php

/**
 * @file
 * Define the "parent id" MaPS System® property.
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem\Property;

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface;
use Drupal\maps_import\Mapping\Mapping;

class ParentId extends Property {

  /**
   * @inheritdoc
   */
	protected $typeCode = 'int';

  /**
   * @inheritdoc
   * @todo use uid scope in request ?
   */
  public function processValues($values, EntityInterface $entity, ConverterInterface $currentConverter) {
    $return = array();

    foreach ($entity->getRelatedEntities() as $relatedEntity) {
      if ($found = Mapping::getEntityFromMapsId('object', $relatedEntity[$this->getId()], $currentConverter->getUidScope(), $currentConverter->getPid())) {
        $return = array_merge($return, $found);
      }
    }

    return $return ?: NULL;
  }

  /**
   * @inheritdoc
   *
   * There are different possible cases, depending on the multiple state
   * of the Drupal field.
   */
  public function sanitize($values) {
    $sanitized = array();

    if ($values && is_array($values)) {
      // Dealing with a single field.
      if (isset($values['entity_id'])) {
        // @check Do we have to check here the parent entity type ?
        $sanitized = $values['entity_id'];
      }
      else {
        foreach ($values as $value) {
          // @todo Manage multiple node references.
          if (isset($value['entity_id'])) {
            $sanitized[] = $value['entity_id'];
          }
        }
      }
    }

    return $sanitized ?: $values;
  }

}

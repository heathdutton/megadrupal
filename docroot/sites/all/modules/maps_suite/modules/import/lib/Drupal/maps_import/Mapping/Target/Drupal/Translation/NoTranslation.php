<?php

/**
 * @file
 * Manage "NO" translations on Drupal Entity.
 */

namespace Drupal\maps_import\Mapping\Target\Drupal\Translation;

use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface as MapsEntityInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\PropertyWrapperInterface;
use Drupal\maps_import\Mapping\Target\Drupal\EntityInterface;
use Drupal\maps_import\Mapping\Target\Drupal\Field\FieldInterface;
use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Mapping\Mapper\Mapper;
use Drupal\maps_suite\Log\Context\Context as LogContext;


class NoTranslation extends Translation {

  /**
   * @inheritdoc
   */
  public function __construct(EntityInterface $entity, array $existingEntities = array()) {
    $this->setEntity($entity);

    $info = !empty($existingEntities[EntityInterface::LANGUAGE_NONE]) ? $existingEntities[EntityInterface::LANGUAGE_NONE] : array();
    $this->getEntity()->addEntity(EntityInterface::LANGUAGE_NONE, $info);
  }

  /**
   * @inheritdoc
   */
  public function setValue(FieldInterface $field, PropertyWrapperInterface $property, MapsEntityInterface $mapsEntity, $required = FALSE) {
    $defaultLanguage = $mapsEntity->getProfile()
      ->getLanguage(language_default()->language);

    // If the mapping item and there is no value for this property.
    if ($required && (!$mapsEntity->hasValues($property, $defaultLanguage) && !$mapsEntity->hasValues($property, Profile::MAPS_SUITE_MAPS_SYSTEM_LANGUAGE_DEFAULT_ID))) {
      return FALSE;
    }

    // Get the values from MaPS Entity.
    if ($mapsEntity->hasValues($property, $defaultLanguage)) {
      $values = $property->getValues($mapsEntity, $defaultLanguage);
    }
    else {
      if ($mapsEntity->hasValues($property, Profile::MAPS_SUITE_MAPS_SYSTEM_LANGUAGE_DEFAULT_ID)) {
        $values = $property->getValues($mapsEntity, Profile::MAPS_SUITE_MAPS_SYSTEM_LANGUAGE_DEFAULT_ID);
      }
      else {
        return !$required;
      }
    }

    // Sanitize values.
    if ($property->isMultiple()) {
      if ($field->isMultiple()) {
        foreach ($values as $id => $value) {
          $values[$id] = $field->sanitize($value);
        }
      }
      else {
        $values = is_array($values) ? $field->sanitize(reset($values)) : $field->sanitize($values);
      }
    }
    else {
      $values = $field->sanitize($values);
      // We check if it is a text formatted value.
      if (is_array($values) && !$field->isMultiple()) {
        if (!isset($values['value']) && !isset($values['format'])) {
          $values = reset($values);
        }
        else {
          $values['value'] = reset($values['value']);
        }
      }
    }

    if ((!is_null($values) && !$property->isMultiple())
      || !empty($values) && $property->isMultiple()
    ) {
      Mapper::log()->addContext(new LogContext($field->getId()), 'child');

      $wrapper = $this->getEntity()->getEntities();
      $wrapper = reset($wrapper);

      // Set the values in the wrapper.
      try {
        $wrapper->{$field->getId()}->set($property->sanitize($values));

        $message = (is_array($values) && isset($values['value'])) ? $values['value'] : $values;
        Mapper::log()
          ->addContext(new LogContext('value'), 'child')
          ->addMessage($message);
      } catch (\EntityMetadataWrapperException $e) {
        Mapper::log()->addContext(new LogContext('error'), 'child');

        if ($required) {
          Mapper::log()->addMessageFromException($e);
          return FALSE;
        }
        else {
          Mapper::log()
            ->addMessage($e->getMessage(), array('level' => WATCHDOG_WARNING));
        }
      }
    }
    else {
      if ($required) {
        return FALSE;
      }
    }

    Mapper::log()->moveToParent('fields');

    return TRUE;
  }

}

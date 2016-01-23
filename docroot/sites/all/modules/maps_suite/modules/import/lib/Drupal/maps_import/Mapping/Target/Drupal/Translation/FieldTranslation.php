<?php

/**
 * @file
 * Manage "Field" translations on Drupal Entity.
 */

namespace Drupal\maps_import\Mapping\Target\Drupal\Translation;

use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface as MapsEntityInterface;
use Drupal\maps_import\Mapping\Target\Drupal\EntityInterface;
use Drupal\maps_import\Mapping\Target\Drupal\Field\FieldInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\PropertyWrapperInterface;
use Drupal\maps_import\Exception\MappingException;
use Drupal\maps_import\Mapping\Mapper\Mapper;
use Drupal\maps_suite\Log\Context\Context as LogContext;

class FieldTranslation extends Translation {

  /**
   * @inheritdoc
   */
  public function __construct(EntityInterface $entity, array $existingEntities = array()) {
    $this->setEntity($entity);

    $defaultLanguage = $entity->getProfile()->getDefaultLanguage();

    if (!$defaultLanguage) {
      throw new MappingException('There is no default language.');
    }

    $info = !empty($existingEntities[$defaultLanguage]) ? $existingEntities[$defaultLanguage] : array();

    $this->getEntity()->addEntity($defaultLanguage, $info);
  }

  /**
   * @inheritdoc
   */
  public function setValue(FieldInterface $field, PropertyWrapperInterface $property, MapsEntityInterface $mapsEntity, $required = FALSE) {
    // There is only one entity in Field translation.
    $wrapper = $this->getEntity()->getEntities();
    $wrapper = reset($wrapper);

    $wrapper_language = $wrapper->getPropertyLanguage();

    // Retrieve values for given property in the MaPS entity.
    if (!$mapsEntity->hasValues($property)) {
      return !$required;
    }
    $values = $property->getValues($mapsEntity);

    $mapped_statuses = array_filter($mapsEntity->getProfile()->getLanguages());

    // Sanitize values.
    foreach ($values as $languageId => $translatedValue) {
      if ($property->isMultiple()) {
        if ($field->isMultiple()) {
          foreach ($translatedValue as $id => $value) {
            $values[$languageId][$id] = $field->sanitize($value);
          }
        }
        else {
          // If the property is multiple but not the field, we only use the first value of the property.
          $values[$languageId] = is_array($translatedValue) ? $field->sanitize(reset($translatedValue)) : $field->sanitize($translatedValue);
        }
      }
      else {
        $value = $field->sanitize($translatedValue);
        $values[$languageId] = $value;
        // Get only the first value if the field is not multiple.
        if (is_array($value) && !$field->isMultiple()) {
          // Check if it is a text formatted or not.
          if (!isset($value['value']) && !isset($value['format'])) {
            $values[$languageId] = reset($value);
          }
          else {
            $value['value'] = reset($value['value']);
            $values[$languageId] = $value;
          }
        }
      }
    }

    // Check if there are values.
    if ((!is_null($values) && !$property->isMultiple())
      || !empty($values) && $property->isMultiple()
    ) {
      global $language;

      Mapper::log()->addContext(new LogContext($field->getId()), 'child');

      // Ensure that we have values for the default language.
      if (!isset($values[$mapsEntity->getProfile()
            ->getLanguage($language->language)]) && !isset($values[0])
      ) {
        return !$required;
      }

      foreach ($values as $idLanguage => $value) {
        $required_by_lang = $required && ($mapsEntity->getProfile()
              ->getLangcode($idLanguage) == $language->language);

        // Avoid processing values for unmanaged languages.
        if (!isset($mapped_statuses[$idLanguage]) && $idLanguage != 0) {
          if ($required_by_lang) {
            return FALSE;
          }
          continue;
        }

        // Avoid processing null values.
        if (is_null($value) ||
          is_array($value) && isset($value['value']) && is_null($value['value'])
        ) {
          if ($required_by_lang) {
            return FALSE;
          }
          continue;
        }

        try {
          // Retrieve langcode
          $langcode = $mapsEntity->getProfile()->getLangcode($idLanguage);

          if (!is_null($langcode)) {
            $wrapper->language($langcode);
          }
          $value = $property->sanitize($value);

          // Set value.
          $wrapper->{$field->getId()}->set($value);

          // Initialize the wrapper language.
          $wrapper->language($wrapper_language->language);

          $message = (is_array($values) && isset($values['value'])) ? $values['value'] : $values;
          Mapper::log()
            ->addContext(new LogContext('value'), 'child')
            ->addMessage($message);
        } catch (\EntityMetadataWrapperException $e) {
          Mapper::log()->addContext(new LogContext('error'), 'child');

          if ($required_by_lang) {
            Mapper::log()->addMessageFromException($e);
            return FALSE;
          }
          else {
            Mapper::log()
              ->addMessage($e->getMessage(), array('level' => WATCHDOG_WARNING));
          }
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

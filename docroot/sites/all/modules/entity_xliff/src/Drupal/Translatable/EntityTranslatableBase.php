<?php

/**
 * @file
 * Base class for making entities translatable.
 */

namespace EntityXliff\Drupal\Translatable;

use EggsCereal\Utils\Data;
use EntityXliff\Drupal\Factories\EntityTranslatableFactory;
use EntityXliff\Drupal\Interfaces\EntityTranslatableInterface;
use EntityXliff\Drupal\Mediator\EntityMediator;
use EntityXliff\Drupal\Mediator\FieldMediator;
use EntityXliff\Drupal\Utils\DrupalHandler;


/**
 * Class EntityTranslatableBase
 */
abstract class EntityTranslatableBase implements EntityTranslatableInterface  {

  /**
   * @var \EntityDrupalWrapper
   */
  protected $entity;

  /**
   * @var DrupalHandler
   */
  protected $drupal;

  /**
   * @var array
   */
  protected $entityInfo;

  /**
   * @var EntityTranslatableFactory
   */
  protected $translatableFactory;

  /**
   * @var FieldMediator
   */
  protected $fieldMediator;

  /**
   * @var \EntityDrupalWrapper[]
   */
  protected $targetEntities = array();

  /**
   * An array of entity metadata wrappers for which data has been updated and
   * should be saved.
   *
   * @var \EntityDrupalWrapper[]
   */
  protected $entitiesNeedSave = array();

  /**
   * @var EntityTranslatableInterface[]
   */
  protected $translatables = array();

  /**
   * Creates a Translatable from an Entity wrapper.
   *
   * @param \EntityDrupalWrapper $entityWrapper
   *   Metadata wrapper for the entity we wish to translate.
   *
   * @param DrupalHandler $handler
   *   (Optional) Inject the utility Drupal handler.
   *
   * @param EntityTranslatableFactory $factory
   *   (Optional) Inject the entity translatable factory.
   *
   * @param FieldMediator $fieldMediator
   *   (Optional) Inject the field mediator.
   */
  public function __construct(\EntityDrupalWrapper $entityWrapper, DrupalHandler $handler = NULL, EntityTranslatableFactory $factory = NULL, FieldMediator $fieldMediator = NULL) {
    // If no Drupal Handler was provided, instantiate it manually.
    if ($handler === NULL) {
      $handler = new DrupalHandler();
    }

    // If no translatable factory was provided, instantiate it manually.
    if ($factory === NULL) {
      $factory = EntityTranslatableFactory::getInstance($handler);
    }

    if ($fieldMediator === NULL) {
      $fieldMediator = new FieldMediator($handler);
    }

    $this->entity = $entityWrapper;
    $this->drupal = $handler;
    $this->translatableFactory = $factory;
    $this->fieldMediator = $fieldMediator;
    $this->entityInfo = $handler->entityGetInfo();
  }

  /**
   * {@inheritdoc}
   */
  public function getIdentifier() {
    return $this->entity->getIdentifier();
  }

  /**
   * {@inheritdoc}
   */
  public function getLabel() {
    return $this->entity->label();
  }

  /**
   * {@inheritdoc}
   */
  public function getSourceLanguage() {
    $language = $this->entity->language->value();
    if ($language === DrupalHandler::LANGUAGE_NONE || empty($language)) {
      $language = $this->drupal->languageDefault('language');
    }
    return $language;
  }

  /**
   * {@inheritdoc}
   */
  public function getData() {
    $data = array();
    $fields = $this->getTranslatableFields();

    // Allow modules to alter translatable fields for this entity.
    $this->drupal->entityXliffLoadModuleIncs();
    $this->drupal->alter('entity_xliff_translatable_fields', $fields, $this->entity);

    // Iterate through all fields we're expecting to translate.
    foreach ($fields as $field) {
      if ($fieldData = $this->getFieldFromEntity($this->entity, $field)) {
        $data[$field] = $fieldData;
      }
    }

    return $data;
  }

  /**
   * {@inheritdoc}
   *
   * Adds optional parameter $saveData, mostly used internally.
   */
  public function setData(array $data, $targetLanguage, $saveData = TRUE) {
    // Add translated data.
    $this->addTranslatedDataRecursive($data, array(), $targetLanguage);

    // Do not proceed to saving the data if specified.
    if (!$saveData) {
      return;
    }

    // Attempt to initialize translation.
    $this->initializeTranslation();

    // Save any entities that need saving (this includes the target entity).
    foreach ($this->entitiesNeedSave as $key => $wrapper) {
      $translatable = $this->translatableFactory->getTranslatable($wrapper);
      $translatable->initializeTranslation();
      $type = $wrapper->type();
      $this->drupal->alter('entity_xliff_presave', $wrapper, $type);
      $translatable->saveWrapper($wrapper, $targetLanguage);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function getTranslatableFields() {
    $fields = array();

    // If the wrapped entity is not translatable, return no fields.
    if (!$this->isTranslatable()) {
      return $fields;
    }

    foreach ($this->entity->getPropertyInfo() as $property => $info) {
      if (isset($info['field']) && $info['field']) {
        $fields[] = $property;
      }
    }
    return $fields;
  }

  /**
   * {@inheritdoc}
   */
  public function saveWrapper(\EntityDrupalWrapper $wrapper, $targetLanguage) {
    $wrapper->save();
  }

  /**
   * @param $translation
   * @param array $key
   * @param string $targetLang
   */
  protected function addTranslatedDataRecursive($translation, array $key = array(), $targetLang) {
    if (isset($translation['#text'])) {
      $values = array(
        '#translation' => $translation,
      );
      $this->setItem($key, $values, $targetLang);
      return;
    }
    foreach ($this->drupal->elementChildren($translation) as $item) {
      $this->addTranslatedDataRecursive($translation[$item], array_merge($key, array($item)), $targetLang);
    }
  }

  /**
   * Updates the values for a specific substructure in the data array.
   *
   * The values are either set or updated but never deleted.
   *
   * @param $key
   *   Key pointing to the item the values should be applied.
   *   The key can be either be an array containing the keys of a nested array
   *   hierarchy path or a string with '][' or '|' as delimiter.
   * @param $values
   *   Nested array of values to set.
   * @param string $targetLang
   *   The target language of the item being set.
   */
  public function setItem($key, $values = array(), $targetLang) {
    foreach ($values as $index => $value) {
      // In order to preserve existing values, we can not apply the values array
      // at once. We need to apply each containing value on its own.
      // If $value is an array we need to advance the hierarchy level.
      if (is_array($value)) {
        $this->setItem(array_merge(Data::ensureArrayKeys($key), array($index)), $value, $targetLang);
      }
      // Apply the value.
      else {
        // Do not bother setting empty values.
        $trimmed = trim($value);
        if (!empty($trimmed)) {
          // Get the list of relevant array keys via the xliff serializer.
          $arrayKeys = Data::ensureArrayKeys($key);
          array_pop($arrayKeys);

          // Set the value via a method inspired by drupal's nested array setter.
          $this->entitySetNestedValue($this->getTargetEntity($targetLang), $arrayKeys, $trimmed, $targetLang);
        }
      }
    }
  }

  /**
   * @param \EntityDrupalWrapper $wrapper
   * @param string $field
   * @param int $delta
   * @return array
   */
  public function getFieldFromEntity(\EntityDrupalWrapper $wrapper, $field, $delta = NULL) {
    $response = array();
    $fieldWrapper = $delta !== NULL ? $wrapper->{$field}[$delta] : $wrapper->{$field};
    $fieldInfo = $fieldWrapper->info();
    $type = isset($fieldInfo['type']) ? $fieldInfo['type'] : 'text';

    // Check for getters against known types.
    if ($handler = $this->fieldMediator->getInstance($fieldWrapper)) {
      if ($text = $handler->getValue($fieldWrapper)) {
        if (is_array($text)) {
          $response = $text;
        }
        else {
          $response['#label'] = $fieldInfo['label'];
          $response['#text'] = $text;
        }
      }
    }
    // If this is an entity reference, restart the process recursively with the
    // referenced entity as the starting point.
    elseif(isset($this->entityInfo[$type])) {
      $response += $this->getEntityFromEntity($fieldWrapper);
    }
    // If this is a list, call ourselves recursively for each item.
    elseif (preg_match('/list<(.*?)>/', $type, $matches)) {
      foreach ($fieldWrapper->getIterator() as $delta => $subFieldWrapper) {
        if ($text = $this->getFieldFromEntity($wrapper, $field, $delta)) {
          $response[$delta] = $text;
        }
      }
    }
    else {
      $this->drupal->watchdog('entity xliff', 'Could not pull translatable data. Unknown field type %type.', array(
        '%type' => $type,
      ), DrupalHandler::WATCHDOG_WARNING);
    }

    return $response;
  }

  /**
   * @param \EntityDrupalWrapper $wrapper
   * @return array
   */
  protected function getEntityFromEntity(\EntityDrupalWrapper $wrapper) {
    // Ensure that this wrapper represents real data, not just a placeholder
    // that has no data. Also make sure we know how to translate thie entity.
    if ($wrapper->getIdentifier() && $translatable = $this->translatableFactory->getTranslatable($wrapper)) {
      return $translatable->getData();
    }
    else {
      return array();
    }
  }

  /**
   * @param \EntityMetadataWrapper $wrapper
   * @param array $parents
   * @param $value
   * @param $targetLang
   */
  protected function entitySetNestedValue(\EntityMetadataWrapper $wrapper, array $parents, $value, $targetLang) {
    // Get the field reference.
    $ref = array_shift($parents);
    if (is_numeric($ref)) {
      $field = &$wrapper[$ref]; // @codeCoverageIgnoreStart
    } // @codeCoverageIgnoreEnd
    else {
      $field = &$wrapper->{$ref};
    }

    // Base case: we are setting a basic field on an entity.
    if (count($parents) === 0) {
      // Set the value on the field.
      if ($handler = $this->fieldMediator->getInstance($field)) {
        $handler->setValue($field, $value);

        // If this is an EntityDrupalWrapper, we need to mark the wrapper as needing
        // saved.
        if (is_a($wrapper, 'EntityDrupalWrapper')) {
          $targetId = $wrapper->getIdentifier();
          $targetType = $wrapper->type();

          // If this is a brand new entity, we need to initialize and save it first.
          if ($targetId === FALSE) {
            $translatable = $this->translatableFactory->getTranslatable($wrapper);
            $this->drupal->alter('entity_xliff_presave', $wrapper, $targetType);
            $translatable->saveWrapper($wrapper, $targetLang);
            $targetId = $wrapper->getIdentifier();
          }

          // Mark this entity as needing saved.
          $this->entitiesNeedSave[$targetType . ':' . $targetId] = $wrapper;
        }

        return TRUE;
      }
      else {
        return FALSE;
      }
    }
    // Recursive case. We need to go deeper.
    else {
      // Ensure we're always setting data against the target entity.
      if (is_a($field, 'EntityDrupalWrapper')) {
        $targetId = $field->getIdentifier();
        $targetType = $field->type();
        $needsSaveKey = $targetType . ':' . $targetId;

        // If the entity exists and we already have it in static cache, use it.
        if ($targetId && isset($this->entitiesNeedSave[$needsSaveKey])) {
          $field = $this->entitiesNeedSave[$needsSaveKey];
        }
        else {
          // Otherwise, ensure we're using the translation.
          if ($translatable = $this->translatableFactory->getTranslatable($field)) {
            // If the ref's source language is the target language ignore.
            if ($translatable->getSourceLanguage() !== $targetLang) {
              // Otherwise, get the target entity.
              $translatable->initializeTranslation();
              $field = $translatable->getTargetEntity($targetLang);
            }
          }
        }

        // If this is a new entity, we need to initialize and save it first.
        if ($targetId === FALSE) {
          $translatable = $this->translatableFactory->getTranslatable($field);
          $translatable->initializeTranslation();
          $this->drupal->alter('entity_xliff_presave', $field, $targetType);
          $translatable->saveWrapper($field, $targetLang);
          $targetId = $field->getIdentifier();
        }

        // Always attempt to pull the entity from static cache.
        $needsSaveKey = $targetType . ':' . $targetId;
        if (isset($this->entitiesNeedSave[$needsSaveKey])) {
          $field = $this->entitiesNeedSave[$needsSaveKey];
        }
      }

      // Attempt to set the nested value.
      $set = $this->entitySetNestedValue($field, $parents, $value, $targetLang);
      if ($set) {
        // If the child is an entity, we need to set the reference.
        if (is_a($field, 'EntityDrupalWrapper')) {
          $targetId = $field->getIdentifier();
          $targetType = $field->type();

          if (is_numeric($ref)) {
            // @codeCoverageIgnoreStart
            $vals = $wrapper->raw();
            $vals[$ref] = $field->getIdentifier();
            $wrapper->set($vals);
          } // @codeCoverageIgnoreEnd
          else {
            $wrapper->{$ref}->set($field->getIdentifier());
          }

          // Mark this entity as needing saved.
          $this->entitiesNeedSave[$targetType . ':' . $targetId] = $field;
        }
        elseif (is_a($field, 'EntityListWrapper')) {
          $needsSaveKey = $wrapper->type() . ':' . $wrapper->getIdentifier();
          if (!isset($this->entitiesNeedSave[$needsSaveKey])) {
            $this->entitiesNeedSave[$needsSaveKey] = $wrapper;
          }
        }
      }

      return $set;
    }
  }

  /**
   * Returns a given Entity wrapper's parent (if one exists).
   *
   * @param \EntityMetadataWrapper $child
   *   The child Entity Wrapper to check.
   *
   * @return \EntityDrupalWrapper|bool
   *   Returns the given entity's parent if one exists. FALSE if it has no
   *   parent.
   */
  public function getParent(\EntityMetadataWrapper $child) {
    $fieldInfo = $child->info();
    if (isset($fieldInfo['parent'])) {
      $parent = $fieldInfo['parent'];
      // It's possible the parent is a list wrapper. If so, find ITS parent.
      // Do so recursively 'til we get to a DrupalWrapper.
      if (!is_a($parent, 'EntityDrupalWrapper')) {
        return $this->getParent($parent);
      }
      else {
        return $parent;
      }
    }
    else {
      return FALSE;
    }
  }

  /**
   * Returns the raw entity object given its entity wrapper.
   *
   * @param \EntityDrupalWrapper $wrapper
   *   The Entity wrapper whose raw entity object is desired.
   *
   * @return mixed
   *   The raw entity object.
   */
  public function getRawEntity(\EntityDrupalWrapper $wrapper) {
    $raw = $wrapper->raw();
    if (!is_object($raw)) {
      $entities = $this->drupal->entityLoad($wrapper->type(), array((int) $raw));
      $raw = reset($entities);
    }
    return $raw;
  }

}

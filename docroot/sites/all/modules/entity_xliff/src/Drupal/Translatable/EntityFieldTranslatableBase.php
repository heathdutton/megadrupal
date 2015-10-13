<?php

/**
 * @file
 * Base class for making entities translatable using Entity Translation module.
 */

namespace EntityXliff\Drupal\Translatable;

use EntityXliff\Drupal\Utils\DrupalHandler;


/**
 * Class EntityFieldTranslatableBase
 */
abstract class EntityFieldTranslatableBase extends EntityTranslatableBase {

  /**
   * {@inheritdoc}
   */
  public function getSourceLanguage() {
    $rawEntity = $this->entity->raw();
    $type = $this->entity->type();
    $handler = $this->drupal->entityTranslationGetHandler($type, $rawEntity);
    $language = $handler->getLanguage();
    return $language;
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
      if (isset($info['field']) && $info['field'] && isset($info['translatable']) && $info['translatable']) {
        $fields[] = $property;
      }
    }
    return $fields;
  }

  /**
   * {@inheritdoc}
   */
  public function isTranslatable() {
    $type = $this->entity->type();
    $bundle = $this->entity->getBundle();
    return $this->drupal->entityTranslationEnabled($type, $bundle);
  }

  /**
   * {@inheritdoc}
   */
  public function getTargetEntity($targetLanguage) {
    if (!isset($this->targetEntities[$targetLanguage]) || empty($this->targetEntities[$targetLanguage])) {
      $target = clone $this->entity;
      $target->language->set($this->getSourceLanguage());
      $target->language($targetLanguage);
      $this->targetEntities[$targetLanguage] = $target;
    }
    return $this->targetEntities[$targetLanguage];
  }

  /**
   * {@inheritdoc}
   */
  public function initializeTranslation() {
    if ($this->entity->language->value() === DrupalHandler::LANGUAGE_NONE) {
      $sourceLanguage = $this->getSourceLanguage();
      $this->entity->language($sourceLanguage);
      $rawEntity = $this->getRawEntity($this->entity);
      $type = $this->entity->type();

      // Initialize translation for this entity.
      $handler = $this->drupal->entityTranslationGetHandler($type, $rawEntity);
      $handler->setOriginalLanguage($sourceLanguage);
      $handler->initOriginalTranslation();
      $handler->saveTranslations();

      $this->entity = $this->drupal->entityMetadataWrapper($type, $handler->getEntity());
    }
  }

  /**
   * {@inheritdoc}
   */
  public function saveWrapper(\EntityDrupalWrapper $wrapper, $targetLanguage) {
    $rawEntity = $this->getRawEntity($wrapper);
    $type = $wrapper->type();
    $handler = $this->drupal->entityTranslationGetHandler($type, $rawEntity);

    // Set the target language on the entity translation handler.
    $handler->setFormLanguage($targetLanguage);

    // Save the entity.
    $wrapper->save();

    // Set translation tidbits on the translation handler and save translations.
    $handler->setTranslation(array(
      'translate' => 0,
      'status' => TRUE,
      'language' => $targetLanguage,
      // @see EntityFieldTranslatableBase::initializeTranslation()
      'source' => $this->getSourceLanguage(),
    ));
    $handler->saveTranslations();
  }

}

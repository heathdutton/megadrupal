<?php

/**
 * @file
 * Defines a field collection translatable compatible with the nodes translated
 * by the core Translation module.
 */

namespace EntityXliff\Drupal\Translatable\Content;

use EntityXliff\Drupal\Translatable\EntityTranslatableBase;
use EntityXliff\Drupal\Utils\DrupalHandler;

/**
 * Class FieldCollectionTranslatable
 * @package EntityXliff\Drupal\Translatable
 */
class FieldCollectionTranslatable extends EntityTranslatableBase {

  /**
   * Static cached value of the source language for this field collection. It's
   * stored this way due to the expense associated with determining a host's
   * source language.
   *
   * @var string
   */
  protected $sourceLanguage = '';

  /**
   * {@inheritdoc}
   */
  public function getSourceLanguage() {
    // Load the source language once from the host entity.
    if (empty($this->sourceLanguage)) {
      $hostEntity = $this->getHostEntity($this->entity->value());
      $hostEntityType = $this->entity->value()->hostEntityType();
      $hostWrapper = $this->drupal->entityMetadataWrapper($hostEntityType, $hostEntity);
      $translatable = $this->translatableFactory->getTranslatable($hostWrapper);
      $this->sourceLanguage = $translatable->getSourceLanguage();
    }

    if ($this->sourceLanguage === DrupalHandler::LANGUAGE_NONE || empty($this->sourceLanguage)) {
      $this->sourceLanguage = $this->drupal->languageDefault('language');
    }

    return $this->sourceLanguage;
  }

  /**
   * {@inheritdoc}
   */
  public function getTargetEntity($targetLanguage) {
    if (!isset($this->targetEntities[$targetLanguage]) || empty($this->targetEntities[$targetLanguage])) {
      $target = $this->getRawEntity($this->entity);

      // Pull the parent/host entity.
      if ($rawHost = $this->getHostEntity($target)) {
        $host = $this->drupal->entityMetadataWrapper($target->hostEntityType(), $rawHost);
      }
      else {
        $host = $this->getParent($this->entity);
        $rawHost = $this->getRawEntity($host);
      }

      // If the language of the host does not match the specified target
      // language, then we need to create a new field collection. Otherwise,
      // we're just updating the existing field collection.
      if ($host->language->value() !== $targetLanguage) {
        unset($target->item_id, $target->revision_id);
        $target->is_new = TRUE;
        $target->setHostEntity($host->type(), $rawHost);
      }
      else {
        $target->is_new = TRUE;
        $target->setHostEntity($host->type(), $rawHost);
        $target->is_new = FALSE;
        $target->is_new_revision = FALSE;
        $target->default_revision = TRUE;
      }

      $this->targetEntities[$targetLanguage] = $this->drupal->entityMetadataWrapper('field_collection_item', $target);
    }

    return $this->targetEntities[$targetLanguage];
  }

  /**
   * {@inheritdoc}
   */
  public function isTranslatable() {
    // @todo Determine best way to do this based on host entity paradigm...
    return TRUE;
  }

  /**
   * {@inheritdoc}
   *
   * This should never be called anyway since a Field Collection will always be
   * saved in the context of a host entity. But just in case, this should be a
   * no-op for this entity type.
   */
  public function initializeTranslation() {}

  /**
   * {@inheritdoc}
   *
   * Do not save the host entity; that is taken care of elsewhere.
   */
  public function saveWrapper(\EntityDrupalWrapper $wrapper, $targetLanguage) {
    $wrapper->value()->save(TRUE);
  }

  /**
   * Returns a given field collection item entity's host entity.
   * @param \FieldCollectionItemEntity $fieldCollection
   * @return bool|mixed|object
   */
  public function getHostEntity(\FieldCollectionItemEntity $fieldCollection) {
    // Pull the parent/host entity with the maximum user privilege possible.
    $tempUser = clone $GLOBALS['user'];
    $this->drupal->saveSession(FALSE);
    $GLOBALS['user'] = $this->drupal->userLoad(1);
    $rawHost = $fieldCollection->hostEntity();
    $GLOBALS['user'] = $tempUser;
    $this->drupal->saveSession(TRUE);
    return $rawHost;
  }

}

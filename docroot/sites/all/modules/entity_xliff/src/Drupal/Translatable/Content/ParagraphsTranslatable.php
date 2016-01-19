<?php

/**
 * @file
 * Defines a paragraphs translatable compatible with Core content translation.
 */

namespace EntityXliff\Drupal\Translatable\Content;

use EntityXliff\Drupal\Translatable\EntityTranslatableBase;
use EntityXliff\Drupal\Utils\DrupalHandler;

/**
 * Class ParagraphsTranslatable
 * @package EntityXliff\Drupal\Translatable\Content
 */
class ParagraphsTranslatable extends EntityTranslatableBase {

  /**
   * Static cached value of the source language for this paragraphs item. It's
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
      $target = $this->entity->value();
      $this->targetEntities[$targetLanguage] = $this->drupal->entityMetadataWrapper('paragraphs_item', $target);
    }

    return $this->targetEntities[$targetLanguage];
  }

  /**
   * {@inheritdoc}
   *
   * If this class is even instantiated, it means Paragraphs are translatable.
   */
  public function isTranslatable() {
    return TRUE;
  }

  /**
   * {@inheritdoc}
   *
   * This should never be called anyway since a Paragraphs will always be saved
   * in the context of a host entity. But just in case, this should be a no-op
   * for this entity type.
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
   * Returns a given paragraphs item entity's host entity.
   * @param \ParagraphsItemEntity $paragraphsItem
   * @return bool|mixed|object
   */
  public function getHostEntity(\ParagraphsItemEntity $paragraphsItem) {
    // Pull the parent/host entity with the maximum user privilege possible.
    $tempUser = clone $GLOBALS['user'];
    $this->drupal->saveSession(FALSE);
    $GLOBALS['user'] = $this->drupal->userLoad(1);
    $rawHost = $paragraphsItem->hostEntity();
    $GLOBALS['user'] = $tempUser;
    $this->drupal->saveSession(TRUE);
    return $rawHost;
  }

}

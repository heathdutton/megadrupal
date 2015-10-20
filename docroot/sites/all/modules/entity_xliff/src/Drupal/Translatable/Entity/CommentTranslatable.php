<?php

/**
 * @file
 * Defines a Comment translatable compatible with Entity Translation module.
 */

namespace EntityXliff\Drupal\Translatable\Entity;

use EntityXliff\Drupal\Translatable\EntityFieldTranslatableBase;


/**
 * Class CommentTranslatable
 * @package EntityXliff\Drupal\Translatable\Entity
 */
class CommentTranslatable extends EntityFieldTranslatableBase {

  /**
   * {@inheritdoc}
   *
   * Comments do not have a $wrapper->language property or meaningful language
   * info... So we do as the entity base does, but do not attempt to set a
   * language.
   */
  public function getTargetEntity($targetLanguage) {
    if (!isset($this->targetEntities[$targetLanguage]) || empty($this->targetEntities[$targetLanguage])) {
      $target = clone $this->entity;
      $target->language($targetLanguage);
      $this->targetEntities[$targetLanguage] = $target;
    }
    return $this->targetEntities[$targetLanguage];
  }

  /**
   * {@inheritdoc}
   *
   * Comments do not seem to require initialization...
   */
  public function initializeTranslation() {}

}

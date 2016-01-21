<?php

/**
 * @file
 * Defines a Term translatable compatible with Entity Translation module.
 */

namespace EntityXliff\Drupal\Translatable\Entity;

use EntityXliff\Drupal\Translatable\EntityFieldTranslatableBase;


/**
 * Class TaxonomyTermTranslatable
 * @package EntityXliff\Drupal\Translatable\Entity
 */
class TaxonomyTermTranslatable extends EntityFieldTranslatableBase {

  /**
   * {@inheritdoc}
   *
   * Taxonomy terms do not have a $wrapper->language property or meaningful
   * language info... So we do as the entity base does, but do not attempt to
   * set a language.
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
   * Taxonomy terms do not seem to require initialization...
   */
  public function initializeTranslation() {}

}

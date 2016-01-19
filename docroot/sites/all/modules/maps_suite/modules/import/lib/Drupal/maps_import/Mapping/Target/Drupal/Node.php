<?php

/**
 * @file
 * Class that defines a Drupal node Entity.
 */

namespace Drupal\maps_import\Mapping\Target\Drupal;

use Drupal\maps_import\Converter\ConverterInterface;

class Node extends Entity {

  /**
   * @inheritdoc
   */
  protected function getTranslationClass() {
    switch (locale_multilingual_node_type($this->getBundle())) {
      case self::TRANSLATION_NONE:
      default:
        return 'Drupal\\maps_import\\Mapping\\Target\\Drupal\\Translation\\NoTranslation';

      case self::TRANSLATION_ENABLED:

      case self::TRANSLATION_FIELD:
        return 'Drupal\\maps_import\\Mapping\\Target\\Drupal\\Translation\\FieldTranslation';

      case self::TRANSLATION_CONTENT:
        return 'Drupal\\maps_import\\Mapping\\Target\\Drupal\\Translation\\ContentTranslation';

    }//end switch
  }

  /**
   * @inheritdoc
   */
  protected function createEntity($languageId, $entityId = NULL) {
    if ($wrapper = parent::createEntity($languageId, $entityId)) {
      // Ensure there is an author, for _node_save_revision().
      $wrapper->author->set(drupal_anonymous_user());
      $wrapper->value()->uid = 0;
    }

    return $wrapper;
  }

  /**
   * @inheritdoc
   */
  public function unpublishEntities(ConverterInterface $converter) {
    foreach ($this->getIdentifiers() as $nid) {
      $node = node_load($nid, NULL, TRUE);
      $node->status = 0;

      node_save($node);
    }
  }

  /**
   * @inheritdoc
   */
  public function hasPublicationFeature() {
    return TRUE;
  }

  /**
   * @inheritdoc
   */
  public function save() {
    foreach ($this->getEntities() as $wrapper) {
      $entity = $wrapper->value();

      // Add a value for "uid" property, because none is set by
      // entity API.
      if ($author = $wrapper->author->value()) {
        $entity->uid = isset($author->uid) && FALSE !== $author->uid ? $author->uid : 0;
      }
      elseif (!isset($entity->uid)) {
        $entity->uid = 0;
      }
    }

    parent::save();
  }

}

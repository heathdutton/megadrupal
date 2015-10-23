<?php

/**
 * @file
 * Class that defines a Drupal Taxonomy term Entity.
 */

namespace Drupal\maps_import\Mapping\Target\Drupal;

use Drupal\maps_import\Exception\MappingException;
use Drupal\maps_import\Converter\ConverterInterface;

class TaxonomyTerm extends Entity {

  /**
   * @inheritdoc
   */
  protected function createEntity($languageId, $entityId = NULL) {
    if ($wrapper = parent::createEntity($languageId, $entityId)) {
      if (!$vocabulary = taxonomy_vocabulary_machine_name_load($wrapper->getBundle())) {
        throw new MappingException('The target vocabulary %name does not exist.', 0, array('%name' => $wrapper->getBundle()));
      }

      $wrapper->value()->vid = $vocabulary->vid;
    }

    return $wrapper;
  }

  /**
   * @inheritdoc
   */
  protected function getTranslationClass() {
    if (!function_exists('i18n_taxonomy_vocabulary_mode')) {
      return 'Drupal\\maps_import\\Mapping\\Target\\Drupal\\Translation\\NoTranslation';
    }

  	$taxonomy = taxonomy_vocabulary_machine_name_load($this->getBundle());
  	switch ((int) i18n_taxonomy_vocabulary_mode($taxonomy->vid)) {
      case self::TRANSLATION_NONE:
      default:
        return 'Drupal\\maps_import\\Mapping\\Target\\Drupal\\Translation\\NoTranslation';

      case self::TRANSLATION_ENABLED:

      case self::TRANSLATION_FIELD:
        return 'Drupal\\maps_import\\Mapping\\Target\\Drupal\\Translation\\FieldTranslation';

      case self::TRANSLATION_CONTENT:
        return 'Drupal\\maps_import\\Mapping\\Target\\Drupal\\Translation\\ContentTranslation';
    }
  }


  /**
   * @inheritdoc
   */
  public function save() {
    foreach ($this->getEntities() as $wrapper) {
      $entity = $wrapper->value();

      if (!isset($entity->name) || !drupal_strlen($entity->name)) {
        throw new MappingException('Term name is required.', 0, array(), array('$term' => $entity));
      }

      if (!empty($entity->parent) && isset($entity->tid) && in_array($entity->tid, $entity->parent)) {
        throw new MappingException('A term cannot be its own child.',  0, array(), array('$term' => $entity));
      }

      if (!isset($entity->parent) || (is_array($entity->parent) && empty($entity->parent))) {
        $entity->parent = array(0);
      }

      if (!isset($entity->weight) || !is_numeric($entity->weight) || $entity->weight != (string) intval($entity->weight)) {
        $entity->weight = 0;
      }

      parent::save();
    }
  }

  /**
   * @inheritdoc
   */
  public function deleteEntities(ConverterInterface $converter) {
    foreach ($this->getIdentifiers() as $tid) {
      taxonomy_term_delete($tid);
    }
  }

}

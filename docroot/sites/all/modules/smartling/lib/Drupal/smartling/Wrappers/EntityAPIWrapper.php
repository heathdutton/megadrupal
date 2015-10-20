<?php

/**
 * @file
 * Smartling log.
 */

namespace Drupal\smartling\Wrappers;

/**
 * Class EntityAPIWrapper.
 */
class EntityAPIWrapper {
  public function entityMetadataWrapper($entity_type, $entity) {
    return entity_metadata_wrapper($entity_type, $entity);
  }

  public function getBundle($entity_type, $entity) {
    $wrapper = $this->entityMetadataWrapper($entity_type, $entity);
    return $wrapper->getBundle();
  }

  public function getID($entity_type, $entity) {
    $wrapper = $this->entityMetadataWrapper($entity_type, $entity);
    return $wrapper->getIdentifier();
  }

  public function getOriginalEntity($entity_type, $entity) {
    switch ($entity_type) {
      case 'node':
        $entity = smartling_get_original_node($entity);
        break;

      case 'taxonomy_term':
        $entity = smartling_get_original_taxonomy_term($entity);
        break;
    }
    return $entity;
  }

  public function getLink($entity_type, $entity) {
    return smartling_get_link_to_entity($entity_type, $entity);
  }

  public function entityLanguage($entity_type, $entity) {
    return entity_language($entity_type, $entity);
  }

  public function entityLoad($entity_type, $ids = FALSE, $conditions = array(), $reset = FALSE) {
    return entity_load($entity_type, $ids, $conditions, $reset);
  }

  public function entityLoadSingle($entity_type, $id) {
    return entity_load_single($entity_type, $id);
  }

  public function entitySave($entity_type, $entity) {
    return entity_save($entity_type, $entity);
  }

  public function entityCreate($entity_type, array $values) {
    return entity_create($entity_type, $values);
  }

  public function nodeObjectPrepare($node) {
    node_object_prepare($node);
  }

  public function translationNodeGetTranslations($tnid) {
    return translation_node_get_translations($tnid);
  }

  public function entityGetInfo($entity_type = NULL) {
    return entity_get_info($entity_type);
  }

  public function taxonomyVocabularyMachineNameLoad($name) {
    return taxonomy_vocabulary_machine_name_load($name);
  }

  public function entityTypeIsFieldable($entity_type) {
    return entity_type_is_fieldable($entity_type);
  }



}

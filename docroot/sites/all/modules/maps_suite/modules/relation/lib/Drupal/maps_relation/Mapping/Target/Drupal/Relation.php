<?php

/**
 * @file
 * Class that defines a Drupal file Entity.
 */

namespace Drupal\maps_relation\Mapping\Target\Drupal;

use Drupal\maps_relation\Relation\Relation as ConverterRelation;
use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Mapping\Mapping;
use Drupal\maps_import\Mapping\MappingInterface;
use Drupal\maps_import\Mapping\Target\Drupal\Entity;

class Relation extends Entity {

  /**
   * @inheritdoc
   */
  protected function getTranslationClass() {
    return 'Drupal\\maps_import\\Mapping\\Target\\Drupal\\Translation\\NoTranslation';
  }

  /**
   * @inheritdoc
   */
  public function save() {
    // Get the converter relation.
    $converter_relation_id = str_replace('relation_', '', $this->getConverter()->getName());

    $converter_relation = ConverterRelation::load($converter_relation_id);

    if ($converter_relation) {
      foreach ($this->getEntities() as $wrapper) {
        $entity = Mapping::getCurrentEntity();
        $relatedEntities = Mapping::getCurrentEntity()->getRelatedEntities();
        $current_entity = $relatedEntities[$entity->id];

        $values = $entity->getRawValues();

        $endpoints = array();

        foreach ($converter_relation->getEndpoints() as $i => $endpoint) {
          unset($entity_id, $entity_type);

          $exploded = explode(':', $endpoint);
          $endpoint_type = array_shift($exploded);
          $endpoint_value = array_shift($exploded);

          switch ($endpoint_type) {
            case 'current_entity':
              $result = Mapping::getEntityIdFromMapsId(
                'object',
                $entity->id,
                $this->getConverter()->getUidScope(),
                $this->getConverter()->getPid());
              $entity_id = reset($result);
              $entity_type = $converter_relation->getConverter()->getEntityType();
              break;

            case 'property':
              if ($endpoint_value === 'source_id') {
                $query = db_select(MappingInterface::DB_ENTITIES_TABLE, 'e');
                $query->join('maps_import_object_ids', 't', 'e.id = t.correspondence_id');
                $query->join('maps_import_objects', 'o', 't.maps_id = o.id');
                $query
                  ->fields('e')
                  ->condition('o.source_id', $current_entity['source_id']);

                $result = $query->execute()->fetchAllAssoc('id_language', \PDO::FETCH_ASSOC);

                $entity_info = reset($result);
                $entity_id = $entity_info['entity_id'];
                $entity_type = $entity_info['entity_type'];
              }
              else if ($endpoint_value === 'parent_id') {
                $result = Mapping::getEntityFromMapsId(
                  'object',
                  $current_entity['parent_id'],
                  $this->getConverter()->getUidScope(),
                  $this->getConverter()->getPid());

                $entity_info = reset($result);
                $entity_id = $entity_info['entity_id'];
                $entity_type = $entity_info['entity_type'];
              }

              break;
          }

          if (!empty($entity_id) && !empty($entity_type)) {
            $loaded_entity = entity_load($entity_type, array($entity_id));
            $endpoints[] = entity_metadata_wrapper($entity_type, reset($loaded_entity));
          }

        }

        $wrapper->endpoints->set($endpoints);
      }
    }

    parent::save();
  }

  /**
   * Delete all existing Drupal Relations for the current converter.
   *
   * @param ConverterRelation $converter
   *   The related converter.
   */
  public static function deleteExistingRelations(ConverterInterface $converter, $entity_id) {
    $ids_table = "maps_import_{$converter->getType()}_ids";

    // Firstly, get the list of Relations to delete.
    $query = db_select('maps_import_entities', 'e');
    $query->join($ids_table, 'i', 'i.correspondence_id = e.id');
    $query
      ->fields('e')
      ->condition('e.cid', $converter->getCid(), '=')
      ->condition('i.maps_id', $entity_id);

    $result = $query->execute()->fetchAllAssoc('id', \PDO::FETCH_ASSOC);
    foreach ($result as $id => $row) {
      db_delete('maps_import_entities')
        ->condition('id', $id)
        ->execute();

      db_delete($ids_table)
        ->condition('correspondence_id', $id)
        ->execute();

      relation_delete($row['entity_id']);
    }
  }

}

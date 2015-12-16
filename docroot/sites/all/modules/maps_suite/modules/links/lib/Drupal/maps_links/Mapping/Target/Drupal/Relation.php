<?php

/**
 * @file
 * Class that defines a Drupal file Entity.
 */

namespace Drupal\maps_links\Mapping\Target\Drupal;

use Drupal\maps_import\Fetcher\Fetcher;
use Drupal\maps_import\Mapping\MappingInterface;
use Drupal\maps_import\Mapping\Target\Drupal\Entity;
use Drupal\maps_links\Mapping\Link;

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
    // @todo manage logs.

    $entity = Link::getCurrentEntity();
    $pid = $this->getConverter()->getProfile()->getPid();

    // Here we need to take care of the source object, since the links are often
    // created in MaPS System® using this value.
    $sourceIds = array(
      'source' => $entity->getSource(),
      'target' => $entity->getTarget(),
    );
    $entities = $this->getEntitiesFromLinkIds('object', $sourceIds, $this->getConverter()->getUidScope(), $pid);

    // Get Drupal entities.
    if (count($entities) > 1) {
      foreach ($entities as $existingEntity) {
        if ($existingEntity['id'] == $sourceIds['source'] || $existingEntity['source_id'] == $sourceIds['source']) {
          $sourceEntity = $existingEntity;
        }
        else {
          $targetEntity = $existingEntity;
        }
      }
    }

    if (empty($sourceEntity) || empty($targetEntity)) {
      // Impossible to save a relation without full endpoints...
      return;
    }

    // Set endpoints
    foreach ($this->getEntities() as $wrapper) {
    	$wrapper->endpoints->set(array(
        entity_metadata_wrapper($sourceEntity['entity_type'], entity_load_single($sourceEntity['entity_type'], $sourceEntity['entity_id'])),
        entity_metadata_wrapper($targetEntity['entity_type'], entity_load_single($targetEntity['entity_type'], $targetEntity['entity_id'])),
      ));
    }

    parent::save();
  }

  protected function getEntitiesFromLinkIds($type, array $mapsIds, $uid_scope = 1, $pid = 0) {
    $table = 'maps_import_' . $type . '_ids';

    $query = db_select(MappingInterface::DB_ENTITIES_TABLE, 'e');
    $query->join($table, 't', 'e.id = t.correspondence_id');
    $query->join(Fetcher::DB_OBJ_TABLE, 'o', 'o.id = t.maps_id');
    $query
      ->fields('e', array('entity_type', 'entity_id'))
      ->fields('o', array('id', 'source_id'))
      ->condition(db_or()
        ->condition('e.uid_scope', array(0, 1))
        ->condition(db_and()
          ->condition('e.uid_scope', 2)
          ->condition('e.pid', $pid)))
      ->condition(db_or()
        ->condition('o.id', $mapsIds)
        ->condition('o.source_id', $mapsIds));

    $result = $query->execute()->fetchAll(\PDO::FETCH_ASSOC);
    return !empty($result) ? $result : array();
  }

}

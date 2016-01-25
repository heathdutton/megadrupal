<?php

/**
 * @file
 * Handle objects mapping operation.
 */

namespace Drupal\maps_import\Mapping\Mapper;

use Drupal\maps_import\Fetcher\Fetcher;
use Drupal\maps_import\Mapping\Source\MapsSystem\Object as MapsSystemObject;

/**
 * Base class for all mapping operations.
 */
class Object extends Mapper {

  /**
   * @inheritdoc
   */
  public function getTitle() {
    return t('Objects mapping');
  }

  /**
   * @inheritdoc
   */
  public function getDescription() {
    return t('The objects mapping operation is related to the profile %profile.', array('%profile' => $this->getProfile()->getTitle()));
  }

  /**
   * @inheritdoc
   */
  public function getType() {
    return 'object_mapping';
  }

  /**
   * @inheritdoc
   */
  protected function getConverterClass() {
    return "Drupal\\maps_import\\Converter\\Object";
  }

  /**
   * @inheritdoc
   */
  protected function getEntityTable() {
    return Fetcher::DB_OBJ_TABLE;
  }

  /**
   * @inheritdoc
   */
  protected function createEntity(array $entity) {
    return new MapsSystemObject($entity);
  }

  /**
   * Get the current list of object entities to process
   * and add related medias to objects.
   */
  protected function getCurrentEntities($maxEntities) {
    $objects = parent::getCurrentEntities($maxEntities);

    if (empty($objects)) {
      return $objects;
    }

    $query = db_select('maps_import_object_media', 'om');
    $query->join('maps_import_medias', 'm', 'm.id = om.media_id');
    $query->join('maps_import_media_ids', 'mi', 'mi.maps_id = m.id');
    // The join on the pid is required, otherwise there could be collision
    // between medias with the same ID but from different profiles.
    $query->join('maps_import_entities', 'e', 'e.id = mi.correspondence_id AND e.pid = m.pid');

    $query->condition('m.pid', (int) $this->getProfile()->getPid())
      ->condition('om.object_id', array_keys($objects))
      ->fields('om', array('media_id', 'object_id'))
      ->fields('m', array('type', 'attributes', 'weight'))
      ->fields('e', array('entity_id'));

    $medias = $query
      ->execute()
      ->fetchAll(\PDO::FETCH_ASSOC);

    foreach ($objects as $id => &$object) {
      foreach ($medias as $media) {
        if ((int) $media['object_id'] == (int) $id) {
          $object['medias'][] = $media;
        }
      }
    }

    return $objects;
  }

  /**
   * @inheritdoc
   */
  protected function processRun($index = 1, $range = 0) {
    parent::processRun($index, $range);

    // Message displayed under the progressbar.
    $this->context['message'] = t('Mapping objects for profile "@title" (@current/@total)', array(
      '@title' => $this->getProfile()->getTitle(),
      '@current' => $this->context['results'][$this->getType()]['processed_op'],
      '@total' => $this->context['results'][$this->getType()]['total_op'],
    ));
  }

}

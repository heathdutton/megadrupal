<?php

namespace Drupal\panelizer_deploy;

class EntityConverter extends AbstractConverter implements ConverterInterface {
  protected $entity_type;

  public function __construct($entity_type) {
    $this->entity_type = $entity_type;
  }

  public function getKeyField() {
    $info = \entity_get_info($this->entity_type);

    return $info['entity keys']['id'];
  }

  public function getUUIDField() {
    $info = \entity_get_info($this->entity_type);

    return $info['entity keys']['uuid'];
  }

  /**
   * Convert an array of ids to ids => uuids
   *
   * @param $ids
   *   array of ids
   * @return array
   *   array of id => uuid
   */
  protected function convertUUIDs($ids) {
    return \entity_get_uuid_by_id($this->entity_type, $ids);
  }

  /**
   * Convert an array of ids to uuid => id
   *
   * @param $uuids
   *   array of uuids
   * @return array
   *   array of uuid => id
   */
  protected function convertIds($uuids) {
    $return = \entity_get_id_by_uuid($this->entity_type, $uuids);
    return $return;
  }
}
<?php

namespace Drupal\panelizer_deploy;

class DisplayConverter extends AbstractConverter implements ConverterInterface {


  /**
   * Get the key for the object
   *
   * @return string
   */
  public function getKeyField() {
    return "did";
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
    return $this->queryHelper("panels_display", 'did', 'uuid', $ids);
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
    $return = $this->queryHelper("panels_display", 'uuid', 'did', $uuids);
    return $return;
  }
}

<?php

namespace Drupal\panelizer_deploy;

class PaneConverter extends AbstractConverter implements ConverterInterface {
  /**
   * Get the key for the object
   *
   * @return string
   */
  public function getKeyField() {
    return "pid";
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
    return $this->queryHelper("panels_pane", 'pid', 'uuid', $ids);
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
    $return = $this->queryHelper("panels_pane", 'uuid', 'pid', $uuids);
    return $return;
  }
}
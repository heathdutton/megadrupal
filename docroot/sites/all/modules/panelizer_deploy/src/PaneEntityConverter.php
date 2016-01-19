<?php

namespace Drupal\panelizer_deploy;

class PaneEntityConverter extends EntityConverter implements ConverterInterface {
  /**
   * Before converting the object setup the
   * tracking of ids and references
   *
   * @param $objects array
   * @return array
   */
  protected function preConvert(&$objects) {
    if (!is_array($objects)) {
      $things = array(&$objects);
    } else {
      $things = &$objects;
    }

    // track the ids
    $ids = array();
    $i = 0;
    foreach ($things as &$thing) {
      $this->refs[$i] = &$thing->entity_id;
      $ids[] = $this->refs[$i];
      $i++;
    }

    return $ids;
  }

}
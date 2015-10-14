<?php

namespace Drupal\panelizer_deploy;


class EntityContextConverter extends EntityConverter implements ConverterInterface {

  /**
   * Override the preconvert.
   *
   * The objects will be ctools context arrays
   *
   * @param array $objects
   * @param $key_field
   * @return array
   */
  protected function preConvert(&$objects, $key_field) {
    $ids = array();
    $i = 0;
    foreach ($this->$objects as &$context) {
      $this->refs[$i] = &$context['entity_id'];
      $ids[] = $this->refs[$i];
      $i++;
    }

    return $ids;
  }
}
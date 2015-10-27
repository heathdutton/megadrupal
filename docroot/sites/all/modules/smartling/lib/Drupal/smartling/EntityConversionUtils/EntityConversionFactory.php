<?php

namespace Drupal\smartling\EntityConversionUtils;

class EntityConversionFactory {
  protected function getService($name) {
    return drupal_container()->get($name);
  }

  public function getConverter($entity_type) {
    switch($entity_type) {
      case 'node':
        return $this->getService('smartling.entity_conversion_utils.node');
        break;

      case 'taxonomy_term':
        return $this->getService('smartling.entity_conversion_utils.taxonomy_term');
        break;
    }
    return $this->getService('smartling.entity_conversion_utils.entity');
  }
}
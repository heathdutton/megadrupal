<?php

namespace Drupal\maps_import\Mapping\Target\Drupal\Field;

/**
 * @file 
 * Custom management of the node reference field.
 */

class Node extends DefaultField {

  /**
   * @inheritdoc
   */
  public function sanitize($values) {
    if (is_array($values)) {
      $values = reset($values);
    }
    
    $value = isset($values['entity_id']) ? (int) $values['entity_id'] : $values;
    $node = node_load($value);
    $wrapper = entity_metadata_wrapper('node', $node);
    
    return intval($wrapper->nid->value());
  }
  
}

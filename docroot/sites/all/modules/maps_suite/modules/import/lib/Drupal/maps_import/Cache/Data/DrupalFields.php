<?php

/**
 * @file
 * Class that loads Drupal fields.
 */

namespace Drupal\maps_import\Cache\Data;

use Drupal\maps_import\Exception\CacheException;

/**
 * The MaPS Import Cache class related to Drupal fields.
 */
class DrupalFields extends Data {

  /**
   * @inheritdoc
   */
  protected function getWildcard() {
    return 'maps_import:drupal_fields:';
  }

  /**
   * @inheritdoc
   *
   * @param array $options
   *   The required keys are:
   *   - entity_type: The entity type.
   *   - bundle: The entity bundle.
   */
  protected function getCacheKey(array $options = array()) {
    if (empty($options['entity_type']) || empty($options['bundle'])) {
      throw new CacheException('Impossible to build the list of Drupal fields: some required options are missing.', 0, array(), array('options' => $options));
    }

    return $options['entity_type'] . ':' . $options['bundle'];
  }

  /**
   * @inheritdoc
   *
   * @param array $options
   *   See the getCacheKey() method above.
   *
   * @todo handle languages properly!
   */
  protected function buildData(array $options = array()) {
    // Ensure the $options variable has required values.
    $this->getCacheKey($options);

    $fields = array();

    $info = entity_get_property_info($options['entity_type']) + array('properties' => array());
    $properties = $info['properties'];

    if (isset($info['bundles'][$options['bundle']]['properties'])) {
      $properties += $info['bundles'][$options['bundle']]['properties'];
    }

    foreach ($properties as $name => $property) {
      if (isset($property['maps_import_handler'])) {
        $property['id'] = $name;
        $fields[$name] = new $property['maps_import_handler']($property);
      }
    }

    return $fields;
  }

}

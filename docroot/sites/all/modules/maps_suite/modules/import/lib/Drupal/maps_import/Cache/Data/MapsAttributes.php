<?php

/**
 * @file
 * Class that loads MaPS System attributes.
 */

namespace Drupal\maps_import\Cache\Data;

use Drupal\maps_import\Mapping\Source\MapsSystem as MS;
use Drupal\maps_import\Exception\CacheException;

/**
 * The MaPS Import Cache class related to MaPS System attributes.
 */
class MapsAttributes extends Data {

  /**
   * @inheritdoc
   */
  protected function getWildcard() {
    return 'maps_import:maps_attributes:';
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
    if (empty($options['profile'])) {
      throw new CacheException('Impossible to build the list of MaPS System attributes: profile is missing.');
    }

    return $options['profile']->getPid();
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
    $this->getCacheKey($options);

    $profile = $options['profile'];
    
    if (!$languageId = $profile->getLanguage()) {
      return array();
    }

    $attributes = array();
    if (!isset($attributes[$languageId])) {
      $attributes[$languageId] = array();

      foreach ($profile->getConfigurationTypes('attribute', $languageId) as $id => $attribute) {
        $attribute += unserialize($attribute['data']);
        unset($attribute['data']);

        if (isset($attribute['attribute_type_code'])) {
          $class = 'Drupal\\maps_import\\Mapping\\Source\\MapsSystem\\Attribute\\' . maps_suite_drupal2camelcase($attribute['attribute_type_code']);
          $attributes[$languageId]['attribute:' . $id] = class_exists($class) ? new $class($attribute) : new MS\Attribute\DefaultAttribute($attribute);
        }
      }
    }

    return $attributes[$languageId];
  }

}

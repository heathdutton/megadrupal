<?php

/**
 * @file
 * Class that loads mapping items.
 */

namespace Drupal\maps_import\Cache\Data;

use Drupal\maps_import\Exception\CacheException;
use Drupal\maps_import\Mapping\Source\MapsSystem as MS;
use Drupal\maps_import\Mapping\Target\Drupal\Field;
use Drupal\maps_import\Mapping\Item\Item;

/**
 * The MaPS Import Cache class related to Mapping items.
 */
class MappingItems extends Data {

  /**
   * @inheritdoc
   */
  protected function getWildcard() {
    return 'maps_import:mapping_items:';
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
    if (empty($options['converter'])) {
      throw new CacheException('Impossible to build the list of mapping items: some required options are missing.', 0, array(), array('options' => $options));
    }

    if (empty($options['type'])) {
      $options['type'] = 'default';
    }
    
    return $options['converter']->getCid() . ':' . $options['type'];
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
    $items = array();

    $this->getCacheKey($options);
    $_converter = $options['converter'];
    
    $result = db_select('maps_import_mapping_item')
      ->fields('maps_import_mapping_item')
      ->condition('cid', $_converter->getCid())
      // Object and Media converters are stored in the same table, but not the Link ones.
      ->condition('type', $_converter->getType() === 'link' ? 'link' : 'object')
      ->orderBy('weight')
      ->execute()
      ->fetchAll();

    foreach ($result as $item) {
      $item_options = unserialize($item->options);
      $converter = $_converter;

      if ($options['type'] === 'all' || ($options['type'] === 'delayed' xor empty($item_options['post_save']))) {
        if (!empty($item_options['post_save'])) {
          $class = get_class($_converter);
          $converter = new $class($_converter->getProfile());
          $converter->setBundle($item_options['bundle']);
          $converter->setEntityType($item_options['entity_type']);
        }
      }
      else {
        continue;
      }

      $property = MS\PropertyWrapper::load($converter, $item->property_id);
      $field = Field\DefaultField::load($converter, $item->field_name);

      if (!empty($item_options['source'])) {
        $property->setOptions($item_options['source']);
      }

      if (!empty($item_options['target'])) {
        $field->setOptions($item_options['target']);
      }

      $items[$item->id] = new Item(array(
        'id' => $item->id,
        'property' => $property,
        'field' => $field,
        'weight' => $item->weight,
        'required' => $item->required,
        'options' => $item_options,
        'converter' => $options['type'] === 'delayed' ? $converter : $_converter,
      ));
    }

    return $items;
  }

}

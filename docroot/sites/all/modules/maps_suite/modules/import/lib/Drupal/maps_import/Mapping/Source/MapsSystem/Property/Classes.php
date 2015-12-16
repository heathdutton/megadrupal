<?php
/**
 * Created by PhpStorm.
 * User: lucas
 * Date: 26/06/15
 * Time: 14:25
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem\Property;

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface;

class Classes extends Property {

  /**
   * @inheritdoc
   */
  protected $typeCode = 'int';

  /**
   * @inheritdoc
   */
  protected $multiple = TRUE;

  /**
   * @inheritdoc
   */
  public function processValues($values, EntityInterface $entity, ConverterInterface $currentConverter) {
    $return = array();
    $values = unserialize($values);
    foreach ($values as $value) {
      $return[] = $value['value'];
    }

    return $return;
  }
}
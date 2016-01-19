<?php

/**
 * @file
 * Child converter class for link converters.
 */

namespace Drupal\maps_import\Converter\Child\Relation;

use Drupal\maps_import\Converter as ParentConverter;
use Drupal\maps_import\Converter\Object as ParentObject;
use Drupal\maps_import\Converter\Child\ChildInterface as ChildInterface;

/**
 * Object Child Converter class.
 */
class Object extends ParentObject implements ChildInterface {
  
  /**
   * @inheritdoc
   */
  public function getParent() {
    return 0;
  }

  /**
   * @inheritdoc
   */
  public function setParent(ParentConverter\ConverterInterface $converter) {}
  
  /**
   * @inheritdoc
   */
  public function getUid() {
    return $this->uid;
  }

 /**
   * @inheritdoc
   */
  public function getParentId() {
    return 0;
  }
  
}

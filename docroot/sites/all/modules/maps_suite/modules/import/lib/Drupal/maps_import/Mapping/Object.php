<?php

/**
 * @file
 * Class for Object Mapping.
 */

namespace Drupal\maps_import\Mapping;

/**
 * The Mapping class related to MaPS System® objects.
 */
class Object extends Mapping {

  /**
   * @inheritdoc
   */
	protected $type = 'object_mapping';

  /**
   * @inheritdoc
   */
  public function getSourceProperties() {
    // @todo use a cache class.
    // @todo handle languages
    static $properties;

    if (!isset($properties)) {
      $properties = parent::buildSourcePropertyHandlers(array(
        'id' => array('title' => 'ID', 'class' => 'ObjectId'),
        'parent_id' => array('title' => 'Parent ID', 'class' => 'ParentId'),
        'weight' => array('title' => 'Weight'),
        'source_id' => array('title' => 'Source ID', 'class' => 'ObjectId'),
        'classes' => array('title' => 'Classes', 'class' => 'Classes'),
        'code' => array('title' => 'Code', 'class' => 'Code'),
        // We are not supposed to use this property, since it only indicate the
        // published state of the MaPS object, which is already handled by the
        // Drupal\maps_import\Mapping\Source\MapsSystem\Entity::isPublished
        'status' => array('title' => 'Status', 'class' => 'Status'),
        'nature' => array('title' => 'Nature'),
        'type' => array('title' => 'Type'),
        'updated' => array('title' => 'Last updated time'),
      ));
    }

    return $properties;
  }

}

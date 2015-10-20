<?php

/**
 * @file
 * Class for Medias mapping.
 */

namespace Drupal\maps_import\Mapping;

/**
 * The Mapping class related to MaPS System® media.
 */
class Media extends Mapping {

  /**
   * @inheritdoc
   */	
  protected $type = 'media_mapping';

  /**
   * @inheritdoc
   */
  public function getSourceProperties() {
    // @todo use a cache class.
    // @todo handle languages
    static $properties;

    if (!isset($properties)) {
      $properties = parent::buildSourcePropertyHandlers(array(
        'id' => array('title' => 'Media ID'),
        'filename' => array('title' => 'File name'),
        'type' => array('title' => 'Type'),
        'weight' => array('title' => 'Weight'),
      ));
    }

    return $properties;
  }

}

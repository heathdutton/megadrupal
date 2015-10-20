<?php

/**
 * @file
 *
 * Defines a MaPS System® media.
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem;

/**
 * The MaPS System® Source media class.
 */
class Media extends Entity {

  /**
   * The media file name.
   *
   * @var string
   */
  public $filename;

  /**
   * The media type.
   *
   * @var int
   */
  public $type;

  /**
   * The media weight.
   *
   * @var int
   */
  public $weight;

  /**
   * @inheritdoc
   */
  public function __construct(array $entity) {
    parent::__construct($entity);
    $this->attributes = unserialize($entity['attributes']);
  }

}

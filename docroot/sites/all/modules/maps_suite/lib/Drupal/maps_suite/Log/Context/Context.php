<?php

/**
 * @file
 * Defines the MaPS Suite Log Context class.
 */

namespace Drupal\maps_suite\Log\Context;

/**
 * MaPS Suite Log Context base class.
 */
class Context implements ContextInterface {

  /**
   * The context type.
   *
   * @var string
   */
  protected $type;

  /**
   * The context attributes.
   *
   * @var array
   */
  protected $attributes;

  /**
   * Class constructor.
   *
   * @param $type
   *   The context type.
   * @param $attributes
   *   An associative array that defines the contect attributes.
   *
   * @return Context
   */
  public function __construct($type, array $attributes = array()) {
    $this->type = $type;
    $this->attributes = $attributes;
  }

  /**
   * @inheritdoc
   */
  public function getType() {
    return $this->type;
  }

  /**
   * @inheritdoc
   */
  public function getAttributes() {
    return $this->attributes;
  }

}

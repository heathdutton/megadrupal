<?php

/**
 * @file
 * Defines the MaPS Suite Log Context interface.
 */

namespace Drupal\maps_suite\Log\Context;

/**
 * MaPS Suite Log Context interface.
 */
interface ContextInterface {

  /**
   * Get the context type.
   *
   * @return string
   */
  public function getType();

}

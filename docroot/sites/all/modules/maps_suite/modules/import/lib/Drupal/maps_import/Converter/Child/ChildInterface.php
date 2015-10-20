<?php

/**
 * @file
 * Child Converter interface.
 */

namespace Drupal\maps_import\Converter\Child;

use Drupal\maps_import\Converter\ConverterInterface;

/**
 * Child Converter interface.
 */
interface ChildInterface extends ConverterInterface {

  /**
   * Load and return the parent converter.
   *
   * @return ConverterInterface
   *   The parent converter object.
   */
  public function getParent();

  /**
   * Set the parent converter.
   *
   * @param ConverterInterface
   *   The parent converter object.
   */
  public function setParent(ConverterInterface $converter);

}

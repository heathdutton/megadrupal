<?php

/**
 * @file
 * Defines the classes and interfaces related to MaPS Import operations.
 */

namespace Drupal\maps_import\Operation;

/**
 * The MaPS Import Operation interface.
 */
interface OperationInterface {

  /**
   * Get the operation translated title, that may be used in some administrative UI.
   *
   * @return string
   */
  public function getTitle();

  /**
   * Get the operation translated description.
   *
   * @return string
   */
  public function getDescription();

  /**
   * Get the operation title and description.
   *
   * @return string
   *   An HTML formatted string.
   */
  public function getFullDescription();

  /**
   * Get the machine readable operation type.
   *
   * @return string
   */
  public function getType();

  /**
   * Get the current log instance.
   *
   * @return MapsLogInterface
   *   The current log instance.
   */
  public function getlog();

  /**
   * Return a boolean indicating whether the current operation belongs
   * to a batch API set or not.
   *
   * @return boolean
   *    The boolean indicating if batch or not.
   */
  public function isBatch();

  /**
   * Returns an array of operations for batch processing.
   *
   * @return array
   *    The array of operations.
   */
  public function batchOperations();

  /**
   * Get the total number of operations.
   */
  public function getTotalOperations(array $args = array());
    
}


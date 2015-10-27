<?php

/**
 * @file
 * Contains \Drupal\plug_config\Plugin\Config\DefaultConfigEntityInterface.
 */

namespace Drupal\plug_config\Plugin\Config;

interface DefaultConfigEntityInterface extends ConfigInterface, EntityInterface {

  /**
   * Gets the label
   *
   * @return string
   *   The label.
   */
  public function getLabel();

  /**
   * Sets the label.
   *
   * @param string $label
   *   The label.
   */
  public function setLabel($label);

  /**
   * Gets the name
   *
   * @return string
   *   The name.
   */
  public function getName();

  /**
   * Sets the name.
   *
   * @param string $name
   *   The name.
   */
  public function setName($name);

  /**
   * Function that checks if the entity is locked or not based on its status.
   *
   * @return bool
   */
  public function isLocked();

}

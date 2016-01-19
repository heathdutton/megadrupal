<?php
/**
 * @file
 * Contains Drupal\CommonMark\ExtensionInterface.
 */

namespace Drupal\CommonMark;

/**
 * Interface ExtensionInterface.
 *
 * @package Drupal\CommonMark
 */
interface ExtensionInterface {

  /**
   * Retrieves a setting.
   *
   * @param string $name
   *   The name of the setting to retrieve.
   *
   * @return mixed
   *   The settings value or NULL if not set.
   */
  public function getSetting($name);

  /**
   * Retrieves the current settings.
   *
   * @return array
   *   The settings array
   */
  public function getSettings();

  /**
   * Sets a specific setting.
   *
   * @param string $name
   *   The name of the setting to set.
   * @param mixed $value
   *   (optional) The value to set. If not provided it will be removed.
   */
  public function setSetting($name, $value = NULL);

  /**
   * Provides settings to an extension.
   *
   * @param array $settings
   *   The settings array.
   */
  public function setSettings(array $settings = []);

}

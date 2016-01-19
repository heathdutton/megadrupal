<?php
/**
 * @file
 * Contains Drupal\CommonMark\Extension.
 */

namespace Drupal\CommonMark;

/**
 * Class Extension.
 *
 * @package Drupal\CommonMark
 */
class Extension implements ExtensionInterface {

  /**
   * The settings array.
   *
   * @var array
   */
  protected $settings = [];

  /**
   * {@inheritdoc}
   */
  public function getSetting($name) {
    return isset($this->settings[$name]) ? $this->settings[$name] : NULL;
  }

  /**
   * {@inheritdoc}
   */
  public function getSettings() {
    return $this->settings;
  }

  /**
   * {@inheritdoc}
   */
  public function setSetting($name, $value = NULL) {
    if (isset($value)) {
      // Get the type of the exist value (if any).
      if (isset($this->settings[$name]) && ($type = gettype($this->settings[$name]))) {
        $original_value = is_object($value) ? clone $value : $value;
        if (!settype($value, $type)) {
          $value = $original_value;
        }
      }
    }
    $this->settings[$name] = $value;
  }

  /**
   * {@inheritdoc}
   */
  public function setSettings(array $settings = []) {
    foreach ($settings as $name => $value) {
      $this->setSetting($name, $value);
    }
  }

}

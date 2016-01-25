<?php

/**
 * @file
 * Contains a
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

namespace Drupal\globals;

use Drupal\ghost\Exception\GhostException;

/**
 * Class GlobalItem
 *
 * @package Drupal\globals
 */
class GlobalItem {

  /**
   * The name.
   *
   * @var string
   */
  public $name;

  /**
   * The key.
   *
   * @var string
   */
  protected $key;

  /**
   * The default value.
   *
   * @var mixed
   */
  public $default;

  /**
   * The type.
   *
   * @var string
   */
  public $type;

  /**
   * The description.
   *
   * @var string
   */
  public $description;

  /**
   * The settings.
   *
   * @var array
   */
  public $settings;

  /**
   * Lazy initialiser.
   *
   * @param string $key
   *   Unique key for this item.
   * @param array $settings
   *   An array of settings.
   *
   * @return static
   *   An instance of GlobalItem
   * @static
   */
  static public function init($key, $settings) {
    $item = new static($key);

    foreach ($settings as $setting_key => $setting_value) {
      if ($setting_key == 'key') {
        continue;
      }
      if (property_exists($item, $setting_key)) {
        $item->$setting_key = $setting_value;
      }
    }

    $item->setSettings($settings);

    return $item;
  }

  /**
   * Constructor.
   *
   * @param string $key
   *   Unique key for this item.
   */
  public function __construct($key) {
    $this->key = $key;
  }

  /**
   * Getter for name.
   *
   * @return string
   *   The name.
   */
  public function getName() {

    return $this->name;
  }

  /**
   * Setter for name.
   *
   * @param string $name
   *   The value for name.
   */
  public function setName($name) {

    $this->name = $name;
  }

  /**
   * Getter for key.
   *
   * @return string
   *   The key.
   */
  public function getKey() {

    return $this->key;
  }

  /**
   * Getter for default.
   *
   * @param mixed|null $override
   *   An optional override.
   *
   * @return mixed The default
   * The default
   */
  public function getDefault($override = NULL) {

    if (!empty($override)) {
      return $override;
    }
    elseif (isset($this->default)) {
      return $this->default;
    }

    return NULL;
  }

  /**
   * Setter for default.
   *
   * @param mixed $default
   *   The value for default.
   */
  public function setDefault($default) {

    $this->default = $default;
  }

  /**
   * Getter for type.
   *
   * @return string
   *   The type
   */
  public function getType() {

    if (isset($this->type)) {
      return $this->type;
    }

    return GLOBALS_TYPE_VARIABLE;
  }

  /**
   * Setter for type.
   *
   * @param string $type
   *   The value for type.
   */
  public function setType($type) {

    $this->type = $type;
  }

  /**
   * Getter for description.
   *
   * @return string
   *   The description.
   */
  public function getDescription() {

    if (isset($this->description)) {
      return $this->description;
    }

    return '';
  }

  /**
   * Setter for description.
   *
   * @param string $description
   *   The value for description.
   */
  public function setDescription($description) {

    $this->description = $description;
  }

  /**
   * Getter for settings.
   *
   * @return array
   *   The settings
   */
  public function getSettings() {

    return $this->settings;
  }

  /**
   * Setter for settings.
   *
   * @param array $settings
   *   The value for settings.
   */
  public function setSettings($settings) {

    $this->settings = $settings;
  }

  /**
   * Determine if this should be hidden.
   *
   * @return bool
   *   TRUE if it is hidden.
   */
  public function isHidden() {
    if (isset($this->settings['hidden']) && $this->settings['hidden'] == TRUE) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Get the global value.
   *
   * @return mixed
   *   Result of the query.
   * @throws \Drupal\ghost\Exception\GhostException
   */
  public function getValue() {
    return variable_get($this->getKey(), $this->getDefaultValue());
  }

  /**
   * Get a global value filtered.
   *
   * @param string $filter
   *   Filter name. Defaults to 'check_plain'. If you provide another filter
   *   value, its up to you to ensure the output is secure.
   *
   * @return mixed
   *   Result of the query.
   * @throws \Drupal\ghost\Exception\GhostException
   */
  public function getFilteredValue($filter = 'check_plain') {

    if (is_callable($filter)) {
      return $filter($this->getValue());
    }
    else {
      throw new GhostException('Invalid filter supplied');
    }
  }

  /**
   * Get the default value.
   *
   * @return mixed|null
   *   The default value.
   */
  public function getDefaultValue() {
    if (isset($this->default)) {
      return $this->default;
    }

    return NULL;
  }

  /**
   * Set the value.
   *
   * @param mixed $value
   *   Value to set
   */
  public function setValue($value) {
    variable_set($this->getKey(), $value);
  }

  /**
   * Get the defined form element settings.
   *
   * @return array
   *   An array of form element settings.
   */
  public function getFormElement(){

    if (isset($this->settings['form element'])) {
      return $this->settings['form element'];
    }

    return array();
  }
}

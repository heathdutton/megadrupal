<?php
/**
 * @file
 * Giant Bomb data property handler.
 */

class GiantBombBaseProperty {
  function __construct($value, $settings = array(), $object_info) {
    $this->value = $value;
    $this->settings = $settings;
    $this->object = $object_info;
  }

  public function settings_set($settings) {
    $this->settings = $settings;
  }

  /**
   * Ensure value matches this property type
   *
   * @param mixed $value
   *
   * @return bool
   *   Whether the value is valid.
   */
  public function validate() {
    return FALSE;
  }

  /**
   * Prepare the value for saving.
   *
   * @return processed value, or FALSE to cancel save.
   */
  public function value() {
    return FALSE;
  }

  static function form(&$form, &$form_state, $settings = array()) {
    return NULL;
  }
}
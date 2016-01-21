<?php

/**
 * Class for core profile picture field to AX convertion handling
 */
class openid_profile_picture {
  var $plugin;
  var $name;
  var $options = array();

  /**
   * Fake constructor -- this is easier to deal with than the real
   * constructor because we are retaining PHP4 compatibility, which
   * would require all child classes to implement their own constructor.
   */
  function init($plugin) {
    $this->plugin = $plugin;
  }

  /**
   * Convert a value to a string which is in compliance with the OpenID Attribute Exchange specification 
   */
  function convert_to_ax($value, $count = 1) {
    if (file_exists($value)) {
      //TODO: check this value!!
      return url($value, array('absolute' => TRUE));
    }
  }

  /**
   * Convert a value from a string which is in compliance with the OpenID Attribute Exchange specification
   * to a value that Drupal can handle.
   * This converts single values.
   */
  function convert_from_ax_single($value) {
    return $value;
  }

  /**
   * Convert a value from a string which is in compliance with the OpenID Attribute Exchange specification
   * to a value that Drupal can handle.
   * This converts multiple values.
   */
   function convert_from_ax_multiple($values) {
    return FALSE;
  }

  /**
   * Returns the amount of values that should be saved for multiple attributes
   */
  function get_values_count($count = 1, $field_name = NULL) {
    //TODO: field_name in mapping?

    return 1;
  }

  /**
   * Returns the attributes available
   */
  function get_attributes() {
    return array(
      'picture' => t('Account: Picture'),
    );
  }
}
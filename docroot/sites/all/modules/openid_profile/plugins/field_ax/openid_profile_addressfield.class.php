<?php

/**
 * Class for basic field to AX convertion handling
 */
class openid_profile_addressfield {
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
    $field_elements = explode('::', $this->plugin['mapping']['value']);
    $field_name = array_shift($field_elements);
    $field_column = array_shift($field_elements);

    if ($count == 1) {
      return $value[LANGUAGE_NONE][0][$field_column];
    }
    else {
      $values = array();
      foreach ($value[LANGUAGE_NONE] as $key => $address) {
        $values[]['value'] = $address[$field_column];
      }
      return $values;
    }
  }

  /**
   * Convert a value from a string which is in compliance with the OpenID Attribute Exchange specification
   * to a value that Drupal can handle.
   * This converts single values.
   */
  function convert_from_ax_single($value) {
    $field_elements = explode('::', $this->plugin['mapping']['value']);
    $field_name = array_shift($field_elements);
    $field_column = array_shift($field_elements);

    // Use the advanced drupal_static() pattern, since this is called very often.
    static $drupal_static_fast;
    if (!isset($drupal_static_fast)) {
      $drupal_static_fast[$field_name] = &drupal_static(__FUNCTION__ . '_' . $field_name);
    }
    $field_data = &$drupal_static_fast[$field_name];
    $field_data[LANGUAGE_NONE][0][$field_column] = $value;
    return $field_data;
  }

  /**
   * Convert a value from a string which is in compliance with the OpenID Attribute Exchange specification
   * to a value that Drupal can handle.
   * This converts multiple values.
   */
  function convert_from_ax_multiple($values) {
    $field_elements = explode('::', $this->plugin['mapping']['value']);
    $field_name = array_shift($field_elements);
    $field_column = array_shift($field_elements);

    // Use the advanced drupal_static() pattern, since this is called very often.
    static $drupal_static_fast;
    if (!isset($drupal_static_fast)) {
      $drupal_static_fast[$field_name] = &drupal_static(__FUNCTION__ . '_' . $field_name);
    }
    $field_data = &$drupal_static_fast[$field_name];

    $return = array();
    foreach ($values as $value) {
      $field_data[LANGUAGE_NONE][][$field_column] = $value;
     }
     return $field_data;
  }

  /**
   * Returns the amount of values that should be saved for multiple attributes
   */
  function get_values_count($count = 1, $field_name = NULL) {
    //TODO: Field_name in mapping?

    // Get information about field
    $field_info = field_info_field($field_name);

    // AX was set to unlimited. So limit to the field's cardinality. 
    // Or return -1 for unlimited.
    if ($count == 'unlimited'){
      return $field_info['cardinality'];
    }

    // Both values have a maximum amount of values. 
    // So take the most restrictive (minor) one.
    elseif ($field_info['cardinality'] >= 1) {
      return min($count, $field_info['cardinality']);
    }

    // Unlimited values can be handled but AX limits to a maximum.
    elseif ($field_info['cardinality'] == -1 && is_numeric($count)) {
      return $count;
    }
  }

  /**
   * Returns the attributes available
   */
  function get_attributes() {
    $attributes = array();
    $field_types = (array) $this->plugin['field_types'];
    $instances = field_read_instances(array('entity_type' => 'user', 'bundle' => 'user'));
    foreach ($instances as $instance) {
      $field_info = field_info_field($instance['field_name']);
      if (in_array($field_info['type'], $field_types)) {
        $properties = addressfield_data_property_info();
        foreach ($properties as $property_name => $property) {
          $attributes[$instance['field_name'] . '::' . $property_name] = t('Field: @label: @property', array(
            '@label' => $instance['label'],
            '@property' => $property['label'],
          ));
        }
      }
    }
    return $attributes;
  }
}
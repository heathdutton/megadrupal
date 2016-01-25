<?php

/**
 * @file
 * Views handler for displaying the customer address.
 */

/**
 * Field handler to provide simple renderer to display the customer address as
 * a single field
 */
class ec_store_views_handler_field_formatted_address extends views_handler_field {
  /**
   * Override the init function to add the additional fields for the names
   */
  function init(&$view, &$data) {
    parent::init($view, $data);
    $schema = drupal_get_schema($this->table);
    foreach ($schema['fields'] as $field => $info) {
      $this->additional_fields[$field] = $field;
    }
  }

  function render($values) {
    $schema = drupal_get_schema($this->table);
    $address = array();
    foreach (array_intersect_key($this->aliases, $schema['fields']) as $key => $alias) {
      $address[$key] = $values->$alias;
    }
    $address['name'] = check_plain(!empty($address['fullname']) ? $address['fullname'] : $address['firstname'] . ' ' . $address['lastname']);
    return theme('formatted_address', array('address' => $address, 'include_name' => FALSE));
  }
}

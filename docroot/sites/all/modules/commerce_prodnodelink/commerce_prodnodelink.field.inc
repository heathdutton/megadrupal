<?php

/**
 * @file
 * Field related hook implementations for the module.
 */


/**
 * Implements hook_field_create_instance().
 */
function commerce_prodnodelink_field_create_instance($instance) {
  $field_info = field_info_field($instance['field_name']);

  if ($field_info['type'] == 'commerce_product_reference') {
    // See https://api.drupal.org/comment/52813#comment-52813
    $instance_info = field_info_instance(
      $instance['entity_type'],
      $instance['field_name'],
      $instance['bundle']
    );

    $name = commerce_prodnodelink_get_variable_name($instance_info);
    variable_set($name, $instance['field_name']);
  }
}


/**
 * Implements hook_field_delete_instance().
 */
function commerce_prodnodelink_field_delete_instance($instance) {
  $field_info = field_info_field($instance['field_name']);

  if ($field_info['type'] == 'commerce_product_reference') {
    $name = commerce_prodnodelink_get_variable_name($instance);

    if ($name) {
      variable_del($name);
    }
  }
}


/**
 * Implements hook_field_attach_delete_bundle().
 */
function commerce_prodnodelink_field_attach_delete_bundle($entity_type, $bundle, $instances) {
  if ($entity_type == 'node') {
    $vars = commerce_prodnodelink_vars('data');
    $var_prefix = 'commerce_prodnodelink_data_' . $bundle . '_';

    foreach ($vars as $name => $value) {
      if (strpos($name, $var_prefix) !== FALSE) {
        variable_del($name);
      }
    }
  }
}

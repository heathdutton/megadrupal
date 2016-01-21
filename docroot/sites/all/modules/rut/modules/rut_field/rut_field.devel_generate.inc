<?php

/**
 * @file
 * Devel Generate support for Rut Field module.
 */

/**
 * Implements hook_devel_generate().
 */
function rut_field_devel_generate($object, $field, $instance, $bundle) {
  if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
    return devel_generate_multiple('_rut_field_devel_generate', $object, $field, $instance, $bundle);
  }
  else {
    return _rut_field_devel_generate($object, $field, $instance, $bundle);
  }
}


/**
 * Generate and set a RUT to the element of a new entity.
 */
function _rut_field_devel_generate($object, $field, $instance, $bundle) {
  $object_field = array();

  $object_field['value'] = rut_generate_rut();
  return $object_field;
}

<?php

/**
 * @file
 * Devel Generate support for HMS Field module.
 */

/**
 * Implements hook_devel_generate().
 */
function hms_field_devel_generate($object, $field, $instance, $bundle) {
  if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
    return devel_generate_multiple('_hms_field_devel_generate', $object, $field, $instance, $bundle);
  }
  else {
    return _hms_field_devel_generate($object, $field, $instance, $bundle);
  }
}

/**
 * Callback for hook_devel_generate().
 */
function _hms_field_devel_generate($object, $field, $instance, $bundle) {
  return array(
    'value' => mt_rand(-86400, 86400),
  );
}
<?php
/**
 * @file 
 * Party hat developer tools
 */
 
function party_hat_devel_generate($object, $field, $instance, $bundle) {
  if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
    return devel_generate_multiple('_party_hat_devel_generate', $object, $field, $instance, $bundle);
  }
  else {
    return _party_hat_devel_generate($object, $field, $instance, $bundle);
  }
}

function _party_hat_devel_generate($object, $field, $instance, $bundle) {
  $object_field = array();

  // Get All hats
  $hats = party_hat_get_all_hats();
  $id = array_rand($hats);
  $hat = $hats[$id];
  $object_field['hat_name'] = $hat->name;
  return $object_field;
}

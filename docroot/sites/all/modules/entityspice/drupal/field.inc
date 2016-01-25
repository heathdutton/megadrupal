<?php

/**
 * @file
 * Tools to works with fields.
 */

/**
 * Given a field types, List all fields available of that type.
 *
 * Good for when you want user to select an available field of specific type
 * in form.
 *
 * @param string $type
 *   Machine name of requested field type.
 * @param boolean/array $has_none_option
 *   Indicates whether to add a 'NONE' option as list frst element. If it's
 *   value is non-array, Then a default array of ('NONE'=> 'NONE') will be
 *   used, Otherwise the passed array will be used as none option.
 *   If an array is passed it must only have one key and it's value must (only)
 *   be a single translated string.
 *
 * @return
 *   An array suitable for form API select element.
 */
function _entityspice_generate_field_list($type = NULL, $none_option=TRUE) {
  // Also correctly checks an empty array.
  if($none_option) {
    if(is_array($none_option)) {
      $fields = $none_option;
    }
    else {
      $fields = array('NONE' => t('NONE'));
    }
  }
  else {
    $fields = array();
  }

  foreach (field_info_fields() as $field) {
    if($type && $field['type'] != $type) {
      continue;
    }

    $fields[$field['field_name']] = $field['field_name'];
  }

  sort($fields);
  return $fields;
}

/**
 * Same as _entityspice_generate_field_list but only fields belonging to a
 * bundle of specific entity type.
 */
function _entityspice_generate_field_list_from_bundle($etype, $bundle, $none_option = FALSE) {
  // Also correctly checks an empty array.
  if($none_option) {
    if(is_array($none_option)) {
      $fields = $none_option;
    }
    else {
      $fields = array('NONE' => t('NONE'));
    }
  }
  else {
    $fields = array();
  }

  foreach (field_info_instances($etype, $bundle) as $name => $crap) {
    $fields[$name] =  check_plain($name) . ' - ' . check_plain($crap['label']);
  }

  ksort($fields);
  return $fields;
}

<?php

/**
 * Extract one property of objects stored in an array.
 *
 * @param array $data
 *  The array containing the objects to extract property from.
 * @param string $property
 *  Name of property to extract.
 * @param string $key
 *  Wether to preserve keys in input array or not. If is FALSE, Numeric values
 *  in order of input array data is used. If is TRUE, Same keys from input
 *  array is preserved and if is an string, A property from each object is used
 *  as key.
 */
function entityspice_array_extract_object_prop(array $data, $property, $key=FALSE) {
  $result = array();
  if($key) {
    if($key === TRUE) {
      foreach ($data as $key => $obj) {
        $result[$key] = $obj->{$property};
      }
    }
    else {
      foreach ($data as $obj) {
        $result[$obj->{$key}] = $obj->{$property};
      }
    }
  }
  else {
    foreach ($data as $obj) {
      $result[] = $obj->{$property};
    }
  }

  return $result;
}

/**
 * Get a key from array if it is set. Other wise returns $default.
 *
 * If $not_false is set to TRUE, then $data[$key] is checked so it does not
 * evaluate to FALSE, if so, $default is returned.
 */
function _entityspice_array_get(array $data, $key, $default, $not_false=FALSE) {
  if($not_false) {
    $result = (isset($data[$key]) && $data[$key]) ? $data[$key] : $default;
  }
  else {
    $result = isset($data[$key]) ? $data[$key] : $default;
  }

  return $result;
}

/**
 * Extract one value from array of arrays.
 *
 * @param array $data
 *  The array containing the arrays.
 * @param string $nested_key
 *  key to extract from nested arrays.
 *
 * @return array
 *  An array whose keys are same as $data, but values is a value from nested
 *  array of that key.
 */
function _entityspice_array_extract_key_value(array $data, $nested_key, $check_isset = FALSE) {
  $result = array();

  foreach ($data as $key => $nested_array) {
    if($check_isset && !isset($nested_array[$nested_key])) {
      continue;
    }

    $result[$key] = $nested_array[$nested_key];
  }

  return $result;
}

/**
 * Search a property in array of objects for $value.
 *
 * @param array $data
 *   Array of objects to search
 * @param string $property
 *   Name of property to check.
 * @param mixed $value
 *   Value to compare against.
 * @param boolean $absolute_comparision
 *   Use === for comparision or not.
 *
 * @return boolean
 *   TRUE if $value found in $property of one of objects FALSE otherwise.
 */
function _entityspice_array_obj_property_search(array $data, $property, $value, $absolute_comparision = FALSE) {
  foreach ($data as $obj) {
    if($absolute_comparision) {
      if($obj->{$property} == $value) {
        return TRUE;
      }
    }
    else {
      if($obj->{$property} === $value) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

function _entityspice_copy_array_by_prop($source, $target, $properties) {
  if(!is_array($properties)) {
    $properties = $properties();
  }

  foreach ($properties as $prop) {
    $target[$prop] = $source[$prop];
  }
}

/**
 * Check to see if $key exists in array and $data[$key] evalutes as TRUE.
 *
 * WARNING: IT USES A SIMPLE PHP EVALUATION:
 *   if: $data['haha'] === '0';
 *   then: _entityspice_a_key_true($data, 'haha') === FALSE
 *   as: if('0') === if(FALSE)
 *
 * @return boolean
 *   TRUE if $data[$key] is set and it's value evalutes to true.
 */
function _entityspice_a_key_true(array $data, $key) {
  return isset($data[$key]) && $data[$key];
}

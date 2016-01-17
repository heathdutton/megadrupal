<?php

/**
 * Retrieves applicable content types.
 *
 * Content types can be used with this module if they come from cck and have a
 * workflow configured.
 *
 * @return array Array of content type names, indexed by content type
 */
function wf_required_fields_get_types_applicable() {
  static $cache = null;
  if ($cache === null) {
    $cache = array();
    $types = node_type_get_types();
    foreach ($types as $key => $value) {
      if (workflow_get_workflow_type_map_by_type($key) !== false) {
        $cache[$key] = $value->name;
      }
    }
  }
  return $cache;
}

/**
 * Retrieves the configured content types.
 *
 * @return array Array of content types, indexed by content type
 */
function wf_required_fields_get_types_configured() {
    $settings = variable_get(
      'wf_required_fields',
      array('types' => array('config' => array()))
    );
    return $settings['types']['config'];
}

/**
 * Checks whether a certain field is configured to be required in a node of a
 * certain content type in a certain state.
 *
 * @param string $type The content type to check
 * @param string $field The name of the field to check
 * @param int $sid The sid of the state to check
 * @return bool true: field is required; false otherwise
 */
function wf_required_fields_is_required($type, $field, $sid) {
  $settings = variable_get( 'wf_required_fields', array() );
  if (!isset($settings['settings'][$type]['table'][$field][$sid]['required'])) {
    return false;
  }
  return $settings['settings'][$type]['table'][$field][$sid]['required'];
}

/**
 * Finds the array that has a '#required' key inside a Forms API array that has
 * $field as distant parent.
 *
 * @param array $array The Forms API array
 * @param string $field
 * @return array The array that has a '#required' key as distant parent; null
 *  if none found
 */
function &wf_required_fields_find_required(&$array, $field) {
  if (!is_array($array)) {
    return null;
  }
  if (array_key_exists($field, $array)) {
    return wf_required_fields_get_if_recursive_subkey($array[$field], '#required');
  }
  foreach (array_keys($array) as $key) {
    $child =& wf_required_fields_find_required($array[$key], $field);
    if ($child !== null) {
      return $child;
    }
  }
  return null;
}

/**
 * Looks for a $key in a given &$array recursively.
 *
 * @param array $array The array to search
 * @param string $key The key
 * @return array The array that has the $key; null if none found
 */
function &wf_required_fields_get_if_recursive_subkey(&$array, $key) {
  if (!is_array($array)) {
    return null;
  }
  if (array_key_exists($key, $array)) {
    return $array;
  }
  foreach (array_keys($array) as $i) {
    if (is_array($array[$i])) {
      $child =& wf_required_fields_get_if_recursive_subkey($array[$i], $key);
      if ($child !== null) {
        return $child;
      }
    }
  }
  return null;
}

/**
 * Check which fields are not filled out properly for the specified node to be
 * promoted to the specified state.
 *
 * If the type of the node is not configured to be handled by this module,
 * this function produces errors at random.
 *
 * @param stdClass $node The node to check
 * @param int $sid The state to check
 * @return array Array of field names (indexed by field type)
 */
function wf_required_fields_get_missing_fields($node, $sid) {
  $non_empty = array('0', 0);
  $missing = array();
  $type = $node->type;
  $all_fields = field_info_fields();
  $instances = field_info_instances('node', $type);
  foreach ($instances as $field_name => $instance) {
    foreach (array_keys($all_fields[$field_name]['columns']) as $column) {
      $field = $node->{$field_name}['und'];
      if (wf_required_fields_is_required($type, $field_name, $sid)) {
        $first_field_element = array_shift($field);
        $value = $first_field_element[$column];
        if (!in_array($value, $non_empty, true) && empty($value)) {
          $missing[$field_name] = $instance['label'];
        }
      }
    }
  }
  return $missing;
}

/**
 * Sets the given Forms API field to required
 *
 * @param array $array The Forms API element
 */
function wf_required_fields_set_required(&$element) {
  foreach (element_children($element) as $key) {
    if (isset($element[$key]) && $element[$key]
      && (!isset($element['#type']) || !in_array($element['#type'], array('radios','checkboxes')))) {
      // Recurse through all children elements.
      wf_required_fields_set_required($element[$key]);
    }
  }

  if (isset($element['#type'])) {
    $element['#required'] = '1';
    $none = array('- None -', '---', '<none>');
    switch ($element['#type']) {
      case 'select':
        foreach ($none as $opt) {
          $i = array_search(t($opt), $element['#options']);
          if ($i !== false) {
            unset($element['#options'][$i]);
          }
        }
        break;

      case 'radios':
        if (isset($element['_none'])) {
          $element['_none']['#access'] = FALSE;
        }
        unset($element['#options']['_none']);
    }
  }
}

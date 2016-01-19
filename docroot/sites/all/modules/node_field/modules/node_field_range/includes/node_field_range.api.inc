<?php

/**
 * Validate node field in form.
 */
function node_field_range_validation($element, &$form_state) {
  $value = (int) $element['#value'];
  $id = $element['#field_id'];
  $result = node_field_range_is_allowed_value($id, $value);
  if (!$result['allowed']) {
    form_error($element, $result['message']);
  }
}

/**
 * Check node field for allowed values.
 * @param Node field data array or Node field id.
 * @param int $value
 */
function node_field_range_is_allowed_value($node_field, $value) {
  if (!is_array($node_field)) {
    $node_field = node_field_load_node_field($node_field);
  }
  $result = array(
    'allowed' => TRUE,
    'message' => NULL,
  );
  if (empty($node_field['settings']['range']) || empty($value)) {
    return $result;
  }
  $title = $node_field['title'];
  $range = $node_field['settings']['range'];

  switch ($node_field['type']) {
    case 'text':
      if (empty($range['min']) && empty($range['max'])) {
        return $result;
      }
      if (!empty($range['min']) && $value < $range['min']) {
        $result['allowed'] = FALSE;
        $result['message'] = t('Minimal value for @title field is @value', array('@title' => $title, '@value' => $range['min']));
      }
      if (!empty($range['max']) && $value > $range['max']) {
        $result['allowed'] = FALSE;
        $result['message'] = t('Maximum value for @title field is @value', array('@title' => $title, '@value' => $range['max']));
      }
      return $result;
      break;
  }
  return $result;
}
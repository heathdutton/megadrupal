<?php

/**
 * Init $node->node_fields param.
 *
 * @param object $node
 */
function node_field_init_node_fields($node) {
  $node_fields = node_field_db_node_fields_select($node);
  drupal_alter('node_fields', $node, $node_fields);
  node_field_set_node_fields($node, $node_fields, FALSE);
}

/**
 * Set node_fields node param.
 *
 * @param object $node
 * @return array of node fields.
 */
function node_field_set_node_fields($node, $node_fields, $set_changed_param = TRUE) {
  $node->node_fields = $node_fields;
  if ($set_changed_param) {
    $node->node_fields_changed = TRUE;
  }
}

function node_field_is_node_fields_changed($node) {
  if (!empty($node->node_fields_changed)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Get node_fields param from node.
 *
 * @param object $node
 * @return array of node fields.
 */
function node_field_get_node_fields($node) {
  // Init node node_fields param if it doesn't exist yet.
  if (!node_field_is_node_fields_init($node)) {
    node_field_init_node_fields($node);
  }
  // Return array of node fields if there are any.
  if (!empty($node->node_fields)) {
    return $node->node_fields;
  }
  return FALSE;
}

function node_field_is_node_fields_init($node) {
  if (isset($node->node_fields)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Check if node type is configured to have node fields.
 *
 * @param string $type
 * @return bool TRUE/FALSE
 */
function node_field_is_node_field_content_type($type) {
  $node_types = variable_get('node_field_node_types');
  if (empty($node_types[$type])) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Create a new field. Field array should contain all values you need but not field id.
 *
 * @param array $field
 * @return bool result
 */
function node_field_create_node_field(&$field) {
  $result = node_field_db_field_insert($field);
  if ($result) {
    module_invoke_all('node_field_insert', $field);
  }
  return $result;
}

/**
 * Load node field.
 *
 * @param int $fid
 * @return array / bool
 *   array - field.
 *   FALSE - field doesn't exist.
 */
function node_field_load_node_field($fid) {
  $node_field = node_field_db_field_select(array('id' => $fid));
  if ($node_field) {
    module_invoke_all('node_field_load', $node_field);
    return array_pop($node_field);
  }
  return FALSE;
}

/**
 * Update field based. Input param should have id field.
 *
 * @param array $field - node field.
 * @return updated field
 */
function node_field_update_node_field(&$field) {
  $result = node_field_db_field_update($field);
  if ($result) {
    module_invoke_all('node_field_update', $field);
  }
  return $result;
}

/**
 * Delete node field.
 *
 * @param array $field
 * @return bool
 *   TRUE - success
 *   FALSE - failure
 */
function node_field_delete_node_field($field) {
  module_invoke_all('node_field_delete', $field);
  $values = array(
    'id' => $field['id'],
  );
  return node_field_db_field_delete($values);
}

/**
 * Delete all node fields.
 *
 * @param object $node
 */
function node_field_delete_node_fields($node) {
  $values = array(
    'nid' => $node->nid,
  );
  return node_field_db_field_delete($values);
}

/**
 * Get settings from node field.
 *
 * @param object $node_field
 * @return array $settings.
 */
function node_field_get_node_field_settings($node_field) {
  $settings = isset($node_field['settings']) ? $node_field['settings'] : FALSE;
  return $settings;
}

/**
 * Get node_fields param from node as machine name assoc array.
 *
 * @param object $node
 * @return array of node fields.
 */
function node_field_get_node_fields_assoc($node) {
  $assoc_fields = array();
  $fields = node_field_get_node_fields($node);

  if ($fields) {
    foreach ($fields as $field) {
      $assoc_fields[$field['machine_name']] = $field;
    }
    return $assoc_fields;
  }
  return FALSE;
}

/**
 * Get renderable array of node fields.
 *
 * @param $node_fields
 * @return $content
 */
function node_field_node_content_add_node_fields($node_fields) {
  $content = array(
    '#theme_wrappers' => array('node_field_fields'),
  );
  foreach ($node_fields as $node_field) {
    $node_field = node_field_node_field_formatter($node_field);
    $field = array(
      '#theme' => 'node_field_field',
      '#node_field' => $node_field,
    );
    // Add classes to field (if there are any).
    if (!empty($node_field['settings']['node_field_class'])) {
      // Class(es) of the field.
      $classes = explode(' ', $node_field['settings']['node_field_class']);
      $classes = array_filter($classes);
      $classes = array_map('drupal_html_class', $classes);
      $prefix = '<div class="' . implode(' ', $classes) . '">';
      $suffix = '</div>';
      $field['#prefix'] = $prefix;
      $field['#suffix'] = $suffix;
    }
    $content[] = $field;
  }
  return $content;
}

/**
 * Copy node fields from one node to another.
 * @param object $carrier_node Node from need to copy.
 * @param object $node Node to need to copy.
 */
function node_field_copy_node_fields($carrier_node, $node) {
  $carrier_node_fields = node_field_get_node_fields($carrier_node);
  if (!empty($carrier_node_fields)) {
    foreach ($carrier_node_fields as $carrier_node_field) {
      $carrier_node_field['id'] = NULL;
      $carrier_node_field['nid'] = $node->nid;
      node_field_create_node_field($carrier_node_field);
    }
  }
}

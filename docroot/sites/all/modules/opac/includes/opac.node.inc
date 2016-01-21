<?php
/**
 * @file
 * file containing functions related to nodes processing.
 */

/**
 * Record transformation process.
 *
 * Create a new node or update it if existing.
 *
 * @param array $record
 *   Containing record values.
 *
 * @param array $mapping
 *   Which record fields should be mapped with which node fields.
 *
 * @param object $node
 *   An existing node for the record.
 *
 * @return object
 *   An new or updated node.
 */
function opac_transform_to_node($record, $mapping, $node = NULL) {
  $new = FALSE;
  // Means we got a new record. So create a new node.
  if (!$node) {
    $new = TRUE;
    $node = opac_create_node($record['opac_content_type']);
  }
  $record_id = $record['#record_id'];
  $server_id = $record['#server_id'];

  if ($node->type != $record['opac_content_type']) {
    node_delete($node->nid);

    // It may happen that changing rules for determining
    // opac content types causes the node no longer has type.
    // In this case, the node is not re-created.
    if ($record['opac_content_type']) {
      $oldct = $node->type;
      $node = opac_create_node($record['opac_content_type']);

      // Log this.
      $message = "Content type has changed (%oldct -> %newct). ";
      $message .= "Record: %recordid, Server: %servid, Title: %title. ";
      $message .= "Creating new node.";
      $variables = array(
        '%oldct' => $oldct,
        '%newct' => $node->type,
        '%recordid' => $record_id,
        '%servid' => $server_id,
        '%title' => $record['title'],
      );
      watchdog('OPAC HARVEST', $message, $variables, WATCHDOG_NOTICE);
    }
    elseif (!$new) {
      $message = "Record n° %recordid coming from %servid has no longer type.";
      $message .= " Title: %title. Node %nid has been deleted.";
      $variables = array(
        '%recordid' => $record_id,
        '%servid' => $server_id,
        '%title' => $record['title'],
        '%nid' => $node->nid,
      );
      watchdog('OPAC HARVEST - Node deleting', $message, $variables, WATCHDOG_NOTICE);
      return NULL;
    }
  }

  // Apply the mapping to the node. Means that
  // mapped fields are switched from record to the node.
  opac_record_to_node_fields($record, $node, $mapping);

  // We don't want an opac node to be promoted
  // to the front page.
  // May be opac module should provide a rules maker
  // to define if a node should be promoted?
  $node->promote = 0;
  return $node;
}

/**
 * Apply a mapping to a node.
 *
 * @param array $record
 *   Containing record values.
 *
 * @param object $node
 *   An existing node for the record.
 *
 * @param array $mappings
 *   Which record fields should be mapped with which node fields.
 */
function opac_record_to_node_fields($record, &$node, $mappings) {
  module_load_include('inc', 'opac', 'includes/opac.plugin');
  $node->title = $record['title'];

  // Loop through all mapped fields.
  // All values in remotefield fields will
  // be copied in nodefield.
  foreach ($mappings['mapped'] as $mapping) {
    $nodefield = $mapping['node_field_name'];
    $node->{$nodefield}['und'] = array();
    $field_info = field_info_field($nodefield);
    $instance = field_info_instance('node', $nodefield, $node->type);

    $plugin = opac_plugin_get($mapping['plugin']);
    opac_plugin_require_file($plugin);
    if ($plugin && function_exists($plugin['value callback'])) {
      $node->{$nodefield} = $plugin['value callback']($record, $mapping);
    }
    else {
      $max_length = isset($field_info['settings']['max_length']) ? $field_info['settings']['max_length'] : '';
      if (isset($field_info['columns']['value'])) {
        $remotefield = $mapping['mapped_with'];
        if (isset($record[$remotefield])) {
          foreach ($record[$remotefield] as $value) {
            $value = $max_length ? drupal_substr($value, 0, $max_length) : $value;
            $node->{$nodefield}['und'][] = array('value' => $value);
          }
        }
      }
      else {
        $message = "Field '@field' does not contain a 'value' column. "
          . "Please consider using a plugin for this field.";
        $variables = array('@field' => $nodefield);
        watchdog('opac', $message, $variables, WATCHDOG_WARNING);
      }
    }

    // Use field default value if
    // it is empty.
    if (!count($node->{$nodefield}['und'])) {
      $node->{$nodefield}['und'] = field_get_default_value('node', $node, $field_info, $instance, $node->language);
    }
  }
}

/**
 * Create a new node.
 *
 * @return object
 *   A new node.
 */
function opac_create_node($type) {
  $node = new stdClass();
  $node->type = $type;
  node_object_prepare($node);
  $node->uid = 1;
  $node->comment = 0;
  $node->language = 'und';
  return $node;
}

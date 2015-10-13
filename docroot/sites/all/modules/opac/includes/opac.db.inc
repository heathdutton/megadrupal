<?php
/**
 * @file
 * db functions for ils_connector
 */

/**
 * Get mapping for a given ILS server and field.
 *
 * @param string $serv_id
 *   A array containing server machine name.
 *
 * @param string $fieldname
 *   A array containing field machine name.
 *
 * @return array
 *   mapping for field that match the conditions.
 */
function opac_get_mapping($serv_id, $content_type, $fieldname) {
  $mapped_field = '';
  $mapped_field = db_select('opac_fields_mapping', 'fm')
    ->fields('fm')
    ->condition('serv_id', $serv_id, '=')
    ->condition('content_type', $content_type, '=')
    ->condition('node_field_name', $fieldname, '=')
    ->execute()
    ->fetchAssoc();

  if ($mapped_field && $mapped_field['plugin_options']) {
    $plugin_options = unserialize($mapped_field['plugin_options']);
    $mapped_field = array_merge($mapped_field, $plugin_options);
  }
  return $mapped_field;
}

/**
 * Get mapping for a given ILS server.
 *
 * @param string $serv_id
 *   A array containing server machine name.
 *
 * @return array
 *   mapping for the matched server.
 */
function opac_get_server_mapping($serv_id, $content_type = NULL) {
  $query = db_select('opac_fields_mapping', 'fm')
    ->fields('fm')
    ->condition('serv_id', $serv_id, '=');

  if ($content_type) {
    $query->condition('content_type', $content_type, '=');
  }

  $result = $query->execute();

  $mapping = array();
  while ($entry = $result->fetchAssoc()) {
    $mapping[$entry['node_field_name']] = $entry;
  }

  return $mapping;
}

/**
 * Add a content-type for an ILS server.
 *
 * @param array $values
 *   A array containing content type values.
 */
function opac_content_type_add($values) {
  $result = db_insert('opac_content_type')
    ->fields(array(
      'serv_id' => $values['serv_id'],
      'content_type' => $values['content_type'],
      'matching_field' => $values['matching_field'],
      'value' => $values['value'],
      'weight' => $values['weight'],
    ))
    ->execute();
}

/**
 * Edit a content-type for an ILS server.
 *
 * @param array $values
 *   A array containing content type values.
 */
function opac_content_type_edit($values) {
  $result = db_update('opac_content_type')
    ->fields(array(
      'matching_field' => $values['matching_field'],
      'value' => $values['value'],
      'weight' => $values['weight'],
    ))
    ->condition('serv_id', $values['serv_id'])
    ->condition('content_type', $values['content_type'])
    ->execute();
}

/**
 * Delete a content-type for an ILS server.
 *
 * @param array $values
 *   A array containing content type values.
 */
function opac_content_type_delete($values) {
  $result = db_delete('opac_content_type')
      ->condition('serv_id', $values['serv_id'])
      ->condition('content_type', $values['content_type'])
      ->execute();
}

/**
 * Change the weight of an opac content type rule.
 *
 * @param string $serv_id
 *   ILS Server identifier.
 *
 * @param string $content_type
 *   Content type machine name.
 *
 * @param int $weight
 *   New weight of the content type.
 */
function opac_content_type_mod_weight($serv_id, $content_type, $weight) {
  $result = db_update('opac_content_type')
    ->fields(array(
      'weight' => $weight,
    ))
    ->condition('serv_id', $serv_id)
    ->condition('content_type', $content_type)
    ->execute();
}

/**
 * Get machine names of all content_types used by ILS servers.
 *
 * @return array
 *   machine names of content types
 */
function opac_get_content_types($serv_id = NULL) {
  $query = db_select('opac_content_type', 'ct')
    ->fields('ct')
    ->orderBy('weight', 'ASC');

  if ($serv_id) {
    $query->condition('serv_id', $serv_id, '=');
  }
  $result = $query->execute();

  $content_types = array();
  while ($content_type = $result->fetchAssoc()) {
    $content_types[$content_type['content_type']] = $content_type;
  }
  return $content_types;
}

/**
 * Get machine name of a specific ILS servers/content type.
 *
 * @return array
 *   content type
 */
function opac_get_content_type($serv_id, $type_name) {
  $query = db_select('opac_content_type', 'ct')
    ->fields('ct')
    ->condition('serv_id', $serv_id)
    ->condition('content_type', $type_name);

  $result = $query->execute();
  return $result->fetchAssoc();
}

/**
 * Add a mapping for an ILS server.
 *
 * @param array $mapping
 *   A array containing mapping values whose server identifier.
 */
function opac_mapping_add($mapping) {
  $serv_id = db_insert('opac_fields_mapping')
    ->fields(array(
      'serv_id' => $mapping['serv_id'],
      'content_type' => $mapping['content_type'],
      'node_field_name' => $mapping['node_field_name'],
      'node_field_label' => $mapping['node_field_label'],
      'mapped_with' => $mapping['mapped_with'],
      'plugin' => $mapping['plugin'],
      'plugin_options' => $mapping['plugin_options'],
    ))
    ->execute();
}

/**
 * Perform a mapping modification.
 *
 * @param array $mapping
 *   A array containing mapping values whose server identifier.
 */
function opac_mapping_mod($mapping) {
  $serv_id = db_update('opac_fields_mapping')
    ->fields(array(
      'mapped_with' => $mapping['mapped_with'],
      'plugin' => $mapping['plugin'],
      'plugin_options' => $mapping['plugin_options'],
    ))
    ->condition('serv_id', $mapping['serv_id'], '=')
    ->condition('content_type', $mapping['content_type'], '=')
    ->condition('node_field_name', $mapping['node_field_name'], '=')
    ->execute();
}

/**
 * Delete a mapping.
 *
 * @param array $mapping
 *   A array containing mapping values whose server identifier.
 */
function opac_mapping_del($mapping) {
  $num_deleted = db_delete('opac_fields_mapping')
    ->condition('serv_id', $mapping['serv_id'], '=')
    ->condition('content_type', $mapping['content_type'], '=')
    ->condition('node_field_name', $mapping['node_field_name'], '=')
    ->execute();
}

/**
 * Add a record in opac_records table.
 *
 * @param int $nid
 *   Node id.
 *
 * @param string $recordid
 *   ILS reccord identifier.
 *
 * @param string $serverid
 *   Server identifier the record come from.
 */
function opac_add_record($nid, $recordid, $serverid) {
  $result = db_insert('opac_records')
    ->fields(array(
      'nid' => $nid,
      'recordid' => $recordid,
      'serv_id' => $serverid,
    ))
    ->execute();
}

/**
 * Get opac record.
 *
 * @param array $nid
 *   The node id to match.
 *
 * @return array
 *   Opac record.
 */
function opac_get_record($nid) {
  $opac_record = db_select('opac_records', 'r')
    ->fields('r', array('recordid', 'serv_id'))
    ->condition('nid', $nid)
    ->execute()
    ->fetchAssoc();

  return array(
    $opac_record['recordid'],
    $opac_record['serv_id'],
  );
}

/**
 * Get all opac records.
 *
 * @return array
 *   Opac records keyed with node id.
 */
function opac_get_node_ids() {
  $result = db_select('opac_records', 'r')
    ->fields('r')
    ->execute();

  $node_ids = array();
  while ($record = $result->fetchAssoc()) {
    $rid = $record['recordid'];
    $sid = $record['serv_id'];
    $nid = $record['nid'];
    $node_ids[$sid][$rid] = $nid;
  }

  return $node_ids;
}

/**
 * Get a node id for a given record.
 *
 * @param string $serverid
 *   The server where the record come from.
 *
 * @param string $recordid
 *   The record identifier.
 *
 * @return string
 *   The node id.
 */
function opac_get_node_id($serverid, $recordid) {
  $nid = db_select('opac_records', 'r')
    ->fields('r')
    ->condition('serv_id', $serverid)
    ->condition('recordid', $recordid)
    ->execute()
    ->fetchField();

  return $nid;
}

/**
 * Delete an opac record.
 *
 * @param int $nid
 *   Node id for wihich deleting record.
 */
function opac_delete_record($nid) {
  $num_deleted = db_delete('opac_records')
    ->condition('nid', $nid)
    ->execute();

  return $num_deleted;
}

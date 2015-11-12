<?php

/**
 * @file
 * Provides API and management functions for the salesforce webform maps.
 */

/**
 * Get the settings associated with a node.
 *
 * @param int $nid
 *   The node ID.
 *
 * @returns array
 *   An associative array of setting names and values
 *    - suppress_errors: (int) Flag indicating if errors should be suppressed.
 */
function salesforce_webforms_settings_get($nid) {
  // Set default values.
  $return = array(
    'suppress_errors' => FALSE,
    'nid' => $nid,
  );

  // Pull any settings found in the database.
  $settings = db_select('salesforce_webforms_settings', 's')
    ->fields('s', array('settingid', 'nid', 'suppress_errors'))
    ->condition('nid', $nid)
    ->execute()
    ->fetchAll();

  foreach ($settings as $setting) {
    $return['settingid'] = $setting->settingid;
    $return['suppress_errors'] = $setting->suppress_errors;
  }
  return $return;
}

/**
 * Deletes settings associated with a node.
 *
 * @param int $nid
 *   The node ID.
 */
function salesforce_webforms_settings_delete($nid) {
  // Delete mapping.
  db_delete('salesforce_webforms_settings')
    ->condition('nid', $nid)
    ->execute();
}

/**
 * Saves the settings.
 *
 * @param array $settings
 *   Associative array of settings:
 *   - settingid: (int) ID of existing record to update. Will create new if
 *       missing.
 *   - nid: (int) Node ID
 *   - suppress_errors: (int) flag indicating if errors should be suppressed.
 */
function salesforce_webforms_settings_save($settings) {
  // See if we're updating an existing record.
  $count = 0;
  $fields = array(
    'nid' => $settings['nid'],
    'suppress_errors' => $settings['suppress_errors'],
  );

  if (isset($settings['settingid'])) {
    $count = db_update('salesforce_webforms_settings')
      ->fields($fields)
      ->condition('settingid', $settings['settingid'])
      ->execute();
  }
  else {
    db_insert('salesforce_webforms_settings')
      ->fields($fields)
      ->execute();
  }
}

/**
 * Get a map entry.
 *
 * @param int $mapid
 *   The ID of the map to load.
 *
 * @return array
 *   An associative array of map data containing:
 *   - mapid: (int) Map ID.
 *   - mapname: (string) The name of the map.
 *   - nid: (int) The node ID that owns the webform.
 *   - sf_object: (string) The Salesforce object being mapped to.
 */
function salesforce_webforms_get_map($mapid) {
  $result = db_query("SELECT mapid, mapname, nid, sf_object, displayorder FROM {salesforce_webforms_map} where mapid = :mapid", array(":mapid" => $mapid), array('fetch' => PDO::FETCH_ASSOC));
  $map = $result->fetchAssoc();
  $map['fields'] = salesforce_webforms_get_map_fields($mapid, $map['nid']);
  return $map;
}

/**
 * Gets an array of maps assigned to a webform node.
 *
 * @param int $nid
 *   The node ID of the node which owns the webform.
 *
 * @return array
 *   An array of associative arrays containing:
 *   - mapid: (int) Map ID.
 *   - mapname: (string) The name of the map.
 *   - nid: (int) The node ID that owns the webform.
 *   - sf_object: (string) The Salesforce object being mapped to.
 *   - displayorder: (int) The order the mappings should be shown.
 */
function salesforce_webforms_get_node_maps($nid) {
  $maps = array();
  $result = db_query("SELECT mapid, mapname, nid, sf_object, displayorder FROM {salesforce_webforms_map} WHERE nid = :nid ORDER BY displayorder", array(':nid' => $nid), array('fetch' => PDO::FETCH_ASSOC));
  foreach ($result as $map) {
    $map['fields'] = salesforce_webforms_get_map_fields($map['mapid'], $map['nid']);
    $maps[] = $map;
  }
  return $maps;
}

/**
 * Get an array of fields mapped in a map.
 *
 * @param int $mapid
 *   The ID of the map to load.
 * @param int $nid
 *   The ID of the node which owns the webform.
 *
 * @return array
 *   An array of associative arrays of fields containing:
 *   - fieldmapid: (int) The ID of this field map.
 *   - sf_fieldname: (string) The Salesforce object being mapped to.
 *   - source_data: (string) The data to be written to Salesforce. May be a
 *     literal string, or may contain tokens from the webform.
 *   - keyfield: (bool) Indicates if this field should be used as a lookup to
 *     determine which object should be updated or not.
 */
function salesforce_webforms_get_map_fields($mapid, $nid) {
  $fields = array();
  $schema_version = salesforce_webforms_schema_version();
  $include_filter = ($schema_version >= 7004);


  $sql = "SELECT fieldmapid,sf_fieldname,source_data,is_keyfield";
  if ($include_filter) {
    $sql .= ",filter";
  }
  $sql .= " FROM {salesforce_webforms_field} WHERE mapid = :mapid";
  $result = db_query($sql, array(':mapid' => $mapid));
  foreach ($result as $row) {
    $fields[] = array(
      'fieldmapid' => $row->fieldmapid,
      'sf_fieldname' => $row->sf_fieldname,
      'source_data' => $row->source_data,
      'filter' => $include_filter ? $row->filter : NULL,
      'keyfield' => $row->is_keyfield,
    );
  }

  return $fields;
}

/**
 * Reads the maps and settings for nodes and store in the node.
 *
 * @param array $nodes
 *   An array of node objects.
 */
function salesforce_webforms_get_nodes_maps($nodes) {
  $nids = array_keys($nodes);

  // First look for settings.
  $settings = db_select('salesforce_webforms_settings', 's')
    ->fields('s', array('settingid', 'nid', 'suppress_errors'))
    ->condition('nid', $nids, 'IN')
    ->execute()
    ->fetchAll();

  foreach ($settings as $setting) {
    $nodes[$setting->nid]->webform['salesforce_settings'] = array(
      'settingid' => $setting->settingid,
      'nid' => $setting->nid,
      'suppress_errors' => $setting->suppress_errors,
    );
  }

  $maps = db_select('salesforce_webforms_map', 'm')
    ->fields('m', array('mapid', 'mapname', 'nid', 'sf_object', 'displayorder'))
    ->condition('nid', $nids, 'IN')
    ->orderBy('displayorder', 'ASC')
    ->execute()
    ->fetchAll();

  if (count($maps) == 0) {
    return;
  }

  $mids = array();
  $map_list = array();
  foreach ($maps as $map) {
    $mids[] = $map->mapid;
    $map_list[$map->mapid] = array(
      'mapname' => $map->mapname,
      'object' => $map->sf_object,
      'nid' => $map->nid,
      'map_fields' => array(),
    );
  }

  // Now get all of the fields for those maps.
  $schema_version = salesforce_webforms_schema_version();
  $include_filter = ($schema_version >= 7004);


  $field_list = array('mapid', 'sf_fieldname', 'source_data', 'is_keyfield');
  if ($include_filter) {
    $field_list[] = 'filter';
  }
  $fields = db_select('salesforce_webforms_field', 'f')
    ->fields('f', $field_list)
    ->condition('mapid', $mids, 'IN')
    ->execute()
    ->fetchAll();

  foreach ($fields as $field) {
    $map_list[$field->mapid]['map_fields'][$field->sf_fieldname] = array(
      'target' => $field->source_data,
      'keyfield' => $field->is_keyfield,
      'filter' => $include_filter ? $field->filter : NULL,
    );
  }

  // Now put the maps into the right nodes.
  foreach ($map_list as $mapid => $map) {
    $nodes[$map['nid']]->webform['salesforce'][$mapid] = $map;
  }
}

/**
 * Deletes all of the maps associated with a given node.
 *
 * @param int $nid
 *   The node ID to process.
 */
function salesforce_webforms_delete_node_maps($nid) {
  // Find the mapid of all current maps for this node.
  $maps = db_select('salesforce_webforms_map', 'm')
    ->fields('m', array('mapid'))
    ->condition('nid', $nid, '=')
    ->execute()
    ->fetchAll();

  foreach ($maps as $map) {
    $mapids[] = $map->mapid;
  }

  if (count($maps) == 0) {
    return;
  }

  // Delete the fields.
  db_delete('salesforce_webforms_field')
    ->condition('mapid', $mapids, 'IN')
    ->execute();

  // And now delete the maps.
  db_delete('salesforce_webforms_map')
    ->condition('mapid', $mapids, 'IN')
    ->execute();
}

/**
 * Rebuilds the map tables from the contents of the node object.
 */
function salesforce_webforms_save_node_maps($node) {
  if (!isset($node->webform['salesforce'])) {
    return;
  }

  $nid = $node->nid;

  // Delete the current maps.
  salesforce_webforms_delete_node_maps($nid);

  // Add back the maps.
  foreach ($node->webform['salesforce'] as $map) {
    $map['action'] = 'add';
    $map['nid'] = $nid;
    salesforce_webforms_map_save($map);
  }
}

/**
 * Deletes a mapping and dependencies.
 *
 * @param int $mapid
 *   The ID of the map to be deleted.
 */
function salesforce_webforms_delete_mapping($mapid) {
  // Delete mapping.
  db_delete('salesforce_webforms_map')
    ->condition('mapid', $mapid)
    ->execute();

  // Delete fields.
  db_delete('salesforce_webforms_field')
    ->condition('mapid', $mapid)
    ->execute();
}

/**
 * Helper function.
 *
 * Takes the full list of fields and returns a basic
 * representation for display purposes.
 *
 * @param string $obj
 *   The name of the Salesforce object to read.
 * @param array $fields
 *   An associative array of fields as defined in the map.
 *
 * @return array
 *   An associative array, where the keys are the Salesforce field names and
 *   the values are the Salesforce field labels.
 */
function salesforce_webforms_map_fields_basic($obj, array $fields) {
  $ret = array();
  $desc = salesforce_webforms_get_object_fields($obj);
  if ($fields) {
    foreach ($fields as $details) {
      $fname = $details['sf_fieldname'];
      $iskey = $details['keyfield'];
      $ret[$fname] = ($iskey ? '*' : '') . $desc[$fname]['label'];
    }
  }

  return $ret;
}

/**
 * Gets the Salesforce keys assigned to a mapping.
 *
 * @param int $sid
 *   The webform submission ID.
 *
 * @return array
 *   An associative array where the keys are the name of the map, as the
 *   values are the Salesforce IDs of the object created from those maps for
 *   the given submission.
 */
function salesforce_webforms_get_submission_keys($sid) {
  $result = db_query("SELECT mapname,sfid FROM {salesforce_webforms_key} WHERE sid = :sid", array(':sid' => $sid));
  $keys = $result->fetchAllKeyed();
  return $keys;
}

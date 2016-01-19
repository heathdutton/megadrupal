<?php

/**
 * @file
 * Core functionality for the CiviCRM Taxonomy Groups module.
 */

/**
 * Retrieves a row from the civicrm_content_types table.
 *
 * @param string $type
 *   A content type name.
 *
 * @param bool $reset
 *   If the value should be reset or loaded from cache.
 *
 * @return array
 *   An array of data for this content type.
 */
function civicrm_content_types_get($type, $reset = FALSE) {
  static $index = array();

  if (isset($index[$type]) && !$reset) {
    return $index[$type];
  }

  $query = db_select('civicrm_content_types', 'ct');
  $query->join('node_type', 'nt', 'nt.type = ct.type');
  $query->fields('ct', array('gid'))
    ->fields('nt', array('name', 'description'))
    ->condition('ct.type', $type);

  $result = $query->execute()->fetchAssoc();

  $index[$type] = $result;

  return $result;
}

/**
 * Create or update a CiviCRM group.
 *
 * @param string $name
 *   The name of the group.
 *
 * @param string $description
 *   The description of the group.
 *
 * @param int $group_id
 *   The CiviCRM group ID.
 *
 * @return int
 *   Returns the new/updated group ID.
 *
 * @see civicrm_content_types_link_write()
 */
function civicrm_content_types_group_write($name, $description, $group_id = NULL) {
  if (!civicrm_initialize()) {
    return;
  }

  module_load_include('php', 'civicrm', '/../api/v2/Group');

  $settings = variable_get('civicrm_content_types_settings', array());

  // TODO: Check if the group name doesn't already exist.
  $description = strip_tags($description);
  $description = trim($description);

  $params = array(
    'name' => $name,
    'title' => $name,
    'description' => $description,
    'is_active' => 1,
    'visibility' => 'Public Pages',
    'group_type' => array('2' => 1),
  );
  if ($group_id) {
    $params['id'] = $group_id;
  }

  $result = &civicrm_group_add($params);

  if (civicrm_error($result)) {
    drupal_set_message($result['error_message'], 'error');
  }
  else {
    return $result['result'];
  }
}

/**
 * Deletes a CiviCRM group. Wrapper for the CiviCRM API.
 *
 * @param int $group_id
 *   The ID of the group.
 *
 * @return bool|string
 *   Returns TRUE if successful. If not, returns an error from CiviCRM.
 */
function civicrm_content_types_group_delete($group_id) {
  if (!civicrm_initialize()) {
    return;
  }

  module_load_include('php', 'civicrm', '/../api/v2/Group');

  $params = array('id' => $group_id);
  $result = &civicrm_group_delete($params);

  if (civicrm_error($result)) {
    drupal_set_message($result['error_message'], 'error');
  }
  else {
    return TRUE;
  }
}

/**
 * Create or update a CiviCRM group.
 *
 * @param string $type
 *   The name of a content type.
 *
 * @see civicrm_content_types_term_insert()
 */
function civicrm_content_types_link_write($type) {
  $link = civicrm_content_types_get($type);
  $gid = NULL;
  if ($link['gid']) {
    $gid = $link['gid'];
  }

  $description = (isset($link->description)) ? $link->description : NULL;
  $group = civicrm_content_types_group_write($type, $description, $gid);

  if (!$group) {
    drupal_set_message(t('There was an error; A CiviCRM Group could not be created for %name.', array('%name' => $term->name)), 'error');
    return FALSE;
  }

  if (!isset($link['gid'])) {
    $record = array('type' => $type, 'gid' => $group->id);
    drupal_write_record('civicrm_content_types', $record);
  }

  return TRUE;
}

/**
 * Remove a CiviCRM group.
 *
 * @param string $type
 *   The name of a content type.
 *
 * @param bool $purge_group
 *   If the CiviCRM should be removed.
 */
function civicrm_content_types_link_delete($type, $purge_group = FALSE) {
  $settings = variable_get('civicrm_content_types_settings', array());
  $link = civicrm_content_types_get($type);

  if ($purge_group) {
    // Delete the group.
    civicrm_content_types_group_delete($link['gid']);
  }

  // Delete the record from the civicrm_content_types table.
  db_query("DELETE FROM {civicrm_content_types} WHERE type = :type", array(':type' => $type));
}

/**
 * Retrieves the readable name for a content type.
 *
 * @param string $type
 *   The machine-readable name of the content type.
 *
 * @return string
 *   The readable name for the content type.
 */
function civicrm_content_types_get_name($type) {
  return db_query("SELECT name FROM {node_type} WHERE type = :type", array(':type' => $type))->fetchField();
}

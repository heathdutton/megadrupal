<?php

/**
 * @file
 * Core functionality for the CiviCRM Taxonomy Groups module.
 */

/**
 *
 */
function civicrm_taxonomy_get($vid, $tid = NULL, $reset = FALSE) {
  static $index = array();

  if (isset($index[$vid][$tid]) && !$reset) {
    return $index[$vid][$tid];
  }

  $query = db_select('civicrm_taxonomy', 'ct');
  $query->fields('ct', array('vid', 'tid', 'gid'))
    ->condition('ct.vid', $vid)
    ->condition('ct.tid', $tid);

  $row = $query->execute()->fetchAssoc();

  $index[$vid][$tid] = $row;

  return $row;
}

/**
 * Create or update a CiviCRM Taxonomy group.
 *
 * @param string $name
 *   The name of the group.
 *
 * @param string description
 *   The description of the group.
 *
 * @param int $parent_id
 *
 * @param int $group_id
 *
 * @return
 *   Returns the new/updated group ID.
 *
 * @see civicrm_taxonomy_link_write()
 */
function civicrm_taxonomy_group_write($name, $description, $parent_id = NULL, $group_id = NULL) {
  if (!civicrm_initialize()) {
    return;
  }

  require_once(drupal_get_path('module', 'civicrm') . '/../api/v2/Group.php');

  $settings = variable_get('civicrm_taxonomy_settings', array());

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
  if ($parent_id) {
    $params['parents'] = $parent_id;
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
 * Deletes a CiviCRM Taxonomy group. Wrapper for the CiviCRM API.
 *
 * @param int $group_id
 *   The ID of the group.
 *
 * @return
 *   Returns TRUE if successful. If not, returns an error from CiviCRM.
 */
function civicrm_taxonomy_group_delete($group_id) {
  if (!civicrm_initialize()) {
    return;
  }

  require_once(drupal_get_path('module', 'civicrm') . '/../api/v2/Group.php');

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
 * Create or update a CiviCRM Taxonomy group.
 *
 * @param object $term
 *   A taxonomy term object.
 *
 * @see civicrm_taxonomy_term_insert()
 */
function civicrm_taxonomy_link_write($term) {
  if (is_array($term)) {
    $term = (object) $term;
  }
  $link = civicrm_taxonomy_get($term->vid, $term->tid);
  $gid = NULL;
  if ($link['gid']) {
    $gid = $link['gid'];
  }

  $parent = isset($term->parents) ? array_pop($term->parents) : NULL;
  $parent_link = civicrm_taxonomy_get($term->vid, $parent);
  $pid = $parent_link['gid'];

  $description = (isset($term->description)) ? $term->description : NULL;
  $group = civicrm_taxonomy_group_write($term->name, $description, $pid, $gid);

  if (!$group) {
    drupal_set_message('There was an error; A CiviCRM Group could not be created for '. $term->name .'.', 'error');
    return FALSE;
  }

  if (!isset($link['gid'])) {
    $record = array('tid' => $term->tid, 'vid' => $term->vid, 'gid' => $group->id);
    drupal_write_record('civicrm_taxonomy', $record);
  }

  return TRUE;
}

/**
 * Remove a CiviCRM Taxonomy group.
 *
 * @param int $vid
 *   A taxonomy vocabulary ID.
 *
 * @param int $tid
 *   A taxonomy term ID.
 *
 * @param bool $purge_group
 *   Whether the entire group should be removed.
 */
function civicrm_taxonomy_link_delete($vid, $tid, $purge_group = FALSE) {
  $settings = variable_get('civicrm_taxonomy_settings', array());
  $link = civicrm_taxonomy_get($vid, $tid);

  if ($purge_group) {
    // Delete the group.
    civicrm_taxonomy_group_delete($link['gid']);
  }

  // Delete the record from the civicrm_taxonomy table.
  db_query("DELETE FROM {civicrm_taxonomy} WHERE vid = :vid AND tid = :tid", array(':vid' => $vid, ':tid' => $tid));
}

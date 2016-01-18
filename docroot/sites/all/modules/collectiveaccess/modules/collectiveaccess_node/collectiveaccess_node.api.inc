<?php
// $Id$

/**
 * @file:
 * CollectiveAccess Node API functions for node & CA object mapping
 */

/**
 * Adds mapping between a node and a CollectiveAccess item
 * @param int nid: The node ID
 * @param int ca_item_id: The CollectiveAccess item ID
 */
function collectiveaccess_node_add($nid, $ca_item_id) {
  $existing = collectiveaccess_node_get_ca_item_id($nid);
  if (!$existing) {
    db_query("INSERT INTO {collectiveaccess_node} (nid, ca_item_id) VALUES (%d, %d)", $nid, $ca_item_id);
  }
  else if ($existing != $ca_item_id) {
    db_query("UPDATE {collectiveaccess_node} SET ca_item_id = %d WHERE nid = %d", $ca_item_id, $nid);
  }
}

/**
 * Get the CollectiveAccess item ID for a given node ID
 * @param int nid: Node ID to get the CollectiveAccess item ID for
 * @return int: the CollectiveAccess item ID
 */
function collectiveaccess_node_get_ca_item_id($nid) {
  return db_result(db_query("SELECT ca_item_id FROM {collectiveaccess_node} WHERE nid = %d", $nid));
}

/**
 * Get the node ID for a given CollectiveAccess item ID
 * @param int ca_item_id: CollectiveAccess item ID to get the node ID for
 * @return int: the node ID
 */
function collectiveaccess_node_get_nid($ca_item_id) {
  return db_result(db_query("SELECT nid FROM {collectiveaccess_node} WHERE ca_item_id = %d", $ca_item_id));
}

/**
 * Remove the mapping between a node and a CollectiveAccess item ID
 */
function collectiveaccess_node_delete($id, $type = 'nid') {
  switch ($type) {
    case 'nid':
      db_query("DELETE FROM {collectiveaccess_node} WHERE nid = %d", $id);
      break;
    case 'ca_item_id':
      db_query("DELETE FROM {collectiveaccess_node} WHERE ca_item_id = %d", $id);
      break;
  }
}
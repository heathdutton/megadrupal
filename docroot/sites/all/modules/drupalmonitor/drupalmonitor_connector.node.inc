<?php
/**
 * @file
 * This file handles node metrics.
 */

/**
 * Returns node count per content type.
 */
function drupalmonitor_connector_node_contenttypes() {
  return db_query("SELECT n.type, count(*) as counter FROM {node} n GROUP BY n.type")->fetchAllKeyed();
}

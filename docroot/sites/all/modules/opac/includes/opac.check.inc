<?php

/**
 * @file
 * Functions for opac-check drush command.
 */

/**
 * Search for missing nodes.
 *
 * @return array
 *   An array of nids.
 */
function opac_check_missing_nodes() {
  $query = db_select('opac_records', 'r');
  $query->leftjoin('node', 'n', 'r.nid = n.nid');
  $missing_nodes = $query->fields('r', array('nid'))
    ->isNull('n.nid')
    ->groupBy('r.nid')
    ->execute()
    ->fetchCol();

  return $missing_nodes;
}

/**
 * Search for missing records.
 *
 * @return array
 *   An array of nids.
 */
function opac_check_missing_records() {
  module_load_include('inc', 'opac', 'includes/opac.db');
  $opac_content_types = array_unique(array_keys(opac_get_content_types()));

  $query = db_select('node', 'n');
  $query->leftjoin('opac_records', 'r', 'n.nid = r.nid');
  $missing_records = $query->fields('n', array('nid'))
    ->isNull('r.nid')
    ->condition('n.type', $opac_content_types, 'IN')
    ->groupBy('n.nid')
    ->execute()
    ->fetchCol();

  return $missing_records;
}

/**
 * Search for duplicate nodes.
 *
 * @return array
 *   An associative array where keys are serv_id and values are an array of
 *   associative arrays where keys are recordids and values are an array of
 *   nids. Example:
 *     array(
 *       'serv_id1' => array(
 *         recordid1 => array(nid1, nid2)
 *       )
 *     )
 */
function opac_check_duplicate_nodes() {
  $query = db_select('opac_records', 'r');
  $query->addExpression("GROUP_CONCAT(r.nid SEPARATOR ',')", 'nids');
  $results = $query->fields('r', array('recordid', 'serv_id'))
    ->groupBy('recordid')
    ->groupBy('serv_id')
    ->having('COUNT(*) > 1')
    ->execute()
    ->fetchAll(PDO::FETCH_ASSOC);
  $duplicate_nodes = array();
  foreach ($results as $result) {
    $serv_id = $result['serv_id'];
    $recordid = $result['recordid'];
    $duplicate_nodes[$serv_id][$recordid] = explode(',', $result['nids']);
  }

  return $duplicate_nodes;
}

/**
 * Search for duplicate records.
 *
 * @return array
 *   An associative array where keys are nids and values are arrays of
 *   associative arrays that contains the keys 'recordid' and 'servid'. Example:
 *     array(
 *       nid1 => array(
 *         array(
 *           'recordid' => recordid1,
 *           'serv_id' => 'serv_id1',
 *         )
 *       )
 *     )
 */
function opac_check_duplicate_records() {
  $nids = db_select('opac_records', 'r')
    ->fields('r', array('nid'))
    ->groupBy('nid')
    ->having('COUNT(*) > 1')
    ->execute()
    ->fetchCol();
  $duplicate_records = array();
  foreach ($nids as $nid) {
    $results = db_select('opac_records', 'r')
      ->fields('r', array('recordid', 'serv_id'))
      ->condition('nid', $nid, '=')
      ->execute()
      ->fetchAll(PDO::FETCH_ASSOC);
    $duplicate_records[$nid] = $results;
  }

  return $duplicate_records;
}

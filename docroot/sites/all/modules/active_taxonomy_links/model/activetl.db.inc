<?php

function activetl_db_processed_nodes_info_select() {
  $query = db_select('activetl_nodes', 'atl')
    ->fields('atl')
    ->fields('n', array('type'));
  $query->join('node', 'n', 'n.nid = atl.nid');
  $query = $query->execute();
  $result = $query->fetchAllAssoc('id');
  return $result;
}

function activetl_db_node_info_select($node, $field) {
  $query = db_select('activetl_nodes', 'nodes')
    ->fields('nodes')
    ->condition('nid', $node->nid)
    ->condition('field', $field)
    ->execute();
  $result = $query->fetchAssoc();
  return $result;
}

function activetl_db_node_info_update($values, $keys = array()) {
  $result = drupal_write_record('activetl_nodes', $values, $keys);
  return $result;
}

function activetl_db_node_info_delete($node, $field) {
  $query = db_delete('activetl_nodes')
    ->condition('nid', $node->nid)
    ->condition('field', $field);
  $result = $query->execute();
  return $result;
}

function activetl_db_unprocessed_nodes_select($content_types) {
  $result = array();
  if (!empty($content_types)) {
    $query = db_select('node', 'n')
      ->fields('n', array('nid', 'type'))
      ->condition('n.type', $content_types, 'IN');
    $query->leftJoin('activetl_nodes', 'atl', 'atl.nid = n.nid');
    $query->groupBy('n.nid');
    $query->isNull('atl.value');
    $query = $query->execute();
    $result = $query->fetchAllAssoc('nid');
  }
  return $result;
}
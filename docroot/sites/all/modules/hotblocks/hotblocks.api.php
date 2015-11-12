<?php

/**
 * @file
 * Hooks provided by Hotblocks.
 */

/**
 * Allows modules to alter the list of content available to assign to a given hotblock
 * In the example, $rows is not just altered but replaced entirely
 *
 * @param $rows
 * - The rows of records (nodes, entities, or Drupal blocks) already gathered by Hotblocks based on the block's settings
 * for allowed content. Each row is a stdclass object that must have entity_type, entity_id, friendlyType, and title
 * properties.  See hotblocks_item_preload()
 * @param $delta
 * - The block delta/machine_name of the Hotblock that the user is adding content to.
 */
function hook_hotblocks_assignable_content_alter(&$rows, $delta) {
  if(stristr($delta, 'my_blocks_') !== FALSE) {

    // Query for published 'article' nodes
    $query = db_select('node', 'n');
    $query->fields('n');
    $query->orderBy('n.title', 'ASC');
    $query->innerJoin('node_type','nt', 'n.type = nt.type');
    $query->condition('n.status', '1', '=');
    $query->addField('nt', 'name', 'friendlytype');
    $query->condition('n.type', 'article', '=');

    // Timeline type should be blog
    $query->leftJoin('field_data_field_timeline_type', 'tt', 'tt.entity_id = n.nid');
    $query->condition('tt.entity_type', 'node', '=');
    $query->condition('tt.field_timeline_type_tid', TERMID_BLOG, '=');

    // Respect node access and make distinct
    $query->addTag('node_access');
    $query->distinct();

    $result = $query->execute();

    // Populate values required by hotblocks
    $rows = array();
    foreach ($result as $oRow) {
      $oRow->entity_type = 'node';
      $oRow->entity_id = $oRow->nid;
      $rows[] = $oRow;
    }
  }
}

/**
 * This hook allows the user to react to content in a Hotblock being changed.
 * @param $action
 * - A string with the values 'removed', 'added', or 'reordered'.
 * @param $delta
 * - The delta/machine_name of the hotblock being changed
 */
function hook_hotblocks_block_changed($action, $delta) {
  switch ($action) {
    case 'removed':
    case 'added':
    case 'reordered':
    default:
      cache_clear_all();
      break;
  }
}
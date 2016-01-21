<?php

/**
 * @file
 * General functions for Smart Chart Item.
 */

/**
 * Get Smart Chart Item title by id.
 *
 * @param int $sc_item_id
 *   The item nid.
 *
 * @return string
 *   Returns the title name.
 */
function sci_get_item_title_by_id($sc_item_id) {
  $query = db_select('node', 'n')
    ->fields('n', array('title'));
  $query->condition('n.nid', $sc_item_id);

  $result = $query->execute()->fetchAssoc();

  return $result['title'];
}

/**
 * Check if item has Chart Type.
 *
 * This function checks if the given Smart Chart Item id belongs inside a Smart
 * Chart that is type of the given taxonomy tid.
 *
 * @param int $sci_id
 *   The Smart Chart Item id.
 * @param int $tid
 *   The Smart Chart Type Taxonomy Term id.
 *
 * @return bool
 *   If the item has Chart Type returns TRUE, otherwise returns FALSE.
 */
function sci_item_has_chart_type($sci_id, $tid) {

  $query = db_select('field_data_field_sc_child', 'c')
    ->fields('c', array('entity_id'))
    ->condition('c.field_sc_child_target_id', $sci_id);
  $query->join('field_data_field_sc_chart', 'sc', 'c.entity_id = sc.entity_id');
  $query->join('field_data_field_sc_type', 'sct', 'sct.entity_id = sc.field_sc_chart_target_id');
  $query->condition('sct.field_sc_type_tid', $tid);
  $sci = $query->execute()->fetchAssoc();

  return !empty($sci) ? TRUE : FALSE;
}

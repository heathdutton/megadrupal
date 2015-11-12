<?php

/**
 * @file
 * Hooks provided by the Smart Chart module.
 */

/**
 * Check access for a Smart Chart.
 *
 * @param $chart_nid
 *   Smart Chart nid.
 * @param $uid
 *   User id to check for.
 *
 * @return boolean
 *   TRUE or FALSE.
 */
function hook_sc_chart_access($chart_nid, $uid) {

  $chart = node_load($chart_nid);

  return ($chart->uid == $uid);
}

/**
 * Check access for a Smart Chart item.
 *
 * @param $item
 *   Smart Chart item.
 * @param $uid
 *   User id to check for.
 *
 * @return boolean
 *   TRUE or FALSE.
 */
function hook_sc_item_access($item, $uid) {

  return ($item->uid == $uid);
}

/**
 * Get $nid of item and create/alter its structure and appearance.
 *
 * @param $item_nid
 *   The item nid from the Smart Chart.
 * @param $form_item
 *   The form item to be changed.
 * @return array
 *   The form item array.
 */
function hook_sc_item_alter($item_nid, $form_item) {

  $item = node_load($item_nid);

  $form_item = array(
    '#type' => 'markup',
    '#markup' => $item->title,
  );

  return $form_item;
}

/**
 * Alter query that gets first item.
 *
 * @param $chart_nid
 *   Chart nid.
 * @param $query
 *   Query to get first item.
 * @return object
 *   New query.
 */
function hook_sc_first_item_query_alter($chart_nid, $query) {

  $query->condition('weight.field_sc_weight_value', 0);
  $query->join('field_data_field_sc_weight', 'weight', 'n.nid = weight.entity_id');

  return $query;
}

/**
 * Alter query that gets children of item.
 *
 * @param $parent_nid
 *   Parent item nid.
 * @param $query
 *   Query to get children of item.
 * @return object
 *   New query.
 */
function hook_sc_children_query_alter($parent_nid, $query) {

  $query->condition('weight.field_sc_weight_value', 0);
  $query->join('field_data_field_sc_weight', 'weight', 'n.nid = weight.entity_id');

  return $query;
}

/**
 * Alter actions(buttons).
 *
 * @param $item_nid
 *   The item nid from the Smart Chart.
 * @param $item_actions
 *   The form item to be changed.
 * @return array
 *   The form item array.
 */
function hook_sc_item_actions($item_nid, $item_actions) {

  $item = node_load($item_nid);

  $item_actions['chart_button_item_link_' . $item_nid] = array(
    '#type' => 'markup',
    '#markup' => l($item->title, drupal_get_path_alias('node/' . $item_nid)),
  );

  return $item_actions;
}

/**
 * Alter new connection. It runs before a new connections is saved.
 *
 * @param $smart_connection
 *   Smart Connection.
 * @return $smart_connection
 *   Smart Connection altered.
 */
function hook_sc_connection_new($smart_connection) {

  $smart_connection->title = '[Altered]' . $smart_connection->title;

  return $smart_connection;
}

/**
 * Alter new item. It runs before a new item is saved.
 *
 * @param $item
 *   Smart Chart item.
 * @return $item
 *   Smart Chart item altered.
 */
function hook_sc_item_new($item) {

  $item->title = '[Altered]' . $item->title;

  return $item;
}

/**
 * Runs before item is broken from chart.
 *
 * @param $chart_nid
 *   Smart Chart nid.
 * @param $item_nid
 *   Smart Chart item nid.
 */
function hook_sc_item_break($chart_nid, $item_nid) {

  $item = node_load($item_nid);
  watchdog('smart_chart', t('Item :item broke from chart.', array(':item' => $item->title)));
}

/**
 * Runs before content type is added to Smart Chart.
 *
 * @param $type
 *   Content type.
 */
function hook_sc_add_type($type) {

  watchdog('smart_chart', t('Content type :type added to Smart Chart.', array(':type' => $type)));
}

/**
 * Run before content type is removed from Smart Chart.
 *
 * @param $type
 *   Content type.
 */
function hook_sc_remove_type($type) {

  watchdog('smart_chart', t('Content type :type removed from Smart Chart.', array(':type' => $type)));
}

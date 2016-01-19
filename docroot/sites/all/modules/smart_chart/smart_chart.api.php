<?php

/**
 * @file
 * Hooks provided by the Smart Chart module.
 */

/**
 * Check access for a Smart Chart.
 *
 * @param int $chart_nid
 *   Smart Chart nid.
 * @param $account
 *   User object to check for.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function hook_sc_chart_access($chart_nid, $account) {

  $chart = node_load($chart_nid);

  return ($chart->uid == $account->uid);
}

/**
 * Check access for a Smart Chart item.
 *
 * @param $item
 *   Smart Chart item.
 * @param $uid
 *   User id to check for.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function hook_sc_item_access($item, $uid) {

  return ($item->uid == $uid);
}

/**
 * Get $nid of item and create/alter its structure and appearance.
 *
 * @param $form_item
 *   The form item to be changed.
 * @param $context
 *   An array containing the item nid,
 *     - item_nid: Item nid.
 *
 * @return array
 *   The form item array.
 */
function hook_sc_item_alter(&$form_item, $context) {

  $item_nid = $context['item_nid'];

  $item = node_load($item_nid);

  $form_item = array(
    '#type' => 'markup',
    '#markup' => $item->title,
  );
}

/**
 * Alter query that gets first item.
 *
 * @param object $query
 *   Query to get first item.
 * @param array $data
 *   More non alterable data.
 *
 * @internal param $chart_nid Chart nid.*   Chart nid.
 * @internal param $user The user object.*   The user object.
 *
 * @return object
 *   New query.
 */
function hook_sc_first_item_query_alter($query, array $data) {

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
 *
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
 * @param $item_actions
 *   Item actions container.
 * @param $context
 *   An associative array containing:
 *     - item_nid: Item nid.
 *     - chart_nid: Chart nid.
 *     - chart_type: Chart type machine name.
 *
 * @return array
 *   The form item array.
 */
function hook_sc_actions_alter(&$item_actions, $context) {

  $item = node_load($context['item_nid']);

  $item_actions['chart_button_item_link_' . $context['item_nid']] = array(
    '#type' => 'markup',
    '#markup' => l($item->title, drupal_get_path_alias('node/' . $context['item_nid'])),
  );

  return $item_actions;
}

/**
 * Alter new connection. It runs before a new connections is saved.
 *
 * @param $smart_connection
 *   Smart Connection entity.
 *
 * @return $smart_connection
 *   Smart Connection altered entity.
 */
function hook_sc_connection_new($smart_connection) {

  $smart_connection->title = '[Altered]' . $smart_connection->title->value();

  return $smart_connection;
}

/**
 * Hook that runs after a new connection is saved.
 *
 * @param $smart_connection
 *   Smart Connection entity.
 */
function hook_sc_connection_post_new($smart_connection) {

  watchdog('smart_chart', 'New connection.');
}


/**
 * Alter new item. It runs before a new item is saved.
 *
 * @param $item
 *   Smart Chart item entity.
 *
 * @return $item
 *   Smart Chart item altered entity.
 */
function hook_sc_item_new($item) {

  $item->title = '[Altered]' . $item->title->value();

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
  watchdog('smart_chart', 'Item :item broke from chart.', array(':item' => $item->title));
}

/**
 * Runs after item is broken from chart.
 *
 * @param $chart_nid
 *   Smart Chart nid.
 * @param $item_nid
 *   Smart Chart item nid.
 */
function hook_sc_item_post_break($chart_nid, $item_nid) {

  $item = node_load($item_nid);
  watchdog('smart_chart', 'Item :item broke from chart.', array(':item' => $item->title));
}

/**
 * Runs before content type is added to Smart Chart.
 *
 * @param $type
 *   Content type.
 */
function hook_sc_add_type($type) {

  watchdog('smart_chart', 'Content type :type added to Smart Chart.', array(':type' => $type));
}

/**
 * Run before content type is removed from Smart Chart.
 *
 * @param $type
 *   Content type.
 */
function hook_sc_remove_type($type) {

  watchdog('smart_chart', 'Content type :type removed from Smart Chart.', array(':type' => $type));
}

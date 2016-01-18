<?php

/**
 * @file
 * Views hooks and callback registries.
 */

/**
 * Implements hook_views_data_alter().
 */
function uc_order_access_views_data_alter(&$data) {
  $data['uc_orders']['table']['base']['access query tag'] = 'uc_order_access_order_access';
  $data['uc_orders']['actions']['field']['handler'] = 'uc_order_access_field_order_actions';
  $data['uc_orders']['order_id']['field']['handler'] = 'uc_order_access_handler_field_order_id';
  $data['uc_orders']['order_status']['filter']['handler'] = 'uc_order_access_handler_filter_order_status';
}

/**
 * Implements hook_views_analyze().
 */
/*
function uc_order_access_views_analyze($view) {
  $ret = array();
  // Check for something other than the default display:
  if ($view->base_table == 'uc_orders') {
    foreach ($view->display as $id => $display) {
      if (empty($display->handler)) {
        continue;
      }
      if (!$display->handler->is_defaulted('access') || !$display->handler->is_defaulted('filters')) {
        // check for no access control
        $access = $display->handler->get_option('access');
        if (empty($access['type']) || $access['type'] == 'none') {
           $select = db_select('role', 'r');
          $select->innerJoin('role_permission', 'p', 'r.rid = p.rid');
          $result = $select->fields('r', array('name'))
          ->fields('p', array('permission'))
          ->condition('r.name', array('anonymous user', 'authenticated user'), 'IN')
          ->condition('p.permission', 'access content')
          ->execute();

          foreach ($result as $role) {
          $role->safe = TRUE;
          $roles[$role->name] = $role;
          }
          if (!($roles['anonymous user']->safe && $roles['authenticated user']->safe)) {
          $ret[] = views_ui_analysis(t('Some roles lack permission to access content, but display %display has no access control.', array('%display' => $display->display_title)), 'warning');
          }
          $filters = $display->handler->get_option('filters');
          foreach ($filters as $filter) {
          if ($filter['table'] == 'node' && ($filter['field'] == 'status' || $filter['field'] == 'status_extra')) {
          continue 2;
          }
          }
          $ret[] = views_ui_analysis(t('Display %display has no access control but does not contain a filter for published nodes.', array('%display' => $display->display_title)), 'warning');
        }
      }
    }
  }
  foreach ($view->display as $id => $display) {
    if ($display->display_plugin == 'page') {
      if ($display->handler->get_option('path') == 'node/%') {
        $ret[] = views_ui_analysis(t('Display %display has set node/% as path. This will not produce what you want. If you want to have multiple versions of the node view, use panels.', array('%display' => $display->display_title)), 'warning');
      }
    }
  }

  return $ret;
}
*/

<?php

/**
 * Implements hook_views_default_views_alter().
 */
function bookit_calendar_views_default_views_alter(&$views) {
  foreach (commerce_info_fields('entityreference') as $field_name => $field) {
    if ($field['settings']['target_type'] == 'bookit_item' and isset($field['bundles']['node'])) {
      foreach ($field['bundles']['node'] as $type) {
        $handler =& $views[$type . '_bookit']->display['default']->handler;

        /* Filter criterion: Booking Item: Calendar Date */
        $handler->display->display_options['filters']['date']['id'] = 'date';
        $handler->display->display_options['filters']['date']['table'] = 'bookit_item';
        $handler->display->display_options['filters']['date']['field'] = 'date';
        $handler->display->display_options['filters']['date']['relationship'] = "{$field_name}_target_id";
        $handler->display->display_options['filters']['date']['operator'] = 'between';
        $handler->display->display_options['filters']['date']['form_type'] = 'date_popup';
        $handler->display->display_options['filters']['date']['booking_group'] = "node_{$type}";
        $handler->display->display_options['filters']['date']['exposed'] = TRUE;
        $handler->display->display_options['filters']['date']['expose']['operator_id'] = 'date_op';
        $handler->display->display_options['filters']['date']['expose']['label'] = 'Date';
        $handler->display->display_options['filters']['date']['expose']['operator'] = 'date_op';
        $handler->display->display_options['filters']['date']['expose']['identifier'] = 'date';
        $handler->display->display_options['filters']['date']['expose']['remember'] = TRUE;
        $handler->display->display_options['filters']['date']['expose']['remember_roles'] = array(
          2 => 2,
          1 => 1,
          3 => 0,
        );

        $handler =& $views[$type . '_bookit_item_table']->display['default']->handler;

        /* Filter criterion: Booking Item: Calendar Date */
        $handler->display->display_options['filters']['date']['id'] = 'date';
        $handler->display->display_options['filters']['date']['table'] = 'bookit_item';
        $handler->display->display_options['filters']['date']['field'] = 'date';
        $handler->display->display_options['filters']['date']['operator'] = 'between';
        $handler->display->display_options['filters']['date']['form_type'] = 'date_popup';
        $handler->display->display_options['filters']['date']['booking_group'] = "node_{$type}";
        $handler->display->display_options['filters']['date']['expose']['operator_id'] = 'date_op';
        $handler->display->display_options['filters']['date']['expose']['label'] = 'Calendar Date';
        $handler->display->display_options['filters']['date']['expose']['operator'] = 'date_op';
        $handler->display->display_options['filters']['date']['expose']['identifier'] = 'date';
        $handler->display->display_options['filters']['date']['expose']['remember'] = TRUE;
        $handler->display->display_options['filters']['date']['expose']['remember_roles'] = array(
          2 => 2,
          1 => 1,
          0 => 0,
          3 => 0,
        );
      }
    }
  }
}

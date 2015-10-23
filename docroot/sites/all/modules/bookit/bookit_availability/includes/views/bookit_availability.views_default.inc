<?php

/**
 * Implements hook_views_default_views_alter().
 */
function bookit_availability_views_default_views_alter(&$views) {
  foreach (commerce_info_fields('entityreference') as $field_name => $field) {
    if ($field['settings']['target_type'] == 'bookit_item' and isset($field['bundles']['node'])) {
      foreach ($field['bundles']['node'] as $type) {

        $handler =& $views[$type . '_bookit_item_table']->display['default']->handler;

        $handler->display->display_options['style_options']['columns']['availability'] = 'availability';
        $handler->display->display_options['style_options']['info']['availability'] = array(
          'sortable' => 0,
          'default_sort_order' => 'asc',
          'align' => '',
          'separator' => '',
          'empty_column' => 1,
        );

        /* Field: Bookable Item: Availability */
        $handler->display->display_options['fields']['availability']['id'] = 'availability';
        $handler->display->display_options['fields']['availability']['table'] = 'bookit_item';
        $handler->display->display_options['fields']['availability']['field'] = 'availability';
        $handler->display->display_options['fields']['availability']['format_plural'] = TRUE;
        $handler->display->display_options['fields']['availability']['format_plural_singular'] = '1 room left';
        $handler->display->display_options['fields']['availability']['format_plural_plural'] = '@count rooms left';
      }
    }
  }
}

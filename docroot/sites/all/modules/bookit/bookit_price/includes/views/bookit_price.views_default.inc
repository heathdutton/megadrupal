<?php

/**
 * Implements hook_views_default_views_alter().
 */
function bookit_price_views_default_views_alter(&$views) {
  foreach (commerce_info_fields('entityreference') as $field_name => $field) {
    if ($field['settings']['target_type'] == 'bookit_item' and isset($field['bundles']['node'])) {
      foreach ($field['bundles']['node'] as $type) {
        $handler =& $views[$type . '_bookit']->display['default']->handler;

        /* Sort criterion: Bookable Item: Price */
        $handler->display->display_options['sorts'] = array(
          'price' => array(
            'id' => 'price',
            'table' => 'bookit_item',
            'field' => 'price',
            'relationship' => 'field_room_target_id',
          ),
        ) + $handler->display->display_options['sorts'];

        $handler =& $views[$type . '_bookit_item_table']->display['default']->handler;

        $handler->display->display_options['style_options']['columns']['price'] = 'price';
        $handler->display->display_options['style_options']['info']['price'] = array(
          'sortable' => 0,
          'default_sort_order' => 'asc',
          'align' => '',
          'separator' => '',
          'empty_column' => 1,
        );

        /* Sort criterion: Bookable Item: Price */
        $handler->display->display_options['sorts']['price']['id'] = 'price';
        $handler->display->display_options['sorts']['price']['table'] = 'bookit_item';
        $handler->display->display_options['sorts']['price']['field'] = 'price';

        /* Field: Bookable Item: Price */
        $handler->display->display_options['fields']['price']['id'] = 'price';
        $handler->display->display_options['fields']['price']['table'] = 'bookit_item';
        $handler->display->display_options['fields']['price']['field'] = 'price';
        $handler->display->display_options['fields']['price']['settings'] = array(
          'calculation' => FALSE,
        );

        if (isset($views[$type . '_bookit_item_form'])) {
          $handler =& $views[$type . '_bookit_item_form']->display['default']->handler;

          /* Field: Bookable Item: Price */
          $handler->display->display_options['fields'] = array_merge(
            array(
              'title' => array(),
              'price' => array(
                'id' => 'price',
                'table' => 'bookit_item',
                'field' => 'price',
                'element_label_colon' => FALSE,
                'settings' => array(
                  'calculation' => FALSE,
                ),
              ),
            ),
            $handler->display->display_options['fields']
          );

          /* Sort criterion: Bookable Item: Price */
          $handler->display->display_options['sorts']['price']['id'] = 'price';
          $handler->display->display_options['sorts']['price']['table'] = 'bookit_item';
          $handler->display->display_options['sorts']['price']['field'] = 'price';
        }
      }
    }
  }
}

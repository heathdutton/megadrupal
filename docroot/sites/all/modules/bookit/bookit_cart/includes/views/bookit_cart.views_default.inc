<?php

/**
 * Implements hook_views_default_views().
 */
function bookit_cart_views_default_views() {
  $views = array();

  // Foreach entity reference field that references a bookit_item for a node.
  foreach (commerce_info_fields('entityreference') as $field_name => $field) {
    if ($field['settings']['target_type'] == 'bookit_item' and isset($field['bundles']['node'])) {
      foreach ($field['bundles']['node'] as $type) {
        // Load the content type.
        $content_type = node_type_load($type);

        // Create a default view for this content type.
        $view = new view();
        $view->name = "{$type}_bookit_item_form";
        $view->description = "Booking add to cart form for the content type {$content_type->name}";
        $view->tag = 'bookit';
        $view->base_table = 'bookit_item';
        $view->human_name = drupal_ucfirst($content_type->name) . ' booking form';
        $view->core = 7;
        $view->api_version = '3.0';
        $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

        /* Display: Master */
        $handler = $view->new_display('default', 'Master', 'default');
        $handler->display->display_options['use_ajax'] = TRUE;
        $handler->display->display_options['use_more_always'] = FALSE;
        $handler->display->display_options['access']['type'] = 'none';
        $handler->display->display_options['cache']['type'] = 'none';
        $handler->display->display_options['query']['type'] = 'views_query';
        $handler->display->display_options['exposed_form']['type'] = 'basic';
        $handler->display->display_options['pager']['type'] = 'none';
        $handler->display->display_options['pager']['options']['offset'] = '0';
        $handler->display->display_options['style_plugin'] = 'table';
        $handler->display->display_options['style_options']['columns'] = array(
          'item_id' => 'item_id',
        );
        $handler->display->display_options['style_options']['default'] = '-1';
        $handler->display->display_options['style_options']['info'] = array(
          'item_id' => array(
            'sortable' => 0,
            'default_sort_order' => 'asc',
            'align' => '',
            'separator' => '',
            'empty_column' => 0,
          ),
        );
        /* Field: Bookable Item: Title */
        $handler->display->display_options['fields']['title']['id'] = 'title';
        $handler->display->display_options['fields']['title']['table'] = 'bookit_item';
        $handler->display->display_options['fields']['title']['field'] = 'title';
        $handler->display->display_options['fields']['title']['link_to_item'] = 0;

        /* Field: Bookable Item: Booking Quantity */
        $handler->display->display_options['fields']['booking_quantity']['id'] = 'booking_quantity';
        $handler->display->display_options['fields']['booking_quantity']['table'] = 'bookit_item';
        $handler->display->display_options['fields']['booking_quantity']['field'] = 'booking_quantity';
        $handler->display->display_options['fields']['booking_quantity']['label'] = 'Quantity';
        $handler->display->display_options['fields']['booking_quantity']['element_label_colon'] = FALSE;

        /* Field: Bookable Item: Booking Button */
        $handler->display->display_options['fields']['booking_button']['id'] = 'booking_button';
        $handler->display->display_options['fields']['booking_button']['table'] = 'bookit_item';
        $handler->display->display_options['fields']['booking_button']['field'] = 'booking_button';
        $handler->display->display_options['fields']['booking_button']['label'] = 'Book Now';
        $handler->display->display_options['fields']['booking_button']['element_label_colon'] = FALSE;

        /* Contextual filter: Bookable Item: Bookable Item ID */
        $handler->display->display_options['arguments']['item_id']['id'] = 'item_id';
        $handler->display->display_options['arguments']['item_id']['table'] = 'bookit_item';
        $handler->display->display_options['arguments']['item_id']['field'] = 'item_id';
        $handler->display->display_options['arguments']['item_id']['default_action'] = 'not found';
        $handler->display->display_options['arguments']['item_id']['default_argument_type'] = 'fixed';
        $handler->display->display_options['arguments']['item_id']['summary']['number_of_records'] = '0';
        $handler->display->display_options['arguments']['item_id']['summary']['format'] = 'default_summary';
        $handler->display->display_options['arguments']['item_id']['summary_options']['items_per_page'] = '25';
        $handler->display->display_options['arguments']['item_id']['break_phrase'] = TRUE;

        /* Filter criterion: Bookable Item: Calendar Date */
        $handler->display->display_options['filters']['date']['id'] = 'date';
        $handler->display->display_options['filters']['date']['table'] = 'bookit_item';
        $handler->display->display_options['filters']['date']['field'] = 'date';
        $handler->display->display_options['filters']['date']['operator'] = 'between';
        $handler->display->display_options['filters']['date']['exposed'] = TRUE;
        $handler->display->display_options['filters']['date']['expose']['operator_id'] = 'date_op';
        $handler->display->display_options['filters']['date']['expose']['operator'] = 'date_op';
        $handler->display->display_options['filters']['date']['expose']['identifier'] = 'date';
        $handler->display->display_options['filters']['date']['expose']['remember'] = TRUE;
        $handler->display->display_options['filters']['date']['expose']['remember_roles'] = array(
          2 => 2,
          1 => 1,
          0 => 0,
          3 => 0,
        );
        $handler->display->display_options['filters']['date']['form_type'] = 'date_popup';
        $handler->display->display_options['filters']['date']['booking_group'] = "node_{$type}";

        /* Sort criterion: Bookable Item: Bookable Item ID */
        $handler->display->display_options['sorts']['item_id']['id'] = 'item_id';
        $handler->display->display_options['sorts']['item_id']['table'] = 'bookit_item';
        $handler->display->display_options['sorts']['item_id']['field'] = 'item_id';


        // Add the view to views list.
        $views[$view->name] = $view;
      }
    }
  }

  return $views;
}

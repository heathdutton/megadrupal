<?php

/**
 * Implements hook_views_default_views().
 */
function bookit_option_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'bookit_options';
  $view->description = 'Display a list of booking options for booking platform admin.';
  $view->tag = 'bookit';
  $view->base_table = 'bookit_option';
  $view->human_name = 'Booking Options';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Booking Options';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer bookit_option entities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'type' => 'type',
    'operations' => 'operations',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* No results behavior: Booking Option: Empty text */
  $handler->display->display_options['empty']['empty_text']['id'] = 'empty_text';
  $handler->display->display_options['empty']['empty_text']['table'] = 'bookit_option';
  $handler->display->display_options['empty']['empty_text']['field'] = 'empty_text';
  $handler->display->display_options['empty']['empty_text']['empty'] = TRUE;
  /* Field: Booking Option: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'bookit_option';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['link_to_option'] = 1;
  /* Field: Booking Option: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'bookit_option';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['link_to_option'] = 0;
  $handler->display->display_options['fields']['type']['use_raw_value'] = 0;
  /* Field: Booking Option: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'bookit_option';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = 'Operations';
  $handler->display->display_options['fields']['operations']['add_destination'] = 1;
  /* Sort criterion: Booking Option: Booking Option ID */
  $handler->display->display_options['sorts']['option_id']['id'] = 'option_id';
  $handler->display->display_options['sorts']['option_id']['table'] = 'bookit_option';
  $handler->display->display_options['sorts']['option_id']['field'] = 'option_id';

  /* Display: Admin Page */
  $handler = $view->new_display('page', 'Admin Page', 'admin_page');
  $handler->display->display_options['path'] = 'admin/bookit/config/options/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Booking Options';
  $handler->display->display_options['tab_options']['description'] = 'Manage options and option types in the booking platform.';
  $handler->display->display_options['tab_options']['weight'] = '';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['bookit_options'] = array(
    t('Master'),
    t('Booking Options'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Title'),
    t('Type'),
    t('Operations'),
    t('Admin Page'),
  );

  $views[$view->name] = $view;

  return $views;
}

/**
 * Implements hook_views_default_views_alter().
 */
function bookit_option_views_default_views_alter(&$views) {
  // TODO: Rebuild this.
  // foreach (commerce_info_fields('entityreference') as $field_name => $field) {
  //   if ($field['settings']['target_type'] == 'bookit_item' and isset($field['bundles']['node'])) {
  //     foreach ($field['bundles']['node'] as $type) {
  //       if (isset($views[$type . '_bookit_item_form'])) {
  //         $handler =& $views[$type . '_bookit_item_form']->display['default']->handler;

  //         // Handle the field sorting.
  //         $handler->display->display_options['fields'] = array_merge(
  //           array(
  //             'title' => array(),
  //             'bookit_options' => array(),
  //             'price' => array(),
  //             'price_1' => array(),
  //             'booking_quantity' => array(),
  //             'booking_button' => array(),
  //             'booking_button_1' => array(),
  //           ),
  //           $handler->display->display_options['fields']
  //         );

  //         /* Field: Bookable Item: Booking Options */
  //         $handler->display->display_options['fields']['bookit_options']['id'] = 'bookit_options';
  //         $handler->display->display_options['fields']['bookit_options']['table'] = 'field_data_bookit_options';
  //         $handler->display->display_options['fields']['bookit_options']['field'] = 'bookit_options';
  //         $handler->display->display_options['fields']['bookit_options']['label'] = 'Option';
  //         $handler->display->display_options['fields']['bookit_options']['settings'] = array(
  //           'link' => 0,
  //         );

  //         /* Field: Bookable Item: Booking Options Price */
  //         $handler->display->display_options['fields']['price']['exclude'] = TRUE;
  //         $handler->display->display_options['fields']['price_1']['id'] = 'price_1';
  //         $handler->display->display_options['fields']['price_1']['table'] = 'field_data_bookit_options';
  //         $handler->display->display_options['fields']['price_1']['field'] = 'price';
  //         $handler->display->display_options['fields']['price_1']['label'] = 'Price';
  //         $handler->display->display_options['fields']['price_1']['empty'] = '[price]';
  //         $handler->display->display_options['fields']['price_1']['settings'] = array(
  //           'calculation' => FALSE,
  //         );

  //         /* Field: Bookable Item: Booking Options Booking Button */
  //         $handler->display->display_options['fields']['booking_button']['exclude'] = TRUE;
  //         $handler->display->display_options['fields']['booking_button_1']['id'] = 'booking_button_1';
  //         $handler->display->display_options['fields']['booking_button_1']['table'] = 'field_data_bookit_options';
  //         $handler->display->display_options['fields']['booking_button_1']['field'] = 'booking_button';
  //         $handler->display->display_options['fields']['booking_button_1']['label'] = 'Book Now';
  //         $handler->display->display_options['fields']['booking_button_1']['empty'] = '[booking_button]';

  //         /* Sort criterion: Bookable Item: Booking Options Price */
  //         $handler->display->display_options['sorts']['price']['id'] = 'price';
  //         $handler->display->display_options['sorts']['price']['table'] = 'field_data_bookit_options';
  //         $handler->display->display_options['sorts']['price']['field'] = 'price';
  //       }
  //     }
  //   }
  // }
}

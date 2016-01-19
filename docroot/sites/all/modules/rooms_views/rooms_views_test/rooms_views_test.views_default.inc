<?php

/**
 * @file
 * Dummy views to test Rooms Views module
 */

/**
 * Implements hook_views_default_views().
 */
function rooms_views_test_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'rooms_views_test_default_view';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'rooms_units';
  $view->human_name = 'rooms_views_test_default_view';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'rooms_views_test_default_view';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Bookable Units: Bookable units ID */
  $handler->display->display_options['fields']['unit_id']['id'] = 'unit_id';
  $handler->display->display_options['fields']['unit_id']['table'] = 'rooms_units';
  $handler->display->display_options['fields']['unit_id']['field'] = 'unit_id';
  /* Field: Bookable Units: Booking form */
  $handler->display->display_options['fields']['booking']['id'] = 'booking';
  $handler->display->display_options['fields']['booking']['table'] = 'rooms_units';
  $handler->display->display_options['fields']['booking']['field'] = 'booking';
  $handler->display->display_options['fields']['booking']['text'] = 'Book this awesome room';
  /* Filter criterion: Bookable Units: Rooms Availability */
  $handler->display->display_options['filters']['availability']['id'] = 'availability';
  $handler->display->display_options['filters']['availability']['table'] = 'rooms_units';
  $handler->display->display_options['filters']['availability']['field'] = 'availability';
  $handler->display->display_options['filters']['availability']['value'] = array(
    'rooms_start_date' => DateTime::__set_state(array(
      'date' => '2014-10-24 00:00:00',
      'timezone_type' => 3,
      'timezone' => 'Europe/Madrid',
    )),
    'rooms_end_date' => DateTime::__set_state(array(
      'date' => '2014-10-31 00:00:00',
      'timezone_type' => 3,
      'timezone' => 'Europe/Madrid',
    )),
  );
  $handler->display->display_options['filters']['availability']['exposed'] = TRUE;
  $handler->display->display_options['filters']['availability']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['availability']['expose']['label'] = 'Rooms Availability';
  $handler->display->display_options['filters']['availability']['expose']['operator'] = 'availability_op';
  $handler->display->display_options['filters']['availability']['expose']['identifier'] = 'availability';
  $handler->display->display_options['filters']['availability']['expose']['required'] = TRUE;
  $handler->display->display_options['filters']['availability']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'rooms-views-test-default-view';

  $views['rooms_views_test_default_view'] = $view;
  return $views;
}

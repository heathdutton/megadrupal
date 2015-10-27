<?php

/**
 * @file
 * Provides default view for the studyroom_space module.
 */

/**
 * Implements hook_views_default_views().
 */
function studyroom_reservation_views_default_views() {
  $view = new view();
  $view->name = 'studyroom_reservations';
  $view->description = 'A list of all reservations';
  $view->tag = 'studyroom, reservation';
  $view->base_table = 'studyroom_reservation';
  $view->human_name = 'Studyroom Reservations';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Reservations';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access reservations overview';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'reservation_id' => 'reservation_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'reservation_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty ';
  $handler->display->display_options['empty']['area']['content'] = 'No reservations have been created yet';
  if (module_exists('views_bulk_operations')) {
    /* Field: Bulk operations: Reservation */
    $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
    $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'studyroom_reservation';
    $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
    $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
    $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
      'action::views_bulk_operations_delete_item' => array(
        'selected' => 1,
        'postpone_processing' => 0,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
      ),
      'action::views_bulk_operations_script_action' => array(
        'selected' => 0,
        'postpone_processing' => 0,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
      ),
      'action::views_bulk_operations_modify_action' => array(
        'selected' => 0,
        'postpone_processing' => 0,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
        'settings' => array(
          'show_all_tokens' => 1,
          'display_values' => array(
            '_all_' => '_all_',
          ),
        ),
      ),
      'action::views_bulk_operations_argument_selector_action' => array(
        'selected' => 0,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
        'settings' => array(
          'url' => '',
        ),
      ),
      'action::system_send_email_action' => array(
        'selected' => 0,
        'postpone_processing' => 0,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
      ),
    );
  }
  /* Field: Reservation: Reservation ID */
  $handler->display->display_options['fields']['reservation_id']['id'] = 'reservation_id';
  $handler->display->display_options['fields']['reservation_id']['table'] = 'studyroom_reservation';
  $handler->display->display_options['fields']['reservation_id']['field'] = 'reservation_id';
  $handler->display->display_options['fields']['reservation_id']['separator'] = '';
  /* Field: Reservation: Link */
  $handler->display->display_options['fields']['link_studyroom_reservation']['id'] = 'link_studyroom_reservation';
  $handler->display->display_options['fields']['link_studyroom_reservation']['table'] = 'studyroom_reservation';
  $handler->display->display_options['fields']['link_studyroom_reservation']['field'] = 'link_studyroom_reservation';
  $handler->display->display_options['fields']['link_studyroom_reservation']['label'] = 'View';
  /* Field: Reservation: Reservation Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'studyroom_reservation';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Reservation: Reservation datetime */
  $handler->display->display_options['fields']['field_reservation_datetime']['id'] = 'field_reservation_datetime';
  $handler->display->display_options['fields']['field_reservation_datetime']['table'] = 'field_data_field_reservation_datetime';
  $handler->display->display_options['fields']['field_reservation_datetime']['field'] = 'field_reservation_datetime';
  $handler->display->display_options['fields']['field_reservation_datetime']['label'] = 'Date/Time';
  $handler->display->display_options['fields']['field_reservation_datetime']['settings'] = array(
    'format_type' => 'medium',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Reservation: Duration */
  $handler->display->display_options['fields']['duration']['id'] = 'duration';
  $handler->display->display_options['fields']['duration']['table'] = 'studyroom_reservation';
  $handler->display->display_options['fields']['duration']['field'] = 'duration';
  $handler->display->display_options['fields']['duration']['granularity'] = '2';
  /* Field: Reservation: Space ID */
  $handler->display->display_options['fields']['space_id']['id'] = 'space_id';
  $handler->display->display_options['fields']['space_id']['table'] = 'studyroom_reservation';
  $handler->display->display_options['fields']['space_id']['field'] = 'space_id';
  $handler->display->display_options['fields']['space_id']['label'] = 'Space';
  $handler->display->display_options['fields']['space_id']['separator'] = '';
  $handler->display->display_options['fields']['space_id']['space_name'] = TRUE;
  /* Field: Reservation: occupancy */
  $handler->display->display_options['fields']['field_reservation_occupancy']['id'] = 'field_reservation_occupancy';
  $handler->display->display_options['fields']['field_reservation_occupancy']['table'] = 'field_data_field_reservation_occupancy';
  $handler->display->display_options['fields']['field_reservation_occupancy']['field'] = 'field_reservation_occupancy';
  $handler->display->display_options['fields']['field_reservation_occupancy']['label'] = 'Occupancy';
  $handler->display->display_options['fields']['field_reservation_occupancy']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 0,
  );
  /* Field: Reservation: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'studyroom_reservation';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'studyroom_reservations_admin_page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = STUDYROOM_RESERVATION_ADMIN_PATH . '/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'Reservations';
  $handler->display->display_options['tab_options']['description'] = 'Manage reservations';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  $views[$view->name] = $view;
  return $views;
}

<?php

/**
 * @file
 * Provide default view for studyroom_closed module.
 */

/**
 * Implements hook_views_default_views().
 */
function studyroom_closed_views_default_views() {
  $view = new view();
  $view->name = 'Closed reservations';
  $view->description = 'A list of all closed reservations';
  $view->tag = 'studyroom';
  $view->base_table = 'studyroom_closed';
  $view->human_name = 'Closed reservations';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Studyroom closed reservations';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer studyroom_closed';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'operations' => 'operations',
    'conflict_studyroom_closed' => 'conflict_studyroom_closed',
    'field_studyroom_closed_dates' => 'field_studyroom_closed_dates',
    'spaces' => 'spaces',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
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
    'conflict_studyroom_closed' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_studyroom_closed_dates' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'spaces' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty ';
  $handler->display->display_options['empty']['area']['content'] = 'No closed reservations have been created yet';
  if (module_exists('views_bulk_operations')) {
    /* Field: Bulk operations: Closed Reservation */
    $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
    $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'studyroom_closed';
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
  /* Field: Closed Reservation: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'studyroom_closed';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  /* Field: Closed Reservation: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'studyroom_closed';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  /* Field: Closed Reservation: Conflicts */
  $handler->display->display_options['fields']['conflict_studyroom_closed']['id'] = 'conflict_studyroom_closed';
  $handler->display->display_options['fields']['conflict_studyroom_closed']['table'] = 'studyroom_closed';
  $handler->display->display_options['fields']['conflict_studyroom_closed']['field'] = 'conflict_studyroom_closed';
  /* Field: Closed Reservation: Dates */
  $handler->display->display_options['fields']['field_studyroom_closed_dates']['id'] = 'field_studyroom_closed_dates';
  $handler->display->display_options['fields']['field_studyroom_closed_dates']['table'] = 'field_data_field_studyroom_closed_dates';
  $handler->display->display_options['fields']['field_studyroom_closed_dates']['field'] = 'field_studyroom_closed_dates';
  $handler->display->display_options['fields']['field_studyroom_closed_dates']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_studyroom_closed_dates']['settings'] = array(
    'format_type' => 'medium',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_studyroom_closed_dates']['delta_offset'] = '0';
  /* Field: Closed Reservation: Spaces */
  $handler->display->display_options['fields']['spaces']['id'] = 'spaces';
  $handler->display->display_options['fields']['spaces']['table'] = 'studyroom_closed';
  $handler->display->display_options['fields']['spaces']['field'] = 'spaces';
  $handler->display->display_options['fields']['spaces']['empty'] = 'No spaces';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'studyroom_closeds_admin_page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/studyroom/closed';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Closed Reservations';
  $handler->display->display_options['menu']['description'] = 'Add, edit and delete closed reservations.';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'Closed Reservations';
  $handler->display->display_options['tab_options']['description'] = 'Manage closed reservations';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  $views[$view->name] = $view;
  return $views;
}

<?php
/**
 * @file
 * tutor_statistics.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function tutor_statistics_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'tutor_exercises';
  $view->description = 'A list of tutor exercises on the site, along with streak statistics for the acting user.';
  $view->tag = 'Tutor';
  $view->base_table = 'node';
  $view->human_name = 'Tutor exercises';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Exercises';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'tutor_streak_max' => 'tutor_streak_max',
    'tutor_streak_current' => 'tutor_streak_current',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'tutor_streak_max' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'tutor_streak_current' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Tutor question statistics: Answer statistics */
  $handler->display->display_options['relationships']['eck_tutor_statistics']['id'] = 'eck_tutor_statistics';
  $handler->display->display_options['relationships']['eck_tutor_statistics']['table'] = 'node';
  $handler->display->display_options['relationships']['eck_tutor_statistics']['field'] = 'eck_tutor_statistics';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Exercise';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Tutor question statistics: Best streak */
  $handler->display->display_options['fields']['tutor_streak_max']['id'] = 'tutor_streak_max';
  $handler->display->display_options['fields']['tutor_streak_max']['table'] = 'field_data_tutor_streak_max';
  $handler->display->display_options['fields']['tutor_streak_max']['field'] = 'tutor_streak_max';
  $handler->display->display_options['fields']['tutor_streak_max']['relationship'] = 'eck_tutor_statistics';
  /* Field: Tutor question statistics: Current streak */
  $handler->display->display_options['fields']['tutor_streak_current']['id'] = 'tutor_streak_current';
  $handler->display->display_options['fields']['tutor_streak_current']['table'] = 'field_data_tutor_streak_current';
  $handler->display->display_options['fields']['tutor_streak_current']['field'] = 'tutor_streak_current';
  $handler->display->display_options['fields']['tutor_streak_current']['relationship'] = 'eck_tutor_statistics';
  $handler->display->display_options['fields']['tutor_streak_current']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'tutor_exercise' => 'tutor_exercise',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pane_category']['name'] = 'Tutor';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['allow']['use_pager'] = 0;
  $handler->display->display_options['allow']['items_per_page'] = 0;
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 0;
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 'title_override';
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 0;
  $handler->display->display_options['inherit_panels_path'] = '1';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['enabled'] = FALSE;
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'exercises';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Exercises';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $export['tutor_exercises'] = $view;

  $view = new view();
  $view->name = 'tutor_statistics';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'eck_tutor_statistics';
  $view->human_name = 'Question statistics';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Your stats for this question';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Tutor question statistics: Current streak */
  $handler->display->display_options['fields']['tutor_streak_current']['id'] = 'tutor_streak_current';
  $handler->display->display_options['fields']['tutor_streak_current']['table'] = 'field_data_tutor_streak_current';
  $handler->display->display_options['fields']['tutor_streak_current']['field'] = 'tutor_streak_current';
  $handler->display->display_options['fields']['tutor_streak_current']['element_type'] = 'span';
  $handler->display->display_options['fields']['tutor_streak_current']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['tutor_streak_current']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  /* Field: Tutor question statistics: Best streak */
  $handler->display->display_options['fields']['tutor_streak_max']['id'] = 'tutor_streak_max';
  $handler->display->display_options['fields']['tutor_streak_max']['table'] = 'field_data_tutor_streak_max';
  $handler->display->display_options['fields']['tutor_streak_max']['field'] = 'tutor_streak_max';
  $handler->display->display_options['fields']['tutor_streak_max']['element_type'] = 'span';
  $handler->display->display_options['fields']['tutor_streak_max']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['tutor_streak_max']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  /* Contextual filter: Tutor question statistics: Question node */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'eck_tutor_statistics';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  /* Contextual filter: Tutor question statistics: User */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'eck_tutor_statistics';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;

  /* Display: EVA Field */
  $handler = $view->new_display('entity_view', 'EVA Field', 'entity_view_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'tutor_exercise',
  );
  $export['tutor_statistics'] = $view;

  return $export;
}

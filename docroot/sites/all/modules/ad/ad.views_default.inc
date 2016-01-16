<?php
/**
 * @file
 * ad.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function ad_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'advertisement_statistics';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'eck_tracked_event';
  $view->human_name = 'Advertisement Statistics';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Advertisement Statistics';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer ads';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
    'created' => 'created',
    'url' => 'url',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'url' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Tracked Event: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'eck_tracked_event';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Timestamp';
  $handler->display->display_options['fields']['created']['date_format'] = 'medium';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Tracked Event: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'eck_tracked_event';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  /* Field: Tracked Event: tracked_event type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'eck_tracked_event';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = 'Type';
  /* Contextual filter: Tracked Event: ad */
  $handler->display->display_options['arguments']['ad']['id'] = 'ad';
  $handler->display->display_options['arguments']['ad']['table'] = 'eck_tracked_event';
  $handler->display->display_options['arguments']['ad']['field'] = 'ad';
  $handler->display->display_options['arguments']['ad']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['ad']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['ad']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['ad']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['ad']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Tracked Event: tracked_event type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'eck_tracked_event';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'click' => 'click',
    'impression' => 'impression',
  );
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/ad/stats';
  $export['advertisement_statistics'] = $view;

  return $export;
}

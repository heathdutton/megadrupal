<?php
/**
 * @file
 * Exports Yandex Map view configuration.
 */

/**
 * Implements hook_views_default_views().
 */
function yamaps_example_feature_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'yandex_maps_views_example';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Yandex Maps View';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Yandex Maps View';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default_map';
  $handler->display->display_options['style_options']['yandex_map_field'] = 'field_yandex_maps_example_field';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Yandex Map */
  $handler->display->display_options['fields']['field_yandex_maps_example_field']['id'] = 'field_yandex_maps_example_field';
  $handler->display->display_options['fields']['field_yandex_maps_example_field']['table'] = 'field_data_field_yandex_maps_example_field';
  $handler->display->display_options['fields']['field_yandex_maps_example_field']['field'] = 'field_yandex_maps_example_field';
  $handler->display->display_options['fields']['field_yandex_maps_example_field']['label'] = '';
  $handler->display->display_options['fields']['field_yandex_maps_example_field']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_yandex_maps_example_field']['click_sort_column'] = 'coords';
  $handler->display->display_options['fields']['field_yandex_maps_example_field']['settings'] = array(
    'controls' => 1,
    'traffic' => 1,
    'width' => '100%',
    'height' => '400px',
    'behaviors' => array(
      'clickZoom' => 'clickZoom',
      'scrollZoom' => 'scrollZoom',
      'dblClickZoom' => 'dblClickZoom',
      'drag' => 'drag',
      'ruler' => 'ruler',
      'rightMouseButtonMagnifier' => 'rightMouseButtonMagnifier',
      'multiTouch' => 0,
    ),
    'yamaps_display_options' => array(
      'display_type' => 'map',
      'open_button_text' => 'Open map',
    ),
  );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
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
    'yandex_maps_example' => 'yandex_maps_example',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'yandex-maps-views-example';
  $export['yandex_maps_views_example'] = $view;

  return $export;
}

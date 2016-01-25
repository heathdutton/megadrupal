<?php
/**
 * @file
 * Default example view.
 */

/**
 * Implements hook_views_default_views().
 */
function views_isotope_example_views_default_views() {
  $view = new view();
  $view->name = 'isotope_example';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Isotope Example';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Isotope Example';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'isotope_grid';
  $handler->display->display_options['style_options']['data_fields'] = array(
    'views_isotope_example_color' => 'views_isotope_example_color',
    'views_isotope_example_shape' => 'views_isotope_example_shape',
    'views_isotope_example_size' => 'views_isotope_example_size',
  );
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Field: Content: Color */
  $handler->display->display_options['fields']['views_isotope_example_color']['id'] = 'views_isotope_example_color';
  $handler->display->display_options['fields']['views_isotope_example_color']['table'] = 'field_data_views_isotope_example_color';
  $handler->display->display_options['fields']['views_isotope_example_color']['field'] = 'views_isotope_example_color';
  /* Field: Content: Shape */
  $handler->display->display_options['fields']['views_isotope_example_shape']['id'] = 'views_isotope_example_shape';
  $handler->display->display_options['fields']['views_isotope_example_shape']['table'] = 'field_data_views_isotope_example_shape';
  $handler->display->display_options['fields']['views_isotope_example_shape']['field'] = 'views_isotope_example_shape';
  /* Field: Content: Size */
  $handler->display->display_options['fields']['views_isotope_example_size']['id'] = 'views_isotope_example_size';
  $handler->display->display_options['fields']['views_isotope_example_size']['table'] = 'field_data_views_isotope_example_size';
  $handler->display->display_options['fields']['views_isotope_example_size']['field'] = 'views_isotope_example_size';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'views_isotope_example' => 'views_isotope_example',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Description */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['ui_name'] = 'Description';
  $handler->display->display_options['header']['area']['content'] = 'You can explore this example for yourself at <strong>/admin/structure/views/view/isotope_example/edit/</strong>';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  /* Header: Color Filter */
  $handler->display->display_options['header']['view_1']['id'] = 'view_1';
  $handler->display->display_options['header']['view_1']['table'] = 'views';
  $handler->display->display_options['header']['view_1']['field'] = 'view';
  $handler->display->display_options['header']['view_1']['ui_name'] = 'Color Filter';
  $handler->display->display_options['header']['view_1']['view_to_insert'] = 'isotope_example:color_filter';
  /* Header: Shape Filter */
  $handler->display->display_options['header']['view_2']['id'] = 'view_2';
  $handler->display->display_options['header']['view_2']['table'] = 'views';
  $handler->display->display_options['header']['view_2']['field'] = 'view';
  $handler->display->display_options['header']['view_2']['ui_name'] = 'Shape Filter';
  $handler->display->display_options['header']['view_2']['view_to_insert'] = 'isotope_example:shape_filter';
  /* Header: Size Filter */
  $handler->display->display_options['header']['view_3']['id'] = 'view_3';
  $handler->display->display_options['header']['view_3']['table'] = 'views';
  $handler->display->display_options['header']['view_3']['field'] = 'view';
  $handler->display->display_options['header']['view_3']['ui_name'] = 'Size Filter';
  $handler->display->display_options['header']['view_3']['view_to_insert'] = 'isotope_example:size_filter';
  /* Header: Sorter */
  $handler->display->display_options['header']['view']['id'] = 'view';
  $handler->display->display_options['header']['view']['table'] = 'views';
  $handler->display->display_options['header']['view']['field'] = 'view';
  $handler->display->display_options['header']['view']['ui_name'] = 'Sorter';
  $handler->display->display_options['header']['view']['label'] = 'Sorter';
  $handler->display->display_options['header']['view']['view_to_insert'] = 'isotope_example:sorter';
  $handler->display->display_options['path'] = 'isotope-example/views-example';

  /* Display: Sorter */
  $handler = $view->new_display('block', 'Sorter', 'sorter');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'isotope_sorter';
  $handler->display->display_options['style_options']['data_fields'] = array(
    'views_isotope_example_color' => 'views_isotope_example_color',
    'views_isotope_example_shape' => 'views_isotope_example_shape',
    'views_isotope_example_size' => 'views_isotope_example_size',
    'title' => 0,
  );
  $handler->display->display_options['style_options']['original'] = 'Original';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;

  /* Display: Color Filter */
  $handler = $view->new_display('block', 'Color Filter', 'color_filter');
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'isotope_filter';
  $handler->display->display_options['style_options']['filter_group'] = 'Color';
  $handler->display->display_options['style_options']['data_fields'] = 'views_isotope_example_color';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Color */
  $handler->display->display_options['fields']['views_isotope_example_color']['id'] = 'views_isotope_example_color';
  $handler->display->display_options['fields']['views_isotope_example_color']['table'] = 'field_data_views_isotope_example_color';
  $handler->display->display_options['fields']['views_isotope_example_color']['field'] = 'views_isotope_example_color';
  $handler->display->display_options['fields']['views_isotope_example_color']['label'] = '';
  $handler->display->display_options['fields']['views_isotope_example_color']['element_label_colon'] = FALSE;

  /* Display: Size Filter */
  $handler = $view->new_display('block', 'Size Filter', 'size_filter');
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'isotope_filter';
  $handler->display->display_options['style_options']['filter_group'] = 'Size';
  $handler->display->display_options['style_options']['data_fields'] = 'views_isotope_example_size';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Size */
  $handler->display->display_options['fields']['views_isotope_example_size']['id'] = 'views_isotope_example_size';
  $handler->display->display_options['fields']['views_isotope_example_size']['table'] = 'field_data_views_isotope_example_size';
  $handler->display->display_options['fields']['views_isotope_example_size']['field'] = 'views_isotope_example_size';
  $handler->display->display_options['fields']['views_isotope_example_size']['label'] = '';
  $handler->display->display_options['fields']['views_isotope_example_size']['element_label_colon'] = FALSE;

  /* Display: Shape Filter */
  $handler = $view->new_display('block', 'Shape Filter', 'shape_filter');
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'isotope_filter';
  $handler->display->display_options['style_options']['filter_group'] = 'Shape';
  $handler->display->display_options['style_options']['data_fields'] = 'views_isotope_example_shape';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Shape */
  $handler->display->display_options['fields']['views_isotope_example_shape']['id'] = 'views_isotope_example_shape';
  $handler->display->display_options['fields']['views_isotope_example_shape']['table'] = 'field_data_views_isotope_example_shape';
  $handler->display->display_options['fields']['views_isotope_example_shape']['field'] = 'views_isotope_example_shape';
  $handler->display->display_options['fields']['views_isotope_example_shape']['label'] = '';
  $handler->display->display_options['fields']['views_isotope_example_shape']['element_label_colon'] = FALSE;

  $views[$view->name] = $view;
  return $views;
}

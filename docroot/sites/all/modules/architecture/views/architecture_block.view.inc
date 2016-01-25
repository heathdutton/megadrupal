<?php

/**
 * @file
 * Defines a view for displaying blocks.
 */

/*
 * View 'architecture_list_all_blocks'
 */
if (module_exists('block')) {
  $view = new view();
  $view->name = 'architecture_list_all_blocks';
  $view->description = 'Gives you the lay of the land for your blocks.';
  $view->tag = 'architecture';
  $view->base_table = 'block';
  $view->human_name = 'architecture: Blocks';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'architecture: List All Blocks';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'use architecture';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'delta' => 'delta',
    'status' => 'status',
    'region' => 'region',
    'theme' => 'theme',
    'module' => 'module',
    'body' => 'body',
    'php' => 'php',
    'format' => 'format',
  );
  $handler->display->display_options['style_options']['default'] = 'delta';
  $handler->display->display_options['style_options']['info'] = array(
    'delta' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'region' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'theme' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'module' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'body' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'php' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'format' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
  );
  /* Field: Block: Delta */
  $handler->display->display_options['fields']['delta']['id'] = 'delta';
  $handler->display->display_options['fields']['delta']['table'] = 'block';
  $handler->display->display_options['fields']['delta']['field'] = 'delta';
  /* Field: Block: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'block';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: Block: Region */
  $handler->display->display_options['fields']['region']['id'] = 'region';
  $handler->display->display_options['fields']['region']['table'] = 'block';
  $handler->display->display_options['fields']['region']['field'] = 'region';
  /* Field: Block: Theme */
  $handler->display->display_options['fields']['theme']['id'] = 'theme';
  $handler->display->display_options['fields']['theme']['table'] = 'block';
  $handler->display->display_options['fields']['theme']['field'] = 'theme';
  /* Field: Block: Module */
  $handler->display->display_options['fields']['module']['id'] = 'module';
  $handler->display->display_options['fields']['module']['table'] = 'block';
  $handler->display->display_options['fields']['module']['field'] = 'module';
  /* Field: Block: body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'block_custom';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  /* Field: Block: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'block_custom';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'Inline PHP';
  $handler->display->display_options['fields']['php']['empty'] = 'None.';
  /* Field: Block: format */
  $handler->display->display_options['fields']['format']['id'] = 'format';
  $handler->display->display_options['fields']['format']['table'] = 'block_custom';
  $handler->display->display_options['fields']['format']['field'] = 'format';
  /* Filter criterion: Block: Theme */
  $handler->display->display_options['filters']['theme']['id'] = 'theme';
  $handler->display->display_options['filters']['theme']['table'] = 'block';
  $handler->display->display_options['filters']['theme']['field'] = 'theme';
  $handler->display->display_options['filters']['theme']['exposed'] = TRUE;
  $handler->display->display_options['filters']['theme']['expose']['operator_id'] = 'theme_op';
  $handler->display->display_options['filters']['theme']['expose']['label'] = 'Theme';
  $handler->display->display_options['filters']['theme']['expose']['description'] = 'Select which theme\'s blocks to display';
  $handler->display->display_options['filters']['theme']['expose']['operator'] = 'theme_op';
  $handler->display->display_options['filters']['theme']['expose']['identifier'] = 'Theme';
  $handler->display->display_options['filters']['theme']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    4 => 0,
    3 => 0,
    6 => 0,
    7 => 0,
  );
  /* Filter criterion: Block: Region */
  $handler->display->display_options['filters']['region']['id'] = 'region';
  $handler->display->display_options['filters']['region']['table'] = 'block';
  $handler->display->display_options['filters']['region']['field'] = 'region';
  $handler->display->display_options['filters']['region']['exposed'] = TRUE;
  $handler->display->display_options['filters']['region']['expose']['operator_id'] = 'region_op';
  $handler->display->display_options['filters']['region']['expose']['label'] = 'Region';
  $handler->display->display_options['filters']['region']['expose']['description'] = 'Select which region\'s blocks to display';
  $handler->display->display_options['filters']['region']['expose']['operator'] = 'region_op';
  $handler->display->display_options['filters']['region']['expose']['identifier'] = 'Region';
  $handler->display->display_options['filters']['region']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    4 => 0,
    3 => 0,
    6 => 0,
    7 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'admin/reports/architecture/blocks';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Blocks';
  $handler->display->display_options['menu']['description'] = 'List of blocks';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';

  if(module_exists('views_data_export')) {

    /* Display: Data export */
    $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
    $handler->display->display_options['pager']['type'] = 'some';
    $handler->display->display_options['pager']['options']['items_per_page'] = '0';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
    $handler->display->display_options['style_options']['provide_file'] = 1;
    $handler->display->display_options['style_options']['parent_sort'] = 1;
    $handler->display->display_options['style_options']['quote'] = 1;
    $handler->display->display_options['style_options']['trim'] = 0;
    $handler->display->display_options['style_options']['replace_newlines'] = 0;
    $handler->display->display_options['style_options']['header'] = 1;

    /* Menu settings */
    $handler->display->display_options['path'] = 'admin/reports/architecture/blocks/export';
    $handler->display->display_options['menu']['type'] = 'tab';
    $handler->display->display_options['menu']['title'] = 'Blocks export';
    $handler->display->display_options['menu']['description'] = 'Export list of blocks';
    $handler->display->display_options['menu']['weight'] = '0';
    $handler->display->display_options['menu']['context'] = 0;
    $handler->display->display_options['tab_options']['weight'] = '0';

    $handler->display->display_options['style_options']['filename'] = variable_get('site_name') . '-' . date('YmdAhis') . '-block.csv';
  }
}

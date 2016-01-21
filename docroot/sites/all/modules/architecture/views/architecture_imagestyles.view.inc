<?php

/**
 * @file
 * Defines a view for displaying imagestyles
 */

/*
 * View 'architecture_list_all_imagestyles'
 */
$view = new view();
$view->name = 'architecture_list_all_imagestyles';
$view->description = 'Gives you the lay of the land of your imagestyles.';
$view->tag = 'architecture';
$view->base_table = 'imagestyles';
$view->human_name = 'architecture: Imagestyles';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'architecture: List all Imagestyles';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'use architecture';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* Field: Imagestyles: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'imagestyles';
$handler->display->display_options['fields']['title']['field'] = 'title';
/* Field: Imagestyles: Effects */
$handler->display->display_options['fields']['effects']['id'] = 'effects';
$handler->display->display_options['fields']['effects']['table'] = 'imagestyles';
$handler->display->display_options['fields']['effects']['field'] = 'effects';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['grouping'] = array(
  0 => array(
    'field' => 'preview',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  1 => array(
    'field' => 'title',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
);
$handler->display->display_options['style_options']['columns'] = array(
  'effect_weight' => 'effect_weight',
  'title' => 'title',
  'effects' => 'effects',
  'settings' => 'settings',
  'effect_desc' => 'effect_desc',
  'preview' => 'preview',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'effect_weight' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'title' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'effects' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'settings' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'effect_desc' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'preview' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Imagestyles: Weight */
$handler->display->display_options['fields']['effect_weight']['id'] = 'effect_weight';
$handler->display->display_options['fields']['effect_weight']['table'] = 'imagestyles';
$handler->display->display_options['fields']['effect_weight']['field'] = 'effect_weight';
/* Field: Imagestyles: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'imagestyles';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['exclude'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['text'] = '<h2>[title]</h2>';
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: Imagestyles: Effects */
$handler->display->display_options['fields']['effects']['id'] = 'effects';
$handler->display->display_options['fields']['effects']['table'] = 'imagestyles';
$handler->display->display_options['fields']['effects']['field'] = 'effects';
$handler->display->display_options['fields']['effects']['label'] = 'Effect';
/* Field: Imagestyles: Settings */
$handler->display->display_options['fields']['settings']['id'] = 'settings';
$handler->display->display_options['fields']['settings']['table'] = 'imagestyles';
$handler->display->display_options['fields']['settings']['field'] = 'settings';
/* Field: Imagestyles: Description */
$handler->display->display_options['fields']['effect_desc']['id'] = 'effect_desc';
$handler->display->display_options['fields']['effect_desc']['table'] = 'imagestyles';
$handler->display->display_options['fields']['effect_desc']['field'] = 'effect_desc';
/* Field: Imagestyles: Preview */
$handler->display->display_options['fields']['preview']['id'] = 'preview';
$handler->display->display_options['fields']['preview']['table'] = 'imagestyles';
$handler->display->display_options['fields']['preview']['field'] = 'preview';
$handler->display->display_options['fields']['preview']['label'] = '';
$handler->display->display_options['fields']['preview']['exclude'] = TRUE;
$handler->display->display_options['fields']['preview']['element_label_colon'] = FALSE;
$handler->display->display_options['path'] = 'admin/reports/architecture/imagestyles';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Image Styles';
$handler->display->display_options['menu']['description'] = 'Image style and effects info';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;

if(module_exists('views_data_export')) {
  // Add export link to Page view if relevant.
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['label'] = 'Export';
  $handler->display->display_options['header']['area']['content'] = '<a href="export" title="Export the result of this view">Export to CSV</a>';
  $handler->display->display_options['header']['area']['format'] = 'html';
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['style_options']['filename'] = variable_get('site_name') . '-' . date('YmdAHis') . '-imagestyles.csv';
  /* Menu settings */
  $handler->display->display_options['path'] = 'admin/reports/architecture/imagestyles/export';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Image styles export';
  $handler->display->display_options['menu']['description'] = 'Export list of image styles';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
}
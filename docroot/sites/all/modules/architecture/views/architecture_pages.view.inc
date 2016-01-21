<?php

/**
 * @file
 * Defines a view for displaying pages.
 */

/*
 * View 'architecture_list_all_pages'
*/
$view = views_new_view();
$view->name = 'architecture_list_all_pages';
$view->description = 'Gives you the lay of the land for your pages.';
$view->tag = 'architecture';
$view->base_table = 'page_manager_pages';
$view->human_name = 'architecture: Pages';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = 'architecture: List All Pages';
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
$handler->display->display_options['style_options']['default'] = '-1';
/* Field: Page manager: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['title']['field'] = 'title';
/* Field: Page manager: Description */
$handler->display->display_options['fields']['description']['id'] = 'description';
$handler->display->display_options['fields']['description']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['description']['field'] = 'description';
/* Field: Page manager: Enabled */
$handler->display->display_options['fields']['enabled']['id'] = 'enabled';
$handler->display->display_options['fields']['enabled']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['enabled']['field'] = 'enabled';
/* Field: Page manager: Storage */
$handler->display->display_options['fields']['storage']['id'] = 'storage';
$handler->display->display_options['fields']['storage']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['storage']['field'] = 'storage';
/* Field: Page manager: Type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['type']['field'] = 'type';
/* Field: Page manager: Path */
$handler->display->display_options['fields']['path']['id'] = 'path';
$handler->display->display_options['fields']['path']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['path']['field'] = 'path';
/* Field: Page manager: Handler */
$handler->display->display_options['fields']['handler']['id'] = 'handler';
$handler->display->display_options['fields']['handler']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['handler']['field'] = 'handler';
/* Field: Page manager: Handler Subtype */
$handler->display->display_options['fields']['handler_subtype']['id'] = 'handler_subtype';
$handler->display->display_options['fields']['handler_subtype']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['handler_subtype']['field'] = 'handler_subtype';
/* Field: Page manager: Handler Title */
$handler->display->display_options['fields']['handler_title']['id'] = 'handler_title';
$handler->display->display_options['fields']['handler_title']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['handler_title']['field'] = 'handler_title';
/* Field: Page manager: Handler Admin Title */
$handler->display->display_options['fields']['handler_admin_title']['id'] = 'handler_admin_title';
$handler->display->display_options['fields']['handler_admin_title']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['handler_admin_title']['field'] = 'handler_admin_title';
/* Field: Page manager: Handler Body */
$handler->display->display_options['fields']['handler_body']['id'] = 'handler_body';
$handler->display->display_options['fields']['handler_body']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['handler_body']['field'] = 'handler_body';
/* Field: Page manager: Handler Panel */
$handler->display->display_options['fields']['handler_panel']['id'] = 'handler_panel';
$handler->display->display_options['fields']['handler_panel']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['handler_panel']['field'] = 'handler_panel';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['grouping'] = array(
  0 => array(
    'field' => 'title',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  1 => array(
    'field' => 'enabled',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  2 => array(
    'field' => 'storage',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  3 => array(
    'field' => 'type',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  4 => array(
    'field' => 'path',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  5 => array(
    'field' => 'description',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
);
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'description' => 'description',
  'enabled' => 'enabled',
  'storage' => 'storage',
  'type' => 'type',
  'path' => 'path',
  'handler' => 'handler',
  'handler_subtype' => 'handler_subtype',
  'handler_title' => 'handler_title',
  'handler_admin_title' => 'handler_admin_title',
  'handler_body' => 'handler_body',
  'handler_panel' => 'handler_panel',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'description' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'enabled' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'storage' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'type' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'path' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'handler' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'handler_subtype' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 1,
  ),
  'handler_title' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 1,
  ),
  'handler_admin_title' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 1,
  ),
  'handler_body' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 1,
  ),
  'handler_panel' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 1,
  ),
);
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Page manager: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['exclude'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['text'] = '<h2>[title]</h2>';
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: Page manager: Description */
$handler->display->display_options['fields']['description']['id'] = 'description';
$handler->display->display_options['fields']['description']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['description']['field'] = 'description';
$handler->display->display_options['fields']['description']['label'] = '';
$handler->display->display_options['fields']['description']['exclude'] = TRUE;
$handler->display->display_options['fields']['description']['alter']['strip_tags'] = TRUE;
$handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
/* Field: Page manager: Enabled */
$handler->display->display_options['fields']['enabled']['id'] = 'enabled';
$handler->display->display_options['fields']['enabled']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['enabled']['field'] = 'enabled';
$handler->display->display_options['fields']['enabled']['exclude'] = TRUE;
/* Field: Page manager: Storage */
$handler->display->display_options['fields']['storage']['id'] = 'storage';
$handler->display->display_options['fields']['storage']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['storage']['field'] = 'storage';
$handler->display->display_options['fields']['storage']['exclude'] = TRUE;
/* Field: Page manager: Type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['type']['field'] = 'type';
$handler->display->display_options['fields']['type']['exclude'] = TRUE;
/* Field: Page manager: Path */
$handler->display->display_options['fields']['path']['id'] = 'path';
$handler->display->display_options['fields']['path']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['path']['field'] = 'path';
$handler->display->display_options['fields']['path']['exclude'] = TRUE;
/* Field: Page manager: Handler */
$handler->display->display_options['fields']['handler']['id'] = 'handler';
$handler->display->display_options['fields']['handler']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['handler']['field'] = 'handler';
/* Field: Page manager: Handler Subtype */
$handler->display->display_options['fields']['handler_subtype']['id'] = 'handler_subtype';
$handler->display->display_options['fields']['handler_subtype']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['handler_subtype']['field'] = 'handler_subtype';
/* Field: Page manager: Handler Title */
$handler->display->display_options['fields']['handler_title']['id'] = 'handler_title';
$handler->display->display_options['fields']['handler_title']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['handler_title']['field'] = 'handler_title';
/* Field: Page manager: Handler Admin Title */
$handler->display->display_options['fields']['handler_admin_title']['id'] = 'handler_admin_title';
$handler->display->display_options['fields']['handler_admin_title']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['handler_admin_title']['field'] = 'handler_admin_title';
/* Field: Page manager: Handler Body */
$handler->display->display_options['fields']['handler_body']['id'] = 'handler_body';
$handler->display->display_options['fields']['handler_body']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['handler_body']['field'] = 'handler_body';
/* Field: Page manager: Handler Panel */
$handler->display->display_options['fields']['handler_panel']['id'] = 'handler_panel';
$handler->display->display_options['fields']['handler_panel']['table'] = 'page_manager_pages';
$handler->display->display_options['fields']['handler_panel']['field'] = 'handler_panel';

/* Menu settings */
$handler->display->display_options['path'] = 'admin/reports/architecture/pages';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Pages';
$handler->display->display_options['menu']['description'] = 'List of page manager and panels pages';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['tab_options']['weight'] = '0';

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
  $handler->display->display_options['style_options']['filename'] = variable_get('site_name') . '-' . date('YmdAHis') . '-pages.csv';
  /* Menu settings */
  $handler->display->display_options['path'] = 'admin/reports/architecture/pages/export';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Pages export';
  $handler->display->display_options['menu']['description'] = 'Export list of page manager and panels pages';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
}
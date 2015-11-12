<?php
/**
 * @file
 * nm_announcements.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function nm_announcements_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'nm_announcements';
  $view->description = 'Provides a landing page, pages by year & month, block of recent posts and archive block.';
  $view->tag = 'nodemaker, announcements';
  $view->base_table = 'node';
  $view->human_name = 'Announcements';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Announcements';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'All Announcements';
  $handler->display->display_options['link_display'] = 'page';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'There currently are not any announcements.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Content: Headline Image */
  $handler->display->display_options['fields']['field_nm_headline_image']['id'] = 'field_nm_headline_image';
  $handler->display->display_options['fields']['field_nm_headline_image']['table'] = 'field_data_field_nm_headline_image';
  $handler->display->display_options['fields']['field_nm_headline_image']['field'] = 'field_nm_headline_image';
  $handler->display->display_options['fields']['field_nm_headline_image']['label'] = '';
  $handler->display->display_options['fields']['field_nm_headline_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_headline_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_nm_headline_image']['settings'] = array(
    'image_style' => 'nodemaker_thumbnail',
    'image_link' => 'content',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
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
    'nm_announcement' => 'nm_announcement',
  );
  /* Filter criterion: Content: Post date */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'node';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  
  /* Display: Page: Landing */
  $handler = $view->new_display('page', 'Page: Landing', 'page');
  $handler->display->display_options['display_description'] = 'The main Announcements page';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'nm_announcement' => 'nm_announcement',
  );
  $handler->display->display_options['path'] = 'announcements';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Announcements';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  
  /* Display: Page: Year */
  $handler = $view->new_display('page', 'Page: Year', 'page_2');
  $handler->display->display_options['display_description'] = 'All posts made in the year.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Created year */
  $handler->display->display_options['arguments']['created_year']['id'] = 'created_year';
  $handler->display->display_options['arguments']['created_year']['table'] = 'node';
  $handler->display->display_options['arguments']['created_year']['field'] = 'created_year';
  $handler->display->display_options['arguments']['created_year']['default_action'] = 'default';
  $handler->display->display_options['arguments']['created_year']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_year']['exception']['title'] = 'All Years';
  $handler->display->display_options['arguments']['created_year']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_year']['title'] = '%1 Announcements';
  $handler->display->display_options['arguments']['created_year']['breadcrumb_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_year']['breadcrumb'] = 'Announcements';
  $handler->display->display_options['arguments']['created_year']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['created_year']['default_argument_options']['index'] = '1';
  $handler->display->display_options['arguments']['created_year']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['created_year']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['created_year']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['created_year']['validate']['type'] = 'numeric';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'nm_announcement' => 'nm_announcement',
  );
  $handler->display->display_options['path'] = 'announcements/%';
  
  /* Display: Page: Year / Month */
  $handler = $view->new_display('page', 'Page: Year / Month', 'page_1');
  $handler->display->display_options['display_description'] = 'All posts made in the year and month.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Created year */
  $handler->display->display_options['arguments']['created_year']['id'] = 'created_year';
  $handler->display->display_options['arguments']['created_year']['table'] = 'node';
  $handler->display->display_options['arguments']['created_year']['field'] = 'created_year';
  $handler->display->display_options['arguments']['created_year']['default_action'] = 'default';
  $handler->display->display_options['arguments']['created_year']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_year']['exception']['title'] = 'All Years';
  $handler->display->display_options['arguments']['created_year']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_year']['title'] = '%1 Announcements';
  $handler->display->display_options['arguments']['created_year']['breadcrumb_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_year']['breadcrumb'] = 'Announcements';
  $handler->display->display_options['arguments']['created_year']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['created_year']['default_argument_options']['index'] = '1';
  $handler->display->display_options['arguments']['created_year']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['created_year']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['created_year']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['created_year']['validate']['type'] = 'numeric';
  /* Contextual filter: Content: Created month */
  $handler->display->display_options['arguments']['created_month']['id'] = 'created_month';
  $handler->display->display_options['arguments']['created_month']['table'] = 'node';
  $handler->display->display_options['arguments']['created_month']['field'] = 'created_month';
  $handler->display->display_options['arguments']['created_month']['default_action'] = 'default';
  $handler->display->display_options['arguments']['created_month']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_month']['exception']['title'] = '';
  $handler->display->display_options['arguments']['created_month']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_month']['title'] = '%2 %1 Announcements';
  $handler->display->display_options['arguments']['created_month']['breadcrumb_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_month']['breadcrumb'] = '%1';
  $handler->display->display_options['arguments']['created_month']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['created_month']['default_argument_options']['index'] = '2';
  $handler->display->display_options['arguments']['created_month']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['created_month']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['created_month']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'nm_announcement' => 'nm_announcement',
  );
  $handler->display->display_options['path'] = 'announcements/%/%';
  
  /* Display: Block: Archive */
  $handler = $view->new_display('block', 'Block: Archive', 'block_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Archive';
  $handler->display->display_options['display_description'] = 'Links to Year/Month pages';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Created year */
  $handler->display->display_options['arguments']['created_year']['id'] = 'created_year';
  $handler->display->display_options['arguments']['created_year']['table'] = 'node';
  $handler->display->display_options['arguments']['created_year']['field'] = 'created_year';
  $handler->display->display_options['arguments']['created_year']['default_action'] = 'default';
  $handler->display->display_options['arguments']['created_year']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_year']['exception']['title'] = 'All Years';
  $handler->display->display_options['arguments']['created_year']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_year']['title'] = '%1 Archive';
  $handler->display->display_options['arguments']['created_year']['breadcrumb_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_year']['breadcrumb'] = '%1';
  $handler->display->display_options['arguments']['created_year']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['created_year']['default_argument_options']['index'] = '1';
  $handler->display->display_options['arguments']['created_year']['summary']['sort_order'] = 'desc';
  $handler->display->display_options['arguments']['created_year']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['created_year']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['created_year']['summary_options']['base_path'] = 'announcements';
  $handler->display->display_options['arguments']['created_year']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['created_year']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['created_year']['validate']['type'] = 'numeric';
  $handler->display->display_options['arguments']['created_year']['validate']['fail'] = 'summary';
  /* Contextual filter: Content: Created month */
  $handler->display->display_options['arguments']['created_month']['id'] = 'created_month';
  $handler->display->display_options['arguments']['created_month']['table'] = 'node';
  $handler->display->display_options['arguments']['created_month']['field'] = 'created_month';
  $handler->display->display_options['arguments']['created_month']['default_action'] = 'summary';
  $handler->display->display_options['arguments']['created_month']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_month']['exception']['title'] = '';
  $handler->display->display_options['arguments']['created_month']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_month']['title'] = '%2 %1 Announcements';
  $handler->display->display_options['arguments']['created_month']['breadcrumb_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_month']['breadcrumb'] = '%2';
  $handler->display->display_options['arguments']['created_month']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['created_month']['default_argument_options']['index'] = '2';
  $handler->display->display_options['arguments']['created_month']['summary']['sort_order'] = 'desc';
  $handler->display->display_options['arguments']['created_month']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['created_month']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['created_month']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['created_month']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['created_month']['validate']['type'] = 'numeric';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'nm_announcement' => 'nm_announcement',
  );
  
  /* Display: Block: Archive - Static */
  $handler = $view->new_display('block', 'Block: Archive - Static', 'block_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Archive';
  $handler->display->display_options['display_description'] = 'Links to Year/Month pages';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Created year */
  $handler->display->display_options['arguments']['created_year']['id'] = 'created_year';
  $handler->display->display_options['arguments']['created_year']['table'] = 'node';
  $handler->display->display_options['arguments']['created_year']['field'] = 'created_year';
  $handler->display->display_options['arguments']['created_year']['default_action'] = 'default';
  $handler->display->display_options['arguments']['created_year']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_year']['exception']['title'] = 'All Years';
  $handler->display->display_options['arguments']['created_year']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_year']['title'] = '%1 Archive';
  $handler->display->display_options['arguments']['created_year']['breadcrumb_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_year']['breadcrumb'] = '%1';
  $handler->display->display_options['arguments']['created_year']['default_argument_type'] = 'node_created';
  $handler->display->display_options['arguments']['created_year']['summary']['sort_order'] = 'desc';
  $handler->display->display_options['arguments']['created_year']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['created_year']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['created_year']['summary_options']['base_path'] = 'announcements';
  $handler->display->display_options['arguments']['created_year']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['created_year']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['created_year']['validate']['type'] = 'numeric';
  $handler->display->display_options['arguments']['created_year']['validate']['fail'] = 'summary';
  /* Contextual filter: Content: Created month */
  $handler->display->display_options['arguments']['created_month']['id'] = 'created_month';
  $handler->display->display_options['arguments']['created_month']['table'] = 'node';
  $handler->display->display_options['arguments']['created_month']['field'] = 'created_month';
  $handler->display->display_options['arguments']['created_month']['default_action'] = 'summary';
  $handler->display->display_options['arguments']['created_month']['exception']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_month']['exception']['title'] = '';
  $handler->display->display_options['arguments']['created_month']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_month']['title'] = '%2 %1 Announcements';
  $handler->display->display_options['arguments']['created_month']['breadcrumb_enable'] = TRUE;
  $handler->display->display_options['arguments']['created_month']['breadcrumb'] = '%2';
  $handler->display->display_options['arguments']['created_month']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['created_month']['default_argument_options']['index'] = '2';
  $handler->display->display_options['arguments']['created_month']['summary']['sort_order'] = 'desc';
  $handler->display->display_options['arguments']['created_month']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['created_month']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['created_month']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['created_month']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['created_month']['validate']['type'] = 'numeric';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'nm_announcement' => 'nm_announcement',
  );
  
  /* Display: Block: Recent posts */
  $handler = $view->new_display('block', 'Block: Recent posts', 'recent');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['display_description'] = 'The most recent posts with link to landing page.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '3';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['links'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Headline Image */
  $handler->display->display_options['fields']['field_nm_headline_image']['id'] = 'field_nm_headline_image';
  $handler->display->display_options['fields']['field_nm_headline_image']['table'] = 'field_data_field_nm_headline_image';
  $handler->display->display_options['fields']['field_nm_headline_image']['field'] = 'field_nm_headline_image';
  $handler->display->display_options['fields']['field_nm_headline_image']['label'] = '';
  $handler->display->display_options['fields']['field_nm_headline_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_headline_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_nm_headline_image']['settings'] = array(
    'image_style' => 'nodemaker_thumbnail',
    'image_link' => 'content',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_type'] = 'h3';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['field_nm_body']['id'] = 'field_nm_body';
  $handler->display->display_options['fields']['field_nm_body']['table'] = 'field_data_field_nm_body';
  $handler->display->display_options['fields']['field_nm_body']['field'] = 'field_nm_body';
  $handler->display->display_options['fields']['field_nm_body']['label'] = '';
  $handler->display->display_options['fields']['field_nm_body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_body']['type'] = 'text_summary_or_trimmed';
  $handler->display->display_options['fields']['field_nm_body']['settings'] = array(
    'trim_length' => '600',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'nm_announcement' => 'nm_announcement',
  );
  
  /* Display: Feed */
  $handler = $view->new_display('feed', 'Feed', 'feed');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'nm_announcement' => 'nm_announcement',
  );
  $handler->display->display_options['path'] = 'announcements.xml';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
  );

  $export['nm_announcements'] = $view;

  return $export;
}

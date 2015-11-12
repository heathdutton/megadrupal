<?php

/**
 * @file
 * Views integration for the datastore system module.
 */

/**
 * Implements hook_views_default_views().
 */
function datastore_system_views_default_views() {
  $view = new view;
  $view->name = 'datastore_system_users';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'datastore';
  $view->human_name = 'datastore_system_users';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Site Users';
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '0';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
$handler->display->display_options['style_plugin'] = 'datastore_visualization';
$handler->display->display_options['style_options']['field_value'] = 'datastore_field_system_value';
$handler->display->display_options['style_options']['field_label'] = 'datastore_field_system_timestamp';
$handler->display->display_options['style_options']['width'] = '600';
$handler->display->display_options['style_options']['height'] = '250';
/* Field: DataStore: datastore_field_system (datastore_field_system) */
$handler->display->display_options['fields']['datastore_field_system_value']['id'] = 'datastore_field_system_value';
$handler->display->display_options['fields']['datastore_field_system_value']['table'] = 'field_data_datastore_field_system';
$handler->display->display_options['fields']['datastore_field_system_value']['field'] = 'datastore_field_system_value';
$handler->display->display_options['fields']['datastore_field_system_value']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['datastore_field_system_value']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['datastore_field_system_value']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['datastore_field_system_value']['alter']['external'] = 0;
$handler->display->display_options['fields']['datastore_field_system_value']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['datastore_field_system_value']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['datastore_field_system_value']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['datastore_field_system_value']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['datastore_field_system_value']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['datastore_field_system_value']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['datastore_field_system_value']['alter']['trim'] = 0;
$handler->display->display_options['fields']['datastore_field_system_value']['alter']['html'] = 0;
$handler->display->display_options['fields']['datastore_field_system_value']['element_label_colon'] = 1;
$handler->display->display_options['fields']['datastore_field_system_value']['element_default_classes'] = 1;
$handler->display->display_options['fields']['datastore_field_system_value']['hide_empty'] = 0;
$handler->display->display_options['fields']['datastore_field_system_value']['empty_zero'] = 0;
$handler->display->display_options['fields']['datastore_field_system_value']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['datastore_field_system_value']['format_plural'] = 0;
/* Field: DataStore: datastore_field_system (datastore_field_system:timestamp) */
$handler->display->display_options['fields']['datastore_field_system_timestamp']['id'] = 'datastore_field_system_timestamp';
$handler->display->display_options['fields']['datastore_field_system_timestamp']['table'] = 'field_data_datastore_field_system';
$handler->display->display_options['fields']['datastore_field_system_timestamp']['field'] = 'datastore_field_system_timestamp';
$handler->display->display_options['fields']['datastore_field_system_timestamp']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['datastore_field_system_timestamp']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['datastore_field_system_timestamp']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['datastore_field_system_timestamp']['alter']['external'] = 0;
$handler->display->display_options['fields']['datastore_field_system_timestamp']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['datastore_field_system_timestamp']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['datastore_field_system_timestamp']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['datastore_field_system_timestamp']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['datastore_field_system_timestamp']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['datastore_field_system_timestamp']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['datastore_field_system_timestamp']['alter']['trim'] = 0;
$handler->display->display_options['fields']['datastore_field_system_timestamp']['alter']['html'] = 0;
$handler->display->display_options['fields']['datastore_field_system_timestamp']['element_label_colon'] = 1;
$handler->display->display_options['fields']['datastore_field_system_timestamp']['element_default_classes'] = 1;
$handler->display->display_options['fields']['datastore_field_system_timestamp']['hide_empty'] = 0;
$handler->display->display_options['fields']['datastore_field_system_timestamp']['empty_zero'] = 0;
$handler->display->display_options['fields']['datastore_field_system_timestamp']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['datastore_field_system_timestamp']['date_format'] = 'long';
/* Sort criterion: DataStore: datastore_field_system (datastore_field_system:timestamp) */
$handler->display->display_options['sorts']['datastore_field_system_timestamp']['id'] = 'datastore_field_system_timestamp';
$handler->display->display_options['sorts']['datastore_field_system_timestamp']['table'] = 'field_data_datastore_field_system';
$handler->display->display_options['sorts']['datastore_field_system_timestamp']['field'] = 'datastore_field_system_timestamp';
/* Filter criterion: DataStore: Domain */
$handler->display->display_options['filters']['domain']['id'] = 'domain';
$handler->display->display_options['filters']['domain']['table'] = 'datastore';
$handler->display->display_options['filters']['domain']['field'] = 'domain';
$handler->display->display_options['filters']['domain']['value'] = array(
  'system' => 'system',
);
/* Filter criterion: DataStore: Category */
$handler->display->display_options['filters']['category']['id'] = 'category';
$handler->display->display_options['filters']['category']['table'] = 'datastore';
$handler->display->display_options['filters']['category']['field'] = 'category';
$handler->display->display_options['filters']['category']['value'] = 'user_count';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'datastore-system-users';


  $views = array();
  $views[$view->name] = $view;
  return $views;
}

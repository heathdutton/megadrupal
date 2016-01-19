<?php

/**
 * Implements hook_views_default_views().
 */
function cm_airing_views_default_views() {
  $view = new view;
  $view->name = 'airings';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'cm_airing';
  $view->human_name = 'Airings';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Airings';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = FALSE;
  $handler->display->display_options['header']['area']['content'] = '<a href="<?php print url(\'airing/add\'); ?>>Add Airing</a>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = 0;
  /* Field: Airing: Airing ID */
  $handler->display->display_options['fields']['airing_id']['id'] = 'airing_id';
  $handler->display->display_options['fields']['airing_id']['table'] = 'cm_airing';
  $handler->display->display_options['fields']['airing_id']['field'] = 'airing_id';
  $handler->display->display_options['fields']['airing_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['airing_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['airing_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['airing_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['airing_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['airing_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['airing_id']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['airing_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['airing_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['airing_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['airing_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['airing_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['airing_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['airing_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['airing_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['airing_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['airing_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['airing_id']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['airing_id']['format_plural'] = 0;
  /* Field: Airing: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'cm_airing';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'airing/[airing_id]';
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  /* Field: Airing: Date */
  $handler->display->display_options['fields']['field_airing_date']['id'] = 'field_airing_date';
  $handler->display->display_options['fields']['field_airing_date']['table'] = 'field_data_field_airing_date';
  $handler->display->display_options['fields']['field_airing_date']['field'] = 'field_airing_date';
  $handler->display->display_options['fields']['field_airing_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_airing_date']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_airing_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_airing_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_airing_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_airing_date']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_airing_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_airing_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_airing_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_airing_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_airing_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_airing_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_airing_date']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_airing_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_airing_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_airing_date']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_airing_date']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_airing_date']['settings'] = array(
	'format_type' => 'short',
	'fromto' => 'both',
	'multiple_number' => '',
	'multiple_from' => '',
	'multiple_to' => '',
	'show_repeat_rule' => 'hide',
  );
  $handler->display->display_options['fields']['field_airing_date']['group_rows'] = 1;
  $handler->display->display_options['fields']['field_airing_date']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_airing_date']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['field_airing_date']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['field_airing_date']['field_api_classes'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'airing/[airing_id]/edit';
  $handler->display->display_options['fields']['nothing']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nothing']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nothing']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nothing']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nothing']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nothing']['hide_alter_empty'] = 0;
  
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'airing';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Airings';
  $handler->display->display_options['menu']['weight'] = '0';

  $views['airings'] = $view;

  return $views;
}
<?php
/**
 * @file
 * Default views for the Calendar module.
 */

/**
 * Set up so it can be used as an API to create default calendars for
 * specific date fields.
 *
 * Use variable_set() to establish criteria for default calendars.
 * Set the variable in custom modules or in settings.
 *
 * Example: Add a new default calendar to custom
 * calendars that are already configured:
 *
 * $options = variable_get('agres_views_default_view_options', array());
 * $option = array(
 *   'name' => 'example_event',
 *   'description' => 'An example event calendar for the date field.',
 *   'path' => 'example_event',
 *   'types' => array('example_content_type'),
 *   'date_fields' => array('field_example_date'),
 *   );
 * $options[] = $option;
 * variable_set('agres_views_default_view_options', $options);
 *
 */
function agres_views_views_default_views() {
  $views = array();

//  $view = new view;
//  $view->name = 'agres_view';
//  $view->description = 'A bookingcalendar';
//  $view->tag = 'agres_view';
//  $view->base_table = 'node';
//  $view->human_name = 'Booking Calendar';
//  $view->core = 7;
//  $view->api_version = '3.0';
//  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
//
//  /* Display: Master */
//  $handler = $view->new_display('default', 'Master', 'default');
//  $handler->display->display_options['title'] = 'Booking Calendar';
//  $handler->display->display_options['link_display'] = 'page_1';
//  $handler->display->display_options['access']['type'] = 'perm';
//  $handler->display->display_options['access']['perm'] = 'view agres_views bookingcalendar';
//  $handler->display->display_options['cache']['type'] = 'none';
//  $handler->display->display_options['query']['type'] = 'views_query';
//  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
//  $handler->display->display_options['exposed_form']['type'] = 'basic';
//  $handler->display->display_options['pager']['type'] = 'date_views_pager';
//  $handler->display->display_options['pager']['options']['date_id'] = 'month';
//  $handler->display->display_options['style_plugin'] = 'agres_views_style';
//  $handler->display->display_options['style_options']['name_size'] = '3';
//  $handler->display->display_options['style_options']['with_weekno'] = '0';
//  $handler->display->display_options['style_options']['multiday_theme'] = '1';
//  $handler->display->display_options['style_options']['theme_style'] = '1';
//  $handler->display->display_options['style_options']['max_items'] = '0';
//  $handler->display->display_options['row_plugin'] = 'agres_views_node';
//  /* Field: Content: Title */
//  $handler->display->display_options['fields']['title']['id'] = 'title';
//  $handler->display->display_options['fields']['title']['table'] = 'node';
//  $handler->display->display_options['fields']['title']['field'] = 'title';
//  $handler->display->display_options['fields']['title']['label'] = '';
//  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
//  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
//  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
//  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
//  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
//  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
//  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
//  /* Field: Content: Booking Date */
//  $handler->display->display_options['fields']['field_agres_rdate']['id'] = 'field_agres_rdate';
//  $handler->display->display_options['fields']['field_agres_rdate']['table'] = 'field_data_field_agres_rdate';
//  $handler->display->display_options['fields']['field_agres_rdate']['field'] = 'field_agres_rdate';
//  $handler->display->display_options['fields']['field_agres_rdate']['exclude'] = TRUE;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['alter_text'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['make_link'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['absolute'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['external'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['replace_spaces'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['trim_whitespace'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['nl2br'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['word_boundary'] = 1;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['ellipsis'] = 1;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['strip_tags'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['trim'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['html'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['element_label_colon'] = 1;
//  $handler->display->display_options['fields']['field_agres_rdate']['element_default_classes'] = 1;
//  $handler->display->display_options['fields']['field_agres_rdate']['hide_empty'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['empty_zero'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['hide_alter_empty'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['settings'] = array(
//    'format_type' => 'long',
//    'fromto' => 'both',
//    'multiple_number' => '',
//    'multiple_from' => '',
//    'multiple_to' => '',
//    'show_repeat_rule' => 'show',
//  );
//  $handler->display->display_options['fields']['field_agres_rdate']['field_api_classes'] = 0;
//  /* Field: Content: Nid */
//  $handler->display->display_options['fields']['nid']['id'] = 'nid';
//  $handler->display->display_options['fields']['nid']['table'] = 'node';
//  $handler->display->display_options['fields']['nid']['field'] = 'nid';
//  /* Sort criterion: Content: Updated date */
//  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
//  $handler->display->display_options['sorts']['changed']['table'] = 'node';
//  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
//  /* Filter criterion: Content: Published */
//  $handler->display->display_options['filters']['status']['id'] = 'status';
//  $handler->display->display_options['filters']['status']['table'] = 'node';
//  $handler->display->display_options['filters']['status']['field'] = 'status';
//  $handler->display->display_options['filters']['status']['value'] = 1;
//  $handler->display->display_options['filters']['status']['group'] = 0;
//  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
//
//  /* Display: Month */
//  $handler = $view->new_display('page', 'Month', 'page_1');
//  $handler->display->display_options['defaults']['pager'] = FALSE;
//  $handler->display->display_options['pager']['type'] = 'date_views_pager';
//  $handler->display->display_options['pager']['options']['date_id'] = 'month';
//  $handler->display->display_options['pager']['options']['link_format'] = 'clean';
//  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
//  $handler->display->display_options['style_plugin'] = 'agres_views_style';
//  $handler->display->display_options['style_options']['name_size'] = '3';
//  $handler->display->display_options['style_options']['mini'] = '0';
//  $handler->display->display_options['style_options']['with_weekno'] = '0';
//  $handler->display->display_options['style_options']['multiday_theme'] = '0';
//  $handler->display->display_options['style_options']['theme_style'] = '1';
//  $handler->display->display_options['style_options']['max_items'] = '0';
//  $handler->display->display_options['defaults']['style_options'] = FALSE;
//  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
//  $handler->display->display_options['row_plugin'] = 'agres_views_node';
//  $handler->display->display_options['row_options']['colors']['legend'] = 'type';
//  $handler->display->display_options['defaults']['row_options'] = FALSE;
//  $handler->display->display_options['defaults']['fields'] = FALSE;
//  /* Field: Content: Title */
//  $handler->display->display_options['fields']['title']['id'] = 'title';
//  $handler->display->display_options['fields']['title']['table'] = 'node';
//  $handler->display->display_options['fields']['title']['field'] = 'title';
//  $handler->display->display_options['fields']['title']['label'] = '';
//  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
//  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
//  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
//  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
//  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
//  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
//  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
//  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
//  /* Field: Content: Booking Date */
//  $handler->display->display_options['fields']['field_agres_rdate']['id'] = 'field_agres_rdate';
//  $handler->display->display_options['fields']['field_agres_rdate']['table'] = 'field_data_field_agres_rdate';
//  $handler->display->display_options['fields']['field_agres_rdate']['field'] = 'field_agres_rdate';
//  $handler->display->display_options['fields']['field_agres_rdate']['exclude'] = TRUE;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['alter_text'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['make_link'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['absolute'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['external'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['replace_spaces'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['trim_whitespace'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['nl2br'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['word_boundary'] = 1;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['ellipsis'] = 1;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['strip_tags'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['trim'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['html'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['element_label_colon'] = 1;
//  $handler->display->display_options['fields']['field_agres_rdate']['element_default_classes'] = 1;
//  $handler->display->display_options['fields']['field_agres_rdate']['hide_empty'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['empty_zero'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['hide_alter_empty'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['settings'] = array(
//    'format_type' => 'long',
//    'fromto' => 'both',
//    'multiple_number' => '',
//    'multiple_from' => '',
//    'multiple_to' => '',
//    'show_repeat_rule' => 'show',
//  );
//  $handler->display->display_options['fields']['field_agres_rdate']['field_api_classes'] = 0;
//  /* Field: Content: Nid */
//  $handler->display->display_options['fields']['nid']['id'] = 'nid';
//  $handler->display->display_options['fields']['nid']['table'] = 'node';
//  $handler->display->display_options['fields']['nid']['field'] = 'nid';
//  $handler->display->display_options['fields']['nid']['label'] = '';
//  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 1;
//  $handler->display->display_options['fields']['nid']['alter']['text'] = 'N:[nid]';
//  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
//  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
//  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
//  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
//  $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
//  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
//  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
//  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
//  $handler->display->display_options['fields']['nid']['link_to_node'] = 1;
//  /* Field: Content: Order_id */
//  $handler->display->display_options['fields']['field_agres_orderid']['id'] = 'field_agres_orderid';
//  $handler->display->display_options['fields']['field_agres_orderid']['table'] = 'field_data_field_agres_orderid';
//  $handler->display->display_options['fields']['field_agres_orderid']['field'] = 'field_agres_orderid';
//  $handler->display->display_options['fields']['field_agres_orderid']['label'] = '';
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['alter_text'] = 1;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['text'] = 'O:[field_agres_orderid]';
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['make_link'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['absolute'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['external'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['replace_spaces'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['trim_whitespace'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['nl2br'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['word_boundary'] = 1;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['ellipsis'] = 1;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['strip_tags'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['trim'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['html'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['element_label_colon'] = FALSE;
//  $handler->display->display_options['fields']['field_agres_orderid']['element_default_classes'] = 1;
//  $handler->display->display_options['fields']['field_agres_orderid']['hide_empty'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['empty_zero'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['hide_alter_empty'] = 1;
//  $handler->display->display_options['fields']['field_agres_orderid']['type'] = 'number_unformatted';
//  $handler->display->display_options['fields']['field_agres_orderid']['settings'] = array(
//    'thousand_separator' => ' ',
//    'prefix_suffix' => 1,
//  );
//  $handler->display->display_options['fields']['field_agres_orderid']['field_api_classes'] = 0;
//  $handler->display->display_options['defaults']['arguments'] = FALSE;
//  /* Contextual filter: Date: Date (node) */
//  $handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
//  $handler->display->display_options['arguments']['date_argument']['table'] = 'node';
//  $handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
//  $handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
//  $handler->display->display_options['arguments']['date_argument']['default_argument_skip_url'] = 0;
//  $handler->display->display_options['arguments']['date_argument']['add_delta'] = 'yes';
//  $handler->display->display_options['arguments']['date_argument']['granularity'] = 'month';
//  $handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
//    'field_data_field_agres_rdate.field_agres_rdate_value' => 'field_data_field_agres_rdate.field_agres_rdate_value',
//    'field_data_field_agres_rdate.field_agres_rdate_value2' => 'field_data_field_agres_rdate.field_agres_rdate_value2',
//  );
//  $handler->display->display_options['arguments']['date_argument']['date_method'] = 'AND';
//  $handler->display->display_options['path'] = 'agres_view/month';
//  $handler->display->display_options['menu']['type'] = 'normal';
//  $handler->display->display_options['menu']['title'] = 'Booking Calendar';
//  $handler->display->display_options['menu']['description'] = 'A booking Calendar';
//  $handler->display->display_options['menu']['weight'] = '0';
//  $handler->display->display_options['menu']['name'] = 'main-menu';
//  $handler->display->display_options['tab_options']['type'] = 'normal';
//  $handler->display->display_options['tab_options']['title'] = 'Month';
//  $handler->display->display_options['tab_options']['weight'] = '0';
//
//  /* Display: Week */
//  $handler = $view->new_display('page', 'Week', 'page_2');
//  $handler->display->display_options['defaults']['pager'] = FALSE;
//  $handler->display->display_options['pager']['type'] = 'date_views_pager';
//  $handler->display->display_options['pager']['options']['date_id'] = 'week';
//  $handler->display->display_options['pager']['options']['link_format'] = 'clean';
//  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
//  $handler->display->display_options['style_plugin'] = 'agres_views_style';
//  $handler->display->display_options['style_options']['name_size'] = '3';
//  $handler->display->display_options['style_options']['mini'] = '0';
//  $handler->display->display_options['style_options']['with_weekno'] = '0';
//  $handler->display->display_options['style_options']['multiday_theme'] = '1';
//  $handler->display->display_options['style_options']['theme_style'] = '0';
//  $handler->display->display_options['style_options']['max_items'] = '0';
//  $handler->display->display_options['style_options']['agres_views_type'] = 'week';
//  $handler->display->display_options['defaults']['style_options'] = FALSE;
//  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
//  $handler->display->display_options['row_plugin'] = 'agres_views_node';
//  $handler->display->display_options['defaults']['row_options'] = FALSE;
//  $handler->display->display_options['defaults']['fields'] = FALSE;
//  /* Field: Content: Title */
//  $handler->display->display_options['fields']['title']['id'] = 'title';
//  $handler->display->display_options['fields']['title']['table'] = 'node';
//  $handler->display->display_options['fields']['title']['field'] = 'title';
//  $handler->display->display_options['fields']['title']['label'] = '';
//  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
//  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
//  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
//  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
//  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
//  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
//  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
//  /* Field: Content: Booking Date */
//  $handler->display->display_options['fields']['field_agres_rdate']['id'] = 'field_agres_rdate';
//  $handler->display->display_options['fields']['field_agres_rdate']['table'] = 'field_data_field_agres_rdate';
//  $handler->display->display_options['fields']['field_agres_rdate']['field'] = 'field_agres_rdate';
//  $handler->display->display_options['fields']['field_agres_rdate']['exclude'] = TRUE;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['alter_text'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['make_link'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['absolute'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['external'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['replace_spaces'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['trim_whitespace'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['nl2br'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['word_boundary'] = 1;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['ellipsis'] = 1;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['strip_tags'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['trim'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['html'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['element_label_colon'] = 1;
//  $handler->display->display_options['fields']['field_agres_rdate']['element_default_classes'] = 1;
//  $handler->display->display_options['fields']['field_agres_rdate']['hide_empty'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['empty_zero'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['hide_alter_empty'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['settings'] = array(
//    'format_type' => 'long',
//    'fromto' => 'both',
//    'multiple_number' => '',
//    'multiple_from' => '',
//    'multiple_to' => '',
//    'show_repeat_rule' => 'show',
//  );
//  $handler->display->display_options['fields']['field_agres_rdate']['field_api_classes'] = 0;
//  /* Field: Content: Nid */
//  $handler->display->display_options['fields']['nid']['id'] = 'nid';
//  $handler->display->display_options['fields']['nid']['table'] = 'node';
//  $handler->display->display_options['fields']['nid']['field'] = 'nid';
//  $handler->display->display_options['fields']['nid']['label'] = '';
//  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 1;
//  $handler->display->display_options['fields']['nid']['alter']['text'] = 'N:[nid]';
//  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
//  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
//  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
//  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
//  $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
//  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
//  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
//  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
//  $handler->display->display_options['fields']['nid']['link_to_node'] = 1;
//  /* Field: Content: Order_id */
//  $handler->display->display_options['fields']['field_agres_orderid']['id'] = 'field_agres_orderid';
//  $handler->display->display_options['fields']['field_agres_orderid']['table'] = 'field_data_field_agres_orderid';
//  $handler->display->display_options['fields']['field_agres_orderid']['field'] = 'field_agres_orderid';
//  $handler->display->display_options['fields']['field_agres_orderid']['label'] = '';
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['alter_text'] = 1;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['text'] = 'O:[field_agres_orderid] ';
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['make_link'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['absolute'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['external'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['replace_spaces'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['trim_whitespace'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['nl2br'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['word_boundary'] = 1;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['ellipsis'] = 1;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['strip_tags'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['trim'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['html'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['element_label_colon'] = FALSE;
//  $handler->display->display_options['fields']['field_agres_orderid']['element_default_classes'] = 1;
//  $handler->display->display_options['fields']['field_agres_orderid']['hide_empty'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['empty_zero'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['hide_alter_empty'] = 1;
//  $handler->display->display_options['fields']['field_agres_orderid']['settings'] = array(
//    'thousand_separator' => ' ',
//    'prefix_suffix' => 0,
//  );
//  $handler->display->display_options['fields']['field_agres_orderid']['field_api_classes'] = 0;
//  $handler->display->display_options['defaults']['arguments'] = FALSE;
//  /* Contextual filter: Date: Date (node) */
//  $handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
//  $handler->display->display_options['arguments']['date_argument']['table'] = 'node';
//  $handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
//  $handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
//  $handler->display->display_options['arguments']['date_argument']['default_argument_skip_url'] = 0;
//  $handler->display->display_options['arguments']['date_argument']['granularity'] = 'week';
//  $handler->display->display_options['arguments']['date_argument']['add_delta'] = 'yes';
//  $handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
//    'field_data_field_agres_rdate.field_agres_rdate_value' => 'field_data_field_agres_rdate.field_agres_rdate_value',
//    'field_data_field_agres_rdate.field_agres_rdate_value2' => 'field_data_field_agres_rdate.field_agres_rdate_value2',
//  );
//  $handler->display->display_options['arguments']['date_argument']['date_method'] = 'AND';
//  $handler->display->display_options['path'] = 'agres_view/week';
//  $handler->display->display_options['menu']['title'] = 'Week';
//  $handler->display->display_options['menu']['weight'] = '2';
//
//  /* Display: Day */
//  $handler = $view->new_display('page', 'Day', 'page_3');
//  $handler->display->display_options['defaults']['pager'] = FALSE;
//  $handler->display->display_options['pager']['type'] = 'date_views_pager';
//  $handler->display->display_options['pager']['options']['date_id'] = 'day';
//  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
//  $handler->display->display_options['style_plugin'] = 'agres_views_style';
//  $handler->display->display_options['style_options']['name_size'] = '3';
//  $handler->display->display_options['style_options']['mini'] = '0';
//  $handler->display->display_options['style_options']['with_weekno'] = '0';
//  $handler->display->display_options['style_options']['multiday_theme'] = '0';
//  $handler->display->display_options['style_options']['theme_style'] = '0';
//  $handler->display->display_options['style_options']['max_items'] = '0';
//  $handler->display->display_options['style_options']['agres_views_type'] = 'day';
//  $handler->display->display_options['defaults']['style_options'] = FALSE;
//  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
//  $handler->display->display_options['row_plugin'] = 'agres_views_node';
//  $handler->display->display_options['defaults']['row_options'] = FALSE;
//  $handler->display->display_options['defaults']['fields'] = FALSE;
//  /* Field: Content: Title */
//  $handler->display->display_options['fields']['title']['id'] = 'title';
//  $handler->display->display_options['fields']['title']['table'] = 'node';
//  $handler->display->display_options['fields']['title']['field'] = 'title';
//  $handler->display->display_options['fields']['title']['label'] = '';
//  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
//  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
//  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
//  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
//  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
//  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
//  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
//  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
//  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
//  /* Field: Content: Booking Date */
//  $handler->display->display_options['fields']['field_agres_rdate']['id'] = 'field_agres_rdate';
//  $handler->display->display_options['fields']['field_agres_rdate']['table'] = 'field_data_field_agres_rdate';
//  $handler->display->display_options['fields']['field_agres_rdate']['field'] = 'field_agres_rdate';
//  $handler->display->display_options['fields']['field_agres_rdate']['exclude'] = TRUE;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['alter_text'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['make_link'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['absolute'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['external'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['replace_spaces'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['trim_whitespace'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['nl2br'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['word_boundary'] = 1;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['ellipsis'] = 1;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['strip_tags'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['trim'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['alter']['html'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['element_label_colon'] = 1;
//  $handler->display->display_options['fields']['field_agres_rdate']['element_default_classes'] = 1;
//  $handler->display->display_options['fields']['field_agres_rdate']['hide_empty'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['empty_zero'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['hide_alter_empty'] = 0;
//  $handler->display->display_options['fields']['field_agres_rdate']['settings'] = array(
//    'format_type' => 'long',
//    'fromto' => 'both',
//    'multiple_number' => '',
//    'multiple_from' => '',
//    'multiple_to' => '',
//    'show_repeat_rule' => 'show',
//  );
//  $handler->display->display_options['fields']['field_agres_rdate']['field_api_classes'] = 0;
//  /* Field: Content: Nid */
//  $handler->display->display_options['fields']['nid']['id'] = 'nid';
//  $handler->display->display_options['fields']['nid']['table'] = 'node';
//  $handler->display->display_options['fields']['nid']['field'] = 'nid';
//  $handler->display->display_options['fields']['nid']['label'] = '';
//  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 1;
//  $handler->display->display_options['fields']['nid']['alter']['text'] = 'N:[nid]';
//  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
//  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
//  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
//  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
//  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
//  $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
//  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
//  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
//  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
//  $handler->display->display_options['fields']['nid']['link_to_node'] = 1;
//  /* Field: Content: Order_id */
//  $handler->display->display_options['fields']['field_agres_orderid']['id'] = 'field_agres_orderid';
//  $handler->display->display_options['fields']['field_agres_orderid']['table'] = 'field_data_field_agres_orderid';
//  $handler->display->display_options['fields']['field_agres_orderid']['field'] = 'field_agres_orderid';
//  $handler->display->display_options['fields']['field_agres_orderid']['label'] = '';
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['alter_text'] = 1;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['text'] = 'O:[field_agres_orderid]';
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['make_link'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['absolute'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['external'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['replace_spaces'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['trim_whitespace'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['nl2br'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['word_boundary'] = 1;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['ellipsis'] = 1;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['strip_tags'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['trim'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['alter']['html'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['element_label_colon'] = FALSE;
//  $handler->display->display_options['fields']['field_agres_orderid']['element_default_classes'] = 1;
//  $handler->display->display_options['fields']['field_agres_orderid']['hide_empty'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['empty_zero'] = 0;
//  $handler->display->display_options['fields']['field_agres_orderid']['hide_alter_empty'] = 1;
//  $handler->display->display_options['fields']['field_agres_orderid']['settings'] = array(
//    'thousand_separator' => ' ',
//    'prefix_suffix' => 0,
//  );
//  $handler->display->display_options['fields']['field_agres_orderid']['field_api_classes'] = 0;
//  $handler->display->display_options['defaults']['arguments'] = FALSE;
//  /* Contextual filter: Date: Date (node) */
//  $handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
//  $handler->display->display_options['arguments']['date_argument']['table'] = 'node';
//  $handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
//  $handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
//  $handler->display->display_options['arguments']['date_argument']['default_argument_skip_url'] = 0;
//  $handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
//  $handler->display->display_options['arguments']['date_argument']['granularity'] = 'day';
//  $handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
//    'field_data_field_agres_rdate.field_agres_rdate_value' => 'field_data_field_agres_rdate.field_agres_rdate_value',
//    'field_data_field_agres_rdate.field_agres_rdate_value2' => 'field_data_field_agres_rdate.field_agres_rdate_value2',
//  );
//  $handler->display->display_options['arguments']['date_argument']['date_method'] = 'AND';
//  $handler->display->display_options['path'] = 'agres_view/day';
//  $handler->display->display_options['menu']['title'] = 'Day';
//  $handler->display->display_options['menu']['weight'] = '3';
//
//  /* Display: Year */
//  $handler = $view->new_display('page', 'Year', 'page');
//  $handler->display->display_options['defaults']['pager'] = FALSE;
//  $handler->display->display_options['pager']['type'] = 'date_views_pager';
//  $handler->display->display_options['pager']['options']['date_id'] = 'year';
//  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
//  $handler->display->display_options['style_plugin'] = 'agres_views_style';
//  $handler->display->display_options['style_options']['name_size'] = '1';
//  $handler->display->display_options['style_options']['mini'] = '0';
//  $handler->display->display_options['style_options']['with_weekno'] = '0';
//  $handler->display->display_options['style_options']['multiday_theme'] = '1';
//  $handler->display->display_options['style_options']['theme_style'] = '1';
//  $handler->display->display_options['style_options']['max_items'] = '0';
//  $handler->display->display_options['style_options']['agres_views_type'] = 'year';
//  $handler->display->display_options['defaults']['style_options'] = FALSE;
//  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
//  $handler->display->display_options['row_plugin'] = 'agres_views_node';
//  $handler->display->display_options['defaults']['row_options'] = FALSE;
//  $handler->display->display_options['defaults']['arguments'] = FALSE;
//  /* Contextual filter: Date: Date (node) */
//  $handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
//  $handler->display->display_options['arguments']['date_argument']['table'] = 'node';
//  $handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
//  $handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
//  $handler->display->display_options['arguments']['date_argument']['default_argument_skip_url'] = 0;
//  $handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
//  $handler->display->display_options['arguments']['date_argument']['granularity'] = 'year';
//  $handler->display->display_options['arguments']['date_argument']['add_delta'] = 'yes';
//  $handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
//    'node.changed' => 'node.changed',
//  );
//  $handler->display->display_options['path'] = 'agres_view/year';
//  $handler->display->display_options['menu']['title'] = 'Year';
//  $handler->display->display_options['menu']['weight'] = '4';
//
//  /* Display: Block */
//  $handler = $view->new_display('block', 'Block', 'block_1');
//  $handler->display->display_options['defaults']['pager'] = FALSE;
//  $handler->display->display_options['pager']['type'] = 'date_views_pager';
//  $handler->display->display_options['pager']['options']['date_id'] = 'mini';
//  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
//  $handler->display->display_options['style_plugin'] = 'agres_views_style';
//  $handler->display->display_options['style_options']['name_size'] = '1';
//  $handler->display->display_options['style_options']['mini'] = '1';
//  $handler->display->display_options['style_options']['with_weekno'] = '0';
//  $handler->display->display_options['style_options']['multiday_theme'] = '1';
//  $handler->display->display_options['style_options']['theme_style'] = '1';
//  $handler->display->display_options['style_options']['max_items'] = '0';
//  $handler->display->display_options['defaults']['style_options'] = FALSE;
//  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
//  $handler->display->display_options['row_plugin'] = 'agres_views_node';
//  $handler->display->display_options['defaults']['row_options'] = FALSE;
//  $handler->display->display_options['defaults']['arguments'] = FALSE;
//  /* Contextual filter: Date: Date (node) */
//  $handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
//  $handler->display->display_options['arguments']['date_argument']['table'] = 'node';
//  $handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
//  $handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
//  $handler->display->display_options['arguments']['date_argument']['default_argument_skip_url'] = 0;
//  $handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
//  $handler->display->display_options['arguments']['date_argument']['add_delta'] = 'yes';
//  $handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
//    'node.changed' => 'node.changed',
//  );
//
//  /* Display: Upcoming */
//  $handler = $view->new_display('block', 'Upcoming', 'block_2');
//  $handler->display->display_options['display_description'] = 'Upcoming events block';
//  $handler->display->display_options['defaults']['pager'] = FALSE;
//  $handler->display->display_options['pager']['type'] = 'some';
//  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
//  $handler->display->display_options['pager']['options']['offset'] = '0';
//  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
//  $handler->display->display_options['style_plugin'] = 'list';
//  $handler->display->display_options['defaults']['style_options'] = FALSE;
//  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
//  $handler->display->display_options['row_plugin'] = 'fields';
//  $handler->display->display_options['defaults']['row_options'] = FALSE;
//  $handler->display->display_options['defaults']['filters'] = FALSE;
//  /* Filter criterion: Content: Published */
//  $handler->display->display_options['filters']['status']['id'] = 'status';
//  $handler->display->display_options['filters']['status']['table'] = 'node';
//  $handler->display->display_options['filters']['status']['field'] = 'status';
//  $handler->display->display_options['filters']['status']['value'] = 1;
//  $handler->display->display_options['filters']['status']['group'] = 0;
//  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
//  /* Filter criterion: Date: Date (node) */
//  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
//  $handler->display->display_options['filters']['date_filter']['table'] = 'node';
//  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
//  $handler->display->display_options['filters']['date_filter']['operator'] = '>=';
//  $handler->display->display_options['filters']['date_filter']['default_date'] = 'now';
//  $handler->display->display_options['filters']['date_filter']['add_delta'] = 'yes';
//  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
//    'node.changed' => 'node.changed',
//  );
//  $translatables['agres_view'] = array(
//    t('Master'),
//    t('Booking Calendar'),
//    t('more'),
//    t('Apply'),
//    t('Reset'),
//    t('Sort by'),
//    t('Asc'),
//    t('Desc'),
//    t('Booking Date'),
//    t('Nid'),
//    t('Month'),
//    t('N:[nid]'),
//    t('O:[field_agres_orderid]'),
//    t('All'),
//    t('Week'),
//    t('O:[field_agres_orderid] '),
//    t('Day'),
//    t('Year'),
//    t('Block'),
//    t('Upcoming'),
//    t('Upcoming events block'),
//  );
//  $views['agres_view'] = $view;

  $view = new view;
$view->name = 'agres_view';
$view->description = 'A bookingcalendar';
$view->tag = 'agres_view';
$view->base_table = 'node';
$view->human_name = 'Booking Calendar';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Booking Calendar';
$handler->display->display_options['link_display'] = 'page_1';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'view agres_views bookingcalendar';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'date_views_pager';
$handler->display->display_options['pager']['options']['date_id'] = 'month';
$handler->display->display_options['style_plugin'] = 'agres_views_style';
$handler->display->display_options['style_options']['name_size'] = '3';
$handler->display->display_options['style_options']['with_weekno'] = '0';
$handler->display->display_options['style_options']['multiday_theme'] = '1';
$handler->display->display_options['style_options']['theme_style'] = '1';
$handler->display->display_options['style_options']['max_items'] = '0';
$handler->display->display_options['row_plugin'] = 'agres_views_node';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title']['alter']['external'] = 0;
$handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['title']['element_default_classes'] = 1;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Content: Booking Date */
$handler->display->display_options['fields']['field_agres_rdate']['id'] = 'field_agres_rdate';
$handler->display->display_options['fields']['field_agres_rdate']['table'] = 'field_data_field_agres_rdate';
$handler->display->display_options['fields']['field_agres_rdate']['field'] = 'field_agres_rdate';
$handler->display->display_options['fields']['field_agres_rdate']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['external'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['element_label_colon'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['element_default_classes'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['settings'] = array(
  'format_type' => 'long',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_repeat_rule' => 'show',
);
$handler->display->display_options['fields']['field_agres_rdate']['field_api_classes'] = 0;
/* Field: Content: Nid */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'node';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
/* Sort criterion: Content: Updated date */
$handler->display->display_options['sorts']['changed']['id'] = 'changed';
$handler->display->display_options['sorts']['changed']['table'] = 'node';
$handler->display->display_options['sorts']['changed']['field'] = 'changed';
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 0;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

/* Display: Month */
$handler = $view->new_display('page', 'Month', 'page_1');
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'date_views_pager';
$handler->display->display_options['pager']['options']['date_id'] = 'month';
$handler->display->display_options['pager']['options']['link_format'] = 'clean';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'agres_views_style';
$handler->display->display_options['style_options']['name_size'] = '3';
$handler->display->display_options['style_options']['mini'] = '0';
$handler->display->display_options['style_options']['with_weekno'] = '0';
$handler->display->display_options['style_options']['multiday_theme'] = '0';
$handler->display->display_options['style_options']['theme_style'] = '1';
$handler->display->display_options['style_options']['max_items'] = '0';
$handler->display->display_options['style_options']['agres_views_type'] = 'month';
$handler->display->display_options['style_options']['track_date'] = '2';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'agres_views_node';
$handler->display->display_options['row_options']['hide_empty'] = 0;
$handler->display->display_options['row_options']['default_field_elements'] = 1;
$handler->display->display_options['row_options']['colors']['legend'] = 'type';
$handler->display->display_options['row_options']['colors']['calendar_colors_type'] = array(
  'article' => '#ffffff',
  'page' => '#ffffff',
  'product' => '#ffffff',
  'agreservation' => '#ffffff',
  'agres_timeframe' => '#ffffff',
  'agreservations_unit' => '#ffffff',
  'agreservation_unit_info' => '#ffffff',
  'agres_category' => '#ffffff',
  'agres_rate' => '#ffffff',
  'agreservations_unittype' => '#ffffff',
);
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['exclude'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title']['alter']['external'] = 0;
$handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['title']['element_default_classes'] = 1;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Content: Booking Date */
$handler->display->display_options['fields']['field_agres_rdate']['id'] = 'field_agres_rdate';
$handler->display->display_options['fields']['field_agres_rdate']['table'] = 'field_data_field_agres_rdate';
$handler->display->display_options['fields']['field_agres_rdate']['field'] = 'field_agres_rdate';
$handler->display->display_options['fields']['field_agres_rdate']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['external'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['element_label_colon'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['element_default_classes'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['settings'] = array(
  'format_type' => 'long',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_repeat_rule' => 'show',
);
$handler->display->display_options['fields']['field_agres_rdate']['field_api_classes'] = 0;
/* Field: Content: Nid */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'node';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
$handler->display->display_options['fields']['nid']['label'] = '';
$handler->display->display_options['fields']['nid']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['nid']['alter']['text'] = 'N:[nid]';
$handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['nid']['alter']['external'] = 0;
$handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['nid']['alter']['html'] = 0;
$handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['nid']['hide_empty'] = 0;
$handler->display->display_options['fields']['nid']['empty_zero'] = 0;
$handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['nid']['link_to_node'] = 1;
/* Field: Content: Order_id */
$handler->display->display_options['fields']['field_agres_orderid']['id'] = 'field_agres_orderid';
$handler->display->display_options['fields']['field_agres_orderid']['table'] = 'field_data_field_agres_orderid';
$handler->display->display_options['fields']['field_agres_orderid']['field'] = 'field_agres_orderid';
$handler->display->display_options['fields']['field_agres_orderid']['label'] = '';
$handler->display->display_options['fields']['field_agres_orderid']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['text'] = 'O:[field_agres_orderid]';
$handler->display->display_options['fields']['field_agres_orderid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['external'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_agres_orderid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['field_agres_orderid']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['field_agres_orderid']['type'] = 'number_unformatted';
$handler->display->display_options['fields']['field_agres_orderid']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
$handler->display->display_options['fields']['field_agres_orderid']['field_api_classes'] = 0;
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Date: Date (node) */
$handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['table'] = 'node';
$handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
$handler->display->display_options['arguments']['date_argument']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['date_argument']['add_delta'] = 'yes';
$handler->display->display_options['arguments']['date_argument']['granularity'] = 'month';
$handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
  'field_data_field_agres_rdate.field_agres_rdate_value' => 'field_data_field_agres_rdate.field_agres_rdate_value',
  'field_data_field_agres_rdate.field_agres_rdate_value2' => 'field_data_field_agres_rdate.field_agres_rdate_value2',
);
$handler->display->display_options['arguments']['date_argument']['date_method'] = 'AND';
$handler->display->display_options['path'] = 'agres_view/month';
$handler->display->display_options['menu']['type'] = 'default tab';
$handler->display->display_options['menu']['title'] = 'Month';
$handler->display->display_options['menu']['description'] = 'A booking Calendar';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'main-menu';
$handler->display->display_options['menu']['context'] = 0;

$handler->display->display_options['tab_options']['type'] = 'normal';
$handler->display->display_options['tab_options']['title'] = 'Booking Calendar';
$handler->display->display_options['tab_options']['weight'] = '0';
$handler->display->display_options['tab_options']['name'] = 'main-menu';

/* Display: Week */
$handler = $view->new_display('page', 'Week', 'page_2');
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'date_views_pager';
$handler->display->display_options['pager']['options']['date_id'] = 'week';
$handler->display->display_options['pager']['options']['link_format'] = 'clean';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'agres_views_style';
$handler->display->display_options['style_options']['name_size'] = '3';
$handler->display->display_options['style_options']['mini'] = '0';
$handler->display->display_options['style_options']['with_weekno'] = '0';
$handler->display->display_options['style_options']['multiday_theme'] = '1';
$handler->display->display_options['style_options']['theme_style'] = '0';
$handler->display->display_options['style_options']['max_items'] = '0';
$handler->display->display_options['style_options']['agres_views_type'] = 'week';
$handler->display->display_options['style_options']['track_date'] = '2';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'agres_views_node';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title']['alter']['external'] = 0;
$handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['title']['element_default_classes'] = 1;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Content: Booking Date */
$handler->display->display_options['fields']['field_agres_rdate']['id'] = 'field_agres_rdate';
$handler->display->display_options['fields']['field_agres_rdate']['table'] = 'field_data_field_agres_rdate';
$handler->display->display_options['fields']['field_agres_rdate']['field'] = 'field_agres_rdate';
$handler->display->display_options['fields']['field_agres_rdate']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['external'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['element_label_colon'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['element_default_classes'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['settings'] = array(
  'format_type' => 'long',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_repeat_rule' => 'show',
);
$handler->display->display_options['fields']['field_agres_rdate']['field_api_classes'] = 0;
/* Field: Content: Nid */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'node';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
$handler->display->display_options['fields']['nid']['label'] = '';
$handler->display->display_options['fields']['nid']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['nid']['alter']['text'] = 'N:[nid]';
$handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['nid']['alter']['external'] = 0;
$handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['nid']['alter']['html'] = 0;
$handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['nid']['hide_empty'] = 0;
$handler->display->display_options['fields']['nid']['empty_zero'] = 0;
$handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['nid']['link_to_node'] = 1;
/* Field: Content: Order_id */
$handler->display->display_options['fields']['field_agres_orderid']['id'] = 'field_agres_orderid';
$handler->display->display_options['fields']['field_agres_orderid']['table'] = 'field_data_field_agres_orderid';
$handler->display->display_options['fields']['field_agres_orderid']['field'] = 'field_agres_orderid';
$handler->display->display_options['fields']['field_agres_orderid']['label'] = '';
$handler->display->display_options['fields']['field_agres_orderid']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['text'] = 'O:[field_agres_orderid] ';
$handler->display->display_options['fields']['field_agres_orderid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['external'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_agres_orderid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['field_agres_orderid']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['field_agres_orderid']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 0,
);
$handler->display->display_options['fields']['field_agres_orderid']['field_api_classes'] = 0;
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Date: Date (node) */
$handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['table'] = 'node';
$handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
$handler->display->display_options['arguments']['date_argument']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['date_argument']['granularity'] = 'week';
$handler->display->display_options['arguments']['date_argument']['add_delta'] = 'yes';
$handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
  'field_data_field_agres_rdate.field_agres_rdate_value' => 'field_data_field_agres_rdate.field_agres_rdate_value',
  'field_data_field_agres_rdate.field_agres_rdate_value2' => 'field_data_field_agres_rdate.field_agres_rdate_value2',
);
$handler->display->display_options['arguments']['date_argument']['date_method'] = 'AND';
$handler->display->display_options['path'] = 'agres_view/week';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Week';
$handler->display->display_options['menu']['weight'] = '2';
$handler->display->display_options['menu']['context'] = 0;

/* Display: Day */
$handler = $view->new_display('page', 'Day', 'page_3');
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'date_views_pager';
$handler->display->display_options['pager']['options']['date_id'] = 'day';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'agres_views_style';
$handler->display->display_options['style_options']['name_size'] = '3';
$handler->display->display_options['style_options']['mini'] = '0';
$handler->display->display_options['style_options']['with_weekno'] = '0';
$handler->display->display_options['style_options']['multiday_theme'] = '0';
$handler->display->display_options['style_options']['theme_style'] = '0';
$handler->display->display_options['style_options']['max_items'] = '0';
$handler->display->display_options['style_options']['agres_views_type'] = 'day';
$handler->display->display_options['style_options']['track_date'] = '2';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'agres_views_node';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['exclude'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title']['alter']['external'] = 0;
$handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['title']['element_default_classes'] = 1;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Content: Booking Date */
$handler->display->display_options['fields']['field_agres_rdate']['id'] = 'field_agres_rdate';
$handler->display->display_options['fields']['field_agres_rdate']['table'] = 'field_data_field_agres_rdate';
$handler->display->display_options['fields']['field_agres_rdate']['field'] = 'field_agres_rdate';
$handler->display->display_options['fields']['field_agres_rdate']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['external'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['element_label_colon'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['element_default_classes'] = 1;
$handler->display->display_options['fields']['field_agres_rdate']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['field_agres_rdate']['settings'] = array(
  'format_type' => 'long',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_repeat_rule' => 'show',
);
$handler->display->display_options['fields']['field_agres_rdate']['field_api_classes'] = 0;
/* Field: Content: Nid */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'node';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
$handler->display->display_options['fields']['nid']['label'] = '';
$handler->display->display_options['fields']['nid']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['nid']['alter']['text'] = 'N:[nid]';
$handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['nid']['alter']['external'] = 0;
$handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['nid']['alter']['html'] = 0;
$handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['nid']['hide_empty'] = 0;
$handler->display->display_options['fields']['nid']['empty_zero'] = 0;
$handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['nid']['link_to_node'] = 1;
/* Field: Content: Order_id */
$handler->display->display_options['fields']['field_agres_orderid']['id'] = 'field_agres_orderid';
$handler->display->display_options['fields']['field_agres_orderid']['table'] = 'field_data_field_agres_orderid';
$handler->display->display_options['fields']['field_agres_orderid']['field'] = 'field_agres_orderid';
$handler->display->display_options['fields']['field_agres_orderid']['label'] = '';
$handler->display->display_options['fields']['field_agres_orderid']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['text'] = 'O:[field_agres_orderid]';
$handler->display->display_options['fields']['field_agres_orderid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['external'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_agres_orderid']['element_default_classes'] = 1;
$handler->display->display_options['fields']['field_agres_orderid']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_agres_orderid']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['field_agres_orderid']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 0,
);
$handler->display->display_options['fields']['field_agres_orderid']['field_api_classes'] = 0;
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Date: Date (node) */
$handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['table'] = 'node';
$handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
$handler->display->display_options['arguments']['date_argument']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['date_argument']['granularity'] = 'day';
$handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
  'field_data_field_agres_rdate.field_agres_rdate_value' => 'field_data_field_agres_rdate.field_agres_rdate_value',
  'field_data_field_agres_rdate.field_agres_rdate_value2' => 'field_data_field_agres_rdate.field_agres_rdate_value2',
);
$handler->display->display_options['arguments']['date_argument']['date_method'] = 'AND';
$handler->display->display_options['path'] = 'agres_view/day';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Day';
$handler->display->display_options['menu']['weight'] = '3';
$handler->display->display_options['menu']['context'] = 0;

/* Display: Year */
$handler = $view->new_display('page', 'Year', 'page');
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'date_views_pager';
$handler->display->display_options['pager']['options']['date_id'] = 'year';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'agres_views_style';
$handler->display->display_options['style_options']['name_size'] = '1';
$handler->display->display_options['style_options']['mini'] = '0';
$handler->display->display_options['style_options']['with_weekno'] = '0';
$handler->display->display_options['style_options']['multiday_theme'] = '1';
$handler->display->display_options['style_options']['theme_style'] = '1';
$handler->display->display_options['style_options']['max_items'] = '0';
$handler->display->display_options['style_options']['agres_views_type'] = 'year';
$handler->display->display_options['style_options']['track_date'] = '2';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'agres_views_node';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Date: Date (node) */
$handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['table'] = 'node';
$handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
$handler->display->display_options['arguments']['date_argument']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['date_argument']['granularity'] = 'year';
$handler->display->display_options['arguments']['date_argument']['add_delta'] = 'yes';
$handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
  'node.changed' => 'node.changed',
);
$handler->display->display_options['path'] = 'agres_view/year';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Year';
$handler->display->display_options['menu']['weight'] = '4';
$handler->display->display_options['menu']['context'] = 0;

/* Display: Block */
$handler = $view->new_display('block', 'Block', 'block_1');
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'date_views_pager';
$handler->display->display_options['pager']['options']['date_id'] = 'mini';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'agres_views_style';
$handler->display->display_options['style_options']['name_size'] = '1';
$handler->display->display_options['style_options']['mini'] = '1';
$handler->display->display_options['style_options']['with_weekno'] = '0';
$handler->display->display_options['style_options']['multiday_theme'] = '1';
$handler->display->display_options['style_options']['theme_style'] = '1';
$handler->display->display_options['style_options']['max_items'] = '0';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'agres_views_node';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Date: Date (node) */
$handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['table'] = 'node';
$handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
$handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
$handler->display->display_options['arguments']['date_argument']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['date_argument']['add_delta'] = 'yes';
$handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
  'node.changed' => 'node.changed',
);

/* Display: Upcoming */
$handler = $view->new_display('block', 'Upcoming', 'block_2');
$handler->display->display_options['display_description'] = 'Upcoming events block';
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '5';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'list';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 0;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Date: Date (node) */
$handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
$handler->display->display_options['filters']['date_filter']['table'] = 'node';
$handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
$handler->display->display_options['filters']['date_filter']['operator'] = '>=';
$handler->display->display_options['filters']['date_filter']['default_date'] = 'now';
$handler->display->display_options['filters']['date_filter']['add_delta'] = 'yes';
$handler->display->display_options['filters']['date_filter']['date_fields'] = array(
  'node.changed' => 'node.changed',
);
$translatables['agres_view'] = array(
  t('Master'),
  t('Booking Calendar'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Booking Date'),
  t('Nid'),
  t('Month'),
  t('N:[nid]'),
  t('O:[field_agres_orderid]'),
  t('All'),
  t('Week'),
  t('O:[field_agres_orderid] '),
  t('Day'),
  t('Year'),
  t('Block'),
  t('Upcoming'),
  t('Upcoming events block'),
);
 $views['agres_view'] = $view;
  return $views;
}

<?php
/**
 * @file
 * debut_in_the_news.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function debut_in_the_news_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'in_the_news';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'In The News';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'In The News';
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['hide_empty'] = 0;
  $handler->display->display_options['row_options']['default_field_elements'] = 1;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['title']['alter']['path'] = '[field_link]';
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 1;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['target'] = '_blank';
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
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 0;
  /* Field: Content: Date Published */
  $handler->display->display_options['fields']['field_date_published']['id'] = 'field_date_published';
  $handler->display->display_options['fields']['field_date_published']['table'] = 'field_data_field_date_published';
  $handler->display->display_options['fields']['field_date_published']['field'] = 'field_date_published';
  $handler->display->display_options['fields']['field_date_published']['label'] = '';
  $handler->display->display_options['fields']['field_date_published']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_date_published']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_date_published']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_date_published']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_date_published']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_date_published']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_date_published']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_date_published']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_date_published']['settings'] = array(
    'format_type' => 'event_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_date_published']['field_api_classes'] = 0;
  /* Sort criterion: Content: Date Published (field_date_published) */
  $handler->display->display_options['sorts']['field_date_published_value']['id'] = 'field_date_published_value';
  $handler->display->display_options['sorts']['field_date_published_value']['table'] = 'field_data_field_date_published';
  $handler->display->display_options['sorts']['field_date_published_value']['field'] = 'field_date_published_value';
  $handler->display->display_options['sorts']['field_date_published_value']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'in_the_news' => 'in_the_news',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['title']['alter']['path'] = '[field_link]';
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 1;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['target'] = '_blank';
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
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 0;
  /* Field: Content: Date Published */
  $handler->display->display_options['fields']['field_date_published']['id'] = 'field_date_published';
  $handler->display->display_options['fields']['field_date_published']['table'] = 'field_data_field_date_published';
  $handler->display->display_options['fields']['field_date_published']['field'] = 'field_date_published';
  $handler->display->display_options['fields']['field_date_published']['label'] = '';
  $handler->display->display_options['fields']['field_date_published']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_date_published']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_date_published']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_date_published']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_date_published']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_date_published']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_date_published']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_date_published']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_date_published']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_date_published']['settings'] = array(
    'format_type' => 'event_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_date_published']['field_api_classes'] = 0;
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_content_image']['id'] = 'field_content_image';
  $handler->display->display_options['fields']['field_content_image']['table'] = 'field_data_field_content_image';
  $handler->display->display_options['fields']['field_content_image']['field'] = 'field_content_image';
  $handler->display->display_options['fields']['field_content_image']['label'] = '';
  $handler->display->display_options['fields']['field_content_image']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_content_image']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_content_image']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_content_image']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_content_image']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_content_image']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_content_image']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_content_image']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_content_image']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_content_image']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_content_image']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_content_image']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_content_image']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_content_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_content_image']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_content_image']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_content_image']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_content_image']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_content_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_content_image']['settings'] = array(
    'image_style' => '',
    'image_link' => '',
  );
  $handler->display->display_options['fields']['field_content_image']['field_api_classes'] = 0;
  $handler->display->display_options['path'] = 'news';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'News';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Feed */
  $handler = $view->new_display('feed', 'Feed', 'feed');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['path'] = 'news.xml';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
  );

  /* Display: Recent News */
  $handler = $view->new_display('block', 'Recent News', 'block_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Recent News';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '3';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['block_description'] = 'Recent news';
  $export['in_the_news'] = $view;

  return $export;
}

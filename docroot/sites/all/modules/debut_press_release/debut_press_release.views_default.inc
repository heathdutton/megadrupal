<?php
/**
 * @file
 * debut_press_release.views_default.inc
 */

/**
 * Implementation of hook_views_default_views().
 */
function debut_press_release_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'press_release';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'press_release';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'press_release';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Content: Dateline */
  $handler->display->display_options['fields']['field_dateline']['id'] = 'field_dateline';
  $handler->display->display_options['fields']['field_dateline']['table'] = 'field_data_field_dateline';
  $handler->display->display_options['fields']['field_dateline']['field'] = 'field_dateline';
  $handler->display->display_options['fields']['field_dateline']['label'] = '';
  $handler->display->display_options['fields']['field_dateline']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_dateline']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['field_dateline']['alter']['text'] = '[field_dateline-value] — ';
  $handler->display->display_options['fields']['field_dateline']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_dateline']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_dateline']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_dateline']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_dateline']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_dateline']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_dateline']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_dateline']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_dateline']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_dateline']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_dateline']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_dateline']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_dateline']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_dateline']['hide_empty'] = 1;
  $handler->display->display_options['fields']['field_dateline']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_dateline']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_dateline']['field_api_classes'] = 0;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['body']['alter']['text'] = '[field_dateline][body-value]';
  $handler->display->display_options['fields']['body']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['body']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['body']['alter']['external'] = 0;
  $handler->display->display_options['fields']['body']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['body']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['body']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '300';
  $handler->display->display_options['fields']['body']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['body']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['body']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['body']['alter']['trim'] = 1;
  $handler->display->display_options['fields']['body']['alter']['html'] = 1;
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['body']['hide_empty'] = 0;
  $handler->display->display_options['fields']['body']['empty_zero'] = 0;
  $handler->display->display_options['fields']['body']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['body']['type'] = 'text_summary_or_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '300',
  );
  $handler->display->display_options['fields']['body']['field_api_classes'] = 0;
  /* Field: Content: Release Date */
  $handler->display->display_options['fields']['field_release_date']['id'] = 'field_release_date';
  $handler->display->display_options['fields']['field_release_date']['table'] = 'field_data_field_release_date';
  $handler->display->display_options['fields']['field_release_date']['field'] = 'field_release_date';
  $handler->display->display_options['fields']['field_release_date']['label'] = '';
  $handler->display->display_options['fields']['field_release_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_release_date']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_release_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_release_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_release_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_release_date']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_release_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_release_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_release_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_release_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_release_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_release_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_release_date']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_release_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_release_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_release_date']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_release_date']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_release_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  $handler->display->display_options['fields']['field_release_date']['field_api_classes'] = 0;
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
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'press_release' => 'press_release',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Press Releases';
  $handler->display->display_options['path'] = 'press-release';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Press Releases';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';

  /* Display: Feed */
  $handler = $view->new_display('feed', 'Feed', 'feed');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Press Releases';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['path'] = 'press-release.xml';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
    'block' => 'block',
    'block_1' => 0,
  );
  $handler->display->display_options['sitename_title'] = 0;

  /* Display: Recent Press Releases */
  $handler = $view->new_display('block', 'Recent Press Releases', 'block');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Recent Press Releases';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['use_more_always'] = TRUE;
  $handler->display->display_options['use_more_text'] = 'Read more press releases';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '3';
  $handler->display->display_options['block_description'] = 'Recent Press Releases';

  /* Display: Featured Press Release */
  $handler = $view->new_display('block', 'Featured Press Release', 'block_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Featured Press Release';
  $handler->display->display_options['display_description'] = 'A featured press release, controlled by the Promote to Front setting';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more_always'] = TRUE;
  $handler->display->display_options['use_more_text'] = '';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['group_rendered'] = 1;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Promoted to front page */
  $handler->display->display_options['sorts']['promote']['id'] = 'promote';
  $handler->display->display_options['sorts']['promote']['table'] = 'node';
  $handler->display->display_options['sorts']['promote']['field'] = 'promote';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['block_description'] = 'Featured Press Release';
  $export['press_release'] = $view;

  return $export;
}

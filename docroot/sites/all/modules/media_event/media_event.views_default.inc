<?php

/**
 * Implements hook_views_default_views().
 */
function media_event_views_default_views() {
  $view = new view;
  $view->name = 'media_events';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'media_event';
  $view->human_name = 'Media Events';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Media Events';
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
  $handler->display->display_options['header']['area']['content'] = '<a href="<?php print url(\'media-event/add\'); ?>>Add Media Event</a>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = 0;
  /* Field: Media Event: Media Event ID */
  $handler->display->display_options['fields']['media_event_id']['id'] = 'media_event_id';
  $handler->display->display_options['fields']['media_event_id']['table'] = 'media_event';
  $handler->display->display_options['fields']['media_event_id']['field'] = 'media_event_id';
  $handler->display->display_options['fields']['media_event_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['media_event_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['media_event_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['media_event_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['media_event_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['media_event_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['media_event_id']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['media_event_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['media_event_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['media_event_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['media_event_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['media_event_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['media_event_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['media_event_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['media_event_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['media_event_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['media_event_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['media_event_id']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['media_event_id']['format_plural'] = 0;
  /* Field: Media Event: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'media_event';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'media-event/[media_event_id]';
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
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'media-event/[media_event_id]/edit';
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
  $handler->display->display_options['path'] = 'media-events';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Media Events';
  $handler->display->display_options['menu']['weight'] = '0';

  $views['media_events'] = $view;

  return $views;
}
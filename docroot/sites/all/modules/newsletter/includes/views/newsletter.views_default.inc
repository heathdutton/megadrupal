<?php

/**
 * @file
 * Creates the default view for the 'Newsletter archive'
 */

/**
 * Implementation of hook_views_default_views().
 */
function newsletter_views_default_views() {

  $view = new view;
  $view->name = 'newsletter_archive';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'newsletter_newsletter';
  $view->human_name = 'Newsletter archive';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Newsletter archive';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access newsletter statistics';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nnid' => 'nnid',
    'title' => 'title',
    'send_id' => 'send_id',
    'field_newsletter_body' => 'field_newsletter_body',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nnid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'send_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_newsletter_body' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = FALSE;
  $handler->display->display_options['header']['area']['content'] = 'The issue is stored <strong>after</strong> a newsletter list is sent to all subscribers.
  The contents of the issue could be different from subscriber to subscriber, especially if the newsletter templates are personalized (eg use subscriber tokens).';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['header']['area']['tokenize'] = 0;
  /* Field: Newsletter: Nnid */
  $handler->display->display_options['fields']['nnid']['id'] = 'nnid';
  $handler->display->display_options['fields']['nnid']['table'] = 'newsletter_newsletter';
  $handler->display->display_options['fields']['nnid']['field'] = 'nnid';
  /* Field: Newsletter: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'newsletter_newsletter';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  /* Field: Newsletter: Send ID */
  $handler->display->display_options['fields']['send_id']['id'] = 'send_id';
  $handler->display->display_options['fields']['send_id']['table'] = 'newsletter_newsletter';
  $handler->display->display_options['fields']['send_id']['field'] = 'send_id';
  $handler->display->display_options['fields']['send_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['send_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['send_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['send_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['send_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['send_id']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['send_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['send_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['send_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['send_id']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['send_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['send_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['send_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['send_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['send_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['send_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['send_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['send_id']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['send_id']['format_plural'] = 0;
  /* Field: Field: Body */
  $handler->display->display_options['fields']['field_newsletter_body']['id'] = 'field_newsletter_body';
  $handler->display->display_options['fields']['field_newsletter_body']['table'] = 'field_data_field_newsletter_body';
  $handler->display->display_options['fields']['field_newsletter_body']['field'] = 'field_newsletter_body';
  $handler->display->display_options['fields']['field_newsletter_body']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_newsletter_body']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_newsletter_body']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_newsletter_body']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_newsletter_body']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_newsletter_body']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_newsletter_body']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_newsletter_body']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_newsletter_body']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_newsletter_body']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_newsletter_body']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_newsletter_body']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_newsletter_body']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_newsletter_body']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_newsletter_body']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_newsletter_body']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_newsletter_body']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_newsletter_body']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_newsletter_body']['field_api_classes'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/config/media/newsletter/archive';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Issue archive';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;

  $views[$view->name] = $view;
  return $views;
}

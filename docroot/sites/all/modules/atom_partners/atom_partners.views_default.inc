<?php
/**
 * @file
 * atom_partners.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function atom_partners_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'partners';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Partners';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Our Partners';
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['use_more_always'] = TRUE;
  $handler->display->display_options['use_more_text'] = 'All Partners';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['group_rendered'] = 1;
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Logo */
  $handler->display->display_options['fields']['field_partner_logo']['id'] = 'field_partner_logo';
  $handler->display->display_options['fields']['field_partner_logo']['table'] = 'field_data_field_partner_logo';
  $handler->display->display_options['fields']['field_partner_logo']['field'] = 'field_partner_logo';
  $handler->display->display_options['fields']['field_partner_logo']['label'] = '';
  $handler->display->display_options['fields']['field_partner_logo']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['text'] = '[field_partner_logo]';
  $handler->display->display_options['fields']['field_partner_logo']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['path'] = '[field_partner_website]';
  $handler->display->display_options['fields']['field_partner_logo']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['alt'] = '[title]';
  $handler->display->display_options['fields']['field_partner_logo']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['element_type'] = '0';
  $handler->display->display_options['fields']['field_partner_logo']['element_label_type'] = '0';
  $handler->display->display_options['fields']['field_partner_logo']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_partner_logo']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_partner_logo']['element_default_classes'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_partner_logo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_partner_logo']['settings'] = array(
    'image_style' => 'partner_logo',
    'image_link' => 'content',
  );
  $handler->display->display_options['fields']['field_partner_logo']['field_api_classes'] = 0;
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
    'partner' => 'partner',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more_always'] = TRUE;
  $handler->display->display_options['use_more_text'] = 'All Partners';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Logo */
  $handler->display->display_options['fields']['field_partner_logo']['id'] = 'field_partner_logo';
  $handler->display->display_options['fields']['field_partner_logo']['table'] = 'field_data_field_partner_logo';
  $handler->display->display_options['fields']['field_partner_logo']['field'] = 'field_partner_logo';
  $handler->display->display_options['fields']['field_partner_logo']['label'] = '';
  $handler->display->display_options['fields']['field_partner_logo']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['text'] = '[field_partner_logo]';
  $handler->display->display_options['fields']['field_partner_logo']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['path'] = '[field_partner_website]';
  $handler->display->display_options['fields']['field_partner_logo']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['alt'] = '[title]';
  $handler->display->display_options['fields']['field_partner_logo']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_partner_logo']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_partner_logo']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_partner_logo']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_partner_logo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_partner_logo']['settings'] = array(
    'image_style' => 'partner_logo',
    'image_link' => 'content',
  );
  $handler->display->display_options['fields']['field_partner_logo']['field_api_classes'] = 0;
  $handler->display->display_options['path'] = 'partners';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Partners';
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $export['partners'] = $view;

  return $export;
}

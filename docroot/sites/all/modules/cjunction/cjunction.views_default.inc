<?php

/**
 * Implementation of hook_views_default_views().
 */
function cjunction_views_default_views() {
  $views = array();

  // Views - Products
  $view = new view;
  $view->name = 'commission_junction';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Commission Junction';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Commission Junction';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '12';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['style_options']['columns'] = '3';
  $handler->display->display_options['style_options']['fill_single_line'] = 1;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['hide_empty'] = 0;
  $handler->display->display_options['row_options']['default_field_elements'] = 1;
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
  /* Field: Content: Image URL */
  $handler->display->display_options['fields']['cj_image_url']['id'] = 'cj_image_url';
  $handler->display->display_options['fields']['cj_image_url']['table'] = 'field_data_cj_image_url';
  $handler->display->display_options['fields']['cj_image_url']['field'] = 'cj_image_url';
  $handler->display->display_options['fields']['cj_image_url']['label'] = '';
  $handler->display->display_options['fields']['cj_image_url']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['cj_image_url']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['cj_image_url']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['cj_image_url']['alter']['external'] = 0;
  $handler->display->display_options['fields']['cj_image_url']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['cj_image_url']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['cj_image_url']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['cj_image_url']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['cj_image_url']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['cj_image_url']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['cj_image_url']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['cj_image_url']['alter']['html'] = 0;
  $handler->display->display_options['fields']['cj_image_url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cj_image_url']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['cj_image_url']['hide_empty'] = 0;
  $handler->display->display_options['fields']['cj_image_url']['empty_zero'] = 0;
  $handler->display->display_options['fields']['cj_image_url']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['cj_image_url']['field_api_classes'] = 0;
  /* Field: Content: Price */
  $handler->display->display_options['fields']['cj_price']['id'] = 'cj_price';
  $handler->display->display_options['fields']['cj_price']['table'] = 'field_data_cj_price';
  $handler->display->display_options['fields']['cj_price']['field'] = 'cj_price';
  $handler->display->display_options['fields']['cj_price']['label'] = '';
  $handler->display->display_options['fields']['cj_price']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['cj_price']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['cj_price']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['cj_price']['alter']['external'] = 0;
  $handler->display->display_options['fields']['cj_price']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['cj_price']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['cj_price']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['cj_price']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['cj_price']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['cj_price']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['cj_price']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['cj_price']['alter']['html'] = 0;
  $handler->display->display_options['fields']['cj_price']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cj_price']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['cj_price']['hide_empty'] = 0;
  $handler->display->display_options['fields']['cj_price']['empty_zero'] = 0;
  $handler->display->display_options['fields']['cj_price']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['cj_price']['field_api_classes'] = 0;
  /* Field: Content: Buy URL */
  $handler->display->display_options['fields']['cj_buy_url']['id'] = 'cj_buy_url';
  $handler->display->display_options['fields']['cj_buy_url']['table'] = 'field_data_cj_buy_url';
  $handler->display->display_options['fields']['cj_buy_url']['field'] = 'cj_buy_url';
  $handler->display->display_options['fields']['cj_buy_url']['label'] = '';
  $handler->display->display_options['fields']['cj_buy_url']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['cj_buy_url']['alter']['text'] = 'Buy Now';
  $handler->display->display_options['fields']['cj_buy_url']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['cj_buy_url']['alter']['path'] = '[cj_buy_url-value] ';
  $handler->display->display_options['fields']['cj_buy_url']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['cj_buy_url']['alter']['external'] = 0;
  $handler->display->display_options['fields']['cj_buy_url']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['cj_buy_url']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['cj_buy_url']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['cj_buy_url']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['cj_buy_url']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['cj_buy_url']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['cj_buy_url']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['cj_buy_url']['alter']['html'] = 0;
  $handler->display->display_options['fields']['cj_buy_url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['cj_buy_url']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['cj_buy_url']['hide_empty'] = 0;
  $handler->display->display_options['fields']['cj_buy_url']['empty_zero'] = 0;
  $handler->display->display_options['fields']['cj_buy_url']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['cj_buy_url']['field_api_classes'] = 0;
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
    'cjproduct' => 'cjproduct',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'node';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['value'] = '1';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'commission-junction';


  $views[$view->name] = $view;

  return $views;
}

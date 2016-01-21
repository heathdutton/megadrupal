<?php

/**
 * @file
 * Affiliate store default views.
 */

/**
 * Implements hook_views_default_views().
 */
function affiliate_store_views_default_views() {
  $view = new view;
  $view->name = 'affiliate_store';
  $view->description = 'Display list of affiliate store products.';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Affiliate store';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = TRUE;

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Store';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access affiliate store product';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
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
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Affiliate store: Product image */
  $handler->display->display_options['fields']['image_url']['id'] = 'image_url';
  $handler->display->display_options['fields']['image_url']['table'] = 'affiliate_store_product';
  $handler->display->display_options['fields']['image_url']['field'] = 'image_url';
  $handler->display->display_options['fields']['image_url']['label'] = '';
  $handler->display->display_options['fields']['image_url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['image_url']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['image_url']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['image_url']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['image_url']['alter']['external'] = 0;
  $handler->display->display_options['fields']['image_url']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['image_url']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['image_url']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['image_url']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['image_url']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['image_url']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['image_url']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['image_url']['alter']['html'] = 0;
  $handler->display->display_options['fields']['image_url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['image_url']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['image_url']['hide_empty'] = 0;
  $handler->display->display_options['fields']['image_url']['empty_zero'] = 0;
  $handler->display->display_options['fields']['image_url']['hide_alter_empty'] = 1;
  /* Field: Affiliate store: Product thumbnail */
  $handler->display->display_options['fields']['thumb_url']['id'] = 'thumb_url';
  $handler->display->display_options['fields']['thumb_url']['table'] = 'affiliate_store_product';
  $handler->display->display_options['fields']['thumb_url']['field'] = 'thumb_url';
  $handler->display->display_options['fields']['thumb_url']['label'] = '';
  $handler->display->display_options['fields']['thumb_url']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['thumb_url']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['thumb_url']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['thumb_url']['alter']['external'] = 0;
  $handler->display->display_options['fields']['thumb_url']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['thumb_url']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['thumb_url']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['thumb_url']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['thumb_url']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['thumb_url']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['thumb_url']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['thumb_url']['alter']['html'] = 0;
  $handler->display->display_options['fields']['thumb_url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['thumb_url']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['thumb_url']['hide_empty'] = 0;
  $handler->display->display_options['fields']['thumb_url']['empty_zero'] = 0;
  $handler->display->display_options['fields']['thumb_url']['hide_alter_empty'] = 1;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['body']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['body']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['body']['alter']['external'] = 0;
  $handler->display->display_options['fields']['body']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['body']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['body']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['body']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['body']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['body']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['body']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['body']['alter']['html'] = 0;
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['body']['hide_empty'] = 0;
  $handler->display->display_options['fields']['body']['empty_zero'] = 0;
  $handler->display->display_options['fields']['body']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['body']['field_api_classes'] = 0;
  /* Field: Affiliate store: Product currency */
  $handler->display->display_options['fields']['currency']['id'] = 'currency';
  $handler->display->display_options['fields']['currency']['table'] = 'affiliate_store_product';
  $handler->display->display_options['fields']['currency']['field'] = 'currency';
  $handler->display->display_options['fields']['currency']['label'] = '';
  $handler->display->display_options['fields']['currency']['exclude'] = TRUE;
  $handler->display->display_options['fields']['currency']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['currency']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['currency']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['currency']['alter']['external'] = 0;
  $handler->display->display_options['fields']['currency']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['currency']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['currency']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['currency']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['currency']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['currency']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['currency']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['currency']['alter']['html'] = 0;
  $handler->display->display_options['fields']['currency']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['currency']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['currency']['hide_empty'] = 0;
  $handler->display->display_options['fields']['currency']['empty_zero'] = 0;
  $handler->display->display_options['fields']['currency']['hide_alter_empty'] = 1;
  /* Field: Affiliate store: Product price */
  $handler->display->display_options['fields']['price']['id'] = 'price';
  $handler->display->display_options['fields']['price']['table'] = 'affiliate_store_product';
  $handler->display->display_options['fields']['price']['field'] = 'price';
  $handler->display->display_options['fields']['price']['label'] = '';
  $handler->display->display_options['fields']['price']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['price']['alter']['text'] = '[currency] [price]';
  $handler->display->display_options['fields']['price']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['price']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['price']['alter']['external'] = 0;
  $handler->display->display_options['fields']['price']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['price']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['price']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['price']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['price']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['price']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['price']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['price']['alter']['html'] = 0;
  $handler->display->display_options['fields']['price']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['price']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['price']['hide_empty'] = 1;
  $handler->display->display_options['fields']['price']['empty_zero'] = 1;
  $handler->display->display_options['fields']['price']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['price']['set_precision'] = 1;
  $handler->display->display_options['fields']['price']['precision'] = '2';
  $handler->display->display_options['fields']['price']['format_plural'] = 0;
  /* Field: Affiliate store: Product link */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'affiliate_store_product';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['label'] = '';
  $handler->display->display_options['fields']['url']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['url']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['url']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['url']['alter']['external'] = 0;
  $handler->display->display_options['fields']['url']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['url']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['url']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['url']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['url']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['url']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['url']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['url']['alter']['html'] = 0;
  $handler->display->display_options['fields']['url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['url']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['url']['hide_empty'] = 0;
  $handler->display->display_options['fields']['url']['empty_zero'] = 0;
  $handler->display->display_options['fields']['url']['hide_alter_empty'] = 1;
  /* Sort criterion: Content: Sticky */
  $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['table'] = 'node';
  $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Sort criterion: Affiliate store: Product price */
  $handler->display->display_options['sorts']['price']['id'] = 'price';
  $handler->display->display_options['sorts']['price']['table'] = 'affiliate_store_product';
  $handler->display->display_options['sorts']['price']['field'] = 'price';
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
    'affiliate_store_product' => 'affiliate_store_product',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'affiliate-store2';

  $views[$view->name] = $view;
  return $views;
}

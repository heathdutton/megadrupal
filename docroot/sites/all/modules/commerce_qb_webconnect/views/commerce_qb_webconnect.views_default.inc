<?php

function commerce_qb_webconnect_views_default_views(){
  $view = new view;
  $view->name = 'quickbooks_exports';
  $view->description = 'List of orders, products and customers to be exported to Quickbooks';
  $view->tag = 'default';
  $view->base_table = 'commerce_qb_exports';
  $view->human_name = 'Quickbooks Exports';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Quickbooks Exports';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Quickbooks export item: Quickbooks export item ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'commerce_qb_exports';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = 'ID';
  $handler->display->display_options['fields']['id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['id']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['id']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['id']['format_plural'] = 0;
  /* Field: Quickbooks export item: Link to referenced entity */
  $handler->display->display_options['fields']['link_to_entity']['id'] = 'link_to_entity';
  $handler->display->display_options['fields']['link_to_entity']['table'] = 'commerce_qb_exports';
  $handler->display->display_options['fields']['link_to_entity']['field'] = 'link_to_entity';
  $handler->display->display_options['fields']['link_to_entity']['label'] = 'Export';
  $handler->display->display_options['fields']['link_to_entity']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['link_to_entity']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['link_to_entity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['link_to_entity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['link_to_entity']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['link_to_entity']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['link_to_entity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['link_to_entity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['link_to_entity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['link_to_entity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['link_to_entity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['link_to_entity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['link_to_entity']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['link_to_entity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['link_to_entity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['link_to_entity']['empty_zero'] = 0;
  $handler->display->display_options['fields']['link_to_entity']['hide_alter_empty'] = 0;
  /* Field: Quickbooks export item: Label */
  $handler->display->display_options['fields']['item_type']['id'] = 'item_type';
  $handler->display->display_options['fields']['item_type']['table'] = 'commerce_qb_exports';
  $handler->display->display_options['fields']['item_type']['field'] = 'item_type';
  $handler->display->display_options['fields']['item_type']['label'] = 'Type';
  $handler->display->display_options['fields']['item_type']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['item_type']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['item_type']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['item_type']['alter']['external'] = 0;
  $handler->display->display_options['fields']['item_type']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['item_type']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['item_type']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['item_type']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['item_type']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['item_type']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['item_type']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['item_type']['alter']['html'] = 0;
  $handler->display->display_options['fields']['item_type']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['item_type']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['item_type']['hide_empty'] = 0;
  $handler->display->display_options['fields']['item_type']['empty_zero'] = 0;
  $handler->display->display_options['fields']['item_type']['hide_alter_empty'] = 0;
  /* Field: Quickbooks export item: Status_code */
  $handler->display->display_options['fields']['status_code']['id'] = 'status_code';
  $handler->display->display_options['fields']['status_code']['table'] = 'commerce_qb_exports';
  $handler->display->display_options['fields']['status_code']['field'] = 'status_code';
  $handler->display->display_options['fields']['status_code']['label'] = 'Status';
  $handler->display->display_options['fields']['status_code']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['status_code']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['status_code']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['status_code']['alter']['external'] = 0;
  $handler->display->display_options['fields']['status_code']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['status_code']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['status_code']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['status_code']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['status_code']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['status_code']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['status_code']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['status_code']['alter']['html'] = 0;
  $handler->display->display_options['fields']['status_code']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['status_code']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['status_code']['hide_empty'] = 0;
  $handler->display->display_options['fields']['status_code']['empty_zero'] = 0;
  $handler->display->display_options['fields']['status_code']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['status_code']['format_plural'] = 0;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  
  $views['quickbooks_exports'] = $view;
  
  return $views;
}
<?php
/**
 * @file
 * amazon_examples.views_default.inc
 */

/**
 * Implementation of hook_views_default_views().
 */
function amazon_examples_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'amazon_example_view';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Amazon Example View';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Amazon Example View';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Content: ASIN (field_asin) - asin */
  $handler->display->display_options['relationships']['field_asin_asin']['id'] = 'field_asin_asin';
  $handler->display->display_options['relationships']['field_asin_asin']['table'] = 'field_data_field_asin';
  $handler->display->display_options['relationships']['field_asin_asin']['field'] = 'field_asin_asin';
  $handler->display->display_options['relationships']['field_asin_asin']['required'] = 1;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid']['link_to_node'] = 1;
  /* Field: Amazon: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'amazon_item';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_asin_asin';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  /* Field: Amazon: ASIN */
  $handler->display->display_options['fields']['asin']['id'] = 'asin';
  $handler->display->display_options['fields']['asin']['table'] = 'amazon_item';
  $handler->display->display_options['fields']['asin']['field'] = 'asin';
  $handler->display->display_options['fields']['asin']['relationship'] = 'field_asin_asin';
  $handler->display->display_options['fields']['asin']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['asin']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['asin']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['asin']['alter']['external'] = 0;
  $handler->display->display_options['fields']['asin']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['asin']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['asin']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['asin']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['asin']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['asin']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['asin']['alter']['html'] = 0;
  $handler->display->display_options['fields']['asin']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['asin']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['asin']['hide_empty'] = 0;
  $handler->display->display_options['fields']['asin']['empty_zero'] = 0;
  /* Field: Amazon: Detail page URL */
  $handler->display->display_options['fields']['detailpageurl']['id'] = 'detailpageurl';
  $handler->display->display_options['fields']['detailpageurl']['table'] = 'amazon_item';
  $handler->display->display_options['fields']['detailpageurl']['field'] = 'detailpageurl';
  $handler->display->display_options['fields']['detailpageurl']['relationship'] = 'field_asin_asin';
  $handler->display->display_options['fields']['detailpageurl']['exclude'] = TRUE;
  $handler->display->display_options['fields']['detailpageurl']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['detailpageurl']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['detailpageurl']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['detailpageurl']['alter']['external'] = 0;
  $handler->display->display_options['fields']['detailpageurl']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['detailpageurl']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['detailpageurl']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['detailpageurl']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['detailpageurl']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['detailpageurl']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['detailpageurl']['alter']['html'] = 0;
  $handler->display->display_options['fields']['detailpageurl']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['detailpageurl']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['detailpageurl']['hide_empty'] = 0;
  $handler->display->display_options['fields']['detailpageurl']['empty_zero'] = 0;
  /* Field: Amazon: Publisher */
  $handler->display->display_options['fields']['publisher']['id'] = 'publisher';
  $handler->display->display_options['fields']['publisher']['table'] = 'amazon_item';
  $handler->display->display_options['fields']['publisher']['field'] = 'publisher';
  $handler->display->display_options['fields']['publisher']['relationship'] = 'field_asin_asin';
  $handler->display->display_options['fields']['publisher']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['publisher']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['publisher']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['publisher']['alter']['external'] = 0;
  $handler->display->display_options['fields']['publisher']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['publisher']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['publisher']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['publisher']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['publisher']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['publisher']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['publisher']['alter']['html'] = 0;
  $handler->display->display_options['fields']['publisher']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['publisher']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['publisher']['hide_empty'] = 0;
  $handler->display->display_options['fields']['publisher']['empty_zero'] = 0;
  /* Field: Amazon: List price (formatted) */
  $handler->display->display_options['fields']['listpriceformattedprice']['id'] = 'listpriceformattedprice';
  $handler->display->display_options['fields']['listpriceformattedprice']['table'] = 'amazon_item';
  $handler->display->display_options['fields']['listpriceformattedprice']['field'] = 'listpriceformattedprice';
  $handler->display->display_options['fields']['listpriceformattedprice']['relationship'] = 'field_asin_asin';
  $handler->display->display_options['fields']['listpriceformattedprice']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['listpriceformattedprice']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['listpriceformattedprice']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['listpriceformattedprice']['alter']['external'] = 0;
  $handler->display->display_options['fields']['listpriceformattedprice']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['listpriceformattedprice']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['listpriceformattedprice']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['listpriceformattedprice']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['listpriceformattedprice']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['listpriceformattedprice']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['listpriceformattedprice']['alter']['html'] = 0;
  $handler->display->display_options['fields']['listpriceformattedprice']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['listpriceformattedprice']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['listpriceformattedprice']['hide_empty'] = 0;
  $handler->display->display_options['fields']['listpriceformattedprice']['empty_zero'] = 0;
  /* Field: Amazon: Product image */
  $handler->display->display_options['fields']['product_image']['id'] = 'product_image';
  $handler->display->display_options['fields']['product_image']['table'] = 'amazon_item_image';
  $handler->display->display_options['fields']['product_image']['field'] = 'product_image';
  $handler->display->display_options['fields']['product_image']['relationship'] = 'field_asin_asin';
  $handler->display->display_options['fields']['product_image']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['product_image']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['product_image']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['product_image']['alter']['external'] = 0;
  $handler->display->display_options['fields']['product_image']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['product_image']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['product_image']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['product_image']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['product_image']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['product_image']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['product_image']['alter']['html'] = 0;
  $handler->display->display_options['fields']['product_image']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['product_image']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['product_image']['hide_empty'] = 0;
  $handler->display->display_options['fields']['product_image']['empty_zero'] = 0;
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
    'amazon_example' => 'amazon_example',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'amazon_example_view';
  $translatables['amazon_example_view'] = array(
    t('Master'),
    t('Amazon Example View'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('ASIN from field_asin'),
    t('Nid'),
    t('Title'),
    t('ASIN'),
    t('Detail page URL'),
    t('Publisher'),
    t('List price (formatted)'),
    t('Product image'),
    t('Page'),
  );
  $export['amazon_example_view'] = $view;

  return $export;
}

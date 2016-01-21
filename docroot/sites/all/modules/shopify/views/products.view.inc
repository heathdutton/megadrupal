<?php
$view = new view();
$view->name = 'shopify_products';
$view->description = 'A View of Shopify Products';
$view->tag = 'shopify';
$view->base_table = 'shopify_products';
$view->human_name = 'Shopify Products';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Products';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'view shopify products';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['distinct'] = TRUE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['row_plugin'] = 'fields';
/* Field: Shopify Product: Product Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'shopify_products';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['element_type'] = 'h2';
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: Shopify Product: Shopify Product Image */
$handler->display->display_options['fields']['shopify_product_images']['id'] = 'shopify_product_images';
$handler->display->display_options['fields']['shopify_product_images']['table'] = 'field_data_shopify_product_images';
$handler->display->display_options['fields']['shopify_product_images']['field'] = 'shopify_product_images';
$handler->display->display_options['fields']['shopify_product_images']['label'] = '';
$handler->display->display_options['fields']['shopify_product_images']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['shopify_product_images']['click_sort_column'] = 'fid';
$handler->display->display_options['fields']['shopify_product_images']['settings'] = array(
  'image_style' => 'thumbnail',
  'image_link' => 'content',
  'field_formatter_label' => '',
);
$handler->display->display_options['fields']['shopify_product_images']['delta_limit'] = '1';
$handler->display->display_options['fields']['shopify_product_images']['delta_offset'] = '0';
/* Field: Shopify Product: Body HTML */
$handler->display->display_options['fields']['body_html']['id'] = 'body_html';
$handler->display->display_options['fields']['body_html']['table'] = 'shopify_products';
$handler->display->display_options['fields']['body_html']['field'] = 'body_html';
$handler->display->display_options['fields']['body_html']['label'] = '';
$handler->display->display_options['fields']['body_html']['alter']['max_length'] = '500';
$handler->display->display_options['fields']['body_html']['alter']['trim'] = TRUE;
$handler->display->display_options['fields']['body_html']['alter']['html'] = TRUE;
$handler->display->display_options['fields']['body_html']['element_label_colon'] = FALSE;
/* Field: Shopify Product: Add to Cart form */
$handler->display->display_options['fields']['add_to_cart_form']['id'] = 'add_to_cart_form';
$handler->display->display_options['fields']['add_to_cart_form']['table'] = 'shopify_products';
$handler->display->display_options['fields']['add_to_cart_form']['field'] = 'add_to_cart_form';
$handler->display->display_options['fields']['add_to_cart_form']['label'] = '';
$handler->display->display_options['fields']['add_to_cart_form']['element_label_colon'] = FALSE;
/* Field: Shopify Product: Shopify Product Tags */
$handler->display->display_options['fields']['shopify_product_tags']['id'] = 'shopify_product_tags';
$handler->display->display_options['fields']['shopify_product_tags']['table'] = 'field_data_shopify_product_tags';
$handler->display->display_options['fields']['shopify_product_tags']['field'] = 'shopify_product_tags';
$handler->display->display_options['fields']['shopify_product_tags']['label'] = 'Tags';
$handler->display->display_options['fields']['shopify_product_tags']['settings'] = array(
  'field_formatter_label' => '',
);
$handler->display->display_options['fields']['shopify_product_tags']['delta_offset'] = '0';
/* Field: Shopify Product: Shopify Collections */
$handler->display->display_options['fields']['shopify_product_collections']['id'] = 'shopify_product_collections';
$handler->display->display_options['fields']['shopify_product_collections']['table'] = 'field_data_shopify_product_collections';
$handler->display->display_options['fields']['shopify_product_collections']['field'] = 'shopify_product_collections';
$handler->display->display_options['fields']['shopify_product_collections']['label'] = 'Collections';
$handler->display->display_options['fields']['shopify_product_collections']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['shopify_product_collections']['settings'] = array(
  'field_formatter_label' => '',
);
$handler->display->display_options['fields']['shopify_product_collections']['delta_offset'] = '0';
/* Sort criterion: Shopify Product: Product Title */
$handler->display->display_options['sorts']['title']['id'] = 'title';
$handler->display->display_options['sorts']['title']['table'] = 'shopify_products';
$handler->display->display_options['sorts']['title']['field'] = 'title';
/* Filter criterion: Shopify Product: Variant ID */
$handler->display->display_options['filters']['variant_id']['id'] = 'variant_id';
$handler->display->display_options['filters']['variant_id']['table'] = 'shopify_products';
$handler->display->display_options['filters']['variant_id']['field'] = 'variant_id';
$handler->display->display_options['filters']['variant_id']['value']['value'] = '0';
/* Filter criterion: Hide unpublished products */
$handler->display->display_options['filters']['published_at']['id'] = 'published_at';
$handler->display->display_options['filters']['published_at']['table'] = 'shopify_products';
$handler->display->display_options['filters']['published_at']['field'] = 'published_at';
$handler->display->display_options['filters']['published_at']['ui_name'] = 'Hide unpublished products';
$handler->display->display_options['filters']['published_at']['operator'] = '>';
$handler->display->display_options['filters']['published_at']['value']['value'] = '1970-01-01 00:00:00';

/* Display: Products Page */
$handler = $view->new_display('page', 'Products Page', 'page_products');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Products';
$handler->display->display_options['path'] = 'products';

/* Display: Tags Page */
$handler = $view->new_display('page', 'Tags Page', 'page_tags');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Tags';
$handler->display->display_options['defaults']['relationships'] = FALSE;
/* Relationship: Shopify Product: Shopify Product Tags (shopify_product_tags) */
$handler->display->display_options['relationships']['shopify_product_tags_tid']['id'] = 'shopify_product_tags_tid';
$handler->display->display_options['relationships']['shopify_product_tags_tid']['table'] = 'field_data_shopify_product_tags';
$handler->display->display_options['relationships']['shopify_product_tags_tid']['field'] = 'shopify_product_tags_tid';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Shopify Product: Product Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'shopify_products';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['element_type'] = 'h2';
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: Shopify Product: Shopify Product Image */
$handler->display->display_options['fields']['shopify_product_images']['id'] = 'shopify_product_images';
$handler->display->display_options['fields']['shopify_product_images']['table'] = 'field_data_shopify_product_images';
$handler->display->display_options['fields']['shopify_product_images']['field'] = 'shopify_product_images';
$handler->display->display_options['fields']['shopify_product_images']['label'] = '';
$handler->display->display_options['fields']['shopify_product_images']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['shopify_product_images']['click_sort_column'] = 'fid';
$handler->display->display_options['fields']['shopify_product_images']['settings'] = array(
  'image_style' => 'thumbnail',
  'image_link' => 'content',
  'field_formatter_label' => '',
);
$handler->display->display_options['fields']['shopify_product_images']['delta_limit'] = '1';
$handler->display->display_options['fields']['shopify_product_images']['delta_offset'] = '0';
/* Field: Shopify Product: Body HTML */
$handler->display->display_options['fields']['body_html']['id'] = 'body_html';
$handler->display->display_options['fields']['body_html']['table'] = 'shopify_products';
$handler->display->display_options['fields']['body_html']['field'] = 'body_html';
$handler->display->display_options['fields']['body_html']['label'] = '';
$handler->display->display_options['fields']['body_html']['alter']['max_length'] = '500';
$handler->display->display_options['fields']['body_html']['alter']['trim'] = TRUE;
$handler->display->display_options['fields']['body_html']['alter']['html'] = TRUE;
$handler->display->display_options['fields']['body_html']['element_label_colon'] = FALSE;
/* Field: Shopify Product: Add to Cart form */
$handler->display->display_options['fields']['add_to_cart_form']['id'] = 'add_to_cart_form';
$handler->display->display_options['fields']['add_to_cart_form']['table'] = 'shopify_products';
$handler->display->display_options['fields']['add_to_cart_form']['field'] = 'add_to_cart_form';
$handler->display->display_options['fields']['add_to_cart_form']['label'] = '';
$handler->display->display_options['fields']['add_to_cart_form']['element_label_colon'] = FALSE;
/* Field: Shopify Product: Shopify Product Tags */
$handler->display->display_options['fields']['shopify_product_tags']['id'] = 'shopify_product_tags';
$handler->display->display_options['fields']['shopify_product_tags']['table'] = 'field_data_shopify_product_tags';
$handler->display->display_options['fields']['shopify_product_tags']['field'] = 'shopify_product_tags';
$handler->display->display_options['fields']['shopify_product_tags']['label'] = 'Tags';
$handler->display->display_options['fields']['shopify_product_tags']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['shopify_product_tags']['settings'] = array(
  'field_formatter_label' => '',
);
$handler->display->display_options['fields']['shopify_product_tags']['delta_offset'] = '0';
/* Field: Shopify Product: Shopify Collections */
$handler->display->display_options['fields']['shopify_product_collections']['id'] = 'shopify_product_collections';
$handler->display->display_options['fields']['shopify_product_collections']['table'] = 'field_data_shopify_product_collections';
$handler->display->display_options['fields']['shopify_product_collections']['field'] = 'shopify_product_collections';
$handler->display->display_options['fields']['shopify_product_collections']['label'] = 'Collections';
$handler->display->display_options['fields']['shopify_product_collections']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['shopify_product_collections']['settings'] = array(
  'field_formatter_label' => '',
);
$handler->display->display_options['fields']['shopify_product_collections']['delta_offset'] = '0';
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Shopify Product: Shopify Product Tags (shopify_product_tags) */
$handler->display->display_options['arguments']['shopify_product_tags_tid']['id'] = 'shopify_product_tags_tid';
$handler->display->display_options['arguments']['shopify_product_tags_tid']['table'] = 'field_data_shopify_product_tags';
$handler->display->display_options['arguments']['shopify_product_tags_tid']['field'] = 'shopify_product_tags_tid';
$handler->display->display_options['arguments']['shopify_product_tags_tid']['default_action'] = 'not found';
$handler->display->display_options['arguments']['shopify_product_tags_tid']['exception']['title_enable'] = TRUE;
$handler->display->display_options['arguments']['shopify_product_tags_tid']['exception']['title'] = 'Tags';
$handler->display->display_options['arguments']['shopify_product_tags_tid']['title_enable'] = TRUE;
$handler->display->display_options['arguments']['shopify_product_tags_tid']['title'] = '%1';
$handler->display->display_options['arguments']['shopify_product_tags_tid']['default_argument_type'] = 'raw';
$handler->display->display_options['arguments']['shopify_product_tags_tid']['default_argument_options']['index'] = '2';
$handler->display->display_options['arguments']['shopify_product_tags_tid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['shopify_product_tags_tid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['shopify_product_tags_tid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['shopify_product_tags_tid']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['shopify_product_tags_tid']['validate']['type'] = 'taxonomy_term';
$handler->display->display_options['arguments']['shopify_product_tags_tid']['validate_options']['vocabularies'] = array(
  'shopify_tags' => 'shopify_tags',
);
$handler->display->display_options['arguments']['shopify_product_tags_tid']['break_phrase'] = TRUE;
$handler->display->display_options['path'] = 'products/tags/%';

/* Display: Collections Page */
$handler = $view->new_display('page', 'Collections Page', 'page_collections');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Collections';
$handler->display->display_options['defaults']['relationships'] = FALSE;
/* Relationship: Shopify Product: Shopify Collections (shopify_product_collections) */
$handler->display->display_options['relationships']['shopify_product_collections_tid']['id'] = 'shopify_product_collections_tid';
$handler->display->display_options['relationships']['shopify_product_collections_tid']['table'] = 'field_data_shopify_product_collections';
$handler->display->display_options['relationships']['shopify_product_collections_tid']['field'] = 'shopify_product_collections_tid';
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Shopify Product: Shopify Collections (shopify_product_collections) */
$handler->display->display_options['arguments']['shopify_product_collections_tid']['id'] = 'shopify_product_collections_tid';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['table'] = 'field_data_shopify_product_collections';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['field'] = 'shopify_product_collections_tid';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['default_action'] = 'not found';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['title_enable'] = TRUE;
$handler->display->display_options['arguments']['shopify_product_collections_tid']['title'] = '%1';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['shopify_product_collections_tid']['validate']['type'] = 'taxonomy_term';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['validate_options']['vocabularies'] = array(
  'shopify_collections' => 'shopify_collections',
);
$handler->display->display_options['arguments']['shopify_product_collections_tid']['break_phrase'] = TRUE;
$handler->display->display_options['path'] = 'products/collections/%';

/* Display: Collections Header Attachment */
$handler = $view->new_display('attachment', 'Collections Header Attachment', 'attachment_collections');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '1';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['relationships'] = FALSE;
/* Relationship: Shopify Product: Shopify Collections (shopify_product_collections) */
$handler->display->display_options['relationships']['shopify_product_collections_tid']['id'] = 'shopify_product_collections_tid';
$handler->display->display_options['relationships']['shopify_product_collections_tid']['table'] = 'field_data_shopify_product_collections';
$handler->display->display_options['relationships']['shopify_product_collections_tid']['field'] = 'shopify_product_collections_tid';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Taxonomy term: Shopify Collection Image */
$handler->display->display_options['fields']['shopify_collection_image']['id'] = 'shopify_collection_image';
$handler->display->display_options['fields']['shopify_collection_image']['table'] = 'field_data_shopify_collection_image';
$handler->display->display_options['fields']['shopify_collection_image']['field'] = 'shopify_collection_image';
$handler->display->display_options['fields']['shopify_collection_image']['relationship'] = 'shopify_product_collections_tid';
$handler->display->display_options['fields']['shopify_collection_image']['label'] = '';
$handler->display->display_options['fields']['shopify_collection_image']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['shopify_collection_image']['click_sort_column'] = 'fid';
$handler->display->display_options['fields']['shopify_collection_image']['settings'] = array(
  'image_style' => 'medium',
  'image_link' => '',
);
/* Field: Taxonomy term: Term description */
$handler->display->display_options['fields']['description']['id'] = 'description';
$handler->display->display_options['fields']['description']['table'] = 'taxonomy_term_data';
$handler->display->display_options['fields']['description']['field'] = 'description';
$handler->display->display_options['fields']['description']['relationship'] = 'shopify_product_collections_tid';
$handler->display->display_options['fields']['description']['label'] = '';
$handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
$handler->display->display_options['defaults']['sorts'] = FALSE;
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Shopify Product: Shopify Collections (shopify_product_collections) */
$handler->display->display_options['arguments']['shopify_product_collections_tid']['id'] = 'shopify_product_collections_tid';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['table'] = 'field_data_shopify_product_collections';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['field'] = 'shopify_product_collections_tid';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['default_action'] = 'not found';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['title_enable'] = TRUE;
$handler->display->display_options['arguments']['shopify_product_collections_tid']['title'] = '%1';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['default_argument_type'] = 'taxonomy_tid';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['shopify_product_collections_tid']['break_phrase'] = TRUE;
$handler->display->display_options['displays'] = array(
  'page_2' => 'page_2',
  'default' => 0,
  'page' => 0,
  'page_1' => 0,
);

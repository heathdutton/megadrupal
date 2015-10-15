<?php
/**
 * @file
 * Default Collection menu listing view for Shopify module.
 */
$view = new view();
$view->name = 'collections_menu';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'taxonomy_term_data';
$view->human_name = 'Collections Menu';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Collections';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'view shopify products';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['row_plugin'] = 'fields';
/* Field: Taxonomy term: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['label'] = '';
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['name']['link_to_taxonomy'] = TRUE;
/* Filter criterion: Taxonomy vocabulary: Machine name */
$handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
$handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
$handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
$handler->display->display_options['filters']['machine_name']['value'] = array(
  'shopify_collections' => 'shopify_collections',
);
/* Filter criterion: Taxonomy term: Shopify Collection Published Date (shopify_collection_published_at) */
$handler->display->display_options['filters']['shopify_collection_published_at_value']['id'] = 'shopify_collection_published_at_value';
$handler->display->display_options['filters']['shopify_collection_published_at_value']['table'] = 'field_data_shopify_collection_published_at';
$handler->display->display_options['filters']['shopify_collection_published_at_value']['field'] = 'shopify_collection_published_at_value';
$handler->display->display_options['filters']['shopify_collection_published_at_value']['operator'] = '>';
$handler->display->display_options['filters']['shopify_collection_published_at_value']['value']['value'] = '0';

/* Display: Block */
$handler = $view->new_display('block', 'Block', 'block');

<?php
/**
 * @file commerce_rec.views.inc
 * Default views definitions for product recommendations
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_rec_views_default_views() {

  $views = array();

  $views['commerce_rec_personal_recommendations'] = recommender_default_view_template(
      'commerce_rec_personal_recommendations',
      'Commerce Products Recommender: personal recommendations',
      'Recommend to the current user other products this user might be interested based on the users purchasing history',
      'recommender_prediction',
      'commerce_product',
      'title',
      'current_user',
      'commerce_rec'
    );

  $view = new view;
  $view->name = 'commerce_rec_similar_products';
  $view->description = 'Display on a product page "users who have ordered this product also ordered"';
  $view->tag = 'recommendation';
  $view->base_table = 'recommender_similarity';
  $view->human_name = 'Commerce Products Recommender: similar products';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Commerce Products Recommender: similar products';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Recommender Similarity: Entity ID (Target) */
  $handler->display->display_options['relationships']['target_eid']['id'] = 'target_eid';
  $handler->display->display_options['relationships']['target_eid']['table'] = 'recommender_similarity';
  $handler->display->display_options['relationships']['target_eid']['field'] = 'target_eid';
  $handler->display->display_options['relationships']['target_eid']['required'] = 1;
  $handler->display->display_options['relationships']['target_eid']['base_entity_type'] = 'commerce_product';
  /* Relationship: Content: Referenced product */
  $handler->display->display_options['relationships']['field_product_product_id']['id'] = 'field_product_product_id';
  $handler->display->display_options['relationships']['field_product_product_id']['table'] = 'field_data_field_product';
  $handler->display->display_options['relationships']['field_product_product_id']['field'] = 'field_product_product_id';
  $handler->display->display_options['relationships']['field_product_product_id']['relationship'] = 'target_eid';
  $handler->display->display_options['relationships']['field_product_product_id']['required'] = 0;
  /* Relationship: Commerce Product: Node referencing products from field_product */
  $handler->display->display_options['relationships']['field_product']['id'] = 'field_product';
  $handler->display->display_options['relationships']['field_product']['table'] = 'commerce_product';
  $handler->display->display_options['relationships']['field_product']['field'] = 'field_product';
  $handler->display->display_options['relationships']['field_product']['relationship'] = 'field_product_product_id';
  $handler->display->display_options['relationships']['field_product']['label'] = 'Node';
  $handler->display->display_options['relationships']['field_product']['required'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_product';
  $handler->display->display_options['fields']['title']['label'] = '';
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
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Sort criterion: Recommender Similarity: Similarity score */
  $handler->display->display_options['sorts']['score']['id'] = 'score';
  $handler->display->display_options['sorts']['score']['table'] = 'recommender_similarity';
  $handler->display->display_options['sorts']['score']['field'] = 'score';
  $handler->display->display_options['sorts']['score']['order'] = 'DESC';
  /* Contextual filter: Recommender Similarity: Entity ID (Source) */
  $handler->display->display_options['arguments']['source_eid']['id'] = 'source_eid';
  $handler->display->display_options['arguments']['source_eid']['table'] = 'recommender_similarity';
  $handler->display->display_options['arguments']['source_eid']['field'] = 'source_eid';
  $handler->display->display_options['arguments']['source_eid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['source_eid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['source_eid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['source_eid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['source_eid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['source_eid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['source_eid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['source_eid']['not'] = 0;
  /* Filter criterion: Recommender Application: Application name */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'recommender_app';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['value'] = array(
      'commerce_rec' => 'commerce_rec',
  );
  /* Filter criterion: Recommender Similarity: Similarity score */
  $handler->display->display_options['filters']['score']['id'] = 'score';
  $handler->display->display_options['filters']['score']['table'] = 'recommender_similarity';
  $handler->display->display_options['filters']['score']['field'] = 'score';
  $handler->display->display_options['filters']['score']['operator'] = '>';
  $handler->display->display_options['filters']['score']['value']['value'] = '0';
  $translatables['commerce_rec_similar_products'] = array(
      t('Master'),
      t('Commerce Products Recommender: similar products'),
      t('more'),
      t('Apply'),
      t('Reset'),
      t('Sort by'),
      t('Asc'),
      t('Desc'),
      t('Recommended entity (by similarity)'),
      t('Product'),
      t('Node'),
      t('All'),
  );

  $views[$view->name] = $view;

  $view = new view;
  $view->name = 'commerce_rec_product_display';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Commerce Product Display: similar products';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Customers Also Bought';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['hide_empty'] = 0;
  $handler->display->display_options['row_options']['default_field_elements'] = 1;
  /* Relationship: Content: Referenced product */
  $handler->display->display_options['relationships']['field_product_product_id']['id'] = 'field_product_product_id';
  $handler->display->display_options['relationships']['field_product_product_id']['table'] = 'field_data_field_product';
  $handler->display->display_options['relationships']['field_product_product_id']['field'] = 'field_product_product_id';
  $handler->display->display_options['relationships']['field_product_product_id']['required'] = 0;
  /* Field: Commerce Product: Product ID */
  $handler->display->display_options['fields']['product_id']['id'] = 'product_id';
  $handler->display->display_options['fields']['product_id']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['product_id']['field'] = 'product_id';
  $handler->display->display_options['fields']['product_id']['relationship'] = 'field_product_product_id';
  $handler->display->display_options['fields']['product_id']['label'] = '';
  $handler->display->display_options['fields']['product_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['product_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['product_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['product_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['product_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['product_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['product_id']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['product_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['product_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['product_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['product_id']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['product_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['product_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['product_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['product_id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['product_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['product_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['product_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['product_id']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['product_id']['link_to_product'] = 0;
  /* Field: Global: View */
  $handler->display->display_options['fields']['view']['id'] = 'view';
  $handler->display->display_options['fields']['view']['table'] = 'views';
  $handler->display->display_options['fields']['view']['field'] = 'view';
  $handler->display->display_options['fields']['view']['label'] = '';
  $handler->display->display_options['fields']['view']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['view']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['view']['hide_empty'] = 0;
  $handler->display->display_options['fields']['view']['empty_zero'] = 0;
  $handler->display->display_options['fields']['view']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['view']['view'] = 'commerce_rec_similar_products';
  $handler->display->display_options['fields']['view']['arguments'] = '[product_id]';
  $handler->display->display_options['fields']['view']['query_aggregation'] = 0;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['nid']['not'] = 0;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['block_description'] = 'Customers Also Bought';
  $translatables['commerce_rec_product_display'] = array(
      t('Master'),
      t('Customers Also Bought'),
      t('more'),
      t('Apply'),
      t('Reset'),
      t('Sort by'),
      t('Asc'),
      t('Desc'),
      t('Product'),
      t('All'),
      t('Block'),
  );

  $views[$view->name] = $view;

  return $views;
}

<?php
/**
 * @file
 * bids_list.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_auction_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'bids_list';
  $view->description = 'Provides a page as a tab on auction display nodes that shows a list of bids on the auction. ';
  $view->tag = 'default';
  $view->base_table = 'eck_commerce_auction_bid';
  $view->human_name = 'Bids list';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Bids list';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'eck view commerce_auction_bid auction_bid entities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'rendered_entity' => 'rendered_entity',
    'bid_amount' => 'bid_amount',
    'created' => 'created',
  );
  $handler->display->display_options['style_options']['default'] = 'bid_amount';
  $handler->display->display_options['style_options']['info'] = array(
    'rendered_entity' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'bid_amount' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
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
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No bids yet';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'There are no bids for this auction yet!';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Relationship: Commerce Auction Bid: Owner User ID */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'eck_commerce_auction_bid';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = 0;
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_auction_bid_refs_node']['id'] = 'reverse_auction_bid_refs_node';
  $handler->display->display_options['relationships']['reverse_auction_bid_refs_node']['table'] = 'eck_commerce_auction_bid';
  $handler->display->display_options['relationships']['reverse_auction_bid_refs_node']['field'] = 'reverse_auction_bid_refs_node';
  $handler->display->display_options['relationships']['reverse_auction_bid_refs_node']['required'] = 0;
  /* Field: User: Rendered User */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_user';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['relationship'] = 'uid';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'User';
  $handler->display->display_options['fields']['rendered_entity']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['empty_zero'] = 0;
  $handler->display->display_options['fields']['rendered_entity']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'full';
  /* Field: Commerce Auction Bid: Bid amount */
  $handler->display->display_options['fields']['bid_amount']['id'] = 'bid_amount';
  $handler->display->display_options['fields']['bid_amount']['table'] = 'field_data_bid_amount';
  $handler->display->display_options['fields']['bid_amount']['field'] = 'bid_amount';
  $handler->display->display_options['fields']['bid_amount']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['bid_amount']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['bid_amount']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['bid_amount']['alter']['external'] = 0;
  $handler->display->display_options['fields']['bid_amount']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['bid_amount']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['bid_amount']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['bid_amount']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['bid_amount']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['bid_amount']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['bid_amount']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['bid_amount']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['bid_amount']['alter']['html'] = 0;
  $handler->display->display_options['fields']['bid_amount']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['bid_amount']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['bid_amount']['hide_empty'] = 0;
  $handler->display->display_options['fields']['bid_amount']['empty_zero'] = 0;
  $handler->display->display_options['fields']['bid_amount']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['bid_amount']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['bid_amount']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['bid_amount']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['bid_amount']['field_api_classes'] = 0;
  /* Field: Commerce Auction Bid: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'eck_commerce_auction_bid';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Time';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['created']['alter']['external'] = 0;
  $handler->display->display_options['fields']['created']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['created']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['created']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['created']['alter']['html'] = 0;
  $handler->display->display_options['fields']['created']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['created']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['created']['hide_empty'] = 0;
  $handler->display->display_options['fields']['created']['empty_zero'] = 0;
  $handler->display->display_options['fields']['created']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'reverse_auction_bid_refs_node';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['nid']['not'] = 0;
  /* Filter criterion: Commerce Auction Bid: commerce_auction_bid type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'eck_commerce_auction_bid';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'auction_bid' => 'auction_bid',
  );

  $export['bids_list'] = $view;

  return $export;
}

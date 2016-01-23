<?php
/**
 * @file
 * austrofeedr_base_publishing.views_default.inc
 */

/**
 * Implementation of hook_views_default_views().
 */
function austrofeedr_base_publishing_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'publishing_updates_list';
  $view->description = '';
  $view->tag = 'austrofeedr';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'entity_id' => 'entity_id',
    'entity_id_1' => 'entity_id_1',
    'entity_id_2' => 'entity_id_2',
    'entity_id_3' => 'entity_id_3',
    'title' => 'title',
    'changed' => 'changed',
    'timestamp' => 'timestamp',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'entity_id' => array(
      'sortable' => 0,
      'align' => '',
      'separator' => '',
    ),
    'entity_id_1' => array(
      'sortable' => 0,
      'align' => '',
      'separator' => '',
    ),
    'entity_id_2' => array(
      'sortable' => 0,
      'align' => '',
      'separator' => '',
    ),
    'entity_id_3' => array(
      'sortable' => 0,
      'align' => '',
      'separator' => '',
    ),
    'title' => array(
      'sortable' => 0,
      'align' => '',
      'separator' => '',
    ),
    'changed' => array(
      'sortable' => 0,
      'align' => '',
      'separator' => '',
    ),
    'timestamp' => array(
      'sortable' => 0,
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['created']['alter']['html'] = 0;
  $handler->display->display_options['fields']['created']['hide_empty'] = 0;
  $handler->display->display_options['fields']['created']['empty_zero'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['tid_1']['id'] = 'tid_1';
  $handler->display->display_options['fields']['tid_1']['table'] = 'taxonomy_index';
  $handler->display->display_options['fields']['tid_1']['field'] = 'tid';
  $handler->display->display_options['fields']['tid_1']['label'] = 'tags';
  $handler->display->display_options['fields']['tid_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['tid_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['tid_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['tid_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['tid_1']['empty_zero'] = 0;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['entity_id_1']['id'] = 'entity_id_1';
  $handler->display->display_options['fields']['entity_id_1']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['entity_id_1']['field'] = 'body';
  $handler->display->display_options['fields']['entity_id_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['entity_id_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['entity_id_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['entity_id_1']['alter']['max_length'] = '255';
  $handler->display->display_options['fields']['entity_id_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['entity_id_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['entity_id_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['entity_id_1']['alter']['trim'] = 1;
  $handler->display->display_options['fields']['entity_id_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['entity_id_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['entity_id_1']['empty_zero'] = 0;
  /* Field: Feeds item: Item URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'feeds_item';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['url']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['url']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['url']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['url']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['url']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['url']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['url']['alter']['html'] = 0;
  $handler->display->display_options['fields']['url']['hide_empty'] = 0;
  $handler->display->display_options['fields']['url']['empty_zero'] = 0;
  $handler->display->display_options['fields']['url']['display_as_link'] = 1;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'update' => 'update',
  );

  /* Display: Updates details */
  $handler = $view->new_display('page', 'Updates details', 'updates_details');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    2 => '2',
  );
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['created']['alter']['html'] = 0;
  $handler->display->display_options['fields']['created']['hide_empty'] = 0;
  $handler->display->display_options['fields']['created']['empty_zero'] = 0;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'taxonomy_index';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['label'] = 'incoming tags';
  $handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['tid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['tid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['tid']['empty_zero'] = 0;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['tid_1']['id'] = 'tid_1';
  $handler->display->display_options['fields']['tid_1']['table'] = 'taxonomy_index';
  $handler->display->display_options['fields']['tid_1']['field'] = 'tid';
  $handler->display->display_options['fields']['tid_1']['label'] = 'tags';
  $handler->display->display_options['fields']['tid_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['tid_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['tid_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['tid_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['tid_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['tid_1']['empty_zero'] = 0;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['entity_id_1']['id'] = 'entity_id_1';
  $handler->display->display_options['fields']['entity_id_1']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['entity_id_1']['field'] = 'body';
  $handler->display->display_options['fields']['entity_id_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['entity_id_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['entity_id_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['entity_id_1']['alter']['max_length'] = '255';
  $handler->display->display_options['fields']['entity_id_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['entity_id_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['entity_id_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['entity_id_1']['alter']['trim'] = 1;
  $handler->display->display_options['fields']['entity_id_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['entity_id_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['entity_id_1']['empty_zero'] = 0;
  /* Field: Feeds item: Item URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'feeds_item';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['url']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['url']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['url']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['url']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['url']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['url']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['url']['alter']['html'] = 0;
  $handler->display->display_options['fields']['url']['hide_empty'] = 0;
  $handler->display->display_options['fields']['url']['empty_zero'] = 0;
  $handler->display->display_options['fields']['url']['display_as_link'] = 1;
  /* Field: Content: Author */
  $handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['table'] = 'field_data_field_update_author';
  $handler->display->display_options['fields']['entity_id']['field'] = 'field_update_author';
  $handler->display->display_options['fields']['entity_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['entity_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['entity_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['entity_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['entity_id']['empty_zero'] = 0;
  /* Field: Content: Item URL */
  $handler->display->display_options['fields']['entity_id_2']['id'] = 'entity_id_2';
  $handler->display->display_options['fields']['entity_id_2']['table'] = 'field_data_field_update_url';
  $handler->display->display_options['fields']['entity_id_2']['field'] = 'field_update_url';
  /* Field: Feeds item: Import date */
  $handler->display->display_options['fields']['imported']['id'] = 'imported';
  $handler->display->display_options['fields']['imported']['table'] = 'feeds_item';
  $handler->display->display_options['fields']['imported']['field'] = 'imported';
  $handler->display->display_options['fields']['imported']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['imported']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['imported']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['imported']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['imported']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['imported']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['imported']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['imported']['alter']['html'] = 0;
  $handler->display->display_options['fields']['imported']['hide_empty'] = 0;
  $handler->display->display_options['fields']['imported']['empty_zero'] = 0;
  /* Field: Feeds item: Item GUID */
  $handler->display->display_options['fields']['guid']['id'] = 'guid';
  $handler->display->display_options['fields']['guid']['table'] = 'feeds_item';
  $handler->display->display_options['fields']['guid']['field'] = 'guid';
  $handler->display->display_options['fields']['guid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['guid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['guid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['guid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['guid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['guid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['guid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['guid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['guid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['guid']['empty_zero'] = 0;
  /* Field: Feeds item: Owner feed nid */
  $handler->display->display_options['fields']['feed_nid']['id'] = 'feed_nid';
  $handler->display->display_options['fields']['feed_nid']['table'] = 'feeds_item';
  $handler->display->display_options['fields']['feed_nid']['field'] = 'feed_nid';
  $handler->display->display_options['fields']['feed_nid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['feed_nid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['feed_nid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['feed_nid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['feed_nid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['feed_nid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['feed_nid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['feed_nid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['feed_nid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['feed_nid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['feed_nid']['format_plural'] = 0;
  $handler->display->display_options['path'] = 'updates_details';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Detailed Updates';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';

  /* Display: Updates overview */
  $handler = $view->new_display('page', 'Updates overview', 'updates_overview');
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['links'] = 1;
  $handler->display->display_options['row_options']['comments'] = 0;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = 'updates_overview';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Updates';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $export['publishing_updates_list'] = $view;

  return $export;
}

<?php
/**
 * @file
 * cta_button.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function registration_views_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'registration_numbers';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Registration numbers';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Registration numbers';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Relationship: Registration info: Nid */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'registration_node';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  $handler->display->display_options['relationships']['nid']['required'] = 1;
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
  /* Field: Registration info: Capacity */
  $handler->display->display_options['fields']['capacity']['id'] = 'capacity';
  $handler->display->display_options['fields']['capacity']['table'] = 'registration_node';
  $handler->display->display_options['fields']['capacity']['field'] = 'capacity';
  $handler->display->display_options['fields']['capacity']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['capacity']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['capacity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['capacity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['capacity']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['capacity']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['capacity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['capacity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['capacity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['capacity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['capacity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['capacity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['capacity']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['capacity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['capacity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['capacity']['empty_zero'] = 0;
  $handler->display->display_options['fields']['capacity']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['capacity']['format_plural'] = 0;
  /* Field: Registration info: Available */
  $handler->display->display_options['fields']['available']['id'] = 'available';
  $handler->display->display_options['fields']['available']['table'] = 'registration_node';
  $handler->display->display_options['fields']['available']['field'] = 'available';
  $handler->display->display_options['fields']['available']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['available']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['available']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['available']['alter']['external'] = 0;
  $handler->display->display_options['fields']['available']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['available']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['available']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['available']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['available']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['available']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['available']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['available']['alter']['html'] = 0;
  $handler->display->display_options['fields']['available']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['available']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['available']['hide_empty'] = 0;
  $handler->display->display_options['fields']['available']['empty_zero'] = 0;
  $handler->display->display_options['fields']['available']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['available']['format_plural'] = 0;
  /* Field: Registration info: Occupied */
  $handler->display->display_options['fields']['occupied']['id'] = 'occupied';
  $handler->display->display_options['fields']['occupied']['table'] = 'registration_node';
  $handler->display->display_options['fields']['occupied']['field'] = 'occupied';
  $handler->display->display_options['fields']['occupied']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['occupied']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['occupied']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['occupied']['alter']['external'] = 0;
  $handler->display->display_options['fields']['occupied']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['occupied']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['occupied']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['occupied']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['occupied']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['occupied']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['occupied']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['occupied']['alter']['html'] = 0;
  $handler->display->display_options['fields']['occupied']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['occupied']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['occupied']['hide_empty'] = 0;
  $handler->display->display_options['fields']['occupied']['empty_zero'] = 0;
  $handler->display->display_options['fields']['occupied']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['occupied']['format_plural'] = 0;
  /* Sort criterion: Registration info: Occupied */
  $handler->display->display_options['sorts']['occupied']['id'] = 'occupied';
  $handler->display->display_options['sorts']['occupied']['table'] = 'registration_node';
  $handler->display->display_options['sorts']['occupied']['field'] = 'occupied';
  $handler->display->display_options['sorts']['occupied']['relationship'] = 'nid';
  $handler->display->display_options['sorts']['occupied']['order'] = 'DESC';
  /* Sort criterion: Registration info: Available */
  $handler->display->display_options['sorts']['available']['id'] = 'available';
  $handler->display->display_options['sorts']['available']['table'] = 'registration_node';
  $handler->display->display_options['sorts']['available']['field'] = 'available';
  $handler->display->display_options['sorts']['available']['relationship'] = 'nid';
  $handler->display->display_options['sorts']['available']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'cursus' => 'cursus',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'registration-numbers';
  $translatables['registration_numbers'] = array(
    t('Master'),
    t('Registration numbers'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('Node'),
    t('Capacity'),
    t('.'),
    t(','),
    t('Available'),
    t('Occupied'),
    t('Page'),
  );

  $export['registration_numbers'] = $view;
  return $export;
}
<?php
/**
 * @file
 * nm_core.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function nm_core_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'nm_related_content';
  $view->description = 'Displays related content';
  $view->tag = 'nodemaker, core';
  $view->base_table = 'node';
  $view->human_name = 'Related Content';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Related Content';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['relationship'] = 'field_nm_related_content_target_id';
  $handler->display->display_options['row_options']['links'] = 0;
  $handler->display->display_options['row_options']['comments'] = 0;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_nm_related_content_target_id']['id'] = 'field_nm_related_content_target_id';
  $handler->display->display_options['relationships']['field_nm_related_content_target_id']['table'] = 'field_data_field_nm_related_content';
  $handler->display->display_options['relationships']['field_nm_related_content_target_id']['field'] = 'field_nm_related_content_target_id';
  $handler->display->display_options['relationships']['field_nm_related_content_target_id']['required'] = 1;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_nm_related_content_target_id';
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
  /* Field: Content: Headline Image */
  $handler->display->display_options['fields']['field_nm_headline_image']['id'] = 'field_nm_headline_image';
  $handler->display->display_options['fields']['field_nm_headline_image']['table'] = 'field_data_field_nm_headline_image';
  $handler->display->display_options['fields']['field_nm_headline_image']['field'] = 'field_nm_headline_image';
  $handler->display->display_options['fields']['field_nm_headline_image']['relationship'] = 'field_nm_related_content_target_id';
  $handler->display->display_options['fields']['field_nm_headline_image']['label'] = '';
  $handler->display->display_options['fields']['field_nm_headline_image']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_nm_headline_image']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_nm_headline_image']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_nm_headline_image']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_nm_headline_image']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_nm_headline_image']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_nm_headline_image']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_nm_headline_image']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_nm_headline_image']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_nm_headline_image']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_nm_headline_image']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_nm_headline_image']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_nm_headline_image']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_nm_headline_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_nm_headline_image']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_nm_headline_image']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_nm_headline_image']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_nm_headline_image']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_nm_headline_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_nm_headline_image']['settings'] = array(
    'image_style' => 'thumbnail',
    'image_link' => 'content',
  );
  $handler->display->display_options['fields']['field_nm_headline_image']['field_api_classes'] = 0;
  /* Sort criterion: Content: Related Content (field_nm_related_content:delta) */
  $handler->display->display_options['sorts']['delta']['id'] = 'delta';
  $handler->display->display_options['sorts']['delta']['table'] = 'field_data_field_nm_related_content';
  $handler->display->display_options['sorts']['delta']['field'] = 'delta';
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
  $handler->display->display_options['arguments']['nid']['specify_validation'] = 1;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['access'] = 0;
  $handler->display->display_options['arguments']['nid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['nid']['not'] = 0;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'field_nm_related_content_target_id';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $export['nm_related_content'] = $view;

  return $export;
}

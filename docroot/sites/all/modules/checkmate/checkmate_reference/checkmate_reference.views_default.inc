<?php
/**
 * @file
 * The default status views.
 */

/**
 * Implements hook_views_default_views().
 */
function checkmate_reference_views_default_views() {
  $view = new view;
  $view->name = 'checkmate_reference';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'checkmate_item_status';
  $view->human_name = 'Checkmate reference';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Checkmate item status: Date changed */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'checkmate_item_status';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['external'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['html'] = 0;
  $handler->display->display_options['fields']['changed']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['changed']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['changed']['hide_empty'] = 0;
  $handler->display->display_options['fields']['changed']['empty_zero'] = 0;
  $handler->display->display_options['fields']['changed']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  /* Field: Checkmate item status: Status */
  $handler->display->display_options['fields']['checkmate_status']['id'] = 'checkmate_status';
  $handler->display->display_options['fields']['checkmate_status']['table'] = 'field_data_checkmate_status';
  $handler->display->display_options['fields']['checkmate_status']['field'] = 'checkmate_status';
  $handler->display->display_options['fields']['checkmate_status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['checkmate_status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['checkmate_status']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['checkmate_status']['alter']['external'] = 0;
  $handler->display->display_options['fields']['checkmate_status']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['checkmate_status']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['checkmate_status']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['checkmate_status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['checkmate_status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['checkmate_status']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['checkmate_status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['checkmate_status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['checkmate_status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['checkmate_status']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['checkmate_status']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['checkmate_status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['checkmate_status']['empty_zero'] = 0;
  $handler->display->display_options['fields']['checkmate_status']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['checkmate_status']['field_api_classes'] = 0;
  /* Contextual filter: Checkmate item status: Cid */
  $handler->display->display_options['arguments']['cid']['id'] = 'cid';
  $handler->display->display_options['arguments']['cid']['table'] = 'checkmate_item_status';
  $handler->display->display_options['arguments']['cid']['field'] = 'cid';
  $handler->display->display_options['arguments']['cid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['cid']['title_enable'] = 1;
  $handler->display->display_options['arguments']['cid']['title'] = '%1';
  $handler->display->display_options['arguments']['cid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['cid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['cid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['cid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['cid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['cid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['cid']['not'] = 0;
  /* Contextual filter: Checkmate item status: Checkmate_entity_type */
  $handler->display->display_options['arguments']['checkmate_entity_type']['id'] = 'checkmate_entity_type';
  $handler->display->display_options['arguments']['checkmate_entity_type']['table'] = 'checkmate_item_status';
  $handler->display->display_options['arguments']['checkmate_entity_type']['field'] = 'checkmate_entity_type';
  $handler->display->display_options['arguments']['checkmate_entity_type']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['checkmate_entity_type']['title_enable'] = 1;
  $handler->display->display_options['arguments']['checkmate_entity_type']['title'] = '%2';
  $handler->display->display_options['arguments']['checkmate_entity_type']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['checkmate_entity_type']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['checkmate_entity_type']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['checkmate_entity_type']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['checkmate_entity_type']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['checkmate_entity_type']['glossary'] = 0;
  $handler->display->display_options['arguments']['checkmate_entity_type']['limit'] = '0';
  $handler->display->display_options['arguments']['checkmate_entity_type']['transform_dash'] = 0;
  $handler->display->display_options['arguments']['checkmate_entity_type']['break_phrase'] = 0;
  /* Contextual filter: Checkmate item status: Checkmate_bundle */
  $handler->display->display_options['arguments']['checkmate_bundle']['id'] = 'checkmate_bundle';
  $handler->display->display_options['arguments']['checkmate_bundle']['table'] = 'checkmate_item_status';
  $handler->display->display_options['arguments']['checkmate_bundle']['field'] = 'checkmate_bundle';
  $handler->display->display_options['arguments']['checkmate_bundle']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['checkmate_bundle']['title_enable'] = 1;
  $handler->display->display_options['arguments']['checkmate_bundle']['title'] = '%3';
  $handler->display->display_options['arguments']['checkmate_bundle']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['checkmate_bundle']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['checkmate_bundle']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['checkmate_bundle']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['checkmate_bundle']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['checkmate_bundle']['glossary'] = 0;
  $handler->display->display_options['arguments']['checkmate_bundle']['limit'] = '0';
  $handler->display->display_options['arguments']['checkmate_bundle']['transform_dash'] = 0;
  $handler->display->display_options['arguments']['checkmate_bundle']['break_phrase'] = 0;
  /* Contextual filter: Checkmate item status: Checkmate_entity_id */
  $handler->display->display_options['arguments']['checkmate_entity_id']['id'] = 'checkmate_entity_id';
  $handler->display->display_options['arguments']['checkmate_entity_id']['table'] = 'checkmate_item_status';
  $handler->display->display_options['arguments']['checkmate_entity_id']['field'] = 'checkmate_entity_id';
  $handler->display->display_options['arguments']['checkmate_entity_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['checkmate_entity_id']['title_enable'] = 1;
  $handler->display->display_options['arguments']['checkmate_entity_id']['title'] = '%4';
  $handler->display->display_options['arguments']['checkmate_entity_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['checkmate_entity_id']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['checkmate_entity_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['checkmate_entity_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['checkmate_entity_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['checkmate_entity_id']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['checkmate_entity_id']['not'] = 0;
  $views[$view->name] = $view;
  return $views;
}

<?php
/**
 * @file
 * contextual_help.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function contextual_help_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'contextual_help';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Contextual help';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Contextual help';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = '<span id="no-help-message">No help defined for this page yet.</span>';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Context path */
  $handler->display->display_options['fields']['field_context_path']['id'] = 'field_context_path';
  $handler->display->display_options['fields']['field_context_path']['table'] = 'field_data_field_context_path';
  $handler->display->display_options['fields']['field_context_path']['field'] = 'field_context_path';
  $handler->display->display_options['fields']['field_context_path']['label'] = '';
  $handler->display->display_options['fields']['field_context_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_context_path']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_context_path']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_context_path']['type'] = 'link_plain';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['element_default_classes'] = FALSE;
  /* Contextual filter: Content: Context path (field_context_path:url) */
  $handler->display->display_options['arguments']['field_context_path_url']['id'] = 'field_context_path_url';
  $handler->display->display_options['arguments']['field_context_path_url']['table'] = 'field_data_field_context_path';
  $handler->display->display_options['arguments']['field_context_path_url']['field'] = 'field_context_path_url';
  $handler->display->display_options['arguments']['field_context_path_url']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_context_path_url']['default_argument_type'] = 'php';
  $handler->display->display_options['arguments']['field_context_path_url']['default_argument_options']['code'] = '$path = request_path();
if ($path == \'\') {
  return \'<front>\';
}
return $path;';
  $handler->display->display_options['arguments']['field_context_path_url']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_context_path_url']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_context_path_url']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_context_path_url']['limit'] = '0';
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
    'contextual_help' => 'contextual_help',
  );

  /* Display: Modal links block */
  $handler = $view->new_display('block', 'Modal links block', 'modal_links_block');
  $handler->display->display_options['block_description'] = 'Contextual help:: Modal links';
  $export['contextual_help'] = $view;

  return $export;
}

<?php
/**
 * @file
 * mnn_inventory_search.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function mnn_inventory_search_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'mnn_inventory_seach';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'MNN: Inventory Seach';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Reservations Inventory: Seach';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Filter';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = 0;
  $handler->display->display_options['exposed_form']['options']['autosubmit_hide'] = 1;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = '<div class="messages warning">No items in the inventory match this criteria</div>';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Field: Content: Inventory Number */
  $handler->display->display_options['fields']['field_inventory_number']['id'] = 'field_inventory_number';
  $handler->display->display_options['fields']['field_inventory_number']['table'] = 'field_data_field_inventory_number';
  $handler->display->display_options['fields']['field_inventory_number']['field'] = 'field_inventory_number';
  $handler->display->display_options['fields']['field_inventory_number']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_inventory_number']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_inventory_number']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_inventory_number']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_inventory_number']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_inventory_number']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_inventory_number']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_inventory_number']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_inventory_number']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_inventory_number']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_inventory_number']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_inventory_number']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_inventory_number']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_inventory_number']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_inventory_number']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_inventory_number']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_inventory_number']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_inventory_number']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_inventory_number']['field_api_classes'] = 0;
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
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['type']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['type']['alter']['external'] = 0;
  $handler->display->display_options['fields']['type']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['type']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['type']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['type']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['type']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['type']['alter']['html'] = 0;
  $handler->display->display_options['fields']['type']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['type']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['type']['hide_empty'] = 0;
  $handler->display->display_options['fields']['type']['empty_zero'] = 0;
  $handler->display->display_options['fields']['type']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['type']['link_to_node'] = 0;
  $handler->display->display_options['fields']['type']['machine_name'] = 0;
  /* Field: Content: Inventory */
  $handler->display->display_options['fields']['field_reservations_inventory']['id'] = 'field_reservations_inventory';
  $handler->display->display_options['fields']['field_reservations_inventory']['table'] = 'field_data_field_reservations_inventory';
  $handler->display->display_options['fields']['field_reservations_inventory']['field'] = 'field_reservations_inventory';
  $handler->display->display_options['fields']['field_reservations_inventory']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_reservations_inventory']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_reservations_inventory']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_reservations_inventory']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_reservations_inventory']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_reservations_inventory']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_reservations_inventory']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_reservations_inventory']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_reservations_inventory']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_reservations_inventory']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_reservations_inventory']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_reservations_inventory']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_reservations_inventory']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_reservations_inventory']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_reservations_inventory']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_reservations_inventory']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_inventory']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_reservations_inventory']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_reservations_inventory']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_reservations_inventory']['field_api_classes'] = 0;
  /* Field: Reservations: Bucket/Resource Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'reservations_node_type';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Reservable';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['status']['alter']['external'] = 0;
  $handler->display->display_options['fields']['status']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['status']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['status']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['status']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['status']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['status']['empty_zero'] = 0;
  $handler->display->display_options['fields']['status']['hide_alter_empty'] = 1;
  /* Field: Reservations: Default booking availability -- resource/bucket */
  $handler->display->display_options['fields']['default_availability']['id'] = 'default_availability';
  $handler->display->display_options['fields']['default_availability']['table'] = 'reservations_reservation_item_node';
  $handler->display->display_options['fields']['default_availability']['field'] = 'default_availability';
  $handler->display->display_options['fields']['default_availability']['label'] = 'Availability';
  $handler->display->display_options['fields']['default_availability']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['default_availability']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['default_availability']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['default_availability']['alter']['external'] = 0;
  $handler->display->display_options['fields']['default_availability']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['default_availability']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['default_availability']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['default_availability']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['default_availability']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['default_availability']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['default_availability']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['default_availability']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['default_availability']['alter']['html'] = 0;
  $handler->display->display_options['fields']['default_availability']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['default_availability']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['default_availability']['hide_empty'] = 0;
  $handler->display->display_options['fields']['default_availability']['empty_zero'] = 0;
  $handler->display->display_options['fields']['default_availability']['hide_alter_empty'] = 1;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR',
  );
  /* Filter criterion: Content: Inventory Number (field_inventory_number) */
  $handler->display->display_options['filters']['field_inventory_number_value']['id'] = 'field_inventory_number_value';
  $handler->display->display_options['filters']['field_inventory_number_value']['table'] = 'field_data_field_inventory_number';
  $handler->display->display_options['filters']['field_inventory_number_value']['field'] = 'field_inventory_number_value';
  $handler->display->display_options['filters']['field_inventory_number_value']['group'] = 1;
  $handler->display->display_options['filters']['field_inventory_number_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_inventory_number_value']['expose']['operator_id'] = 'field_inventory_number_value_op';
  $handler->display->display_options['filters']['field_inventory_number_value']['expose']['label'] = 'Inventory Number';
  $handler->display->display_options['filters']['field_inventory_number_value']['expose']['operator'] = 'field_inventory_number_value_op';
  $handler->display->display_options['filters']['field_inventory_number_value']['expose']['identifier'] = 'field_inventory_number_value';
  $handler->display->display_options['filters']['field_inventory_number_value']['expose']['required'] = 0;
  $handler->display->display_options['filters']['field_inventory_number_value']['expose']['multiple'] = FALSE;
  /* Filter criterion: Content: Inventory (field_reservations_inventory) */
  $handler->display->display_options['filters']['field_reservations_inventory_tid']['id'] = 'field_reservations_inventory_tid';
  $handler->display->display_options['filters']['field_reservations_inventory_tid']['table'] = 'field_data_field_reservations_inventory';
  $handler->display->display_options['filters']['field_reservations_inventory_tid']['field'] = 'field_reservations_inventory_tid';
  $handler->display->display_options['filters']['field_reservations_inventory_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_reservations_inventory_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_reservations_inventory_tid']['expose']['operator_id'] = 'field_reservations_inventory_tid_op';
  $handler->display->display_options['filters']['field_reservations_inventory_tid']['expose']['label'] = 'Inventory';
  $handler->display->display_options['filters']['field_reservations_inventory_tid']['expose']['operator'] = 'field_reservations_inventory_tid_op';
  $handler->display->display_options['filters']['field_reservations_inventory_tid']['expose']['identifier'] = 'field_reservations_inventory_tid';
  $handler->display->display_options['filters']['field_reservations_inventory_tid']['expose']['multiple'] = 1;
  $handler->display->display_options['filters']['field_reservations_inventory_tid']['expose']['reduce'] = 0;
  $handler->display->display_options['filters']['field_reservations_inventory_tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['filters']['field_reservations_inventory_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_reservations_inventory_tid']['vocabulary'] = 'reservations_inventory';
  $handler->display->display_options['filters']['field_reservations_inventory_tid']['error_message'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['required'] = 0;
  $handler->display->display_options['filters']['title']['expose']['multiple'] = FALSE;
  /* Filter criterion: Reservations: Reservations type */
  $handler->display->display_options['filters']['type_setting']['id'] = 'type_setting';
  $handler->display->display_options['filters']['type_setting']['table'] = 'reservations_node_type';
  $handler->display->display_options['filters']['type_setting']['field'] = 'type_setting';
  $handler->display->display_options['filters']['type_setting']['value'] = array(
    'bucket' => 'bucket',
    'resource' => 'resource',
  );
  $handler->display->display_options['filters']['type_setting']['group'] = 2;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'equipment_non_reservable_' => 'equipment_non_reservable_',
    'facility_non_reservable_' => 'facility_non_reservable_',
  );
  $handler->display->display_options['filters']['type']['group'] = 2;
  /* Filter criterion: Reservations: Sub type -- resource/bucket */
  $handler->display->display_options['filters']['sub_type']['id'] = 'sub_type';
  $handler->display->display_options['filters']['sub_type']['table'] = 'reservations_reservation_item_node';
  $handler->display->display_options['filters']['sub_type']['field'] = 'sub_type';
  $handler->display->display_options['filters']['sub_type']['operator'] = 'not in';
  $handler->display->display_options['filters']['sub_type']['value'] = array(
    2 => '2',
  );
  /* Filter criterion: Content: Legacy Inventory ID (field_invenroty_id) */
  $handler->display->display_options['filters']['field_invenroty_id_value']['id'] = 'field_invenroty_id_value';
  $handler->display->display_options['filters']['field_invenroty_id_value']['table'] = 'field_data_field_invenroty_id';
  $handler->display->display_options['filters']['field_invenroty_id_value']['field'] = 'field_invenroty_id_value';
  $handler->display->display_options['filters']['field_invenroty_id_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_invenroty_id_value']['expose']['operator_id'] = 'field_invenroty_id_value_op';
  $handler->display->display_options['filters']['field_invenroty_id_value']['expose']['label'] = 'Legacy Inventory ID';
  $handler->display->display_options['filters']['field_invenroty_id_value']['expose']['operator'] = 'field_invenroty_id_value_op';
  $handler->display->display_options['filters']['field_invenroty_id_value']['expose']['identifier'] = 'field_invenroty_id_value';
  $handler->display->display_options['filters']['field_invenroty_id_value']['expose']['multiple'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/inventory/manage/search';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Inventory';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $export['mnn_inventory_seach'] = $view;

  return $export;
}

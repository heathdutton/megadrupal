<?php

/**
 * @file
 * Providing extra functionality for the things_ids UI via views.
 */


/**
 * Implements hook_views_data_alter()
 */
function things_ids_views_data_alter(&$data) { 

  $data['things_ids']['link_things_ids'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the things_ids.'),
      'handler' => 'things_ids_handler_link_field',
    ),
  );
  $data['things_ids']['edit_things_ids'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the things_ids.'),
      'handler' => 'things_ids_handler_edit_link_field',
    ),
  );
  $data['things_ids']['delete_things_ids'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the things_ids.'),
      'handler' => 'things_ids_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows things_ids/things_ids/%things_ids_id/op
  $data['things_ids']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this things_ids.'),
      'handler' => 'things_ids_handler_things_ids_operations_field',
    ),
  );
}


/**
 * Implements hook_views_default_views().
 */
function things_ids_views_default_views() {

  $views = array();

  $view = new view;
  $view->name = 'things_ids';
  $view->description = 'A list of all Things Identifiers';
  $view->tag = 'Things Identifiers';
  $view->base_table = 'things_ids';
  $view->human_name = 'Things Identifiers';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Things Identifiers';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create any Things Identifier type';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'token' => 'token',
    'things_ids_id' => 'things_ids_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(

    'token' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'things_ids_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty ';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'No Things Identifiers have been created yet';
  /* Field: things_ids: things_ids ID */
  $handler->display->display_options['fields']['things_ids_id']['id'] = 'things_ids_id';
  $handler->display->display_options['fields']['things_ids_id']['table'] = 'things_ids';
  $handler->display->display_options['fields']['things_ids_id']['field'] = 'things_ids_id';
  $handler->display->display_options['fields']['things_ids_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['things_ids_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['things_ids_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['things_ids_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['things_ids_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['things_ids_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['things_ids_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['things_ids_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['things_ids_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['things_ids_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['things_ids_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['things_ids_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['things_ids_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['things_ids_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['things_ids_id']['empty_zero'] = 0;
  /* Field: things_ids: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'things_ids';
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
  $handler->display->display_options['fields']['type']['machine_name'] = 0;

  /* Field: things_ids: Token  */
  $handler->display->display_options['fields']['token']['id'] = 'token';
  $handler->display->display_options['fields']['token']['table'] = 'things_ids';
  $handler->display->display_options['fields']['token']['field'] = 'token';
  $handler->display->display_options['fields']['token']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['token']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['token']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['token']['alter']['external'] = 0;
  $handler->display->display_options['fields']['token']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['token']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['token']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['token']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['token']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['token']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['token']['alter']['html'] = 0;
  $handler->display->display_options['fields']['token']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['token']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['token']['hide_empty'] = 0;
  $handler->display->display_options['fields']['token']['empty_zero'] = 0;
  /* Field: things_ids: Link */
  $handler->display->display_options['fields']['link_things_ids']['id'] = 'link_things_ids';
  $handler->display->display_options['fields']['link_things_ids']['table'] = 'things_ids';
  $handler->display->display_options['fields']['link_things_ids']['field'] = 'link_things_ids';
  $handler->display->display_options['fields']['link_things_ids']['label'] = 'View';
  $handler->display->display_options['fields']['link_things_ids']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['link_things_ids']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['link_things_ids']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['link_things_ids']['alter']['external'] = 0;
  $handler->display->display_options['fields']['link_things_ids']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['link_things_ids']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['link_things_ids']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['link_things_ids']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['link_things_ids']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['link_things_ids']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['link_things_ids']['alter']['html'] = 0;
  $handler->display->display_options['fields']['link_things_ids']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['link_things_ids']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['link_things_ids']['hide_empty'] = 0;
  $handler->display->display_options['fields']['link_things_ids']['empty_zero'] = 0;
  /* Field: things_ids: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'things_ids';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['external'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['html'] = 0;
  $handler->display->display_options['fields']['operations']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['operations']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['operations']['hide_empty'] = 0;
  $handler->display->display_options['fields']['operations']['empty_zero'] = 0;
  
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'things_ids_admin_page');
  $handler->display->display_options['path'] = 'admin/content/things_ids/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'Things Identifiers';
  $handler->display->display_options['tab_options']['description'] = 'Manage Things Identifiers';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['things_ids'] = array(
    t('Master'),
    t('Things Identifiers'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('Empty '),
    t('No Things Identifiers have been created yet'),
    t('things_ids ID'),
    t('.'),
    t(','),
    t('Token'),
    t('View'),
    t('Operations links'),
    t('Page'),
  );
  $views['things_ids_view'] = $view;

  return $views;
}

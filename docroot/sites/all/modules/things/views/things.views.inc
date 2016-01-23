<?php

/**
 * @file
 * Providing extra functionality for the Things UI via views.
 */


/**
 * Implements hook_views_data()
 */
function things_views_data_alter(&$data) { 
  $data['things']['link_things'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the things.'),
      'handler' => 'things_handler_link_field',
    ),
  );
  $data['things']['edit_things'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the things.'),
      'handler' => 'things_handler_edit_link_field',
    ),
  );
  $data['things']['delete_things'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the things.'),
      'handler' => 'things_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows things/things/%things_id/op
  $data['things']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this things.'),
      'handler' => 'things_handler_things_operations_field',
    ),
  );
}


/**
 * Implements hook_views_default_views().
 */
function things_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'things';
  $view->description = 'A list of all things';
  $view->tag = 'things';
  $view->base_table = 'things';
  $view->human_name = 'Things';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Things';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create any things type';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'things_id' => 'things_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'things_id' => array(
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
  $handler->display->display_options['empty']['area']['content'] = 'No things have been created yet';
  /* Field: Things: Things ID */
  $handler->display->display_options['fields']['things_id']['id'] = 'things_id';
  $handler->display->display_options['fields']['things_id']['table'] = 'things';
  $handler->display->display_options['fields']['things_id']['field'] = 'things_id';
  $handler->display->display_options['fields']['things_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['things_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['things_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['things_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['things_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['things_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['things_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['things_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['things_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['things_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['things_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['things_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['things_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['things_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['things_id']['empty_zero'] = 0;
  /* Field: Things: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'things';
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
  /* Field: Things: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'things';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  /* Field: Things: Link */
  $handler->display->display_options['fields']['link_things']['id'] = 'link_things';
  $handler->display->display_options['fields']['link_things']['table'] = 'things';
  $handler->display->display_options['fields']['link_things']['field'] = 'link_things';
  $handler->display->display_options['fields']['link_things']['label'] = 'View';
  $handler->display->display_options['fields']['link_things']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['link_things']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['link_things']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['link_things']['alter']['external'] = 0;
  $handler->display->display_options['fields']['link_things']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['link_things']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['link_things']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['link_things']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['link_things']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['link_things']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['link_things']['alter']['html'] = 0;
  $handler->display->display_options['fields']['link_things']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['link_things']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['link_things']['hide_empty'] = 0;
  $handler->display->display_options['fields']['link_things']['empty_zero'] = 0;
  /* Field: Things: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'things';
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
  $handler = $view->new_display('page', 'Page', 'things_admin_page');
  $handler->display->display_options['path'] = 'admin/content/things/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'Things';
  $handler->display->display_options['tab_options']['description'] = 'Manage things';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['things'] = array(
    t('Master'),
    t('Things'),
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
    t('No things have been created yet'),
    t('Things ID'),
    t('.'),
    t(','),
    t('Name'),
    t('View'),
    t('Operations links'),
    t('Page'),
  );
  $views['things_view'] = $view;
  return $views;

}

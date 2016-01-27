<?php

/**
 * @file
 * Providing extra functionality for the state_entity UI via views.
 */


/**
 * Implements hook_views_data_alter()
 */
function state_entity_views_data_alter(&$data) { 

  $data['state_entity']['link_state_entity'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the state_entity.'),
      'handler' => 'state_entity_handler_link_field',
    ),
  );
  $data['state_entity']['edit_state_entity'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the state_entity.'),
      'handler' => 'state_entity_handler_edit_link_field',
    ),
  );
  $data['state_entity']['delete_state_entity'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the state_entity.'),
      'handler' => 'state_entity_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows state_entity/state_entity/%state_entity_id/op
  $data['state_entity']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this state_entity.'),
      'handler' => 'state_entity_handler_state_entity_operations_field',
    ),
  );
}


/**
 * Implements hook_views_default_views().
 */
function state_entity_views_default_views() {

  $views = array();

  $view = new view;
  $view->name = 'state_entity';
  $view->description = 'A list of all State Entity';
  $view->tag = 'State Entity';
  $view->base_table = 'state_entity';
  $view->human_name = 'State Entity';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'State Entity';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create any State Entity type';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'state_entity_id' => 'state_entity_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(

    'state_entity_id' => array(
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
  $handler->display->display_options['empty']['area']['content'] = 'No State Entity have been created yet';
  /* Field: state_entity: state_entity ID */
  $handler->display->display_options['fields']['state_entity_id']['id'] = 'state_entity_id';
  $handler->display->display_options['fields']['state_entity_id']['table'] = 'state_entity';
  $handler->display->display_options['fields']['state_entity_id']['field'] = 'state_entity_id';
  $handler->display->display_options['fields']['state_entity_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['state_entity_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['state_entity_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['state_entity_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['state_entity_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['state_entity_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['state_entity_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['state_entity_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['state_entity_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['state_entity_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['state_entity_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['state_entity_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['state_entity_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['state_entity_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['state_entity_id']['empty_zero'] = 0;
  /* Field: state_entity: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'state_entity';
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

  /* Field: state_entity: Link */
  $handler->display->display_options['fields']['link_state_entity']['id'] = 'link_state_entity';
  $handler->display->display_options['fields']['link_state_entity']['table'] = 'state_entity';
  $handler->display->display_options['fields']['link_state_entity']['field'] = 'link_state_entity';
  $handler->display->display_options['fields']['link_state_entity']['label'] = 'View';
  $handler->display->display_options['fields']['link_state_entity']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['link_state_entity']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['link_state_entity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['link_state_entity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['link_state_entity']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['link_state_entity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['link_state_entity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['link_state_entity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['link_state_entity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['link_state_entity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['link_state_entity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['link_state_entity']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['link_state_entity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['link_state_entity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['link_state_entity']['empty_zero'] = 0;
  /* Field: state_entity: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'state_entity';
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
  $handler = $view->new_display('page', 'Page', 'state_entity_admin_page');
  $handler->display->display_options['path'] = 'admin/content/state_entity/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'State Entity';
  $handler->display->display_options['tab_options']['description'] = 'Manage State Entity';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['state_entity'] = array(
    t('Master'),
    t('State Entity'),
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
    t('No State Entity have been created yet'),
    t('state_entity ID'),
    t('.'),
    t(','),
    t('View'),
    t('Operations links'),
    t('Page'),
  );
  $views['state_entity_view'] = $view;

  return $views;
}

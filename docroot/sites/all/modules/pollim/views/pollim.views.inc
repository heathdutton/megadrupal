<?php

/**
 * @file
 * Providing extra functionality for the pollim UI via views.
 */


/**
 * Implements hook_views_data()
 */
function pollim_views_data_alter(&$data) { 
  $data['pollim']['link_pollim'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the pollim.'),
      'handler' => 'pollim_handler_link_field',
    ),
  );
  $data['pollim']['edit_pollim'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the pollim.'),
      'handler' => 'pollim_handler_edit_link_field',
    ),
  );
  $data['pollim']['delete_pollim'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the pollim.'),
      'handler' => 'pollim_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows pollims/pollim/%pollim_id/op
  $data['pollim']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this pollim.'),
      'handler' => 'pollim_handler_pollim_operations_field',
    ),
  );
}


/**
 * Implements hook_views_default_views().
 */
function pollim_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'pollims';
  $view->description = 'A list of all pollims';
  $view->tag = 'pollims';
  $view->base_table = 'pollim';
  $view->human_name = 'pollims';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'pollims';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer pollims';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'pollim_id' => 'pollim_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'pollim_id' => array(
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
  $handler->display->display_options['empty']['area']['content'] = 'No pollims have been created yet';
  /* Field: pollim: pollim ID */
  $handler->display->display_options['fields']['pollim_id']['id'] = 'pollim_id';
  $handler->display->display_options['fields']['pollim_id']['table'] = 'pollim';
  $handler->display->display_options['fields']['pollim_id']['field'] = 'pollim_id';
  $handler->display->display_options['fields']['pollim_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['pollim_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['pollim_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['pollim_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['pollim_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['pollim_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['pollim_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['pollim_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['pollim_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['pollim_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['pollim_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['pollim_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['pollim_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['pollim_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['pollim_id']['empty_zero'] = 0;
  /* Field: pollim: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'pollim';
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
  /* Field: pollim: Link */
  $handler->display->display_options['fields']['link_pollim']['id'] = 'link_pollim';
  $handler->display->display_options['fields']['link_pollim']['table'] = 'pollim';
  $handler->display->display_options['fields']['link_pollim']['field'] = 'link_pollim';
  $handler->display->display_options['fields']['link_pollim']['label'] = 'View';
  $handler->display->display_options['fields']['link_pollim']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['link_pollim']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['link_pollim']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['link_pollim']['alter']['external'] = 0;
  $handler->display->display_options['fields']['link_pollim']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['link_pollim']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['link_pollim']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['link_pollim']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['link_pollim']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['link_pollim']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['link_pollim']['alter']['html'] = 0;
  $handler->display->display_options['fields']['link_pollim']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['link_pollim']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['link_pollim']['hide_empty'] = 0;
  $handler->display->display_options['fields']['link_pollim']['empty_zero'] = 0;
  /* Field: pollim: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'pollim';
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
  $handler = $view->new_display('page', 'Page', 'pollims_admin_page');
  $handler->display->display_options['path'] = 'admin/content/pollims/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'pollims';
  $handler->display->display_options['tab_options']['description'] = 'Manage pollims';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  
  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_latest_pollim');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entity';
  $handler->display->display_options['row_options']['view_mode'] = 'default';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Pollim: Created */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'pollim';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  
  
  $translatables['pollims'] = array(
    t('Master'),
    t('pollims'),
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
    t('No pollims have been created yet'),
    t('pollim ID'),
    t('.'),
    t(','),
    t('Name'),
    t('View'),
    t('Operations links'),
    t('Page'),
  );
  $views[] = $view;
  return $views;

}

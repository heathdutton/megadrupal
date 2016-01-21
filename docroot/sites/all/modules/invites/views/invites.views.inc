<?php

/**
 * @file
 * Providing extra functionality for the Invites UI via views.
 */


/**
 * Implements hook_views_data()
 */
function invites_views_data_alter(&$data) {
  $data['invites']['link_invites'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the invites.'),
      'handler' => 'invites_handler_link_field',
    ),
  );
  $data['invites']['edit_invites'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the invites.'),
      'handler' => 'invites_handler_edit_link_field',
    ),
  );
  $data['invites']['delete_invites'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the invites.'),
      'handler' => 'invites_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows invites/invites/%invites_id/op
  $data['invites']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this invites.'),
      'handler' => 'invites_handler_invites_operations_field',
    ),
  );
}


/**
 * Implements hook_views_default_views().
 */
function invites_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'invites';
  $view->description = 'A list of all invites';
  $view->tag = 'invites';
  $view->base_table = 'invites';
  $view->human_name = 'Invites';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Invites';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create any invites type';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'mail' => 'mail',
    'invites_id' => 'invites_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'mail' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'invites_id' => array(
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
  $handler->display->display_options['empty']['area']['content'] = 'No invites have been created yet';
  /* Field: Invites: Invites ID */
  $handler->display->display_options['fields']['invites_id']['id'] = 'invites_id';
  $handler->display->display_options['fields']['invites_id']['table'] = 'invites';
  $handler->display->display_options['fields']['invites_id']['field'] = 'invites_id';
  $handler->display->display_options['fields']['invites_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['invites_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['invites_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['invites_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['invites_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['invites_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['invites_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['invites_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['invites_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['invites_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['invites_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['invites_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['invites_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['invites_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['invites_id']['empty_zero'] = 0;
  /* Field: Invites: Name */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'invites';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['external'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['mail']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['mail']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['mail']['alter']['html'] = 0;
  $handler->display->display_options['fields']['mail']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['mail']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['mail']['hide_empty'] = 0;
  $handler->display->display_options['fields']['mail']['empty_zero'] = 0;
  /* Field: Invites: Link */
  $handler->display->display_options['fields']['link_invites']['id'] = 'link_invites';
  $handler->display->display_options['fields']['link_invites']['table'] = 'invites';
  $handler->display->display_options['fields']['link_invites']['field'] = 'link_invites';
  $handler->display->display_options['fields']['link_invites']['label'] = 'View';
  $handler->display->display_options['fields']['link_invites']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['link_invites']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['link_invites']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['link_invites']['alter']['external'] = 0;
  $handler->display->display_options['fields']['link_invites']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['link_invites']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['link_invites']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['link_invites']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['link_invites']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['link_invites']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['link_invites']['alter']['html'] = 0;
  $handler->display->display_options['fields']['link_invites']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['link_invites']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['link_invites']['hide_empty'] = 0;
  $handler->display->display_options['fields']['link_invites']['empty_zero'] = 0;
  /* Field: Invites: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'invites';
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
  $handler = $view->new_display('page', 'Page', 'invites_admin_page');
  $handler->display->display_options['path'] = 'admin/content/invites/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'Invites';
  $handler->display->display_options['tab_options']['description'] = 'Manage invites';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['invites'] = array(
    t('Master'),
    t('Invites'),
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
    t('No invites have been created yet'),
    t('Invites ID'),
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

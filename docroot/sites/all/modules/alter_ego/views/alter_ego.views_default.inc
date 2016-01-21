<?php


/**
 * Implement hook_views_default_views().
 */
function alter_ego_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'avatars';
  $view->description = 'A list of all avatars';
  $view->tag = 'avatars';
  $view->base_table = 'avatar';
  $view->human_name = 'Avatars';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Avatars';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create any avatar type';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'aid' => 'aid',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'tid' => array(
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
  $handler->display->display_options['empty']['area']['content'] = 'No avatars have been created yet';
  /* Field: Avatar: Avatar ID */
  $handler->display->display_options['fields']['aid']['id'] = 'aid';
  $handler->display->display_options['fields']['aid']['table'] = 'avatar';
  $handler->display->display_options['fields']['aid']['field'] = 'aid';
  $handler->display->display_options['fields']['aid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['aid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['aid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['aid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['aid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['aid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['aid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['aid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['aid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['aid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['aid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['aid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['aid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['aid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['aid']['empty_zero'] = 0;
  /* Field: Avatar: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'avatar';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  /* Field: Avatar: Link */
  $handler->display->display_options['fields']['link_avatar']['id'] = 'link_avatar';
  $handler->display->display_options['fields']['link_avatar']['table'] = 'avatar';
  $handler->display->display_options['fields']['link_avatar']['field'] = 'link_avatar';
  $handler->display->display_options['fields']['link_avatar']['label'] = 'View';
  $handler->display->display_options['fields']['link_avatar']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['link_avatar']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['link_avatar']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['link_avatar']['alter']['external'] = 0;
  $handler->display->display_options['fields']['link_avatar']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['link_avatar']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['link_avatar']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['link_avatar']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['link_avatar']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['link_avatar']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['link_avatar']['alter']['html'] = 0;
  $handler->display->display_options['fields']['link_avatar']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['link_avatar']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['link_avatar']['hide_empty'] = 0;
  $handler->display->display_options['fields']['link_avatar']['empty_zero'] = 0;
  /* Field: Avatar: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'avatar';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['external'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['html'] = 0;
  $handler->display->display_options['fields']['operations']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['operations']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['operations']['hide_empty'] = 0;
  $handler->display->display_options['fields']['operations']['empty_zero'] = 0;
  
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'avatars_admin_page');
  $handler->display->display_options['path'] = 'admin/content/avatars/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'Avatars';
  $handler->display->display_options['tab_options']['description'] = 'Manage avatars';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  
  $views[$view->name] = $view;

  return $views;

}

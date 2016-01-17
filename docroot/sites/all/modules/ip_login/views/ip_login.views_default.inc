<?php

/**
 * @file
 * Default view at Admin > People > IP Login.
 */

/**
 * Implements hook_views_default_views().
 */
function ip_login_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'ip_login';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'IP Login';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'IP Login';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer users';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = 0;
  $handler->display->display_options['exposed_form']['options']['autosubmit_hide'] = 1;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '5, 10, 20, 40, 60, 100';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 1;
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Username';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['name']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  $handler->display->display_options['fields']['name']['format_username'] = 1;
  /* Field: User: IP Login */
  $handler->display->display_options['fields']['field_ip_login']['id'] = 'field_ip_login';
  $handler->display->display_options['fields']['field_ip_login']['table'] = 'field_data_field_ip_login';
  $handler->display->display_options['fields']['field_ip_login']['field'] = 'field_ip_login';
  $handler->display->display_options['fields']['field_ip_login']['label'] = 'IP Address';
  $handler->display->display_options['fields']['field_ip_login']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_ip_login']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_ip_login']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_ip_login']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_ip_login']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_ip_login']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_ip_login']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_ip_login']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_ip_login']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_ip_login']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_ip_login']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_ip_login']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_ip_login']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_ip_login']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_ip_login']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_ip_login']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_ip_login']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_ip_login']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_ip_login']['click_sort_column'] = 'start';
  $handler->display->display_options['fields']['field_ip_login']['type'] = 'field_ipaddress_shorthand';
  $handler->display->display_options['fields']['field_ip_login']['group_rows'] = 1;
  $handler->display->display_options['fields']['field_ip_login']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_ip_login']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['field_ip_login']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['field_ip_login']['field_api_classes'] = 0;
  /* Field: User: Roles */
  $handler->display->display_options['fields']['rid']['id'] = 'rid';
  $handler->display->display_options['fields']['rid']['table'] = 'users_roles';
  $handler->display->display_options['fields']['rid']['field'] = 'rid';
  $handler->display->display_options['fields']['rid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['rid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['rid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['rid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['rid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['rid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['rid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['rid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['rid']['hide_alter_empty'] = 1;
  /* Field: User: Last access */
  $handler->display->display_options['fields']['access']['id'] = 'access';
  $handler->display->display_options['fields']['access']['table'] = 'users';
  $handler->display->display_options['fields']['access']['field'] = 'access';
  $handler->display->display_options['fields']['access']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['access']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['access']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['access']['alter']['external'] = 0;
  $handler->display->display_options['fields']['access']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['access']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['access']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['access']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['access']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['access']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['access']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['access']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['access']['alter']['html'] = 0;
  $handler->display->display_options['fields']['access']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['access']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['access']['hide_empty'] = 0;
  $handler->display->display_options['fields']['access']['empty_zero'] = 0;
  $handler->display->display_options['fields']['access']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['access']['date_format'] = 'time ago';
  /* Field: User: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'users';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
  $handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['external'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['edit_node']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
  $handler->display->display_options['fields']['edit_node']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['edit_node']['text'] = 'edit';
  /* Sort criterion: User: Last access */
  $handler->display->display_options['sorts']['access']['id'] = 'access';
  $handler->display->display_options['sorts']['access']['table'] = 'users';
  $handler->display->display_options['sorts']['access']['field'] = 'access';
  $handler->display->display_options['sorts']['access']['order'] = 'DESC';
  /* Filter criterion: User: IP Login (field_ip_login:start) */
  $handler->display->display_options['filters']['field_ip_login_start']['id'] = 'field_ip_login_start';
  $handler->display->display_options['filters']['field_ip_login_start']['table'] = 'field_data_field_ip_login';
  $handler->display->display_options['filters']['field_ip_login_start']['field'] = 'field_ip_login_start';
  $handler->display->display_options['filters']['field_ip_login_start']['operator'] = 'not empty';
  $handler->display->display_options['filters']['field_ip_login_start']['group'] = 1;
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Username';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['multiple'] = FALSE;
  $handler->display->display_options['filters']['uid']['expose']['reduce'] = 0;
  
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/people/ip-login';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'IP Login';
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;


  $views['ip_login'] = $view;

  return $views;
}

<?php
/**
 * @file
 * Provides default views.
 */

/**
 * Implements hook_views_default_views().
 */
function uiplog_views_default_views() {
  $view = new view;
  $view->name = 'useriplog';
  $view->description = 'View IP address of recently logged in users.';
  $view->tag = '';
  $view->base_table = 'uiplog';
  $view->human_name = 'User IP Log';
  $view->core = 0;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'User IP Log';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer users';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'ip' => 'ip',
    'timestamp' => 'timestamp',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'ip' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'timestamp' => array(
      'sortable' => 0,
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['text']['id'] = 'text';
  $handler->display->display_options['empty']['text']['table'] = 'views';
  $handler->display->display_options['empty']['text']['field'] = 'area';
  $handler->display->display_options['empty']['text']['empty'] = FALSE;
  $handler->display->display_options['empty']['text']['content'] = 'No results found.';
  $handler->display->display_options['empty']['text']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['text']['tokenize'] = 0;
  /* Relationship: User IP: User id */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'uiplog';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = 0;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: User IP: IP Address */
  $handler->display->display_options['fields']['ip']['id'] = 'ip';
  $handler->display->display_options['fields']['ip']['table'] = 'uiplog';
  $handler->display->display_options['fields']['ip']['field'] = 'ip';
  $handler->display->display_options['fields']['ip']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['ip']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['ip']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['ip']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['ip']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['ip']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['ip']['alter']['html'] = 0;
  $handler->display->display_options['fields']['ip']['hide_empty'] = 0;
  $handler->display->display_options['fields']['ip']['empty_zero'] = 0;
  /* Field: User IP: Login Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'uiplog';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
  $handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
  $handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
  /* Sort criterion: User IP: Login Timestamp */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'uiplog';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  /* Filter criterion: User IP: User id */
  $handler->display->display_options['filters']['uid_2']['id'] = 'uid_2';
  $handler->display->display_options['filters']['uid_2']['table'] = 'uiplog';
  $handler->display->display_options['filters']['uid_2']['field'] = 'uid';
  $handler->display->display_options['filters']['uid_2']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid_2']['expose']['operator_id'] = 'uid_2_op';
  $handler->display->display_options['filters']['uid_2']['expose']['label'] = 'User id';
  $handler->display->display_options['filters']['uid_2']['expose']['operator'] = 'uid_2_op';
  $handler->display->display_options['filters']['uid_2']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid_2']['expose']['required'] = FALSE;
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'auth_user';
  $handler->display->display_options['filters']['uid']['expose']['required'] = FALSE;
  $handler->display->display_options['filters']['uid']['expose']['reduce'] = 0;
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['filters']['uid_1']['table'] = 'users';
  $handler->display->display_options['filters']['uid_1']['field'] = 'uid';
  $handler->display->display_options['filters']['uid_1']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid_1']['operator'] = 'not in';
  $handler->display->display_options['filters']['uid_1']['value'] = array(
    0 => 0,
  );
  $handler->display->display_options['filters']['uid_1']['expose']['operator'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'admin/reports/useriplog';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'User IP Log';
  $handler->display->display_options['menu']['description'] = 'View IP address of recently logged in users.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';


  $views[$view->name] = $view;
  return $views;
}

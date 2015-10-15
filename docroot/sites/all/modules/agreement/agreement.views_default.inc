<?php

/**
 * @file
 * Default views for Agreement module.
 */

/**
 * Implements hook_views_default_views().
 */
function agreement_views_default_views() {
  /*
   * View 'agreements'
   */
$view = new view;
$view->name = 'Agreements';
$view->description = 'A list of all agreements';
$view->tag = 'agreement';
$view->base_table = 'users';
$view->human_name = '';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = 'Agreements';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'administer users';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['grouping'] = '';
$handler->display->display_options['style_options']['columns'] = array(
  'name' => 'name',
  'agreed_date' => 'agreed_date',
  'status' => 'status',
);
$handler->display->display_options['style_options']['default'] = 'name';
$handler->display->display_options['style_options']['info'] = array(
  'name' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'agreed_date' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'status' => array(
    'sortable' => 1,
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
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
/* Field: User: Agreement date */
$handler->display->display_options['fields']['agreed_date']['id'] = 'agreed_date';
$handler->display->display_options['fields']['agreed_date']['table'] = 'agreement';
$handler->display->display_options['fields']['agreed_date']['field'] = 'agreed_date';
$handler->display->display_options['fields']['agreed_date']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['agreed_date']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['agreed_date']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['agreed_date']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['agreed_date']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['agreed_date']['alter']['trim'] = 0;
$handler->display->display_options['fields']['agreed_date']['alter']['html'] = 0;
$handler->display->display_options['fields']['agreed_date']['empty'] = 'Unknown';
$handler->display->display_options['fields']['agreed_date']['hide_empty'] = 0;
$handler->display->display_options['fields']['agreed_date']['empty_zero'] = 1;
/* Filter criterion: User: Name */
$handler->display->display_options['filters']['uid_1']['id'] = 'uid_1';
$handler->display->display_options['filters']['uid_1']['table'] = 'users';
$handler->display->display_options['filters']['uid_1']['field'] = 'uid';
$handler->display->display_options['filters']['uid_1']['operator'] = 'not in';
$handler->display->display_options['filters']['uid_1']['value'] = array(
  0 => 0,
);
$handler->display->display_options['filters']['uid_1']['group'] = '0';
$handler->display->display_options['filters']['uid_1']['expose']['operator'] = FALSE;
/* Filter criterion: User: Name */
$handler->display->display_options['filters']['uid']['id'] = 'uid';
$handler->display->display_options['filters']['uid']['table'] = 'users';
$handler->display->display_options['filters']['uid']['field'] = 'uid';
$handler->display->display_options['filters']['uid']['value'] = '';
$handler->display->display_options['filters']['uid']['group'] = '0';
$handler->display->display_options['filters']['uid']['exposed'] = TRUE;
$handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
$handler->display->display_options['filters']['uid']['expose']['label'] = 'Username';
$handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
$handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
$handler->display->display_options['filters']['uid']['expose']['required'] = FALSE;
$handler->display->display_options['filters']['uid']['expose']['reduce'] = 0;
/* Filter criterion: User: Agreement date */
$handler->display->display_options['filters']['agreed_date']['id'] = 'agreed_date';
$handler->display->display_options['filters']['agreed_date']['table'] = 'agreement';
$handler->display->display_options['filters']['agreed_date']['field'] = 'agreed_date';
$handler->display->display_options['filters']['agreed_date']['operator'] = 'between';
$handler->display->display_options['filters']['agreed_date']['group'] = '0';
$handler->display->display_options['filters']['agreed_date']['exposed'] = TRUE;
$handler->display->display_options['filters']['agreed_date']['expose']['operator_id'] = 'agreed_date_op';
$handler->display->display_options['filters']['agreed_date']['expose']['label'] = 'Agreement date between';
$handler->display->display_options['filters']['agreed_date']['expose']['operator'] = 'agreed_date_op';
$handler->display->display_options['filters']['agreed_date']['expose']['identifier'] = 'agreed_date';
$handler->display->display_options['filters']['agreed_date']['expose']['required'] = FALSE;

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['path'] = 'admin/people/agreements';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Agreements';
$handler->display->display_options['menu']['description'] = 'A list of all agreements';
$handler->display->display_options['menu']['weight'] = '10';
$handler->display->display_options['menu']['name'] = 'user-menu';
$handler->display->display_options['menu']['context'] = 0;
  $views[$view->name] = $view;

  return $views;
}

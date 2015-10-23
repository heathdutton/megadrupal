<?php
/**
 * Implementation of hook_views_default_views().
 */
function user_mailman_register_views_default_views() {
  /*
   * View 'user_mailman_subscriptions'
   */
  $view = new view();
  $view->name = 'user_mailman_subscriptions';
  $view->description = 'User subscriptions page grouped by list name';
  $view->tag = 'mailman';
  $view->base_table = 'users';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Mailing lists view';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer mailman_manager';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = 'name_1';
  $handler->display->display_options['style_options']['columns'] = array(
    'uid' => 'uid',
    'lmod' => 'lmod',
    'lchecktime' => 'lchecktime',
    'description' => 'description',
    'name_1' => 'name_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'uid' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'lmod' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'lchecktime' => array(
      'separator' => '',
    ),
    'description' => array(
      'separator' => '',
    ),
    'name_1' => array(
      'separator' => '',
    ),
  );
  /* Field: Mailman Subscribers: Subscription page */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'mailman_users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['text'] = '[name] ([uid])';
  /* Field: Mailman Subscribers: Subscription status */
  $handler->display->display_options['fields']['lstatus']['id'] = 'lstatus';
  $handler->display->display_options['fields']['lstatus']['table'] = 'mailman_users';
  $handler->display->display_options['fields']['lstatus']['field'] = 'lstatus';
  /* Field: Mailman Subscribers: Moderated */
  $handler->display->display_options['fields']['lmod']['id'] = 'lmod';
  $handler->display->display_options['fields']['lmod']['table'] = 'mailman_users';
  $handler->display->display_options['fields']['lmod']['field'] = 'lmod';
  $handler->display->display_options['fields']['lmod']['not'] = 0;
  /* Field: Mailman Subscribers: Last sync */
  $handler->display->display_options['fields']['lchecktime']['id'] = 'lchecktime';
  $handler->display->display_options['fields']['lchecktime']['table'] = 'mailman_users';
  $handler->display->display_options['fields']['lchecktime']['field'] = 'lchecktime';
  /* Field: Mailman Lists: Description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'mailman_lists';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['exclude'] = TRUE;
  $handler->display->display_options['fields']['description']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['description']['alter']['text'] = '([description])';
  $handler->display->display_options['fields']['description']['hide_empty'] = TRUE;
  /* Field: Mailman Lists: Mailing list name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'mailman_lists';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['label'] = '';
  $handler->display->display_options['fields']['name_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name_1']['alter']['text'] = '<h3>[name_1]  [description]</h3>';
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = '0';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'User name';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  /* Filter criterion: Mailman Lists: Mailing list name */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'mailman_lists';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['group'] = '0';
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Mailing list name';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  /* Filter criterion: Mailman Subscribers: Subscription status */
  $handler->display->display_options['filters']['lstatus']['id'] = 'lstatus';
  $handler->display->display_options['filters']['lstatus']['table'] = 'mailman_users';
  $handler->display->display_options['filters']['lstatus']['field'] = 'lstatus';
  $handler->display->display_options['filters']['lstatus']['value'] = array(
    1 => '1',
    2 => '2',
    3 => '3',
  );
  $handler->display->display_options['filters']['lstatus']['exposed'] = TRUE;
  $handler->display->display_options['filters']['lstatus']['expose']['operator_id'] = 'lstatus_op';
  $handler->display->display_options['filters']['lstatus']['expose']['label'] = 'Subscription status';
  $handler->display->display_options['filters']['lstatus']['expose']['operator'] = 'lstatus_op';
  $handler->display->display_options['filters']['lstatus']['expose']['identifier'] = 'lstatus';
  $handler->display->display_options['filters']['lstatus']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['lstatus']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    8 => 0,
    6 => 0,
    15 => 0,
    26 => 0,
    25 => 0,
    24 => 0,
    9 => 0,
    10 => 0,
    22 => 0,
    19 => 0,
    20 => 0,
    18 => 0,
    14 => 0,
    5 => 0,
    16 => 0,
    11 => 0,
    21 => 0,
    23 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'admin/people/user_mailman_subscriptions';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Mailing lists';
  $handler->display->display_options['menu']['description'] = 'List user subscriptions grouped by list';
  $handler->display->display_options['menu']['weight'] = '0';
  $translatables['user_mailman_subscriptions'] = array(
    t('Defaults'),
    t('Mailing lists view'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Subscription page'),
    t('Name'),
    t('[name] ([uid])'),
    t('Subscription status'),
    t('Moderated'),
    t('Last sync'),
    t('Description'),
    t('([description])'),
    t('<h3>[name_1]  [description]</h3>'),
    t('User name'),
    t('Mailing list name'),
    t('Page'),
  );

  $views[$view->name] = $view;

  return $views;
}

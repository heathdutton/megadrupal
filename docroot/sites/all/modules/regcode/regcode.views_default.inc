<?php

/**
 * @file
 * Default Views for regcode module.
 */

/**
 * Implements hook_views_default_views().
 */
function regcode_views_default_views() {
  $view = new view();
  $view->name = 'regcode';
  $view->description = 'Provides an administrative interface to manage registration codes.';
  $view->tag = '';
  $view->base_table = 'regcode';
  $view->human_name = 'Registration codes';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Registration codes';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer registration codes';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '200';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '50, 100, 200, 500, 1000';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = TRUE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['style_options']['columns'] = array(
    'rid' => 'rid',
    'created' => 'created',
    'is_active' => 'is_active',
    'code' => 'code',
    'uses' => 'uses',
    'maxuses' => 'maxuses',
    'begins' => 'begins',
    'expires' => 'expires',
    'lastused' => 'lastused',
    'name' => 'name',
    'tid' => 'tid',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'rid' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'created' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'is_active' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'code' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'uses' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'maxuses' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'begins' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'expires' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'lastused' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'name' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'tid' => array(
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['order'] = 'desc';
  /* Relationship: Regcode: User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'regcode';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Regcode: ID */
  $handler->display->display_options['fields']['rid']['id'] = 'rid';
  $handler->display->display_options['fields']['rid']['table'] = 'regcode';
  $handler->display->display_options['fields']['rid']['field'] = 'rid';
  $handler->display->display_options['fields']['rid']['exclude'] = TRUE;
  /* Field: Regcode: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'regcode';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'd/m/y';
  /* Field: Regcode: Active */
  $handler->display->display_options['fields']['is_active']['id'] = 'is_active';
  $handler->display->display_options['fields']['is_active']['table'] = 'regcode';
  $handler->display->display_options['fields']['is_active']['field'] = 'is_active';
  $handler->display->display_options['fields']['is_active']['not'] = 0;
  /* Field: Regcode: Code */
  $handler->display->display_options['fields']['code']['id'] = 'code';
  $handler->display->display_options['fields']['code']['table'] = 'regcode';
  $handler->display->display_options['fields']['code']['field'] = 'code';
  /* Field: Regcode: Uses */
  $handler->display->display_options['fields']['uses']['id'] = 'uses';
  $handler->display->display_options['fields']['uses']['table'] = 'regcode';
  $handler->display->display_options['fields']['uses']['field'] = 'uses';
  $handler->display->display_options['fields']['uses']['empty'] = '0';
  /* Field: Regcode: Maximum uses */
  $handler->display->display_options['fields']['maxuses']['id'] = 'maxuses';
  $handler->display->display_options['fields']['maxuses']['table'] = 'regcode';
  $handler->display->display_options['fields']['maxuses']['field'] = 'maxuses';
  $handler->display->display_options['fields']['maxuses']['label'] = 'Max';
  /* Field: Regcode: Begins */
  $handler->display->display_options['fields']['begins']['id'] = 'begins';
  $handler->display->display_options['fields']['begins']['table'] = 'regcode';
  $handler->display->display_options['fields']['begins']['field'] = 'begins';
  $handler->display->display_options['fields']['begins']['empty'] = '-';
  $handler->display->display_options['fields']['begins']['date_format'] = 'custom';
  $handler->display->display_options['fields']['begins']['custom_date_format'] = 'd/m/y';
  /* Field: Regcode: Expires */
  $handler->display->display_options['fields']['expires']['id'] = 'expires';
  $handler->display->display_options['fields']['expires']['table'] = 'regcode';
  $handler->display->display_options['fields']['expires']['field'] = 'expires';
  $handler->display->display_options['fields']['expires']['empty'] = '-';
  $handler->display->display_options['fields']['expires']['date_format'] = 'custom';
  $handler->display->display_options['fields']['expires']['custom_date_format'] = 'd/m/y';
  /* Field: Regcode: Last used */
  $handler->display->display_options['fields']['lastused']['id'] = 'lastused';
  $handler->display->display_options['fields']['lastused']['table'] = 'regcode';
  $handler->display->display_options['fields']['lastused']['field'] = 'lastused';
  $handler->display->display_options['fields']['lastused']['empty'] = '-';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'User';
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = TRUE;
  $handler->display->display_options['fields']['name']['anonymous_text'] = '-';
  /* Field: Taxonomy: All terms */
  $handler->display->display_options['fields']['tid']['id'] = 'tid';
  $handler->display->display_options['fields']['tid']['table'] = 'regcode_term';
  $handler->display->display_options['fields']['tid']['field'] = 'tid';
  $handler->display->display_options['fields']['tid']['label'] = 'Tags';
  $handler->display->display_options['fields']['tid']['link_to_taxonomy'] = 0;
  $handler->display->display_options['fields']['tid']['limit'] = 1;
  $handler->display->display_options['fields']['tid']['vids'] = array(
    1 => 1,
  );
  /* Filter criterion: Taxonomy term: Term */
  $handler->display->display_options['filters']['tid']['id'] = 'tid';
  $handler->display->display_options['filters']['tid']['table'] = 'taxonomy_term_data';
  $handler->display->display_options['filters']['tid']['field'] = 'tid';
  $handler->display->display_options['filters']['tid']['group'] = '0';
  $handler->display->display_options['filters']['tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['tid']['expose']['operator_id'] = 'tid_op';
  $handler->display->display_options['filters']['tid']['expose']['label'] = 'Tag';
  $handler->display->display_options['filters']['tid']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['tid']['expose']['operator'] = 'tid_op';
  $handler->display->display_options['filters']['tid']['expose']['identifier'] = 'tid';
  $handler->display->display_options['filters']['tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  $handler->display->display_options['filters']['tid']['type'] = 'select';
  $handler->display->display_options['filters']['tid']['vocabulary'] = 'regcode';
  /* Filter criterion: Regcode: Created */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'regcode';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['operator'] = '>=';
  $handler->display->display_options['filters']['created']['value']['type'] = 'offset';
  $handler->display->display_options['filters']['created']['group'] = '0';
  $handler->display->display_options['filters']['created']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = 'Created';
  $handler->display->display_options['filters']['created']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
  $handler->display->display_options['filters']['created']['expose']['remember'] = TRUE;
  /* Filter criterion: Regcode: Active */
  $handler->display->display_options['filters']['is_active']['id'] = 'is_active';
  $handler->display->display_options['filters']['is_active']['table'] = 'regcode';
  $handler->display->display_options['filters']['is_active']['field'] = 'is_active';
  $handler->display->display_options['filters']['is_active']['value'] = 'All';
  $handler->display->display_options['filters']['is_active']['group'] = '0';
  $handler->display->display_options['filters']['is_active']['exposed'] = TRUE;
  $handler->display->display_options['filters']['is_active']['expose']['label'] = 'Active';
  $handler->display->display_options['filters']['is_active']['expose']['identifier'] = 'active';
  $handler->display->display_options['filters']['is_active']['expose']['remember'] = TRUE;

  /* Display: List */
  $handler = $view->new_display('page', 'List', 'page_admin');
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['text']['id'] = 'area';
  $handler->display->display_options['empty']['text']['table'] = 'views';
  $handler->display->display_options['empty']['text']['field'] = 'area';
  $handler->display->display_options['empty']['text']['content'] = 'You have not created any registration codes.';
  $handler->display->display_options['empty']['text']['format'] = 'plain_text';
  $handler->display->display_options['path'] = 'admin/config/people/regcode/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['description'] = 'List and manage registration codes';
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Registration codes';
  $handler->display->display_options['tab_options']['description'] = 'Manage user registration codes.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  $views[$view->name] = $view;

  return $views;
}

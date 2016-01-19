<?php
/**
 * @file
 * Default views for the OAuth2 API Token module.
 */

/**
 * Implements hook_views_default_views().
 */
function oauth2_api_token_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'oauth2_api_token';
  $view->description = '';
  $view->tag = 'oauth2_api_token';
  $view->base_table = 'oauth2_server_token';
  $view->human_name = 'API tokens';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'API Tokens';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'oauth2_api_token manage own';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'api_token_app' => 'api_token_app',
    'scopes' => 'scopes',
    'created' => 'created',
    'last_access' => 'last_access',
    'token_id' => 'token_id',
    'nothing_1' => 'nothing_1',
    'nothing' => 'nothing_1',
    'nothing_2' => 'nothing_1',
  );
  $handler->display->display_options['style_options']['default'] = 'api_token_app';
  $handler->display->display_options['style_options']['info'] = array(
    'api_token_app' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'scopes' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'last_access' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'token_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => ' &middot; ',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_2' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No API tokens found.';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No API tokens found.';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  /* Field: OAuth2 Server - Token: Application */
  $handler->display->display_options['fields']['api_token_app']['id'] = 'api_token_app';
  $handler->display->display_options['fields']['api_token_app']['table'] = 'field_data_api_token_app';
  $handler->display->display_options['fields']['api_token_app']['field'] = 'api_token_app';
  /* Field: Field: Scopes */
  $handler->display->display_options['fields']['scopes']['id'] = 'scopes';
  $handler->display->display_options['fields']['scopes']['table'] = 'field_data_scopes';
  $handler->display->display_options['fields']['scopes']['field'] = 'scopes';
  $handler->display->display_options['fields']['scopes']['settings'] = array(
    'link' => 0,
  );
  $handler->display->display_options['fields']['scopes']['delta_offset'] = '0';
  $handler->display->display_options['fields']['scopes']['separator'] = ' ';
  /* Field: OAuth2 Server - Token: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'oauth2_server_token';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: OAuth2 Server - Token: Last access */
  $handler->display->display_options['fields']['last_access']['id'] = 'last_access';
  $handler->display->display_options['fields']['last_access']['table'] = 'oauth2_server_token';
  $handler->display->display_options['fields']['last_access']['field'] = 'last_access';
  $handler->display->display_options['fields']['last_access']['date_format'] = 'short';
  $handler->display->display_options['fields']['last_access']['second_date_format'] = 'long';
  /* Field: OAuth2 Server - Token: Oauth2 server - token ID */
  $handler->display->display_options['fields']['token_id']['id'] = 'token_id';
  $handler->display->display_options['fields']['token_id']['table'] = 'oauth2_server_token';
  $handler->display->display_options['fields']['token_id']['field'] = 'token_id';
  $handler->display->display_options['fields']['token_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['token_id']['separator'] = '';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Links';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'View';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'user/!1/api-tokens/[token_id]';
  $handler->display->display_options['fields']['nothing_1']['alter']['absolute'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Edit';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'user/!1/api-tokens/[token_id]/edit';
  $handler->display->display_options['fields']['nothing']['alter']['absolute'] = TRUE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'Delete';
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Delete';
  $handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'user/!1/api-tokens/[token_id]/delete';
  $handler->display->display_options['fields']['nothing_2']['alter']['absolute'] = TRUE;
  /* Sort criterion: OAuth2 Server - Token: Application (api_token_app) */
  $handler->display->display_options['sorts']['api_token_app_value']['id'] = 'api_token_app_value';
  $handler->display->display_options['sorts']['api_token_app_value']['table'] = 'field_data_api_token_app';
  $handler->display->display_options['sorts']['api_token_app_value']['field'] = 'api_token_app_value';
  /* Contextual filter: OAuth2 Server - Token: User uid */
  $handler->display->display_options['arguments']['user']['id'] = 'user';
  $handler->display->display_options['arguments']['user']['table'] = 'oauth2_server_token';
  $handler->display->display_options['arguments']['user']['field'] = 'user';
  $handler->display->display_options['arguments']['user']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['user']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['user']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['user']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['user']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['user']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['user']['validate']['type'] = 'user';
  $handler->display->display_options['arguments']['user']['validate']['fail'] = 'empty';
  /* Filter criterion: OAuth2 Server - Token: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'oauth2_server_token';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'api_token' => 'api_token',
  );

  /* Display: Embed */
  $handler = $view->new_display('embed', 'Embed', 'embed_1');

  $views[$view->name] = $view;

  return $views;
}

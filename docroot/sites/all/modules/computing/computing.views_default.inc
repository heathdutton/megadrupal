<?php

/**
 * Implements hook_views_default_views().
 */
function computing_views_default_views() {
  $views = array();

  // The computing records view.

  $view = new view();
  $view->name = 'computing_records';
  $view->description = 'Displays the list of computing records.';
  $view->tag = 'computing';
  $view->base_table = 'computing_record';
  $view->human_name = 'Computing Records';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Records';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer computing records';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No results message';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No computing record yet.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Computing Record: Computing record ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'computing_record';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  /* Field: Computing Record: Application */
  $handler->display->display_options['fields']['application']['id'] = 'application';
  $handler->display->display_options['fields']['application']['table'] = 'computing_record';
  $handler->display->display_options['fields']['application']['field'] = 'application';
  /* Field: Computing Record: Command */
  $handler->display->display_options['fields']['command']['id'] = 'command';
  $handler->display->display_options['fields']['command']['table'] = 'computing_record';
  $handler->display->display_options['fields']['command']['field'] = 'command';
  /* Field: Computing Record: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'computing_record';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  /* Field: Computing Record: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'computing_record';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = 'User';
  /* Field: Computing Record: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'computing_record';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: Computing Record: Weight */
  $handler->display->display_options['fields']['weight']['id'] = 'weight';
  $handler->display->display_options['fields']['weight']['table'] = 'computing_record';
  $handler->display->display_options['fields']['weight']['field'] = 'weight';
  /* Field: Computing Record: Message */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'computing_record';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  /* Field: Computing Record: Input */
  $handler->display->display_options['fields']['input']['id'] = 'input';
  $handler->display->display_options['fields']['input']['table'] = 'computing_record';
  $handler->display->display_options['fields']['input']['field'] = 'input';
  $handler->display->display_options['fields']['input']['format'] = 'serialized';
  /* Field: Computing Record: Output */
  $handler->display->display_options['fields']['output']['id'] = 'output';
  $handler->display->display_options['fields']['output']['table'] = 'computing_record';
  $handler->display->display_options['fields']['output']['field'] = 'output';
  $handler->display->display_options['fields']['output']['format'] = 'serialized';
  /* Field: Computing Record: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'computing_record';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Computing Record: Changed */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'computing_record';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'long';
  /* Sort criterion: Computing Record: Created */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'computing_record';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Sort criterion: Computing Record: Changed */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'computing_record';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  /* Filter criterion: Computing Record: Application */
  $handler->display->display_options['filters']['application']['id'] = 'application';
  $handler->display->display_options['filters']['application']['table'] = 'computing_record';
  $handler->display->display_options['filters']['application']['field'] = 'application';
  $handler->display->display_options['filters']['application']['exposed'] = TRUE;
  $handler->display->display_options['filters']['application']['expose']['operator_id'] = 'application_op';
  $handler->display->display_options['filters']['application']['expose']['label'] = 'Application';
  $handler->display->display_options['filters']['application']['expose']['operator'] = 'application_op';
  $handler->display->display_options['filters']['application']['expose']['identifier'] = 'application';
  $handler->display->display_options['filters']['application']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Computing Record: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'computing_record';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/config/system/computing/records';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Records';
  $handler->display->display_options['menu']['description'] = 'Displays the list of computing records';
  $handler->display->display_options['menu']['weight'] = '-5';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Computing';
  $handler->display->display_options['tab_options']['description'] = 'Tabs related to Drupal Computing module.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';


  $views[$view->name] = $view;

  // return default views
  return $views;
}
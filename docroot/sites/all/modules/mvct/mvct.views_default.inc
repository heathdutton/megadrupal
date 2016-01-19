<?php
/**
 * @file
 * Default views to the mvct project.
 */

/**
 * Implements hook_views_default_views().
 */
function mvct_views_default_views() {

  $view = new view();
  $view->name = 'mvct_statistics';
  $view->description = 'Displays a list nodes tracked by the MVCT module, with their viewing statistics.';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'MVCT Statistics';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'MVCT Statistics';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer mvct';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Most Viewed per Content Type: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'mvct';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Most Viewed per Content Type: Last visit */
  $handler->display->display_options['fields']['last_viewed']['id'] = 'last_viewed';
  $handler->display->display_options['fields']['last_viewed']['table'] = 'mvct';
  $handler->display->display_options['fields']['last_viewed']['field'] = 'last_viewed';
  $handler->display->display_options['fields']['last_viewed']['date_format'] = 'long';
  $handler->display->display_options['fields']['last_viewed']['second_date_format'] = 'long';
  /* Field: Most Viewed per Content Type: Number of visits */
  $handler->display->display_options['fields']['view_count']['id'] = 'view_count';
  $handler->display->display_options['fields']['view_count']['table'] = 'mvct';
  $handler->display->display_options['fields']['view_count']['field'] = 'view_count';
  /* Sort criterion: Most Viewed per Content Type: Number of visits */
  $handler->display->display_options['sorts']['view_count']['id'] = 'view_count';
  $handler->display->display_options['sorts']['view_count']['table'] = 'mvct';
  $handler->display->display_options['sorts']['view_count']['field'] = 'view_count';
  $handler->display->display_options['sorts']['view_count']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Most Viewed per Content Type: Number of visits */
  $handler->display->display_options['filters']['view_count']['id'] = 'view_count';
  $handler->display->display_options['filters']['view_count']['table'] = 'mvct';
  $handler->display->display_options['filters']['view_count']['field'] = 'view_count';
  $handler->display->display_options['filters']['view_count']['operator'] = '>=';
  $handler->display->display_options['filters']['view_count']['value']['value'] = '1';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/config/system/mvct-statistics';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Most Viewed per Content Type - Statistics';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $views[$view->name] = $view;

  return $views;
}


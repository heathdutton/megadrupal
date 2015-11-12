<?php

/**
 * @file
 * Views file.
 */

/**
 * Implements hook_views_default_views().
 */
function link_click_count_views_default_views() {

$view = new view();
$view->name = 'link_click_count_stats';
$view->description = 'Stats of clicks happend on links on this site.';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'Link Click Count Stats';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

// Display: Master
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Link Click Count Stats';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '25';
$handler->display->display_options['style_plugin'] = 'table';
// No results behavior: Global: Text area
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'Oops';
$handler->display->display_options['empty']['area']['empty'] = TRUE;
$handler->display->display_options['empty']['area']['content'] = 'Site donot have any click stats.';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
// Field: Content: Title 
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
// Field: Link Click Count: Link Node id
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'link_click_count';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
$handler->display->display_options['fields']['nid']['label'] = 'Node id where link exist';
// Field: Link Click Count: Link url
$handler->display->display_options['fields']['url']['id'] = 'url';
$handler->display->display_options['fields']['url']['table'] = 'link_click_count';
$handler->display->display_options['fields']['url']['field'] = 'url';
$handler->display->display_options['fields']['url']['label'] = 'Link clicked';
// Field: COUNT(Link Click Count: Link url) 
$handler->display->display_options['fields']['url_1']['id'] = 'url_1';
$handler->display->display_options['fields']['url_1']['table'] = 'link_click_count';
$handler->display->display_options['fields']['url_1']['field'] = 'url';
$handler->display->display_options['fields']['url_1']['group_type'] = 'count';
$handler->display->display_options['fields']['url_1']['label'] = 'Clicks happened';
// Sort criterion: Content: Post date 
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'node';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
// Filter criterion: Content: Published 
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
// Filter criterion: Link Click Count: Link click date 
$handler->display->display_options['filters']['date']['id'] = 'date';
$handler->display->display_options['filters']['date']['table'] = 'link_click_count';
$handler->display->display_options['filters']['date']['field'] = 'date';
$handler->display->display_options['filters']['date']['operator'] = 'between';
$handler->display->display_options['filters']['date']['exposed'] = TRUE;
$handler->display->display_options['filters']['date']['expose']['operator_id'] = 'date_op';
$handler->display->display_options['filters']['date']['expose']['label'] = 'Clicks happened between two dates';
$handler->display->display_options['filters']['date']['expose']['description'] = 'CCYY-MM-DD format is prefered.';
$handler->display->display_options['filters']['date']['expose']['operator'] = 'date_op';
$handler->display->display_options['filters']['date']['expose']['identifier'] = 'date';
$handler->display->display_options['filters']['date']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);

// Display: Page 
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'link-click-count-stats';
$translatables['link_click_count_stats'] = array(
  t('Master'),
  t('Link Click Count Stats'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('« first'),
  t('‹ previous'),
  t('next ›'),
  t('last »'),
  t('Oops'),
  t('Site donot have any click stats.'),
  t('Title'),
  t('Node id where link exist'),
  t('.'),
  t(','),
  t('Link clicked'),
  t('Clicks happened'),
  t('Clicks happened between two dates'),
  t('CCYY-MM-DD format is prefered.'),
  t('Page'),
);

  $views[$view->name] = $view;
  return $views;
}


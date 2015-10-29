<?php
/**
 * @file
 * Default View for Zeitgeist
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

$view = new view();
$view->name = 'zeitgeist';
$view->description = 'Default view for Zeitgeist';
$view->tag = 'default';
$view->base_table = 'zeitgeist';
$view->human_name = 'Zeitgeist';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'search content';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '20';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['quantity'] = '9';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'ts' => 'ts',
  'search' => 'search',
  'category' => 'category',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'ts' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'search' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'category' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* Field: COUNT(Zeitgeist: Timestamp) */
$handler->display->display_options['fields']['ts']['id'] = 'ts';
$handler->display->display_options['fields']['ts']['table'] = 'zeitgeist';
$handler->display->display_options['fields']['ts']['field'] = 'ts';
$handler->display->display_options['fields']['ts']['group_type'] = 'count';
$handler->display->display_options['fields']['ts']['label'] = 'Count';
/* Field: Zeitgeist: Search */
$handler->display->display_options['fields']['search']['id'] = 'search';
$handler->display->display_options['fields']['search']['table'] = 'zeitgeist';
$handler->display->display_options['fields']['search']['field'] = 'search';
$handler->display->display_options['fields']['search']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['search']['alter']['path'] = 'search/[category]/[search]';
/* Field: Zeitgeist: Search kind */
$handler->display->display_options['fields']['category']['id'] = 'category';
$handler->display->display_options['fields']['category']['table'] = 'zeitgeist';
$handler->display->display_options['fields']['category']['field'] = 'category';
$handler->display->display_options['fields']['category']['label'] = 'Type';
/* Sort criterion: COUNT(Zeitgeist: Timestamp) */
$handler->display->display_options['sorts']['ts']['id'] = 'ts';
$handler->display->display_options['sorts']['ts']['table'] = 'zeitgeist';
$handler->display->display_options['sorts']['ts']['field'] = 'ts';
$handler->display->display_options['sorts']['ts']['group_type'] = 'count';
$handler->display->display_options['sorts']['ts']['order'] = 'DESC';
$translatables['zeitgeist'] = array(
  t('Defaults'),
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
  t('Count'),
  t('Search'),
  t('Type'),
);

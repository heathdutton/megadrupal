<?php

/**
 * @file
 * Contains default views for the Spain LOPD module.
 */

/**
 * Implementation of hook_views_default_views().
 */
function lopd_views_default_views() {
  $view = new view();
  $view->name = 'LOPD';
  $view->description = 'LOPD';
  $view->tag = 'LOPD';
  $view->base_table = 'lopd';
  $view->human_name = '';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'LOPD Access log';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access LOPD data';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['style_options']['columns'] = array(
    'lopdid' => 'lopdid',
    'operation' => 'operation',
    'timestamp' => 'timestamp',
    'name' => 'name',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'lopdid' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'operation' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'timestamp' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'name' => array(
      'sortable' => 0,
      'separator' => '',
    ),
  );
  /* Relationship: LOPD: Usuario */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'lopd';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'User';
  /* Field: LOPD: LOPD ID */
  $handler->display->display_options['fields']['lopdid']['id'] = 'lopdid';
  $handler->display->display_options['fields']['lopdid']['table'] = 'lopd';
  $handler->display->display_options['fields']['lopdid']['field'] = 'lopdid';
  /* Field: LOPD: Sello temporal */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'lopd';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['label'] = 'Timestamp';
  /* Field: LOPD: Operación */
  $handler->display->display_options['fields']['operation']['id'] = 'operation';
  $handler->display->display_options['fields']['operation']['table'] = 'lopd';
  $handler->display->display_options['fields']['operation']['field'] = 'operation';
  $handler->display->display_options['fields']['operation']['label'] = 'Operation';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'User';
  /* Field: LOPD: Nombre */
  $handler->display->display_options['fields']['authname']['id'] = 'authname';
  $handler->display->display_options['fields']['authname']['table'] = 'lopd';
  $handler->display->display_options['fields']['authname']['field'] = 'authname';
  $handler->display->display_options['fields']['authname']['label'] = 'Authname';
  /* Field: LOPD: IP */
  $handler->display->display_options['fields']['ip']['id'] = 'ip';
  $handler->display->display_options['fields']['ip']['table'] = 'lopd';
  $handler->display->display_options['fields']['ip']['field'] = 'ip';

  /* Display: LOPD report page */
  $handler = $view->new_display('page', 'LOPD report page', 'page_lopd_report');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/reports/lopd';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'LOPD';
  $handler->display->display_options['menu']['description'] = 'LOPD Access log';
  $handler->display->display_options['menu']['weight'] = '';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $translatables['LOPD'] = array(
    t('Defaults'),
    t('LOPD Access log'),
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
    t('User'),
    t('LOPD ID'),
    t('Timestamp'),
    t('Operation'),
    t('Authname'),
    t('IP'),
    t('LOPD report page'),
  );

  $views[$view->name] = $view;
  return $views;
}
<?php

/**
 * @file
 * mosaik.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function mosaik_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'mosaik';
  $view->description = '';
  $view->tag = 'Mosaik';
  $view->base_table = 'mosaik';
  $view->human_name = 'Mosaik';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Mosaik';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'view_mosaik' => 'name',
    'layout' => 'layout',
    'path' => 'path',
    'menu' => 'menu',
    'sidebars' => 'sidebars',
    'edit_mosaik' => 'edit_mosaik',
    'clone_mosaik' => 'edit_mosaik',
    'delete_mosaik' => 'edit_mosaik',
    'mosaik_pieces' => 'mosaik_pieces',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' - ',
      'empty_column' => 0,
    ),
    'view_mosaik' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'layout' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'path' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'menu' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'sidebars' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_mosaik' => array(
      'align' => '',
      'separator' => ' | ',
      'empty_column' => 0,
    ),
    'clone_mosaik' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'delete_mosaik' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mosaik_pieces' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No mosaik found.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Mosaik: Mosaik Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'mosaik';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Mosaik';
  $handler->display->display_options['fields']['name']['element_type'] = 'strong';
  /* Field: Mosaik: Link */
  $handler->display->display_options['fields']['view_mosaik']['id'] = 'view_mosaik';
  $handler->display->display_options['fields']['view_mosaik']['table'] = 'mosaik';
  $handler->display->display_options['fields']['view_mosaik']['field'] = 'view_mosaik';
  $handler->display->display_options['fields']['view_mosaik']['label'] = '';
  $handler->display->display_options['fields']['view_mosaik']['element_label_colon'] = FALSE;
  /* Field: Mosaik: Layout */
  $handler->display->display_options['fields']['layout']['id'] = 'layout';
  $handler->display->display_options['fields']['layout']['table'] = 'mosaik';
  $handler->display->display_options['fields']['layout']['field'] = 'layout';
  /* Field: Mosaik: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'mosaik';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  /* Field: Mosaik: Menu */
  $handler->display->display_options['fields']['menu']['id'] = 'menu';
  $handler->display->display_options['fields']['menu']['table'] = 'mosaik';
  $handler->display->display_options['fields']['menu']['field'] = 'menu';
  $handler->display->display_options['fields']['menu']['not'] = 0;
  /* Field: Mosaik: Hide Sidebars */
  $handler->display->display_options['fields']['sidebars']['id'] = 'sidebars';
  $handler->display->display_options['fields']['sidebars']['table'] = 'mosaik';
  $handler->display->display_options['fields']['sidebars']['field'] = 'sidebars';
  $handler->display->display_options['fields']['sidebars']['not'] = 0;
  /* Field: Mosaik: Edit link */
  $handler->display->display_options['fields']['edit_mosaik']['id'] = 'edit_mosaik';
  $handler->display->display_options['fields']['edit_mosaik']['table'] = 'mosaik';
  $handler->display->display_options['fields']['edit_mosaik']['field'] = 'edit_mosaik';
  $handler->display->display_options['fields']['edit_mosaik']['label'] = 'Operations';
  /* Field: Mosaik: Clone link */
  $handler->display->display_options['fields']['clone_mosaik']['id'] = 'clone_mosaik';
  $handler->display->display_options['fields']['clone_mosaik']['table'] = 'mosaik';
  $handler->display->display_options['fields']['clone_mosaik']['field'] = 'clone_mosaik';
  $handler->display->display_options['fields']['clone_mosaik']['label'] = '';
  $handler->display->display_options['fields']['clone_mosaik']['element_label_colon'] = FALSE;
  /* Field: Mosaik: Delete link */
  $handler->display->display_options['fields']['delete_mosaik']['id'] = 'delete_mosaik';
  $handler->display->display_options['fields']['delete_mosaik']['table'] = 'mosaik';
  $handler->display->display_options['fields']['delete_mosaik']['field'] = 'delete_mosaik';
  $handler->display->display_options['fields']['delete_mosaik']['label'] = '';
  $handler->display->display_options['fields']['delete_mosaik']['element_label_colon'] = FALSE;
  /* Field: Mosaik: Pieces */
  $handler->display->display_options['fields']['mosaik_pieces']['id'] = 'mosaik_pieces';
  $handler->display->display_options['fields']['mosaik_pieces']['table'] = 'mosaik';
  $handler->display->display_options['fields']['mosaik_pieces']['field'] = 'mosaik_pieces';
  /* Sort criterion: Mosaik: Mosaik Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'mosaik';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  /* Filter criterion: Mosaik: Mosaik Name */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'mosaik';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['operator'] = 'contains';
  $handler->display->display_options['filters']['name']['group'] = 1;
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Mosaik Name';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
  );
  /* Filter criterion: Mosaik: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'mosaik';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
  );
  /* Filter criterion: Mosaik: Layout */
  $handler->display->display_options['filters']['layout']['id'] = 'layout';
  $handler->display->display_options['filters']['layout']['table'] = 'mosaik';
  $handler->display->display_options['filters']['layout']['field'] = 'layout';
  $handler->display->display_options['filters']['layout']['operator'] = 'contains';
  $handler->display->display_options['filters']['layout']['group'] = 1;
  $handler->display->display_options['filters']['layout']['exposed'] = TRUE;
  $handler->display->display_options['filters']['layout']['expose']['operator_id'] = 'layout_op';
  $handler->display->display_options['filters']['layout']['expose']['label'] = 'Layout';
  $handler->display->display_options['filters']['layout']['expose']['operator'] = 'layout_op';
  $handler->display->display_options['filters']['layout']['expose']['identifier'] = 'layout';
  $handler->display->display_options['filters']['layout']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
  );
  $translatables['mosaik'] = array(
    t('Master'),
    t('Mosaik'),
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
    t('No mosaik found.'),
    t('Layout'),
    t('Path'),
    t('Menu'),
    t('Hide Sidebars'),
    t('Operations'),
    t('Pieces'),
    t('Mosaik Name'),
    t('Title'),
  );
  $export['mosaik'] = $view;

  return $export;
}

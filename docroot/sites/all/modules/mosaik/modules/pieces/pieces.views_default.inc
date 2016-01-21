<?php

/**
 * @file
 * pieces.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function pieces_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'pieces';
  $view->description = '';
  $view->tag = 'Mosaik';
  $view->base_table = 'pieces';
  $view->human_name = 'Pieces';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Pieces';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer mosaik';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Reset';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Offset';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« first';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ previous';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'next ›';
  $handler->display->display_options['pager']['options']['tags']['last'] = 'last »';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = 'name';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No piece was found.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Pieces: Piece Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'pieces';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Piece';
  $handler->display->display_options['fields']['name']['element_type'] = 'strong';
  /* Field: Pieces: Piece Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'pieces';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Pieces: Edit link */
  $handler->display->display_options['fields']['edit_piece']['id'] = 'edit_piece';
  $handler->display->display_options['fields']['edit_piece']['table'] = 'pieces';
  $handler->display->display_options['fields']['edit_piece']['field'] = 'edit_piece';
  $handler->display->display_options['fields']['edit_piece']['label'] = 'Operations';
  $handler->display->display_options['fields']['edit_piece']['element_label_colon'] = FALSE;
  /* Field: Pieces: Delete link */
  $handler->display->display_options['fields']['delete_piece']['id'] = 'delete_piece';
  $handler->display->display_options['fields']['delete_piece']['table'] = 'pieces';
  $handler->display->display_options['fields']['delete_piece']['field'] = 'delete_piece';
  $handler->display->display_options['fields']['delete_piece']['label'] = '';
  $handler->display->display_options['fields']['delete_piece']['element_label_colon'] = FALSE;
  /* Filter criterion: Globale: Combine fields filter */
  $handler->display->display_options['filters']['combine']['id'] = 'combine';
  $handler->display->display_options['filters']['combine']['table'] = 'views';
  $handler->display->display_options['filters']['combine']['field'] = 'combine';
  $handler->display->display_options['filters']['combine']['operator'] = 'contains';
  $handler->display->display_options['filters']['combine']['exposed'] = TRUE;
  $handler->display->display_options['filters']['combine']['expose']['operator_id'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['label'] = 'Piece';
  $handler->display->display_options['filters']['combine']['expose']['operator'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['identifier'] = 'combine';
  $handler->display->display_options['filters']['combine']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
  );
  $handler->display->display_options['filters']['combine']['fields'] = array(
    'name' => 'name',
    'title' => 'title',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'title' => 'name',
    'edit_piece' => 'edit_piece',
    'delete_piece' => 'edit_piece',
  );
  $handler->display->display_options['style_options']['default'] = 'name';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' - ',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_piece' => array(
      'align' => '',
      'separator' => ' | ',
      'empty_column' => 0,
    ),
    'delete_piece' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = 'admin/structure/mosaik/pieces';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Pieces';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 1;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'pieces_block');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['offset_label'] = 'Offset';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« first';
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ previous';
  $handler->display->display_options['pager']['options']['tags']['next'] = 'next ›';
  $handler->display->display_options['pager']['options']['tags']['last'] = 'last »';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'title' => 'name',
    'edit_piece' => 'edit_piece',
    'delete_piece' => 'edit_piece',
  );
  $handler->display->display_options['style_options']['default'] = 'name';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' - ',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_piece' => array(
      'align' => '',
      'separator' => ' | ',
      'empty_column' => 0,
    ),
    'delete_piece' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Pieces: Piece Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'pieces';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Piece';
  $handler->display->display_options['fields']['name']['element_type'] = 'strong';
  /* Field: Pieces: Piece Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'pieces';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Pieces: Edit link */
  $handler->display->display_options['fields']['edit_piece']['id'] = 'edit_piece';
  $handler->display->display_options['fields']['edit_piece']['table'] = 'pieces';
  $handler->display->display_options['fields']['edit_piece']['field'] = 'edit_piece';
  $handler->display->display_options['fields']['edit_piece']['label'] = 'Operations';
  $handler->display->display_options['fields']['edit_piece']['element_label_colon'] = FALSE;
  /* Field: Pieces: Delete link */
  $handler->display->display_options['fields']['delete_piece']['id'] = 'delete_piece';
  $handler->display->display_options['fields']['delete_piece']['table'] = 'pieces';
  $handler->display->display_options['fields']['delete_piece']['field'] = 'delete_piece';
  $handler->display->display_options['fields']['delete_piece']['label'] = '';
  $handler->display->display_options['fields']['delete_piece']['element_label_colon'] = FALSE;
  $translatables['pieces'] = array(
    t('Master'),
    t('Pieces'),
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
    t('No piece was found.'),
    t('Piece'),
    t('Operations'),
    t('Page'),
    t('Block'),
  );

  $export['pieces'] = $view;

  return $export;
}

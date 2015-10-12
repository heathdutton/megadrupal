<?php
/**
 * Implementation of hook_views_default_views().
 */
function entity_email_views_default_views() {

  $view = new view();
  $view->name = 'entity_email_type';
  $view->description = 'Display a list of entity emails.';
  $view->tag = 'default';
  $view->base_table = 'entity_email_type';
  $view->human_name = 'Entity Email Type';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'created' => 'created',
    'label' => 'label',
    'name' => 'name',
    'tags' => 'tags',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'label' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'tags' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Email type: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'entity_email_type';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  /* Field: Email type: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'entity_email_type';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  /* Field: Email type: Date created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'entity_email_type';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  /* Field: Email type: Tags */
  $handler->display->display_options['fields']['tags']['id'] = 'tags';
  $handler->display->display_options['fields']['tags']['table'] = 'entity_email_type';
  $handler->display->display_options['fields']['tags']['field'] = 'tags';
  /* Field: Edit */
  $handler->display->display_options['fields']['edit']['id'] = 'edit';
  $handler->display->display_options['fields']['edit']['table'] = 'views';
  $handler->display->display_options['fields']['edit']['field'] = 'nothing';
  $handler->display->display_options['fields']['edit']['ui_name'] = 'Edit';
  $handler->display->display_options['fields']['edit']['label'] = '';
  $handler->display->display_options['fields']['edit']['alter']['text'] = '<a href=\'/admin/structure/entity_email_type/manage/[name]\'>edit</a>';
  $handler->display->display_options['fields']['edit']['element_label_colon'] = FALSE;
  /* Field: Clone */
  $handler->display->display_options['fields']['clone']['id'] = 'clone';
  $handler->display->display_options['fields']['clone']['table'] = 'views';
  $handler->display->display_options['fields']['clone']['field'] = 'nothing';
  $handler->display->display_options['fields']['clone']['ui_name'] = 'Clone';
  $handler->display->display_options['fields']['clone']['label'] = '';
  $handler->display->display_options['fields']['clone']['alter']['text'] = '<a href=\'/admin/structure/entity_email_type/manage/[name]/clone\'>clone</a>';
  $handler->display->display_options['fields']['clone']['element_label_colon'] = FALSE;
  /* Field: Delete */
  $handler->display->display_options['fields']['delete']['id'] = 'delete';
  $handler->display->display_options['fields']['delete']['table'] = 'views';
  $handler->display->display_options['fields']['delete']['field'] = 'nothing';
  $handler->display->display_options['fields']['delete']['ui_name'] = 'Delete';
  $handler->display->display_options['fields']['delete']['label'] = '';
  $handler->display->display_options['fields']['delete']['alter']['text'] = '<a href=\'/admin/structure/entity_email_type/manage/[name]/delete\'>delete</a>';
  $handler->display->display_options['fields']['delete']['element_label_colon'] = FALSE;
  /* Field: Export */
  $handler->display->display_options['fields']['export']['id'] = 'export';
  $handler->display->display_options['fields']['export']['table'] = 'views';
  $handler->display->display_options['fields']['export']['field'] = 'nothing';
  $handler->display->display_options['fields']['export']['ui_name'] = 'Export';
  $handler->display->display_options['fields']['export']['label'] = '';
  $handler->display->display_options['fields']['export']['alter']['text'] = '<a href=\'/admin/structure/entity_email_type/manage/[name]/export\'>export</a>';
  $handler->display->display_options['fields']['export']['element_label_colon'] = FALSE;

  $views[$view->name] = $view;

  return $views;
}

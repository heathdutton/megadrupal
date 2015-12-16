<?php

/**
 * Implements hook_views_default_views().
 */
function translatable_content_panes_views_default_views() {
  $view = new view();
  $view->name = 'translatable_content_panes';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'translatable_content_pane';
  $view->human_name = 'Translatable Content Panes';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Translatable Content Panes';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'edit translatable content panes';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'admin_title' => 'admin_title',
    'edit' => 'edit',
  );
  $handler->display->display_options['style_options']['default'] = 'admin_title';
  $handler->display->display_options['style_options']['info'] = array(
    'admin_title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Translatable content panes: Admin title */
  $handler->display->display_options['fields']['admin_title']['id'] = 'admin_title';
  $handler->display->display_options['fields']['admin_title']['table'] = 'translatable_content_pane';
  $handler->display->display_options['fields']['admin_title']['field'] = 'admin_title';
  $handler->display->display_options['fields']['admin_title']['label'] = 'Translatable content pane';
  /* Field: Translatable content panes: Edit link */
  $handler->display->display_options['fields']['edit']['id'] = 'edit';
  $handler->display->display_options['fields']['edit']['table'] = 'translatable_content_pane';
  $handler->display->display_options['fields']['edit']['field'] = 'edit';
  $handler->display->display_options['fields']['edit']['label'] = 'Operations';
  /* Sort criterion: Translatable content panes: Admin title */
  $handler->display->display_options['sorts']['admin_title']['id'] = 'admin_title';
  $handler->display->display_options['sorts']['admin_title']['table'] = 'translatable_content_pane';
  $handler->display->display_options['sorts']['admin_title']['field'] = 'admin_title';
  /* Filter criterion: Translatable content panes: Code content */
  $handler->display->display_options['filters']['code_content']['id'] = 'code_content';
  $handler->display->display_options['filters']['code_content']['table'] = 'translatable_content_pane';
  $handler->display->display_options['filters']['code_content']['field'] = 'code_content';
  $handler->display->display_options['filters']['code_content']['value'] = '0';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/translatable-content-panes';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Translatable content panes';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
  $translatables['translatable_content_panes'] = array(
    t('Master'),
    t('Translatable Content Panes'),
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
    t('Translatable content pane'),
    t('Operations'),
    t('Page'),
  );

  return array(
    $view->name => $view,
  );
}

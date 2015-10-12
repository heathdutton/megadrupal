<?php

/**
 * Views for the most viewed conent.
 */
/**
 * Implements hook_views_default_views().
 */
function most_viewed_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'most_viewed_content';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'most_viewed_hits';
  $view->human_name = 'Most viewed content';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Most viewed content';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access most viewed settings';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: MVC entities: MVC ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'most_viewed_hits';
  $handler->display->display_options['fields']['id']['field'] = 'id';

  /* Display: Entity statistics (all time) */
  $handler = $view->new_display('page', 'Entity statistics (all time)', 'mvc_entity_stats');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Entity statistics';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['query'] = FALSE;
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'entity_id_1' => 'entity_id_1',
    'entity_type' => 'entity_type',
    'bundle' => 'bundle',
    'entity_id' => 'entity_id',
  );
  $handler->display->display_options['style_options']['default'] = 'entity_id';
  $handler->display->display_options['style_options']['info'] = array(
    'entity_id_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'bundle' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: MVC entities: Entity id */
  $handler->display->display_options['fields']['entity_id_1']['id'] = 'entity_id_1';
  $handler->display->display_options['fields']['entity_id_1']['table'] = 'most_viewed_hits';
  $handler->display->display_options['fields']['entity_id_1']['field'] = 'entity_id';
  /* Field: MVC entities: Entity type */
  $handler->display->display_options['fields']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['fields']['entity_type']['table'] = 'most_viewed_hits';
  $handler->display->display_options['fields']['entity_type']['field'] = 'entity_type';
  /* Field: MVC entities: Bundle */
  $handler->display->display_options['fields']['bundle']['id'] = 'bundle';
  $handler->display->display_options['fields']['bundle']['table'] = 'most_viewed_hits';
  $handler->display->display_options['fields']['bundle']['field'] = 'bundle';
  /* Field: MVC entities: Entity id */
  $handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['table'] = 'most_viewed_hits';
  $handler->display->display_options['fields']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['entity_id']['label'] = 'Number of views';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: MVC entities: Entity type */
  $handler->display->display_options['filters']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['table'] = 'most_viewed_hits';
  $handler->display->display_options['filters']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['group'] = 1;
  $handler->display->display_options['filters']['entity_type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_type']['expose']['operator_id'] = 'entity_type_op';
  $handler->display->display_options['filters']['entity_type']['expose']['label'] = 'Entity type';
  $handler->display->display_options['filters']['entity_type']['expose']['operator'] = 'entity_type_op';
  $handler->display->display_options['filters']['entity_type']['expose']['identifier'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: MVC entities: Bundle */
  $handler->display->display_options['filters']['bundle']['id'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['table'] = 'most_viewed_hits';
  $handler->display->display_options['filters']['bundle']['field'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['group'] = 1;
  $handler->display->display_options['filters']['bundle']['exposed'] = TRUE;
  $handler->display->display_options['filters']['bundle']['expose']['operator_id'] = 'bundle_op';
  $handler->display->display_options['filters']['bundle']['expose']['label'] = 'Bundle';
  $handler->display->display_options['filters']['bundle']['expose']['operator'] = 'bundle_op';
  $handler->display->display_options['filters']['bundle']['expose']['identifier'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['bundle']['group_info']['label'] = 'Bundle';
  $handler->display->display_options['filters']['bundle']['group_info']['identifier'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['group_info']['default_group'] = '1';
  $handler->display->display_options['filters']['bundle']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Page',
      'operator' => '=',
      'value' => 'page',
    ),
    2 => array(
      'title' => '',
      'operator' => '=',
      'value' => '',
    ),
    3 => array(
      'title' => '',
      'operator' => '=',
      'value' => '',
    ),
  );
  /* Filter criterion: MVC entities: Entity id */
  $handler->display->display_options['filters']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['filters']['entity_id']['table'] = 'most_viewed_hits';
  $handler->display->display_options['filters']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['filters']['entity_id']['group'] = 1;
  $handler->display->display_options['filters']['entity_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_id']['expose']['operator_id'] = 'entity_id_op';
  $handler->display->display_options['filters']['entity_id']['expose']['label'] = 'Entity id';
  $handler->display->display_options['filters']['entity_id']['expose']['operator'] = 'entity_id_op';
  $handler->display->display_options['filters']['entity_id']['expose']['identifier'] = 'entity_id';
  $handler->display->display_options['filters']['entity_id']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['path'] = 'admin/reports/mvc/entity-stats';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'MVC Entity statistics';
  $handler->display->display_options['menu']['weight'] = '-1';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Most viewed content';
  $handler->display->display_options['tab_options']['description'] = 'Most viewed content statictics';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  /* Display: Hits */
  $handler = $view->new_display('page', 'Hits', 'mvc_hits');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Hits';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'entity_id' => 'entity_id',
    'entity_type' => 'entity_type',
    'bundle' => 'bundle',
    'created' => 'created',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'entity_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'bundle' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: MVC entities: Entity id */
  $handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['table'] = 'most_viewed_hits';
  $handler->display->display_options['fields']['entity_id']['field'] = 'entity_id';
  /* Field: MVC entities: Entity type */
  $handler->display->display_options['fields']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['fields']['entity_type']['table'] = 'most_viewed_hits';
  $handler->display->display_options['fields']['entity_type']['field'] = 'entity_type';
  /* Field: MVC entities: Bundle */
  $handler->display->display_options['fields']['bundle']['id'] = 'bundle';
  $handler->display->display_options['fields']['bundle']['table'] = 'most_viewed_hits';
  $handler->display->display_options['fields']['bundle']['field'] = 'bundle';
  /* Field: MVC entities: Date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'most_viewed_hits';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Hit\'s date';
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'd-m-Y H:i:s';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: MVC entities: MVC ID */
  $handler->display->display_options['sorts']['id']['id'] = 'id';
  $handler->display->display_options['sorts']['id']['table'] = 'most_viewed_hits';
  $handler->display->display_options['sorts']['id']['field'] = 'id';
  $handler->display->display_options['sorts']['id']['order'] = 'DESC';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: MVC entities: Entity type */
  $handler->display->display_options['filters']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['table'] = 'most_viewed_hits';
  $handler->display->display_options['filters']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['group'] = 1;
  $handler->display->display_options['filters']['entity_type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_type']['expose']['operator_id'] = 'entity_type_op';
  $handler->display->display_options['filters']['entity_type']['expose']['label'] = 'Entity type';
  $handler->display->display_options['filters']['entity_type']['expose']['operator'] = 'entity_type_op';
  $handler->display->display_options['filters']['entity_type']['expose']['identifier'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: MVC entities: Bundle */
  $handler->display->display_options['filters']['bundle']['id'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['table'] = 'most_viewed_hits';
  $handler->display->display_options['filters']['bundle']['field'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['group'] = 1;
  $handler->display->display_options['filters']['bundle']['exposed'] = TRUE;
  $handler->display->display_options['filters']['bundle']['expose']['operator_id'] = 'bundle_op';
  $handler->display->display_options['filters']['bundle']['expose']['label'] = 'Bundle';
  $handler->display->display_options['filters']['bundle']['expose']['operator'] = 'bundle_op';
  $handler->display->display_options['filters']['bundle']['expose']['identifier'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['bundle']['group_info']['label'] = 'Bundle';
  $handler->display->display_options['filters']['bundle']['group_info']['identifier'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['group_info']['default_group'] = '1';
  $handler->display->display_options['filters']['bundle']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Page',
      'operator' => '=',
      'value' => 'page',
    ),
    2 => array(
      'title' => '',
      'operator' => '=',
      'value' => '',
    ),
    3 => array(
      'title' => '',
      'operator' => '=',
      'value' => '',
    ),
  );
  /* Filter criterion: MVC entities: Entity id */
  $handler->display->display_options['filters']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['filters']['entity_id']['table'] = 'most_viewed_hits';
  $handler->display->display_options['filters']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['filters']['entity_id']['group'] = 1;
  $handler->display->display_options['filters']['entity_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_id']['expose']['operator_id'] = 'entity_id_op';
  $handler->display->display_options['filters']['entity_id']['expose']['label'] = 'Entity id';
  $handler->display->display_options['filters']['entity_id']['expose']['operator'] = 'entity_id_op';
  $handler->display->display_options['filters']['entity_id']['expose']['identifier'] = 'entity_id';
  $handler->display->display_options['filters']['entity_id']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['path'] = 'admin/reports/mvc/hits';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'MVC hits';
  $handler->display->display_options['menu']['description'] = 'Each hit of page';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Most viewed statistics';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  /* Display: Bundle statistics (all time) */
  $handler = $view->new_display('page', 'Bundle statistics (all time)', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Bundle statistics';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['query'] = FALSE;
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'entity_id' => 'entity_id',
    'entity_type' => 'entity_type',
    'bundle' => 'bundle',
  );
  $handler->display->display_options['style_options']['default'] = 'entity_id';
  $handler->display->display_options['style_options']['info'] = array(
    'entity_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'bundle' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: MVC entities: Entity id */
  $handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['table'] = 'most_viewed_hits';
  $handler->display->display_options['fields']['entity_id']['field'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['entity_id']['label'] = 'Number of views';
  /* Field: MVC entities: Entity type */
  $handler->display->display_options['fields']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['fields']['entity_type']['table'] = 'most_viewed_hits';
  $handler->display->display_options['fields']['entity_type']['field'] = 'entity_type';
  /* Field: MVC entities: Bundle */
  $handler->display->display_options['fields']['bundle']['id'] = 'bundle';
  $handler->display->display_options['fields']['bundle']['table'] = 'most_viewed_hits';
  $handler->display->display_options['fields']['bundle']['field'] = 'bundle';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: MVC entities: Entity type */
  $handler->display->display_options['filters']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['table'] = 'most_viewed_hits';
  $handler->display->display_options['filters']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['group'] = 1;
  $handler->display->display_options['filters']['entity_type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['entity_type']['expose']['operator_id'] = 'entity_type_op';
  $handler->display->display_options['filters']['entity_type']['expose']['label'] = 'Entity type';
  $handler->display->display_options['filters']['entity_type']['expose']['operator'] = 'entity_type_op';
  $handler->display->display_options['filters']['entity_type']['expose']['identifier'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: MVC entities: Bundle */
  $handler->display->display_options['filters']['bundle']['id'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['table'] = 'most_viewed_hits';
  $handler->display->display_options['filters']['bundle']['field'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['group'] = 1;
  $handler->display->display_options['filters']['bundle']['exposed'] = TRUE;
  $handler->display->display_options['filters']['bundle']['expose']['operator_id'] = 'bundle_op';
  $handler->display->display_options['filters']['bundle']['expose']['label'] = 'Bundle';
  $handler->display->display_options['filters']['bundle']['expose']['operator'] = 'bundle_op';
  $handler->display->display_options['filters']['bundle']['expose']['identifier'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['bundle']['group_info']['label'] = 'Bundle';
  $handler->display->display_options['filters']['bundle']['group_info']['identifier'] = 'bundle';
  $handler->display->display_options['filters']['bundle']['group_info']['default_group'] = '1';
  $handler->display->display_options['filters']['bundle']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Page',
      'operator' => '=',
      'value' => 'page',
    ),
    2 => array(
      'title' => '',
      'operator' => '=',
      'value' => '',
    ),
    3 => array(
      'title' => '',
      'operator' => '=',
      'value' => '',
    ),
  );
  $handler->display->display_options['path'] = 'admin/reports/mvc/bundle-stats';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'MVC Bundle statistics';
  $handler->display->display_options['menu']['weight'] = '';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Most viewed content';
  $handler->display->display_options['tab_options']['description'] = 'Most viewed content statictics';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  $views[$view->name] = $view;

  return $views;
}

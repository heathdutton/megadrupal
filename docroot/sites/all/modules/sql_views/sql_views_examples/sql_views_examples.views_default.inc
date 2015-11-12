<?php

/**
 * @file
 * Default views.
 */

/**
 * Implements hook_views_default_views().
 */
function sql_views_examples_views_default_views() {

  // Authors view.
  $view = new view();
  $view->name = 'sql_views_authors';
  $view->description = '';
  $view->tag = 'sql_views_examples';
  $view->base_table = 'sql_views_examples_authors';
  $view->human_name = 'Authors';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Authors';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer users';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'mail' => 'mail',
    'count' => 'count',
    'percentage' => 'percentage',
    'uid' => 'uid',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mail' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'count' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'percentage' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'uid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = 'No results were found.';
  /* Field: Authors (SQL View): Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'sql_views_examples_authors';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = 'ID';
  /* Field: Authors (SQL View): Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'sql_views_examples_authors';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  /* Field: Authors (SQL View): count */
  $handler->display->display_options['fields']['count']['id'] = 'count';
  $handler->display->display_options['fields']['count']['table'] = 'sql_views_examples_authors';
  $handler->display->display_options['fields']['count']['field'] = 'count';
  $handler->display->display_options['fields']['count']['label'] = 'Total';
  $handler->display->display_options['fields']['count']['separator'] = '';
  /* Field: Authors (SQL View): percentage */
  $handler->display->display_options['fields']['percentage']['id'] = 'percentage';
  $handler->display->display_options['fields']['percentage']['table'] = 'sql_views_examples_authors';
  $handler->display->display_options['fields']['percentage']['field'] = 'percentage';
  $handler->display->display_options['fields']['percentage']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['percentage']['precision'] = '2';
  $handler->display->display_options['fields']['percentage']['separator'] = '';
  /* Sort criterion: Authors (SQL View): count */
  $handler->display->display_options['sorts']['count']['id'] = 'count';
  $handler->display->display_options['sorts']['count']['table'] = 'sql_views_examples_authors';
  $handler->display->display_options['sorts']['count']['field'] = 'count';
  $handler->display->display_options['sorts']['count']['order'] = 'DESC';
  /* Sort criterion: Authors (SQL View): Uid */
  $handler->display->display_options['sorts']['uid']['id'] = 'uid';
  $handler->display->display_options['sorts']['uid']['table'] = 'sql_views_examples_authors';
  $handler->display->display_options['sorts']['uid']['field'] = 'uid';
  /* Filter criterion: Count greater than */
  $handler->display->display_options['filters']['count']['id'] = 'count';
  $handler->display->display_options['filters']['count']['table'] = 'sql_views_examples_authors';
  $handler->display->display_options['filters']['count']['field'] = 'count';
  $handler->display->display_options['filters']['count']['ui_name'] = 'Count greater than';
  $handler->display->display_options['filters']['count']['operator'] = '>';
  $handler->display->display_options['filters']['count']['exposed'] = TRUE;
  $handler->display->display_options['filters']['count']['expose']['operator_id'] = 'count_op';
  $handler->display->display_options['filters']['count']['expose']['label'] = 'Count greater than';
  $handler->display->display_options['filters']['count']['expose']['operator'] = 'count_op';
  $handler->display->display_options['filters']['count']['expose']['identifier'] = 'count_greater';
  $handler->display->display_options['filters']['count']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Count less than */
  $handler->display->display_options['filters']['count_1']['id'] = 'count_1';
  $handler->display->display_options['filters']['count_1']['table'] = 'sql_views_examples_authors';
  $handler->display->display_options['filters']['count_1']['field'] = 'count';
  $handler->display->display_options['filters']['count_1']['ui_name'] = 'Count less than';
  $handler->display->display_options['filters']['count_1']['operator'] = '<';
  $handler->display->display_options['filters']['count_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['count_1']['expose']['operator_id'] = 'count_1_op';
  $handler->display->display_options['filters']['count_1']['expose']['label'] = 'Count less than';
  $handler->display->display_options['filters']['count_1']['expose']['operator'] = 'count_1_op';
  $handler->display->display_options['filters']['count_1']['expose']['identifier'] = 'count_less';
  $handler->display->display_options['filters']['count_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/authors';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Authors';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $views[$view->name] = $view;

  $view = new view();
  $view->name = 'sql_views_example_fields';
  $view->description = '';
  $view->tag = 'sql_views_examples';
  $view->base_table = 'sql_views_examples_fields';
  $view->human_name = 'Fields';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Fields';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer site configuration';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_name' => 'field_name',
    'type' => 'type',
    'module' => 'module',
    'records' => 'records',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'module' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'records' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = 'No results were found.';
  /* Field: Fields (SQL View): field_name */
  $handler->display->display_options['fields']['field_name']['id'] = 'field_name';
  $handler->display->display_options['fields']['field_name']['table'] = 'sql_views_examples_fields';
  $handler->display->display_options['fields']['field_name']['field'] = 'field_name';
  /* Field: Fields (SQL View): type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'sql_views_examples_fields';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Fields (SQL View): module */
  $handler->display->display_options['fields']['module']['id'] = 'module';
  $handler->display->display_options['fields']['module']['table'] = 'sql_views_examples_fields';
  $handler->display->display_options['fields']['module']['field'] = 'module';
  /* Field: Fields (SQL View): records */
  $handler->display->display_options['fields']['records']['id'] = 'records';
  $handler->display->display_options['fields']['records']['table'] = 'sql_views_examples_fields';
  $handler->display->display_options['fields']['records']['field'] = 'records';
  /* Sort criterion: Fields (SQL View): field_name */
  $handler->display->display_options['sorts']['field_name']['id'] = 'field_name';
  $handler->display->display_options['sorts']['field_name']['table'] = 'sql_views_examples_fields';
  $handler->display->display_options['sorts']['field_name']['field'] = 'field_name';
  /* Filter criterion: Fields (SQL View): field_name */
  $handler->display->display_options['filters']['field_name']['id'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['table'] = 'sql_views_examples_fields';
  $handler->display->display_options['filters']['field_name']['field'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_name']['group'] = 1;
  $handler->display->display_options['filters']['field_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_name']['expose']['operator_id'] = 'field_name_op';
  $handler->display->display_options['filters']['field_name']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['field_name']['expose']['operator'] = 'field_name_op';
  $handler->display->display_options['filters']['field_name']['expose']['identifier'] = 'field_name';
  $handler->display->display_options['filters']['field_name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Fields (SQL View): type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'sql_views_examples_fields';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['operator'] = 'contains';
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Fields (SQL View): module */
  $handler->display->display_options['filters']['module']['id'] = 'module';
  $handler->display->display_options['filters']['module']['table'] = 'sql_views_examples_fields';
  $handler->display->display_options['filters']['module']['field'] = 'module';
  $handler->display->display_options['filters']['module']['operator'] = 'contains';
  $handler->display->display_options['filters']['module']['group'] = 1;
  $handler->display->display_options['filters']['module']['exposed'] = TRUE;
  $handler->display->display_options['filters']['module']['expose']['operator_id'] = 'module_op';
  $handler->display->display_options['filters']['module']['expose']['label'] = 'Module';
  $handler->display->display_options['filters']['module']['expose']['operator'] = 'module_op';
  $handler->display->display_options['filters']['module']['expose']['identifier'] = 'module';
  $handler->display->display_options['filters']['module']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/fields';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Fields';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $views[$view->name] = $view;

  // Content index.
  $view = new view();
  $view->name = 'content_index';
  $view->description = '';
  $view->tag = 'sql_views_examples';
  $view->base_table = 'sql_views_examples_content_index';
  $view->human_name = 'Content index';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Content index';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['style_options']['columns'] = '12';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Footer: Global: Result summary */
  $handler->display->display_options['footer']['result']['id'] = 'result';
  $handler->display->display_options['footer']['result']['table'] = 'views';
  $handler->display->display_options['footer']['result']['field'] = 'result';
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = 'No results were found.';
  /* Field: Content index (SQL View): Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'sql_views_examples_content_index';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = 'ID';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'node_nid';
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['relationship'] = 'node_nid';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['relationship'] = 'node_nid';
  $handler->display->display_options['fields']['created']['label'] = 'Created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'short';
  /* Contextual filter: Content index (SQL View): letter_1 */
  $handler->display->display_options['arguments']['letter_1']['id'] = 'letter_1';
  $handler->display->display_options['arguments']['letter_1']['table'] = 'sql_views_examples_content_index';
  $handler->display->display_options['arguments']['letter_1']['field'] = 'letter_1';
  $handler->display->display_options['arguments']['letter_1']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['letter_1']['title'] = 'Content - %1';
  $handler->display->display_options['arguments']['letter_1']['breadcrumb_enable'] = TRUE;
  $handler->display->display_options['arguments']['letter_1']['breadcrumb'] = '%1';
  $handler->display->display_options['arguments']['letter_1']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['letter_1']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['letter_1']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['letter_1']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['letter_1']['limit'] = '0';
  /* Contextual filter: Content index (SQL View): letter_2 */
  $handler->display->display_options['arguments']['letter_2']['id'] = 'letter_2';
  $handler->display->display_options['arguments']['letter_2']['table'] = 'sql_views_examples_content_index';
  $handler->display->display_options['arguments']['letter_2']['field'] = 'letter_2';
  $handler->display->display_options['arguments']['letter_2']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['letter_2']['title'] = 'Content - %1%2';
  $handler->display->display_options['arguments']['letter_2']['breadcrumb_enable'] = TRUE;
  $handler->display->display_options['arguments']['letter_2']['breadcrumb'] = '%2';
  $handler->display->display_options['arguments']['letter_2']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['letter_2']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['letter_2']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['letter_2']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['letter_2']['limit'] = '0';
  /* Contextual filter: Content index (SQL View): letter_3 */
  $handler->display->display_options['arguments']['letter_3']['id'] = 'letter_3';
  $handler->display->display_options['arguments']['letter_3']['table'] = 'sql_views_examples_content_index';
  $handler->display->display_options['arguments']['letter_3']['field'] = 'letter_3';
  $handler->display->display_options['arguments']['letter_3']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['letter_3']['title'] = 'Content - %1%2%3';
  $handler->display->display_options['arguments']['letter_3']['breadcrumb_enable'] = TRUE;
  $handler->display->display_options['arguments']['letter_3']['breadcrumb'] = '%3';
  $handler->display->display_options['arguments']['letter_3']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['letter_3']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['letter_3']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['letter_3']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['letter_3']['limit'] = '0';
  /* Filter criterion: Content index (SQL View): letter_3 */
  $handler->display->display_options['filters']['letter_3']['id'] = 'letter_3';
  $handler->display->display_options['filters']['letter_3']['table'] = 'sql_views_examples_content_index';
  $handler->display->display_options['filters']['letter_3']['field'] = 'letter_3';
  $handler->display->display_options['filters']['letter_3']['operator'] = 'longerthan';
  $handler->display->display_options['filters']['letter_3']['value'] = '0';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content index (SQL View): Content */
  $handler->display->display_options['relationships']['node_nid']['id'] = 'node_nid';
  $handler->display->display_options['relationships']['node_nid']['table'] = 'sql_views_examples_content_index';
  $handler->display->display_options['relationships']['node_nid']['field'] = 'node_nid';
  $handler->display->display_options['path'] = 'content';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Content index';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Letter 1 */
  $handler = $view->new_display('attachment', 'Letter 1', 'attachment_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'First letter';
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['style_options']['columns'] = '12';
  $handler->display->display_options['style_options']['fill_single_line'] = FALSE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = 'No results were found.';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content index (SQL View): letter_1 */
  $handler->display->display_options['fields']['letter_1']['id'] = 'letter_1';
  $handler->display->display_options['fields']['letter_1']['table'] = 'sql_views_examples_content_index';
  $handler->display->display_options['fields']['letter_1']['field'] = 'letter_1';
  $handler->display->display_options['fields']['letter_1']['label'] = '';
  $handler->display->display_options['fields']['letter_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['letter_1']['element_label_colon'] = FALSE;
  /* Field: COUNT(Content index (SQL View): Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'sql_views_examples_content_index';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['text'] = '[letter_1] ([nid])';
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['path'] = 'content/[letter_1]';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );

  /* Display: Letter 2 */
  $handler = $view->new_display('attachment', 'Letter 2', 'attachment_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Second letter';
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content index (SQL View): letter_2 */
  $handler->display->display_options['fields']['letter_2']['id'] = 'letter_2';
  $handler->display->display_options['fields']['letter_2']['table'] = 'sql_views_examples_content_index';
  $handler->display->display_options['fields']['letter_2']['field'] = 'letter_2';
  $handler->display->display_options['fields']['letter_2']['label'] = '';
  $handler->display->display_options['fields']['letter_2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['letter_2']['element_label_colon'] = FALSE;
  /* Field: COUNT(Content index (SQL View): Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'sql_views_examples_content_index';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['text'] = '[letter_2] ([nid])';
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['path'] = 'content/!1/[letter_2]';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content index (SQL View): letter_1 */
  $handler->display->display_options['arguments']['letter_1']['id'] = 'letter_1';
  $handler->display->display_options['arguments']['letter_1']['table'] = 'sql_views_examples_content_index';
  $handler->display->display_options['arguments']['letter_1']['field'] = 'letter_1';
  $handler->display->display_options['arguments']['letter_1']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['letter_1']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['letter_1']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['letter_1']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['letter_1']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['letter_1']['limit'] = '0';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
  );

  /* Display: Letter 3 */
  $handler = $view->new_display('attachment', 'Letter 3', 'attachment_3');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Third letter';
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content index (SQL View): letter_3 */
  $handler->display->display_options['fields']['letter_3']['id'] = 'letter_3';
  $handler->display->display_options['fields']['letter_3']['table'] = 'sql_views_examples_content_index';
  $handler->display->display_options['fields']['letter_3']['field'] = 'letter_3';
  $handler->display->display_options['fields']['letter_3']['label'] = '';
  $handler->display->display_options['fields']['letter_3']['exclude'] = TRUE;
  $handler->display->display_options['fields']['letter_3']['element_label_colon'] = FALSE;
  /* Field: COUNT(Content index (SQL View): Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'sql_views_examples_content_index';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['text'] = '[letter_3] ([nid])';
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['path'] = 'content/!1/!2/[letter_3]';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content index (SQL View): letter_1 */
  $handler->display->display_options['arguments']['letter_1']['id'] = 'letter_1';
  $handler->display->display_options['arguments']['letter_1']['table'] = 'sql_views_examples_content_index';
  $handler->display->display_options['arguments']['letter_1']['field'] = 'letter_1';
  $handler->display->display_options['arguments']['letter_1']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['letter_1']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['letter_1']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['letter_1']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['letter_1']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['letter_1']['limit'] = '0';
  /* Contextual filter: Content index (SQL View): letter_2 */
  $handler->display->display_options['arguments']['letter_2']['id'] = 'letter_2';
  $handler->display->display_options['arguments']['letter_2']['table'] = 'sql_views_examples_content_index';
  $handler->display->display_options['arguments']['letter_2']['field'] = 'letter_2';
  $handler->display->display_options['arguments']['letter_2']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['letter_2']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['letter_2']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['letter_2']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['letter_2']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['letter_2']['limit'] = '0';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );

  $views[$view->name] = $view;

  return $views;
}

<?php

/**
 * Implements hook_views_default_views().
 */
function vsf_management_views_default_views() {
  $export = array();



  $view = new view();
  $view->name = 'vsf_mgt_node_file_usage';
  $view->description = '';
  $view->tag = 'vsf_management';
  $view->base_table = 'file_managed';
  $view->human_name = 'Node file usage';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'File usage';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer nodes';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'filename' => 'filename',
    'image' => 'image',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'filename' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'image' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: File Usage: Content */
  $handler->display->display_options['relationships']['file_to_node']['id'] = 'file_to_node';
  $handler->display->display_options['relationships']['file_to_node']['table'] = 'file_usage';
  $handler->display->display_options['relationships']['file_to_node']['field'] = 'file_to_node';
  $handler->display->display_options['relationships']['file_to_node']['required'] = TRUE;
  /* Field: File: File as image */
  $handler->display->display_options['fields']['image']['id'] = 'image';
  $handler->display->display_options['fields']['image']['table'] = 'file_managed';
  $handler->display->display_options['fields']['image']['field'] = 'image';
  $handler->display->display_options['fields']['image']['label'] = '';
  $handler->display->display_options['fields']['image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['image']['image_style'] = 'vsf_management_thumbnail';
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  $handler->display->display_options['fields']['filename']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['filename']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['filename']['link_to_file'] = TRUE;
  /* Field: File Usage: Use count */
  $handler->display->display_options['fields']['count']['id'] = 'count';
  $handler->display->display_options['fields']['count']['table'] = 'file_usage';
  $handler->display->display_options['fields']['count']['field'] = 'count';
  $handler->display->display_options['fields']['count']['separator'] = '';
  /* Field: File Usage: Module */
  $handler->display->display_options['fields']['module']['id'] = 'module';
  $handler->display->display_options['fields']['module']['table'] = 'file_usage';
  $handler->display->display_options['fields']['module']['field'] = 'module';
  /* Sort criterion: File Usage: Module */
  $handler->display->display_options['sorts']['module']['id'] = 'module';
  $handler->display->display_options['sorts']['module']['table'] = 'file_usage';
  $handler->display->display_options['sorts']['module']['field'] = 'module';
  /* Sort criterion: File: File ID */
  $handler->display->display_options['sorts']['fid']['id'] = 'fid';
  $handler->display->display_options['sorts']['fid']['table'] = 'file_managed';
  $handler->display->display_options['sorts']['fid']['field'] = 'fid';
  /* Contextual filter: File Usage: Entity ID */
  $handler->display->display_options['arguments']['id']['id'] = 'id';
  $handler->display->display_options['arguments']['id']['table'] = 'file_usage';
  $handler->display->display_options['arguments']['id']['field'] = 'id';
  $handler->display->display_options['arguments']['id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: File Usage: Entity type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'file_usage';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = 'node';
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Entity type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'node/%/files';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'File usage';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $export[$view->name] = $view;







  $view = new view();
  $view->name = 'vsf_mgt_files';
  $view->description = '';
  $view->tag = 'vsf_management';
  $view->base_table = 'file_managed';
  $view->human_name = 'Admin files';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Files';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer nodes';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'uri' => 'uri',
    'fid' => 'fid',
    'views_bulk_operations' => 'views_bulk_operations',
    'image' => 'image',
    'filename' => 'filename',
    'status' => 'status',
    'filesize' => 'filesize',
    'timestamp' => 'timestamp',
    'count' => 'count',
  );
  $handler->display->display_options['style_options']['default'] = 'timestamp';
  $handler->display->display_options['style_options']['info'] = array(
    'uri' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'fid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'views_bulk_operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'image' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'filename' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'filesize' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'count' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: File: Path */
  $handler->display->display_options['fields']['uri']['id'] = 'uri';
  $handler->display->display_options['fields']['uri']['table'] = 'file_managed';
  $handler->display->display_options['fields']['uri']['field'] = 'uri';
  $handler->display->display_options['fields']['uri']['label'] = '';
  $handler->display->display_options['fields']['uri']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uri']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uri']['file_download_path'] = TRUE;
  /* Field: File: File ID */
  $handler->display->display_options['fields']['fid']['id'] = 'fid';
  $handler->display->display_options['fields']['fid']['table'] = 'file_managed';
  $handler->display->display_options['fields']['fid']['field'] = 'fid';
  $handler->display->display_options['fields']['fid']['label'] = '';
  $handler->display->display_options['fields']['fid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['fid']['element_label_colon'] = FALSE;
  /* Field: Bulk operations: File */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'file_managed';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['row_clickable'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: File: File as image */
  $handler->display->display_options['fields']['image']['id'] = 'image';
  $handler->display->display_options['fields']['image']['table'] = 'file_managed';
  $handler->display->display_options['fields']['image']['field'] = 'image';
  $handler->display->display_options['fields']['image']['label'] = '';
  $handler->display->display_options['fields']['image']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['image']['alter']['path'] = '[uri]';
  $handler->display->display_options['fields']['image']['alter']['target'] = '_blank';
  $handler->display->display_options['fields']['image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['image']['image_style'] = 'vsf_management_thumbnail';
  /* Field: File: Name */
  $handler->display->display_options['fields']['filename']['id'] = 'filename';
  $handler->display->display_options['fields']['filename']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filename']['field'] = 'filename';
  $handler->display->display_options['fields']['filename']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['filename']['alter']['path'] = '[uri]';
  $handler->display->display_options['fields']['filename']['alter']['target'] = '_blank';
  $handler->display->display_options['fields']['filename']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['filename']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['filename']['link_to_file'] = TRUE;
  /* Field: File: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'file_managed';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: File: Size */
  $handler->display->display_options['fields']['filesize']['id'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filesize']['field'] = 'filesize';
  /* Field: File: Upload date */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'file_managed';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  $handler->display->display_options['fields']['timestamp']['second_date_format'] = 'long';
  /* Field: SUM(File Usage: Use count) */
  $handler->display->display_options['fields']['count']['id'] = 'count';
  $handler->display->display_options['fields']['count']['table'] = 'file_usage';
  $handler->display->display_options['fields']['count']['field'] = 'count';
  $handler->display->display_options['fields']['count']['group_type'] = 'sum';
  $handler->display->display_options['fields']['count']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['count']['alter']['path'] = 'admin/content/files/[fid]/usage';
  $handler->display->display_options['fields']['count']['alter']['target'] = '_blank';
  $handler->display->display_options['fields']['count']['empty'] = '0';
  $handler->display->display_options['fields']['count']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['count']['separator'] = '';
  /* Filter criterion: File: Name */
  $handler->display->display_options['filters']['filename']['id'] = 'filename';
  $handler->display->display_options['filters']['filename']['table'] = 'file_managed';
  $handler->display->display_options['filters']['filename']['field'] = 'filename';
  $handler->display->display_options['filters']['filename']['operator'] = 'contains';
  $handler->display->display_options['filters']['filename']['exposed'] = TRUE;
  $handler->display->display_options['filters']['filename']['expose']['operator_id'] = 'filename_op';
  $handler->display->display_options['filters']['filename']['expose']['label'] = 'Name (contains)';
  $handler->display->display_options['filters']['filename']['expose']['operator'] = 'filename_op';
  $handler->display->display_options['filters']['filename']['expose']['identifier'] = 'filename';
  $handler->display->display_options['filters']['filename']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    4 => 0,
    3 => 0,
  );
  /* Filter criterion: File: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'file_managed';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    5 => 0,
    4 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/files';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Files';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $export[$view->name] = $view;







  $view = new view();
  $view->name = 'vsf_mgt_file_usage';
  $view->description = '';
  $view->tag = 'vsf_management';
  $view->base_table = 'file_managed';
  $view->human_name = 'Admin file usage';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'File usage';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer nodes';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No file, or no file usage...';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  /* Field: File Usage: Module */
  $handler->display->display_options['fields']['module']['id'] = 'module';
  $handler->display->display_options['fields']['module']['table'] = 'file_usage';
  $handler->display->display_options['fields']['module']['field'] = 'module';
  /* Field: File Usage: Entity type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'file_usage';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = 'Type';
  /* Field: File Usage: Entity ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'file_usage';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = 'ID';
  $handler->display->display_options['fields']['id']['separator'] = '';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Link?';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[type]/[id]';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = '[type]/[id]';
  $handler->display->display_options['fields']['nothing']['alter']['target'] = '_blank';
  /* Field: File Usage: Use count */
  $handler->display->display_options['fields']['count']['id'] = 'count';
  $handler->display->display_options['fields']['count']['table'] = 'file_usage';
  $handler->display->display_options['fields']['count']['field'] = 'count';
  /* Contextual filter: File: File ID */
  $handler->display->display_options['arguments']['fid']['id'] = 'fid';
  $handler->display->display_options['arguments']['fid']['table'] = 'file_managed';
  $handler->display->display_options['arguments']['fid']['field'] = 'fid';
  $handler->display->display_options['arguments']['fid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['fid']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['fid']['default_argument_options']['index'] = '3';
  $handler->display->display_options['arguments']['fid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['fid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['fid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['fid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['fid']['validate']['type'] = 'numeric';
  /* Filter criterion: File Usage: Use count */
  $handler->display->display_options['filters']['count']['id'] = 'count';
  $handler->display->display_options['filters']['count']['table'] = 'file_usage';
  $handler->display->display_options['filters']['count']['field'] = 'count';
  $handler->display->display_options['filters']['count']['operator'] = '>';
  $handler->display->display_options['filters']['count']['value']['value'] = '0';
  $handler->display->display_options['filters']['count']['expose']['operator_id'] = 'count_op';
  $handler->display->display_options['filters']['count']['expose']['label'] = 'Use count';
  $handler->display->display_options['filters']['count']['expose']['operator'] = 'count_op';
  $handler->display->display_options['filters']['count']['expose']['identifier'] = 'count';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/files/%/usage';

  $export[$view->name] = $view;



  return $export;
}

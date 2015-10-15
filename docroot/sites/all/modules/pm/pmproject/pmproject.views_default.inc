<?php
/**
 * @file
 * Views integration default settings.
 *
 * Stores default values for views. The view provides a list of timtracking
 * nodes and is available at 'pm/timetrackings'
 */

/**
 * Implements hook_views_default_views().
 */
function pmproject_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'pmproject_list';
  $view->description = '';
  $view->tag = 'pm';
  $view->base_table = 'node';
  $view->human_name = 'PM Project';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Projects';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'Project Management Project: access';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'pm_projectstatus' => 'pm_projectstatus',
    'pm_projectpriority' => 'pm_projectpriority',
    'pm_projectcategory' => 'pm_projectcategory',
    'pm_manager' => 'pm_manager',
    'edit_node' => 'edit_node',
    'delete_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'pm_projectstatus' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'pm_projectpriority' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'pm_projectcategory' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'pm_manager' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => ' ',
      'empty_column' => 1,
    ),
    'delete_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No Projects matched your search.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Project';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Status */
  $handler->display->display_options['fields']['pm_projectstatus']['id'] = 'pm_projectstatus';
  $handler->display->display_options['fields']['pm_projectstatus']['table'] = 'field_data_pm_projectstatus';
  $handler->display->display_options['fields']['pm_projectstatus']['field'] = 'pm_projectstatus';
  /* Field: Content: Priority */
  $handler->display->display_options['fields']['pm_projectpriority']['id'] = 'pm_projectpriority';
  $handler->display->display_options['fields']['pm_projectpriority']['table'] = 'field_data_pm_projectpriority';
  $handler->display->display_options['fields']['pm_projectpriority']['field'] = 'pm_projectpriority';
  /* Field: Content: Category */
  $handler->display->display_options['fields']['pm_projectcategory']['id'] = 'pm_projectcategory';
  $handler->display->display_options['fields']['pm_projectcategory']['table'] = 'field_data_pm_projectcategory';
  $handler->display->display_options['fields']['pm_projectcategory']['field'] = 'pm_projectcategory';
  /* Field: Content: Project manager */
  $handler->display->display_options['fields']['pm_manager']['id'] = 'pm_manager';
  $handler->display->display_options['fields']['pm_manager']['table'] = 'field_data_pm_manager';
  $handler->display->display_options['fields']['pm_manager']['field'] = 'pm_manager';
  $handler->display->display_options['fields']['pm_manager']['settings'] = array(
    'link' => 1,
  );
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Operations';
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['label'] = '';
  $handler->display->display_options['fields']['delete_node']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'pmproject' => 'pmproject',
  );
  $handler->display->display_options['arguments']['nid']['validate']['fail'] = 'ignore';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'pmproject' => 'pmproject',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Project';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['title']['group_info']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['group_info']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['group_info']['remember'] = FALSE;
  $handler->display->display_options['filters']['title']['group_info']['group_items'] = array(
    1 => array(),
    2 => array(),
    3 => array(),
  );
  /* Filter criterion: Content: Status (pm_projectstatus) */
  $handler->display->display_options['filters']['pm_projectstatus_value']['id'] = 'pm_projectstatus_value';
  $handler->display->display_options['filters']['pm_projectstatus_value']['table'] = 'field_data_pm_projectstatus';
  $handler->display->display_options['filters']['pm_projectstatus_value']['field'] = 'pm_projectstatus_value';
  $handler->display->display_options['filters']['pm_projectstatus_value']['group'] = 1;
  $handler->display->display_options['filters']['pm_projectstatus_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['pm_projectstatus_value']['expose']['operator_id'] = 'pm_projectstatus_value_op';
  $handler->display->display_options['filters']['pm_projectstatus_value']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['pm_projectstatus_value']['expose']['operator'] = 'pm_projectstatus_value_op';
  $handler->display->display_options['filters']['pm_projectstatus_value']['expose']['identifier'] = 'pm_projectstatus_value';
  $handler->display->display_options['filters']['pm_projectstatus_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['pm_projectstatus_value']['reduce_duplicates'] = TRUE;
  /* Filter criterion: Content: Priority (pm_projectpriority) */
  $handler->display->display_options['filters']['pm_projectpriority_value']['id'] = 'pm_projectpriority_value';
  $handler->display->display_options['filters']['pm_projectpriority_value']['table'] = 'field_data_pm_projectpriority';
  $handler->display->display_options['filters']['pm_projectpriority_value']['field'] = 'pm_projectpriority_value';
  $handler->display->display_options['filters']['pm_projectpriority_value']['group'] = 1;
  $handler->display->display_options['filters']['pm_projectpriority_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['pm_projectpriority_value']['expose']['operator_id'] = 'pm_projectpriority_value_op';
  $handler->display->display_options['filters']['pm_projectpriority_value']['expose']['label'] = 'Priority';
  $handler->display->display_options['filters']['pm_projectpriority_value']['expose']['operator'] = 'pm_projectpriority_value_op';
  $handler->display->display_options['filters']['pm_projectpriority_value']['expose']['identifier'] = 'pm_projectpriority_value';
  $handler->display->display_options['filters']['pm_projectpriority_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['pm_projectpriority_value']['reduce_duplicates'] = TRUE;
  /* Filter criterion: Content: Category (pm_projectcategory) */
  $handler->display->display_options['filters']['pm_projectcategory_value']['id'] = 'pm_projectcategory_value';
  $handler->display->display_options['filters']['pm_projectcategory_value']['table'] = 'field_data_pm_projectcategory';
  $handler->display->display_options['filters']['pm_projectcategory_value']['field'] = 'pm_projectcategory_value';
  $handler->display->display_options['filters']['pm_projectcategory_value']['group'] = 1;
  $handler->display->display_options['filters']['pm_projectcategory_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['pm_projectcategory_value']['expose']['operator_id'] = 'pm_projectcategory_value_op';
  $handler->display->display_options['filters']['pm_projectcategory_value']['expose']['label'] = 'Category';
  $handler->display->display_options['filters']['pm_projectcategory_value']['expose']['operator'] = 'pm_projectcategory_value_op';
  $handler->display->display_options['filters']['pm_projectcategory_value']['expose']['identifier'] = 'pm_projectcategory_value';
  $handler->display->display_options['filters']['pm_projectcategory_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['pm_projectcategory_value']['reduce_duplicates'] = TRUE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'pm/projects';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Projects';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;

  $views[$view->name] = $view;

  return $views;
}

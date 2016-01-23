<?php
/**
 * @file
 * hotfolder.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function hotfolder_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'job_records';
  $view->description = 'Job records';
  $view->tag = 'Job records';
  $view->base_table = 'node';
  $view->human_name = 'Job records';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Job records';
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = TRUE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'title' => 'title',
    'field_job_record_watch' => 'field_job_record_watch',
    'created' => 'created',
    'changed' => 'changed',
    'entity_id_3' => 'entity_id_3',
    'entity_id' => 'entity_id',
    'entity_id_4' => 'entity_id_4',
    'field_job_record_stage' => 'field_job_record_stage',
    'edit_node' => 'edit_node',
    'delete_node' => 'edit_node',
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
    'field_job_record_watch' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_id_3' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_id' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_id_4' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_job_record_stage' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
    'delete_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_job_record_watch_target_id']['id'] = 'field_job_record_watch_target_id';
  $handler->display->display_options['relationships']['field_job_record_watch_target_id']['table'] = 'field_data_field_job_record_watch';
  $handler->display->display_options['relationships']['field_job_record_watch_target_id']['field'] = 'field_job_record_watch_target_id';
  $handler->display->display_options['relationships']['field_job_record_watch_target_id']['label'] = 'Watch';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = 'Job#';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Job';
  /* Field: Content: Watch */
  $handler->display->display_options['fields']['field_job_record_watch']['id'] = 'field_job_record_watch';
  $handler->display->display_options['fields']['field_job_record_watch']['table'] = 'field_data_field_job_record_watch';
  $handler->display->display_options['fields']['field_job_record_watch']['field'] = 'field_job_record_watch';
  $handler->display->display_options['fields']['field_job_record_watch']['settings'] = array(
    'link' => 1,
  );
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Created';
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Last Updated';
  /* Field: Content: Source File */
  $handler->display->display_options['fields']['entity_id_3']['id'] = 'entity_id_3';
  $handler->display->display_options['fields']['entity_id_3']['table'] = 'field_data_field_job_record_source_file';
  $handler->display->display_options['fields']['entity_id_3']['field'] = 'field_job_record_source_file';
  $handler->display->display_options['fields']['entity_id_3']['click_sort_column'] = 'fid';
  /* Field: Content: Configured Actions */
  $handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['table'] = 'field_data_field_job_record_actions';
  $handler->display->display_options['fields']['entity_id']['field'] = 'field_job_record_actions';
  /* Field: Content: Status */
  $handler->display->display_options['fields']['entity_id_4']['id'] = 'entity_id_4';
  $handler->display->display_options['fields']['entity_id_4']['table'] = 'field_data_field_job_record_status';
  $handler->display->display_options['fields']['entity_id_4']['field'] = 'field_job_record_status';
  /* Field: Content: Stage */
  $handler->display->display_options['fields']['field_job_record_stage']['id'] = 'field_job_record_stage';
  $handler->display->display_options['fields']['field_job_record_stage']['table'] = 'field_data_field_job_record_stage';
  $handler->display->display_options['fields']['field_job_record_stage']['field'] = 'field_job_record_stage';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Operations';
  $handler->display->display_options['fields']['edit_node']['text'] = 'edit';
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['text'] = 'delete';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['sorts']['created']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['created']['expose']['label'] = 'Post date';
  $handler->display->display_options['sorts']['created']['granularity'] = 'minute';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'job_record' => 'job_record',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Job ID';
  $handler->display->display_options['filters']['title']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember'] = TRUE;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title_1']['id'] = 'title_1';
  $handler->display->display_options['filters']['title_1']['table'] = 'node';
  $handler->display->display_options['filters']['title_1']['field'] = 'title';
  $handler->display->display_options['filters']['title_1']['relationship'] = 'field_job_record_watch_target_id';
  $handler->display->display_options['filters']['title_1']['group'] = 1;
  $handler->display->display_options['filters']['title_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title_1']['expose']['operator_id'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['label'] = 'Watch';
  $handler->display->display_options['filters']['title_1']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['title_1']['expose']['operator'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['identifier'] = 'title_1';
  $handler->display->display_options['filters']['title_1']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['title_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Post date */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'node';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['group'] = 1;
  $handler->display->display_options['filters']['created']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = 'Created';
  $handler->display->display_options['filters']['created']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
  $handler->display->display_options['filters']['created']['expose']['remember'] = TRUE;
  /* Filter criterion: Content: Updated date */
  $handler->display->display_options['filters']['changed']['id'] = 'changed';
  $handler->display->display_options['filters']['changed']['table'] = 'node';
  $handler->display->display_options['filters']['changed']['field'] = 'changed';
  $handler->display->display_options['filters']['changed']['group'] = 1;
  $handler->display->display_options['filters']['changed']['exposed'] = TRUE;
  $handler->display->display_options['filters']['changed']['expose']['operator_id'] = 'changed_op';
  $handler->display->display_options['filters']['changed']['expose']['label'] = 'Last Updated';
  $handler->display->display_options['filters']['changed']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['changed']['expose']['operator'] = 'changed_op';
  $handler->display->display_options['filters']['changed']['expose']['identifier'] = 'changed';
  $handler->display->display_options['filters']['changed']['expose']['remember'] = TRUE;
  /* Filter criterion: Content: Status (field_job_record_status) */
  $handler->display->display_options['filters']['field_job_record_status_value']['id'] = 'field_job_record_status_value';
  $handler->display->display_options['filters']['field_job_record_status_value']['table'] = 'field_data_field_job_record_status';
  $handler->display->display_options['filters']['field_job_record_status_value']['field'] = 'field_job_record_status_value';
  $handler->display->display_options['filters']['field_job_record_status_value']['group'] = 1;
  $handler->display->display_options['filters']['field_job_record_status_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['operator_id'] = 'field_job_record_status_value_op';
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['operator'] = 'field_job_record_status_value_op';
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['identifier'] = 'field_job_record_status_value';
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    16 => 0,
    17 => 0,
    18 => 0,
    19 => 0,
    20 => 0,
    21 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    25 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
    29 => 0,
    30 => 0,
    31 => 0,
    32 => 0,
    33 => 0,
    34 => 0,
    35 => 0,
    36 => 0,
    37 => 0,
    38 => 0,
    39 => 0,
    40 => 0,
    41 => 0,
    42 => 0,
    43 => 0,
    44 => 0,
    45 => 0,
    46 => 0,
    47 => 0,
    48 => 0,
    49 => 0,
    50 => 0,
    51 => 0,
    52 => 0,
    53 => 0,
    54 => 0,
    55 => 0,
    56 => 0,
    57 => 0,
    58 => 0,
    59 => 0,
    60 => 0,
    61 => 0,
    62 => 0,
    63 => 0,
    64 => 0,
    65 => 0,
    66 => 0,
    67 => 0,
    68 => 0,
    69 => 0,
    70 => 0,
    71 => 0,
    72 => 0,
    73 => 0,
    74 => 0,
    75 => 0,
    76 => 0,
    77 => 0,
    78 => 0,
    79 => 0,
    80 => 0,
    81 => 0,
    82 => 0,
    83 => 0,
    84 => 0,
    85 => 0,
    86 => 0,
    87 => 0,
  );
  /* Filter criterion: Content: Stage (field_job_record_stage) */
  $handler->display->display_options['filters']['field_job_record_stage_value']['id'] = 'field_job_record_stage_value';
  $handler->display->display_options['filters']['field_job_record_stage_value']['table'] = 'field_data_field_job_record_stage';
  $handler->display->display_options['filters']['field_job_record_stage_value']['field'] = 'field_job_record_stage_value';
  $handler->display->display_options['filters']['field_job_record_stage_value']['group'] = 1;
  $handler->display->display_options['filters']['field_job_record_stage_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_job_record_stage_value']['expose']['operator_id'] = 'field_job_record_stage_value_op';
  $handler->display->display_options['filters']['field_job_record_stage_value']['expose']['label'] = 'Stage (field_job_record_stage)';
  $handler->display->display_options['filters']['field_job_record_stage_value']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_job_record_stage_value']['expose']['operator'] = 'field_job_record_stage_value_op';
  $handler->display->display_options['filters']['field_job_record_stage_value']['expose']['identifier'] = 'field_job_record_stage_value';

  /* Display: Job records */
  $handler = $view->new_display('page', 'Job records', 'job_records');
  $handler->display->display_options['display_description'] = 'Job records';
  $handler->display->display_options['path'] = 'job_records';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Job records';
  $handler->display->display_options['menu']['description'] = 'Job records';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Job history */
  $handler = $view->new_display('page', 'Job history', 'job_history');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Job history';
  $handler->display->display_options['display_description'] = 'Job history';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Watch (field_job_record_watch) */
  $handler->display->display_options['arguments']['field_job_record_watch_target_id']['id'] = 'field_job_record_watch_target_id';
  $handler->display->display_options['arguments']['field_job_record_watch_target_id']['table'] = 'field_data_field_job_record_watch';
  $handler->display->display_options['arguments']['field_job_record_watch_target_id']['field'] = 'field_job_record_watch_target_id';
  $handler->display->display_options['arguments']['field_job_record_watch_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_job_record_watch_target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['field_job_record_watch_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_job_record_watch_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_job_record_watch_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_job_record_watch_target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_job_record_watch_target_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['field_job_record_watch_target_id']['validate_options']['types'] = array(
    'watch_configuration' => 'watch_configuration',
  );
  $handler->display->display_options['arguments']['field_job_record_watch_target_id']['validate_options']['access'] = TRUE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'job_record' => 'job_record',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Job ID';
  $handler->display->display_options['filters']['title']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember'] = TRUE;
  /* Filter criterion: Content: Post date */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'node';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['group'] = 1;
  $handler->display->display_options['filters']['created']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = 'Created';
  $handler->display->display_options['filters']['created']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
  $handler->display->display_options['filters']['created']['expose']['remember'] = TRUE;
  /* Filter criterion: Content: Updated date */
  $handler->display->display_options['filters']['changed']['id'] = 'changed';
  $handler->display->display_options['filters']['changed']['table'] = 'node';
  $handler->display->display_options['filters']['changed']['field'] = 'changed';
  $handler->display->display_options['filters']['changed']['group'] = 1;
  $handler->display->display_options['filters']['changed']['exposed'] = TRUE;
  $handler->display->display_options['filters']['changed']['expose']['operator_id'] = 'changed_op';
  $handler->display->display_options['filters']['changed']['expose']['label'] = 'Last Updated';
  $handler->display->display_options['filters']['changed']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['changed']['expose']['operator'] = 'changed_op';
  $handler->display->display_options['filters']['changed']['expose']['identifier'] = 'changed';
  $handler->display->display_options['filters']['changed']['expose']['remember'] = TRUE;
  /* Filter criterion: Content: Status (field_job_record_status) */
  $handler->display->display_options['filters']['field_job_record_status_value']['id'] = 'field_job_record_status_value';
  $handler->display->display_options['filters']['field_job_record_status_value']['table'] = 'field_data_field_job_record_status';
  $handler->display->display_options['filters']['field_job_record_status_value']['field'] = 'field_job_record_status_value';
  $handler->display->display_options['filters']['field_job_record_status_value']['group'] = 1;
  $handler->display->display_options['filters']['field_job_record_status_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['operator_id'] = 'field_job_record_status_value_op';
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['operator'] = 'field_job_record_status_value_op';
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['identifier'] = 'field_job_record_status_value';
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['remember'] = TRUE;
  /* Filter criterion: Content: Stage (field_job_record_stage) */
  $handler->display->display_options['filters']['field_job_record_stage_value']['id'] = 'field_job_record_stage_value';
  $handler->display->display_options['filters']['field_job_record_stage_value']['table'] = 'field_data_field_job_record_stage';
  $handler->display->display_options['filters']['field_job_record_stage_value']['field'] = 'field_job_record_stage_value';
  $handler->display->display_options['filters']['field_job_record_stage_value']['group'] = 1;
  $handler->display->display_options['filters']['field_job_record_stage_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_job_record_stage_value']['expose']['operator_id'] = 'field_job_record_stage_value_op';
  $handler->display->display_options['filters']['field_job_record_stage_value']['expose']['label'] = 'Stage (field_job_record_stage)';
  $handler->display->display_options['filters']['field_job_record_stage_value']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_job_record_stage_value']['expose']['operator'] = 'field_job_record_stage_value_op';
  $handler->display->display_options['filters']['field_job_record_stage_value']['expose']['identifier'] = 'field_job_record_stage_value';
  $handler->display->display_options['path'] = 'node/%/jobs';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Job history';
  $handler->display->display_options['menu']['description'] = 'Job history';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Job records block */
  $handler = $view->new_display('block', 'Job records block', 'job_block');
  $handler->display->display_options['display_description'] = 'Job records block';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['separator'] = ' ';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Status */
  $handler->display->display_options['fields']['entity_id_4']['id'] = 'entity_id_4';
  $handler->display->display_options['fields']['entity_id_4']['table'] = 'field_data_field_job_record_status';
  $handler->display->display_options['fields']['entity_id_4']['field'] = 'field_job_record_status';
  $handler->display->display_options['fields']['entity_id_4']['exclude'] = TRUE;
  /* Field: Content: Stage */
  $handler->display->display_options['fields']['field_job_record_stage']['id'] = 'field_job_record_stage';
  $handler->display->display_options['fields']['field_job_record_stage']['table'] = 'field_data_field_job_record_stage';
  $handler->display->display_options['fields']['field_job_record_stage']['field'] = 'field_job_record_stage';
  $handler->display->display_options['fields']['field_job_record_stage']['label'] = '';
  $handler->display->display_options['fields']['field_job_record_stage']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_job_record_stage']['alter']['text'] = '( [entity_id_4] / [field_job_record_stage-value] )';
  $handler->display->display_options['fields']['field_job_record_stage']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'job_record' => 'job_record',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['block_description'] = 'Job records block';

  /* Display: Stuck jobs */
  $handler = $view->new_display('page', 'Stuck jobs', 'jobs_stuck');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Stuck jobs';
  $handler->display->display_options['display_description'] = 'Stuck jobs';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'Jobs listed here are at least 30 minutes old and may be stuck in the queue.';
  $handler->display->display_options['header']['area']['format'] = 'plain_text';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'job_record' => 'job_record',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Job ID';
  $handler->display->display_options['filters']['title']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember'] = TRUE;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title_1']['id'] = 'title_1';
  $handler->display->display_options['filters']['title_1']['table'] = 'node';
  $handler->display->display_options['filters']['title_1']['field'] = 'title';
  $handler->display->display_options['filters']['title_1']['relationship'] = 'field_job_record_watch_target_id';
  $handler->display->display_options['filters']['title_1']['group'] = 1;
  $handler->display->display_options['filters']['title_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title_1']['expose']['operator_id'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['label'] = 'Watch';
  $handler->display->display_options['filters']['title_1']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['title_1']['expose']['operator'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['identifier'] = 'title_1';
  $handler->display->display_options['filters']['title_1']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['title_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Content: Post date */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'node';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['operator'] = '<';
  $handler->display->display_options['filters']['created']['value']['value'] = '-30 minutes';
  $handler->display->display_options['filters']['created']['value']['type'] = 'offset';
  $handler->display->display_options['filters']['created']['group'] = 1;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = 'Post date';
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
  $handler->display->display_options['filters']['created']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    16 => 0,
    17 => 0,
    18 => 0,
    19 => 0,
    20 => 0,
    21 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    25 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
    29 => 0,
    30 => 0,
    31 => 0,
    32 => 0,
    33 => 0,
    34 => 0,
    35 => 0,
    36 => 0,
    37 => 0,
    38 => 0,
    39 => 0,
    40 => 0,
    41 => 0,
    42 => 0,
    43 => 0,
    44 => 0,
    45 => 0,
    46 => 0,
    47 => 0,
    48 => 0,
    49 => 0,
    50 => 0,
    51 => 0,
    52 => 0,
    53 => 0,
    54 => 0,
    55 => 0,
    56 => 0,
    57 => 0,
    58 => 0,
    59 => 0,
    60 => 0,
    61 => 0,
    62 => 0,
    63 => 0,
    64 => 0,
    65 => 0,
    66 => 0,
    67 => 0,
    68 => 0,
    69 => 0,
    70 => 0,
    71 => 0,
    72 => 0,
    73 => 0,
    74 => 0,
    75 => 0,
    76 => 0,
    77 => 0,
    78 => 0,
    79 => 0,
    80 => 0,
    81 => 0,
    82 => 0,
    83 => 0,
    84 => 0,
    85 => 0,
    86 => 0,
    87 => 0,
  );
  /* Filter criterion: Content: Status (field_job_record_status) */
  $handler->display->display_options['filters']['field_job_record_status_value']['id'] = 'field_job_record_status_value';
  $handler->display->display_options['filters']['field_job_record_status_value']['table'] = 'field_data_field_job_record_status';
  $handler->display->display_options['filters']['field_job_record_status_value']['field'] = 'field_job_record_status_value';
  $handler->display->display_options['filters']['field_job_record_status_value']['value'] = 'ready';
  $handler->display->display_options['filters']['field_job_record_status_value']['group'] = 1;
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['operator_id'] = 'field_job_record_status_value_op';
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['operator'] = 'field_job_record_status_value_op';
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['identifier'] = 'field_job_record_status_value';
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_job_record_status_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    16 => 0,
    17 => 0,
    18 => 0,
    19 => 0,
    20 => 0,
    21 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    25 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
    29 => 0,
    30 => 0,
    31 => 0,
    32 => 0,
    33 => 0,
    34 => 0,
    35 => 0,
    36 => 0,
    37 => 0,
    38 => 0,
    39 => 0,
    40 => 0,
    41 => 0,
    42 => 0,
    43 => 0,
    44 => 0,
    45 => 0,
    46 => 0,
    47 => 0,
    48 => 0,
    49 => 0,
    50 => 0,
    51 => 0,
    52 => 0,
    53 => 0,
    54 => 0,
    55 => 0,
    56 => 0,
    57 => 0,
    58 => 0,
    59 => 0,
    60 => 0,
    61 => 0,
    62 => 0,
    63 => 0,
    64 => 0,
    65 => 0,
    66 => 0,
    67 => 0,
    68 => 0,
    69 => 0,
    70 => 0,
    71 => 0,
    72 => 0,
    73 => 0,
    74 => 0,
    75 => 0,
    76 => 0,
    77 => 0,
    78 => 0,
    79 => 0,
    80 => 0,
    81 => 0,
    82 => 0,
    83 => 0,
    84 => 0,
    85 => 0,
    86 => 0,
    87 => 0,
  );
  /* Filter criterion: Content: Stage (field_job_record_stage) */
  $handler->display->display_options['filters']['field_job_record_stage_value']['id'] = 'field_job_record_stage_value';
  $handler->display->display_options['filters']['field_job_record_stage_value']['table'] = 'field_data_field_job_record_stage';
  $handler->display->display_options['filters']['field_job_record_stage_value']['field'] = 'field_job_record_stage_value';
  $handler->display->display_options['filters']['field_job_record_stage_value']['operator'] = '!=';
  $handler->display->display_options['filters']['field_job_record_stage_value']['value'] = 'prep';
  $handler->display->display_options['filters']['field_job_record_stage_value']['group'] = 1;
  $handler->display->display_options['filters']['field_job_record_stage_value']['expose']['operator_id'] = 'field_job_record_stage_value_op';
  $handler->display->display_options['filters']['field_job_record_stage_value']['expose']['label'] = 'Stage (field_job_record_stage)';
  $handler->display->display_options['filters']['field_job_record_stage_value']['expose']['operator'] = 'field_job_record_stage_value_op';
  $handler->display->display_options['filters']['field_job_record_stage_value']['expose']['identifier'] = 'field_job_record_stage_value';
  $handler->display->display_options['filters']['field_job_record_stage_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
    10 => 0,
    11 => 0,
    12 => 0,
    13 => 0,
    14 => 0,
    15 => 0,
    16 => 0,
    17 => 0,
    18 => 0,
    19 => 0,
    20 => 0,
    21 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    25 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
    29 => 0,
    30 => 0,
    31 => 0,
    32 => 0,
    33 => 0,
    34 => 0,
    35 => 0,
    36 => 0,
    37 => 0,
    38 => 0,
    39 => 0,
    40 => 0,
    41 => 0,
    42 => 0,
    43 => 0,
    44 => 0,
    45 => 0,
    46 => 0,
    47 => 0,
    48 => 0,
    49 => 0,
    50 => 0,
    51 => 0,
    52 => 0,
    53 => 0,
    54 => 0,
    55 => 0,
    56 => 0,
    57 => 0,
    58 => 0,
    59 => 0,
    60 => 0,
    61 => 0,
    62 => 0,
    63 => 0,
    64 => 0,
    65 => 0,
    66 => 0,
    67 => 0,
    68 => 0,
    69 => 0,
    70 => 0,
    71 => 0,
    72 => 0,
    73 => 0,
    74 => 0,
    75 => 0,
    76 => 0,
    77 => 0,
    78 => 0,
    79 => 0,
    80 => 0,
    81 => 0,
    82 => 0,
    83 => 0,
    84 => 0,
    85 => 0,
    86 => 0,
    87 => 0,
  );
  $handler->display->display_options['path'] = 'job_records_stuck';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Stuck jobs';
  $handler->display->display_options['menu']['description'] = 'Stuck jobs';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['job_records'] = $view;

  $view = new view();
  $view->name = 'watch_configurations';
  $view->description = 'Watch configurations';
  $view->tag = 'Watch configurations';
  $view->base_table = 'node';
  $view->human_name = 'Watch configurations';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Watch configurations';
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = TRUE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'title' => 'title',
    'created' => 'created',
    'name' => 'created',
    'changed' => 'changed',
    'name_1' => 'changed',
    'entity_id_1' => 'entity_id_1',
    'entity_id_2' => 'entity_id_2',
    'entity_id' => 'entity_id',
    'edit_node' => 'edit_node',
    'delete_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
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
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '</br> by ',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '</br> by ',
      'empty_column' => 0,
    ),
    'name_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_id_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_id_2' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'entity_id' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
    'delete_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Relationship: Content revision: User */
  $handler->display->display_options['relationships']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['relationships']['uid_1']['table'] = 'node_revision';
  $handler->display->display_options['relationships']['uid_1']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid_1']['label'] = 'Revised by';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = 'Watch#';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Watch';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Configured';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'By';
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Last Updated';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'uid_1';
  /* Field: Content: Watch Folder */
  $handler->display->display_options['fields']['entity_id_1']['id'] = 'entity_id_1';
  $handler->display->display_options['fields']['entity_id_1']['table'] = 'field_data_field_watch_config_folder';
  $handler->display->display_options['fields']['entity_id_1']['field'] = 'field_watch_config_folder';
  $handler->display->display_options['fields']['entity_id_1']['label'] = 'Folder';
  /* Field: Content: Source Filename Pattern */
  $handler->display->display_options['fields']['entity_id_2']['id'] = 'entity_id_2';
  $handler->display->display_options['fields']['entity_id_2']['table'] = 'field_data_field_watch_config_src_pattern';
  $handler->display->display_options['fields']['entity_id_2']['field'] = 'field_watch_config_src_pattern';
  $handler->display->display_options['fields']['entity_id_2']['label'] = 'Filename Pattern';
  /* Field: Content: Configured Actions */
  $handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['table'] = 'field_data_field_watch_config_actions';
  $handler->display->display_options['fields']['entity_id']['field'] = 'field_watch_config_actions';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Operations';
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'watch_configuration' => 'watch_configuration',
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Watch ID';
  $handler->display->display_options['filters']['title']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember'] = TRUE;
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'By User';
  $handler->display->display_options['filters']['uid']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember'] = TRUE;
  /* Filter criterion: Content: Post date */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'node';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = 'Configured';
  $handler->display->display_options['filters']['created']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
  $handler->display->display_options['filters']['created']['expose']['remember'] = TRUE;
  /* Filter criterion: Content: Updated date */
  $handler->display->display_options['filters']['changed']['id'] = 'changed';
  $handler->display->display_options['filters']['changed']['table'] = 'node';
  $handler->display->display_options['filters']['changed']['field'] = 'changed';
  $handler->display->display_options['filters']['changed']['exposed'] = TRUE;
  $handler->display->display_options['filters']['changed']['expose']['operator_id'] = 'changed_op';
  $handler->display->display_options['filters']['changed']['expose']['label'] = 'Last Updated';
  $handler->display->display_options['filters']['changed']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['changed']['expose']['operator'] = 'changed_op';
  $handler->display->display_options['filters']['changed']['expose']['identifier'] = 'changed';
  $handler->display->display_options['filters']['changed']['expose']['remember'] = TRUE;
  /* Filter criterion: Content: Watch Folder (field_watch_config_folder) */
  $handler->display->display_options['filters']['field_watch_config_folder_value']['id'] = 'field_watch_config_folder_value';
  $handler->display->display_options['filters']['field_watch_config_folder_value']['table'] = 'field_data_field_watch_config_folder';
  $handler->display->display_options['filters']['field_watch_config_folder_value']['field'] = 'field_watch_config_folder_value';
  $handler->display->display_options['filters']['field_watch_config_folder_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_watch_config_folder_value']['expose']['operator_id'] = 'field_watch_config_folder_value_op';
  $handler->display->display_options['filters']['field_watch_config_folder_value']['expose']['label'] = 'Watch ID';
  $handler->display->display_options['filters']['field_watch_config_folder_value']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_watch_config_folder_value']['expose']['operator'] = 'field_watch_config_folder_value_op';
  $handler->display->display_options['filters']['field_watch_config_folder_value']['expose']['identifier'] = 'field_watch_config_folder_value';
  $handler->display->display_options['filters']['field_watch_config_folder_value']['expose']['remember'] = TRUE;
  /* Filter criterion: Content: Source Filename Pattern (field_watch_config_src_pattern) */
  $handler->display->display_options['filters']['field_watch_config_src_pattern_value']['id'] = 'field_watch_config_src_pattern_value';
  $handler->display->display_options['filters']['field_watch_config_src_pattern_value']['table'] = 'field_data_field_watch_config_src_pattern';
  $handler->display->display_options['filters']['field_watch_config_src_pattern_value']['field'] = 'field_watch_config_src_pattern_value';
  $handler->display->display_options['filters']['field_watch_config_src_pattern_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_watch_config_src_pattern_value']['expose']['operator_id'] = 'field_watch_config_src_pattern_value_op';
  $handler->display->display_options['filters']['field_watch_config_src_pattern_value']['expose']['label'] = 'Filename Pattern';
  $handler->display->display_options['filters']['field_watch_config_src_pattern_value']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_watch_config_src_pattern_value']['expose']['operator'] = 'field_watch_config_src_pattern_value_op';
  $handler->display->display_options['filters']['field_watch_config_src_pattern_value']['expose']['identifier'] = 'field_watch_config_src_pattern_value';
  $handler->display->display_options['filters']['field_watch_config_src_pattern_value']['expose']['remember'] = TRUE;

  /* Display: Watch configurations */
  $handler = $view->new_display('page', 'Watch configurations', 'watches');
  $handler->display->display_options['display_description'] = 'Watch configurations';
  $handler->display->display_options['path'] = 'watch_configurations';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Watch configurations';
  $handler->display->display_options['menu']['description'] = 'Watch configurations';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Watch configurations */
  $handler = $view->new_display('block', 'Watch configurations', 'watches_block');
  $handler->display->display_options['display_description'] = 'Watch configurations';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Stream */
  $handler->display->display_options['fields']['field_watch_config_scheme']['id'] = 'field_watch_config_scheme';
  $handler->display->display_options['fields']['field_watch_config_scheme']['table'] = 'field_data_field_watch_config_scheme';
  $handler->display->display_options['fields']['field_watch_config_scheme']['field'] = 'field_watch_config_scheme';
  $handler->display->display_options['fields']['field_watch_config_scheme']['label'] = '';
  $handler->display->display_options['fields']['field_watch_config_scheme']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_watch_config_scheme']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_watch_config_scheme']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Watch Folder */
  $handler->display->display_options['fields']['entity_id_1']['id'] = 'entity_id_1';
  $handler->display->display_options['fields']['entity_id_1']['table'] = 'field_data_field_watch_config_folder';
  $handler->display->display_options['fields']['entity_id_1']['field'] = 'field_watch_config_folder';
  $handler->display->display_options['fields']['entity_id_1']['label'] = '';
  $handler->display->display_options['fields']['entity_id_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['entity_id_1']['element_label_colon'] = FALSE;
  /* Field: Content: Source Filename Pattern */
  $handler->display->display_options['fields']['field_watch_config_src_pattern']['id'] = 'field_watch_config_src_pattern';
  $handler->display->display_options['fields']['field_watch_config_src_pattern']['table'] = 'field_data_field_watch_config_src_pattern';
  $handler->display->display_options['fields']['field_watch_config_src_pattern']['field'] = 'field_watch_config_src_pattern';
  $handler->display->display_options['fields']['field_watch_config_src_pattern']['label'] = '';
  $handler->display->display_options['fields']['field_watch_config_src_pattern']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_watch_config_src_pattern']['alter']['text'] = '[field_watch_config_scheme][entity_id_1]/[field_watch_config_src_pattern]';
  $handler->display->display_options['fields']['field_watch_config_src_pattern']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'watch_configuration' => 'watch_configuration',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['block_description'] = 'Watch configurations block';
  $export['watch_configurations'] = $view;

  return $export;
}

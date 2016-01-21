<?php
/**
 * @file
 * cod_session_evaluations.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function cod_session_evaluations_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'sessions_eval_report';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'sessions_eval_report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Evaluation Report';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['link_display'] = 'page';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['disable_sql_rewrite'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_eval_session_target_id']['id'] = 'field_eval_session_target_id';
  $handler->display->display_options['relationships']['field_eval_session_target_id']['table'] = 'field_data_field_eval_session';
  $handler->display->display_options['relationships']['field_eval_session_target_id']['field'] = 'field_eval_session_target_id';
  $handler->display->display_options['relationships']['field_eval_session_target_id']['required'] = TRUE;
  /* Field: Content: Session being evaluated */
  $handler->display->display_options['fields']['field_eval_session']['id'] = 'field_eval_session';
  $handler->display->display_options['fields']['field_eval_session']['table'] = 'field_data_field_eval_session';
  $handler->display->display_options['fields']['field_eval_session']['field'] = 'field_eval_session';
  $handler->display->display_options['fields']['field_eval_session']['label'] = 'Session';
  $handler->display->display_options['fields']['field_eval_session']['type'] = 'entityreference_entity_view';
  $handler->display->display_options['fields']['field_eval_session']['settings'] = array(
    'view_mode' => 'listing',
    'links' => 0,
  );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'session_evaluation' => 'session_evaluation',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'field_eval_session_target_id';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Session being evaluated';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    8 => 0,
    9 => 0,
    6 => 0,
    7 => 0,
    10 => 0,
    3 => 0,
    5 => 0,
    4 => 0,
  );

  /* Display: Admin Eval Report */
  $handler = $view->new_display('page', 'Admin Eval Report', 'page');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'field_eval_session' => 'field_eval_session',
    'field_eval_field_1' => 'field_eval_field_1',
    'field_eval_field_3' => 'field_eval_field_3',
    'field_eval_field_4' => 'field_eval_field_4',
    'field_eval_field_2' => 'field_eval_field_2',
    'field_eval_field_5' => 'field_eval_field_5',
    'field_eval_text_field' => 'field_eval_text_field',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'views_bulk_operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_eval_session' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_eval_field_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_eval_field_3' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_eval_field_4' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_eval_field_2' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_eval_field_5' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_eval_text_field' => array(
      'sortable' => 0,
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
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::node_assign_owner_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_revision' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::flag_node_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_make_sticky_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_make_unsticky_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          '_all_' => '_all_',
        ),
      ),
    ),
    'action::views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'url' => '',
      ),
    ),
    'action::node_promote_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_publish_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpromote_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::mollom_action_unpublish_node' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_save_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::panelizer_set_status_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpublish_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpublish_by_keyword_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::pathauto_node_update_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Content: Session being evaluated */
  $handler->display->display_options['fields']['field_eval_session']['id'] = 'field_eval_session';
  $handler->display->display_options['fields']['field_eval_session']['table'] = 'field_data_field_eval_session';
  $handler->display->display_options['fields']['field_eval_session']['field'] = 'field_eval_session';
  $handler->display->display_options['fields']['field_eval_session']['label'] = 'Session';
  $handler->display->display_options['fields']['field_eval_session']['type'] = 'entityreference_entity_view';
  $handler->display->display_options['fields']['field_eval_session']['settings'] = array(
    'view_mode' => 'listing',
    'links' => 0,
  );
  /* Field: Content: How was the session overall? */
  $handler->display->display_options['fields']['field_eval_field_1']['id'] = 'field_eval_field_1';
  $handler->display->display_options['fields']['field_eval_field_1']['table'] = 'field_data_field_eval_field_1';
  $handler->display->display_options['fields']['field_eval_field_1']['field'] = 'field_eval_field_1';
  /* Field: Content: Please rate the speaker's presentation skills. */
  $handler->display->display_options['fields']['field_eval_field_3']['id'] = 'field_eval_field_3';
  $handler->display->display_options['fields']['field_eval_field_3']['table'] = 'field_data_field_eval_field_3';
  $handler->display->display_options['fields']['field_eval_field_3']['field'] = 'field_eval_field_3';
  /* Field: Content: Please rate the quality of speaker's slides and visual aids. */
  $handler->display->display_options['fields']['field_eval_field_4']['id'] = 'field_eval_field_4';
  $handler->display->display_options['fields']['field_eval_field_4']['table'] = 'field_data_field_eval_field_4';
  $handler->display->display_options['fields']['field_eval_field_4']['field'] = 'field_eval_field_4';
  /* Field: Content: Please rate the speaker's mastery of this topic. */
  $handler->display->display_options['fields']['field_eval_field_2']['id'] = 'field_eval_field_2';
  $handler->display->display_options['fields']['field_eval_field_2']['table'] = 'field_data_field_eval_field_2';
  $handler->display->display_options['fields']['field_eval_field_2']['field'] = 'field_eval_field_2';
  /* Field: Content: Did you learn something in this session you can use in real life? */
  $handler->display->display_options['fields']['field_eval_field_5']['id'] = 'field_eval_field_5';
  $handler->display->display_options['fields']['field_eval_field_5']['table'] = 'field_data_field_eval_field_5';
  $handler->display->display_options['fields']['field_eval_field_5']['field'] = 'field_eval_field_5';
  /* Field: Content: Comments */
  $handler->display->display_options['fields']['field_eval_text_field']['id'] = 'field_eval_text_field';
  $handler->display->display_options['fields']['field_eval_text_field']['table'] = 'field_data_field_eval_text_field';
  $handler->display->display_options['fields']['field_eval_text_field']['field'] = 'field_eval_text_field';
  $handler->display->display_options['path'] = 'admin/conference/%/sessions/evaluations';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Evaluations';
  $handler->display->display_options['menu']['weight'] = '30';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Session being evaluated */
  $handler->display->display_options['fields']['field_eval_session_1']['id'] = 'field_eval_session_1';
  $handler->display->display_options['fields']['field_eval_session_1']['table'] = 'field_data_field_eval_session';
  $handler->display->display_options['fields']['field_eval_session_1']['field'] = 'field_eval_session';
  $handler->display->display_options['fields']['field_eval_session_1']['label'] = 'Session';
  $handler->display->display_options['fields']['field_eval_session_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_eval_session_1']['settings'] = array(
    'link' => 0,
  );
  /* Field: Content: How was the session overall? */
  $handler->display->display_options['fields']['field_eval_field_1']['id'] = 'field_eval_field_1';
  $handler->display->display_options['fields']['field_eval_field_1']['table'] = 'field_data_field_eval_field_1';
  $handler->display->display_options['fields']['field_eval_field_1']['field'] = 'field_eval_field_1';
  /* Field: Content: Please rate the speaker's presentation skills. */
  $handler->display->display_options['fields']['field_eval_field_3']['id'] = 'field_eval_field_3';
  $handler->display->display_options['fields']['field_eval_field_3']['table'] = 'field_data_field_eval_field_3';
  $handler->display->display_options['fields']['field_eval_field_3']['field'] = 'field_eval_field_3';
  /* Field: Content: Please rate the quality of speaker's slides and visual aids. */
  $handler->display->display_options['fields']['field_eval_field_4']['id'] = 'field_eval_field_4';
  $handler->display->display_options['fields']['field_eval_field_4']['table'] = 'field_data_field_eval_field_4';
  $handler->display->display_options['fields']['field_eval_field_4']['field'] = 'field_eval_field_4';
  /* Field: Content: Please rate the speaker's mastery of this topic. */
  $handler->display->display_options['fields']['field_eval_field_2']['id'] = 'field_eval_field_2';
  $handler->display->display_options['fields']['field_eval_field_2']['table'] = 'field_data_field_eval_field_2';
  $handler->display->display_options['fields']['field_eval_field_2']['field'] = 'field_eval_field_2';
  /* Field: Content: Did you learn something in this session you can use in real life? */
  $handler->display->display_options['fields']['field_eval_field_5']['id'] = 'field_eval_field_5';
  $handler->display->display_options['fields']['field_eval_field_5']['table'] = 'field_data_field_eval_field_5';
  $handler->display->display_options['fields']['field_eval_field_5']['field'] = 'field_eval_field_5';
  /* Field: Content: Comments */
  $handler->display->display_options['fields']['field_eval_text_field']['id'] = 'field_eval_text_field';
  $handler->display->display_options['fields']['field_eval_text_field']['table'] = 'field_data_field_eval_text_field';
  $handler->display->display_options['fields']['field_eval_text_field']['field'] = 'field_eval_text_field';
  $handler->display->display_options['path'] = 'conference-admin/sessions/evaluations/feed';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );

  /* Display: Speaker's Eval Averages */
  $handler = $view->new_display('panel_pane', 'Speaker\'s Eval Averages', 'cod_session_evaluations_pane');
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_eval_field_1' => 'field_eval_field_1',
    'field_eval_field_2' => 'field_eval_field_2',
    'field_eval_field_3' => 'field_eval_field_3',
    'field_eval_field_4' => 'field_eval_field_4',
    'field_eval_field_5' => 'field_eval_field_5',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_eval_field_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_eval_field_2' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_eval_field_3' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_eval_field_4' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_eval_field_5' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<h2>Evaluations for [title_1]</h2>
  [title] evaluations submitted.
  The following are averages of each evaluation field.';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There are no evaluations to display. Reasons for this could include:
  <ol>
   <li>No evaluations have been created yet.</li>
   <li>You are not a speaker for this session. Only referenced speakers have access to see evaluations.</li>
  </ol>';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_eval_session_target_id']['id'] = 'field_eval_session_target_id';
  $handler->display->display_options['relationships']['field_eval_session_target_id']['table'] = 'field_data_field_eval_session';
  $handler->display->display_options['relationships']['field_eval_session_target_id']['field'] = 'field_eval_session_target_id';
  $handler->display->display_options['relationships']['field_eval_session_target_id']['required'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: How was the session overall? */
  $handler->display->display_options['fields']['field_eval_field_1']['id'] = 'field_eval_field_1';
  $handler->display->display_options['fields']['field_eval_field_1']['table'] = 'field_data_field_eval_field_1';
  $handler->display->display_options['fields']['field_eval_field_1']['field'] = 'field_eval_field_1';
  $handler->display->display_options['fields']['field_eval_field_1']['group_type'] = 'avg';
  $handler->display->display_options['fields']['field_eval_field_1']['label'] = 'Overall';
  /* Field: Content: Please rate the speaker's mastery of this topic. */
  $handler->display->display_options['fields']['field_eval_field_2']['id'] = 'field_eval_field_2';
  $handler->display->display_options['fields']['field_eval_field_2']['table'] = 'field_data_field_eval_field_2';
  $handler->display->display_options['fields']['field_eval_field_2']['field'] = 'field_eval_field_2';
  $handler->display->display_options['fields']['field_eval_field_2']['group_type'] = 'avg';
  /* Field: Content: Please rate the speaker's presentation skills. */
  $handler->display->display_options['fields']['field_eval_field_3']['id'] = 'field_eval_field_3';
  $handler->display->display_options['fields']['field_eval_field_3']['table'] = 'field_data_field_eval_field_3';
  $handler->display->display_options['fields']['field_eval_field_3']['field'] = 'field_eval_field_3';
  $handler->display->display_options['fields']['field_eval_field_3']['group_type'] = 'avg';
  /* Field: Content: Please rate the quality of speaker's slides and visual aids. */
  $handler->display->display_options['fields']['field_eval_field_4']['id'] = 'field_eval_field_4';
  $handler->display->display_options['fields']['field_eval_field_4']['table'] = 'field_data_field_eval_field_4';
  $handler->display->display_options['fields']['field_eval_field_4']['field'] = 'field_eval_field_4';
  $handler->display->display_options['fields']['field_eval_field_4']['group_type'] = 'avg';
  /* Field: Content: Did you learn something in this session you can use in real life? */
  $handler->display->display_options['fields']['field_eval_field_5']['id'] = 'field_eval_field_5';
  $handler->display->display_options['fields']['field_eval_field_5']['table'] = 'field_data_field_eval_field_5';
  $handler->display->display_options['fields']['field_eval_field_5']['field'] = 'field_eval_field_5';
  $handler->display->display_options['fields']['field_eval_field_5']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_eval_field_5']['label'] = 'People who learned something';
  $handler->display->display_options['fields']['field_eval_field_5']['type'] = 'list_key';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_eval_session_target_id';
  $handler->display->display_options['fields']['title']['group_type'] = 'count';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'field_eval_session_target_id';
  $handler->display->display_options['fields']['title_1']['label'] = 'Session Title';
  $handler->display->display_options['fields']['title_1']['exclude'] = TRUE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'field_eval_session_target_id';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'schedule_item' => 'schedule_item',
    'session' => 'session',
  );
  $handler->display->display_options['arguments']['nid']['validate_options']['access'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['fail'] = 'access denied';
  /* Contextual filter: Content: Speaker(s) (field_speakers) */
  $handler->display->display_options['arguments']['field_speakers_target_id']['id'] = 'field_speakers_target_id';
  $handler->display->display_options['arguments']['field_speakers_target_id']['table'] = 'field_data_field_speakers';
  $handler->display->display_options['arguments']['field_speakers_target_id']['field'] = 'field_speakers_target_id';
  $handler->display->display_options['arguments']['field_speakers_target_id']['relationship'] = 'field_eval_session_target_id';
  $handler->display->display_options['arguments']['field_speakers_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_speakers_target_id']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['field_speakers_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_speakers_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_speakers_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_speakers_target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_speakers_target_id']['validate']['type'] = 'current_user_or_role';
  $handler->display->display_options['arguments']['field_speakers_target_id']['validate_options']['restrict_roles'] = TRUE;
  $handler->display->display_options['arguments']['field_speakers_target_id']['validate_options']['roles'] = array(
    5 => '5',
    4 => '4',
  );
  $handler->display->display_options['arguments']['field_speakers_target_id']['validate']['fail'] = 'access denied';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'session_evaluation' => 'session_evaluation',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['argument_input'] = array(
    'nid' => array(
      'type' => 'user',
      'context' => 'entity:comment.author',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Session ID',
    ),
    'field_speakers_target_id' => array(
      'type' => 'none',
      'context' => 'entity:comment.author',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Current User',
    ),
  );

  /* Display: Eval Comments attachment */
  $handler = $view->new_display('attachment', 'Eval Comments attachment', 'attachment_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Comments';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_eval_text_field' => 'field_eval_text_field',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_eval_text_field' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<h2>Comments</h2>';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_eval_session_target_id']['id'] = 'field_eval_session_target_id';
  $handler->display->display_options['relationships']['field_eval_session_target_id']['table'] = 'field_data_field_eval_session';
  $handler->display->display_options['relationships']['field_eval_session_target_id']['field'] = 'field_eval_session_target_id';
  $handler->display->display_options['relationships']['field_eval_session_target_id']['required'] = TRUE;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_speakers_target_id']['id'] = 'field_speakers_target_id';
  $handler->display->display_options['relationships']['field_speakers_target_id']['table'] = 'field_data_field_speakers';
  $handler->display->display_options['relationships']['field_speakers_target_id']['field'] = 'field_speakers_target_id';
  $handler->display->display_options['relationships']['field_speakers_target_id']['relationship'] = 'field_eval_session_target_id';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Comments */
  $handler->display->display_options['fields']['field_eval_text_field']['id'] = 'field_eval_text_field';
  $handler->display->display_options['fields']['field_eval_text_field']['table'] = 'field_data_field_eval_text_field';
  $handler->display->display_options['fields']['field_eval_text_field']['field'] = 'field_eval_text_field';
  $handler->display->display_options['fields']['field_eval_text_field']['label'] = '';
  $handler->display->display_options['fields']['field_eval_text_field']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Session being evaluated (field_eval_session) */
  $handler->display->display_options['arguments']['field_eval_session_target_id']['id'] = 'field_eval_session_target_id';
  $handler->display->display_options['arguments']['field_eval_session_target_id']['table'] = 'field_data_field_eval_session';
  $handler->display->display_options['arguments']['field_eval_session_target_id']['field'] = 'field_eval_session_target_id';
  $handler->display->display_options['arguments']['field_eval_session_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_eval_session_target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['field_eval_session_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_eval_session_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_eval_session_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_eval_session_target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_eval_session_target_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['field_eval_session_target_id']['validate_options']['types'] = array(
    'session' => 'session',
    'session_evaluation' => 'session_evaluation',
  );
  $handler->display->display_options['arguments']['field_eval_session_target_id']['validate']['fail'] = 'empty';
  /* Contextual filter: User: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'users';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'field_speakers_target_id';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['uid']['validate']['type'] = 'user';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'session_evaluation' => 'session_evaluation',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  /* Filter criterion: Content: Comments (field_eval_text_field) */
  $handler->display->display_options['filters']['field_eval_text_field_value']['id'] = 'field_eval_text_field_value';
  $handler->display->display_options['filters']['field_eval_text_field_value']['table'] = 'field_data_field_eval_text_field';
  $handler->display->display_options['filters']['field_eval_text_field_value']['field'] = 'field_eval_text_field_value';
  $handler->display->display_options['filters']['field_eval_text_field_value']['operator'] = 'not empty';
  $handler->display->display_options['filters']['field_eval_text_field_value']['group'] = 1;
  $handler->display->display_options['displays'] = array(
    'cod_session_evaluations_pane' => 'cod_session_evaluations_pane',
    'default' => 0,
    'page' => 0,
  );
  $handler->display->display_options['attachment_position'] = 'after';
  $export['sessions_eval_report'] = $view;

  $view = new view();
  $view->name = 'sessions_speaker_review';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'sessions_speaker_review';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Sessions';
  $handler->display->display_options['css_class'] = 'view-grouped-list sessions-list';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    2 => '2',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_accepted' => 'field_accepted',
    'field_speakers' => 'field_speakers',
    'field_experience' => 'field_experience',
    'field_track' => 'field_track',
    'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_accepted' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_speakers' => array(
      'align' => '',
      'separator' => '',
    ),
    'field_experience' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_track' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No sessions';
  $handler->display->display_options['empty']['area']['content'] = 'There are no sessions with this status.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Evaluations';
  $handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['path'] = 'sessions-eval/[nid]';
  $handler->display->display_options['fields']['nothing']['alter']['alt'] = 'Evaluations';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Field: Content: Status */
  $handler->display->display_options['fields']['field_accepted']['id'] = 'field_accepted';
  $handler->display->display_options['fields']['field_accepted']['table'] = 'field_data_field_accepted';
  $handler->display->display_options['fields']['field_accepted']['field'] = 'field_accepted';
  $handler->display->display_options['fields']['field_accepted']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_accepted']['hide_alter_empty'] = FALSE;
  /* Contextual filter: User: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'users';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['uid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['uid']['title'] = '%1 Sessions';
  $handler->display->display_options['arguments']['uid']['breadcrumb_enable'] = TRUE;
  $handler->display->display_options['arguments']['uid']['breadcrumb'] = 'Sessions';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'session' => 'session',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Status (field_accepted) */
  $handler->display->display_options['filters']['field_accepted_value']['id'] = 'field_accepted_value';
  $handler->display->display_options['filters']['field_accepted_value']['table'] = 'field_data_field_accepted';
  $handler->display->display_options['filters']['field_accepted_value']['field'] = 'field_accepted_value';
  $handler->display->display_options['filters']['field_accepted_value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['field_accepted_value']['group'] = 1;
  /* Filter criterion: Broken/missing handler */
  $handler->display->display_options['filters']['field_track_value']['id'] = 'field_track_value';
  $handler->display->display_options['filters']['field_track_value']['table'] = 'field_data_field_track';
  $handler->display->display_options['filters']['field_track_value']['field'] = 'field_track_value';
  $handler->display->display_options['filters']['field_track_value']['group'] = 1;
  $handler->display->display_options['filters']['field_track_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_track_value']['expose']['operator_id'] = 'field_track_value_op';
  $handler->display->display_options['filters']['field_track_value']['expose']['label'] = 'Track';
  $handler->display->display_options['filters']['field_track_value']['expose']['operator'] = 'field_track_value_op';
  $handler->display->display_options['filters']['field_track_value']['expose']['identifier'] = 'track';
  $handler->display->display_options['filters']['field_track_value']['expose']['multiple'] = TRUE;
  /* Filter criterion: Search: Search Terms */
  $handler->display->display_options['filters']['keys']['id'] = 'keys';
  $handler->display->display_options['filters']['keys']['table'] = 'search_index';
  $handler->display->display_options['filters']['keys']['field'] = 'keys';
  $handler->display->display_options['filters']['keys']['group'] = 1;
  $handler->display->display_options['filters']['keys']['exposed'] = TRUE;
  $handler->display->display_options['filters']['keys']['expose']['operator_id'] = 'keys_op';
  $handler->display->display_options['filters']['keys']['expose']['label'] = 'Filter by keywords:';
  $handler->display->display_options['filters']['keys']['expose']['operator'] = 'keys_op';
  $handler->display->display_options['filters']['keys']['expose']['identifier'] = 'keys';
  /* Filter criterion: Content: Experience level (field_experience) */
  $handler->display->display_options['filters']['field_experience_value']['id'] = 'field_experience_value';
  $handler->display->display_options['filters']['field_experience_value']['table'] = 'field_data_field_experience';
  $handler->display->display_options['filters']['field_experience_value']['field'] = 'field_experience_value';
  $handler->display->display_options['filters']['field_experience_value']['group'] = 1;
  $handler->display->display_options['filters']['field_experience_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_experience_value']['expose']['operator_id'] = 'field_experience_value_op';
  $handler->display->display_options['filters']['field_experience_value']['expose']['label'] = 'Experience level';
  $handler->display->display_options['filters']['field_experience_value']['expose']['operator'] = 'field_experience_value_op';
  $handler->display->display_options['filters']['field_experience_value']['expose']['identifier'] = 'experience_level';
  $handler->display->display_options['filters']['field_experience_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_experience_value']['expose']['multiple'] = TRUE;

  /* Display: Page: My Sessions */
  $handler = $view->new_display('page', 'Page: My Sessions', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'My Sessions';
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'view-grouped-list sessions-list sessions-accepted';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No accepted sessions';
  $handler->display->display_options['empty']['area']['content'] = 'No sessions have been accepted yet. When you accept unprocessed sessions, they will show up here.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'session' => 'session',
    'training' => 'training',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';
  $handler->display->display_options['filters']['uid_current']['group'] = 1;
  $handler->display->display_options['path'] = 'user/%/sessions';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'My Sessions';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-menu-quick-links';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Admin Sessions';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $export['sessions_speaker_review'] = $view;

  return $export;
}

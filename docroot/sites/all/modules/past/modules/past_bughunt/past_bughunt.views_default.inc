<?php

function past_bughunt_views_default_views() {
  $views = array();
  // TODO view.
  $view = new view();
  $view->name = 'past_event_todo';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'past_event';
  $view->human_name = 'Past event TODO';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Past event TODO';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer past';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'event_id' => 'event_id',
    'severity' => 'severity',
    'timestamp' => 'timestamp',
    'module' => 'module',
    'message' => 'message',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'event_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'severity' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
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
    'message' => array(
      'sortable' => 0,
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
  $handler->display->display_options['empty']['area']['content'] = 'No TODO events found.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Flags: event_todo */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'past_event';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['label'] = 'TODO flag';
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'event_todo';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  /* Relationship: Flags: event_done */
  $handler->display->display_options['relationships']['flag_content_rel_1']['id'] = 'flag_content_rel_1';
  $handler->display->display_options['relationships']['flag_content_rel_1']['table'] = 'past_event';
  $handler->display->display_options['relationships']['flag_content_rel_1']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_1']['label'] = 'Done flag';
  $handler->display->display_options['relationships']['flag_content_rel_1']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_1']['flag'] = 'event_done';
  $handler->display->display_options['relationships']['flag_content_rel_1']['user_scope'] = 'any';
  /* Field: Bulk operations: Past Event */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'past_event';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
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
    'rules_component::rules_mass_flag_as_done' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_mark_as_todo' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
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
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Past Event: Past event ID */
  $handler->display->display_options['fields']['event_id']['id'] = 'event_id';
  $handler->display->display_options['fields']['event_id']['table'] = 'past_event';
  $handler->display->display_options['fields']['event_id']['field'] = 'event_id';
  $handler->display->display_options['fields']['event_id']['label'] = 'ID';
  $handler->display->display_options['fields']['event_id']['separator'] = '';
  /* Field: Past Event: Severity */
  $handler->display->display_options['fields']['severity']['id'] = 'severity';
  $handler->display->display_options['fields']['severity']['table'] = 'past_event';
  $handler->display->display_options['fields']['severity']['field'] = 'severity';
  $handler->display->display_options['fields']['severity']['empty'] = 'no past logs found with selected severity';
  /* Field: Past Event: Module */
  $handler->display->display_options['fields']['module']['id'] = 'module';
  $handler->display->display_options['fields']['module']['table'] = 'past_event';
  $handler->display->display_options['fields']['module']['field'] = 'module';
  /* Field: Past Event: Machine_name */
  $handler->display->display_options['fields']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['fields']['machine_name']['table'] = 'past_event';
  $handler->display->display_options['fields']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['fields']['machine_name']['label'] = 'Machine name';
  /* Field: Past Event: Message */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'past_event';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  $handler->display->display_options['fields']['message']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['message']['alter']['path'] = 'admin/reports/past/[event_id]';
  $handler->display->display_options['fields']['message']['alter']['max_length'] = '100';
  $handler->display->display_options['fields']['message']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['message']['alter']['trim'] = TRUE;
  /* Field: Past Event: Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'past_event';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  /* Field: Flags: Flag link */
  $handler->display->display_options['fields']['ops']['id'] = 'ops';
  $handler->display->display_options['fields']['ops']['table'] = 'flag_content';
  $handler->display->display_options['fields']['ops']['field'] = 'ops';
  $handler->display->display_options['fields']['ops']['relationship'] = 'flag_content_rel_1';
  $handler->display->display_options['fields']['ops']['label'] = '';
  $handler->display->display_options['fields']['ops']['element_label_colon'] = FALSE;
  /* Sort criterion: Past Event: Past event ID */
  $handler->display->display_options['sorts']['event_id']['id'] = 'event_id';
  $handler->display->display_options['sorts']['event_id']['table'] = 'past_event';
  $handler->display->display_options['sorts']['event_id']['field'] = 'event_id';
  $handler->display->display_options['sorts']['event_id']['order'] = 'DESC';
  /* Contextual filter: Past Event: Past event ID */
  $handler->display->display_options['arguments']['event_id']['id'] = 'event_id';
  $handler->display->display_options['arguments']['event_id']['table'] = 'past_event';
  $handler->display->display_options['arguments']['event_id']['field'] = 'event_id';
  $handler->display->display_options['arguments']['event_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['event_id']['summary']['format'] = 'default_summary';
  /* Filter criterion: Past Event: Module */
  $handler->display->display_options['filters']['module']['id'] = 'module';
  $handler->display->display_options['filters']['module']['table'] = 'past_event';
  $handler->display->display_options['filters']['module']['field'] = 'module';
  $handler->display->display_options['filters']['module']['group'] = 1;
  $handler->display->display_options['filters']['module']['exposed'] = TRUE;
  $handler->display->display_options['filters']['module']['expose']['operator_id'] = 'module_op';
  $handler->display->display_options['filters']['module']['expose']['label'] = 'Module';
  $handler->display->display_options['filters']['module']['expose']['operator'] = 'module_op';
  $handler->display->display_options['filters']['module']['expose']['identifier'] = 'module';
  /* Filter criterion: Past Event: Severity */
  $handler->display->display_options['filters']['severity']['id'] = 'severity';
  $handler->display->display_options['filters']['severity']['table'] = 'past_event';
  $handler->display->display_options['filters']['severity']['field'] = 'severity';
  $handler->display->display_options['filters']['severity']['group'] = 1;
  $handler->display->display_options['filters']['severity']['exposed'] = TRUE;
  $handler->display->display_options['filters']['severity']['expose']['operator_id'] = 'severity_op';
  $handler->display->display_options['filters']['severity']['expose']['label'] = 'Severity';
  $handler->display->display_options['filters']['severity']['expose']['operator'] = 'severity_op';
  $handler->display->display_options['filters']['severity']['expose']['identifier'] = 'severity';
  $handler->display->display_options['filters']['severity']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['severity']['expose']['multiple'] = TRUE;
  /* Filter criterion: Past Event: Message */
  $handler->display->display_options['filters']['message']['id'] = 'message';
  $handler->display->display_options['filters']['message']['table'] = 'past_event';
  $handler->display->display_options['filters']['message']['field'] = 'message';
  $handler->display->display_options['filters']['message']['operator'] = 'contains';
  $handler->display->display_options['filters']['message']['group'] = 1;
  $handler->display->display_options['filters']['message']['exposed'] = TRUE;
  $handler->display->display_options['filters']['message']['expose']['operator_id'] = 'message_op';
  $handler->display->display_options['filters']['message']['expose']['label'] = 'Message';
  $handler->display->display_options['filters']['message']['expose']['operator'] = 'message_op';
  $handler->display->display_options['filters']['message']['expose']['identifier'] = 'message';
  $handler->display->display_options['filters']['message']['expose']['remember'] = TRUE;
  /* Filter criterion: Flags: Flagged */
  $handler->display->display_options['filters']['flagged']['id'] = 'flagged';
  $handler->display->display_options['filters']['flagged']['table'] = 'flag_content';
  $handler->display->display_options['filters']['flagged']['field'] = 'flagged';
  $handler->display->display_options['filters']['flagged']['relationship'] = 'flag_content_rel_1';
  $handler->display->display_options['filters']['flagged']['value'] = '0';

  /* Display: Page: Main view */
  $handler = $view->new_display('page', 'Page: Main view', 'past_logs_table');
  $handler->display->display_options['display_description'] = 'Shows a list with all past logentries';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['path'] = 'admin/reports/past/todo';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'TODO';
  $handler->display->display_options['menu']['description'] = 'Past event log messages that do still need to be fixed';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Past event log';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'main-menu';

  $views[$view->name] = $view;

  // Done view.
  $view = new view();
  $view->name = 'past_event_done';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'past_event';
  $view->human_name = 'Past event done';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Past event done';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer past';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'event_id' => 'event_id',
    'severity' => 'severity',
    'timestamp' => 'timestamp',
    'module' => 'module',
    'message' => 'message',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'event_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'severity' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
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
    'message' => array(
      'sortable' => 0,
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
  $handler->display->display_options['empty']['area']['content'] = 'No done events found.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Flags: event_todo */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'past_event';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['label'] = 'todo flag';
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'event_todo';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  /* Relationship: Flags: event_done */
  $handler->display->display_options['relationships']['flag_content_rel_1']['id'] = 'flag_content_rel_1';
  $handler->display->display_options['relationships']['flag_content_rel_1']['table'] = 'past_event';
  $handler->display->display_options['relationships']['flag_content_rel_1']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_1']['label'] = 'done flag';
  $handler->display->display_options['relationships']['flag_content_rel_1']['flag'] = 'event_done';
  $handler->display->display_options['relationships']['flag_content_rel_1']['user_scope'] = 'any';
  /* Field: Bulk operations: Past Event */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'past_event';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
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
    'rules_component::rules_mass_flag_as_done' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 0,
      'label' => '',
    ),
    'rules_component::rules_mark_as_todo' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
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
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Past Event: Past event ID */
  $handler->display->display_options['fields']['event_id']['id'] = 'event_id';
  $handler->display->display_options['fields']['event_id']['table'] = 'past_event';
  $handler->display->display_options['fields']['event_id']['field'] = 'event_id';
  $handler->display->display_options['fields']['event_id']['label'] = 'ID';
  $handler->display->display_options['fields']['event_id']['separator'] = '';
  /* Field: Past Event: Severity */
  $handler->display->display_options['fields']['severity']['id'] = 'severity';
  $handler->display->display_options['fields']['severity']['table'] = 'past_event';
  $handler->display->display_options['fields']['severity']['field'] = 'severity';
  $handler->display->display_options['fields']['severity']['empty'] = 'no past logs found with selected severity';
  /* Field: Past Event: Module */
  $handler->display->display_options['fields']['module']['id'] = 'module';
  $handler->display->display_options['fields']['module']['table'] = 'past_event';
  $handler->display->display_options['fields']['module']['field'] = 'module';
  /* Field: Past Event: Machine_name */
  $handler->display->display_options['fields']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['fields']['machine_name']['table'] = 'past_event';
  $handler->display->display_options['fields']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['fields']['machine_name']['label'] = 'Machine name';
  /* Field: Past Event: Message */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'past_event';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  $handler->display->display_options['fields']['message']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['message']['alter']['path'] = 'admin/reports/past/[event_id]';
  $handler->display->display_options['fields']['message']['alter']['max_length'] = '100';
  $handler->display->display_options['fields']['message']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['message']['alter']['trim'] = TRUE;
  /* Field: Past Event: Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'past_event';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  /* Field: Flags: Flag link */
  $handler->display->display_options['fields']['ops']['id'] = 'ops';
  $handler->display->display_options['fields']['ops']['table'] = 'flag_content';
  $handler->display->display_options['fields']['ops']['field'] = 'ops';
  $handler->display->display_options['fields']['ops']['relationship'] = 'flag_content_rel_1';
  $handler->display->display_options['fields']['ops']['label'] = '';
  $handler->display->display_options['fields']['ops']['element_label_colon'] = FALSE;
  /* Sort criterion: Past Event: Past event ID */
  $handler->display->display_options['sorts']['event_id']['id'] = 'event_id';
  $handler->display->display_options['sorts']['event_id']['table'] = 'past_event';
  $handler->display->display_options['sorts']['event_id']['field'] = 'event_id';
  $handler->display->display_options['sorts']['event_id']['order'] = 'DESC';
  /* Contextual filter: Past Event: Past event ID */
  $handler->display->display_options['arguments']['event_id']['id'] = 'event_id';
  $handler->display->display_options['arguments']['event_id']['table'] = 'past_event';
  $handler->display->display_options['arguments']['event_id']['field'] = 'event_id';
  $handler->display->display_options['arguments']['event_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['event_id']['summary']['format'] = 'default_summary';
  /* Filter criterion: Past Event: Module */
  $handler->display->display_options['filters']['module']['id'] = 'module';
  $handler->display->display_options['filters']['module']['table'] = 'past_event';
  $handler->display->display_options['filters']['module']['field'] = 'module';
  $handler->display->display_options['filters']['module']['group'] = 1;
  $handler->display->display_options['filters']['module']['exposed'] = TRUE;
  $handler->display->display_options['filters']['module']['expose']['operator_id'] = 'module_op';
  $handler->display->display_options['filters']['module']['expose']['label'] = 'Module';
  $handler->display->display_options['filters']['module']['expose']['operator'] = 'module_op';
  $handler->display->display_options['filters']['module']['expose']['identifier'] = 'module';
  /* Filter criterion: Past Event: Severity */
  $handler->display->display_options['filters']['severity']['id'] = 'severity';
  $handler->display->display_options['filters']['severity']['table'] = 'past_event';
  $handler->display->display_options['filters']['severity']['field'] = 'severity';
  $handler->display->display_options['filters']['severity']['group'] = 1;
  $handler->display->display_options['filters']['severity']['exposed'] = TRUE;
  $handler->display->display_options['filters']['severity']['expose']['operator_id'] = 'severity_op';
  $handler->display->display_options['filters']['severity']['expose']['label'] = 'Severity';
  $handler->display->display_options['filters']['severity']['expose']['operator'] = 'severity_op';
  $handler->display->display_options['filters']['severity']['expose']['identifier'] = 'severity';
  $handler->display->display_options['filters']['severity']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['severity']['expose']['multiple'] = TRUE;
  /* Filter criterion: Past Event: Message */
  $handler->display->display_options['filters']['message']['id'] = 'message';
  $handler->display->display_options['filters']['message']['table'] = 'past_event';
  $handler->display->display_options['filters']['message']['field'] = 'message';
  $handler->display->display_options['filters']['message']['operator'] = 'contains';
  $handler->display->display_options['filters']['message']['group'] = 1;
  $handler->display->display_options['filters']['message']['exposed'] = TRUE;
  $handler->display->display_options['filters']['message']['expose']['operator_id'] = 'message_op';
  $handler->display->display_options['filters']['message']['expose']['label'] = 'Message';
  $handler->display->display_options['filters']['message']['expose']['operator'] = 'message_op';
  $handler->display->display_options['filters']['message']['expose']['identifier'] = 'message';
  $handler->display->display_options['filters']['message']['expose']['remember'] = TRUE;

  /* Display: Page: Main view */
  $handler = $view->new_display('page', 'Page: Main view', 'past_logs_table');
  $handler->display->display_options['display_description'] = 'Shows a list with all past logentries';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['path'] = 'admin/reports/past/done';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Done';
  $handler->display->display_options['menu']['description'] = 'Past event log messages that were fixed already';
  $handler->display->display_options['menu']['weight'] = '3';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Past event log';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'main-menu';
  $views[$view->name] = $view;

  $view = new view();
  $view->name = 'past_similar_events';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'past_event';
  $view->human_name = 'Past similar events';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Past event log';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer past';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'event_id' => 'event_id',
    'severity' => 'severity',
    'timestamp' => 'timestamp',
    'module' => 'module',
    'message' => 'message',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'event_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'severity' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
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
    'message' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Past Event: Past event ID */
  $handler->display->display_options['fields']['event_id']['id'] = 'event_id';
  $handler->display->display_options['fields']['event_id']['table'] = 'past_event';
  $handler->display->display_options['fields']['event_id']['field'] = 'event_id';
  $handler->display->display_options['fields']['event_id']['label'] = 'ID';
  $handler->display->display_options['fields']['event_id']['separator'] = '';
  /* Field: Past Event: Severity */
  $handler->display->display_options['fields']['severity']['id'] = 'severity';
  $handler->display->display_options['fields']['severity']['table'] = 'past_event';
  $handler->display->display_options['fields']['severity']['field'] = 'severity';
  $handler->display->display_options['fields']['severity']['empty'] = 'no past logs found with selected severity';
  /* Field: Past Event: Module */
  $handler->display->display_options['fields']['module']['id'] = 'module';
  $handler->display->display_options['fields']['module']['table'] = 'past_event';
  $handler->display->display_options['fields']['module']['field'] = 'module';
  /* Field: Past Event: Machine_name */
  $handler->display->display_options['fields']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['fields']['machine_name']['table'] = 'past_event';
  $handler->display->display_options['fields']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['fields']['machine_name']['label'] = 'Machine name';
  /* Field: Past Event: Message */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'past_event';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  $handler->display->display_options['fields']['message']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['message']['alter']['path'] = 'admin/reports/past/[event_id]';
  $handler->display->display_options['fields']['message']['alter']['max_length'] = '100';
  $handler->display->display_options['fields']['message']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['message']['alter']['trim'] = TRUE;
  /* Field: Past Event: Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'past_event';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  /* Sort criterion: Past Event: Past event ID */
  $handler->display->display_options['sorts']['event_id']['id'] = 'event_id';
  $handler->display->display_options['sorts']['event_id']['table'] = 'past_event';
  $handler->display->display_options['sorts']['event_id']['field'] = 'event_id';
  $handler->display->display_options['sorts']['event_id']['order'] = 'DESC';
  /* Filter criterion: Past Event: Module */
  $handler->display->display_options['filters']['module']['id'] = 'module';
  $handler->display->display_options['filters']['module']['table'] = 'past_event';
  $handler->display->display_options['filters']['module']['field'] = 'module';
  $handler->display->display_options['filters']['module']['group'] = 1;
  $handler->display->display_options['filters']['module']['exposed'] = TRUE;
  $handler->display->display_options['filters']['module']['expose']['operator_id'] = 'module_op';
  $handler->display->display_options['filters']['module']['expose']['label'] = 'Module';
  $handler->display->display_options['filters']['module']['expose']['operator'] = 'module_op';
  $handler->display->display_options['filters']['module']['expose']['identifier'] = 'module';
  /* Filter criterion: Past Event: Severity */
  $handler->display->display_options['filters']['severity']['id'] = 'severity';
  $handler->display->display_options['filters']['severity']['table'] = 'past_event';
  $handler->display->display_options['filters']['severity']['field'] = 'severity';
  $handler->display->display_options['filters']['severity']['group'] = 1;
  $handler->display->display_options['filters']['severity']['exposed'] = TRUE;
  $handler->display->display_options['filters']['severity']['expose']['operator_id'] = 'severity_op';
  $handler->display->display_options['filters']['severity']['expose']['label'] = 'Severity';
  $handler->display->display_options['filters']['severity']['expose']['operator'] = 'severity_op';
  $handler->display->display_options['filters']['severity']['expose']['identifier'] = 'severity';
  $handler->display->display_options['filters']['severity']['expose']['multiple'] = TRUE;
  /* Filter criterion: Past Event: Message */
  $handler->display->display_options['filters']['message']['id'] = 'message';
  $handler->display->display_options['filters']['message']['table'] = 'past_event';
  $handler->display->display_options['filters']['message']['field'] = 'message';
  $handler->display->display_options['filters']['message']['operator'] = 'starts';
  $handler->display->display_options['filters']['message']['group'] = 1;
  $handler->display->display_options['filters']['message']['exposed'] = TRUE;
  $handler->display->display_options['filters']['message']['expose']['operator_id'] = 'message_op';
  $handler->display->display_options['filters']['message']['expose']['label'] = 'Message';
  $handler->display->display_options['filters']['message']['expose']['operator'] = 'message_op';
  $handler->display->display_options['filters']['message']['expose']['identifier'] = 'message';
  /* Filter criterion: Past Event: Past event ID */
  $handler->display->display_options['filters']['event_id']['id'] = 'event_id';
  $handler->display->display_options['filters']['event_id']['table'] = 'past_event';
  $handler->display->display_options['filters']['event_id']['field'] = 'event_id';
  $handler->display->display_options['filters']['event_id']['operator'] = '!=';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/reports/past/suggested';
  $translatables['past_similar_events'] = array(
    t('Master'),
    t('Past event log'),
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
    t('ID'),
    t('.'),
    t('Severity'),
    t('No past logs found with selected severity'),
    t('Module'),
    t('Machine name'),
    t('Message'),
    t('Timestamp'),
    t('Page'),
  );
  $views[$view->name] = $view;

  return $views;
}

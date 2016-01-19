<?php
/**
 * @file
 * comstack_pm_report.views_default
 */

/**
 * Implements hook_views_default_views().
 */
function comstack_pm_report_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'cs_pm_report__reported';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'comstack_pm_report';
  $view->human_name = 'Comstack Private Messaging - Reported conversations';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Reported conversations';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view comstack_pm_report entries';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'created' => 'created',
    'name' => 'name',
    'field_cs_pm_report_additional' => 'field_cs_pm_report_additional',
    'field_cs_pm_report_reason' => 'field_cs_pm_report_reason',
    'nothing_1' => 'nothing_1',
    'cs_pm_historical_participants' => 'cs_pm_historical_participants',
    'view_conversation' => 'view_conversation',
    'dismiss_report' => 'view_conversation',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cs_pm_report_additional' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_cs_pm_report_reason' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cs_pm_historical_participants' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'view_conversation' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'dismiss_report' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = '<p>No conversations have been reported.</p>';
  /* Relationship: Comstack PM Report: Reporter */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'comstack_pm_report';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'reporter';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_cs_pm_report_conversation_target_id']['id'] = 'field_cs_pm_report_conversation_target_id';
  $handler->display->display_options['relationships']['field_cs_pm_report_conversation_target_id']['table'] = 'field_data_field_cs_pm_report_conversation';
  $handler->display->display_options['relationships']['field_cs_pm_report_conversation_target_id']['field'] = 'field_cs_pm_report_conversation_target_id';
  $handler->display->display_options['relationships']['field_cs_pm_report_conversation_target_id']['label'] = 'conversation';
  $handler->display->display_options['relationships']['field_cs_pm_report_conversation_target_id']['required'] = TRUE;
  /* Field: Comstack PM Report: Report created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'comstack_pm_report';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Reported';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Reported by';
  /* Field: Comstack PM Report: Additional information */
  $handler->display->display_options['fields']['field_cs_pm_report_additional']['id'] = 'field_cs_pm_report_additional';
  $handler->display->display_options['fields']['field_cs_pm_report_additional']['table'] = 'field_data_field_cs_pm_report_additional';
  $handler->display->display_options['fields']['field_cs_pm_report_additional']['field'] = 'field_cs_pm_report_additional';
  $handler->display->display_options['fields']['field_cs_pm_report_additional']['label'] = '';
  $handler->display->display_options['fields']['field_cs_pm_report_additional']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_cs_pm_report_additional']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_cs_pm_report_additional']['alter']['text'] = '<br><strong>Additional information:</strong><br>[field_cs_pm_report_additional]';
  $handler->display->display_options['fields']['field_cs_pm_report_additional']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_cs_pm_report_additional']['hide_empty'] = TRUE;
  /* Field: Comstack PM Report: Reason */
  $handler->display->display_options['fields']['field_cs_pm_report_reason']['id'] = 'field_cs_pm_report_reason';
  $handler->display->display_options['fields']['field_cs_pm_report_reason']['table'] = 'field_data_field_cs_pm_report_reason';
  $handler->display->display_options['fields']['field_cs_pm_report_reason']['field'] = 'field_cs_pm_report_reason';
  $handler->display->display_options['fields']['field_cs_pm_report_reason']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_cs_pm_report_reason']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['field_cs_pm_report_reason']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_cs_pm_report_reason']['multi_type'] = 'ul';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'Reasons';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '[field_cs_pm_report_reason][field_cs_pm_report_additional]';
  /* Field: Comstack Conversation: Conversation participants */
  $handler->display->display_options['fields']['participants']['id'] = 'participants';
  $handler->display->display_options['fields']['participants']['table'] = 'comstack_conversation';
  $handler->display->display_options['fields']['participants']['field'] = 'participants';
  $handler->display->display_options['fields']['participants']['relationship'] = 'field_cs_pm_report_conversation_target_id';
  /* Field: Comstack Conversation: Conversation historical participants */
  $handler->display->display_options['fields']['historical_participants']['id'] = 'historical_participants';
  $handler->display->display_options['fields']['historical_participants']['table'] = 'comstack_conversation';
  $handler->display->display_options['fields']['historical_participants']['field'] = 'historical_participants';
  $handler->display->display_options['fields']['historical_participants']['relationship'] = 'field_cs_pm_report_conversation_target_id';
  $handler->display->display_options['fields']['historical_participants']['label'] = 'Historical participants';
  /* Field: Comstack PM Report: Link to, or AJAX view the conversation */
  $handler->display->display_options['fields']['view_conversation']['id'] = 'view_conversation';
  $handler->display->display_options['fields']['view_conversation']['table'] = 'comstack_pm_report';
  $handler->display->display_options['fields']['view_conversation']['field'] = 'view_conversation';
  $handler->display->display_options['fields']['view_conversation']['label'] = '';
  $handler->display->display_options['fields']['view_conversation']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['view_conversation']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['view_conversation']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['view_conversation']['newline_prepend'] = FALSE;
  /* Field: Comstack PM Report: Link to dismiss or AJAX dismiss a report. */
  $handler->display->display_options['fields']['dismiss_report']['id'] = 'dismiss_report';
  $handler->display->display_options['fields']['dismiss_report']['table'] = 'comstack_pm_report';
  $handler->display->display_options['fields']['dismiss_report']['field'] = 'dismiss_report';
  $handler->display->display_options['fields']['dismiss_report']['label'] = '';
  $handler->display->display_options['fields']['dismiss_report']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['dismiss_report']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['dismiss_report']['element_default_classes'] = FALSE;
  /* Filter criterion: Comstack PM Report: Dismissed */
  $handler->display->display_options['filters']['dismissed']['id'] = 'dismissed';
  $handler->display->display_options['filters']['dismissed']['table'] = 'comstack_pm_report';
  $handler->display->display_options['filters']['dismissed']['field'] = 'dismissed';
  $handler->display->display_options['filters']['dismissed']['value'] = '0';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/reported-conversations';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Reported conversations';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['cs_pm_report__reported'] = $view;

  $view = new view();
  $view->name = 'cs_pm_report__view_conversation';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'message';
  $view->human_name = 'Comstack Report - View reported conversation';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view comstack_pm_report entries';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options'] = '10, 25, 50, 100';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'cs_pm_text' => 'cs_pm_text',
    'name' => 'name',
    'timestamp' => 'timestamp',
  );
  $handler->display->display_options['style_options']['default'] = 'timestamp';
  $handler->display->display_options['style_options']['info'] = array(
    'cs_pm_text' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
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
  );
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = '<p class="help-text">There aren\'t any messages in this conversation... that isn\'t right.</p>';
  /* Relationship: Message: User uid */
  $handler->display->display_options['relationships']['user']['id'] = 'user';
  $handler->display->display_options['relationships']['user']['table'] = 'message';
  $handler->display->display_options['relationships']['user']['field'] = 'user';
  $handler->display->display_options['relationships']['user']['required'] = TRUE;
  /* Field: Message: Text */
  $handler->display->display_options['fields']['cs_pm_text']['id'] = 'cs_pm_text';
  $handler->display->display_options['fields']['cs_pm_text']['table'] = 'field_data_cs_pm_text';
  $handler->display->display_options['fields']['cs_pm_text']['field'] = 'cs_pm_text';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'user';
  $handler->display->display_options['fields']['name']['label'] = 'Sender';
  /* Field: Message: Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'message';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['label'] = 'Sent';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  $handler->display->display_options['fields']['timestamp']['second_date_format'] = 'long';
  /* Contextual filter: Message: Conversation (cs_pm_conversation) */
  $handler->display->display_options['arguments']['cs_pm_conversation_target_id']['id'] = 'cs_pm_conversation_target_id';
  $handler->display->display_options['arguments']['cs_pm_conversation_target_id']['table'] = 'field_data_cs_pm_conversation';
  $handler->display->display_options['arguments']['cs_pm_conversation_target_id']['field'] = 'cs_pm_conversation_target_id';
  $handler->display->display_options['arguments']['cs_pm_conversation_target_id']['default_action'] = 'access denied';
  $handler->display->display_options['arguments']['cs_pm_conversation_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['cs_pm_conversation_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['cs_pm_conversation_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['cs_pm_conversation_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['cs_pm_conversation_target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['cs_pm_conversation_target_id']['validate']['type'] = 'numeric';
  $handler->display->display_options['arguments']['cs_pm_conversation_target_id']['validate']['fail'] = 'access denied';
  /* Filter criterion: Message: Text (cs_pm_text) */
  $handler->display->display_options['filters']['cs_pm_text_value']['id'] = 'cs_pm_text_value';
  $handler->display->display_options['filters']['cs_pm_text_value']['table'] = 'field_data_cs_pm_text';
  $handler->display->display_options['filters']['cs_pm_text_value']['field'] = 'cs_pm_text_value';
  $handler->display->display_options['filters']['cs_pm_text_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['cs_pm_text_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['cs_pm_text_value']['expose']['operator_id'] = 'cs_pm_text_value_op';
  $handler->display->display_options['filters']['cs_pm_text_value']['expose']['label'] = 'Text contains';
  $handler->display->display_options['filters']['cs_pm_text_value']['expose']['operator'] = 'cs_pm_text_value_op';
  $handler->display->display_options['filters']['cs_pm_text_value']['expose']['identifier'] = 'cs_pm_text_value';
  $handler->display->display_options['filters']['cs_pm_text_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    3 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'user';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Sender';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    4 => 0,
    3 => 0,
  );
  $export['cs_pm_report__view_conversation'] = $view;

  return $export;
}

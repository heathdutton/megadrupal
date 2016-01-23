<?php
/**
 * @file
 * @brief Views to ces message
 *
 * @defgroup ces_message_views Ces Message View
 * @ingroup ces_message
 * @{
 * View from message.
 */

/**
 * Implements hook_views_default_views().
 */
function ces_message_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'ces_message_user';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'ces_message';
  $view->human_name = 'Ces Message';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Ces Message';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = t('Reset');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« ' . t('first');
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ ' . t('previous');
  $handler->display->display_options['pager']['options']['tags']['next'] = t('next') . ' ›';
  $handler->display->display_options['pager']['options']['tags']['last'] = t('last') . ' »';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'mid_1' => 'mid_1',
    'created' => 'created',
    'subject' => 'subject',
    'shortname' => 'shortname',
  );
  $handler->display->display_options['style_options']['default'] = 'mid_1';
  $handler->display->display_options['style_options']['info'] = array(
    'mid_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
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
    'subject' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'shortname' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Ces Message: Ces message Exchange relationship */
  $handler->display->display_options['relationships']['exchange']['id'] = 'exchange';
  $handler->display->display_options['relationships']['exchange']['table'] = 'ces_message';
  $handler->display->display_options['relationships']['exchange']['field'] = 'exchange';
  $handler->display->display_options['relationships']['exchange']['required'] = TRUE;
  /* Relationship: Ces Message: Ces message users relationship */
  $handler->display->display_options['relationships']['user']['id'] = 'user';
  $handler->display->display_options['relationships']['user']['table'] = 'ces_message';
  $handler->display->display_options['relationships']['user']['field'] = 'user';
  $handler->display->display_options['relationships']['user']['required'] = TRUE;
  /* Camp: Ces Message: Ces Message Id */
  $handler->display->display_options['fields']['mid_1']['id'] = 'mid_1';
  $handler->display->display_options['fields']['mid_1']['table'] = 'ces_message';
  $handler->display->display_options['fields']['mid_1']['field'] = 'mid';
  $handler->display->display_options['fields']['mid_1']['label'] = 'Id';
  $handler->display->display_options['fields']['mid_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['mid_1']['separator'] = '';
  /* Camp: Ces Message: Date message */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'ces_message';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = t('Date');
  $handler->display->display_options['fields']['created']['date_format'] = 'medium';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Camp: Ces Message: Assumpte */
  $handler->display->display_options['fields']['subject']['id'] = 'subject';
  $handler->display->display_options['fields']['subject']['table'] = 'ces_message';
  $handler->display->display_options['fields']['subject']['field'] = 'subject';
  $handler->display->display_options['fields']['subject']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['subject']['alter']['path'] = 'ces/bank/account/message/[mid_1]/view';
  /* Camp: Ces Exchange: Exchange Shortname */
  $handler->display->display_options['fields']['shortname']['id'] = 'shortname';
  $handler->display->display_options['fields']['shortname']['table'] = 'ces_exchange';
  $handler->display->display_options['fields']['shortname']['field'] = 'shortname';
  $handler->display->display_options['fields']['shortname']['relationship'] = 'exchange';
  $handler->display->display_options['fields']['shortname']['label'] = t('Exchange');
  /* Contextual filter: Ces Message: User Id */
  $handler->display->display_options['arguments']['user']['id'] = 'user';
  $handler->display->display_options['arguments']['user']['table'] = 'ces_message';
  $handler->display->display_options['arguments']['user']['field'] = 'user';
  $handler->display->display_options['arguments']['user']['default_action'] = 'default';
  $handler->display->display_options['arguments']['user']['exception']['title'] = 'Tot';
  $handler->display->display_options['arguments']['user']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['user']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['user']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['user']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['user']['limit'] = '0';
  /* Filter criterion: Ces Message: Assumpte */
  $handler->display->display_options['filters']['subject']['id'] = 'subject';
  $handler->display->display_options['filters']['subject']['table'] = 'ces_message';
  $handler->display->display_options['filters']['subject']['field'] = 'subject';
  $handler->display->display_options['filters']['subject']['operator'] = 'contains';
  $handler->display->display_options['filters']['subject']['exposed'] = TRUE;
  $handler->display->display_options['filters']['subject']['expose']['operator_id'] = 'subject_op';
  $handler->display->display_options['filters']['subject']['expose']['label'] = t('Subject');
  $handler->display->display_options['filters']['subject']['expose']['operator'] = 'subject_op';
  $handler->display->display_options['filters']['subject']['expose']['identifier'] = 'subject';
  $handler->display->display_options['filters']['subject']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Ces Message: Date message */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'ces_message';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['operator'] = '<';
  $handler->display->display_options['filters']['created']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = t('Date');
  $handler->display->display_options['filters']['created']['expose']['description'] = 'A date in any machine readable format. CCYY-MM-DD HH:MM:SS is preferred.';
  $handler->display->display_options['filters']['created']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
  $handler->display->display_options['filters']['created']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Ces Exchange: Exchange Shortname */
  $handler->display->display_options['filters']['shortname']['id'] = 'shortname';
  $handler->display->display_options['filters']['shortname']['table'] = 'ces_exchange';
  $handler->display->display_options['filters']['shortname']['field'] = 'shortname';
  $handler->display->display_options['filters']['shortname']['relationship'] = 'exchange';
  $handler->display->display_options['filters']['shortname']['operator'] = 'contains';
  $handler->display->display_options['filters']['shortname']['exposed'] = TRUE;
  $handler->display->display_options['filters']['shortname']['expose']['operator_id'] = 'shortname_op';
  $handler->display->display_options['filters']['shortname']['expose']['label'] = t('Exchange');
  $handler->display->display_options['filters']['shortname']['expose']['operator'] = 'shortname_op';
  $handler->display->display_options['filters']['shortname']['expose']['identifier'] = 'shortname';
  $handler->display->display_options['filters']['shortname']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'ces-message';
  $translatables['ces_message_user'] = array(
    t('Master'),
    t('Ces Message'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    '« ' . t('first'),
    '‹ ' . t('previous'),
    t('next') . ' ›',
    t('last') . ' »',
    t('Ces message Exchange relationship'),
    t('Ces message users relationship'),
    t('Id'),
    t('.'),
    t('Data'),
    t('Assumpte'),
    t('Exchange'),
    t('Tot'),
    t('Date'),
    t('A date in any machine readable format. CCYY-MM-DD HH:MM:SS is preferred.'),
    t('Page'),
  );

  $export['messages'] = $view;

  $view = new view();
  $view->name = 'ces_message_admin';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'ces_message';
  $view->human_name = 'Ces Message Admin';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Ces Messages Admin';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = t('Reset');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['tags']['first'] = '« ' . t('first');
  $handler->display->display_options['pager']['options']['tags']['previous'] = '‹ ' . t('previous');
  $handler->display->display_options['pager']['options']['tags']['next'] = t('next') . ' ›';
  $handler->display->display_options['pager']['options']['tags']['last'] = t('last') . ' »';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'mid' => 'mid',
    'created' => 'created',
    'subject' => 'subject',
    'type_message' => 'type_message',
    'exchange' => 'exchange',
    'shortname' => 'shortname',
    'name' => 'name',
  );
  $handler->display->display_options['style_options']['default'] = 'mid';
  $handler->display->display_options['style_options']['info'] = array(
    'mid' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
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
    'subject' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type_message' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'exchange' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'shortname' => array(
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
  );
  /* Relationship: Ces Message: Ces message users relationship */
  $handler->display->display_options['relationships']['exchange']['id'] = 'exchange';
  $handler->display->display_options['relationships']['exchange']['table'] = 'ces_message';
  $handler->display->display_options['relationships']['exchange']['field'] = 'exchange';
  /* Relationship: Ces Message: Ces message users relationship */
  $handler->display->display_options['relationships']['user']['id'] = 'user';
  $handler->display->display_options['relationships']['user']['table'] = 'ces_message';
  $handler->display->display_options['relationships']['user']['field'] = 'user';
  /* Camp: Ces Message: Ces Message Id */
  $handler->display->display_options['fields']['mid']['id'] = 'mid';
  $handler->display->display_options['fields']['mid']['table'] = 'ces_message';
  $handler->display->display_options['fields']['mid']['field'] = 'mid';
  $handler->display->display_options['fields']['mid']['label'] = 'Id';
  $handler->display->display_options['fields']['mid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['mid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['mid']['separator'] = '';
  /* Camp: Ces Message: Date message */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'ces_message';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = t('Data');
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Camp: Ces Message: Assumpte */
  $handler->display->display_options['fields']['subject']['id'] = 'subject';
  $handler->display->display_options['fields']['subject']['table'] = 'ces_message';
  $handler->display->display_options['fields']['subject']['field'] = 'subject';
  $handler->display->display_options['fields']['subject']['label'] = t('Subject');
  $handler->display->display_options['fields']['subject']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['subject']['alter']['path'] = 'ces/bank/account/message/[mid]/view';
  $handler->display->display_options['fields']['subject']['element_label_colon'] = FALSE;
  /* Camp: Ces Message: Tipus */
  $handler->display->display_options['fields']['type_message']['id'] = 'type_message';
  $handler->display->display_options['fields']['type_message']['table'] = 'ces_message';
  $handler->display->display_options['fields']['type_message']['field'] = 'type_message';
  $handler->display->display_options['fields']['type_message']['label'] = t('Type');
  $handler->display->display_options['fields']['type_message']['element_label_colon'] = FALSE;
  /* Camp: Ces Message: Exchange */
  $handler->display->display_options['fields']['exchange']['id'] = 'exchange';
  $handler->display->display_options['fields']['exchange']['table'] = 'ces_message';
  $handler->display->display_options['fields']['exchange']['field'] = 'exchange';
  $handler->display->display_options['fields']['exchange']['exclude'] = TRUE;
  /* Camp: Ces Exchange: Exchange Shortname */
  $handler->display->display_options['fields']['shortname']['id'] = 'shortname';
  $handler->display->display_options['fields']['shortname']['table'] = 'ces_exchange';
  $handler->display->display_options['fields']['shortname']['field'] = 'shortname';
  $handler->display->display_options['fields']['shortname']['relationship'] = 'exchange';
  $handler->display->display_options['fields']['shortname']['label'] = t('Exchange');
  /* Camp: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'user';
  $handler->display->display_options['fields']['name']['label'] = t('User');
  /* Filter criterion: Ces Exchange: Exchange Shortname */
  $handler->display->display_options['filters']['shortname']['id'] = 'shortname';
  $handler->display->display_options['filters']['shortname']['table'] = 'ces_exchange';
  $handler->display->display_options['filters']['shortname']['field'] = 'shortname';
  $handler->display->display_options['filters']['shortname']['relationship'] = 'exchange';
  $handler->display->display_options['filters']['shortname']['operator'] = 'contains';
  $handler->display->display_options['filters']['shortname']['exposed'] = TRUE;
  $handler->display->display_options['filters']['shortname']['expose']['operator_id'] = 'shortname_op';
  $handler->display->display_options['filters']['shortname']['expose']['label'] = t('Exchange');
  $handler->display->display_options['filters']['shortname']['expose']['operator'] = 'shortname_op';
  $handler->display->display_options['filters']['shortname']['expose']['identifier'] = 'shortname';
  $handler->display->display_options['filters']['shortname']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Ces Message: Assumpte */
  $handler->display->display_options['filters']['subject']['id'] = 'subject';
  $handler->display->display_options['filters']['subject']['table'] = 'ces_message';
  $handler->display->display_options['filters']['subject']['field'] = 'subject';
  $handler->display->display_options['filters']['subject']['operator'] = 'contains';
  $handler->display->display_options['filters']['subject']['exposed'] = TRUE;
  $handler->display->display_options['filters']['subject']['expose']['operator_id'] = 'subject_op';
  $handler->display->display_options['filters']['subject']['expose']['label'] = t('Subject');
  $handler->display->display_options['filters']['subject']['expose']['operator'] = 'subject_op';
  $handler->display->display_options['filters']['subject']['expose']['identifier'] = 'subject';
  $handler->display->display_options['filters']['subject']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Ces Message: Type */
  $handler->display->display_options['filters']['type_message']['id'] = 'type_message';
  $handler->display->display_options['filters']['type_message']['table'] = 'ces_message';
  $handler->display->display_options['filters']['type_message']['field'] = 'type_message';
  $handler->display->display_options['filters']['type_message']['operator'] = 'contains';
  $handler->display->display_options['filters']['type_message']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type_message']['expose']['operator_id'] = 'type_message_op';
  $handler->display->display_options['filters']['type_message']['expose']['label'] = t('Type');
  $handler->display->display_options['filters']['type_message']['expose']['operator'] = 'type_message_op';
  $handler->display->display_options['filters']['type_message']['expose']['identifier'] = 'type_message';
  $handler->display->display_options['filters']['type_message']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
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
  $handler->display->display_options['filters']['uid']['expose']['label'] = t('Name');
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'ces/messages/admin';
  $translatables['ces_message_admin'] = array(
    t('Master'),
    t('Ces Messages Admin'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    '« ' . t('first'),
    '‹ ' . t('previous'),
    t('next') . ' ›',
    t('last') . ' »',
    t('Ces message users relationship'),
    t('Id'),
    t('.'),
    t('Data'),
    t('Subject'),
    t('Type'),
    t('Exchange'),
    t('User'),
    t('Subject'),
    t('Type'),
    t('Name'),
    t('Page'),
  );

  $export['messages_all'] = $view;

  return $export;
}
/**
 * @} End of "defgroup ces_message_view".
 */

<?php

/**
 * @file Default Node registration views
 */

/**
 * Implements hook_views_default_views().
 */
function node_registration_views_default_views() {
  $vbo = module_exists('views_bulk_operations');

  $views = array();

$view = new view();
$view->name = 'node_registrations';
$view->description = '';
$view->tag = 'node_registration';
$view->base_table = 'node_registration';
$view->human_name = 'Node registrations';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Registrations';
$handler->display->display_options['css_class'] = 'node-registration';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'administer node registration';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'attended_1' => 'attended_1',
  'cancelled_1' => 'cancelled_1',
  'verified_1' => 'verified_1',
  'waitinglist_1' => 'waitinglist_1',
  'views_bulk_operations' => 'views_bulk_operations',
  'registration_id' => 'registration_id',
  'email' => 'email',
  'title' => 'title',
  'name' => 'name',
  'slots' => 'slots',
  'created' => 'created',
  'cancelled' => 'cancelled',
  'waitinglist' => 'waitinglist',
  'verified' => 'verified',
  'attended' => 'attended',
  'edit_node_registration' => 'edit_node_registration',
  'view_node_registration' => 'view_node_registration',
  'cancel_node_registration' => 'cancel_node_registration',
  'delete_node_registration' => 'delete_node_registration',
  'nothing' => 'nothing',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'attended_1' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'cancelled_1' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'verified_1' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'waitinglist_1' => array(
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
  'registration_id' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'email' => array(
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
  'name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'slots' => array(
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
  'cancelled' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'waitinglist' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'verified' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'attended' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'edit_node_registration' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'view_node_registration' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'cancel_node_registration' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'delete_node_registration' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'nothing' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* Relationship: Node registration: User */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'node_registration';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
/* Relationship: Node registration: Event node */
$handler->display->display_options['relationships']['nid']['id'] = 'nid';
$handler->display->display_options['relationships']['nid']['table'] = 'node_registration';
$handler->display->display_options['relationships']['nid']['field'] = 'nid';
$handler->display->display_options['relationships']['nid']['required'] = TRUE;
/* Field: Node registration: Attended */
$handler->display->display_options['fields']['attended_1']['id'] = 'attended_1';
$handler->display->display_options['fields']['attended_1']['table'] = 'node_registration';
$handler->display->display_options['fields']['attended_1']['field'] = 'attended';
$handler->display->display_options['fields']['attended_1']['label'] = '';
$handler->display->display_options['fields']['attended_1']['exclude'] = TRUE;
$handler->display->display_options['fields']['attended_1']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['attended_1']['type'] = 'y-n';
$handler->display->display_options['fields']['attended_1']['not'] = 0;
/* Field: Node registration: Cancelled */
$handler->display->display_options['fields']['cancelled_1']['id'] = 'cancelled_1';
$handler->display->display_options['fields']['cancelled_1']['table'] = 'node_registration';
$handler->display->display_options['fields']['cancelled_1']['field'] = 'cancelled';
$handler->display->display_options['fields']['cancelled_1']['label'] = '';
$handler->display->display_options['fields']['cancelled_1']['exclude'] = TRUE;
$handler->display->display_options['fields']['cancelled_1']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['cancelled_1']['type'] = 'y-n';
$handler->display->display_options['fields']['cancelled_1']['not'] = 0;
/* Field: Node registration: Verified */
$handler->display->display_options['fields']['verified_1']['id'] = 'verified_1';
$handler->display->display_options['fields']['verified_1']['table'] = 'node_registration';
$handler->display->display_options['fields']['verified_1']['field'] = 'verified';
$handler->display->display_options['fields']['verified_1']['label'] = '';
$handler->display->display_options['fields']['verified_1']['exclude'] = TRUE;
$handler->display->display_options['fields']['verified_1']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['verified_1']['type'] = 'y-n';
$handler->display->display_options['fields']['verified_1']['not'] = 0;
/* Field: Node registration: Waitinglist */
$handler->display->display_options['fields']['waitinglist_1']['id'] = 'waitinglist_1';
$handler->display->display_options['fields']['waitinglist_1']['table'] = 'node_registration';
$handler->display->display_options['fields']['waitinglist_1']['field'] = 'waitinglist';
$handler->display->display_options['fields']['waitinglist_1']['label'] = '';
$handler->display->display_options['fields']['waitinglist_1']['exclude'] = TRUE;
$handler->display->display_options['fields']['waitinglist_1']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['waitinglist_1']['type'] = 'y-n';
$handler->display->display_options['fields']['waitinglist_1']['not'] = 0;
/* Field: Bulk operations: Node registration */
$handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node_registration';
$handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
$handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '20';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
  'action::node_registration_attend_action' => array(
    'selected' => 1,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'postpone_processing' => 0,
  ),
  'action::node_registration_cancel_action' => array(
    'selected' => 1,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'postpone_processing' => 0,
  ),
  'action::views_bulk_operations_delete_item' => array(
    'selected' => 1,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'postpone_processing' => 0,
  ),
  'action::node_registration_send_mail_action' => array(
    'selected' => 1,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'postpone_processing' => 0,
  ),
  'action::node_registration_unattend_action' => array(
    'selected' => 1,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'postpone_processing' => 0,
  ),
  'action::node_registration_unverify_action' => array(
    'selected' => 1,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'postpone_processing' => 0,
  ),
  'action::node_registration_verify_action' => array(
    'selected' => 1,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'postpone_processing' => 0,
  ),
);
/* Field: Node registration: Registration ID */
$handler->display->display_options['fields']['registration_id']['id'] = 'registration_id';
$handler->display->display_options['fields']['registration_id']['table'] = 'node_registration';
$handler->display->display_options['fields']['registration_id']['field'] = 'registration_id';
$handler->display->display_options['fields']['registration_id']['label'] = 'ID';
$handler->display->display_options['fields']['registration_id']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['registration_id']['link_to_node_registration'] = 1;
$handler->display->display_options['fields']['registration_id']['link_to_node_registration_edit'] = 0;
$handler->display->display_options['fields']['registration_id']['link_to_node_registration_email'] = 0;
/* Field: Node registration: E-mail */
$handler->display->display_options['fields']['email']['id'] = 'email';
$handler->display->display_options['fields']['email']['table'] = 'node_registration';
$handler->display->display_options['fields']['email']['field'] = 'email';
$handler->display->display_options['fields']['email']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['email']['link_to_node_registration'] = 0;
$handler->display->display_options['fields']['email']['link_to_node_registration_edit'] = 1;
$handler->display->display_options['fields']['email']['link_to_node_registration_email'] = 0;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['relationship'] = 'nid';
$handler->display->display_options['fields']['title']['label'] = 'Event';
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'uid';
$handler->display->display_options['fields']['name']['label'] = 'User';
$handler->display->display_options['fields']['name']['element_default_classes'] = FALSE;
/* Field: Node registration: Slots */
$handler->display->display_options['fields']['slots']['id'] = 'slots';
$handler->display->display_options['fields']['slots']['table'] = 'node_registration';
$handler->display->display_options['fields']['slots']['field'] = 'slots';
$handler->display->display_options['fields']['slots']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['slots']['separator'] = '';
/* Field: Node registration: Created */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'node_registration';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['created']['date_format'] = 'short';
/* Field: Node registration: Cancelled */
$handler->display->display_options['fields']['cancelled']['id'] = 'cancelled';
$handler->display->display_options['fields']['cancelled']['table'] = 'node_registration';
$handler->display->display_options['fields']['cancelled']['field'] = 'cancelled';
$handler->display->display_options['fields']['cancelled']['element_class'] = 'cancelled [cancelled_1]';
$handler->display->display_options['fields']['cancelled']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['cancelled']['not'] = 0;
/* Field: Node registration: Waitinglist */
$handler->display->display_options['fields']['waitinglist']['id'] = 'waitinglist';
$handler->display->display_options['fields']['waitinglist']['table'] = 'node_registration';
$handler->display->display_options['fields']['waitinglist']['field'] = 'waitinglist';
$handler->display->display_options['fields']['waitinglist']['element_class'] = 'waitinglist [waitinglist_1]';
$handler->display->display_options['fields']['waitinglist']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['waitinglist']['not'] = 0;
/* Field: Node registration: Verified */
$handler->display->display_options['fields']['verified']['id'] = 'verified';
$handler->display->display_options['fields']['verified']['table'] = 'node_registration';
$handler->display->display_options['fields']['verified']['field'] = 'verified';
$handler->display->display_options['fields']['verified']['element_class'] = 'verified [verified_1]';
$handler->display->display_options['fields']['verified']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['verified']['not'] = 0;
/* Field: Node registration: Attended */
$handler->display->display_options['fields']['attended']['id'] = 'attended';
$handler->display->display_options['fields']['attended']['table'] = 'node_registration';
$handler->display->display_options['fields']['attended']['field'] = 'attended';
$handler->display->display_options['fields']['attended']['element_class'] = 'attended [attended_1]';
$handler->display->display_options['fields']['attended']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['attended']['not'] = 0;
/* Field: Node registration: Edit link */
$handler->display->display_options['fields']['edit_node_registration']['id'] = 'edit_node_registration';
$handler->display->display_options['fields']['edit_node_registration']['table'] = 'views_entity_node_registration';
$handler->display->display_options['fields']['edit_node_registration']['field'] = 'edit_node_registration';
$handler->display->display_options['fields']['edit_node_registration']['label'] = '';
$handler->display->display_options['fields']['edit_node_registration']['exclude'] = TRUE;
$handler->display->display_options['fields']['edit_node_registration']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['edit_node_registration']['element_default_classes'] = FALSE;
/* Field: Node registration: View link */
$handler->display->display_options['fields']['view_node_registration']['id'] = 'view_node_registration';
$handler->display->display_options['fields']['view_node_registration']['table'] = 'views_entity_node_registration';
$handler->display->display_options['fields']['view_node_registration']['field'] = 'view_node_registration';
$handler->display->display_options['fields']['view_node_registration']['label'] = '';
$handler->display->display_options['fields']['view_node_registration']['exclude'] = TRUE;
$handler->display->display_options['fields']['view_node_registration']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['view_node_registration']['element_default_classes'] = FALSE;
/* Field: Node registration: Cancel link */
$handler->display->display_options['fields']['cancel_node_registration']['id'] = 'cancel_node_registration';
$handler->display->display_options['fields']['cancel_node_registration']['table'] = 'views_entity_node_registration';
$handler->display->display_options['fields']['cancel_node_registration']['field'] = 'cancel_node_registration';
$handler->display->display_options['fields']['cancel_node_registration']['label'] = '';
$handler->display->display_options['fields']['cancel_node_registration']['exclude'] = TRUE;
$handler->display->display_options['fields']['cancel_node_registration']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['cancel_node_registration']['element_default_classes'] = FALSE;
/* Field: Node registration: Delete link */
$handler->display->display_options['fields']['delete_node_registration']['id'] = 'delete_node_registration';
$handler->display->display_options['fields']['delete_node_registration']['table'] = 'views_entity_node_registration';
$handler->display->display_options['fields']['delete_node_registration']['field'] = 'delete_node_registration';
$handler->display->display_options['fields']['delete_node_registration']['label'] = '';
$handler->display->display_options['fields']['delete_node_registration']['exclude'] = TRUE;
$handler->display->display_options['fields']['delete_node_registration']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['delete_node_registration']['element_default_classes'] = FALSE;
/* Field: Global: Custom text */
$handler->display->display_options['fields']['nothing']['id'] = 'nothing';
$handler->display->display_options['fields']['nothing']['table'] = 'views';
$handler->display->display_options['fields']['nothing']['field'] = 'nothing';
$handler->display->display_options['fields']['nothing']['label'] = 'Actions';
$handler->display->display_options['fields']['nothing']['alter']['text'] = '[view_node_registration] [edit_node_registration] [cancel_node_registration] [delete_node_registration]';
$handler->display->display_options['fields']['nothing']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['nothing']['hide_alter_empty'] = TRUE;
/* Sort criterion: Node registration: Created */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'node_registration';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Contextual filter: Node registration: Node nid */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'node_registration';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
/* Filter criterion: Node registration: Attended */
$handler->display->display_options['filters']['attended']['id'] = 'attended';
$handler->display->display_options['filters']['attended']['table'] = 'node_registration';
$handler->display->display_options['filters']['attended']['field'] = 'attended';
$handler->display->display_options['filters']['attended']['value'] = 'All';
$handler->display->display_options['filters']['attended']['exposed'] = TRUE;
$handler->display->display_options['filters']['attended']['expose']['operator_id'] = '';
$handler->display->display_options['filters']['attended']['expose']['label'] = 'Attended';
$handler->display->display_options['filters']['attended']['expose']['operator'] = 'attended_op';
$handler->display->display_options['filters']['attended']['expose']['identifier'] = 'attended';
/* Filter criterion: Node registration: Cancelled */
$handler->display->display_options['filters']['cancelled']['id'] = 'cancelled';
$handler->display->display_options['filters']['cancelled']['table'] = 'node_registration';
$handler->display->display_options['filters']['cancelled']['field'] = 'cancelled';
$handler->display->display_options['filters']['cancelled']['value'] = 'All';
$handler->display->display_options['filters']['cancelled']['exposed'] = TRUE;
$handler->display->display_options['filters']['cancelled']['expose']['operator_id'] = '';
$handler->display->display_options['filters']['cancelled']['expose']['label'] = 'Cancelled';
$handler->display->display_options['filters']['cancelled']['expose']['operator'] = 'cancelled_op';
$handler->display->display_options['filters']['cancelled']['expose']['identifier'] = 'cancelled';
/* Filter criterion: Node registration: Verified */
$handler->display->display_options['filters']['verified']['id'] = 'verified';
$handler->display->display_options['filters']['verified']['table'] = 'node_registration';
$handler->display->display_options['filters']['verified']['field'] = 'verified';
$handler->display->display_options['filters']['verified']['value'] = 'All';
$handler->display->display_options['filters']['verified']['exposed'] = TRUE;
$handler->display->display_options['filters']['verified']['expose']['operator_id'] = '';
$handler->display->display_options['filters']['verified']['expose']['label'] = 'Verified';
$handler->display->display_options['filters']['verified']['expose']['operator'] = 'verified_op';
$handler->display->display_options['filters']['verified']['expose']['identifier'] = 'verified';

/* Display: All registrations */
$handler = $view->new_display('page', 'All registrations', 'all');
$handler->display->display_options['defaults']['header'] = FALSE;
/* Header: Global: Text area */
$handler->display->display_options['header']['area']['id'] = 'area';
$handler->display->display_options['header']['area']['table'] = 'views';
$handler->display->display_options['header']['area']['field'] = 'area';
$handler->display->display_options['header']['area']['content'] = 'These are ALL registrations. Newest first. Node specific registrations can be found in the Node registrations settings of that node.';
$handler->display->display_options['header']['area']['format'] = 'plain_text';
$handler->display->display_options['path'] = 'admin/content/registrations';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Registrations';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'management';
$handler->display->display_options['tab_options']['weight'] = '0';

/* Display: One event's registrations */
$handler = $view->new_display('page', 'One event\'s registrations', 'event');
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Node registration: Weight */
$handler->display->display_options['sorts']['weight']['id'] = 'weight';
$handler->display->display_options['sorts']['weight']['table'] = 'node_registration';
$handler->display->display_options['sorts']['weight']['field'] = 'weight';
$handler->display->display_options['path'] = 'node/%/node_registration';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Registrations';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'management';
$handler->display->display_options['tab_options']['weight'] = '0';

  // Remove VBO fields if not available.
  if (!$vbo) {
    unset(
      $view->display['default']->display_options['style_options']['columns']['views_bulk_operations'],
      $view->display['default']->display_options['style_options']['info']['views_bulk_operations'],
      $view->display['default']->display_options['fields']['views_bulk_operations']
    );
  }

  $views[$view->name] = $view;







$view = new view();
$view->name = 'node_registration_users';
$view->description = 'This view is for the admin\'s user list when adding a registration. You can sort and filter it etc. The rendered Name field is used in the dropdown. When disabled, the admin is presented an autocompleting textfield instead of a dropdown.';
$view->tag = 'node_registration';
$view->base_table = 'users';
$view->human_name = 'Node registration users';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Node registration users';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'access user profiles';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'list';
$handler->display->display_options['style_options']['default_row_class'] = FALSE;
$handler->display->display_options['style_options']['row_class_special'] = FALSE;
$handler->display->display_options['style_options']['wrapper_class'] = '';
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['row_options']['default_field_elements'] = FALSE;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['label'] = '';
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
/* Sort criterion: User: Name */
$handler->display->display_options['sorts']['name']['id'] = 'name';
$handler->display->display_options['sorts']['name']['table'] = 'users';
$handler->display->display_options['sorts']['name']['field'] = 'name';
/* Filter criterion: User: Active */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'users';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = '1';
$handler->display->display_options['filters']['status']['group'] = 0;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

/* Display: Block */
$handler = $view->new_display('block', 'Block', 'block');

  $views[$view->name] = $view;

  return $views;
}

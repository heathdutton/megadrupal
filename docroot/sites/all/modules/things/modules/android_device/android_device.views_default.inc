<?php
/**
 * @file
 * android_device.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function android_device_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'gcm_identifier_by_user';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'things';
  $view->human_name = 'GCM identifier by user';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_gcm_id_target_id']['id'] = 'field_gcm_id_target_id';
  $handler->display->display_options['relationships']['field_gcm_id_target_id']['table'] = 'field_data_field_gcm_id';
  $handler->display->display_options['relationships']['field_gcm_id_target_id']['field'] = 'field_gcm_id_target_id';
  $handler->display->display_options['relationships']['field_gcm_id_target_id']['label'] = 'GCM id for thing';
  $handler->display->display_options['relationships']['field_gcm_id_target_id']['required'] = TRUE;
  /* Field: Things Identifiers: Token */
  $handler->display->display_options['fields']['token']['id'] = 'token';
  $handler->display->display_options['fields']['token']['table'] = 'things_ids';
  $handler->display->display_options['fields']['token']['field'] = 'token';
  $handler->display->display_options['fields']['token']['relationship'] = 'field_gcm_id_target_id';
  $handler->display->display_options['fields']['token']['label'] = '';
  $handler->display->display_options['fields']['token']['element_label_colon'] = FALSE;
  /* Contextual filter: Things: Owner (field_owner) */
  $handler->display->display_options['arguments']['field_owner_target_id']['id'] = 'field_owner_target_id';
  $handler->display->display_options['arguments']['field_owner_target_id']['table'] = 'field_data_field_owner';
  $handler->display->display_options['arguments']['field_owner_target_id']['field'] = 'field_owner_target_id';
  $handler->display->display_options['arguments']['field_owner_target_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_owner_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_owner_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_owner_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_owner_target_id']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Things: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'things';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'android_device' => 'android_device',
  );

  /* Display: Rules */
  $handler = $view->new_display('views_rules', 'Rules', 'views_rules_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['rules_parameter'] = array(
    'field_owner_target_id' => array(
      'enabled' => 1,
      'type' => 'integer',
      'label' => 'Things: Owner (field_owner)',
      'name' => 'field_owner_target_id',
    ),
  );
  $handler->display->display_options['rules_variables'] = array(
    'token' => array(
      'enabled' => 1,
      'type' => 'text',
      'label' => '(GCM id for thing) Things Identifiers: Token',
      'name' => 'token',
    ),
  );
  $export['gcm_identifier_by_user'] = $view;

  $view = new view();
  $view->name = 'users';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'Users';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = '';
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  /* Sort criterion: User: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'users';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Services */
  $handler = $view->new_display('services', 'Services', 'services_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'users';
  $export['users'] = $view;

  return $export;
}

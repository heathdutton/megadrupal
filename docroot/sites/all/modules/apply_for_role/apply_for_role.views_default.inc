<?php
/**
 * @file
 * Contains default views for the Apply for Role module.
 */

/**
 * Implements hook_views_default_views().
 */
function apply_for_role_views_default_views() {
  $view = new view();
  $view->name = 'apply_for_role_admin';
  $view->description = 'Role applications';
  $view->tag = 'Apply for Role';
  $view->base_table = 'users_roles_apply';
  $view->human_name = 'apply_for_role_admin';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = TRUE; /* Edit this to FALSE to make a default view enabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view role applications';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'uid' => 'uid',
    'user_roles' => 'user_roles',
    'rid' => 'rid',
    'apply_date' => 'apply_date',
    'approved' => 'approved',
    'approve_date' => 'approve_date',
    'nothing' => 'nothing',
    'approve_application' => 'nothing',
    'deny_application' => 'nothing',
    'delete_application' => 'nothing',
    'message' => 'message',
  );
  $handler->display->display_options['style_options']['default'] = 'apply_date';
  $handler->display->display_options['style_options']['info'] = array(
    'uid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'user_roles' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'rid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'apply_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'approved' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'approve_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '<br />',
      'empty_column' => 0,
    ),
    'approve_application' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'deny_application' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'delete_application' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'message' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Apply for Role: User */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users_roles_apply';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = 1;
  /* Field: Apply for Role: Current roles */
  $handler->display->display_options['fields']['user_roles']['id'] = 'user_roles';
  $handler->display->display_options['fields']['user_roles']['table'] = 'users_roles_apply';
  $handler->display->display_options['fields']['user_roles']['field'] = 'user_roles';
  $handler->display->display_options['fields']['user_roles']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['user_roles']['type'] = 'ul';
  /* Field: Apply for Role: Role */
  $handler->display->display_options['fields']['rid']['id'] = 'rid';
  $handler->display->display_options['fields']['rid']['table'] = 'users_roles_apply';
  $handler->display->display_options['fields']['rid']['field'] = 'rid';
  $handler->display->display_options['fields']['rid']['element_label_colon'] = FALSE;
  /* Field: Apply for Role: Date applied */
  $handler->display->display_options['fields']['apply_date']['id'] = 'apply_date';
  $handler->display->display_options['fields']['apply_date']['table'] = 'users_roles_apply';
  $handler->display->display_options['fields']['apply_date']['field'] = 'apply_date';
  $handler->display->display_options['fields']['apply_date']['element_label_colon'] = FALSE;
  /* Field: Apply for Role: Status */
  $handler->display->display_options['fields']['approved']['id'] = 'approved';
  $handler->display->display_options['fields']['approved']['table'] = 'users_roles_apply';
  $handler->display->display_options['fields']['approved']['field'] = 'approved';
  $handler->display->display_options['fields']['approved']['element_label_colon'] = FALSE;
  /* Field: Apply for Role: Date processed */
  $handler->display->display_options['fields']['approve_date']['id'] = 'approve_date';
  $handler->display->display_options['fields']['approve_date']['table'] = 'users_roles_apply';
  $handler->display->display_options['fields']['approve_date']['field'] = 'approve_date';
  $handler->display->display_options['fields']['approve_date']['element_label_colon'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Operations';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Field: Apply for Role: Approve link */
  $handler->display->display_options['fields']['approve_application']['id'] = 'approve_application';
  $handler->display->display_options['fields']['approve_application']['table'] = 'users_roles_apply';
  $handler->display->display_options['fields']['approve_application']['field'] = 'approve_application';
  $handler->display->display_options['fields']['approve_application']['label'] = '';
  $handler->display->display_options['fields']['approve_application']['element_label_colon'] = FALSE;
  /* Field: Apply for Role: Deny link */
  $handler->display->display_options['fields']['deny_application']['id'] = 'deny_application';
  $handler->display->display_options['fields']['deny_application']['table'] = 'users_roles_apply';
  $handler->display->display_options['fields']['deny_application']['field'] = 'deny_application';
  $handler->display->display_options['fields']['deny_application']['label'] = '';
  $handler->display->display_options['fields']['deny_application']['element_label_colon'] = FALSE;
  /* Field: Apply for Role: Delete link */
  $handler->display->display_options['fields']['delete_application']['id'] = 'delete_application';
  $handler->display->display_options['fields']['delete_application']['table'] = 'users_roles_apply';
  $handler->display->display_options['fields']['delete_application']['field'] = 'delete_application';
  $handler->display->display_options['fields']['delete_application']['label'] = '';
  $handler->display->display_options['fields']['delete_application']['element_label_colon'] = FALSE;
  /* Field: Apply for Role: Application message */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'users_roles_apply';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  $handler->display->display_options['fields']['message']['label'] = '';
  $handler->display->display_options['fields']['message']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['message']['empty'] = 'No application message.';

  /* Display: Role applications */
  $handler = $view->new_display('page', 'Role applications', 'apply_for_role_admin');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/people/apply_for_role';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Role applications';
  $handler->display->display_options['menu']['weight'] = '10';
  $views[$view->name] = $view;

  return $views;
}

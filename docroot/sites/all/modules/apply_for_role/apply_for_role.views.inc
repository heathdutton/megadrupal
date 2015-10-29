<?php
/**
 * @file
 * Provide views data and handlers for apply_for_role.module.
 */

/**
 * @defgroup apply_for_role views handlers
 *
 * @{
 */

/**
 * Implements hook_views_data()
 */
function apply_for_role_views_data() {
  $data = array();

  // ----------------------------------------------------------------------
  // users_roles table

  $data['users_roles_apply']['table']['group']  = t('Apply for Role');

  $data['users_roles_apply']['table']['base'] = array(
    'field' => 'aid',
    'title' => t('Role application'),
    'help' => t('Applications for roles by users who have created accounts on your site.'),
    'access query tag' => 'user_access',
  );

  $data['users_roles_apply']['aid'] = array(
    'title' => t('aid'),
    'help' => t('The role application ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
  );

  // uid
  $data['users_roles_apply']['uid'] = array(
    'title' => t('User'),
    'help' => t('The name of the user.'),
    'field' => array(
      'handler' => 'apply_for_role_views_handler_field_user_name',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
      'name field' => 'name',
    ),
    'filter' => array(
      'title' => t('Name'),
      'handler' => 'views_handler_filter_user_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'title' => t('Users'),
      'help' => t('Relate users to applications.'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'uid',
      'label' => t('users'),
    ),
  );

  // uid
  $data['users_roles_apply']['uid_current'] = array(
    'real field' => 'uid',
    'title' => t('Current user'),
    'help' => t('Filter the view to the currently logged in user.'),
    'filter' => array(
      'handler' => 'views_handler_filter_user_current',
      'type' => 'yes-no',
    ),
  );

  // rid
  $data['users_roles_apply']['user_roles'] = array(
    'real field' => 'rid',
    'title' => t('Current roles'),
    'help' => t('Roles that a user belongs to.'),
    'field' => array(
      'handler' => 'apply_for_role_views_handler_field_user_roles',
      'no group by' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_roles',
      'numeric' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_users_roles_rid',
      'name table' => 'role',
      'name field' => 'name',
      'empty field name' => t('No role'),
      'zero is null' => TRUE,
      'numeric' => TRUE,
    ),
  );

  // rid
  $data['users_roles_apply']['rid'] = array(
    'title' => t('Role'),
    'help' => t('The name of the role.'),
    'field' => array(
      'handler' => 'apply_for_role_views_handler_field_role_name',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // apply_date field
  $data['users_roles_apply']['apply_date'] = array(
    'title' => t('Date applied'),
    'help' => t('The date the application was made.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // approved field
  $data['users_roles_apply']['approved'] = array(
    'title' => t('Status'),
    'help' => t('Status of role applications.'),
    'field' => array(
      'handler' => 'apply_for_role_views_handler_field_status_name',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // approve_date field
  $data['users_roles_apply']['approve_date'] = array(
    'title' => t('Date processed'),
    'help' => t('The date the application was processed.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // message field
  $data['users_roles_apply']['message'] = array(
    'title' => t('Application message'),
    'help' => t('The message submitted by an applicant.'),
    'field' => array(
      'handler' => 'apply_for_role_views_handler_field_message',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['users_roles_apply']['approve_application'] = array(
    'field' => array(
      'title' => t('Approve link'),
      'help' => t('Provide a simple link to approve the application.'),
      'handler' => 'apply_for_role_views_handler_field_application_link_approve',
    ),
  );

  $data['users_roles_apply']['deny_application'] = array(
    'field' => array(
      'title' => t('Deny link'),
      'help' => t('Provide a simple link to deny the application.'),
      'handler' => 'apply_for_role_views_handler_field_application_link_deny',
    ),
  );

  $data['users_roles_apply']['delete_application'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the application.'),
      'handler' => 'apply_for_role_views_handler_field_application_link_delete',
    ),
  );

  return $data;
}

/**
 * @}
 */

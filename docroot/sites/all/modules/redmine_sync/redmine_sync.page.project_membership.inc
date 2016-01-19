<?php

require_once 'redmine_sync.constants.inc';
require_once 'redmine_sync.api.custom_fields.form.inc';

/**
 * redmine_sync_project_membership_add_new_form().
 */
function redmine_sync_project_membership_add_new_form($form, &$form_state) {

  $resp_api_init = redmine_sync_init_rest();
  $link_to_rest_api_settings_page = l(t('REST Api settings page'), REDMINE_SYNC_URL_PAGE_SETTINGS_REST_API);
  $link_to_personal_profile = l(t('!name profile page', array('!name' => $GLOBALS['user']->name)), 'user/'.$GLOBALS['user']->uid.'/edit');
  $auth_mode = variable_get('redmine_sync_rest_api_auth_mode', REDMINE_SYNC_REST_API_AUTH_MODE_ADMIN_KEY);

  // Error message.
  if ($resp_api_init == REDMINE_SYNC_REST_API_INIT_EMPTY_PARAMETERS) {
    $form['error_message_1'] = array(
      '#markup' => theme('form_instead_message', array('status' => 'warning', 'message' =>
        t('Redmine REST API parameters are not set!').REDMINE_SYNC_HTML_BR.
        t('Please configure on !settings_page.', array('!settings_page' => $link_to_rest_api_settings_page))
      )),
    );
  }
  if ($resp_api_init == REDMINE_SYNC_REST_API_INIT_INVALID_PARAMETERS_OR_SERVER_NOT_AVAILABLE) {
    $form['error_message_2'] = array(
      '#markup' => theme('form_instead_message', array('status' => 'error', 'message' =>
        t('Redmine REST API parameters are invalid or server is not available.').REDMINE_SYNC_HTML_BR.
        t('Please check configuration on !settings_page or check if database server is accessible.', array('!settings_page' => $link_to_rest_api_settings_page))
      )),
    );
  }
  if ($resp_api_init == REDMINE_SYNC_REST_API_INIT_NO_USER_KEY) {
    $form['error_message_3'] = array(
      '#markup' => theme('form_instead_message', array('status' => 'error', 'message' =>
        t('Redmine REST API user key is not set.').REDMINE_SYNC_HTML_BR.
        t('Please add Remine REST API user key on !profile_page.', array('!profile_page' => $link_to_personal_profile))
      )),
    );
  }

  // If no errors.
  if ($resp_api_init == REDMINE_SYNC_REST_API_INIT_OK) {

    switch ($auth_mode) {
      case REDMINE_SYNC_REST_API_AUTH_MODE_ADMIN_KEY :
        $my_rdm_info = redmine_sync_rest_get_user_by_mail($GLOBALS['user']->mail);
        break;
      case REDMINE_SYNC_REST_API_AUTH_MODE_USER_KEYS :
        $my_rdm_info = redmine_sync_rest_get_my_info();
        break;
    }

    // Check user by email in Redmine.
    if ($my_rdm_info && $my_rdm_info['mail'] == $GLOBALS['user']->mail) {
      $form['#rdm_user_login'] = $my_rdm_info['login'];

      // Init lists.
      $projects_list = redmine_sync_rest_get_projects(null, REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $my_rdm_info['login']);
      $users_list    = redmine_sync_rest_get_users(null, REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $my_rdm_info['login']);
      $roles_list    = redmine_sync_rest_get_user_roles(null, REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $my_rdm_info['login']);
      // Show form.
      $form['project_id'] = array(
        '#type' => 'select',
        '#title' => t('Project'),
        '#options' => array('' => t('- Select -')) + (count($projects_list) ? $projects_list : array()),
        '#required' => true,
      );
      $form['user_id'] = array(
        '#type' => 'select',
        '#title' => t('User'),
        '#options' => array('' => t('- Select -')) + (count($users_list) ? $users_list : array()),
        '#required' => true,
      );
      $form['role_ids'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Roles'),
        '#options' => count($roles_list) ? $roles_list : array(),
        '#required' => true,
      );

      // Actions.
      $form['actions'] = array(
        '#type' => 'actions',
      );
      $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Add new'),
      );

    } else {
      drupal_set_message('We can\'t find your email address in Redmine database.', 'warning');
      drupal_access_denied();
      exit();
    }

  }
  return $form;
}


function redmine_sync_project_membership_add_new_form_submit($form, &$form_state) {
  // Create Project Membership post array.
  $membership_post = array(
    'membership' => array(
      'user_id' => $form_state['values']['user_id'],
    )
  );

  // Added role_ids.
  $c_buf = array('key' => 'role_ids', 'attributes' => array('type' => 'array'), 'values' => array());
  foreach (array_filter($form_state['values']['role_ids']) as $c_role_id) {
    $c_buf['value'][]= array('key' => 'role_id', 'value' => $c_role_id); 
  }
  if (count($c_buf)) {
    $membership_post['membership'][0] = $c_buf;
  }

  $request = redmine_sync_rest_request('projects/'.$form_state['values']['project_id'].'/memberships', 'POST', $membership_post, $form['#rdm_user_login']);
  if ($request->code == 201) {
    drupal_set_message(t('New entry %name with id = %id was created.',       array('%name' => 'Project Membership', '%id' => (int)$request->data_prepared->id)));
    watchdog('redmine_sync', 'New entry %name with id = %id was created.',   array('%name' => 'Project Membership', '%id' => (int)$request->data_prepared->id));
  } else {
    drupal_set_message(t('Can\'t create new entry %name! Error: %error',     array('%name' => 'Project Membership', '%error' => $request->error)), 'error');
    watchdog('redmine_sync', 'Can\'t create new entry %name! Error: %error', array('%name' => 'Project Membership', '%error' => $request->error), WATCHDOG_ERROR);
  }
}

<?php

require_once 'redmine_sync.constants.inc';
require_once 'redmine_sync.api.custom_fields.form.inc';

/**
 * redmine_sync_project_add_new_form().
 */
function redmine_sync_project_add_new_form($form, &$form_state) {

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

      // Show form.
      $form['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#maxlength' => 255,
        '#required' => TRUE,
        '#size' => 60,
      );
      $form['description'] = array(
        '#type' => 'textarea',
        '#title' => t('Description'),
      );
      $form['identifier'] = array(
        '#type' => 'textfield',
        '#title' => t('Identifier'),
        '#description' => t('Length between 1 and 100 characters. Only lower case letters (a-z), numbers, dashes and underscores are allowed, must start with a lower case letter. Once saved, the identifier cannot be changed.'),
        '#maxlength' => 100,
        '#required' => TRUE,
        '#size' => 60,
      );
      $form['homepage'] = array(
        '#type' => 'textfield',
        '#title' => t('Homepage'),
        '#maxlength' => 255,
        '#size' => 60,
      );
      $form['is_public'] = array(
        '#type' => 'checkbox', 
        '#title' => t('Public'),
      );
      $form['parent_id'] = array(
        '#type' => 'select',
        '#title' => t('Subproject of'),
        '#options' => array('' => t('- Select -')) + redmine_sync_rest_get_projects(null, REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $my_rdm_info['login']),
      );
      $form['inherit_members'] = array(
        '#type' => 'checkbox', 
        '#title' => t('Inherit members'),
      );
      $form['enabled_module_names'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Modules'),
        '#description' => t('If you not select any checkbox from this set then all checkboxes will be selected!'),
        '#options' => $GLOBALS['REDMINE_SYNC_MODULE_NAMES_LIST'],
      );
      $form['tracker_ids'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Trackers'),
        '#description' => t('If you not select any checkbox from this set then all checkboxes will be selected!'),
        '#options' => redmine_sync_rest_get_trackers(null, REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $my_rdm_info['login']),
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


function redmine_sync_project_add_new_form_validate($form, &$form_state) {
  if (!preg_match('/^[a-z][a-z0-9_\-]*$/', $form_state['values']['identifier'])) {
    form_set_error('identifier', t('Field %field contains incorrect value.', array('%field' => t('Identifier'))));
  }
}


function redmine_sync_project_add_new_form_submit($form, &$form_state) {
  // Create Project post array.
  $project_post = array(
    'project' => array(
      'name'            => $form_state['values']['name'],
      'description'     => $form_state['values']['description'],
      'identifier'      => $form_state['values']['identifier'],
      'homepage'        => $form_state['values']['homepage'],
      'is_public'       => $form_state['values']['is_public'],
      'parent_id'       => $form_state['values']['parent_id'],
      'inherit_members' => $form_state['values']['inherit_members'],
    )
  );
  // Added enabled_module_names.
  foreach (array_filter($form_state['values']['enabled_module_names']) as $c_value) {
    $project_post['project'][]= array('key' => 'enabled_module_names', 'value' => $c_value);
  }
  // Added tracker_ids.
  foreach (array_filter($form_state['values']['tracker_ids']) as $c_value) {
    $project_post['project'][]= array('key' => 'tracker_ids', 'value' => $c_value);
  }

  $request = redmine_sync_rest_request('projects', 'POST', $project_post, $form['#rdm_user_login']);
  if ($request->code == 201) {
    drupal_set_message(t('New entry %name with id = %id was created.',       array('%name' => 'Project', '%id' => (int)$request->data_prepared->id)));
    watchdog('redmine_sync', 'New entry %name with id = %id was created.',   array('%name' => 'Project', '%id' => (int)$request->data_prepared->id));
  } else {
    drupal_set_message(t('Can\'t create new entry %name! Error: %error',     array('%name' => 'Project', '%error' => $request->error)), 'error');
    watchdog('redmine_sync', 'Can\'t create new entry %name! Error: %error', array('%name' => 'Project', '%error' => $request->error), WATCHDOG_ERROR);
  }
}

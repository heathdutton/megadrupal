<?php

require_once 'redmine_sync.constants.inc';
require_once 'redmine_sync.api.custom_fields.form.inc';

/**
 * redmine_sync_issue_add_new_form().
 */
function redmine_sync_issue_add_new_form($form, &$form_state) {

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

      // Prepare data related by project ID.
      $assignee_list = array();
      $trackers_list = array();
      $issue_categories_list = array();
      $versions_list = array();
      if (isset($form_state['values']['project_id'])) {
        // Init $assignee_list.
        $project_info = redmine_sync_rest_get_project($form_state['values']['project_id'], 'include=trackers,issue_categories');
        $memberships = redmine_sync_rest_get_project_memberships($form_state['values']['project_id'], null, REDMINE_SYNC_RET_VAL_TYPE_DEFAULT);
        foreach ($memberships as $c_membership) {
          $assignee_list[(int)$c_membership['user']['id']] = (string)$c_membership['user']['name'];
        }
        // Init $trackers_list.
        foreach ($project_info['trackers'] as $c_tracker) {
          $trackers_list[(int)$c_tracker['id']] = (string)$c_tracker['name'];
        }
        // Init $issue_categories_list.
        foreach ($project_info['issue_categories'] as $c_category) {
          $issue_categories_list[(int)$c_category['id']] = (string)$c_category['name'];
        }
        // Init $versions_list.
        $versions_list = redmine_sync_rest_get_project_versions($form_state['values']['project_id'], null, REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $my_rdm_info['login']);
      }
      // Init $statuses_list.
      $statuses_list = redmine_sync_rest_get_issue_statuses(null, REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $my_rdm_info['login']);
      foreach ($statuses_list as $c_id => $c_name) {
        if ($c_name != 'New') {
          unset($statuses_list[$c_id]);
        }
      }

      // Show form.
      $form['#attached']['library'][] = array('system', 'ui.datepicker');
      $form['#attached']['js'][] = REDMINE_SYNC_MODULE_PATH.'/redmine_sync.js';
      $form['#attached']['js'][] = drupal_get_path('module', 'locale').'/locale.datepicker.js';
      $form['#attached']['css'][] = REDMINE_SYNC_MODULE_PATH.'/redmine_sync.css';
      $form['project_id'] = array(
        '#type' => 'select',
        '#title' => t('Project'),
        '#options' => redmine_sync_rest_get_projects(null, REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $my_rdm_info['login']),
        '#ajax' => array('event' => 'change', 'callback' => '_js_callback_redmine_sync_issue_add_new_on_project_id_on_change'),
        '#required' => true,
      );
      $form['tracker_id'] = array(
        '#type' => 'select',
        '#title' => t('Tracker'),
        '#options' => array('' => t('- Select -')) + (count($trackers_list) ? $trackers_list : array()),
        '#attributes' => array('id' => 'tracker_id'),
        '#required' => true,
      );
      $form['subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#maxlength' => 255,
        '#required' => true,
      );
      $form['description'] = array(
        '#type' => 'textarea',
        '#title' => t('Description'),
      );
      $form['status_id'] = array(
        '#type' => 'select',
        '#title' => t('Status'),
        '#options' => array('' => t('- Select -')) + (count($statuses_list) ? $statuses_list : array()),
        '#required' => true,
      );
      $form['priority_id'] = array(
        '#type' => 'select',
        '#title' => t('Priority'),
        '#options' => redmine_sync_rest_get_issue_priorities(null, REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $my_rdm_info['login']),
        '#required' => true,
      );
      $form['assigned_to_id'] = array(
        '#type' => 'select',
        '#title' => t('Assignee'),
        '#options' => array('' => t('- Select -')) + (count($assignee_list) ? $assignee_list : array()),
        '#attributes' => array('id' => 'assigned_to_id'),
      );
      $form['category_id'] = array(
        '#type' => 'select',
        '#title' => t('Category'),
        '#options' => array('' => t('- Select -')) + (count($issue_categories_list) ? $issue_categories_list : array()),
        '#attributes' => array('id' => 'category_id'),
      );
      $form['fixed_version_id'] = array(
        '#type' => 'select',
        '#title' => t('Target version'),
        '#options' => array('' => t('- Select -')) + (count($versions_list) ? $versions_list : array()),
        '#attributes' => array('id' => 'fixed_version_id'),
      );
      $form['start_date'] = array(
        '#type' => 'textfield',
        '#title' => t('Start date'),
        '#default_value' => gmdate(REDMINE_SYNC_DEF_DATE_FORMAT),
        '#attributes' => array('class' => array('datepicker')),
        '#element_validate' => array('_redmine_sync_form_date_field_validate'),
        '#maxlength' => 10,
        '#size' => 11,
      );
      $form['due_date'] = array(
        '#type' => 'textfield',
        '#title' => t('Due date'),
        '#attributes' => array('class' => array('datepicker')),
        '#element_validate' => array('_redmine_sync_form_date_field_validate'),
        '#maxlength' => 10,
        '#size' => 11,
      );
      $form['estimated_hours'] = array(
        '#type' => 'textfield',
        '#title' => t('Estimated time'),
        '#element_validate' => array('_redmine_sync_form_time_field_validate'),
        '#description' => t('Values should be from %from to %to.', array('%from' => '0.00', '%to' => '999.99')),
        '#maxlength' => 6,
        '#size' => 2,
      );
      $form['done_ratio'] = array(
        '#type' => 'select',
        '#title' => t('% Done'),
        '#options' => drupal_map_assoc(range(0, 100, 10)),
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


function redmine_sync_issue_add_new_form_submit($form, &$form_state) {
  // Create Issue post array.
  $issue_post = array(
    'issue' => array(
      'project_id'       => $form_state['values']['project_id'],
      'tracker_id'       => $form_state['values']['tracker_id'],
      'subject'          => $form_state['values']['subject'],
      'description'      => $form_state['values']['description'],
      'status_id'        => $form_state['values']['status_id'],
      'priority_id'      => $form_state['values']['priority_id'],
      'assigned_to_id'   => $form_state['values']['assigned_to_id'],
      'category_id'      => $form_state['values']['category_id'],
      'fixed_version_id' => $form_state['values']['fixed_version_id'],
      'start_date'       => $form_state['values']['start_date'],
      'due_date'         => $form_state['values']['due_date'],
      'estimated_hours'  => $form_state['values']['estimated_hours'],
      'done_ratio'       => $form_state['values']['done_ratio'],
    )
  );
  $request = redmine_sync_rest_request('issues', 'POST', $issue_post, $form['#rdm_user_login']);
  if ($request->code == 201) {
    drupal_set_message(t('New entry %name with id = %id was created.',       array('%name' => 'Issue', '%id' => (int)$request->data_prepared->id)));
    watchdog('redmine_sync', 'New entry %name with id = %id was created.',   array('%name' => 'Issue', '%id' => (int)$request->data_prepared->id));
  } else {
    drupal_set_message(t('Can\'t create new entry %name! Error: %error',     array('%name' => 'Issue', '%error' => $request->error)), 'error');
    watchdog('redmine_sync', 'Can\'t create new entry %name! Error: %error', array('%name' => 'Issue', '%error' => $request->error), WATCHDOG_ERROR);
  }
}


function _js_callback_redmine_sync_issue_add_new_on_project_id_on_change($form, &$form_state) {
  $commands = array();
  $commands[] = ajax_command_replace('#tracker_id',       theme('select', array('element' => $form['tracker_id'])));
  $commands[] = ajax_command_replace('#assigned_to_id',   theme('select', array('element' => $form['assigned_to_id'])));
  $commands[] = ajax_command_replace('#category_id',      theme('select', array('element' => $form['category_id'])));
  $commands[] = ajax_command_replace('#fixed_version_id', theme('select', array('element' => $form['fixed_version_id'])));
  return array('#type' => 'ajax', '#commands' => $commands);
}

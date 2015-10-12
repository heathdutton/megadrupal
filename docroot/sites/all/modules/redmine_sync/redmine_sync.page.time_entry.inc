<?php

require_once 'redmine_sync.constants.inc';
require_once 'redmine_sync.api.custom_fields.form.inc';

/**
 * redmine_sync_time_entry_add_new_form().
 */
function redmine_sync_time_entry_add_new_form($form, &$form_state) {

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
      $form['#attached']['library'][] = array('system', 'ui.datepicker');
      $form['#attached']['js'][] = REDMINE_SYNC_MODULE_PATH.'/redmine_sync.js';
      $form['#attached']['js'][] = drupal_get_path('module', 'locale').'/locale.datepicker.js';
      $form['#attached']['css'][] = REDMINE_SYNC_MODULE_PATH.'/redmine_sync.css';
      $form['project_id'] = array(
        '#type' => 'select',
        '#title' => t('Project'),
        '#options' => redmine_sync_rest_get_projects(null, REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST, $my_rdm_info['login']),
        '#ajax' => array('event' => 'change', 'callback' => '_js_callback_redmine_sync_time_entry_add_new_on_project_id_on_change'),
        '#required' => true,
      );
      if (isset($form_state['values']['project_id'])) {
        $issues = redmine_sync_rest_get_issues('project_id='.(int)$form_state['values']['project_id'], REDMINE_SYNC_RET_VAL_TYPE_OPTION_LIST_GROUPED, $my_rdm_info['login']);
      }
      $form['issue_id'] = array(
        '#type' => 'select',
        '#title' => t('Issue'),
        '#options' => array('' => t('- Select -')) + (isset($issues) && count($issues) ? $issues : array()),
        '#attributes' => array('id' => 'issue_id'),
        '#required' => true,
      );
      $form['spent_on'] = array(
        '#type' => 'textfield',
        '#title' => t('Date'),
        '#attributes' => array('class' => array('datepicker')),
        '#element_validate' => array('_redmine_sync_form_date_field_validate'),
        '#required' => true,
        '#maxlength' => 10,
        '#size' => 11,
      );
      $form['hours'] = array(
        '#type' => 'textfield',
        '#title' => t('Hours'),
        '#element_validate' => array('_redmine_sync_form_time_field_validate'),
        '#description' => t('Values should be from %from to %to.', array('%from' => '0.00', '%to' => '999.99')),
        '#required' => true,
        '#maxlength' => 6,
        '#size' => 2,
      );
      $form['comments'] = array(
        '#type' => 'textfield',
        '#title' => t('Comment'),
        '#maxlength' => 255,
        '#size' => 100,
        '#description' => t('Maximum %num characters.', array('%num' => 255)),
      );
      $form['activity_id'] = array(
        '#type' => 'select',
        '#title' => t('Activity'),
        '#options' => redmine_sync_rest_get_time_entry_activities(),
        '#required' => true,
      );

      // Add custom fields to form.
      _redmine_sync_attach_custom_fields($form, $form_state, 'time_entry');
      if (!isset($form['custom_fields']) && $auth_mode == REDMINE_SYNC_REST_API_AUTH_MODE_USER_KEYS) {
        $form['custom_fields']['message'] = array(
          '#markup' => t('Custom fields is not support when Authentication mode = "!mode"', array('!mode' => t('Use personal user keys'))),
        );
      }

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


function redmine_sync_time_entry_add_new_form_submit($form, &$form_state) {
  // Create Time entry post array.
  $time_entry_post = array(
    'time_entry' => array(
      'issue_id'    => $form_state['values']['issue_id'],
      'spent_on'    => $form_state['values']['spent_on'],
      'hours'       => $form_state['values']['hours'],
      'comments'    => $form_state['values']['comments'],
      'activity_id' => $form_state['values']['activity_id'],
    )
  );

  // Redmine's custom fields need special treatment.
  if (isset($form['custom_fields'])) {
    $custom_fields_data = array();
    foreach ($form['custom_fields'] as $custom_field) {
      if (isset($custom_field['#rdm_id'])) {
        $c_post_val = $form_state['values']['rdm_custom_' . $custom_field['#rdm_id']];
        $c_buf = array();
        $c_buf['key'] = 'custom_field';
        $c_buf['attributes']['id'] = $custom_field['#rdm_id'];
        if (is_array($c_post_val)) {
          $c_buf['attributes']['multiple'] = 'true';
          $c_buf['value'][0] = array('key' => 'value', 'attributes' => array('type' => 'array'));
          foreach ($c_post_val as $c_val) {
            $c_buf['value'][0]['value'][] = array('key' => 'value', 'value' => $c_val);
          }
        } else {
          $c_buf['value'] = array('value' => $c_post_val);
        }
        $custom_fields_data[]= $c_buf;
      }
    }
    if (count($custom_fields_data)) {
      $time_entry_post['time_entry'][] = array(
        'key'        => 'custom_fields',
        'attributes' => array('type' => 'array'),
        'value'      => $custom_fields_data,
      );
    }
  }

  $request = redmine_sync_rest_request('time_entries', 'POST', $time_entry_post, $form['#rdm_user_login']);
  if ($request->code == 201) {
    drupal_set_message(t('New entry %name with id = %id was created.',       array('%name' => 'Time entry', '%id' => (int)$request->data_prepared->id)));
    watchdog('redmine_sync', 'New entry %name with id = %id was created.',   array('%name' => 'Time entry', '%id' => (int)$request->data_prepared->id));
  } else {
    drupal_set_message(t('Can\'t create new entry %name! Error: %error',     array('%name' => 'Time entry', '%error' => $request->error)), 'error');
    watchdog('redmine_sync', 'Can\'t create new entry %name! Error: %error', array('%name' => 'Time entry', '%error' => $request->error), WATCHDOG_ERROR);
  }
}


function _js_callback_redmine_sync_time_entry_add_new_on_project_id_on_change($form, &$form_state) {
  $commands = array();
  $commands[] = ajax_command_replace('#issue_id', theme('select', array('element' => $form['issue_id'])));
  foreach ($form['custom_fields'] as $fid => $c_custom_field) {
    if ($c_custom_field['#rdm_el_type'] == 'user' || $c_custom_field['#rdm_el_type'] == 'version') {
      $commands[] = ajax_command_replace('#edit-'.drupal_html_id($fid), theme('select', array('element' => $c_custom_field)));
    }
  }
  return array('#type' => 'ajax', '#commands' => $commands);
}

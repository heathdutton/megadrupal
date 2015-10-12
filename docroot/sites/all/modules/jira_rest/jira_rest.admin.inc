<?php

/**
 * Sets the instance url of jira.
 */
function jira_rest_admin() {

  $form = array();

  $form['jira_rest_jirainstanceurl'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of the JIRA instance'),
    '#default_value' => variable_get('jira_rest_jirainstanceurl', 'https://localhost:8443'),
    '#size' => 80,
    '#description' => t("Enter the URL of your JIRA instance (e.g. https://yourjira.com:8443)"),
    '#required' => TRUE,
  );

  $form['jira_rest_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username of the default user to connect to JIRA'),
    '#default_value' => variable_get('jira_rest_username', '<your username>'),
    '#size' => 80,
    '#description' => t("Enter the username used as default to connect to you JIRA instance (e.g. admin)"),
    '#required' => FALSE,
  );

  $form['jira_rest_password'] = array(
    '#type' => 'password',
    '#title' => t('Password of the default user to connect to JIRA'),
    '#default_value' => variable_get('jira_rest_password', ''),
    '#size' => 80,
    '#description' => t("Enter the password of the default user to connect to JIRA"),
    '#required' => FALSE,
  );

  $form['jira_rest_close_issue_transition_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Default transition ID to close an issue'),
    '#default_value' => variable_get('jira_rest_close_issue_transition_id', '2'),
    '#size' => 4,
    '#description' => t("Enter the default transition ID to close an issue with jira_rest_closeissuefixed()"),
    '#required' => TRUE,
  );

  $form['jira_rest_resolve_issue_transition_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Default transition ID to resolve an issue'),
    '#default_value' => variable_get('jira_rest_resolve_issue_transition_id', '5'),
    '#size' => 4,
    '#description' => t("Enter the default transition ID to resolve an issue with jira_rest_resolveissuefixed()"),
    '#required' => TRUE,
  );

  $form['jira_rest_cache_lifetime'] = array(
    '#type' => 'textfield',
    '#title' => t('JIRA Rest cache lifetime'),
    '#default_value' => variable_get('jira_rest_cache_lifetime', '0'),
    '#size' => 4,
    '#description' => t("Lifetime of the JIRA Rest cache in hours. If enabled, all requests (except downloads) will be cached. Set to -1 for infinity lieftime, 0 to disable."),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Validates correct format of url.
 */
function jira_rest_admin_validate($form, &$form_state) {
  $jira_url = $form_state['values']['jira_rest_jirainstanceurl'];
  if ((strpos(strrev($jira_url), strrev('/')) === 0)) {
    form_set_error('jira_rest_jirainstanceurl', t('URL must not end with "/"'));
  }

  if (!is_numeric($form_state['values']['jira_rest_close_issue_transition_id'])) {
    form_set_error('jira_rest_close_issue_transition_id', t('Transition id must be a numeric value'));
  }

  if (!is_numeric($form_state['values']['jira_rest_resolve_issue_transition_id'])) {
    form_set_error('jira_rest_resolve_issue_transition_id', t('Transition id must be a numeric value'));
  }

  //unsets userdata if username left empty
  if (empty($form_state['values']['jira_rest_username'])) {
    unset($form_state['values']['jira_rest_username']);
    variable_del('jira_rest_username');
    unset($form_state['values']['jira_rest_password']);
    variable_del('jira_rest_password');
  }
}
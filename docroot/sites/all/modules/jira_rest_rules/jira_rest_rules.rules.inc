<?php

/**
 * @file
 * Jira REST API Rules integration.
 */

/**
 * Implements hook_rules_action_info().
 *
 * A very basic Create Jira issue action.
 */
function jira_rest_rules_rules_action_info() {
  $items['jira_rest_rules_create_issue'] = array(
    'label' => t('Create Jira Issue'),
    'group' => t('Jira REST Rules'),
    'named parameter' => TRUE,
    'parameter' => array(
      'project' => array(
        'type' => 'text',
        'label' => t('Project key'),
        'restriction' => 'input',
      ),
      'issuetype' => array(
        'type' => 'text',
        'label' => t('Issuetype name'),
      ),
      'summary' => array(
        'type' => 'text',
        'label' => t('Summary'),
      ),
      'description' => array(
        'type' => 'text',
        'label' => t('Description'),
      ),
    ),
    'provides' => array(
      'jira_instanceurl' => array(
        'type' => 'text',
        'label' => t('Jira instance URL'),
      ),
      'jira_issue_id' => array(
        'type' => 'integer',
        'label' => t('Jira issue ID'),
      ),
      'jira_issue_key' => array(
        'type' => 'text',
        'label' => t('Jira issue key'),
      ),
      'jira_issue_self' => array(
        'type' => 'text',
        'label' => t('Jira issue REST API endpoint'),
      ),
    ),
  );

  return $items;
}

/**
 * Implements hook_rules_condition_info().
 */
function jira_rest_rules_rules_condition_info() {
  $conditions['jira_rest_rules_credentials_exists'] = array(
    'label' => t('Jira REST credentials exists'),
    'group' => t('Jira REST Rules'),
  );

  return $conditions;
}

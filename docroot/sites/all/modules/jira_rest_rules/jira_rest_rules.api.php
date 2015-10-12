<?php

/**
 * @file
 * API documentation for the Jira REST Rules module.
 */

/**
 * Manipulate issue data before submission to Jira.
 *
 * Building a Jira issue data structure in rules action forms can be complex if
 * it must support all of Jiras data types and custom field support.
 *
 * The module provides this alter hook giving you the possibility to build
 * complex issue data in code.
 *
 * The example below Adds project status, project name, and site-name as labels.
 *
 * @param array $issuedata
 *   Issue data as already prefilled by the rules action.
 * @param array $settings
 *   Settings as provided to the rules action.
 * @param string $operation
 *   The Jira issue action. Currently 'create' is the only implemented
 *   operation.
 */
function hook_jira_rest_rules_issuedata_alter(array &$issuedata, array &$settings, &$operation) {
  if ($operation == 'create') {
    $labels = array();

    if (!empty($settings['state']->variables['status'])) {
      $status_labels = update_rules_status_labels();
      $labels[] = $status_labels[$settings['state']->variables['status']];
    }

    if (!empty($settings['state']->variables['project_name'])) {
      $labels[] = $settings['state']->variables['project_name'];
    }

    if ($site_name = variable_get('site_name', FALSE)) {
      $labels[] = $site_name;
    }

    $issuedata['fields']['labels'] = $labels;
  }
}

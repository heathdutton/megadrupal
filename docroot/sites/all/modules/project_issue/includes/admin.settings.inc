<?php

/**
 * @file
 * Code for the admin settings form at project/project-issue-settings.
 */

/**
 * Form builder for the main settings form.
 */
function project_issue_settings_form($form, &$form_state) {
  // Determine the auto followup username from the auto followup setting.
  $followup_username = $anon = variable_get('anonymous', t('Anonymous'));
  $uid = variable_get('project_issue_followup_user', 0);
  if ($uid && $account = user_load($uid)) {
    $followup_username = $account->name;
  }

  $form['project_issue_followup_user'] = array(
    '#title' => t('Auto-followup user'),
    '#type' => 'textfield',
    '#default_value' => $followup_username,
    '#maxlength' => 60,
    '#description' => t('Enter the name of the user which will create automatic followups to issues -- leave empty to disable auto-followup or set to %anon to use the anonymous user.', array('%anon' => $anon)),
    '#element_validate' => array('project_issue_validate_followup_user'),
    '#autocomplete_path' => 'user/autocomplete',
  );

  $form['project_issue_auto_close_days'] = array(
    '#title' => t('Auto-close days'),
    '#type' => 'textfield',
    '#default_value' => (int) variable_get('project_issue_auto_close_days', PROJECT_ISSUE_AUTO_CLOSE_DAYS),
    '#size' => 4,
    '#maxlength' => 10,
    '#description' => t('Issues being in "fixed" state for the specified number of days will be closed by the followup user specified above. For example, if this is 14, and an issue is set to fixed on January 1, then it will be closed on January 15.'),
  );

  $field = field_info_field('field_issue_status');
  $form['project_issue_open_states'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Issue Statuses that are considered "Open"'),
    '#options' => list_allowed_values($field),
    '#default_value' => project_issue_open_states(),
    '#description' => t('Issue statuses specified here will be considered "Open", for example, when listing only Open Issues.'),
  );

  $form['project_issue_site_help'] = array(
    '#type' => 'textarea',
    '#title' => t('Site-wide help text for new issues'),
    '#default_value' => variable_get('project_issue_site_help', ''),
  );

  return system_settings_form($form);
}

function _project_issue_trim(&$item, $key) {
  $item = trim($item);
}

/**
 * Validates that the followup user exists, and has sufficient permissions
 * to follow up on issues.
 */
function project_issue_validate_followup_user($form, &$form_state) {
  $name = $form['#value'];
  if ($name !== '') {
    $anon = variable_get('anonymous', t('Anonymous'));
    // Make this check case-insensitive to allow the admin some data entry leeway.
    $is_anon = drupal_strtolower($name) == drupal_strtolower($anon);
    // Load the user. (don't see a constant for uid 0... )
    $account = $is_anon ? user_load(0) : user_load_by_name($name);
    if ($account) {
      foreach (project_issue_issue_node_types() as $type) {
        if (!user_access('edit any ' . $type . ' content', $account)) {
          form_error($form, t('%name does not have permission to edit any %type content.', array('%name' => $is_anon ? $anon : $name, '%type' => $type)));
        }
      }
      // Transform the username into the more stable user ID.
      $form_state['values']['project_issue_followup_user'] = $account->uid;
    }
    else {
      form_error($form, t('%name is not a valid user.', array('%name' => $name)));
    }
  }
}

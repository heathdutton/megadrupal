<?php

/**
 * @file
 * Token integration for the project_issue module.
 */

/**
 * Implements hook_token_info().
 */
function project_issue_token_info() {
  $info['tokens']['user']['project-issue-assignment-link'] = array(
    'name' => t('Project issue assignment link'),
    'description' => t('A link to the user, or <em>Unassigned</em> if user is anonymous (unlinked).'),
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function project_issue_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  $langcode = !empty($options['language']) ? $options['language']->language : NULL;
  $sanitize = !empty($options['sanitize']);

  if ($type == 'user' && !empty($data['user'])) {
    $account = $data['user'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'project-issue-assignment-link':
          if (!empty($account->uid)) {
            // If user is not anonymous, return the user
            $replacements[$original] = theme('username', array('account' => $account));
          }
          else {
            $replacements[$original] = t('Unassigned', array('langcode' => $langcode));
          }
          break;
      }
    }
  }

  return $replacements;
}

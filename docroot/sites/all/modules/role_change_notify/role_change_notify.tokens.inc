<?php

/**
 * @file
 * Token hooks for role_change_notify module.
 *
 */

/**
 * Implements hook_token_info().
 */
function role_change_notify_token_info() {
  $info['tokens']['user']['role-changed'] = array(
    'name' => t('Role Changed'),
    'description' => t('The role that was changed for the user'),
  );
  return $info;
}

/**
 * Implements hook_tokens().
 */
function role_change_notify_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);

  if ($type == 'user' && !empty($data['user'])) {
    $account = $data['user'];

    if (isset($tokens['role-changed']) && !empty($account->role_changed)) {
      $replacements[$tokens['role-changed']] = $sanitize ? check_plain($account->role_changed) : $account->role_changed;
    }
  }

  return $replacements;
}

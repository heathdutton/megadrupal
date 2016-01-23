<?php

/**
 * @file
 * Builds placeholder replacement tokens for user-related data.
 */

/**
 * Implements hook_token_info().
 */
function baidu_analytics_token_info() {
  // Create a new token type called baidu_analytics.
  $types['baidu_analytics'] = array(
    'name' => t('Baidu Analytics'),
    'description' => t('Tokens added by the Baidu Analytics module.'),
    'needs-data' => 'user',
  );
  $info['role-names'] = array(
    'name' => t('User role names'),
    'description' => t('The role names the user account is a member of as comma separated list.'),
    'needs-data' => 'user',
  );
  $info['role-ids'] = array(
    'name' => t('User role ids'),
    'description' => t('The role ids the user account is a member of as comma separated list.'),
    'needs-data' => 'user',
  );

  // To avoid token name conflicts with other modules such as Google Analytics,
  // add a new token type with user tokens and attach it as a token under user.
  $user_info['baidu_analytics'] = array(
    'type' => 'baidu_analytics',
    'name' => t('Baidu Analytics'),
    'description' => t('Tokens added by the Baidu Analytics module.'),
  );

  return array(
    'types' => $types,
    'tokens' => array(
      'user' => $user_info,
      'baidu_analytics' => $info,
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function baidu_analytics_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $sanitize = !empty($options['sanitize']);
  $replacements = array();

  if ($type == 'user' && !empty($data['user']->roles)) {
    $account = $data['user'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Basic user account information.
        case 'baidu_analytics:role-names':
          $names = implode(',', $account->roles);
          $replacements[$original] = $sanitize ? check_plain($names) : $names;
          break;

        case 'baidu_analytics:role-ids':
          $ids = implode(',', array_keys($account->roles));
          $replacements[$original] = $sanitize ? check_plain($ids) : $ids;
          break;
      }
    }
  }

  return $replacements;
}

<?php
/**
 * @file
 * Rules integration for party user.
 */

/**
 * Implements hook_rules_action_info()
 */
function party_user_rules_action_info() {
  $items['party_user_get_party'] = array(
    'label' => t('Get a party from a user'),
    'group' => t('Party'),
    'parameter' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
        'save' => FALSE,
        'optional' => TRUE,
      ),
    ),
    'provides' => array(
      'party' => array(
        'type' => 'party',
        'label' => t('Party'),
        'save' => FALSE,
      ),
    ),
    'base' => 'party_user_rules_get_party',
  );

  return $items;
}

/**
 * Implement the get a party from a user rule.
 *
 * @param stdClass $account
 *   A user object.
 * @param array $settings
 *   The rules config settings.
 *
 * @return array
 *   An array with the following key-value pairs:
 *   - party: The party object for the given user.
 */
function party_user_rules_get_party($account = NULL, $settings = array()) {
  if (!$account) {
    global $user;
    $account = $user;
  }

  return array('party' => party_user_get_party($account));
}

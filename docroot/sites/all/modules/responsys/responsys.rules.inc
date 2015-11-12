<?php

/**
 * @file
 * Rules integration with Responsys.
 */

/**
 * Implements hook_rules_action_info().
 */
function responsys_rules_action_info() {
  $actions = array();

  $actions['responsys_rules_sync_user'] = array(
    'label' => t('Sync a user with responsys'),
    'parameter' => array(
      'new_account' => array(
        'type' => 'user',
        'label' => t('The new user account.'),
      ),
      'original_account' => array(
        'type' => 'user',
        'label' => t('The original user account (optional).'),
        'optional' => TRUE,
        'allow null' => TRUE,
        'default' => NULL,
      ),
    ),
    'group' => t('Responsys'),
  );

  $actions['responsys_rules_pull_user'] = array(
    'label' => t('Update a user from Responsys'),
    'parameter' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('The user account to update from responsys'),
      ),
    ),
    'group' => t('Responsys'),
  );

  $actions['responsys_rules_delete_user'] = array(
    'label' => t('Unsubscribe a user from responsys'),
    'optional' => TRUE,
    'parameter' => array(
      'email' => array(
        'type' => 'text',
        'label' => t('Email address to unsubscribe'),
      ),
    ),
    'group' => t('Responsys'),
  );

  return $actions;
}

/**
 * Implements hook_rules_condition_info().
 */
function responsys_rules_condition_info() {
  return array(
    'responsys_account_just_synced' => array(
      'label' => t('User account has just synced with Responsys'),
      'parameter' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('The user account to check'),
        ),
      ),
      'help' => t('This condition returns TRUE if the specified accoutn has just synced with Responsys. This is useful because there are two functions to sync with Responsys, one from Responsys to Drupal and one from Drupal to Responsys. The Responsys down to Drupal will generate a user save casuing the other rule to then fire, but we dont want it to fire at this point'),
      'group' => t('Responsys'),
    ),
  );
}

<?php

/**
 * @file
 * Rules integration for the UC CCK Access ACL module.
 */

/**
 * Implementation of hook_rules_event_info().
 */
function uc_cck_access_acl_rules_event_info() {
  return array(
    'uc_cck_access_acl_grant' => array(
      'label' => t('CCK access is granted to a user'),
      'module' => t('UC CCK access ACL'),
      'arguments' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Node'),
        ),
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
        ),
      ),
    ),
    'uc_cck_access_acl_revoke' => array(
      'label' => t('CCK access is revoked from a user'),
      'module' => t('UC CCK access ACL'),
      'arguments' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Node'),
        ),
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
        ),
      ),
    ),
  );
}


/**
 * Implements hook_rules_action_info().
 */
function uc_cck_access_acl_rules_action_info() {
  return array(
  );
}


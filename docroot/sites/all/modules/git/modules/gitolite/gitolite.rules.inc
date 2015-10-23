<?php
/**
 * @file
 *  Rules integration
 */

/**
 * Implements hook_rules_action_info().
 */
function gitolite_rules_action_info() {
  $actions = array();

  $defaults = array(
    'group' => t('Gitolite'),
    'access callback' => 'gitolite_rules_integration_access',
  );

  $sshkey_defaults = $defaults + array(
    'named parameter' => TRUE,
    'parameter' => array(
      'sshkey' => array(
        'type' => 'sshkey',
        'label' => t('SSH Public Key'),
        'save' => FALSE,
      ),
      'server' => array(
        'type' => 'git_server',
        'label' => t('Gitolite Git Server'),
        'save' => FALSE,
      ),
    ),    
  );
  $actions['gitolite_sshkey_sync'] = $sshkey_defaults + array(
    'label' => t('Sync SSH Key to Gitolite server'),
    'base' => 'gitolite_add_user_public_key',
  );
  $actions['gitolite_sshkey_delete'] = $sshkey_defaults + array(
    'label' => t('Delete SSH Key from Gitolite server'),
    'base' => 'gitolite_delete_user_public_key',
  );

  $user_defaults = $defaults + array(
    'named parameter' => TRUE,
    'parameter' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('User'),
        'save' => FALSE,
      ),
      'server' => array(
        'type' => 'git_server',
        'label' => t('Gitolite Git Server'),
        'save' => FALSE,
      ),
    ),
  );
  $actions['gitolite_user_add'] = $user_defaults + array(
    'label' => t('Add user to gitolite server'),
    'base' => 'gitolite_add_user',
  );
  $actions['gitolite_update_user'] = $user_defaults + array(
    'label' => t('Update user in gitolite server'),
    'base' => 'gitolite_update_user',
  );
  $actions['gitolite_delete_user'] = $user_defaults + array(
    'label' => t('Delete user from gitolite server'),
    'base' => 'gitolite_delete_user',
  );

  return $actions;
}

/**
 * Rules integration access callback.
 */
function gitolite_rules_integration_access($type, $name) {
  return user_access('administer gitolite servers');
}

/**
 * Implements hook_rules_event_info().
 */
function gitolite_rules_event_info() {
  $events = array();

  $defaults = array(
    'group' => t('Gitolite'),
    'module' => 'gitolite',
    'access callback' => 'gitolite_rules_integration_access',
  );

  $events['gitolite_user_sshkey_insert'] = $defaults + array(
    'label' => t('After inserting a new ssh public key for a user'),
    'variables' => array(
      'sshkey' => array('type' => 'sshkey', 'label' => t('created sshkey')),
    ),
  );
  $events['gitolite_user_sshkey_update'] = $defaults + array(
    'label' => t('After updating a user\'s existing ssh public key'),
    'variables' => array(
      'sshkey' => array('type' => 'sshkey', 'label' => t('created sshkey')),
    ),
  );
  $events['gitolite_user_sshkey_delete'] = $defaults + array(
    'label' => t('After deleting a user\'s ssh public key'),
    'variables' => array(
      'sshkey' => array('type' => 'sshkey', 'label' => t('created sshkey')),
    ),
  );

  return $events;
}

<?php
/**
 * @file
 * Rules functions for ssh module.
 */

/**
 * Implements hook_rules_action_info().
 */
function ssh_rules_action_info() {
  return array(
    'ssh_execute' => array(
      'label' => t('Run SSH command'),
      'group' => t('System'),
      'access callback' => '_ssh_rules_access',
      'parameter' => array(
        'host' => array(
          'type' => 'integer',
          'label' => t('Remote host'),
          'optional' => FALSE,
          'options list' => '_ssh_rules_options',
        ),
        'command' => array(
          'type' => 'text',
          'label' => t('SSH command'),
          'optional' => FALSE,
          'description' => t('The actual command to be executed. Construct it to deliver a result just as you would enter it into the shell.'),
        ),
        'hide_command' => array(
          'type' => 'boolean',
          'label' => t('Hide command'),
          'description' => t('Although displaying actual SSH commands is limited to permitted roles, you may not want to reveal every command to them or to people reading your database log. In this cases, you may check this option to display a generic placeholder.'),
        ),
      ),
    ),
  );
}

/**
 * Wrapper to actual SSH execution.
 *
 * @see ssh_rules_action_info().
 *
 * @param $hid
 * @param $command
 */
function ssh_execute($hid, $command, $hide_command) {
  $ssh = new SSH();
  $ssh->execute($hid, $command, $hide_command);
}

/**
 * Returns a host options list for the rules action interface.
 */
function _ssh_rules_access() {

  return user_access(SSH_ADMIN_PERMISSION);

}

/**
 * Returns a host options list for the rules action interface.
 */
function _ssh_rules_options() {

  $ssh = new SSH();
  return $ssh->getHostOptions();

}

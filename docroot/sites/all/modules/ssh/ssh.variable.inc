<?php
/**
 * @file
 * Variable related functions for ssh module.
 */

/**
 * Implements hook_variable_info().
 */
function ssh_variable_info($options) {

  // Try to auto-detect the host's defaults, otherwise fall back to a
  // likely default.
  if (!$ssh_command = SSH::findExecutable()) {
    $ssh_command = '/usr/bin/ssh';
  }

  return array(
    'ssh_hosts' => array(
      'type' => 'array',
      'default' => array(),
      'description' => t('Array of pre-defined host sets.'),
      'title' => t('SSH remote hosts'),
    ),
    'ssh_executable' => array(
      'type' => 'string',
      'default' => $ssh_command,
      'description' => t('Path to the local SSH executable'),
      'title' => t('SSH executable'),
    ),
    'ssh_keyfile' => array(
      'type' => 'string',
      'default' => '~/.ssh/id_rsa',
      'description' => t('Path to the SSH keyfile to use'),
      'title' => t('SSH keyfile'),
    ),
    'ssh_hostsfile' => array(
      'type' => 'string',
      'default' => '~/.ssh/known_hosts',
      'description' => t('Path to the SSH known_hosts file to use'),
      'title' => t('SSH keyfile'),
    ),
    'ssh_default_port' => array(
      'type' => 'number',
      'default' => 22,
      'description' => t('Default SSH port to use is no dedicated host port is set.'),
      'title' => t('SSH default port'),
    ),
    'ssh_last_failure' => array(
      'type' => 'number',
      'default' => 0,
      'description' => t('Timestamp the most recently failed SSH execution.'),
      'title' => t('Last SSH failure'),
    ),
    'ssh_last_success' => array(
      'type' => 'number',
      'default' => 0,
      'description' => t('Timestamp of last successful SSH execution.'),
      'title' => t('Last SSH success'),
    ),
  );
}

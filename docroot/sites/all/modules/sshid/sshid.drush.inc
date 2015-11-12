<?php
/**
 * @file
 *  Drush commands for SSH module.
 */

/**
 * Implements hook_drush_command().
 */
function sshid_drush_command() {
  $items = array();

  $items['ssh-config'] = array(
    'description' => 'Render contents of the ~/.ssh/config file for the user Drupal runs as',
    'examples' => array(
      'drush ssh-config' => dt('Generate the contents of a complete ~/.ssh/config file for the user Drupal runs as'),
    ),
  );

  return $items;
}

/**
 * Implementation of ssh-config drush command. Renders ~/.ssh/config file.
 */
function drush_sshid_ssh_config() {
  $file = sshid_config_file_contents();
  drush_print($file);
  drush_print_pipe($file);
}

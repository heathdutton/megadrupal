<?php

/**
 * @file
 * Drush support for phpbb2drupal module
 */

/**
 * Implements hook_drush_help().
 */
function phpbb2drupal_drush_help($section) {
  switch ($section) {
    case 'drush:phpbb-rename-attachments':
      return dt('Rename all phpbb attachments from random name to real name');
    case 'drush:phpbb-rename-rollback':
      return dt('Rename all phpbb attachments from real name to random name');
  }
}

/**
 * Implements of hook_drush_command().
 */
function phpbb2drupal_drush_command() {
  static $commands = FALSE;

  $items['phpbb-rename-attachments'] = array(
    'description' => 'Rename all phpbb attachments from random name to real name',
    'arguments' => array(),
    'examples' => array(
      'phpbb-rename-attachments' => 'Rename Files',
    ),
    'drupal dependencies' => array('phpbb2drupal'),
  );
  $items['phpbb-rename-rollback'] = array(
    'description' => 'Rename all phpbb attachments back to random name',
    'arguments' => array(),
    'examples' => array(
      'phpbb-rename-rollback' => 'Rename Files',
    ),
    'drupal dependencies' => array('phpbb2drupal'),
  );
  return $items;
}

/**
 * Rename all the files in the attachements table.
 *
 * @param string $from Database field containing old file name
 * @param string $to   Database field containing new file name
 * @param bool $change Should we change to the files_directory is configured
 */
function drush_phpbb2drupal_do_rename($from, $to, $path = FALSE) {
  if (!PhpbbConfig::getVar('text_format')) {
    drush_log('Please ensure you have configured the phpbb migration', 'error');
    return;
  }

  $dir = $path ? $path : PhpbbConfig::getVar('files_directory');
  if (drupal_substr($dir, -1) != '/') {
    $dir .= '/';
  }

  PhpbbConfig::createConnection();
  $result = Database::getConnection('default', 'phpbb')
    ->select('{attachments}', 'a')
    ->fields('a', array('real_filename', 'physical_filename'))
    ->execute();

  $count = 0;
  while ($row = $result->fetchAssoc()) {
    if (@rename("file://{$dir}{$row[$from]}", "file://{$dir}{$row[$to]}")) {
      $count++;
    }
    else {
      drush_log("Unable to rename {$row[$from]}", 'warning');
    }
  }

  if ($count == 0) {
    drush_log('No files renamed. If you have a domain set, then this command must be run with the path to the directory that contains the files to be renamed.', 'error');
  }
  else {
    drush_log("{$count} files renamed.");
  }
}

function drush_phpbb2drupal_rename_attachments($path = '') {
  drush_phpbb2drupal_do_rename('physical_filename', 'real_filename', $path);
}

function drush_phpbb2drupal_rename_rollback($path = '') {
  drush_phpbb2drupal_do_rename('real_filename', 'physical_filename', $path);
}
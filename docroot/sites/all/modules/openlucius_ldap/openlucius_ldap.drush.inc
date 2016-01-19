<?php
/**
 * @file
 * This file contains all LDAP methods for drush.
 */

/**
 * Implements hook_drush_command().
 */
function openlucius_ldap_drush_command() {
  $items  = array();

  $items['openlucius_ldap_import'] = array(
    'callback'    => '_openlucius_ldap_sync_setup_import_batch',
    'description' => dt('Import users'),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function openlucius_ldap_drush_help($section) {
  switch ($section) {
    case 'drush:ldap_import':
      return dt("Imports users and map them to the right place.");

    default:
      return dt("Section not found");
  }
}

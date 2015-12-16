<?php
/**
 * @file
 * openlucius_ldap.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function openlucius_ldap_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'realname_pattern';
  $strongarm->value = '[user:field_user_display_name]';
  $export['realname_pattern'] = $strongarm;

  return $export;
}

<?php
/**
 * @file
 * og_ldap_api.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function og_ldap_api_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'og_ldap_api_admins'
  $field_bases['og_ldap_api_admins'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'og_ldap_api_admins',
    'foreign keys' => array(),
    'indexes' => array(
      'dn' => array(
        0 => 'dn',
      ),
    ),
    'locked' => 0,
    'module' => 'ldap_entry_reference',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 0,
    'type' => 'ldap_entry_reference',
  );

  // Exported field_base: 'og_ldap_api_members'
  $field_bases['og_ldap_api_members'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'og_ldap_api_members',
    'foreign keys' => array(),
    'indexes' => array(
      'dn' => array(
        0 => 'dn',
      ),
    ),
    'locked' => 0,
    'module' => 'ldap_entry_reference',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 0,
    'type' => 'ldap_entry_reference',
  );

  return $field_bases;
}

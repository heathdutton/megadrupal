<?php

/**
 * @file
 * Provides entity metadata.
 */

/**
 * Implements hook_entity_property_info_alter()
 */
function og_ldap_api_entity_property_info_alter(&$info) {
  if (isset($info['node']) && !empty($info['node']['bundles'])) {
    foreach ($info['node']['bundles'] as $bundle_name => &$bundle_info) {
      if (empty($bundle_info['properties']) || !og_is_group_type('node', $bundle_name)) {
        continue;
      }

      $bundle_info['properties']['ogldapIsLDAPCapable'] = array(
        'label' => t('Is LDAP capable'),
        'type' => 'boolean',
        'description' => t('Returns TRUE if the group is capable of being LDAP associated.'),
        'getter callback' => 'og_ldap_api_node_property_get',
        'computed' => TRUE,
      );
      $bundle_info['properties']['ogldapIsLDAPEnabled'] = array(
        'label' => t('Is LDAP enabled'),
        'type' => 'boolean',
        'description' => t('Returns TRUE if group is LDAP capable and groups settings are not empty.'),
        'getter callback' => 'og_ldap_api_node_property_get',
        'computed' => TRUE,
      );
      $bundle_info['properties']['ogldapIsLDAPWritable'] = array(
        'label' => t('Is LDAP writable'),
        'type' => 'boolean',
        'description' => t('Returns TRUE if this group can write membership updates back to the LDAP server.'),
        'getter callback' => 'og_ldap_api_node_property_get',
        'computed' => TRUE,
      );
    }
    unset($bundle_info);
  }
}

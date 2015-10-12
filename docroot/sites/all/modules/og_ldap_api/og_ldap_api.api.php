<?php

/**
 * @file
 * API documentation for OG LDAP.
 */

/**
 * group_server plugin 'update membership callback'.
 *
 * Update a user's group membership on the LDAP server.
 *
 * @param $op
 *   The operation to perform on the membership - grant or revoke.
 * @param $group_ldap_dn_values
 *   An array of group LDAP distinguished names
 * @param $account
 *   The user account object associated with the group membership.
 * @param $context
 *   An array of contextual information:
 *   'group_type': The entity type of the group object.
 *   'group': The group object.
 *   'account': The user account object associated with the group membership.
 *   'og_role_name': The OG role name.
 *
 * @return
 *   An array keyed by group ldap dn values with array values of TRUE if update
 *   was successful, FALSE otherwise.
 *
 * @ingroup og_ldap_api_group_server
 */
function CALLBACK_og_ldap_api_update_membership_callback($op, $group_ldap_dn_values, $account, $context) {
  // No example.
}


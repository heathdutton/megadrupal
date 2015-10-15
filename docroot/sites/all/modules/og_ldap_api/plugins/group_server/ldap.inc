<?php

/**
 * @file
 * The ldap module group_server plugin.
 */

if (module_exists('ldap_servers')) {
  $plugin = array(
    'title' => t('ldap module'),
    'update membership callback' => '_og_ldap_api_group_server_ldap_update_membership',
    'update capabilities' => array('grant', 'revoke'),
    'type' => 'ldap',
    'weight' => 0,
  );
}

/**
 * Update membership callback.
 *
 * @see CALLBACK_og_ldap_api_update_membership_callback()
 */
function _og_ldap_api_group_server_ldap_update_membership($op, $group_ldap_dn_values, $account, $context) {
  // Server class methods
  $op_methods = array(
    'grant' => 'groupAddMember',
    'revoke' => 'groupRemoveMember',
  );

  // Exit if no method defined for operation
  if (empty($op_methods[$op])) {
    return;
  }
  $op_method = $op_methods[$op];

  // Resolve LDAP server
  $group_server = _og_ldap_api_group_server_ldap_get_server('og');

  // Exit if no server or operation method does not exist
  if (empty($group_server) || !method_exists($group_server, $op_methods[$op])) {
    return;
  }

  // wrap the user account
  $account_wrapper = entity_metadata_wrapper('user', $account);

  // determine user's dn
  $account_ldap_dn = NULL;
  if (isset($account_wrapper->ldap_user_current_dn)) {
    $account_ldap_dn = $account_wrapper->ldap_user_current_dn->value();
  }

  // Search for dn
  if (empty($account_ldap_dn)) {
    $user_ldap_entry = $group_server->userUserToExistingLdapEntry($account);
    if (empty($user_ldap_entry) || empty($user_ldap_entry['dn'])) {
      return;
    }

    $account_ldap_dn = $user_ldap_entry['dn'];
  }

  // Perform update and build results
  $results = array();
  foreach ($group_ldap_dn_values as $group_ldap_dn) {
    $results[$group_ldap_dn] = 0;
    try {
      $is_ldap_member = $group_server->groupIsMember($group_ldap_dn, $account_ldap_dn);
      if ((!$is_ldap_member && $op == 'grant') || ($is_ldap_member && $op == 'revoke')) {
        $results[$group_ldap_dn] = (int) $group_server->{$op_method}($group_ldap_dn, $account_ldap_dn);

        // Error handling
        if (!$results[$group_ldap_dn]) {
          $err_number = ldap_errno($group_server->connection);

          switch ($group_server->ldap_type) {
            case 'openldap':
              // Open LDAP groupOfNames needs at least 1 member attribure
              if ($op == 'revoke' && $err_number == 65 && drupal_strtolower($group_server->groupObjectClass) == 'groupofnames') {
                // Add empty member - grant method cannot be used since it needs a user dn
                if ($group_server->groupGroupEntryMembershipsConfigured) {
                  $empty_add = array($group_server->groupMembershipsAttr => '');
                  $group_server->connectAndBindIfNotAlready();
                  $empty_result = @ldap_mod_add($group_server->connection, $group_ldap_dn, $empty_add);
                  /** @todo: should we remove these empties at some point? when add a new member? *****/
                }

                // Attempt to remove member again
                if ($empty_result) {
                  $results[$group_ldap_dn] = (int) $group_server->{$op_method}($group_ldap_dn, $account_ldap_dn);
                }
              }
              break;

          }

        }
      }
    }
    catch (Exception $e) {
      $results[$group_ldap_dn] = 0;
    }
  }

  return $results;
}

/**
 * Return the ldap module server controlling the given entry_type
 */
function _og_ldap_api_group_server_ldap_get_server($entry_type) {
  $entry_server = NULL;

  if (!empty($entry_type)) {
    if ($entry_type == 'user') {
      // Get user server
      if (module_exists('ldap_user')) {
        $user_conf = ldap_user_conf();
        if (!empty($user_conf->drupalAcctProvisionServer)) {
          $entry_server = ldap_servers_get_servers($user_conf->drupalAcctProvisionServer, 'enabled', TRUE);
        }
      }
    }
    elseif (in_array($entry_type, array('og', 'role'))) {
      // Get authorization server
      $consumer_type = NULL;
      if ($entry_type == 'og') {
        $consumer_type = 'og_group';
        $consumer_module = 'ldap_authorization_og';
      }
      elseif ($entry_type == 'role') {
        $consumer_type = 'drupal_role';
        $consumer_module = 'ldap_authorization_drupal_role';
      }
      if (isset($consumer_type) && module_exists($consumer_module)) {
        $consumer = ldap_authorization_get_consumer_object($consumer_type);
        if (!empty($consumer) && isset($consumer->consumerConf) &&
            !empty($consumer->consumerConf->status) && !empty($consumer->consumerConf->server)) {
          $entry_server = $consumer->consumerConf->server;
        }
      }
    }
  }

  // Resolve server to return
  $server = NULL;
  if (isset($entry_server)) {
    // Set if entry_server is enabled
    if (!empty($entry_server->status)) {
      $server = $entry_server;
    }
  }

  return $server;
}

<?php

/**
 * @file
 * The ldap module ldap_search plugin for ldap_entry_reference field.
 */
if (module_exists('ldap_servers')) {
  $plugin = array(
    'title' => t('ldap module'),
    'status callback' => 'ldap_entry_reference_search_ldap_status',
    'execute callback' => 'ldap_entry_reference_search_ldap_execute',
    'type' => 'ldap',
    'weight' => 0,
  );
}

/**
 * Status callback.
 *
 * @see CALLBACK_ldap_entry_reference_status_callback()
 */
function ldap_entry_reference_search_ldap_status($params = array()) {
  $status = FALSE;
  $entry_type = NULL;

  // Check specific base_dn or entry_type
  if (!empty($params)) {
    $entry_type = !empty($params['entry_type']) ? $params['entry_type'] : NULL;
  }

  // Get server for entry type
  $server = _ldap_entry_reference_search_ldap_get_server_by_entry_type($entry_type);
  if (empty($server)) {
    return FALSE;
  }

  // Test connection and binding
  try {
    $connect_result = $server->connect();
    if ($connect_result == LDAP_SUCCESS) {
      $bind_result = $server->bind();
      $status = ($bind_result == LDAP_SUCCESS);
    }
  }
  catch (Exception $e) {
    $status = FALSE;
  }

  return $status;
}

/**
 * Execute callback.
 *
 * @see CALLBACK_ldap_entry_reference_execute_callback()
 */
function ldap_entry_reference_search_ldap_execute($params = array()) {
  $entry_type = !empty($params['entry_type']) ? $params['entry_type'] : NULL;
  $server = _ldap_entry_reference_search_ldap_get_server_by_entry_type($entry_type);
  if (empty($server)) {
    return;
  }

  // query server
  $results = _ldap_entry_reference_search_ldap_perform_search($server, $params);

  return $results;
}

/**
 * Perform search on ldap module's server class
 */
function _ldap_entry_reference_search_ldap_perform_search(LdapServer $server, $params) {
  ldap_servers_module_load_include('php', 'ldap_servers', 'LdapServer.class');

  $search_base_dns = array();
  if (!empty($params['base_dn'])) {
    $search_base_dns = array($params['base_dn']);
  }

  if (!empty($server->basedn)) {
    if (!empty($search_base_dns)) {
      // Remove requested bases that are not in the server's bases
      foreach (array_values($search_base_dns) as $k => $search_base_dn) {
        foreach ($server->basedn as $server_base_dn) {
          if (stripos($search_base_dn, $server_base_dn) === FALSE) {
            unset($search_base_dns[$k]);
          }
        }
      }
    }
    else {
      // default to server base dn's
      $search_base_dns = $server->basedn;
    }
  }

  // Exit if no base dns
  if (empty($search_base_dns)) {
    return array();
  }

  // Determine ldap modules' search scope
  $search_scope = LDAP_SCOPE_SUBTREE;
  if (!empty($params['scope'])) {
    switch ($params['scope']) {
      case 'base':
        $search_scope = LDAP_SCOPE_BASE;
        break;

      case 'one':
        $search_scope = LDAP_SCOPE_ONELEVEL;
        break;

      case 'sub':
        $search_scope = LDAP_SCOPE_SUBTREE;
        break;
    }
  }

  // Connect and bind
  $server->connectAndBindIfNotAlready();

  // Query server
  $results = array();
  $count = 0;
  foreach ($search_base_dns as $base_dn) {
    $result = $server->search($base_dn,
                              $params['filter'],
                              $params['attributes'],
                              $params['attrsonly'],
                              $params['sizelimit'],
                              $params['timelimit'],
                              $params['deref'],
                              $search_scope);

    if ($result !== FALSE && $result['count'] > 0) {
      $count = $count + $result['count'];
      $results = array_merge($results, $result);
    }
  }
  $results['count'] = $count;

  $server->disconnect();
  return $results;
}

/**
 * Return the ldap module server controlling the given entry_type
 */
function _ldap_entry_reference_search_ldap_get_server_by_entry_type($entry_type) {
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
  else {
    // Default to first enabled server
    $enabled_servers = ldap_servers_get_servers(NULL, 'enabled', TRUE);
    if (!empty($enabled_servers)) {
      $server = reset($enabled_servers);
    }
  }

  return $server;
}

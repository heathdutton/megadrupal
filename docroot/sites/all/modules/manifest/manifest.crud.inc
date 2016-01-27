<?php

/**
 * @file
 * Manifest module database functions.
 */

/**
 * Save a manifest.
 */
function manifest_save(&$manifest) {
  manifest_delete($manifest->name);
  drupal_write_record('manifest', $manifest);
}

/**
 * Load a manifest.
 *
 * Loads one manifest.
 */
function manifest_load($manifest_name = NULL) {
  $manifests = manifest_load_multiple($manifest_name);
  if (!empty($manifests)) {
    return reset($manifests);
  }
  return FALSE;
}

/**
 * Load manifests.
 *
 * Loads all manifests.
 */
function manifest_load_multiple($manifest_name = NULL) {
  $result = manifest_query_execute('manifest', 'load', array('name' => $manifest_name));
  $manifests = array();
  foreach ($result as $manifest) {
    $manifest->settings = unserialize($manifest->settings);
    manifest_config_load($manifest);
    $manifests[$manifest->name] = $manifest;
  }
  if (!empty($manifests)) {
    return $manifests;
  }
  return FALSE;
}

/**
 * Delete a manifest.
 */
function manifest_delete($manifest_name) {
  manifest_account_delete(NULL, $manifest_name);
  manifest_role_delete(NULL, $manifest_name);
  manifest_ip_delete(NULL, NULL, $manifest_name);
  manifest_config_delete($manifest_name);
  return manifest_query_execute('manifest', 'delete', array('name' => $manifest_name));
}

/**
 * Save a manifest config entry.
 */
function manifest_config_save($manifest_name, $field, $value) {
  if (!is_array($value)) {
    $value = array($value);
  }
  $entry = (object)array();
  for ($i = 0; $i < count($value); $i++) {
    $entry->manifest = $manifest_name;
    $entry->field = $field;
    $entry->delta = $i;
    $entry->value = current($value);
    next($value);
  }
  drupal_write_record('manifest_config', $entry);
}

/**
 * Load a manifest config entry into a manifest.
 */
function manifest_config_load(&$manifest) {
  $result = manifest_query_execute('manifest_config', 'load', array('manifest' => $manifest->name));
  $manifest->config = array();
  foreach ($result as $row) {
    $manifest->config[$row->field][$row->delta] = $row->value;
  }
}

/**
 * Delete a manifest.
 */
function manifest_config_delete($manifest_name) {
  return manifest_query_execute('manifest_config', 'delete', array('manifest' => $manifest_name));
}

/**
 * Save a manifest user entry.
 */
function manifest_account_save($uid, $manifest_name) {
  $existing = manifest_account_load($uid, $manifest_name);
  if (!$existing) {
    $entry = (object)array(
      'uid' => $uid,
      'manifest' => $manifest_name,
      'created' => REQUEST_TIME,
    );
    drupal_write_record('manifest_user', $entry);
  }
}

/**
 * Load a manifest user entry.
 */
function manifest_account_load($uid = NULL, $manifest_name = NULL) {
  $result = manifest_query_execute('manifest_user', 'load', array('uid' => $uid, 'manifest' => $manifest_name));
  $entries = array();
  foreach ($result as $row) {
    $entries[] = $row;
  }
  if (!empty($entries)) {
    return $entries;
  }
  return FALSE;
}

/**
 * Delete a manifest user entry.
 */
function manifest_account_delete($uid = NULL, $manifest_name = NULL) {
  return manifest_query_execute('manifest_user', 'delete', array('uid' => $uid, 'manifest' => $manifest_name));
}

/**
 * Save a manifest role entry.
 */
function manifest_role_save($rid, $manifest_name) {
  $existing = manifest_role_load($rid, $manifest_name);
  if (!$existing) {
    $entry = (object)array(
      'rid' => $rid,
      'manifest' => $manifest_name,
      'created' => REQUEST_TIME,
    );
    drupal_write_record('manifest_role', $entry);
  }
}

/**
 * Load a manifest role entry.
 */
function manifest_role_load($rid = NULL, $manifest_name = NULL) {
  $result = manifest_query_execute('manifest_role', 'load', array('rid' => $rid, 'manifest' => $manifest_name));
  $entries = array();
  foreach ($result as $row) {
    $entries[] = $row;
  }
  if (!empty($entries)) {
    return $entries;
  }
  return FALSE;
}

/**
 * Delete a manifest role entry.
 */
function manifest_role_delete($rid = NULL, $manifest_name = NULL) {
  return manifest_query_execute('manifest_role', 'delete', array('rid' => $rid, 'manifest' => $manifest_name));
}

/**
 * Save a manifest ip entry.
 */
function manifest_ip_save($ip1, $ip2, $manifest_name) {
  $existing = manifest_ip_load($ip1, $ip2, $manifest_name);
  if (!$existing) {
    $entry = (object)array(
      'ip1' => ip2long($ip1),
      'ip2' => $ip2 ? ip2long($ip2) : ip2long($ip1),
      'manifest' => $manifest_name,
      'created' => REQUEST_TIME,
    );
    drupal_write_record('manifest_ip', $entry);
  }
}

/**
 * Load a manifest ip entry.
 */
function manifest_ip_load($ip1 = NULL, $ip2 = NULL, $manifest_name = NULL) {
  $result = manifest_query_execute('manifest_ip', 'load', array('ip1' => $ip1, 'ip2' => $ip2, 'manifest' => $manifest_name));
  $entries = array();
  foreach ($result as $row) {
    $row->ip1 = long2ip($row->ip1);
    $row->ip2 = long2ip($row->ip2);
    $entries[] = $row;
  }
  if (!empty($entries)) {
    return $entries;
  }
  return FALSE;
}

/**
 * Load a manifest ip entry.
 */
function manifest_ip_match($ip, $manifest_name = NULL) {
  $wheres[] = ip2long($ip) . ' >= ip1';
  $wheres[] = ip2long($ip) . ' <= ip2';
  $result = manifest_query_execute('manifest_ip', 'load', array('manifest' => $manifest_name), array(), $wheres);

  $entries = array();
  foreach ($result as $row) {
    $entries[] = $row;
  }
  if (!empty($entries)) {
    return $entries;
  }
  return FALSE;
}

/**
 * Delete a manifest ip entry.
 */
function manifest_ip_delete($ip1 = NULL, $ip2 = NULL, $manifest_name = NULL) {
  return manifest_query_execute('manifest_ip', 'delete', array('ip1' => ip2long($ip1), 'ip2' => ip2long($ip2), 'manifest' => $manifest_name));
}

/**
 * Assemble SQL queries.
 */
function manifest_query_execute($table, $op = 'load', $conditions = array(), $select = array(), $wheres = array(), $args = array()) {
  $placeholder_count = 0;
  if ($op == 'load') {
    $sql = 'SELECT ';
    if ($select) {
      $sql .= implode(', ', $select);
    }
    else {
      $sql .= '*';
    }
  }
  elseif ($op == 'delete') {
    $sql = 'DELETE';
  }
  $sql .= ' FROM {' . $table . '} ';
  $schema = drupal_get_schema($table);
  foreach ($conditions as $field => $values) {
    if (!empty($values)) {
      if (!is_array($values)) {
        $values = array($values);
      }
      $type = $schema['fields'][$field]['type'];
      $placeholder = ':value_' . $placeholder_count++;
      $wheres[] = $field . ' IN (' . $placeholder . ')';
      $args[$placeholder] = $values;
    }
  }
  if (!empty($wheres)) {
    $sql .= 'WHERE ';
    $sql .= implode(' AND ', $wheres);
  }
  return db_query($sql, $args);
}

/**
 * Get a list of manifests the user is in, matching the settings.
 *
 * @param $config
 *   An array of key values that must be true of the $manifest->config.
 * @param $account
 *   An optional user account object.  If not supplied will use the current
 *   user.
 * @return
 *   An array of loaded manifests.
 */
function manifest($config = NULL, $account = NULL) {
  if (is_null($account)) {
    global $user;
    $account = $user;
    $check_ip = TRUE;
  }

  $sql = 'SELECT DISTINCT(m.name) AS manifest FROM {manifest} m';
  $args = array();
  $wheres = array();

  $sql .= ' LEFT JOIN {manifest_user} mu ON mu.manifest = m.name';
  $sql .= ' LEFT JOIN {manifest_role} mr ON mr.manifest = m.name';
  if ($check_ip) {
    $sql .= ' LEFT JOIN {manifest_ip} mi ON mi.manifest = m.name';
  }

  if (!empty($config)) {
    $placeholder_count = 0;
    foreach ($config as $key => $value) {
      $placeholder = $placeholder_count++;
      $sql .= ' INNER JOIN {manifest_config} mc_' . $key . ' ON mc_' . $key . '.manifest = m.name AND mc_' . $key . '.field = :key_' . $placeholder . ' AND mc_' . $key . '.value = :val_' . $placeholder;
      $args[':key_' . $placeholder] = $key;
      $args[':val_' . $placeholder] = $value;
    }
  }

  $wheres[] = 'mu.uid = :uid';
  $args[':uid'] = $account->uid;

  $roles = array_keys($account->roles);
  $wheres[] = 'mr.rid IN (:rids)';
  $args[':rids'] = $roles;

  if ($check_ip) {
    $wheres[] = '(:ip >= mi.ip1 AND :ip <= mi.ip2)';
    $args[':ip'] = ip2long(ip_address());
  }

  $sql .= ' WHERE ' . implode(' OR ', $wheres);

  $result = db_query($sql, $args);
  $manifests = array();
  foreach ($result as $row) {
    $manifests[] = $row->manifest;
  }
  if (!empty($manifests)) {
    return $manifests;
  }
  return FALSE;
}

/**
 * Get a list of users for the supplied manifests, matching the settings.
 *
 * @param $config
 *   An array of key values that must be true of the $manifest->config.
 * @param $manifests
 *   Which manifests to look in.
 * @param $from
 *   For range query, the from param.
 * @param $count
 *   For range query, the count param.
 * @param $get_reasons
 *   Will return array of objects, with data about why a user was in a manifest.
 * @return
 *   An array of uids.
 */
function manifest_users($config = NULL, $manifests = NULL, $from = NULL, $count = NULL, $get_reasons = NULL) {
  $sql = 'SELECT DISTINCT(u.uid) AS uid';
  if ($get_reasons) {
    $sql .= ', (mu.uid = u.uid) AS by_user, (ur.rid = mr.rid) AS by_role';
  }
  $sql .= ' FROM {users} u';
  $args = array();

  $sql .= ' LEFT JOIN {manifest_user} mu ON mu.uid = u.uid';
  $sql .= ' LEFT JOIN {users_roles} ur ON u.uid = ur.uid';
  $sql .= ' LEFT JOIN {manifest_role} mr ON ur.rid = mr.rid';

  if (!empty($config)) {
    $placeholder_count = 0;
    foreach ($config as $key => $value) {
      $placholder = $placeholder_count++;
      $sql .= ' INNER JOIN {manifest_config} mc_' . $key . ' ON mu.manifest = mc_' . $key . '.manifest AND mc_' . $key . '.field = :key_' . $placeholder . ' AND mc_' . $key . '.value = :val_' . $placeholder;
      $args[':key_' . $placeholder] = $key;
      $args[':val_' . $placeholder] = $value;
    }
  }

  if (!empty($manifests)) {
    $wheres[] = 'mu.manifest IN (:manifests)';
    $args[':manifests'] = $manifests;
  }

  $wheres[] = '(mu.uid IS NOT NULL OR mr.rid IS NOT NULL)';

  if (!empty($wheres)) {
    $sql .= ' WHERE ' . implode(' AND ', $wheres);
  }

  if (!is_null($from) && !is_null($count)) {
    $result = db_query_range($sql, $from, $count, $args);
  }
  else {
    $result = db_query($sql, $args);
  }
  $users = array();
  foreach ($result as $row) {
    $users[$row->uid] = $get_reasons ? $row : $row->uid;
  }
  if (!empty($users)) {
    return $users;
  }
  return FALSE;
}
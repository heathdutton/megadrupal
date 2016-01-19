<?php

/**
 * @file
 * Contains functions for the automatic database check.
 */

/**
 * Runs a full database audit.
 */
function account_sentinel_audit() {
  account_sentinel_audit_existence();
  account_sentinel_audit_integrity();
  account_sentinel_audit_changes();
  watchdog('account_sentinel', 'Ran database audit.');
  variable_set('account_sentinel_audit_last', REQUEST_TIME);
}

/**
 * Checks whether stored snapshots are valid.
 */
function account_sentinel_audit_integrity() {
  $hash_key = drupal_get_hash_salt();
  $events = array();

  // Check integrity in account_sentinel_users.
  $query = db_select('account_sentinel_users', 'u')
    ->fields('u', array('uid', 'name', 'pass', 'mail', 'status', 'checksum'));
  $query->where('checksum <> md5(concat(u.uid, u.name, u.pass, u.mail, u.status, :hash_key))', array(':hash_key' => $hash_key));
  $result = $query->execute();

  while (($row = $result->fetchAssoc()) !== FALSE) {
    $events[$row['uid']][] = array(
      'type' => ACCOUNT_SENTINEL_EVENT_TYPE_SNAPSHOT_INVALID,
      'data' => array(
        'table' => 'users',
        'row' => $row,
      ),
    );
  }

  // Check integrity in account_sentinel_users_roles.
  $query = db_select('account_sentinel_users_roles', 'ur')
    ->fields('ur', array('uid', 'rid', 'checksum'));
  $query->where('checksum <> md5(concat(ur.uid, ur.rid, :hash_key))', array(':hash_key' => $hash_key));
  $result = $query->execute();

  while (($row = $result->fetchAssoc()) !== FALSE) {
    $events[$row['uid']][] = array(
      'type' => ACCOUNT_SENTINEL_EVENT_TYPE_SNAPSHOT_INVALID,
      'data' => array(
        'table' => 'users_roles',
        'row' => $row,
      ),
    );
  }

  foreach ($events as $uid => $event_list) {
    account_sentinel_record_events($uid, ACCOUNT_SENTINEL_EVENT_ORIGIN_DB_CHECK, $event_list);
    if (($user = user_load($uid)) !== FALSE) {
      $user = account_sentinel_monitored_account_data($user);
      account_sentinel_update_snapshot($user);
    }
  }
}

/**
 * Checks whether trusted users are the same as the users who have snapshots.
 */
function account_sentinel_audit_existence() {
  $monitored_roles = account_sentinel_get_monitored_roles();

  if (!empty($monitored_roles)) {
    // Check for missing snapshots.
    $missing = db_select('users', 'u');
    $missing->fields('u', array('uid'));
    $missing->rightJoin('users_roles', 'ur', 'u.uid = ur.uid');
    $missing->condition('ur.rid', $monitored_roles);
    $missing->leftJoin('account_sentinel_users', 'asu', 'u.uid = asu.uid');
    $missing->fields('asu', array('uid'));
    $missing->condition('asu.uid', NULL);

    $result = $missing->execute();
    while (($row = $result->fetchAssoc()) !== FALSE) {
      $uid = $row['uid'];

      // UID can be NULL if users_roles contains roles of a forcibly deleted
      // user. That case will be handled in the second part of the function, so
      // we only work with existing users now.
      if (is_numeric($uid)) {
        $changes = array();
        $changes[] = array(
          'type' => ACCOUNT_SENTINEL_EVENT_TYPE_SNAPSHOT_MISSING,
          'data' => array(),
        );

        // A missing snapshot means that the user was granted one or more
        // monitored permissions, so it's also an event type of ROLE_ADD.
        // Get the roles of the user.
        $user = user_load($uid);
        if ($user !== FALSE) {
          $roles = array_intersect($monitored_roles, array_keys($user->roles));

          // Current roles.
          $new = array(
            'uid' => $uid,
            'roles' => $roles,
          );

          // Roles previously stored as snapshots.
          $select_original = db_select('account_sentinel_users_roles', 'asur');
          $select_original->fields('asur', array('rid'))
            ->condition('uid', $uid);
          $original_roles_queried = $select_original->execute();
          $original_roles = $original_roles_queried->fetchAllAssoc('rid');
          $original = array(
            'uid' => $uid,
            'roles' => array_keys($original_roles),
          );

          $changes = array_merge(
            $changes,
            account_sentinel_detect_changes($new, $original)
          );
        }

        account_sentinel_record_events($uid, ACCOUNT_SENTINEL_EVENT_ORIGIN_DB_CHECK, $changes);
        if ($user !== FALSE) {
          $user = account_sentinel_monitored_account_data($user);
          account_sentinel_update_snapshot($user);
        }
      }
    }
  }

  // Check for excess snapshots.
  // Revoked roles are detected in account_sentinel_audit_changes(), so we only
  // look for missing rows in the users table.
  $excess = db_select('users', 'u');
  $excess->fields('asu', array('uid', 'name', 'mail'))
    ->condition('u.uid', NULL)
    ->rightJoin('account_sentinel_users', 'asu', 'u.uid = asu.uid');
  $result = $excess->execute();
  while (($row = $result->fetchAssoc()) !== FALSE) {
    $changes = array(
      array(
        'type' => ACCOUNT_SENTINEL_EVENT_TYPE_USER_DELETE,
        'data' => array(
          'uid' => $row['uid'],
          'name' => $row['name'],
          'mail' => $row['mail'],
        ),
      ),
    );
    account_sentinel_record_events($row['uid'], ACCOUNT_SENTINEL_EVENT_ORIGIN_DB_CHECK, $changes);
    account_sentinel_delete_snapshot($row['uid']);
  }
}

/**
 * Scans the database for manual changes in users and users_roles.
 *
 * Compares the monitored users' actual data to Account Sentinel's stored
 * snapshots.
 */
function account_sentinel_audit_changes() {
  $accounts = array();
  $monitored_roles = account_sentinel_get_monitored_roles();

  // Look for changes in user details.
  $query = db_select('users', 'u');
  $query->fields('u', array('uid', 'name', 'pass', 'mail', 'status'))
    ->fields('asu', array('name', 'pass', 'mail', 'status'))
    ->where('u.name <> asu.name OR u.pass <> asu.pass OR u.mail <> asu.mail OR u.status <> asu.status')
    ->rightJoin('account_sentinel_users', 'asu', 'u.uid = asu.uid');
  $result = $query->execute();

  while (($row = $result->fetchAssoc()) !== FALSE) {
    $accounts[$row['uid']] = array(
      'new' => array(
        'uid' => $row['uid'],
        'name' => $row['name'],
        'pass' => $row['pass'],
        'mail' => $row['mail'],
        'status' => $row['status'],
      ),
      'original' => array(
        'uid' => $row['uid'],
        'name' => $row['asu_name'],
        'pass' => $row['asu_pass'],
        'mail' => $row['asu_mail'],
        'status' => $row['asu_status'],
      ),
    );
  }

  if (!empty($monitored_roles)) {
    // Look for added user roles.
    $added = db_select('users_roles', 'ur');
    $added->fields('ur', array('uid', 'rid'))
      ->condition('ur.rid', $monitored_roles)
      ->notExists(
        db_select('account_sentinel_users_roles', 'asur')
          ->fields('asur', array())
          ->where('ur.rid = asur.rid')
      );
    $added_roles = $added->execute();
    while (($row = $added_roles->fetchAssoc()) !== FALSE) {
      $accounts[$row['uid']]['new']['roles'][] = $row['rid'];
      if (!isset($accounts[$row['uid']]['original']['roles'])) {
        $accounts[$row['uid']]['original']['roles'] = array();
      }
    }
  }

  // Look for removed user roles.
  $removed = db_select('account_sentinel_users_roles', 'asur');
  $removed->fields('asur', array('uid', 'rid'))
    ->notExists(
      db_select('users_roles', 'ur')
        ->fields('ur', array())
        ->where('ur.rid = asur.rid')
    );
  $removed_roles = $removed->execute();
  while (($row = $removed_roles->fetchAssoc()) !== FALSE) {
    $accounts[$row['uid']]['original']['roles'][] = $row['rid'];
    if (!isset($accounts[$row['uid']]['new']['roles'])) {
      $accounts[$row['uid']]['new']['roles'] = array();
    }
  }

  // Evaluate changes.
  foreach ($accounts as $uid => $account_states) {
    $changes = account_sentinel_detect_changes(
      $account_states['new'],
      $account_states['original']
    );

    account_sentinel_record_events($uid, ACCOUNT_SENTINEL_EVENT_ORIGIN_DB_CHECK, $changes);
    if (($user = user_load($uid)) !== FALSE) {
      $user = account_sentinel_monitored_account_data($user);
      account_sentinel_update_snapshot($user);
    }
  }
}

<?php
/**
 * @file
 * Identity related functions
 */

/**
 * Ask Drupal for the external identifiers for this user.
 */
function _janrain_user_has_capture_uuid() {
  $authmaps = db_query("SELECT authname FROM {authmap} WHERE uid = :uid AND module = 'janrain'", array(':uid' => $GLOBALS['user']->uid))->fetchAll();
  // Blarg regex validation of uuid :unamused:
  foreach ($authmaps as &$map) {
    if (1 === preg_match('|^[0-9a-f]{8}-[0-9a-f]{4}-4[0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}$|', $map->authname)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Extends Drupal's limited "single authmap" adding semantics.
 */
function _janrain_ensure_authmap($account, $authname) {
  if (!user_get_authmaps($authname)) {
    // This authname has not been used.
    db_insert('authmap')
      ->fields(array(
        'uid' => $account->uid,
        'authname' => $authname,
        'module' => 'janrain',
        ))
      ->execute();
    watchdog('janrain', 'Linked {{authname}} to {{user}}.', array(
      '{{authname}}' => filter_xss($authname),
      '{{user}}' => format_username($account)), WATCHDOG_INFO);
  }
  else {
    watchdog('janrain', '{{user}} already linked to {{authname}}', array(
      '{{authname}}' => filter_xss($authname),
      '{{user}}' => format_username($account)), WATCHDOG_DEBUG);
  }
}

/**
 * Wrapper to access session identifiers.
 * @todo-3.1 move this into sdk
 */
function _janrain_get_identifiers() {
  return DrupalAdapter::getSessionItem('identifiers') ?: array();
}

/**
 * Ensure all identifiers found in the current session are linked.
 */
function _janrain_link_identifiers($account) {
  $identifiers = _janrain_get_identifiers();
  foreach ($identifiers as $ext_id) {
    _janrain_ensure_authmap($account, $ext_id);
  }
}

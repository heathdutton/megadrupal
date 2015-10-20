<?php
/**
 * @file
 * Db functions for the unilogin project.
 *
 * @author Mikkel Jakobsen <mikkel@adapt.dk>
 */

/**
 * Load a ticket from db. Returns FALSE if there is no match.
 *
 * @param string $name
 *   The generated name of the ticket.
 *
 * @return mixed
 *   array/FALSE
 */
function _unilogin_db_load_ticket($name) {
  if (empty($name)) {
    return FALSE;
  }

  return db_query(
    'SELECT * FROM {unilogin_tickets} WHERE name = :name',
    array(':name' => $name)
  )->fetchObject();
}

/**
 * Save a ticket.
 *
 * @param string $auth
 *   Auth token.
 * @param integer $timestamp
 *   UNI•Login timestamp.
 * @param string $unilogin_id
 *   UNI•Login user id.
 *
 * @return void
 *   Nothing to return.
 */
function _unilogin_db_save_ticket($auth, $timestamp, $unilogin_id) {
  if (empty($auth) || empty($timestamp) || empty($unilogin_id)) {
    return;
  }
  $tname = _unilogin_construct_ticket_name($auth, $timestamp, $unilogin_id);
  $ticket = new stdClass();
  $ticket->name = $tname;
  $ticket->created = time();
  if (!_unilogin_db_load_ticket($tname)) {
    drupal_write_record('unilogin_tickets', $ticket);
  }
}

/**
 * Get UNI•Login id by user id.
 *
 * @param integer $uid
 *   Drupal user id.
 *
 * @return mixed
 *   string/FALSE - Either unilogin id or FALSE.
 */
function _unilogin_db_get_unilogin_id_by_uid($uid) {
  $query = db_select('authmap', 'am');
  $query->fields('am');
  $query->condition('am.uid', $uid, '=');
  $query->condition('am.module', 'unilogin', '=');
  $authname = $query->execute()->fetchObject()->authname;
  if (!empty($authname)) {
    return _unilogin_id_from_authname($authname);
  }
  return FALSE;
}

/**
 * Get user id by UNI•Login id.
 *
 * @param string $unilogin_id
 *   UNI•Login id.
 *
 * @return mixed
 *   integer/FALSE - Either Drupal user id or FALSE.
 */
function _unilogin_db_get_uid_by_unilogin_id($unilogin_id) {
  $query = db_select('authmap', 'am');
  $query->fields('am');
  $query->condition('am.authname', unilogin_authname($unilogin_id), '=');
  $query->condition('am.module', 'unilogin', '=');
  $auth = $query->execute()->fetchObject();
  return !empty($auth) ? (int) $auth->uid : FALSE;
}

/**
 * Delete unilogin id. Deletes relation between uid and unilogin id.
 *
 * @param integer $uid
 *   Drupal user id.
 */
function _unilogin_db_delete_unilogin_id($uid) {
  db_delete('authmap')->condition('uid', $uid)->execute();
}

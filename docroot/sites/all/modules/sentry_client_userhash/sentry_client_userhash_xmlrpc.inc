<?php
// $Id: sentry_client_userhash_xmlrpc.inc,v 1.1.2.1 2010/12/17 19:44:06 nevergone Exp $

/**
 * @file
 * XML-RPC callback, these will be loaded only when someone hits the xmlrpc.php
 */

/**
 * Callback to return the hash of users username, e-mail and password.
 */
function xmlc_sentry_client_userhash($key) {
  if (sentry_client_validate($key)) {
    $setting = variable_get('sentry_client_userhash_settings', 0);
    if ($setting == 0) {
      // Monitoring admin user.
      $result = db_query('SELECT uid, name, pass, mail FROM {users} WHERE uid = :uid', array(':uid' => 1));
      foreach ($result as $account) {
        $hashes[$account->uid] = md5($account->name . $account->pass . $account->mail);
      }
    }
    else {
      // Monitoring selected group, $setting: group id
      $result = db_query('SELECT u.uid, u.name, u.pass, u.mail FROM {users} u INNER JOIN {users_roles} ur ON u.uid = ur.uid WHERE ur.rid = :rid', array(':rid' => $setting));
      foreach ($result as $account) {
        $hashes[$account->uid] = md5($account->name . $account->pass . $account->mail);
      }
    }
    return $hashes;
  }
}

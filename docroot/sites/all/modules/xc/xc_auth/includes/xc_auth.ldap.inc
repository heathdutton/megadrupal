<?php
/**
 * @file
 * XC Authentication LDAP functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

// LDAP INTEGRATION EXPERIMENTAL!!!

/**
 * @addtogroup global variables
 * @{
 */
/**
 * LDAP client
 * @var LDAPInterface
 *   An LDAPInterface object
 */
global $_xc_auth_ldap;

/**
 * Flag indicates whether LDAP authentication is enabled or not.
 * @var boolean
 */
global $_xc_auth_ldap_enabled;
/**
 * @} End of "addtogroup global variables".
 */


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_auth_method_ldap_form(&$form_state) {
  $result = db_query("SELECT sid, name FROM {ldapauth}");

  $servers = array();
  foreach ($result as $server) {
    $servers[$server->sid] = $server->name;
  }

  $form['sid'] = array(
    '#type' => 'checkboxes',
    '#title' => t('LDAP Server(s)'),
    '#options' => $servers,
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_auth_method_ldap_validate($form, &$from_state) { }

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_auth_method_ldap_submit($form, &$from_state) { }

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_auth_method_login_ldap_validate($form, &$from_state) { }

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_auth_method_login_ldap_submit($form, &$from_state) { }

/**
 * This is practically a copy of the functions from the LDAP Integration
 * module to prevent conflict with that module's LDAP object instance
 */
function xc_auth_ldap_authenticate($sid, $user, $pass) {
  global $_xc_auth_ldap, $_xc_auth_ldap_enabled;

  if (!$_xc_auth_ldap_enabled) {
    return FALSE;
  }

  $result = db_query("SELECT sid FROM {ldapauth} WHERE status = :status ORDER BY weight", array(':status' => 1));
  foreach ($result as $row) {
    if (!xc_ldap_server_init($row->sid)) {
      return FALSE;
    }

    if (!($ldap = xc_auth_ldap_user_lookup($user)) || !isset($ldap['dn'])) {
      continue;
    }

    if (($code = xc_auth_ldap_info($row->sid, 'filter_php')) && !eval($code)) {
      continue;
    }

    if (!$_xc_auth_ldap->connect($ldap['dn'], $pass)) {
      continue;
    }

    return $ldap['dn'];
  }
  return FALSE;
}

function xc_ldap_server_init($sid) {
  global $_xc_auth_ldap;

  static $servers = array();
  if (!isset($servers[$sid])) {
    $servers[$sid] = db_query("SELECT * FROM {ldapauth} WHERE status = :status AND sid = :sid", array(':status' => 1, ':sid' => $sid))->fetchObject();
  }

  if ($servers[$sid]) {
    $_xc_auth_ldap = new LDAPInterface();
    $_xc_auth_ldap->setOption('sid', $sid);
    $_xc_auth_ldap->setOption('name', $servers[$sid]->name);
    $_xc_auth_ldap->setOption('server', $servers[$sid]->server);
    $_xc_auth_ldap->setOption('port', $servers[$sid]->port);
    $_xc_auth_ldap->setOption('tls', $servers[$sid]->tls);
    $_xc_auth_ldap->setOption('encrypted', $servers[$sid]->encrypted);
    $_xc_auth_ldap->setOption('basedn', $servers[$sid]->basedn);
    $_xc_auth_ldap->setOption('user_attr', $servers[$sid]->user_attr);
    $_xc_auth_ldap->setOption('mail_attr', $servers[$sid]->mail_attr);
    $_xc_auth_ldap->setOption('binddn', $servers[$sid]->binddn);
    $_xc_auth_ldap->setOption('bindpw', $servers[$sid]->bindpw);
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_auth_ldap_user_lookup($user) {
  global $_xc_auth_ldap;

  if (!$_xc_auth_ldap) {
    return;
  }

  $code = xc_auth_ldap_info($_xc_auth_ldap->getOption('sid'), 'login_php');
  if ($code) {
    $login_name = eval($code);
  }
  else {
    $login_name = $user;
  }

  $_xc_auth_ldap->connect($_xc_auth_ldap->getOption('binddn'), $_xc_auth_ldap->getOption('bindpw'));
  foreach (explode("\r\n", $_xc_auth_ldap->getOption('basedn')) as $base_dn) {
    if (empty($base_dn)) {
      continue;
    }

    $user_attr = $_xc_auth_ldap->getOption('user_attr') ? $_xc_auth_ldap->getOption('user_attr') : LDAPAUTH_DEFAULT_USER_ATTR;
    $filter = $user_attr . '=' . $login_name;
    $result = $_xc_auth_ldap->search($base_dn, $filter);
    if (!$result) {
      continue;
    }

    $num_matches = $result['count'];
    if ($num_matches != 1) {
      xc_log_info('ldapauth', "Error: %num_matches users found with $%filter under %base_dn.",
        array(
        '%num_matches' => $num_matches,
        '%filter' => $filter,
        '%base_dn' => $base_dn,
      ), WATCHDOG_ERROR);
      continue;
    }
    $match = $result[0];

    if (!isset($match[$user_attr][0])) {
      $user_attr = drupal_strtolower($user_attr);
      if (!isset($match[$user_attr][0])) {
        continue;
      }
    }

    foreach ($match[$user_attr] as $value) {
      if (drupal_strtolower(trim($value)) == drupal_strtolower($login_name)) {
        return $match;
      }
    }
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_auth_ldap_info($sid, $req) {
  static $servers = array();
  if (!isset($servers[$sid])) {
    $servers[$sid] = db_query("SELECT * FROM {ldapauth} WHERE sid = :sid", array(':sid' => $sid))->fetchObject();
  }

  switch ($req) {
    case 'login_php':
      return $servers[$sid]->login_php;
      break;
    case 'filter_php':
      return $servers[$sid]->filter_php;
      break;
  }
}

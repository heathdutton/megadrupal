<?php

/**
 * @file
 * Functions for retrieving and setting ILS informations of user.
 */

/**
 * Get ILS login.
 *
 * @param object $account
 *   user object, as returned by user_load().
 *
 * @return string
 *   ILS login.
 */
function opac_user_get_login($account) {
  $opac_login = NULL;

  $auth_method = variable_get('opac_users_auth_method', OPAC_USER_AUTH_METHOD_NONE);
  if ($auth_method == OPAC_USER_AUTH_METHOD_DRUPAL) {
    $opac_login = $account->name;
  }
  elseif ($auth_method == OPAC_USER_AUTH_METHOD_LIBRARY) {
    $opac_login_field = variable_get('opac_users_login_field');
    if (!isset($opac_login_field)) {
      drupal_set_message(t("Variable 'opac_users_login_field' is not set.")
        . " " . t("Please configure it in 'Configuration > OPAC > OPAC user settings'."),
        'error');
    }
    else {
      if (isset($account->$opac_login_field) && count($account->$opac_login_field)) {
        $opac_login = $account->{$opac_login_field}['und'][0]['value'];
      }
    }
  }

  return $opac_login;
}

/**
 * Set ILS login.
 *
 * @param object $account
 *   user object, as returned by user_load().
 *
 * @param string $opac_login
 *   new opac login.
 */
function opac_user_set_login($account, $opac_login) {
  $opac_login_field = variable_get('opac_users_login_field');
  if (!isset($opac_login_field)) {
    drupal_set_message(t("Variable 'opac_users_login_field' is not set.")
      . " " . t("Please configure it in 'Configuration > OPAC > OPAC user settings'."),
      'error');
  }
  else {
    if (isset($account->$opac_login_field)) {
      $account->{$opac_login_field}['und'][0]['value'] = $opac_login;
    }
  }
}

/**
 * Get ILS password.
 *
 * @param object $account
 *   user object, as returned by user_load().
 *
 * @return string
 *   ILS password.
 */
function opac_user_get_password($account) {
  $opac_password = NULL;

  $opac_password_field = variable_get('opac_users_password_field');
  if (!isset($opac_password_field)) {
    drupal_set_message(t("Variable 'opac_users_password_field' is not set.")
      . " " . t("Please configure it in 'Configuration > OPAC > OPAC user settings'."),
      'error');
  }

  if (isset($account->$opac_password_field) && count($account->$opac_password_field)) {
    $opac_password = $account->{$opac_password_field}['und'][0]['value'];
  }

  return $opac_password;
}

/**
 * Set ILS password.
 *
 * @param object $account
 *   user object, as returned by user_load().
 *
 * @param string $opac_password
 *   new opac password.
 */
function opac_user_set_password($account, $opac_password) {
  $opac_password_field = variable_get('opac_users_password_field');
  if (!isset($opac_password_field)) {
    drupal_set_message(t("Variable 'opac_users_password_field' is not set.")
      . " " . t("Please configure it in 'Configuration > OPAC > OPAC user settings'."),
      'error');
  }
  else {
    if (isset($account->$opac_password_field)) {
      $account->{$opac_password_field}['und'][0]['value'] = $opac_password;
    }
  }
}

/**
 * Get ILS server identifier.
 *
 * @param object $account
 *   user object, as returned by user_load().
 *
 * @return string
 *   ILS server identifier.
 */
function opac_user_get_server($account) {
  $opac_server = NULL;

  $opac_server_field = variable_get('opac_users_server_field');
  if (!isset($opac_server_field)) {
    drupal_set_message(t("Variable 'opac_users_server_field' is not set.")
      . " " . t("Please configure it in 'Configuration > OPAC > OPAC user settings'."),
      'error');
  }

  if (isset($account->$opac_server_field) && count($account->$opac_server_field)) {
    $opac_server = $account->{$opac_server_field}['und'][0]['value'];
  }

  return $opac_server;
}

/**
 * Set ILS server.
 *
 * @param object $account
 *   user object, as returned by user_load().
 *
 * @param string $opac_server
 *   new opac server.
 */
function opac_user_set_server($account, $opac_server) {
  $opac_server_field = variable_get('opac_users_server_field');
  if (!isset($opac_server_field)) {
    drupal_set_message(t("Variable 'opac_users_server_field' is not set.")
      . " " . t("Please configure it in 'Configuration > OPAC > OPAC user settings'."),
      'error');
  }
  else {
    if (isset($account->$opac_server_field)) {
      $account->{$opac_server_field}['und'][0]['value'] = $opac_server;
    }
  }
}

/**
 * Get user informations from ILS.
 *
 * @param object $account
 *   User object, as returned by user_load().
 */
function opac_user_get_ils_infos($account) {
  $infos = NULL;
  $server_id = opac_user_get_server($account);
  $server = opac_server_load($server_id);
  $connector = opac_get_instance($server);
  $opac_users_auth_method = variable_get('opac_users_auth_method');
  $login = opac_user_get_login($account);
  if (isset($login)) {
    $id = $connector->lookupUser($login);
    $infos = $connector->patronInfo($id, array_keys($connector->patronFields()));
  }
  return $infos;
}

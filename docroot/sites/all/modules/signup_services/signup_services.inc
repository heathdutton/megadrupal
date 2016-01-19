<?php

/**
 * @file
 * Signup services resources provides services' resources for Signup services
 * module. (http://drupal.org/project/signup).
 */

/**
 * Access callbacks default is check the user_login.
 *
 * @param Array $data
 *
 * @return Boolean user_is_logged_in
 */
function _signup_services_total_access($data) {
  return user_is_logged_in();
}

/**
 * Access callbacks default is check the user_load_signup.
 *
 * @param Array $data
 *
 * @return Boolean user_is_logged_in
 */
function _signup_services_user_load_signup_access($data) {
  return user_is_logged_in();
}

/**
 * Access callbacks default is check the signup_user.
 *
 * @param Array $data
 *
 * @return Boolean user_is_logged_in
 */
function _signup_services_signup_user_access($data) {
  return user_is_logged_in();
}

// function _signup_services_cancel_node_signups_access($data) {
//   return user_is_logged_in();
// }
// function _signup_services_cancel_user_signups_access($data) {
//   return user_is_logged_in();
// }

/**
 * Action: Load signup total.
 *
 * @param Array $data
 *   An array with nid.
 *
 * @return Int $count
 *   An int of the total signup of this node.
 */
function signup_services_total($data) {

  $query = db_select('signup_log');
  $query->condition('nid', $data['nid']);
  $count = $query->countQuery()->execute()->fetchField();

  return $count;
}

/**
 * Action: Load signup data.
 */
function signup_services_user_load_signup($data) {

  $query = db_select('signup_log', 'sl');
  $query->condition('uid', $data['uid']);
  $query->fields('sl', array('nid', 'signup_time', 'attended'));
  $signup = $query->execute()->fetchAll();

  if (!empty($signup)) {
    foreach ($signup as $row) {
      $query = db_select('signup_log');
      $query->condition('nid', $row->nid);
      $count = $query->countQuery()->execute()->fetchField();
      $row->total = $count;

      if (empty($row->attended)) {
        $row->attended = "";
      }
    }

    return array('signup_loaded' => $signup);
  }
  return services_error(t('No node has been signuped by the user'), 404);
}

/**
 * Action: Sign up a user to content.
 */
function signup_services_signup_user($data) {

  $query = db_select('signup_log', 'sl');
  $query->condition('uid', $data['uid']);
  $query->condition('nid', $data['nid']);
  $query->fields('sl', array('uid'));
  $result = $query->execute()->fetchObject();

  // Check the node signup status before signing up the user.
  if (empty($result)) {

    if (!node_load($data['nid'])) {
      return services_error(t('This node does not exist'), 404);
    }
    elseif (!user_load($data['uid'])) {
      return services_error(t('This user does not exist'), 404);
    }

    signup_sign_up_user($data);

    $query = db_select('signup_log');
    $query->condition('nid', $data['nid']);
    $count = $query->countQuery()->execute()->fetchField();

    return $count;
  }
  else {
    services_error(t('This user has already signed up to this node'), 404);
  }
}

/**
 * Action: Cancel all signups for a node.
 */
function signup_services_cancel_node_signups($data) {
  $signups = signup_get_signups($data['nid']);
  foreach ($signups as $signup) {
    signup_cancel_signup($signup, FALSE);
  }
  return TRUE;
}

/**
 * Action: Cancel all signups for a user.
 */
function signup_services_cancel_user_signups($data) {
  $signups = signup_rules_get_user_signups($data['uid']);
  foreach ($signups as $signup) {
    signup_cancel_signup($signup, FALSE);
  }
  return TRUE;
}


<?php

define('CACHE_BYPASS_COOKIE', 'vcache-bypass-token');

$plugin = array(
    'name' => 'Cache bypass',
    'module' => 'steroids',
    'admin_settings_form' => 'steroids_cache_bypass_admin_settings_form',
    'hook_permission' => 'steroids_cache_bypass_permission',
    'hook_init' => 'steroids_cache_bypass_init',
    'hook_user_login' => 'steroids_cache_bypass_user_login',
    'hook_user_logout' => 'steroids_cache_bypass_user_logout',
);

function steroids_cache_bypass_admin_settings_form() {
  $settings = variable_get('steroids_cache_bypass', array());

  $form['steroids_cache_bypass'] = array(
      '#type' => 'fieldset',
      '#title' => t('Cache bypass'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
  );


  $roles = user_roles(TRUE);
  $form['steroids_cache_bypass']['roles'] = array(
      '#type' => 'fieldset',
      '#title' => t('Role configuration'),
      '#collapsible' => TRUE,
  );

  foreach ($roles as $rid => $role) {

    $form['steroids_cache_bypass']['roles'][$rid]['enabled'] = array(
        '#type' => 'checkbox',
        '#title' => t('Role: @role', array('@role' => $role)),
        '#default_value' => (isset($settings['roles'][$rid]['enabled'])) ? $settings['roles'][$rid]['enabled'] : 0,
    );
    $form['steroids_cache_bypass']['roles'][$rid]['token'] = array(
        '#type' => 'textfield',
        '#title' => t('Token: @role', array('@role' => $role)),
        '#default_value' => (isset($settings['roles'][$rid]['token'])) ? $settings['roles'][$rid]['token'] : '',
        '#description' => t('Token is the string that willl be used as cookie value. This token has to be configured in your Varnish configuration file (VCL).'),
        '#states' => array(
            'visible' => array(
                ':input[name="steroids_cache_bypass[roles][' . $rid . '][enabled]"]' => array('checked' => TRUE),
            ),
        ),
        '#required' => FALSE,
    );
  }

  $form['steroids_cache_bypass']['tamper_detection'] = array(
      '#type' => 'checkbox',
      '#title' => t('Tamper detection'),
      '#default_value' => (isset($settings['tamper_detection'])) ? $settings['tamper_detection'] : 0,
      '#description' => t('When the user tampers with the varnish cookie should we detect and restore it. Enabling detection has a performance impact.'),
  );
  return $form;
}

/**
 * Delegation of hook_init().
 */
function steroids_cache_bypass_init() {
  $settings = variable_get('steroids_cache_bypass', array());

  // Only if tamper detection is enabled.
  if (isset($settings['tamper_detection']) && $settings['tamper_detection']) {
    global $user;

    if ($user->uid) {
      // Get token for current user.
      // Current token
      $current_bypass_token = isset($_COOKIE[CACHE_BYPASS_COOKIE]) ? $_COOKIE[CACHE_BYPASS_COOKIE] : FALSE;

      // Token has been found.
      if ($current_bypass_token) {
        $match_found = FALSE;
        // Check if token matches.
        foreach ($settings['roles'] as $rid => $value) {
          if ($value['token'] === $current_bypass_token) {
            $match_found = TRUE;
            break;
          }
        }

        // No tampering detected.
        if ($match_found) {
          return;
        }

        // Tamper detected.
        // Assign new cookie.
        steroids_cache_bypass_set_cookie();
      }

      // No token found
      // Check if user needs to have a token set.
      if (steroids_cache_bypass_check_user()) {
        steroids_cache_bypass_set_cookie();
      }

    } else {
      steroids_cache_bypass_destroy_cookie();
    }
  }
}

/**
 * Delegation of hook_user_login().
 */
function steroids_cache_bypass_user_login(&$edit, $account) {
  steroids_cache_bypass_set_cookie($account);
}

/**
 * Delegation of hook_logout().
 */
function steroids_cache_bypass_user_logout($account) {
  steroids_cache_bypass_destroy_cookie();
}

/**
 * Set the bypass cookie.
 */
function steroids_cache_bypass_set_cookie(&$account) {
  $token = steroids_cache_bypass_user_token($account);
  if ($token) {
    drupal_add_http_header('X-Varnish-Cookie', 'Allow');
    setcookie(CACHE_BYPASS_COOKIE, $token, time() + ini_get('session.cookie_lifetime'), '/');
  } else {
    steroids_cache_bypass_destroy_cookie();
  }
}

/**
 * Destroy the bypass cookie.
 */
function steroids_cache_bypass_destroy_cookie() {
  drupal_add_http_header('X-Varnish-Cookie', 'Allow');
  setcookie(CACHE_BYPASS_COOKIE, "", time() - 60 * 60 * 24 * 100, '/');
}

/**
 * Retreive the token given the users role.
 */
function steroids_cache_bypass_user_token(&$account) {
  $settings = variable_get('steroids_cache_bypass', array());
  $rids = array_keys($account->roles);

  // hardcoded uid == 1
  if ($account->uid == 1) {
    $result = db_select('role', 'r')
            ->fields('r', array('rid', 'weight'))
            ->condition('name', 'administrator')
            ->orderBy('weight', 'DESC')
            ->execute();
  }
  else if (!empty($rids)) {
    $result = db_select('role', 'r')
            ->fields('r', array('rid', 'weight'))
            ->condition('rid', $rids, 'IN')
            ->orderBy('weight', 'DESC')
            ->execute();
  }

  // Find the first configured varnish token role.
  foreach ($result as $rid => $role) {
    if ($settings['roles'][$role->rid]['enabled']) {
      return $settings['roles'][$role->rid]['token'];
    }
  }

  return FALSE;
}

/**
 * Check if the current user needs to have token set.
 */
function steroids_cache_bypass_check_user() {
  global $user;
  $settings = variable_get('steroids_cache_bypass', array());
  foreach ($user->roles as $rid => $role) {
    if (isset($settings['roles'][$rid]) && $settings['roles'][$rid]['enabled']) {
      return TRUE;
    }
  }
  return FALSE;
}

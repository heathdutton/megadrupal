<?php

/**
 * Add user to known impersonate users.
 *
 * @param int $uid
 */
function devcontrol_user_cookie_add($uid) {
  $list = devcontrol_user_cookie_get();
  if (!in_array($uid, $list)) {
    $list[] = $uid;
    devcontrol_cookie_set(DEVCONTROL_IMPERSONATE_COOKIE, implode(',', $list));
  }
}

/**
 * Remove user from known impersonate users.
 *
 * @param int $uid
 */
function devcontrol_user_cookie_del($uid) {
  if (!in_array($uid, $list)) {
    if (empty($list)) {
      devcontrol_cookie_drop(DEVCONTROL_IMPERSONATE_COOKIE);
    } else {
      devcontrol_cookie_set(DEVCONTROL_IMPERSONATE_COOKIE, implode(',', $list));
    }
  }
}

/**
 * Get known impersonate user list.
 *
 * @return string[]
 */
function devcontrol_user_cookie_get() {
  if (isset($_COOKIE[DEVCONTROL_IMPERSONATE_COOKIE])) {
    return explode(',', $_COOKIE[DEVCONTROL_IMPERSONATE_COOKIE]);
  } else {
    return array();
  }
}

/**
 * Autocomplete users.
 */
function devcontrol_user_automcomplete($string) {

  $esc = '%' . db_like($string) . '%';

  $list = db_select('users', 'u')
    ->fields('u', array('uid', 'name'))
    ->condition(
       db_or()
        ->condition('u.name', $esc, 'LIKE')
        ->condition('u.mail', $esc, 'LIKE')
    )
    ->range(0, 16)
    ->execute()
    ->fetchAllKeyed();

  $ret = array();

  foreach ($list as $uid => $name) {
    $ret[$name] = check_plain($name) . ' (' . $uid . ')';
  }

  if (16 === count($ret)) {
    $ret[] = '...';
  }

  drupal_json_output($ret);
  drupal_exit();
}

/**
 * Impersonate form.
 */
function devcontrol_user_impersonate_form($form, &$form_state) {

  global $user;

  $form['account'] = array(
    '#type'              => 'textfield',
    '#autocomplete_path' => 'developer/user/autocomplete',
    '#title'             => t("Type in a user identifier or name"),
    '#title_display'     => 'invisible',
    '#size'              => 20,
  );
  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t("Go"),
  );

  $opts  = array('query' => array('destination' => current_path()));
  $links = array();
  $list  = devcontrol_user_cookie_get();

  if (!empty($list)) {
    $names = db_query("SELECT uid, name FROM {users} WHERE uid IN (:uid)", array(':uid' => $list))->fetchAllKeyed();
    foreach ($names as $uid => $name) {
      $links[] =  l(check_plain($name), 'developer/user/impersonate/' . $uid, $opts);
    }
  }

  if (($uid = devcontrol_access_has_cookie()) && ($user->uid != $uid)) {
    $links[] = l('<strong>' . t("Back to self!") . '</strong>', 'developer/user/relog', $opts + array('html' => true));
  }

  // Append list of known users.
  if (!empty($links)) {
    $form['list'] = array(
      '#prefix' => '<ul>',
      '#suffix' => '</ul>',
    );
    foreach ($links as $link) {
      $form['list'][] = array(
        '#markup' => $link,
        '#prefix' => '<li>',
        '#suffix' => '</li>',
      );
    }
  }

  return $form;
}

/**
 * Impersonate form submit.
 */
function devcontrol_user_impersonate_form_submit($form, &$form_state) {

  global $user;

  if ((!$account = user_load($form_state['values']['account'])) || !$account->uid) {
    if ((!$account = user_load_by_name($form_state['values']['account'])) || !$account->uid) {
      drupal_set_message(t("Account does not seem to exist"), 'error');
    }
  }

  if (isset($account) && $account->uid) {

    if (!devcontrol_access_has_cookie()) {
      drupal_set_message(t("You have been given a cookie!"));
      devcontrol_access_give_cookie();
    }

    // Add account to known list.
    devcontrol_user_cookie_add($account->uid);

    $user = $account;
    drupal_session_regenerate();
  }
}

/**
 * Impersonate a user page.
 */
function devcontrol_user_impersonate_page($uid) {
  global $user;

  if ($account = user_load($uid)) {
    if (!$account->status) {
      drupal_set_message(t("User account is blocked"), 'error');
    } else {
      if ($user->uid == $account->uid) {
        drupal_set_message(t("You are already impersonating this account"), 'error');
      } else {

        if (!devcontrol_access_has_cookie()) {
          drupal_set_message(t("You have been given a cookie!"));
          devcontrol_access_give_cookie();
        }

        $user = $account;
        drupal_session_regenerate();
      }
    }
  }

  if (isset($_GET['destination']) && false === strpos($_GET['destination'], 'impersonate')) {
    $target = $_GET['destination'];
  } else {
    $target = url('<front>');
  }

  drupal_goto($target);
}

/**
 * Relog page.
 */
function devcontrol_user_relog_page() {

  global $user;

  if ($uid = devcontrol_access_has_cookie()) {
    $user = user_load($uid);
    drupal_session_regenerate();
  }


  if (isset($_GET['destination']) && false === strpos($_GET['destination'], 'relog')) {
    $target = $_GET['destination'];
  } else {
    $target = url('<front>');
  }

  drupal_goto($target);
}

/**
 * Drop cookie page.
 */
function devcontrol_user_dropcookie_page() {

  if (devcontrol_access_has_cookie()) {
    devcontrol_cookie_drop(DEVCONTROL_COOKIE_NAME);
  }

  if (isset($_GET['destination']) && false === strpos($_GET['destination'], 'dropcookie')) {
    $target = $_GET['destination'];
  } else {
    $target = url('<front>');
  }

  drupal_goto();
}

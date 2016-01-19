<?php

/**
 * Chat Page
 */
function parachat_page() {
  global $user;

  // get parachat settings
  $data = variable_get('parachat_settings', NULL);
  $data = isset($data) ? unserialize($data) : array();
  // display error and no chat room if they have not configured parachat yet
  if (!$data) {
    drupal_set_message(t('Parachat has not been configured yet'), 'error');
    return '';
  }

  // set username
  $data['username'] = $user->name;

  // create pass hash or send false if external auth isn't setup
  if (isset($data['auth']) && $data['auth'] == 1) {
    $query = db_select('users', 'u');
    $query->fields('u', array('name', 'pass'));
    $query->condition('u.uid', $user->uid);
    $result = $query->execute()->fetchObject();
    $data['pass'] = $user->uid .'-'. md5($result->name . $result->pass);
  }

  // option for admin to set custom name
  if (user_access('administer chat')) {
    if (isset($_GET['user'])) {
      $data['username'] = $_GET['user'];
    }
  }

  // set theme
  $output = theme('parachat', $data);

  return $output;
}

/**
 * Page that parachat calls to identify the user
 */
function parachat_auth_page() {

  header('content-type: text/plain');

  // if auth is turned off, just return result succes in case they left external db on in the parachat admin
  $data = variable_get('parachat_settings', false);
  if ($data) {
    $data = unserialize($data);
    if (!$data['auth']) {
      echo 'Result=Success';
      exit;
    }
  }

  $input = $_GET['pass'];
  $uid = substr($input, 0, strpos($input, '-'));

  $result = db_fetch_object(db_query("SELECT name, pass FROM {users} WHERE uid=%d", $uid));
  $hash = $uid .'-'. md5($result->name . $result->pass);



  if ($hash == $input) {
    echo 'Result=Success';
    exit;
  }

  echo 'Result=Error';
  exit;
}

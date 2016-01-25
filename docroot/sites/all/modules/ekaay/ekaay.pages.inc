<?php

/**
 * @file
 * Provides different page callbacks for the different pages which are called.
 */

/**
 * Check the reverse token.
 *
 * Returns 'ok' i3f the value is valid, else return notok + 403.
 */
function ekaay_check_reverse_token() {
  $name = $_GET['user'];
  $revtoken = $_GET['revtoken'];
  list($expected_token, $token_time) = ekaay_get_rev_info_by_name($name);
  if ($revtoken && $revtoken == $expected_token && time() <= ($token_time + 5 * 60)) {
    print 'ok';
    exit;
  }
  else {
    print 'notok';
    drupal_access_denied();
    exit;
  }
}

/**
 * Displays the registration process iframe.
 */
function ekaay_account_view($account) {
  $element = array();
  $element['ekaay'] = array(
    '#theme' => 'ekaay_register',
    '#user' => $account,
  );

  return $element;
}

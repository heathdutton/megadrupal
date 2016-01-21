<?php
/**
 * @file
 * Drush integration for gate.module.
 */

/**
 * Implements hook_drush_command().
 */
function gate_drush_command() {
  $openclose_arguments = _gate_drush_openclose_arguments();
  $openclose_argument_string = implode(array_keys($openclose_arguments), ', ');
  $items = array();

  $items['gate-status'] = array(
    'description' => dt('Show status of Gate variables.'),
    'aliases' => array('gate-st'),
  );
  $items['gate-close'] = array(
    'description' => dt('Close the specified Gate (overview with "gate-status").'),
    'arguments' => array('command' => $openclose_argument_string),
    'aliases' => array('gate-c'),
    'arguments' => $openclose_arguments,
  );
  $items['gate-open'] = array(
    'description' => dt('Open the specified Gate (overview with "gate-status").'),
    'arguments' => array('command' => $openclose_argument_string),
    'aliases' => array('gate-o'),
    'arguments' => $openclose_arguments,
  );
  $items['gate-user-login'] = array(
    'callback' => 'drush_gate_user_login',
    'description' => 'Extend stanard drush user-link (uli) with a token to bypass gate login blocker on hook_user_login',
    'aliases' => array('gate-uli'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'handle-remote-commands' => TRUE,
    'arguments' => array(
      'user' => 'An optional uid, user name, or email address for the user to log in as. Default is to log in as uid 1. The uid/name/mail options take priority if specified.',
      'path' => 'Optional path to redirect to after logging in.',
    ),
  );

  return $items;
}

/**
 * Drush command callback: gate-status.
 */
function drush_gate_user_login($uid = NULL, $path = NULL) {
  $loginpath = drush_user_login($uid, $path);
  if (!isset($uid)) {
    $uid = 1;
  }
  if ($uid == 0) {
    return;
  }

  $gate_login = _gate_login_init($uid);
  $gate_login->access = 1;
  _gate_login_db($gate_login, 'update');

  return $loginpath . '?gate-token=' . $gate_login->token;
}

/**
 * Drush command callback: gate-status, gate-st.
 */
function drush_gate_status() {
  $arguments = array_keys(_gate_drush_openclose_arguments());
  $string = '';
  foreach ($arguments as $argument) {
    $correct = 25 - strlen($argument);
    while ($correct != 0) {
      $string .= ' ';
      $correct--;
    }
    $variablename = "gate_" . str_replace("-", "_", $argument);
    $current_status = variable_get($variablename, 1);

    if ($current_status == 1) {
      $status = 'open';
    }
    else {
      $status = 'CLOSED';
    }

    $string .= $argument;
    $string .= " => " . $status . "\n";
  }
  echo $string;
}

/**
 * Drush command callback: gate-close, gate-c.
 */
function drush_gate_close($argument = NULL) {
  $arguments = array_keys(_gate_drush_openclose_arguments());
  if ((isset($argument)) && (in_array($argument, $arguments))) {
    _gate_drush_openclose_execute(0, $argument);
  }
  else {
    drush_set_error(dt('Missing or wrong argument for gate command.'));
  }
}

/**
 * Drush command callback: gate-open, gate-o.
 */
function drush_gate_open($argument = NULL) {
  $arguments = array_keys(_gate_drush_openclose_arguments());
  if ((isset($argument)) && (in_array($argument, $arguments))) {
    _gate_drush_openclose_execute(1, $argument);
  }
  else {
    drush_set_error(dt('Missing or wrong argument for gate command.'));
  }
}

/**
 * Drush command callback: gate-onetimelink-adminrole.
 */
function _gate_drush_openclose_execute($op, $argument) {

  $variablename = "gate_" . str_replace("-", "_", $argument);
  $current_status = variable_get($variablename, 1);

  if ($op == 1) {
    $status = 'open';
  }
  else {
    $status = 'CLOSED';
  }
  if ($current_status == $op) {
    drupal_set_message("Nothing changed! " . $argument . " is already on status: " . $status);
  }
  else {
    variable_set($variablename, $op);
    drupal_set_message("Status of " . $argument . " now changed to: " . $status);
  }

}

/**
 * Gate drush command-arguments for gate-close and gate-open.
 */
function _gate_drush_openclose_arguments() {
  return array(
    'onetimelink-user1'     => 'Gate for requesting password-reset/onetime-login link of user 1.',
    'onetimelink-adminrole' => 'Gate for requesting password-reset/onetime-login link on admin role (except user 1).',
    'pwlogin-user1'         => 'Gate for password-login of user 1.',
    'pwlogin-adminrole'     => 'Gate for password-login on admin role (except user 1).',
    'direct-login'          => 'Gate for multi-factor login process.',
    'direct-mail'           => 'Gate for processing mail rules.',
  );
}

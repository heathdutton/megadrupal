<?php
/**
 * @file
 *
 * Simple user authentication example using HTTP Basic Auth and SSL
 *
 */

// Some global settings for all examples are defined here, such as the token.
require_once('enable_examples.inc');

// Set the roles which shall be accepted here
$required_roles = array('administrator');

try {

  // Of cause we do not want the browser to send username and password with the
  // request if we are not on an encrypted channel.
  if (!isset($_SERVER['HTTPS']) || $_SERVER['HTTPS'] != 'on') {
    throw new Exception('Not authorized: No HTTPS');
  }

  // Ask Drupal if the user is valid. If yes, check if the user has the right Role.
  $user_is_authorized = FALSE;
  if (isset($_SERVER['PHP_AUTH_USER']) && trim($_SERVER['PHP_AUTH_USER']) != '') {
    try {
      // Create the user list object
      $dpu = new DrupalUserAuth();

      // Send request (Annotation: You can check login/password as well
      // if the used is blocked in Drupal, to prevent obtaining blocked
      // users, set the 4th argument to TRUE (this is the default as well).
      $dpu->request(
        $_SERVER['PHP_AUTH_USER'],  // login name
        $_SERVER['PHP_AUTH_PW'],    // password
        NULL,                       // email address
        TRUE,                       // active users only (non blocked)
        TRUE                        // include all user fields in the response
      );

      // Check if the login/pass was valid and the user is active
      if ($dpu->valid && $dpu->active) {
        if (empty($roles)) {
          $user_is_authorized = TRUE;
        }
        else {
          foreach ($required_roles as $role) {
            if (in_array($role, $dpu->roles)) {
              $user_is_authorized = TRUE;
              break;
            }
          }
        }
      }
    } catch (Exception $e) {
      // This catch is only for us to debug, we throw an unauthorized again
      print "Exception: \n\n$e\n\n";
      if (isset($dpu)) print "----- DEBUG-----\n\n" . $dpu->getDebug() . "\n\n";
      throw new Exception('Not authorized');
    }
  }

  // This is only for us now to force everytime a new login
  if ($user_is_authorized) {
    @session_start();
    if (!isset($_SESSION['renew'])) {
      $_SESSION['renew'] = 1;
    }
    else {
      unset($_SESSION['renew']);
      $user_is_authorized = FALSE;
    }
  }

  // Not authorized (yet?) --> Send in the header that a basic auth is required
  // and get the hell out of here.
  if (!$user_is_authorized) {
    //header('WWW-Authenticate: Basic realm="Login required"');
    // We always change the relam so that browsers always show the auth.
    // popup again ... this is just for us to test now.
    header('WWW-Authenticate: Basic realm="Login required - ' . time() . '"');
    header('HTTP/1.0 401 Unauthorized');
    throw new Exception('Not authorized');
  }
} catch (Exception $e) {
    // We abort the script here with the exception message text
    die($e->getMessage());
}

// Display resulte
header('Content-Type: text/html; charset=utf-8');
?>
<html><head>
    <title>[User Data Connector - Auth example] You are authorized</title>
    <style>
        body { font-family: monospace; }
    </style>
</head><body>
    <h1>Hello <?= $dpu->name ?></h1>
    You are indeed a user here. Here some information fetched:<br/>
    <ul>
        <li>Your email: <?= $dpu->mail ?></li>
        <li>Roles:<ul><? foreach($dpu->roles as $role) print "<li><i>$role</i></li>"; ?></ul></li>
        <li>Fields:<ul><? foreach($dpu->fields as $fieldname => $fieldvalue) print "<li><i>$fieldname</i>: <b>$fieldvalue</b></li>"; ?></ul></li>
    </ul>
    <br/>
</body></html>

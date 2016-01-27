<?php
/**
 * @file
 *
 * User info class example
 *
 */

// Some global settings for all examples are defined here, such as the token.
require_once('enable_examples.inc');

try {

  // Check if the script is invoked like userinfo.php?user=<USER NAME>
  if (!isset($_GET['user'])) {
    throw new Exception("USAGE: {$_SERVER['PHP_SELF']}?user=username");
  }

  // Create the user list object
  $dpu = new DrupalUserInfo();

  // Send the request
  $dpu->request(
    trim($_GET['user']),    // login name
    NULL,                   // email address
    TRUE,                   // active users only (non blocked)
    TRUE                    // include all user fields in the response
  );
} catch (Exception $e) {
  header('Content-Type: text/plain');
  die($e->getMessage() . "\n\n" . (isset($dpu) ? $dpu->getDebug() : ''));
}

// Display resulte
header('Content-Type: text/html; charset=utf-8');
?>
<html><head>
    <title>[User Data Connector - Info example]</title>
    <style>
        body { font-family: monospace; }
    </style>
</head><body>
    <? if (!$dpu->valid) { ?>
        <h3>Sorry, there is no active user named "<? print $user; ?>"</h3>
    <? } else { ?>
    <h3>Here some information about <? print $dpu->name; ?></h3>
    <ul>
        <li>Email: <?php print $dpu->mail; ?></li>
        <li>Roles: <?php print implode(', ', $dpu->roles); ?></li>
        <li>Fields:<ul><?php foreach ($dpu->fields as $fieldname => $fieldvalue) print "<li><i>$fieldname</i>: <b>$fieldvalue</b></li>"; ?></ul></li>
    </ul>
    <? } ?>
</body></html>

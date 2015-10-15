<?php
/**
 * @file
 *
 * User list class example
 *
 */

// Some global settings for all examples are defined here, such as the token.
require_once('enable_examples.inc');

try {
  // Create the user list object
  $dpu = new DrupalUserList();

  // Send the request
  $dpu->request(
    FALSE,          // List all users, not only active users
    TRUE            // List with user roles
  );
} catch (Exception $e) {
  header('Content-Type: text/plain');
  die($e->getMessage() . "\n\n" . $dpu->getDebug());
}

// Display resulte
header('Content-Type: text/html; charset=utf-8');
?>
<html><head>
    <title>[User Data Connector - list example]</title>
    <style>
        body { font-family: monospace; }
    </style>
</head><body>
    <h3>A list of active users with roles</h3>
    <ul><?
        foreach ($dpu->list as $user) {
            $active = $user['active'] ? 'active' : 'blocked';
            if (is_array($user['roles'])) $user['roles'] = '; <b>Roles:</b> ' . implode(',', $user['roles']);
            print "<li><b>{$user['name']}</b> <i>({$active}, {$user['mail']})</i> {$user['roles']}</li>";
        }
    ?></ul>

<h4>As <b>print_r</b> representation:</h4>
<pre style="margin-left: 20px; font-size: 10px;"><? print_r($dpu->list); ?></pre>
</body></html>

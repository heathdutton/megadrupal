<?php
/**
 * @file
 *
 * Simple server script ping and token testing
 *
 */

// Some global settings for all examples are defined here, such as the token.
require_once('enable_examples.inc');

try {
  // Create the user list object
  $dpu = new DrupalUserAuth();

  // Send request
  $ping = $dpu->ping();
} catch (Exception $e) {
  header('Content-Type: text/plain');
  die($e->getMessage() . "\n\n" . $dpu->getDebug());
}

// Display resulte
header('Content-Type: text/html; charset=utf-8');
?>
<html><head>
    <title>[User Data Connector - Ping example]</title>
    <style>
        body { font-family: monospace; }
    </style>
</head><body>
    <? if($ping) { ?>
        <h1>Ping successful<? print $dpu->getUseSSL() ? '(with SSL)' : '' ?></h1>
        The server required <? print $dpu->getServerProcessTime(); ?>s for processing the request.
    <? } else { ?>
        <h1>Ping failed</h1>
        The server script responded with "<? print $dpu->getError(); ?>".
    <? } ?>
</body></html>

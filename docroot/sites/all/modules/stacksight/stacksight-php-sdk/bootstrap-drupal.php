<?php 

define('STACKSIGHT_BOOTSTRAPED', TRUE);

require_once('SSClientBase.php');
require_once('SSHttpRequest.php');
require_once('SSLogsTracker.php');
require_once('SSUtilities.php');
require_once('platforms/SSDrupalClient.php');

global $ss_client;
$ss_client = new SSDrupalClient(STACKSIGHT_TOKEN, 'drupal');
$ss_client->initApp(STACKSIGHT_APP_ID);
$handle_errors = FALSE;
$handle_fatal_errors = TRUE;
new SSLogsTracker($ss_client, $handle_errors, $handle_fatal_errors);
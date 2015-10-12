<?php 

require_once('SSClientBase.php');
require_once('SSHttpRequest.php');
require_once('SSLogsTracker.php');
require_once('SSUtilities.php');
require_once('platforms/SSWordpressClient.php');
$ss_client = new SSWordpressClient(STACKSIGHT_TOKEN, 'wordpress');
$ss_client->initApp(STACKSIGHT_APP_ID);
new SSLogsTracker($ss_client);
define('STACKSIGHT_BOOTSTRAPED', TRUE);
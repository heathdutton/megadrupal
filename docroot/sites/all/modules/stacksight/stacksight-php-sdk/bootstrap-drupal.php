<?php 

require_once('SSClientBase.php');
require_once('SSHttpRequest.php');
require_once('requests/SSHttpInterface.php');
require_once('requests/SSHttpRequestCurl.php');
require_once('requests/SSHttpRequestMultiCurl.php');
require_once('requests/SSHttpRequestSockets.php');
require_once('requests/SSHttpRequestThread.php');
require_once('SSLogsTracker.php');
require_once('SSUtilities.php');
require_once('platforms/SSDrupalClient.php');

global $ss_client;
if(defined('STACKSIGHT_TOKEN')){
    if(defined('STACKSIGHT_APP_ID'))
        $ss_client = new SSDrupalClient(STACKSIGHT_TOKEN, SSClientBase::PLATFORM_DRUPAL, STACKSIGHT_APP_ID);
    else
        $ss_client = new SSDrupalClient(STACKSIGHT_TOKEN, SSClientBase::PLATFORM_DRUPAL);
    $handle_errors = FALSE;
    $handle_fatal_errors = TRUE;
    if(!defined('STACKSIGHT_INCLUDE_LOGS') || (defined('STACKSIGHT_INCLUDE_LOGS') && STACKSIGHT_INCLUDE_LOGS === true)){
        new SSLogsTracker($ss_client, $handle_errors, $handle_fatal_errors);
    }
    define('STACKSIGHT_BOOTSTRAPED', TRUE);
}
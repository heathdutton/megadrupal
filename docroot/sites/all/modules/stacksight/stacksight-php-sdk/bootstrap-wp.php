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
require_once('platforms/SSWordpressClient.php');

if ( ! function_exists( 'get_plugins' ) ) {
	require_once ABSPATH . 'wp-admin/includes/plugin.php';
}

if(defined('STACKSIGHT_TOKEN')){
	$app_id = (defined('STACKSIGHT_APP_ID')) ? STACKSIGHT_APP_ID : false;
	$group = (defined('STACKSIGHT_GROUP')) ? STACKSIGHT_GROUP : false;
	$ss_client = new SSWordpressClient(STACKSIGHT_TOKEN, SSClientBase::PLATFORM_WORDPRESS, $app_id, $group);
	if(!defined('STACKSIGHT_INCLUDE_LOGS') || (defined('STACKSIGHT_INCLUDE_LOGS') && STACKSIGHT_INCLUDE_LOGS === true)) {
		new SSLogsTracker($ss_client);
	}
	define('STACKSIGHT_BOOTSTRAPED', TRUE);
}


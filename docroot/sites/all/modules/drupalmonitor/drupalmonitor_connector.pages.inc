<?php
/**
 * @file
 *
 */

/**
 * Implements drupalmonitor output.
 */
function drupalmonitor_connector_page_data() {
  timer_start('drupalmonitor');
  $hash_request = '';

  if (!empty($_GET['hash'])) {
    $hash_request = $_GET['hash'];
  }
  else {
    drupal_deliver_page(MENU_ACCESS_DENIED);
  }

  $hash_site = variable_get('drupalmonitor_hash');
  if (empty($hash_site)) {
    header('HTTP/1.1 403 Forbidden');
    $info['drupalmonitor_status'] = "NO SECURITY HASH SET ON WEBSITE";
  }
  elseif ($hash_site == $hash_request) {

  	// Output versions.
    $info['drupalmonitor_version'] = DRUPALMONITOR_VERSION;
    $info['drupalversion'] = VERSION;
    $info['drupalcore'] = DRUPAL_CORE_COMPATIBILITY;

  	// Output allowed data
  	$allowed = '';
  	if (variable_get('drupalmonitor_files_monitoring')) {
  		$allowed[] = 'files';
  	}
  	if (variable_get('drupalmonitor_server_monitoring')) {
  		$allowed[] = 'server';
  	}
  	if (variable_get('drupalmonitor_user_monitoring')) {
  		$allowed[] = 'user';
  	}
  	if (variable_get('drupalmonitor_performance_monitoring')) {
  		$allowed[] = 'performance';
  	}
  	if (variable_get('drupalmonitor_node_monitoring')) {
  		$allowed[] = 'nodes';
  	}
  	if (variable_get('drupalmonitor_modules_monitoring')) {
  		$allowed[] = 'modules';
  	}
  	if (variable_get('drupalmonitor_variables_monitoring')) {
  		$allowed[] = 'variables';
  	}
  	if (variable_get('drupalmonitor_custom_monitoring')) {
  		$allowed[] = 'custom';
  	}
    $info['drupalmonitor_allowed'] = 'data=' . implode(',', $allowed);
	
    require_once dirname(__FILE__) . '/drupalmonitor_connector.user.inc';
    require_once dirname(__FILE__) . '/drupalmonitor_connector.files.inc';
    require_once dirname(__FILE__) . '/drupalmonitor_connector.load.inc';
    require_once dirname(__FILE__) . '/drupalmonitor_connector.status.inc';
    require_once dirname(__FILE__) . '/drupalmonitor_connector.node.inc';
    require_once dirname(__FILE__) . '/drupalmonitor_connector.server.inc';

    // Set correct headers.
    drupal_page_header('Pragma: no-cache');
    drupal_page_header('Expires: 0');
    drupal_page_header('Cache-control: no-cache');

	if (!empty($_GET['data'])) {
	    $data_endpoints = $_GET['data'];
	} else {
		$data_endpoints = '';
	}

    // Server metrics.
    if (preg_match('/server/', $data_endpoints) && variable_get('drupalmonitor_server_monitoring') == 1) {
      $info['server'] = drupalmonitor_connector_get_serverdata();
    }

    // User user count data.
    if (preg_match('/user/', $data_endpoints) && variable_get('drupalmonitor_user_monitoring') == 1) {
      $info['user']['user_usercount'] = drupalmonitor_connector_get_user_usercount();
      $info['user']['user_activesessioncount_300s'] = drupalmonitor_connector_get_user_activesessioncount_300s();
      $info['user']['user_loggedinsessioncount_300s'] = drupalmonitor_connector_get_user_loggedinsessioncount_300s();
    }

    // Files files count data.
    if (preg_match('/files/', $data_endpoints) && variable_get('drupalmonitor_files_monitoring') == 1) {
      $info['files']['files_filescount'] = drupalmonitor_connector_get_files_filescount();
      $info['files']['files_filesfoldersize'] = drupalmonitor_connector_get_files_filesfoldersize();
    }

    // Load request data.
    if (preg_match('/performance/', $data_endpoints) && variable_get('drupalmonitor_performance_monitoring') == 1) {
      $info['performance'] = drupalmonitor_connector_get_loaddata();
    }

    // Node content types data.
    if (preg_match('/nodes/', $data_endpoints) && variable_get('drupalmonitor_node_monitoring') == 1) {
      $info['nodes']['drupalmonitor_node_contenttypes'] = drupalmonitor_connector_node_contenttypes();
    }

    // Drupal status.
    if (preg_match('/modules/', $data_endpoints) && variable_get('drupalmonitor_modules_monitoring') == 1) {
      $info['tbl_system'] = drupalmonitor_connector_status_listmodules();
    }

    if (preg_match('/variables/', $data_endpoints) && variable_get('drupalmonitor_variables_monitoring') == 1) {
      $info['tbl_variable'] = drupalmonitor_connector_status_listvars();
    }

    // Call hook_drupalmonitor().
    if (preg_match('/custom/', $data_endpoints) && variable_get('drupalmonitor_custom_monitoring') == 1) {
      $info['custom'] = module_invoke_all('drupalmonitor');
    }

    // Drupalmonitor infos.
    $info['drupalmonitor_executiontime'] = timer_read('drupalmonitor');
    $info['drupalmonitor_status'] = "OK";

    drupal_json_output($info);
    drupal_exit();
  }
  else {
    drupal_deliver_page(MENU_ACCESS_DENIED, 'drupalmonitor_connector_noaccess');
  }
}

/**
 * Returns correct headers if no access.
 */
function drupalmonitor_connector_noaccess() {
  header('HTTP/1.1 403 Forbidden');
  $info['drupalmonitor_status'] = "NO ACCESS";
  drupal_json_output($info);
  drupal_exit();
}

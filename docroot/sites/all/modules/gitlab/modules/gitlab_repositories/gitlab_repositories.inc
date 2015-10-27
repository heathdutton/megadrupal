<?php

/**
 * @file
 * Callback functions
 */

/**
 * Process request result
 */
function _gitlab_repositories_process_result($data, $processor) {
  $ret = array();
  $processor->addData($data);
  $error = $processor->getError();
  if ($error) {
    $error_keys = array_keys($error);
    watchdog('gitlab_repositories', gitlab_request_error(array_pop($error_keys)), array(), WATCHDOG_NOTICE);
    $ret['error'] = $error;
  } else {
    $data = $processor->getData();
    if (!empty($data)) {
      $ret['data'] = $data;
    }
  }
  return $ret;
}

//========== Repository Files ==========//

/**
 * Repository file API callback
 *
 * @param string $method
 *  Method type. Valid values are 'read', 'create', 'update' and 'delete'.
 * @param string $file_path
 *  Full path to new file. Ex. lib/class.rb
 * @param string $pid
 *  Project ID
 * @param string $private_token
 *  The user's private token for connecting to the api endpoint
 * @param array $options [optional]
 *  List of extra options
 * @return array
 *  Returns the file contents, FALSE otherwise.
 */
function _gitlab_repositories_file($method = 'read', $file_data, $pid, $private_token, $options = array()) {
  // Load the wsconfig
  $wsconfig = wsconfig_load_by_name('gitlab_repositories_files');

  // Apply options
  gitlab_set_wsconfig_options($wsconfig, $options);

  $processor = new gitlabWsdataProcessor();

  // Check for a valid connector
  if ($wsconfig->connector) {
    $data = $wsconfig->call(
      $method,
      array('%id' => $pid),
      drupal_json_encode($file_data),
      gitlab_set_wsconfig_call_options($private_token, $options)
    );
    if (FALSE !== $data) {
      return _gitlab_repositories_process_result($data, $processor);
    }
    else {
      watchdog('gitlab_repositories', 'Repository file @method failed for project id: @id', array('@method' => $method, '@id' => $pid), WATCHDOG_ERROR);
    }
  }
}
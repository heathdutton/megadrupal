<?php

/**
 * @file
 * Defines constants and functions needed by traceview.module, traceview.install
 * and traceview.drush.inc.
 */

// These constants are defined here because traceview.module is not included
// during install until after hook_requirements passes and they are used in
// traceview_requirements().

// The latest tested and confirmed working version of the oboe PHP extension.
define('TRACEVIEW_LATEST_CONFIRMED_VERSION', '1.4.5');

// Versions that are known to have problems and are not recommended.
define('TRACEVIEW_BAD_VERSIONS', serialize(array('1.4.2', '1.4.4')));

/**
 * Sets the Partition, Controller and Action trace attributes.
 */
function traceview_set_attributes() {
  global $user;

  // Partition traffic.
  if (variable_get('traceview_partition_traffic', FALSE)) {
    $traceview_partition = traceview_set_partition();
    if (!empty($traceview_partition)) {
      traceview_oboe_log(NULL, 'info', array('Partition' => $traceview_partition));
    }
    else {
      switch ($user->uid) {
        case 1:
          traceview_oboe_log(NULL, 'info', array('Partition' => 'Admin'));
          break;

        case 0:
          traceview_oboe_log(NULL, 'info', array('Partition' => 'Anonymous'));
          break;

        default:
          traceview_oboe_log(NULL, 'info', array('Partition' => 'Authenticated'));
          break;
      }
    }
  }

  // Track active menu item.
  if (variable_get('traceview_track_menu', FALSE)) {
    // Report the menu entry if we can determine the proper one.
    if (function_exists('menu_get_item')  && $router_item = menu_get_item()) {
      $controller = array(
          'Controller' => $router_item['page_callback'],
          'Action' => (isset($router_item['page_arguments'][0]) ? traceview_get_action($router_item['page_arguments'][0]) : 'NULL'),
      );
    }
    // Else, report if the page is retrieved from cache.
    elseif (drupal_page_get_cache(TRUE)) {
      $controller = array(
          'Controller' => 'drupal_page_cache',
          'Action' => 'NULL',
      );
    }
    // Otherwise, report the bootstrap phase reached.
    else {
      $phase_names = array(
          DRUPAL_BOOTSTRAP_CONFIGURATION => "configuration",
          DRUPAL_BOOTSTRAP_PAGE_CACHE => "page_cache",
          DRUPAL_BOOTSTRAP_DATABASE => "database",
          DRUPAL_BOOTSTRAP_VARIABLES => "variables",
          DRUPAL_BOOTSTRAP_SESSION => "session",
          DRUPAL_BOOTSTRAP_PAGE_HEADER => "page_header",
          DRUPAL_BOOTSTRAP_LANGUAGE => "language",
          DRUPAL_BOOTSTRAP_FULL => "full",
      );
      $controller = array(
          'Controller' => 'drupal_bootstrap',
          'Action' => $phase_names[drupal_get_bootstrap_phase()],
      );
    }
    traceview_set_controller($controller);
  }

  $controller = traceview_set_controller(NULL, FALSE, TRUE);
  if (!empty($controller)) {
    traceview_oboe_log(NULL, 'info', $controller);
  }
}

/**
 * Set the controller to be reported to traceview.
 *
 * @param array $controller
 *   The controller array to be reported to traceview.
 * @param bool $override
 *   Flag to indicate whether the passed controller should override previously
 *   set controller array.
 * @param bool $alter
 *   Flag to indicate whether to call hook_alter on the controller array.
 *
 * @return array
 *   The controller array to be reported to traceview.
 */
function traceview_set_controller($controller = NULL, $override = FALSE, $alter = FALSE) {
  static $traceview_controller = array();
  if (!empty($controller) && ($override || empty($traceview_controller))) {
    $traceview_controller = $controller;
  }
  if (function_exists('drupal_alter') && $alter) {
    drupal_alter('traceview_controller', $traceview_controller);
  }
  return $traceview_controller;
}

/**
 * Get an action string from an unknown argument type.
 *
 * @param mixed $argument
 *   Argument variable as passed to menu callback.
 *
 * @return string
 *   Description (or string conversion) of $argument.
 */
function traceview_get_action($argument) {
  switch (gettype($argument)) {
    case 'integer':
    case 'double':
    case 'boolean':
    case 'string':
      $action = (string) $argument;
      break;

    case 'array':
      $action = 'array';
      break;

    case 'object':
      if (!empty($argument->nid)) {
        $action = (!empty($argument->type) ? $argument->type . '_' : '') . 'node';
      }
      elseif (!empty($argument->uid)) {
        $action = 'user';
      }
      else {
        $action = get_class($argument);
      }
      break;

    case 'resource':
      $action = get_resource_type($argument);
      break;

    default:
      $action = NULL;
      break;
  }
  return $action;
}

/**
 * Set the traceview partition.
 *
 * @param string $partition
 *   Traffic partition name.
 *
 * @return string
 *   Traffic partition name.
 */
function traceview_set_partition($partition = NULL) {
  static $traceview_partition;
  if (!empty($partition)) {
    $traceview_partition = $partition;
  }
  return $traceview_partition;
}

/**
 * Wrapper function for oboe_log to support different library versions, which
 * have different function signatures.
 *
 * @param string $layer
 * @param mixed $label
 * @param mixed $info
 * @param boolean $backtrace
 * @param mixed $edge
 */
function traceview_oboe_log($layer, $label, $info = NULL, $backtrace = NULL, $edge = NULL) {
  // The backtrace option was added in version 1.1.2 and will not be used in
  // earlier versions.
  if ($backtrace === NULL) {
    $backtrace = ($label === 'entry' || $label === 'profile_entry') ? TRUE : FALSE;
  }

  // @see https://support.tv.appneta.com/support/articles/86405-php-instrumentation-release
  switch (TRUE) {
    // Versions ? - 1.1.1
    case version_compare(phpversion('oboe'), '1.1.2', '<'):
      if ($info === NULL) {
        oboe_log($layer, $label);
      }
      elseif ($layer == NULL) {
        oboe_log($label, $info);
      }
      else {
        oboe_log($layer, $label, $info);
      }
      break;

      // Versions 1.1.2 - 1.4.1
      // Added the backtrace parameter.
    case version_compare(phpversion('oboe'), '1.4.2', '<'):
      if ($info === NULL) {
        oboe_log($layer, $label, $backtrace);
      }
      elseif ($layer == NULL) {
        oboe_log($label, $info, $backtrace);
      }
      else {
        oboe_log($layer, $label, $info, $backtrace);
      }
      break;

      // Versions 1.4.2 - 1.4.4
      // These versions broke backwards compatibility such that some calls result
      // in deprecation warnings.  Using @ to suppress warnings is a bad idea, but
      // it's the best option for these versions.
    case version_compare(phpversion('oboe'), '1.4.5', '<'):
      @oboe_log($layer, $label, $info, $backtrace, $edge);
      break;

      // Version 1.4.5+
    default:
      oboe_log($layer, $label, $info, $backtrace, $edge);
      break;
  }
}

/**
 * Add an annotation using the API described here: http://support.tv.appneta.com/kb/how-to/recording-system-events-with-tlog#api
 *
 * @param string $message
 *   Annotation message.
 *
 * @param array $params
 *   Additional annotation parameters.
 *
 * @return boolean
 *   Indicates whether the API call was successful.
 */
function traceview_add_annotation($message, $params = array()) {
  $url = 'https://api.tv.appneta.com/api-v1/log_message';

  $default_params = array(
      'key' => variable_get('traceview_key', FALSE),
      'time' => REQUEST_TIME,
      'appname' => variable_get('traceview_annotation_appname', FALSE),
      'hostname' => gethostname(),
      'username' => variable_get('traceview_annotation_username', variable_get('site_name', 'Drupal')),
      'layer' => 'PHP',
  );

  $params = array_merge($default_params, $params);
  $params['message'] = $message;

  if (empty($params['key'])) {
    watchdog('traceview', "Failed to add TraceView annotation '%annotation': no TraceView client key has been set.", array('%annotation' => $message), WATCHDOG_ERROR);
    return FALSE;
  }

  $http_options = array(
      'headers' => array(
          'Content-Type' => 'application/x-www-form-urlencoded',
      ),
      'method' => 'POST',
      'data' => http_build_query($params),
  );
  $result = drupal_http_request($url, $http_options);

  if (!empty($result->error)) {
    watchdog('traceview', "Failed to add TraceView annotation '%annotation': error message '%error'.", array('%annotation' => $message, '%error' => $result->error), WATCHDOG_ERROR);
    return FALSE;
  }

  return TRUE;
}

/**
 * Wrap calls to drupal_http_request, permitting layer creation and full-stack application tracing.
 *
 * Minimum Drupal version is 7.22: http://drupal.org/drupal-7.22-release-notes
 *
 * @param $url
 *   A string containing a fully qualified URI.
 * @param array $options
 *   (optional) An array with options as per drupal_http_request().
 */
function traceview_drupal_http_request($url, array $options = array()) {
  global $conf;

  // Parse the URL for information about the query being made.
  $uri = parse_url($url);

  // Collect information about non-standard ports
  $remote_host = $uri['host'];
  $remote_host .= isset($uri['port']) && $uri['port'] != 80 ? ':' . $uri['port'] : '';

  // Collect fragments and querystrings
  $service_arg = $uri['path'];
  $service_arg .= isset($uri['fragment']) ? $uri['fragment'] : '';
  $service_arg .= isset($uri['query']) ? $uri['query'] : '';

  // Start the drupal_http_request layer (with a backtrace).
  traceview_oboe_log('drupal_http_request', 'entry', array(
  'IsService' => 'True',
  'RemoteProtocol' => $uri['scheme'],
  'RemoteController' => 'drupal_http_request',
  'RemoteHost' => $remote_host,
  'ServiceArg' => $service_arg,
  ), TRUE);

  // Set an X-Trace header.
  $options['headers']['X-Trace'] = oboe_get_context();

  // Allow drupal_http_request to temporarily bypass the override.
  unset($conf['drupal_http_request_function']);

  // Call drupal_http_request.
  $response = drupal_http_request($url, $options);

  // Put the override back in place.
  $conf['drupal_http_request_function'] = 'traceview_drupal_http_request';

  // Stop the drupal_http_request layer.
  traceview_oboe_log('drupal_http_request', 'exit', array('ResponseCode' => $response->code));

  return $response;
}

/**
 * In order to support configurations where the php-oboe extension may or may
 * not be available during a given application execution, such as during the
 * execution of a drush command, on a singler server in a cluster or on a non-
 * production environment, the following code will optionally define the
 * missing functions as empty functions provided the traceview_fail_silently
 * configuration setting is not empty.
 */
global $conf;
if (!isset($conf['traceview_fail_silently']) || $conf['traceview_fail_silently'] !== FALSE) {
  if (!function_exists('oboe_log')) {
    // These functions intentionally left empty.
    function oboe_log() {}
    function oboe_get_context() {}
    function oboe_get_rum_header() {}
    function oboe_get_rum_footer() {}
  }
}

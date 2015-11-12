<?php
/**
 * @file
 * Drupal RestMini module service provider.
 */

// @todo: Write .yaml drush script.

class RestMiniService extends RestMini {

  /**
   * Default required permission when a registered method doesn't specify a permission.
   *
   * Overriding this property will have no effect because it's being used by final method router().
   *
   * @see hook_restmini_service()
   * @see hook_restmini_service_delegate()
   *
   * @type string
   */
  const PERMISSION_DEFAULT = 'access content';

  /**
   * @type string
   */
  const MACHINE_NAME_REGEX = '/^[a-z][a-z\d_]*$/';

  /**
   * A module name cannot be longer than db:system.filename size minus '.module'.
   *
   * @type string
   */
  const MODULE_NAME_MAXLENGTH = 248;

  /**
   * Max. length of base route, service and endpoint names.
   *
   * @type string
   */
  const NAME_MAXLENGTH = 32;
  // On change, update .api.

  /**
   * @var integer
   */
  protected static $errorCodeOffset = 1300;

  /**
   * Actual numeric values may be affected by non-zero $errorCodeOffset classes extending RestMini.
   *
   * @see RestMini::$errorCodeOffset
   *
   * @var array $errorCodes
   */
  protected static $errorCodes = array(
    'unknown' => 1,
    'algo' => 11,
    'use' => 12,
    //'permission' => 21,
    //'conf_missing' => 31,
    //'conf_bad' => 32,
    'conf_no_services' => 33,
    'conf_service_not_found' => 34,
    'conf_endpoint_not_found' => 35,
    'conf_endpoint_no_methods' => 36,
    'conf_endpoint_no_methods_enabled' => 37,
    // Service errors.
    'service_implementation' => 41,
    'payload_false' => 42,
    'status_not_supported' => 43,
    'missing_redirect_location' => 44,
    'callback_threw_exception' => 45,
    'callback_nonexist_function' => 46,
    'callback_nonexist_class' => 47,
    'callback_nonexist_method' => 48,
  );

  /**
   *  Properties:
   *  - request_path: (str) /base/route/service_name/end_point/path/arguments/if/any
   *  - base_route: (str) base route name
   *  - service: (str) service name
   *  - endpoint: (str) endpoint name
   *  - method: (str) HTTP method (GET|POST etc.)
   *  - responder: (arr) responder's service registry props.
   *  - path_args: (arr) list of path args.
   *
   * @var array|NULL
   */
  protected static $currentResponder;

  /**
   * Whether current request was routed by this module (restmini_service).
   *
   * Private to prevent attempts to override.
   * Overriding wouldn't make sense, because the property is used by methods that are final.
   *
   * @var boolean|NULL
   */
  private static $isRouter;

  /**
   * Was current request routed by this module?
   *
   * @return boolean
   */
  final public static function isRouter() {
    // !! ~ (bool).
    return !!self::$isRouter;
  }

  /**
   * Supports non-Apache servers too.
   *
   * @param string $name
   *   Default: empty (~ get all headers).
   *
   * @return array|string
   *   Array: all readers.
   *   Empty string: header by name $name doesn't exist.
   */
  final public static function requestHeaders($name = '') {
    static $headers;
    if (!$headers) {
      if (function_exists('apache_request_headers')) {
        $headers = apache_request_headers();
      }
      else {
        $headers = array();
        if ($_SERVER) {
          foreach ($_SERVER as $key => $value) {
            if ($value !== '' && substr($key, 0, 5) == 'HTTP_') {
              $headers[str_replace(' ', '-', ucwords(strtolower(str_replace('_', ' ', substr($key, 5)))))] = $value;
            }
          }
          if (array_key_exists('CONTENT_TYPE', $_SERVER)) {
            $headers['Content-Type'] = $_SERVER['CONTENT_TYPE'];
          }
          if (array_key_exists('CONTENT_LENGTH', $_SERVER)) {
            $headers['Content-Length'] = $_SERVER['CONTENT_LENGTH'];
          }
        }
      }
    }

    if (!$name) {
      return $headers;
    }
    return array_key_exists($name, $headers) ? $headers[$name] : '';
  }

  /**
   * Sends status header, a JSON object (conditionally), and exits.
   *
   * Status 404 may be used for two purposes in a REST context: a) to tell that an endpoint doesn't exist at all,
   * and b) to tell that some data doesn't exist.
   * The Drupal menu system will automatically return 404 for a) a non-existent endpoint.
   * Whereas this function should only be used for b) - 'No Results'-like - scenarios.
   *
   *  If $status is empty, status will be according to request method and whether payload is empty (NULL) or not:
   *  - GET|HEAD: 200 if payload, otherwise 404
   *  - POST|PUT: 201 if payload, otherwise 204
   *  - DELETE: 200 if payload, otherwise 204
   *
   *  Response will count as an error and be passed on to restmini_service_error(), with status code 500, if:
   *  - payload is boolean false
   *  - unsupported HTTP request method
   *  - status code 301 or 302 without a redirect_location
   *  - status code <200 or >500
   *
   * Inappropriate use of status codes will be logged to watchdog, unless turned off on the module's admin page
   * (variable: restmini_service_log_bad_response).
   *
   * If $payload is an object, it's length won't be checked; so if it's empty, that won't be detected.
   *
   * jQuery.ajax(): empty objects and null (error or payload) may not exist at all in final jQuery.ajax() response.
   *
   * This method works even if current request wasn't routed by this module.
   *
   * @code
   * // Response object passed, if error or request explicitly asked for response info wrapper (and not status 204, 301, 303 or 304):
   * {
   *   success: true|false,
   *   no_results: true|false
   *   error: false,
   *   message: $message,
   *   payload: $payload
   * }
   * // The request may ask for response info wrapper in two ways:
   * // - send 'X-Rest-Service-Response-Info-Wrapper: 1' header
   * RestMiniClient::make('http://ser.ver', '/rest/service/some_service/some_endpoint', array('headers' => array('X-Rest-Service-Response-Info-Wrapper' => 1)))->get()->result();
   * // - send GET var _service_response_info_wrapper=1
   * RestMiniClient::make('http://ser.ver', '/rest/service/some_service/some_endpoint')->get(NULL, array('_service_response_info_wrapper' => 1))->result();
   * @endcode
   *
   * @see restmini_service_error()
   *
   * @param mixed $payload
   *   NULL|empty string: evaluates to no payload.
   *   FALSE: evaluates to error.
   * @param integer $status
   *   Status <200 or >599 is considered an error and will be passed to restmini_service_error().
   *   Default: zero (~ request considered successful, except if GET and $payload:NULL).
   * @param string $message
   *   Ignored if status 204, 301, 303 or 304.
   *   Default: empty.
   * @param string $redirect_location
   *   Ignored unless $status is 301 or 302.
   *   Default: empty.
   */
  public static function respond($payload, $status = 0, $message = '', $redirect_location = '') {
    // This method should work even if current request wasn't route by this module.

    $success = FALSE;
    $no_results = TRUE;
    $status_text = 'Unknown status';

    // We do not have to check for falsy or inappropriate REQUEST_METHOD, because restmini_service_router() does that (and errs in such case).
    $http_method = $_SERVER['REQUEST_METHOD'];

    // Evaluate payload.
    $payload_empty = FALSE;
    if (!$payload) {
      if ($payload === NULL || $payload === '') {
        $payload_empty = TRUE;
      }
      elseif ($payload === FALSE) {
        self::error('Payload is false', $status, $message, static::errorCode('payload_false'));
        // exits.
      }
    }
    // Could check for empty object, but won't. If a user of this module is sophisticated enough
    // to use objects instead of arrays, the user probably should check for object emptiness.

    try {
      // Autocomplete status code.
      if (!$status) {
        switch ($http_method) {
          case 'GET':
          case 'HEAD':
            $status = !$payload_empty ? 200 : 404;
            break;
          case 'POST':
          case 'PUT':
            $status = !$payload_empty ? 201 : 204;
            break;
          case 'DELETE':
            $status = !$payload_empty ? 200 : 204;
            break;
          default:
            // Some one may be using this function for a request that hasn't routed by our router.
            // Or there's an algo error in restmini_service_router(); should detect unsupported http method.
            throw new Exception('Unsupported request method[' . $http_method . ']. Wrongful use of function ' . __FUNCTION__
              . '() (bypassed restmini_service_router()), or restmini_service algo error', static::errorCode('use'));
        }
      }
      else {
        // Cast status to integer to prevent switch coersion errors.
        $status = (int)$status;
      }

      // Act according to status code.
      switch ($status) {
        case 200: // GET|DELETE|HEAD (or POST, though preferably 201) with payload.
        case 201: // POST|PUT with payload.
          $status_text = $status == 200 ? 'OK' : ($http_method == 'PUT' ? 'Updated' : 'Created');
          // Don't warn if status 200 for method POST, despite restmini_service_status_codes() not listing that combination as valid.

          if (!$payload_empty) {
            $no_results = FALSE;
          }
          else {
            // Empty payload is wrong for these status codes.
            if (variable_get('restmini_service_log_bad_response', TRUE)) {
              static::log(
                'restmini_service propagated',
                'Status code[' . $status . '] response with empty payload',
                NULL,
                array(
                  'responder' => self::$currentResponder,
                  'response' => array(
                    'status' => $status,
                    'status_text' => $status_text,
                    'message' => $message,
                    'payload' => $payload,
                  ),
                ),
                WATCHDOG_WARNING
              );
            }
          }
          $success = TRUE;
          break;

        case 204: // No Content.
          // PUT|DELETE with no payload.
          $status_text = 'No Content';
          // 204 is only for PUT and DELETE.
          if ($http_method != 'PUT' && $http_method != 'DELETE' && variable_get('restmini_service_log_bad_response', TRUE)) {
            static::log(
              'restmini_service propagated',
              'Status code[' . $status . '] in response to non-POST/PUT HTTP method',
              NULL,
              array(
                'responder' => self::$currentResponder,
                'response' => array(
                  'status' => $status,
                  'status_text' => $status_text,
                  'message' => $message,
                ),
              ),
              WATCHDOG_WARNING
            );
          }
          // Payload must be empty when 204 - and it will not be sent.
          if (!$payload_empty && variable_get('restmini_service_log_bad_response', TRUE)) {
            $payload_props = self::payloadTypeLength($payload);
            static::log(
              'restmini_service propagated',
              'Response payload should be empty when status code[' . $status . '], and the payload will not be sent',
              NULL,
              array(
                'responder' => self::$currentResponder,
                'response' => array(
                  'status' => $status,
                  'status_text' => $status_text,
                  'message' => $message,
                  'payload_type' => $payload_props[0],
                  'payload_length' => $payload_props[1],
                ),
              ),
              WATCHDOG_WARNING
            );
          }
          header('HTTP/1.1 ' . $status . ' ' . $status_text);
          // Don't set Content-Type header.
          // Except, the web server will most likely set default (text/html), so override that by an empty type.
          header('Content-Type: ');
          header('Content-Length: 0');

          exit;

        case 301: // Moved Permanently.
        case 302: // Found.
          $status_text = $status == 301 ? 'Moved Permanently' : 'Found';
          if (!$redirect_location) {
            throw new Exception('Status code ' . $status . ' requires non-empty arg redirect_location to function ' . __FUNCTION__ . '()',
              static::errorCode('missing_redirect_location'));
          }
          header('HTTP/1.1 ' . $status . ' ' . $status_text);
          header('Location: ' . $redirect_location);
          header('Content-Length: 0');
          break;

        case 304: // Not Modified.
          $status_text = 'Not Modified';
          // 304 is only for HEAD|GET.
          if (($http_method != 'GET' && $http_method != 'HEAD') && variable_get('restmini_service_log_bad_response', TRUE)) {
            static::log(
              'restmini_service propagated',
              'Status code[' . $status . '] in response to non-HEAD/GET HTTP method',
              NULL,
              array(
                'responder' => self::$currentResponder,
                'response' => array(
                  'status' => $status,
                  'status_text' => $status_text,
                  'message' => $message,
                ),
              ),
              WATCHDOG_WARNING
            );
          }
          // Payload must be empty when 304 - and it will not be sent.
          if (!$payload_empty && variable_get('restmini_service_log_bad_response', TRUE)) {
            $payload_props = self::payloadTypeLength($payload);
            static::log(
              'restmini_service propagated',
              'Response payload should be empty when status code[' . $status . '], and the payload will not be sent',
              NULL,
              array(
                'responder' => self::$currentResponder,
                'response' => array(
                  'status' => $status,
                  'status_text' => $status_text,
                  'message' => $message,
                  'payload_type' => $payload_props[0],
                  'payload_length' => $payload_props[1],
                ),
              ),
              WATCHDOG_WARNING
            );
          }

          header('HTTP/1.1 ' . $status . ' ' . $status_text);
          // Don't set Content-Type nor Content-Length headers.
          // Except, the web server will most likely set default Content-Type (text/html), so override that by an empty type.
          header('Content-Type: ');
          exit;

        case 404: // Not Found.
          // Evaluates to successful, because the endpoint method exists and responds.
          // And we dont want so send response unless X-Rest-Service-Response-Info-Wrapper header the GET var equivalent.
          $success = TRUE;
          $status_text = 'Not Found';
          break;

        case 410: // Not Found.
          // Evaluates to successful, because the endpoint method exists and responds.
          // And we dont want so send response unless X-Rest-Service-Response-Info-Wrapper header the GET var equivalent.
          $success = TRUE;
          $status_text = 'Gone';
          break;

        default:
          $status_text = self::statusText($status);

          if ($status_text == 'Unknown Status') {
            // Status code must be 2xx|3xx|4xx|5xx.
            if ($status < 200 || $status > 599) {
              throw new Exception('Status code[' . $status . '] status text[' . $status_text . '] is not supported by ' . __FUNCTION__ . '()',
                static::errorCode('status_not_supported'));
            }
            $success = TRUE;
            $no_results = $payload_empty;
            // Warn about unsupported status code.
            if (variable_get('restmini_service_log_bad_response', TRUE)) {
              $payload_props = self::payloadTypeLength($payload);
              static::log(
                'restmini_service propagated',
                'Unknown status code[' . $status . ']',
                NULL,
                array(
                  'responder' => self::$currentResponder,
                  'response' => array(
                    'status' => $status,
                    'status_text' => $status_text,
                    'message' => $message,
                    'redirect_location' => $redirect_location,
                    'payload_type' => $payload_props[0],
                    'payload_length' => $payload_props[1],
                  ),
                ),
                WATCHDOG_WARNING
              );
            }
          }
      }
    }
    catch (Exception $xc) {
      self::error($xc, 500, 'Service implementation error', $xc->getCode());
      // exits.
    }

    // Get out.
    header('HTTP/1.1 ' . $status . ' ' . $status_text);
    header('Content-Type: application/json');

    // If error, or request explicitly asks for response info wrapper.
    if (!$success
      // Client:X-Rest-Service-Response-Info-Wrapper ~> Server:HTTP_X_REST_SERVICE_RESPONSE_INFO_WRAPPER.
      || !empty($_SERVER['HTTP_X_REST_SERVICE_RESPONSE_INFO_WRAPPER'])
      || !empty($_GET['_service_response_info_wrapper'])
    ) {
      $response = array(
        'status' => $status,
        'status_text' => $status_text,
        'success' => $success,
        'no_results' => $no_results,
        'error' => !$success,
        'message' => !$message ? ('' . $message) : strip_tags($message),
        'payload' => $payload,
      );
      $stringified = drupal_json_encode($response);
      header('Content-Length: ' . strlen($stringified));
      // Tell client that we're sending the payload in a response info wrapper.
      header('X-Rest-Service-Response-Info-Wrapper: 1');

      if ($http_method != 'HEAD') {
        echo $stringified;
        flush();
      }

      exit;
    }
    // Status 404 shan't send payload.
    if ($payload_empty || $status == 404 || $status == 410) {
      header('Content-Length: 0');
      exit;
    }
    // Status 200, 201, and unknown.
    $stringified = drupal_json_encode($payload);
    header('Content-Length: ' . strlen($stringified));

    if ($http_method != 'HEAD') {
      echo $stringified;
      flush();
    }

    exit;
  }

  /**
   * Sends status header, a JSON object (optional), and exits.
   *
   * Service implementations should use this function in cases where they don't provide error handling by themselves.
   *
   * If non-empty $message or $code: sends JSON-encoded error object.
   *
   * This method works even if current request wasn't routed by this module.
   *
   * @code
   * // Response object passed when $code or $message is non-empty:
   * {
   *   success: false,
   *   no_results: true,
   *   error: true,
   *   error_code: 7913,
   *   error_message: 'Message that ought not expose sensitive system details or data.'
   * }
   * @endcode
   *
   * @param Exception|string|mixed $log
   *   Non-empty will be logged to watchdog.
   *   Default: NULL.
   * @param integer $status
   *   Default: 500 (also if value isn't 4xx or 5xx).
   * @param string $message
   *   Non-empty will be passed back to requestor in JSON response.
   *   Don't expose sensitive system details or data in the message.
   *   Default: empty.
   * @param integer $code
   *   Non-empty will be passed back to requestor in JSON response.
   *   Default: zero.
   */
  public static function error($log, $status = 500, $message = '', $code = 0) {
    // This method should work even if current request wasn't route by this module.

    // Cast status to integer to prevent switch coersion errors.
    $status = (int)$status;

    // Status defaults to 500.
    if (!$status) {
      $status = 500;
    }
    // Status must be 4xx or 5xx.
    elseif ($status < 400 && $status > 599) {
      $status = 500;
    }
    // Set status text.
    $status_text = self::statusText($status);

    // Log Exception or simple message.
    if ($log) {
      // Trace Exception.
      if (is_object($log) && is_subclass_of($log, 'Exception')) {
        static::log(
          'restmini_service propagated',
          $message ? $message : $log->getMessage(),
          $log,
          NULL,
          WATCHDOG_ERROR
        );
      }
      // Simply log.
      else {
        static::log(
          'restmini_service propagated',
          $message ? $message : 'Unknown error',
          NULL,
          array(
            'status' => $status,
            'status_text' => $status_text,
            'code' => $code,
            'log' => $log,
            'responder' => self::$currentResponder,
          ),
          WATCHDOG_ERROR
        );
      }
    }

    // Get out.
    header('HTTP/1.1 ' . $status . ' ' . $status_text);

    if ($_SERVER['REQUEST_METHOD'] != 'HEAD' && ($code || $message)) {
      header('Content-Type: application/json');

      $response = array(
        'status' => $status,
        'status_text' => $status_text,
        'success' => FALSE,
        'no_results' => TRUE,
        'error' => TRUE,
        'code' => (int)$code,
        'message' => !$message ? ('' . $message) : strip_tags($message),
      );
      $stringified = drupal_json_encode($response);

      header('Content-Length: ' . strlen($stringified));
      // Tell client that we're sending the payload in a response info wrapper.
      header('X-Rest-Service-Response-Info-Wrapper: 1');

      echo $stringified;
      flush();
    }
    else {
      header('Content-Length: 0');
    }

    exit;
  }

  /**
   * Get info about current responder.
   *
   *  Native properties:
   *  - request_path: (str) /base/route/service_name/end_point/path/arguments/if/any
   *  - base_route: (str) base route name
   *  - service: (str) service name
   *  - endpoint: (str) endpoint name
   *  - method: (str) HTTP method (GET|POST etc.)
   *  - operation: (str) index|retrieve|create|update|delete
   *  - responder: (arr) responder's service registry props.
   *
   *  Derivative properties:
   *  - resource: (str) service + endpoint
   *  - name: (str) fully qualified name (base route + service + endpoint + operation)
   *  - module: (str)
   *  - parameters: (arr) path|get|post parameters definition
   *
   * @throws Exception
   *   If algo error in this module.
   *
   * @param string $property
   *   Default: empty.
   *
   * @return string|array|NULL|FALSE
   *   Array: all native properties.
   *   NULL: unsupported $property.
   *   FALSE: current request wasn't routed by the RESTmini Service module.
   */
  final public static function currentResponder($property = '') {
    // Not static:: because that property is set by the ::router(); which cannot sensibly use static:: because it's being called by a function.
    // The method is final for the same reason.

    // Did we route this request?
    if (!self::$isRouter) {
      return FALSE;
    }

    // If responder hasn't been populated by router(), then there's an algo error (probably in router()).
    if (!($responder = self::$currentResponder)) { // ~ Copy.
      throw new Exception('No current responder', static::errorCode('algo'));
    }

    // All 'native' properties.
    if (!$property) {
      return $responder;
    }
    // 'Native' properties.
    if (array_key_exists($property, $responder)) {
      return $responder[$property];
    }
    // Derivatives, and properties of sub array 'responder'.
    switch ($property) {
      case 'resource':
        return $responder['service'] . '__' . $responder['endpoint'];
      case 'name':
        return $responder['base_route'] . '__' . $responder['service'] . '__' . $responder['endpoint'] . '__' . $responder['operation'];
      case 'module':
        return $responder['responder']['module'];
      case 'parameters':
        return $responder['responder']['parameters'];
    }
    // Unsupported property.
    return NULL;
  }

  /**
   * Use ::importArguments() instead.
   *
   * @deprecated
   *
   * @see RestMiniService::importArguments()
   *
   * @param array &$failures
   * @param array $parametersDefinition
   * @param boolean $unified
   * @param boolean $continueOnFailure
   *
   * @return array
   */
  public static function importValidateArguments(&$failures, $parametersDefinition = array(), $unified = FALSE, $continueOnFailure = FALSE) {
    $em = 'Method RestMiniService::importValidateArguments() no longer supported, and will be removed in future version of the restmini module.'
      . ' Use RestMiniService::importArguments() instead, which however doesn\'t support custom runtime parameters definition.';
    $validationFailures[] = $em;
    watchdog(
      'restmini_service',
      $em,
      array(),
      WATCHDOG_ERROR
    );

    return $unified ? array() : array(
      'path' => array(),
      'get' => array(),
      'post' => array(),
    );
  }

  /**
   * Imports and validates received arguments for path, get and post parameters.
   *
   * Only arguments declared in current responder's parameters definition
   * - hook_restmini_service/hook_restmini_service_delegate() implementation
   * - will/may be present in the returned array.
   *
   * Reports validation failure if RESTmini Service didn't route current request,
   * or if current reponder has no (or empty) parameters definition.
   *
   * Errs if current operation isn't 'index' and effectively no parameters definition at all.
   * Also errs if any other kind of service implementation error; like wrongly spelled parameters definition buckets (not lowercase path|get|post).
   * Terminates upon error by logging to watchdog and sending status 500.
   *
   * @see RestMini::validate()
   * @see hook_restmini_service()
   *
   * @throws Exception
   *   If there's an algo error in this method or method ::registry().
   *
   * @param array &$validationFailures
   *   Will be populated by validation failure messages (if any).
   *   Must be empty, and must be declared before passed as method arg.
   * @param boolean $unified
   *   Truthy: return shallow array of name-value pairs, disregarding source (path|get|post).
   *   Default: FALSE (~ return deep array, name-value pairs placed in path|get|post sub arrays).
   * @param boolean $continueOnFailure
   *   Default: FALSE (~ abort upon first validation failure).
   *
   * @return array
   *   Imported values, by names, that passed validation - values of failed arguments are flushed.
   *   Responds and exits upon service implementation error.
   */
  public static function importArguments(&$validationFailures, $unified = FALSE, $continueOnFailure = FALSE) {
    // Get pre-defined parameters definition declared in hook_restmini_service()/hook_restmini_service_delegate() implementation.
    if (!self::$isRouter) {
      $em = 'Current request isn\'t routed by restmini_service.';
      $validationFailures[] = $em;
      watchdog(
        'restmini_service',
        $em,
        array(),
        WATCHDOG_ERROR
      );

      return $unified ? array() : array(
        'path' => array(),
        'get' => array(),
        'post' => array(),
      );
    }
    // Empty parameters definition could be hazardous, because no validation will occur.
    if (empty(self::$currentResponder['responder']['parameters'])) {
      // Except: allowed for operation:index.
      if (self::$currentResponder['operation'] == 'index') {

        return $unified ? array() : array(
          'path' => array(),
          'get' => array(),
        );
      }

      static::log(
        self::$currentResponder['responder']['module'],
        'Import and validation of HTTP arguments erred. Parameters definition '
          . (!isset(self::$currentResponder['responder']['parameters']) ? 'not defined' : 'empty') . ', and current operation is not \'index\'',
        NULL,
        array(
          'current responder' => self::$currentResponder,
        ),
        WATCHDOG_ERROR
      );
      static::error(NULL, 500, 'Service implementation error', static::errorCode('service_implementation'));
      exit;
    }

    $parametersDefinition =& self::$currentResponder['responder']['parameters'];

    $values = array();
    $patterns = NULL;
    $failedButContinued = FALSE;

    try {
      foreach ($parametersDefinition as $paramType => $consideredParams) {
        switch ($paramType) {
          case 'path':
            // Create associative array of path args (path args are numerically indexed by nature),
            // using the names of the parameters definition for path parameters.
            $args =& self::$currentResponder['path_args'];
            $nArgs = count($args);
            $nNames = count($names = array_keys($consideredParams));
            // Use the smaller of lengths, if they differ.
            if ($nArgs < $nNames) {
              $nNames = $nArgs;
            }
            $source = array();
            for ($i = 0; $i < $nNames; ++$i) {
              $source[$names[$i]] = $args[$i];
            }
            unset($args, $nArgs, $nNames, $names);
            break;
          case 'get':
            $source =& $_GET;
            break;
          case 'post':
            $source =& $_POST;
            break;
          default:
            throw new Exception(
              'Import and validation of HTTP arguments erred. Unsupported parameters type[' . $paramType
              . '], is not lowercase path|get|post, should have been detected during service registry rebuild or previously in this method',
              static::errorCode('algo')
            );
        }
        if (!$unified) {
          $values[$paramType] = array();
        }

        foreach ($consideredParams as $name => $validationPattern) {
          // Validate, and memorize values that pass validation.
          if (static::validate($validationPattern, $source, $name)) {
            // If the value isn't set originally, and no 'optional':false or 'default' rules, then it still doesn't exist.
            if (isset($source[$name])) {
              if (!$unified) {
                $values[$paramType][$name] = $source[$name];
              }
              else {
                $values[$name] = $source[$name];
              }
            }
          }
          elseif (!$continueOnFailure) {
            // Array union is acceptable here, because $failure is (supposed to be) empty.
            $validationFailures += self::validationFailures();
            return $values;
          }
          else {
            $failedButContinued = TRUE;
          }
        }
      }
    }
    catch (Exception $xc) {
      static::log(
        self::$currentResponder['responder']['module'],
        'Import and validation of HTTP arguments erred',
        $xc,
        NULL,
        WATCHDOG_ERROR
      );

      static::error(NULL, 500, 'Service implementation error', static::errorCode('service_implementation'));
      exit;
    }

    if ($failedButContinued) {
      $validationFailures += self::validationFailures();
    }

    return $values;
  }

  /**
   * For internal purposes only; routes service request to first enabled service endpoint method callback.
   *
   * Executes the responder call back in try-catch.
   *
   * Endpoint method callbacks may handle response dispatchment by themselves (and exit),
   * or by using restmini_service_respond()|restmini_service_error(),
   * or return a payload to this router.
   *
   *  If the callback returns a payload (doesn't exit):
   *  - boolean false payload is considered an error, and router will send 500 header (and no body)
   *  - oherwise the payload will be passed on to restmini_service_respond()
   *
   * The downside of returning payload and relying on router using restmini_service_respond()
   * is less control over the response.
   *
   *  Error scenarios resulting in status 500 (~ Internal Server Error):
   *  - caches not in sync; menu system cache vs. restmini_service cache
   *  - service endpoint method callback not callable (function, class, method doesn't exist, or class method isnt static; typically missing include)
   *
   *  Error scenarios resulting in status 503 (~ Service Unavailable):
   *  - no methods present or enabled for the service endpoint
   *
   * This method should not be overridden (nor called via static::) as long as restmini_service_router() is used,
   * because restmini_service_router() cannot and doesn't call static:: (the function calls RestMiniService::).
   * Unfortunately this also means that this function cannot report error codes correctly when this class is extended
   * (and extender sets a different error code offset).
   *
   * @see restmini_service_router()
   *
   * @param array $args.
   */
  final public static function router($args) {
    // NB: Use self::, not static::, this method is being called by function(s) that don't know of any extending class(es).

    // Aye, this module (not some other REST service API) caught the ball.
    self::$isRouter = TRUE;

    // We trust and don't check these 3 arguments, assuming that they originate
    // from core menu system's invocation of restmini_service_router().
    $route_name = $args[0];
    $service_name = $args[1];
    $endpoint_name = $args[2];

    // Get rid of $route_name, $service_name and $endpoint_name.
    array_splice($args, 0, 3);

    // A request to an undefined service or endpoint may only get here if the menu system's cache has been cleared
    // while this module's cache hasn't been cleared (and no service module implements the service or endpoint anymore).
    // Still - let's be friendly and give error messages with clues to the problem.
    if (!($routes = self::registry())) {
      self::error('Menu system and restmini_service caches not in sync: No services defined',
        500, 'No services defined', self::errorCode('conf_no_services'));
      // exits.
    }
    if (!array_key_exists($route_name, $routes)) {
      self::error('Menu system and restmini_service caches not in sync: Base route[' . $route_name . '] not found',
        500, 'Service not found', self::errorCode('conf_service_not_found'));
      // exits.
    }
    $services = $routes[$route_name];
    if (!array_key_exists($service_name, $services)) {
      self::error('Menu system and restmini_service caches not in sync: Base route[' . $route_name . '] service[' . $service_name
        . '] endpoint[' . $endpoint_name . '] not found', 500, 'Endpoint not found', self::errorCode('conf_endpoint_not_found'));
      // exits.
    }
    if (!array_key_exists($endpoint_name, $services[$service_name])) {
      self::error('Menu system and restmini_service caches not in sync: Base route[' . $route_name . '] service[' . $service_name
        . '] endpoint[' . $endpoint_name . '] not found', 500, 'Endpoint not found', self::errorCode('conf_endpoint_not_found'));
      // exits.
    }
    $endpoint =& $services[$service_name][$endpoint_name];

    // Badly defined endpoint.
    if (empty($endpoint)) {
      // ~ Service Unavailable.
      self::error('Base route[' . $route_name . '] service[' . $service_name . '] endpoint[' . $endpoint_name . '] has no methods',
        503, 'Endpoint not implemented', self::errorCode('conf_endpoint_no_methods'));
      // exits.
    }

    $method_name = '' . $_SERVER['REQUEST_METHOD'];
    // Paranoid.
    if (!$method_name) {
      self::log(
        'restmini_service',
        'Falsy SERVER REQUEST_METHOD',
        NULL,
        $_SERVER['REQUEST_METHOD'],
        WATCHDOG_ERROR
      );
      header('HTTP/1.1 500 Internal Server Error');
      exit;
    }

    $supported_methods =& self::$supportedMethods;

    // If TRACE or something, we don't care to tell which methods are allowed (despite RFC-2616 requirement).
    if (!in_array($method_name, $supported_methods)) {
      header('HTTP/1.1 405 Method Not Allowed');
      exit;
    }

    // If no enabled implementation of the endpoint method,
    // we'll check if other methods are enabled, to give status 405 response.
    if (empty($endpoint[$method_name]['enabled'][0]['callback'])) {
      $enabled_methods = array();
      foreach ($supported_methods as $supported_method) {
        if ($supported_method != $method_name && !empty($endpoint[$supported_method]['enabled'][0]['callback'])) {
          $enabled_methods[] = $supported_method;
        }
      }
      if ($enabled_methods) {
        header('HTTP/1.1 405 Method Not Allowed');
        header('Allow: ' . join(', ', $enabled_methods));
        exit;
      }
      // ~ Service Unavailable.
      self::error('Service[' . $service_name
        . '] endpoint[' . $endpoint_name . '] has no enabled methods',
        503, 'No methods of the endpoint enabled', self::errorCode('conf_endpoint_no_methods_enabled'));
      // exits.
    }

    // We use the first enabled implementation.
    $responder =& $endpoint[$method_name]['enabled'][0];

    // Menu system permission checking doesn't seem to work for these routes.
    $permission = !empty($responder['permission']) ? $responder['permission'] : self::PERMISSION_DEFAULT;
    if (!user_access($permission)) {
      // We don't want any content output, therefore not:
      // drupal_deliver_page(MENU_ACCESS_DENIED);

      // Log precisely like core does.
      watchdog('access denied', check_plain($_GET['q']), NULL, WATCHDOG_WARNING);

      header('HTTP/1.1 403 Forbidden');
      exit;
    }

    $callback = $responder['callback'];

    // Url decode once more (menu system already does it once),
    // in case a path fragment contains double encoded chars (like: / ? & =).
    if ($args) {
      foreach ($args as &$value) {
        if ($value) {
          $value = rawurldecode($value);
        }
      }
      unset($value); // Iteration ref.
    }

    try {
      // Include?
      if (!empty($responder['filename']) && !module_load_include($responder['fileext'], $responder['module'], $responder['filename'])) {
        self::error(
          'Module[' . $responder['module'] . '] service[' . $service_name . '] endpoint[' . $endpoint_name . '] method[' . $method_name
          . '] filename[' . $responder['filename'] . '] fileext[' . $responder['fileext'] . '] cannot be included.',
          500
        );
      }

      // Establish operation name.
      switch ($method_name) {
        case 'GET':
        case 'HEAD':
          $operation = !$args ? 'index' : 'retrieve';
          break;
        case 'POST':
          $operation = 'create';
          // If empty $_POST and Content-Type: application/json - POST data has
          // to be populated.
          if (!$_POST && strpos(self::requestHeaders('Content-Type'), 'application/json') === 0
            && ($input = file_get_contents('php://input'))
          ) {
            $jsonParsed = json_decode($input);
            // Send 404 Bad Request if body not parsable.
            if ($jsonParsed === NULL && json_last_error() !== JSON_ERROR_NONE) {
              header('HTTP/1.1 400 Bad Request');
              exit;
            }
            // Convert JSON object hashmap to PHP associative array.
            $_POST = (array) $jsonParsed;
            unset($input, $jsonParsed); // Relieve memory.
          }
          // Non-empty $_POST must be array.
          elseif (!is_array($_POST)) {
            header('HTTP/1.1 400 Bad Request');
            exit;
          }
          break;
        case 'PUT':
          $operation = 'update';
          // If PUT method and empty $_POST - POST data has to be populated.
          if (!$_POST) {
            $requestBodyContentType = self::requestHeaders('Content-Type');
            if ($requestBodyContentType == 'application/x-www-form-urlencoded') {
              if (($input = file_get_contents('php://input'))) {
                parse_str($input, $_POST);
              }
            }
            elseif (strpos($requestBodyContentType, 'application/json') === 0
              && ($input = file_get_contents('php://input'))
            ) {
              $jsonParsed = json_decode($input);
              // Send 404 Bad Request if body not parsable.
              if ($jsonParsed === NULL && json_last_error() !== JSON_ERROR_NONE) {
                header('HTTP/1.1 400 Bad Request');
                exit;
              }
              // Convert JSON object hashmap to PHP associative array.
              $_POST = (array) $jsonParsed;
              unset($jsonParsed); // Relieve memory.
            }
            unset($input); // Relieve memory.
          }
          // Non-empty $_POST must be array.
          elseif (!is_array($_POST)) {
            header('HTTP/1.1 400 Bad Request');
            exit;
          }
          break;
        case 'DELETE':
          $operation = 'delete';
          break;
        default:
          // Algo error, see ::$supportedMethods.
          self::error(
            'Module[' . $responder['module'] . '] service[' . $service_name . '] endpoint[' . $endpoint_name . '] method[' . $method_name
            . '] not supported, method not in ' . join(', ', self::$supportedMethods) . '.',
            500
          );
          exit; // Explicit exit because otherwise IDE will complain about undeclared var.
      }

      // Memorize current responder properties.
      self::$currentResponder = array(
        'request_path' => $_GET['q'],
        'base_route' => $route_name,
        'service' => $service_name,
        'endpoint' => $endpoint_name,
        'method' => $method_name,
        'operation' => $operation,
        'responder' => &$responder,
        'path_args' => $args,
      );

      // Remove the 'q' argument (Drupal request path) from GET vars,
      // to make $_GET reflect net GET arguments sent by the client.
      unset($_GET['q']);

      // The callback should exit, unless relying on router handling response.
      // If the call_user_func[_array]() returns false it's considered an error,
      // and it may indicate that the callback isnt callable at all.
      // However, call_user_func[_array]() may also - contrary to documentation - return null
      // if the callback function|class|method doesn't exist.
      if (!($payload = ($args ? call_user_func_array($callback, $args) : call_user_func($callback)))
        && (($isNull = $payload === NULL) || $payload === FALSE)
      ) {
        if (($pos = strpos($callback, '::'))) {
          $class = substr($callback, 0, $pos);
          $func = substr($callback, $pos + 2);
          if (!class_exists($class, FALSE)) { // No reason to attempt autoload; that doesn't work in D7.
            self::error(
              'Module[' . $responder['module'] . '] service[' . $service_name . '] endpoint[' . $endpoint_name . '] method[' . $method_name
              . '] callback[' . $callback . '] class[' . $class . '] doesnt exist or isnt included automatically.',
              500, 'Non-existent callback class', self::errorCode('callback_nonexist_class')
            );
            // exits.
          }
          if (!method_exists($class, $func)) {
            self::error(
              'Module[' . $responder['module'] . '] service[' . $service_name . '] endpoint[' . $endpoint_name . '] method[' . $method_name
              . '] callback[' . $callback . '] class[' . $class . '] method[' . $func . '] doesnt exist or isnt static.',
              500, 'Non-existent callback method', self::errorCode('callback_nonexist_method')
            );
            // exits.
          }
        }
        elseif (!function_exists($callback)) {
          self::error(
            'Module[' . $responder['module'] . '] service[' . $service_name . '] endpoint[' . $endpoint_name . '] method[' . $method_name
            . '] callback[' . $callback . '] doesnt exist or isnt included automatically.',
            500, 'Non-existent callback function', self::errorCode('callback_nonexist_function')
          );
          // exits.
        }

        // NULL is a fully valid response payload, whereas FALSE isn't.
        if (!$isNull) {
          self::error(
            'Module[' . $responder['module'] . '] service[' . $service_name . '] endpoint[' . $endpoint_name . '] method[' . $method_name
            . '] callback[' . $callback . '] returned boolean false, despite callback available.',
            500
          );
          // exits.
        }
      }

      // Send to fallback responder.
      self::respond($payload);
    }
    catch (Exception $xc) {
      self::log(
        'restmini_service propagated',
        'Module[' . $responder['module'] . '] service[' . $service_name . '] endpoint[' . $endpoint_name . '] method[' . $method_name
        . '] callback[' . $callback . '] threw exception.',
        $xc,
        NULL,
        WATCHDOG_ERROR
      );
    }

    // Callbacks are not allowed to throw exceptions.
    self::error(NULL, 500, 'Unknown service error', self::errorCode('callback_threw_exception'));
  }

  /**
   * For internal purposes only; get list of registered services and their endpoints and methods.
   *
   * Sets new cached list in cache 'restmini_service__registry', if $refreshCache or the cache is empty.
   *
   * Invokes hook_restmini_service() and hook_restmini_service_delegate().
   *
   * Delegated entries takes precedence over the entries delivered by service modules themselves,
   * because delegation is intended to be a way of controlling all (or most) endpoints from a single module.
   *
   * This method cannot be overridden (nor called via static::) because restmini_service_menu() calls it.
   *
   * @see restmini_service_router()
   *
   * @param boolean $refreshCache
   *   Default: FALSE (~ get from cache, if exists).
   * @return array
   */
  final public static function registry($refreshCache = FALSE) {
    // NB: Use self::, not static::, this method is being called by function(s) that don't know of any extending class(es).

    if (!$refreshCache && ($registry = cache_get('restmini_service__registry')) && ($registry = $registry->data)) {
      return $registry;
    }
    unset($registry);

    // The string pattern checks in this method may not necessarily augment security,
    // but they should at least make it easier to track down configuration errors.

    // We want to record all errors in one go, thus we cannot use exceptions.
    // And unsafe strings in error messages must be HTML escaped, for drupal_set_message().
    $errors = array();

    // Build service registry.
    $routes = array();

    // Get base paths defined available (for all services).
    $defined_base_routes = restmini_service_base_routes();

    if (!$defined_base_routes) {
      $errors[] = 'No REST service base routes defined.';
    }
    else {
      $supported_methods =& self::$supportedMethods;

      $delegates = array();

      // Get implementations declared by hub-like modules on behalf of actual service modules.
      // hook_restmini_service_delegate().
      $modules = module_implements('restmini_service_delegate');
      foreach ($modules as $module_name) {
        // Check for illegally named delegate module (the delegate itself).
        // This check also secures that module isn't named _SELF_.
        if (!preg_match(self::MACHINE_NAME_REGEX, $module_name)) {
          $errors[] = 'Delegate[' . check_plain($module_name) . '] module name is not a valid machine name.';
          continue;
        }
        $function = $module_name . '_restmini_service_delegate';
        $delegates[$module_name] = $function();
      }
      // Get implementations declared by service modules selves.
      $delegates['_SELF_'] = array();
      // hook_restmini_service().
      $modules = module_implements('restmini_service');
      foreach ($modules as $module_name) {
        $function = $module_name . '_restmini_service';
        $delegates['_SELF_'][$module_name] = $function();
      }

      foreach ($delegates as $delegate => $service_modules) {

        $is_delegated = $delegate !== '_SELF_';

        foreach ($service_modules as $module_name => $base_routes) {

          // Check delegated module name.
          if ($is_delegated) {
            if (strlen($module_name) > self::MODULE_NAME_MAXLENGTH) { // Byte length; deliberately not drupal_strlen().
              $errors[] = 'delegate[' . $delegate . '] module[' . check_plain($module_name) . '] is longer than '
                . self::MODULE_NAME_MAXLENGTH . ' ASCII chars.';
              continue;
            }
            if (!preg_match(self::MACHINE_NAME_REGEX, $module_name)) {
              $errors[] = 'delegate[' . $delegate . '] module[' . check_plain($module_name) . '] is not a valid machine name.';
              continue;
            }

            // Do not check if the module exists - it may be installed later.
            // We do not want to flood log with what very well may be rubbish warnings.
            // Administrative list of endpoints must check it though.

            // For error messages.
            $em_start = 'delegate[' . $delegate . '] module[' . $module_name . ']';
          }
          else {
            $em_start = 'module[' . $module_name . ']';
          }

          // Iterate the base routes that the service module implements service endpoint methods to.
          foreach ($base_routes as $route_name => $services) {

            // Check suggested base route name and existence.
            if (strlen($route_name) > self::NAME_MAXLENGTH) { // Byte length; deliberately not drupal_strlen().
              $errors[] = $em_start . ' base route name[' . check_plain($route_name) . '] is longer than ' . self::NAME_MAXLENGTH . ' ASCII chars.';
              continue;
            }
            if (!preg_match(self::MACHINE_NAME_REGEX, $route_name)) {
              $errors[] = $em_start . ' base route name[' . check_plain($module_name) . '] is not a valid machine name.';
              continue;
            }
            if (!array_key_exists($route_name, $routes)) {
              // Check that base route exists.
              if (!array_key_exists($route_name, $defined_base_routes)) {
                $errors[] = $em_start . ' base route[' . $route_name . '] is not defined.';
                continue;
              }

              // Add the base route to routes in use.
              $routes[$route_name] = array();
            }

            foreach ($services as $service_name => $endpoints) {

              // Check that service name is a machine name.
              if (strlen($service_name) > self::NAME_MAXLENGTH) { // Byte length; deliberately not drupal_strlen().
                $errors[] = $em_start . ' base route[' . $route_name . '] service[' . check_plain($service_name)
                  . '] is longer than ' . self::NAME_MAXLENGTH . ' ASCII chars.';
                continue;
              }
              if (!preg_match(self::MACHINE_NAME_REGEX, $service_name)) {
                $errors[] = $em_start . ' base route[' . $route_name . '] service[' . check_plain($service_name) . '] is not a valid machine name.';
                continue;
              }
              if (!array_key_exists($service_name, $routes[$route_name])) {
                // Add the service to services in use of current route.
                $routes[$route_name][$service_name] = array();
              }

              foreach ($endpoints as $endpoint_name => $methods) {

                // Check that endpoint_name is a machine name.
                if (strlen($endpoint_name) > self::NAME_MAXLENGTH) { // Byte length; deliberately not drupal_strlen().
                  $errors[] = $em_start . ' base route[' . $route_name . '] service[' . $service_name . '] endpoint[' . check_plain($endpoint_name)
                    . '] is longer than ' . self::NAME_MAXLENGTH . ' ASCII chars.';
                  continue;
                }
                if (!preg_match(self::MACHINE_NAME_REGEX, $endpoint_name)) {
                  $errors[] = $em_start . ' base route[' . $route_name . '] service[' . $service_name . '] endpoint[' . check_plain($endpoint_name)
                    . '] is not a valid machine name.';
                  continue;
                }
                if (!array_key_exists($endpoint_name, $routes[$route_name][$service_name])) {
                  $routes[$route_name][$service_name][$endpoint_name] = array();
                }

                foreach ($methods as $method_name => $method) {
                  // Check that endpoint_name is sensible.
                  if (strlen($method_name) > 7) { // Byte length; deliberately not drupal_strlen().
                    $errors[] = $em_start . ' base route[' . $route_name . '] service[' . $service_name
                      . '] endpoint[' . $endpoint_name . '] method name[' . check_plain($method_name)
                      . '] is longer than ' . 7 . ' ASCII chars.';
                    continue;
                  }
                  if (!preg_match('/^' . join('|', $supported_methods) . '$/', $method_name)) {
                    $errors[] = $em_start . ' base route[' . $route_name . '] service[' . $service_name
                      . '] endpoint[' . $endpoint_name . '] method name[' . check_plain($method_name)
                      . '] isnt ' . join('&#124;', $supported_methods) . ' or not uppercase.';
                    continue;
                  }

                  // Check callback.
                  // Unfortunately we cannot check here if the callback is callable,
                  // because modules and their included files may not be available at this point during a cache clear.
                  // Instead, restmini_service_router() checks it at runtime if call_user_func_array() returns false.
                  if (empty($method['callback'])) {
                    $errors[] = $em_start . ' base route[' . $route_name . '] service[' . $service_name
                      . '] endpoint[' . $endpoint_name . '] method[' . $method_name . '] \'callback\' bucket is empty or non-existent.';
                    continue;
                  }
                  if (!preg_match('/^[a-zA-Z\d_]+(\:\:[a-zA-Z\d_]+)?$/', $method['callback'])) {
                    $errors[] = $em_start . ' base route[' . $route_name . '] service[' . $service_name
                      . '] endpoint[' . $endpoint_name . '] method[' . $method_name . '] callback[' . check_plain($method['callback']) . ']'
                      . ' is not a valid function or Class::method name.';
                    continue;
                  }

                  $responder = array(
                    // For hook_menu() implementation.
                    'callback' => $method['callback'],
                    // For restmini_service_router.
                    'module' => $module_name,
                  );

                  // For hook_menu() implementation.
                  // Menu 'access arguments' alternative.
                  if (!empty($method['permission'])) {
                    if (!is_string($method['permission'])) {
                      $errors[] = $em_start . ' base route[' . $route_name . '] service[' . $service_name
                        . '] endpoint[' . $endpoint_name . '] method[' . $method_name
                        . '] \'permission\' bucket type[' . gettype($method['permission']) . '] isnt either string or empty.';
                      continue;
                    }

                    $responder['permission'] = $method['permission'];
                  }

                  // Check parameters definition, if exists.
                  if (!empty($method['parameters'])) {
                    if (!is_array($method['parameters'])) {
                      $errors[] = $em_start . ' base route[' . $route_name . '] service[' . $service_name
                        . '] endpoint[' . $endpoint_name . '] method[' . $method_name . '] (optional property) parameters, data type['
                        . gettype($method['parameters']) . '], is not array.';
                      continue;
                    }
                    $paramTypes = array_keys($method['parameters']);
                    // Prevent continue error in switch within more loops.
                    $continue = FALSE;
                    foreach ($paramTypes as $paramType) {
                      switch ('' . $paramType) {
                        case 'path':
                        case 'get':
                        case 'post':
                          if (empty($method['parameters'][$paramType])) {
                            unset($method['parameters'][$paramType]);
                          }
                          elseif (!is_array($method['parameters'][$paramType])) {
                            $errors[] = $em_start . ' base route[' . $route_name . '] service[' . $service_name
                              . '] endpoint[' . $endpoint_name . '] method[' . $method_name . '] parameter type['
                              . $paramType . '], data type[' . gettype($method['parameters']) . '], is not array.';
                            $continue = TRUE;
                          }
                          // Pre-compile validation patterns.
                          try {
                            foreach ($method['parameters'][$paramType] as $paramName => &$validationPattern) {
                              $validationPattern = RestMini::validationPattern('', $validationPattern);
                            }
                          }
                          catch (Exception $xc) {
                            $errors[] = $em_start . ' base route[' . $route_name . '] service[' . $service_name
                              . '] endpoint[' . $endpoint_name . '] method[' . $method_name . '] parameter type['
                              . $paramType . '] parameter name[' . $paramName . '] is not a valid validation pattern.';
                            $continue = TRUE;
                          }
                          break;
                        default:
                          $errors[] = $em_start . ' base route[' . $route_name . '] service[' . $service_name
                            . '] endpoint[' . $endpoint_name . '] method[' . $method_name . '] (optional property) parameter type['
                            . $paramType . '] is not lowercase path|get|post.';
                          $continue = TRUE;
                      }
                      if ($continue) {
                        continue 2;
                      }
                    }

                    $responder['parameters'] = $method['parameters'];
                  }
                  else {
                    $responder['parameters'] = array();
                  }

                  // Make file usable for module_load_include().
                  if (!empty($method['file'])) {
                    $file_parts = pathinfo($method['file']);
                    // For restmini_service_router.
                    $responder['filename'] = $file_parts['dirname'] . ($file_parts['dirname'] === '' ? '' : '/') . $file_parts['filename'];
                    $responder['fileext'] = $file_parts['extension'];
                  }

                  // Save delegate name for service overviews etc.
                  if ($is_delegated) {
                    $responder['delegate'] = $delegate;
                  }

                  // Add the endpoint method to list of either enabled or disabled implementations.
                  if (!array_key_exists($method_name, $routes[$route_name][$service_name][$endpoint_name])) {
                    $routes[$route_name][$service_name][$endpoint_name][$method_name] = array(
                      'enabled' => array(),
                      'disabled' => array(),
                    );
                  }
                  if (!empty($method['disabled']) || (array_key_exists('enabled', $method) && !$method['enabled'])) {
                    $routes[$route_name][$service_name][$endpoint_name][$method_name]['disabled'][] = $responder;
                  }
                  else {
                    $routes[$route_name][$service_name][$endpoint_name][$method_name]['enabled'][] = $responder;
                  }
                }
              }
            }
          }
        }
      }
    }

    cache_set('restmini_service__registry', $routes, 'cache', CACHE_PERMANENT);

    if ($errors) {
      $em = 'Building service registry: ';
      foreach ($errors as $error) {
        watchdog(
          'restmini_service',
          $em . $error,
          array(),
          WATCHDOG_WARNING
        );
        drupal_set_message($em . $error, 'warning');
      }
    }

    return $routes;
  }

  /**
   * Helper for ::respond().
   *
   * @param $payload
   *
   * @return array
   */
  protected static function payloadTypeLength($payload) {
    switch (($type = gettype($payload))) {
      case 'array':
        $length = count($payload);
        break;
      case 'string':
        $length = drupal_strlen($payload);
        break;
      case 'object':
        $type = get_class($payload);
        $length = count(get_object_vars($payload));
        break;
      case 'NULL':
        $length = 0;
        break;
      default:
        $length = 1;
    }
    return array(
      $type,
      $length
    );
  }

}

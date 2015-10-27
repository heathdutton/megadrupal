<?php
/**
 * @file
 * Drupal RestMini Service Test.
 */


// @todo: make GUI.

class RestMiniServiceTest extends RestMini {

  /**
   * @var string
   */
  public $testModule;

  /**
   * @var string
   */
  public $testName;

  /**
   * @var array
   */
  public $testDeclaration;

  /**
   * @var array
   */
  public $testResults;

  /**
   * @var array
   */
  public $resultData = array();

  /**
   * @param string $testModule
   * @param string $testName
   * @param array $testDeclaration
   */
  public function __construct($testModule, $testName, $testDeclaration) {
    $this->testModule = $testModule;
    $this->testName = $testName;
    $this->testDeclaration = $testDeclaration;

    static::$results[$testModule][$testName] = array();
    $this->testResults =& static::$results[$testModule][$testName];
  }

  /**
   * Recurses into previous result data to find value (bucket) specified by RestMiniServiceTestVar keyPath.
   *
   * @throws Exception
   *
   * @param array &$args
   * @param array $keyPath
   */
  protected function prepareArguments(&$args, $keyPath = array()) {
    foreach ($args as $key => &$value) {
      if (is_array($value)) {
        $keys = $keyPath;
        $keys[] = $key;
        $this->prepareArguments($value, $keys);
      }
      elseif (is_object($value)) {
        if (get_class($value) != 'RestMiniServiceTestVar') {
          $value = get_object_vars($value);
          $keys = $keyPath;
          $keys[] = $key;
          $this->prepareArguments($value, $keys);
        }
        else {
          $testVar = $value;

          if (!array_key_exists($testVar->baseRoute, $this->resultData)) {
            throw new Exception('bucket[' . join('][', $keyPath) . '] previous result[base route:' . $testVar->baseRoute . '] doesn\'t exist.');
          }
          $result = $this->resultData[$testVar->baseRoute];

          if (!array_key_exists($testVar->service, $result)) {
            throw new Exception('bucket[' . join('][', $keyPath) . '] previous result[base route:' . $testVar->baseRoute . '][service:'
              . $testVar->service . '] doesn\'t exist.');
          }
          $result = $result[$testVar->service];

          if (!array_key_exists($testVar->endpoint, $result)) {
            throw new Exception('bucket[' . join('][', $keyPath) . '] previous result[base route:' . $testVar->baseRoute . '][service:'
              . $testVar->service . '][endpoint:' . $testVar->endpoint . '] doesn\'t exist.');
          }
          $result = $result[$testVar->endpoint];

          if (!array_key_exists($testVar->operation, $result)) {
            throw new Exception('bucket[' . join('][', $keyPath) . '] previous result[base route:' . $testVar->baseRoute . '][service:'
              . $testVar->service . '][endpoint:' . $testVar->endpoint . '][operation:' . $testVar->operation . '] doesn\'t exist.');
          }
          $result = $result[$testVar->operation];

          if (!array_key_exists($testVar->requestIndex, $result)) {
            throw new Exception('bucket[' . join('][', $keyPath) . '] previous result[base route:' . $testVar->baseRoute . '][service:'
              . $testVar->service . '][endpoint:' . $testVar->endpoint . '][operation:' . $testVar->operation . '][request index:'
              . $testVar->requestIndex . '] doesn\'t exist.');
          }
          $result = $result[$testVar->requestIndex];

          // Recurse.
          $resultPath = '';
          foreach ($testVar->keyPath as $resultKey) {
            if ($result) {
              $isObject = FALSE;
              if (is_array($result) || ($isObject = is_object($result))) {
                if ($isObject) {
                  $resultPath .= '->' . $resultKey;
                  if (property_exists($result, $resultKey)) {
                    $result = $result->{$resultKey};
                    continue;
                  }
                }
                else {
                  $resultPath .= '[' . $resultKey . ']';
                  if (array_key_exists($resultKey, $result)) {
                    $result = $result[$resultKey];
                    continue;
                  }
                }
                throw new Exception('bucket[' . join('][', $keyPath) . '] previous result[base route:' . $testVar->baseRoute . '][service:'
                  . $testVar->service . '][endpoint:' . $testVar->endpoint . '][operation:' . $testVar->operation . '][request index:'
                  . $testVar->requestIndex . ']' . $resultPath . ' doesn\'t exist.');
              }
            }
            throw new Exception('bucket[' . join('][', $keyPath) . '] previous result[base route:' . $testVar->baseRoute . '][service:'
              . $testVar->service . '][endpoint:' . $testVar->endpoint . '][operation:' . $testVar->operation . '][request index:'
              . $testVar->requestIndex . ']' . $resultPath . ', type[' . gettype($result) . '], value[' . $result . '] has no key[' . $resultKey
              . '] because it\'s not an array or object.');
          }

          $value = $result;
        }
      }
      // else...: keep $value.
    }
  }

  /**
   * @param array $filter
   *
   * @return array
   */
  protected function listPublicAttributes($filter = array()) {
    $a = call_user_func('get_object_vars', $this);
    if ($filter) {
      foreach ($filter as $name) {
        unset($a[$name]);
      }
    }
    return $a;
  }

  /**
   * @return boolean
   *   FALSE: on error.
   */
  protected function execute() {
    // We use these a lot.
    $baseRoutes =& static::$baseRoutes;
    $registry =& static::$registry;

    static::message('!nlCommencing test !module:!name...', array('!module' => $this->testModule, '!name' => $this->testName));

    try {
      foreach ($this->testDeclaration as $step => $testBaseRoutes) {

        $this->testResults[$step] = array();

        // Create reusable message, for step.
        $messageStep = 'Step !module:!name:!step';
        $replacersStep = array('!module' => $this->testModule, '!name' => $this->testName, '!step' => $step);

        static::message($messageStep . '...', $replacersStep);

        foreach ($testBaseRoutes as $baseRouteName => $services) {
          if (!array_key_exists($baseRouteName, $baseRoutes)) {
            static::$resultSummary[-1] += 1;
            $this->testResults[$step][$baseRouteName] = 'base route doesn\'t exist';
            static::message($messageStep . ', base route !route_name doesn\'t exist.',
              array_merge($replacersStep, array('!route_name' => $baseRouteName)), 'warning');
            return FALSE;
          }
          if (!array_key_exists($baseRouteName, $registry)) {
            static::$resultSummary[-1] += 1;
            $this->testResults[$step][$baseRouteName] = 'has no registered services';
            static::message($messageStep . ', base route !route_name has no registered services.',
              array_merge($replacersStep, array('!route_name' => $baseRouteName)), 'warning');
            return FALSE;
          }
          $basePath = '/' . $baseRoutes[$baseRouteName];

          if (empty($this->resultData[$baseRouteName])) {
            $this->resultData[$baseRouteName] = array();
          }
          $this->testResults[$step][$baseRouteName] = array();

          foreach ($services as $serviceName => $endpoints) {
            if (!array_key_exists($serviceName, $registry[$baseRouteName])) {
              static::$resultSummary[-1] += 1;
              $this->testResults[$step][$baseRouteName][$serviceName] = 'service doesn\'t exist';
              static::message(
                $messageStep . ', base route !route_name has no service !service, check RESTmini Service settings page.',
                array_merge($replacersStep, array('!route_name' => $baseRouteName, '!service' => $serviceName)),
                'warning'
              );
              return FALSE;
            }
            $servicePath = $basePath . '/' . $serviceName;
            if (empty($this->resultData[$baseRouteName][$serviceName])) {
              $this->resultData[$baseRouteName][$serviceName] = array();
            }
            $this->testResults[$step][$baseRouteName][$serviceName] = array();

            foreach ($endpoints as $endpointName => $requests) {
              if (!array_key_exists($endpointName, $registry[$baseRouteName][$serviceName])) {
                static::$resultSummary[-1] += 1;
                $this->testResults[$step][$baseRouteName][$serviceName][$endpointName] = 'endpoint doesn\'t exist';
                static::message(
                  $messageStep . ', base route !route_name, service !service, has no endpoint !endpoint, check RESTmini Service settings page.',
                  array_merge($replacersStep, array('!route_name' => $baseRouteName, '!service' => $serviceName, '!endpoint' => $endpointName)),
                  'warning'
                );
                return FALSE;
              }
              $endpointPath = $servicePath . '/' . $endpointName;
              if (empty($this->resultData[$baseRouteName][$serviceName][$endpointName])) {
                $this->resultData[$baseRouteName][$serviceName][$endpointName] = array();
              }
              $this->testResults[$step][$baseRouteName][$serviceName][$endpointName] = array();

              // Extend reusable message, for endpoint.
              $messageEndpoint = $messageStep . ' - endpoint !route_name:!service:!endpoint';
              $replacersEndpoint = array_merge(
                $replacersStep,
                array('!route_name' => $baseRouteName, '!service' => $serviceName, '!endpoint' => $endpointName)
              );

              $nRequests = count($requests);
              for ($requestIndex = 0; $requestIndex < $nRequests; ++$requestIndex) {
                $requestProps = $requests[$requestIndex];

                $this->testResults[$step][$baseRouteName][$serviceName][$endpointName][$requestIndex] = array(
                  'method' => '',
                  'operation' => '',
                  // success: 0 ~ unconditional failure | 1 ~ failed assertion | 2 ~ inconclusive | 3 ~ unconditional success.
                  'success' => 0,
                  'reason' => '',
                );

                // The 'method' property is required.
                if (empty($requestProps['method'])) {
                  static::message(
                    $messageEndpoint . ', request index !request_index misses \'method\' bucket.',
                    array_merge($replacersEndpoint, array('!request_index' => $requestIndex)),
                    'error'
                  );
                  return FALSE;
                }
                $method = $requestProps['method'];

                $this->testResults[$step][$baseRouteName][$serviceName][$endpointName][$requestIndex]['method'] = $method;

                // All other properties are optional.
                switch ($method) {
                  case 'HEAD':
                  case 'GET':
                    if (empty($requestProps['operation']) || ($requestProps['operation'] != 'index' && $requestProps['operation'] != 'retrieve')) {
                      $operation = empty($requestProps['parameters']['path']) ? 'index' : 'retrieve';
                    }
                    else {
                      $operation = $requestProps['operation'];
                    }
                    break;
                  case 'POST':
                    $operation = 'create';
                    break;
                  case 'PUT':
                    $operation = 'update';
                    break;
                  case 'DELETE':
                    $operation = 'delete';
                    break;
                  default:
                    static::message(
                      $messageEndpoint . ', request index !request_index, method !method not supported.',
                      array_merge($replacersEndpoint, array('!request_index' => $requestIndex, '!method' => $method)),
                      'error'
                    );
                    return FALSE;
                }
                if (empty($this->resultData[$baseRouteName][$serviceName][$endpointName][$operation])) {
                  $this->resultData[$baseRouteName][$serviceName][$endpointName][$operation] = array();
                }
                $this->testResults[$step][$baseRouteName][$serviceName][$endpointName][$requestIndex]['operation'] = $operation;

                // Extend reusable message, for operation.
                $messageOperation = $messageEndpoint . ' - operation !request_index:!method:!operation';
                $replacersOperation = array_merge(
                  $replacersEndpoint,
                  array('!request_index' => $requestIndex, '!method' => $method, '!operation' => $operation)
                );

                $assertStatus = empty($requestProps['assert_status']) ? 0 : $requestProps['assert_status'];

                $stopOnFailure = !empty($requestProps['stop_on_failure']);

                $parameters = array(
                  'path' => array(),
                  'get' => array(),
                  'post' => array(),
                );
                if (!empty($requestProps['parameters'])) {
                  $paramTypes = array('path', 'get', 'post');
                  foreach ($paramTypes as $paramType) {
                    if (!empty($requestProps['parameters'][$paramType])) {
                      $parameters[$paramType] = $requestProps['parameters'][$paramType];
                      try {
                        $this->prepareArguments($parameters[$paramType]);
                      }
                      catch (Exception $xc) {
                        static::$resultSummary[-1] += 1;
                        $this->testResults[$step][$baseRouteName][$serviceName][$endpointName][$requestIndex]['reason'] =
                          'failed to prepare ' . $paramType . ' parameters';
                        static::message(
                          $messageOperation . ', failed to prepare !arg_type parameters.',
                          array_merge($replacersOperation, array('!arg_type' => $paramType)),
                          'error'
                        );
                        static::inspect(
                          array(
                            'test' => $this->listPublicAttributes(/*array('results')*/),
                            'argument type' => $paramType,
                            'argument buckets' => $parameters[$paramType],
                          ),
                          array(
                            'category' => 'restmini_service_test',
                            'message' => 'Failed to find a value of a previous request of current test',
                            'severity' => WATCHDOG_ERROR,
                            'depth' => 20,
                          )
                        );
                        return FALSE;
                      }
                    }
                  }
                }

                if (empty($requestProps['client_class'])) {
                  $clientClass = 'RestMiniClient';
                }
                else {
                  $clientClass = $requestProps['client_class'];
                  if (!class_exists($clientClass)) {
                    static::$resultSummary[-1] += 1;
                    $this->testResults[$step][$baseRouteName][$serviceName][$endpointName][$requestIndex]['reason'] =
                      'no client class ' . $clientClass;
                    static::message(
                      $messageOperation . ', client class !client doesn\'t exist or isn\'t included.',
                      array_merge($replacersOperation, array('!client' => $clientClass)),
                      'error'
                    );
                    return FALSE;
                  }
                }

                $options = array_merge(
                  static::$clientOptions,
                  !empty($requestProps['client_options']) ? $requestProps['client_options'] : array(),
                  !static::$responseInfo ? array() : array(
                    // Send X-Rest-Service-Response-Info-Wrapper header.
                    'service_response_info_wrapper' => TRUE,
                    'get_headers' => TRUE,
                  )
                );

                if ($clientClass == 'RestMiniClient') {
                  $client = RestMiniClient::make(
                    static::$server,
                    $endpointPath,
                    $options
                  );
                }
                else {
                  $client = call_user_func($clientClass . '::make', static::$server, $endpointPath, $options);
                }

                $client->request($method, $parameters['path'], $parameters['get'], $parameters['post']);

                $clientInfo = $client->info();

                $this->testResults[$step][$baseRouteName][$serviceName][$endpointName][$requestIndex]['status'] = $clientInfo['status'];
                $this->testResults[$step][$baseRouteName][$serviceName][$endpointName][$requestIndex]['content_type'] = $clientInfo['content_type'];
                $this->testResults[$step][$baseRouteName][$serviceName][$endpointName][$requestIndex]['content_length'] = $clientInfo['content_length'];

                // Reduce inspection depth when result payload is part of the stuff to inspect.
                $inspectDepth = 6;
                if (($contentLength = $clientInfo['content_length']) && $contentLength > (static::$inspectOutputMax / 4)) {
                  if ($contentLength > static::$inspectOutputMax) {
                    $inspectDepth = 2;
                  }
                  elseif ($contentLength > (static::$inspectOutputMax / 2)) {
                    $inspectDepth = 3;
                  }
                  elseif ($contentLength > (static::$inspectOutputMax / 3)) {
                    $inspectDepth = 4;
                  }
                  else {
                    $inspectDepth = 5;
                  }
                }
                // Add two layers ['result']['payload'] if response_info.
                if (static::$responseInfo) {
                  $inspectDepth += 1;
                }
                if (!empty($clientInfo['headers']['X-Rest-Service-Response-Info-Wrapper'])) {
                  $inspectDepth += 1;
                }

                $failed = FALSE;

                // Check for request error.
                if (($clientError = $client->error())) {
                  $failed = TRUE;
                  static::$resultSummary[0] += 1;
                  $this->resultData[$baseRouteName][$serviceName][$endpointName][$operation][] = NULL;
                  $this->testResults[$step][$baseRouteName][$serviceName][$endpointName][$requestIndex]['reason'] =
                    'error ' . join(':', $clientError);
                  static::message(
                    $messageOperation . ', client class !client, request failed, error @error.',
                    array_merge($replacersOperation, array('!client' => $clientClass, '@error' => join(':', $clientError))),
                    'warning'
                  );

                  static::inspect(
                    array(
                      'test result' => 'failure (0)',
                      'parameters' => $parameters,
                      'client info' => $clientInfo,
                    ),
                    array(
                      'category' => 'restmini_service_test',
                      'message' => $baseRouteName . '__' . $serviceName . '__' . $endpointName . '__' . $operation . ' - failed request',
                      'severity' => WATCHDOG_WARNING,
                    )
                  );
                }
                else {
                  $result = !static::$responseInfo ? $client->result() : $client->result(array(), TRUE);

                  // Check for parsing error.
                  if (($clientError = $client->error())) {
                    $failed = TRUE;
                    static::$resultSummary[0] += 1;
                    $this->resultData[$baseRouteName][$serviceName][$endpointName][$operation][] = NULL;
                    $this->testResults[$step][$baseRouteName][$serviceName][$endpointName][$requestIndex]['reason'] =
                      'error ' . join(':', $clientError);
                    static::message(
                      $messageOperation . ', client class !client, failed to parse result, error @error.',
                      array_merge($replacersOperation, array('!client' => $clientClass, '@error' => join(':', $clientError))),
                      'warning'
                    );
                    static::inspect(
                      array(
                        'test result' => 'failure (0)',
                        'parameters' => $parameters,
                        'client info' => $clientInfo,
                        'response' => $client->raw(),
                      ),
                      array(
                        'category' => 'restmini_service_test',
                        'message' => $baseRouteName . '__' . $serviceName . '__' . $endpointName . '__' . $operation . ' - failed parsing result',
                        'severity' => WATCHDOG_WARNING,
                      )
                    );
                  }
                  // Check response status against asserted status.
                  elseif ($assertStatus && $clientInfo['status'] != $assertStatus) {
                    $failed = TRUE;
                    $this->resultData[$baseRouteName][$serviceName][$endpointName][$operation][] = NULL;
                    // test success: 1 ~ failed assertion, requires human evaluation.
                    static::$resultSummary[1] += 1;
                    $this->testResults[$step][$baseRouteName][$serviceName][$endpointName][$requestIndex]['success'] = 1;
                    $this->testResults[$step][$baseRouteName][$serviceName][$endpointName][$requestIndex]['reason'] =
                      'expected assert status ' . $assertStatus . ' but received status ' . $clientInfo['status'];
                    static::message(
                      $messageOperation . ', client class !client, expected assert status !assert_status but received status !status.',
                      array_merge($replacersOperation,
                        array('!client' => $clientClass, '!assert_status' => $assertStatus, '!status' => $clientInfo['status'])),
                      'warning'
                    );
                    static::inspect(
                      array(
                        'test result' => 'failed assertion (1)',
                        'parameters' => $parameters,
                        'client info' => $clientInfo,
                        'result' => $result,
                      ),
                      array(
                        'category' => 'restmini_service_test',
                        'message' => $baseRouteName . '__' . $serviceName . '__' . $endpointName . '__' . $operation . ' - failed assertion',
                        'severity' => WATCHDOG_NOTICE,
                        // Decreased inspection depth - the result may be large.
                        'depth' => $inspectDepth,
                      )
                    );
                  }
                  else {
                    // Save result data for later.
                    $data = $result;
                    if (static::$responseInfo) {
                      $data = is_array($result['result']) && array_key_exists('payload', $result['result']) ? $result['result']['payload'] :
                        $result['result'];
                    }
                    $this->resultData[$baseRouteName][$serviceName][$endpointName][$operation][] = $data;

                    // Check for generally (standard-wise) unsuccessful status.
                    if (!static::statusSuccess($method, $clientInfo['status'], $clientInfo['content_length'])) {
                      $failed = TRUE;
                      // test success: 2 ~ inconclusive, requires human evaluation.
                      static::$resultSummary[2] += 1;
                      $this->testResults[$step][$baseRouteName][$serviceName][$endpointName][$requestIndex]['success'] = 2;
                      $this->testResults[$step][$baseRouteName][$serviceName][$endpointName][$requestIndex]['reason'] =
                        'method ' . $method . ' status ' . $clientInfo['status'] . ' content-length ' . $clientInfo['content_length'];

                      static::message(
                        $messageOperation . ', client class !client, status @status content-length @content_length'
                        . ' is either decidedly unsuccessful or doesn\'t comply strictly with REST standards.',
                        array_merge($replacersOperation,
                          array('!client' => $clientClass, '@status' => $clientInfo['status'], '@content_length' => $clientInfo['content_length'])),
                        'warning'
                      );
                    }
                    else {
                      // test success: 3 ~ unconditional success.
                      static::$resultSummary[3] += 1;
                      $this->testResults[$step][$baseRouteName][$serviceName][$endpointName][$requestIndex]['success'] = 3;
                    }
                    $this->testResults[$step][$baseRouteName][$serviceName][$endpointName][$requestIndex]['result'] =
                      drupal_substr($client->raw(), 0, 100);

                    if ($failed || static::$responseInfo) {
                      static::inspect(
                        array(
                          'test result' => $failed ? 'inconclusive (2)' : 'success (3)',
                          'parameters' => $parameters,
                          'client info' => $clientInfo,
                          'result' => $result,
                        ),
                        array(
                          'category' => 'restmini_service_test',
                          'message' => $baseRouteName . '__' . $serviceName . '__' . $endpointName . '__' . $operation
                            . ($failed ? ' - failed assertion' : ' - succeeded'),
                          'severity' => $failed ? WATCHDOG_NOTICE : WATCHDOG_INFO,
                          // Decreased inspection depth - the result may be large.
                          'depth' => $inspectDepth,
                        )
                      );
                    }
                    else {
                      static::inspect(
                        array(
                          'test result' => 'success (3)',
                          'status' => $clientInfo['status'],
                          'content type' => $clientInfo['content_type'],
                          'content length' => $clientInfo['content_length'],
                        ),
                        array(
                          'category' => 'restmini_service_test',
                          'message' => $baseRouteName . '__' . $serviceName . '__' . $endpointName . '__' . $operation . ' - succeeded',
                          'severity' => WATCHDOG_INFO,
                          // Decreased inspection depth - the result may be large.
                          'depth' => $inspectDepth,
                        )
                      );
                    }
                  }
                }

                if ($failed && $stopOnFailure) {
                  static::message(
                    $messageStep . ', stopped because last request failed and it has the \'stop_on_failure\' flag.',
                    $replacersStep,
                    'warning'
                  );
                  return FALSE;
                }
              }
            }
          }
        }
      }

      return TRUE;
    }
    catch (Exception $xc) {
      static::log(
        'restmini_service_test',
        'Something unexpected happened',
        $xc,
        NULL,
        WATCHDOG_ERROR
      );
    }
    return FALSE;
  }


  /**
   * Drush/CLI mode?
   *
   * @var boolean
   */
  protected static $isCli = FALSE;

  /**
   * @var integer
   */
  protected static $inspectOutputMax;

  /**
   * @var string
   */
  protected static $outputTarget = 'print';

  /**
   * @var boolean
   */
  protected static $responseInfo = FALSE;

  /**
   * @var string
   */
  protected static $server;
  /**
   * @var array
   */
  protected static $clientOptions = array();

  /**
   * @var array
   */
  protected static $baseRoutes;

  /**
   * @var array
   */
  protected static $registry;

  /**
   * @var array
   */
  protected static $results = array();

  /**
   * @var array
   */
  protected static $resultSummary = array(
    -1 => 0, // -1: execution failure.
    0 => 0, // 0: failed request.
    1 => 0, // 1: failed assertion.
    2 => 0, // 2: inconclusive.
    3 => 0, // 3: successfull request.
  );

  /**
   * @return array
   */
  public static function listAvailableTests() {
    $tests = array();
    $modules = module_implements('restmini_service_test');
    if (!$modules) {
      static::message('No RESTmini Service tests available, no enabled module implements hook_restmini_service_test().', array(), 'warning');
      return $tests;
    }
    foreach ($modules as $module) {
      $function = $module . '_restmini_service_test';
      $tests[$module] = $function();
    }
    return $tests;
  }

  /**
   * Execute test(s) against service endpoints.
   *
   *  Options:
   *  - (str) output_target: print|watchdog|file (default: print)
   *  - (bool|int) response_info: tells the REST client to wrap response payload in client and service info wrappers (default: not)
   *  - (bool|int) force_complete: a RestMiniClient option (default: not defined)
   *  - (bool|int) ssl_verify: a RestMiniClient option (default: not defined)
   *  - (int) connect_timeout: RestMiniClient option (default: see RestMiniClient)
   *  - (int) request_timeout: RestMiniClient option (default: see RestMiniClient)
   *  - (str) user: a RestMiniClient option, ignored if no pass (default: none)
   *  - (str) pass: a RestMiniClient option, ignored if no user (default: none)
   *
   * @see hook_restmini_service_test()
   * @see restmini_service_example_test_restmini_service_test()
   * @see RestMiniClient::make()
   *
   * @param string $server
   *   Protocol + domain (~ http://ser.ver).
   *   Prepends http:// if no protocol (only http and https supported).
   *   Trailing slash will be removed.
   * @param string|array $tests
   *   String 'all': all test modules and all their tests.
   *   Array: list of test modules. [module]: true means all tests of that module; [module]: array of names means those tests of that module.
   * @param array $options
   *   Default: empty.
   */
  public static function executeSelected($server, $tests, $options = array()) {
    static::$results = array();
    static::$isCli = $isCli = drupal_is_cli();
    static::$inspectOutputMax = Inspect::outputMax();

    // Establish output target.
    if ($options) {
      if (!empty($options['output_target'])) {
        switch ($options['output_target']) {
          case 'print':
          case 'get': // ~ Inspect equivalent.
            static::$outputTarget = 'print';
            break;
          case 'watchdog':
          case 'log': // ~ Inspect equivalent.
            static::$outputTarget = 'watchdog';
            break;
          case 'file':
            static::$outputTarget = 'file';
            break;
          default:
            static::message('Option output_target !output_target is not print|watchdog|file, aborted testing.', array('!output_target' => $options['output_target']), 'error');
            return;
        }
      }
    }

    // Don't make enormous inspections unless output target is file.
    if (static::$outputTarget != 'file' && static::$inspectOutputMax > 1048576) {
      static::$inspectOutputMax = 1048576;
    }

    if (!empty($options['response_info'])) {
      static::$responseInfo = TRUE;
    }

    // RestMiniClient options.
    if (array_key_exists('ssl_verify', $options)) {
      static::$clientOptions['ssl_verify'] = !!$options['ssl_verify'];
    }
    if (array_key_exists('status_vain_result_void', $options)) {
      static::$clientOptions['status_vain_result_void'] = !!$options['status_vain_result_void'];
    }
    if (!empty($options['ssl_cacert_file'])) {
      static::$clientOptions['ssl_cacert_file'] = $options['ssl_cacert_file'];
    }
    if (!empty($options['connect_timeout'])) {
      static::$clientOptions['connect_timeout'] = $options['connect_timeout'];
    }
    if (!empty($options['request_timeout'])) {
      static::$clientOptions['request_timeout'] = $options['request_timeout'];
    }
    if (!empty($options['user']) && !empty($options['pass'])) {
      static::$clientOptions['user'] = $options['user'];
      static::$clientOptions['pass'] = $options['pass'];
    }

    // Don't wait forever giving user feedback.
    static::message('RESTmini Service tests initiated:');
    static::message('- output target: !output_target', array('!output_target' => static::$outputTarget));

    // Check module dependencies.
    $abort = FALSE;
    $dependencies = array('restmini_service', 'restmini_client', 'inspect');
    foreach ($dependencies as $module) {
      if (!module_exists($module)) {
        $abort = TRUE;
        static::message(
          'This module, restmini_service_test, depends on module !module - which either doesn\'t exist or isn\'t enabled.',
          array('!module' => $module),
          'error'
        );
      }
    }

    // Validate arg $server.
    if (!preg_match('/https?:\/\/[^\/]+\/?/', $server)) {
      $abort = TRUE;
      static::message(
        'The server argument must be \'[protocol]://domain\' (like \'https://domain.tld\'), saw @server.',
        array('@server' => $server),
        'error'
      );
    }
    static::$server = $server;

    // Establish tests.
    if (!$tests || ($tests !== 'all' && !is_array($tests))) {
      $abort = TRUE;
      static::message(
        'The tests argument must be \'all\' or array of test modules, [module]:true means all tests of that module, [module]:array of names means those tests of that module, saw @tests.',
        array('@tests' => $tests),
        'error'
      );
    }

    // Check that user is allowed to output to current output_target via Inspect.
    switch (static::$outputTarget) {
      case 'print':
        if (!Inspect::permit('get')) {
          $abort = TRUE;
          static::message('Current user is not allowed to !perm inspections, please check Inspect permissions.', array('!perm' => 'get'), 'error');
        }
        break;
      case 'watchdog':
        if (!Inspect::permit('log')) {
          $abort = TRUE;
          static::message('Current user is not allowed to !perm inspections, please check Inspect permissions.', array('!perm' => 'log'), 'error');
        }
        break;
      case 'file':
        if (!Inspect::permit('file')) {
          $abort = TRUE;
          static::message('Current user is not allowed to !perm inspections, please check Inspect permissions.', array('!perm' => 'file'), 'error');
        }
        break;
    }

    if (!(static::$baseRoutes = restmini_service_base_routes())) {
      $abort = TRUE;
      static::message('This site has no base routes defined, consider clearing cache.', array(), 'warning');
    }
    if (!(static::$registry = RestMiniService::registry())) {
      $abort = TRUE;
      static::message('This site has no registered services endpoint methods, consider clearing cache.', array(), 'warning');
    }

    // List available tests and list of selected tests to be executed.
    $testsAvailable = array();
    $testsSelected = array();
    $nAvailable = $nSelected = $nCompleted = 0;
    if (!$abort) {
      // List available tests.
      $modules = module_implements('restmini_service_test');
      if (!$modules) {
        $abort = TRUE;
        static::message('No RESTmini Service tests available, no enabled module implements hook_restmini_service_test().', array(), 'warning');
      }
      else {
        foreach ($modules as $module) {
          $function = $module . '_restmini_service_test';
          $nAvailable += count($testsAvailable[$module] = array_keys($function()));
        }

        // Make list of selected tests to be executed.
        if ($tests === 'all') {
          $testsSelected = $testsAvailable;
          $nSelected = $nAvailable;
        }
        else {
          foreach ($tests as $module => $names) {
            // Check that the selected test module exists and declares test(s).
            if (empty($testsAvailable[$module])) {
              if (!array_key_exists($module, $testsAvailable)) {
                if (!module_exists($module)) {
                  static::message('RESTmini Service test module !module doesn\'t exist or isn\'t enabled.', array('!module' => $module), 'warning');
                }
                else {
                  static::message('RESTmini Service test module !module declares no tests, doesn\'t implement hook_restmini_service_test().',
                    array('!module' => $module), 'warning');
                }
              }
              else {
                static::message('RESTmini Service test module !module declares no tests in it\'s hook_restmini_service_test() implementation.',
                  array('!module' => $module), 'warning');
              }
              // Abort or continue to next selected module.
              if (!empty($options['force_complete'])) {
                continue;
              }
              $abort = TRUE;
              break;
            }

            if ($names === TRUE) {
              $nSelected += count($testsSelected[$module] = $testsAvailable[$module]);
            }
            else {
              $testsSelected[$module] = array();
              foreach ($names as $name) {
                if (in_array($name, $testsAvailable[$module])) {
                  $testsSelected[$module][] = $name;
                  ++$nSelected;
                }
                else {
                  static::message('RESTmini Service test module !module declares no test !name in it\'s hook_restmini_service_test() implementation.',
                    array('!module' => $module, '!name' => $name), 'warning');
                  // Abort or continue to next selected test.
                  if (!empty($options['force_complete'])) {
                    continue;
                  }
                  $abort = TRUE;
                  break 2;
                }
              }
            }
          }
        }
      }
    }

    // Get out if any failure.
    if ($abort) {
      static::message('RESTmini Service Test aborted all testing.', array(), 'warning');
      return;
    }

    foreach ($testsSelected as $module => $names) {
      $function = $module . '_restmini_service_test';
      $moduleTests = $function();

      static::$results[$module] = array();

      foreach ($names as $name) {
        /** @var RestMiniServiceTest $test */
        $test = new static($module, $name, $moduleTests[$name]);
        if ($test->execute()) {
          static::message('RESTmini Service test !module:!test succeeded.', array('!module' => $module, '!test' => $name), 'success');
        }
        else {
          ++$nCompleted;
          static::message('RESTmini Service test !module:!test failed.', array('!module' => $module, '!test' => $name), 'warning');
        }
      }
    }

    static::inspect(
      array(
        'success (3)' => static::$resultSummary[3],
        'inconclusive (2)' => static::$resultSummary[2],
        'response failed assertion (1)' => static::$resultSummary[1],
        'request failure (0)' => static::$resultSummary[0],
        'test configuration/execution failure (-1)' => static::$resultSummary[-1],
        'detailed info' => self::resultDetailedInfoKeyLabel(static::$results),
      ),
      array(
        'category' => 'restmini_service_test',
        'message' => 'Test results',
        'severity' => WATCHDOG_NOTICE,
      )
    );

    static::message('RESTmini Service tests completed !completed test of !selected selected tests (available: !available).',
      array('!available' => $nAvailable, '!selected' => $nSelected, '!completed' => $nCompleted), $nCompleted < $nSelected ? 'warning' : 'ok');
  }

  /**
   * @param array $results
   * @param integer $depth
   *
   * @return array
   */
  protected static function resultDetailedInfoKeyLabel($results, $depth = 0) {
    $prefixedKeys = array();
    foreach ($results as $key => $value) {
      switch ($depth) {
        case 0:
          $prefix = 'module: ';
          break;
        case 1:
          $prefix = 'test: ';
          break;
        case 2:
          $prefix = 'step: ';
          break;
        case 3:
          $prefix = 'base_route: ';
          break;
        case 4:
          $prefix = 'service: ';
          break;
        case 5:
          $prefix = 'endpoint: ';
          break;
        case 6:
          $prefix = 'request_index: ';
          break;
        default:
          $prefix = '';
      }
      $prefixedKeys[$prefix . $key] = !is_array($value) ? $value : static::resultDetailedInfoKeyLabel($value, $depth + 1);
    }
    return $prefixedKeys;
  }


  /**
   * Prints via drush_log() or drupal_set_message().
   *
   * @param string $message
   * @param array $replacers
   *   Translation replacers.
   *   Default: empty.
   * @param string $type
   *   Values: status|notice|warning|error|ok|success
   *   Default: status.
   */
  protected static function message($message, $replacers = array(), $type = 'status') {
    if (static::$isCli) {
      drush_log(str_replace('!nl', "\n", str_replace(array_keys($replacers), array_values($replacers), $message)) . "\n", $type);
    }
    else {
      drupal_set_message(str_replace('!nl', '<br/>', format_string($message, $replacers)), $type);
    }
  }

  /**
   * Sends var to inspection.
   *
   * @see inspect()
   *
   * @param mixed $var
   * @param mixed $options
   *   See inspect() $options.
   *   Default: NULL.
   */
  protected static function inspect($var, $options = array()) {
    // This method wraps inspect(); remove one stack frame when finding file:line.
    $options['wrappers'] = 1;

    $options['no_fileline'] = TRUE;
    $options['output_max'] = static::$inspectOutputMax;
    $options['truncate'] = 255;

    switch (static::$outputTarget) {
      case 'print':
        echo inspect_get($var, $options) . "\n";
        break;
      case 'file':
        inspect_file($var, $options);
        break;
      default: // watchdog.
        inspect($var, $options);
    }
  }

}

<?php
/**
 * @file
 * Drupal RestMini Client Test module.
 */

/**
 * @return array
 */
function restmini_client_test_drush_command() {
  return array(
    'restmini-client-test' => array(
      'description' => 'Test RESTmini Client.',
      'arguments' => array(
        'server' => 'protocol://domain.tld',
        'tests' => 'some_test,other_test',
      ),
      'options' => array(
        'response-info' => '(int) Tell the REST client to wrap response payload in info wrapper (default: 0).',
        'ssl-verify' => '(int) RestMiniClient option: Turn SSL certificate check on/off (default: 1).',
        'connect-timeout' => '(int) RestMiniClient option (default: see RestMiniClient).',
        'request-timeout' => '(int) RestMiniClient option (default: see RestMiniClient).',
        'user' => '(str) RestMiniClient option: Username for HTTP authentication (default: empty).',
        'pass' => '(str) RestMiniClient option: Password for HTTP authentication (default: empty).',
      ),
      'examples' => array(
        "drush restmini-client-test 'https://ser.ver' 'some_test,other_test' --response-info" => ' ',
      ),
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    )
  );
}

/**
 * @param string $server
 *   Format: 'protocol://domain.tld'
 * @param string $tests
 *   Comma-separated list of test names.
 */
function drush_restmini_client_test($server, $tests) {
  // Module name is function - drush_.
  $module = substr(__FUNCTION__, 6);

  // Check that we are in CLI mode.
  if (!drupal_is_cli()) {
    drush_log('Function ' . __FUNCTION__ . ' is only callable from drush/cli.', 'error');
    return;
  }
  // Check that this module is enabled.
  if (!module_exists($module)) {
    drush_log('This service test module[' . $module . '] isnt enabled.', 'error');
    return;
  }
  // Check args.
  if (!$server) {
    drush_log('Argument server is empty.', 'error');
    return;
  }
  if (!$tests) {
    drush_log('Argument tests is empty.', 'error');
    return;
  }
  $test_names = explode(',', $tests);

  $options = array();
  if (drush_get_option('response-info')) {
    $options['response_info'] = TRUE;
  }
  if (($v = drush_get_option('ssl-verify', NULL)) !== NULL) {
    $options['ssl_verify'] = !!$v;
  }
  if (($v = drush_get_option('connect-timeout', NULL))) {
    $options['connect_timeout'] = $v;
  }
  if (($v = drush_get_option('request-timeout', NULL))) {
    $options['request_timeout'] = $v;
  }
  if (($v = drush_get_option('user', NULL))) {
    $options['user'] = $v;
  }
  if (($v = drush_get_option('pass', NULL))) {
    $options['pass'] = $v;
  }

  $test = 'ssl';
  if (in_array($test, $test_names)) {
    $endpoint = '';
    $method = 'GET';
    $params = array(
      'path' => array(
        'robots.txt',
      ),
      'get' => array(
      ),
      'post' => array(
      ),
    );

    $client = RestMiniClient::make(
      $server,
      $endpoint,
      array_merge(
        $options,
        array(
          'headers' => array(
            //'Accept' => 'text/html',
            'Accept' => 'text/plain',
          ),
        )
      )
    );
    $client->request($method, $params['path'], $params['get'], $params['post']);
    inspect(
      array(
        'info' => $client->info(),
        'raw' => $client->raw(),
      ),
      $test
    );
  }

  drush_log('Sent REST requests, please check watchdog logs.', 'success');
}

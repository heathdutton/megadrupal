<?php
/**
 * @file
 * Drupal RestMini Service Test module.
 */

/**
 * @return array
 */
function restmini_service_test_drush_command() {
  return array(
    'restmini-service-test' => array(
      'description' => 'Test RESTmini Service endpoint methods.',
      'arguments' => array(
        'server' => 'protocol://domain.tld',
        'tests' => 'some_test_module,other_test_module:test_name',
      ),
      'options' => array(
        'output-target' => '(str) Output target: print|watchdog|file (default: print).',
        'response-info' => '(int) Tell the REST client to wrap response payload in client and service info wrappers (default: 0).',
        'force-complete' => '(int) Skip tests that aren\'t available instead of aborting all tests (default: 0).',
        'connect-timeout' => '(int) RestMiniClient option (default: see RestMiniClient).',
        'request-timeout' => '(int) RestMiniClient option (default: see RestMiniClient).',
        'ssl-verify' => '(int) RestMiniClient option: Turn SSL certificate check on/off (default: 1).',
        'ssl-cacert-file' => '(str) RestMiniClient option: Use custom CA cert file instead the common file.',
        'status-vain-result-void' => '(int) RestMiniClient option: result() returns empty string if status >=300.',
        'user' => '(str) RestMiniClient option: Username for HTTP authentication (default: empty).',
        'pass' => '(str) RestMiniClient option: Password for HTTP authentication (default: empty).',
      ),
      'examples' => array(
        "drush restmini-service-test 'https://ser.ver' 'all' --output-target=watchdog" => ' ',
        "drush restmini-service-test 'https://ser.ver' 'restmini_service_example_test' --output-target=watchdog" => ' ',
        "drush restmini-service-test 'https://ser.ver' 'some_test_module,other_test_module:test_name' --response-info --ssl-verify=0" => ' ',
        "drush restmini-service-test 'https://ser.ver' 'some_test_module,other_test_module:some_test,other_test_module:other_test' --user='someuser' --pass='somepassword'" => ' ',
      ),
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    )
  );
}

/**
 * @param string $server
 *   'protocol://domain.tld'.
 * @param string $tests
 *   Comma-separated list of test modules (optionally with specific :test_name appendix), or simply 'all'.
 */
function drush_restmini_service_test($server, $tests) {
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

  // Check/get args.
  if (!$server) {
    drush_log('Argument server is empty.', 'error');
    return;
  }
  if (!$tests) {
    drush_log('Argument tests is empty.', 'error');
    return;
  }
  if ($tests !== 'all') {
    $list = array();
    $le = count($arr = explode(',', $tests));
    for ($i = 0; $i < $le; ++$i) {
      $moduleTest = explode(':', $arr[$i]);
      $module = $moduleTest[0];
      if (!array_key_exists($module, $list)) {
        $list[$module] = array();
      }
      // If [module]:[test]
      if (count($moduleTest) > 1) {
        $list[$module][] = $moduleTest[1];
      }
    }
    if ($list) {
      $tests = array();
      foreach ($list as $module => $names) {
        // If no specific names: flag 'all' with TRUE.
        $tests[$module] = !$names ? TRUE : $names;
      }
    }
    unset($list, $arr, $moduleTest, $module);
  }

  // Get/check options.
  $options = array(
    'output_target' => drush_get_option('output-target', 'print'),
  );
  if (drush_get_option('response-info')) {
    $options['response_info'] = TRUE;
  }
  if (drush_get_option('force-complete')) {
    $options['force_complete'] = TRUE;
  }
  if (($v = drush_get_option('ssl-verify', NULL)) !== NULL) {
    $options['ssl_verify'] = !!$v;
  }
  if (($v = drush_get_option('status-vain-result-void', NULL)) !== NULL) {
    $options['status_vain_result_void'] = !!$v;
  }
  if (($v = drush_get_option('ssl-cacert-file', NULL))) {
    $options['ssl_cacert_file'] = $v;
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

  restmini_service_test($server, $tests, $options);
}

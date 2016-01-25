<?php
/**
 * @file
 *   Checks the status of a Drupal site without having any module installed on the site.
 */

// Here we match the nagios/sensu check pattern.
define('CHECK_OK', 0);
define('CHECK_WARNING', 1);
define('CHECK_CRITICAL', 2);
define('CHECK_UNKNOWN', 3);

/**
 * Implements hook_drush_help().
 */
function check_drush_help($section) {
  switch ($section) {
    case 'meta:check:title':
      return dt('check commands');
    case 'meta:check:summary':
      return dt('A drush extension for providing Nagios/Sensu compatible checks on Drupal sites for status without installing anything on each individual site.');
  }
}

/**
 * Implements hook_drush_command().
 */
function check_drush_command() {
  $items = array();

  $items['check'] = array(
    'description' => 'Perform a Nagios/Sensu compatible check on Drupal site status.',
    'bootstrap' => DRUSH_BOOTSTRAP_MAX,
    'options' => array(
      'all'     => 'Include all checks regardless of whether they usually run by default.',
      'exclude' => 'A comma separated list of checks to explicitly exclude.',
      'cron-max-time-warning' => 'The maximum time to allow cron not to run without a warning specified in minutes.',
      'cron-max-time-critical' => 'The maximum time to allow cron not to run wihtout a critical error specified in minutes.',
      'requirements-warn-level' => 'The level of requirements failures that should trigger a warning (defaults to 2).',
      'requirements-critical-level' => 'The level of requirements failures that should trigger a critical error (defaults to 3).',
    ),
  );

  $items['check-list'] = array(
    'description' => 'List all defined checks.',
    'bootstrap' => DRUSH_BOOTSTRAP_MAX,
  );

  return $items;
}

/**
 * Implements hook_check_info();
 */
function check_check_info() {
  return array(
    'cron' => array(
      'run_by_default' => true,
      'callback' => '_drush_check_last_cron',
    ),
    'security-updates' => array(
      'callback' => '_drush_check_security_updates',
    ),
    'core-requirements' => array(
      'callback' => '_drush_check_requirements',
    ),
  );
}

/**
 * Drush command callback for checking the status of a site.
 *
 * @param $only_defaults
 *   Whether to list only those checks configured to run by default.
 */
function check_get_checks() {
  $checks = drush_command_invoke_all('check_info');
  drush_command_invoke_all_ref('check_info_alter', $checks);
  if (function_exists('module_invoke_all')) {
    $checks = array_merge($checks, module_invoke_all('check_info'));
  }
  return $checks;
}

function drush_check_list() {
  $checks = array(
    array(
      'Title',
      'Function',
      'Run by default',
    ),
  );
  foreach (check_get_checks() as $name => $definition) {
    $checks[] = array(
      $name,
      $definition['callback'],
      !empty($definition['run_by_default']) ? 'Yes' : 'No',
    );
  }
  drush_print_table($checks);
}

/**
 * Run a set of checks by name.
 */
function check_run_checks($checks) {
  // All data about failed checks.
  $failures = array();
  // A linear array of all failed checks.
  $failed_checks = array();

  $exit_code = 0;
  foreach ($checks as $check => $definiton) {
    if (!empty($definition['file'])) {
      if (is_file($definition['file'])) {
        require($definition['file']);
      }
      else {
        $variables = array(
          '@file' => $definition['file']
        );
        drush_log(dt('The file @file was provided in a check definition but could not be required', $variables), 'error');
      }
    }
    drush_log(dt('Running check @check', array('@check' => $check)));
    $check_data = call_user_func_array($definiton['callback'], array());

    // Checks can return nothing if all is well.
    if (empty($check_data)) {
      continue;
    }

    // Raise our exit code to the highest encountered number.
    if ($check_data['status'] > $exit_code) {
      $exit_code = $check_data['status'];
    }
    if ($check_data['status'] !== CHECK_OK) {
      $failed_checks[] = $check;
      $failures[$check_data['status']][] = $check_data;
    }
  }
  return array(
    'failed_checks' => $failed_checks,
    'failures' => $failures,
    'exit_code' => $exit_code,
  );
}

/**
 * Drush command callback for checking the status of a site.
 */
function drush_check() {
  if (drush_get_context('DRUSH_BOOTSTRAP_PHASE') < 6) {
    drush_log('Error bootstrapping Drupal!', 'error');
    drush_set_context('DRUSH_EXIT_CODE', 2);
    return;
  }

  $checks = check_get_checks();

  // Filter to only checks that should run by default.
  foreach ($checks as $name => $check) {
    if (!drush_get_option('all', FALSE) && empty($check['run_by_default'])) {
      unset($checks[$name]);
    }
  }

  // If arguments a provided consider that a whitelist of checks to run.
  $arguments = drush_get_arguments();

  // The first argument is the `check` command.
  array_shift($arguments);

  if (!empty($arguments)) {
    $checks = array_intersect_key($checks, array_flip($arguments));
  }

  // Exclude any checks explicitly suppressed with the --exclude option.
  $checks = array_diff_key($checks, array_flip(explode(',', drush_get_option('exclude'))));

  // Create failures, failed_checks, and exit_code variables
  // from check_run_checks output.
  extract(check_run_checks($checks));

  _check_render_errors($failures);

  if (!empty($failures)) {
    $variables = array(
      '@checks' => implode($failed_checks, ', '),
    );

    drush_set_error(dt('The following checks have failed: @checks', $variables));
  }

  drush_set_context('DRUSH_EXIT_CODE', $exit_code);
}

/**
 * Map a check module alert code to a watchdog alert code.
 */
function _check_translate_alert_level_to_watchdog_level($code) {
  $mapping = array(
    CHECK_OK => WATCHDOG_INFO,
    CHECK_WARNING => WATCHDOG_WARNING,
    CHECK_CRITICAL => WATCHDOG_CRITICAL,
  );
  return $mapping[$code];
}

/**
 * Render check errors using drush_log and watchdog.
 *
 * @return A linear array of failed checks.
 */
function _check_render_errors($failures) {
  $bad_statuses = array(
    CHECK_UNKNOWN => 'UNKNOWN',
    CHECK_WARNING => 'WARNING',
    CHECK_CRITICAL => 'CRITICAL',
  );
  foreach ($bad_statuses as $code => $status) {
    if (!empty($failures[$code])) {
      foreach ($failures[$code] as $failure) {
        $message = $status . ': ' . $failure['message'];
        $variables = isset($failure['variables']) ? $failure['variables'] : array();
        // Druapl <= 7
        if (function_exists('watchdog')) {
          watchdog('check', $message, $variables, _check_translate_alert_level_to_watchdog_level($code));
        }
        // Drupal >= 8
        else {
          $level = $code == CHECK_CRITICAL ? 'critical' : 'error';
          \Drupal::logger('check')->log($level, dt($message, $variables));
        }
        drush_log(dt($message, $variables), 'error');
      }
    }
  }
}

/**
 * Check the last time cron was successfully completed.
 */
function _drush_check_last_cron() {

  // Drupal <= 7
  if (function_exists('variable_get')) {
    // Here we use `time()` and not REQUEST_TIME for D6 backward compatibility.
    $last_cron = variable_get('cron_last', 0);
  }
  // Drupal >= 8
  else {
    $last_cron = \Drupal::state()->get('system.cron_last');
  }
  $minutes_since_last_cron = ceil((time() - $last_cron) / 60);

  // By default if a cron job fails or times out it will not start running again for 60 minutes,
  // so we give it a chance to right itself before complaining.
  $warning = drush_get_option('cron-max-time-warning', 61);

  // Be default, consider going an hour and a half without cron running a critical error.
  $critical = drush_get_option('cron-max-time-warning', 90);

  $variables = array(
    '@mins' => $minutes_since_last_cron,
    '@warning' => $warning,
    '@critical' => $critical,
  );

  $status = CHECK_OK;
  $message = '';

  if ($minutes_since_last_cron > $critical) {
    $status = CHECK_CRITICAL;
    $message = 'cron has not successfully run for @mins mins, set to be critical after @critical';
  }

  elseif ($minutes_since_last_cron > $warning) {
    $status = CHECK_WARNING;
    $message = 'cron has not successfully run for @mins mins, set to warn after @warning';
  }

  return array(
    'status' => $status,
    'message' => $message,
    'variables' => $variables,
  );

}

/**
 * Run the Drupal requirements checks.
 */
function _drush_check_security_updates() {

  $output = drush_invoke_process('@self', 'pm-updatestatus', array(), array('--security-only', '--format=json'), FALSE);

  if ($output['error_status']) {
    return array(
      'status' => CHECK_UNKNOWN,
      'message' => 'Security updates could not be checked.',
    );
  }
  $updates = array();
  $insecure_projects = array();
  $projects = json_decode($output['output'], TRUE);
  if (!empty($projects)) {
    foreach ($projects as $project => $data) {
      $updates[] = $project . '@' . $data['existing_version'];
      $insecure_projects[$project] = $data['existing_version'];
    }
  }
  $variables = array(
    '@insecure_projects' => $insecure_projects,
    // Projects is included as a variable because this is sent to watchdog and some watchdog
    // implementations do intelligent things with overloaded arguments.
    '@updates' => implode(', ', $updates),
  );
  return array(
    'status' => empty($insecure_projects) ? CHECK_OK : CHECK_CRITICAL,
    'message' => 'Security updates required: @updates',
    'variables' => $variables,
  );

}


/**
 * Check the core requirments settings.
 */
function _drush_check_requirements() {

  $parameters = array(
    '--format=json',
    '--severity=' . drush_get_option('requirements-warn-level', 2),
  );
  $output = drush_invoke_process('@self', 'core-requirements', array(), $parameters, FALSE);

  if ($output['error_status']) {
    return array(
      'status' => CHECK_UNKNOWN,
      'message' => 'Security updates could not be checked.',
    );
  }

  $requirements = json_decode($output['output'], TRUE);
  $severity = 0;
  $alerts = array();
  $problem_requirements = array();
  foreach ($requirements as $requirement) {
    $severity = max($severity, $requirement['sid']);
    $alerts[] = $requirement['title'] . ' ' . $requirement['value'];
    $problem_requirements[] = $requirement;
  }
  $variables = array(
    '@problem_requirements' => $problem_requirements,
    // Projects is included as a variable because this is sent to watchdog and
    // some watchdog implementations do intelligent things with overloaded
    // arguments.
    '@alerts' => implode(', ', $alerts),
  );
  if ($severity >= drush_get_option('requirement-warn-level', 1)) {
    $status = 1;
  }
  if ($severity >= drush_get_option('requirement-critical-level', 2)) {
    $status = 2;
  }
  if ($status) {
    return array(
      'status' => $status,
      'message' => 'Drupal requirements checks failed: @alerts',
      'variables' => $variables,
    );
  };

}

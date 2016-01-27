<?php
/**
 * @file
 * Provide Drush integration.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Implements hook_drush_command().
 */
function worker_drush_command() {
  $items = array();

  $items['worker-status'] = array(
    'description' => 'Print the status of the worker, either idle or busy.',
    'aliases' => array('wstatus'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_SITE,
  );
  $items['worker-log'] = array(
    'description' => 'A refreshing job log display.',
    'aliases' => array('wlog'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['worker-reset'] = array(
    'description' => 'Reset the worker in case of catastrophic crash.',
    'aliases' => array('wreset'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_SITE,
  );
  $items['worker-deamon'] = array(
    'description' => 'Worker deamon that spawns a worker to perform jobs and monitors the process.',
    'arguments' => array(
      'user' => 'The user which should be used to perform the job (ex. wwwrun). Should not be the same as the user running the deamon.',
    ),
    'options' => array(
      'monitor' => 'Print monitoring information about what the deamon is doing.',
    ),
    'aliases' => array('wdeamon'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_SITE,
  );
  $items['worker-perform'] = array(
    'description' => 'Perform a job as defined in the specified file.',
    'arguments' => array(
      'file' => 'A PHP file containing the job properties array defined as $properties in the global namespace.',
    ),
    'options' => array(
      'report' => 'Report the result to the conduit server.',
      'summary' => 'Instead of the standard output print only a summary.',
    ),
    'aliases' => array('wperform'),
  );
  $items['worker-requirements'] = array(
    'description' => 'Check the status of the worker requirements.',
    'aliases' => array('wrequirements'),
    'options' => array(
      'summary' => 'Instead of the standard output print only passed or failed.',
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_init().
 */
function worker_drush_init() {
  // When not displaying help information define constants.
  $args = func_get_args();
  if ((!$args || !drush_is_command(current($args))) && module_exists('worker')) {
    // Constants for the relevant files.
    define('DRUSH_WORKER_ERROR', file_directory_path() . '/error.log');
    define('DRUSH_WORKER_LOG', file_directory_path() . '/job.log');
    define('DRUSH_WORKER_JOB', file_directory_path() . '/job.php');
    define('DRUSH_WORKER_RESULT', file_directory_path() . '/job.result');
    define('DRUSH_WORKER_PROCESS', file_directory_path() . '/process.-1');

    // The length of time the deamon should sleep in seconds followed by the
    // maximum length of time a single job is allowed in milliseconds.
    define('DRUSH_WORKER_DEAMON', 60);
    define('DRUSH_WORKER_TIMEOUT', 1000 * 60 * 60 * 2);
  }
}

/**
 * Print the status of the worker, either idle or busy.
 */
function drush_worker_status() {
  worker_include('job');
  drush_print(worker_execute_background_done(-1) ? 'idle' : 'busy');
}

/**
 * A refreshing job log display
 */
function drush_worker_log() {
  $rows = preg_match('/rows (\d+);/', shell_exec('stty -a'), $match) ? (int) $match[1] - 2 : 40;
  passthru("watch tail -n $rows " . escapeshellarg(DRUSH_WORKER_LOG));
}

/**
 * Reset the worker in case of catastrophic crash.
 */
function drush_worker_reset() {
  // Check to ensure the file_directory_path() is defined so that /* is not
  // accidentally removed instead.
  if (file_directory_path()) {
    system('rm -rf ' . file_directory_path() . '/*', $status);
    return $status == 0;
  }
  drush_set_error('reset_failed', dt('Failed to remove clear files directory.'), 'error');
  return FALSE;
}

/**
 * Ensure that the user argument for deamon is valid.
 *
 * @param $user
 *   The user which should be used to perform the job (ex. wwwrun). Should not
 *   be the same as the user running the deamon.
 * @return
 *   TRUE if valid otherwise FALSE.
 */
function drush_worker_deamon_validate($user = NULL) {
  if (!$user || !shell_exec('id ' . escapeshellarg($user))) {
    return drush_set_error('user', dt('A valid user is required.'));
  }
  if ($user == get_current_user()) {
    return drush_set_error('user', dt('The user must not be the same as the current user.'));
  }
}

/**
 * Worker deamon that spawns a worker to perform jobs and monitors the process.
 *
 * @param $user
 *   The user which should be used to perform the job (ex. wwwrun). Should not
 *   be the same as the user running the deamon.
 */
function drush_worker_deamon($user) {
  worker_include('job');
  worker_include('rest');

  $debug = drush_get_option('debug');
  $monitor = drush_get_option('monitor');

  if ($monitor) drush_log(dt('User to perform jobs as: @user.', array('@user' => $user)), 'success');

  // Loop until the process is terminated or a halt is called.
  while (TRUE) {
    if ($monitor) drush_log(dt('Loop started.', array('@user' => $user)), 'success');

    // Look for one of the three conditions before looping.
    // 1) A job is available: store the properties array in a file, launch the
    //    perform process and monitor the result.
    // 2) No job is available, the worker_halt variable is set to TRUE, and the
    //    uptime is within the shutdown window: halt the machine.
    // 3) Neither condition 1 or 2 were met: sleep for DRUSH_WORKER_DEAMON and
    //    loop again.
    if ($properties = worker_rest_claim()) {
      if ($monitor) {
        $data = $properties['item']['data'];
        drush_log(dt('Received job @nid.@vid.@delta.', array('@nid' => number_format($data['nid']), '@vid' => number_format($data['vid']), '@delta' => number_format($data['delta']))), 'success');
      }

      // Store the job properties the job.php file.
      file_put_contents(DRUSH_WORKER_JOB, '<?php $properties = ' . var_export($properties, TRUE) . ';');

      // Perform the current job and monitor the process.
      drush_worker_deamon_perform($user);

      // Kill all processes owned by $user and reset the worker to ensure
      // nothing is left between jobs.
      exec('killall -w -u ' . escapeshellarg($user));

      // If running in debug mode do not reset worker to ensure that files are
      // available to look through.
      if (!$debug) {
        $reset = drush_worker_reset();
        if ($monitor) drush_log(dt('Cleaned environment.'), $reset ? 'success' : 'error');
      }
    }
    elseif (variable_get('worker_halt') && preg_match('/up\s+\d+:(\d+)/', current(worker_execute_output('uptime')), $match) && $match[1] >= 56) {
      // Check the length of uptime and halt the machine if within the last four
      // minutes of an hour. Taking into account that machine reservation time
      // and boot time may take approximately two minutes and shutdown make take
      // another minute or two this will ensure the box stays within a whole
      // hour time block.
      worker_execute_background('halt');
      return;
    }
    else {
      if ($monitor) drush_log(dt('Sleeping for @time.', array('@time' => format_interval(DRUSH_WORKER_DEAMON))), 'success');
      sleep(DRUSH_WORKER_DEAMON);
    }
  }
}

/**
 * Perform a current job and monitor the process.
 *
 * @param $user
 *   The user which should be used to perform the job (ex. wwwrun). Should not
 *   be the same as the user running the deamon.
 */
function drush_worker_deamon_perform($user) {
  $monitor = drush_get_option('monitor');

  // Ensure all files are initialized for inotify.
  file_put_contents(DRUSH_WORKER_ERROR, '');
  file_put_contents(DRUSH_WORKER_LOG, '');
  file_put_contents(DRUSH_WORKER_RESULT, '');
  file_put_contents(DRUSH_WORKER_PROCESS, '');

  // Log file needs to be writable by $user.
  chmod(DRUSH_WORKER_LOG, 0666);

  // Setup inotify watches on the three files.
  $watch_error = inotify_add_watch($monitor_status = inotify_init(), DRUSH_WORKER_ERROR, IN_CLOSE_WRITE);
  $watch_process = inotify_add_watch($monitor_status, DRUSH_WORKER_PROCESS, IN_DELETE_SELF);
  inotify_add_watch($monitor_log = inotify_init(), DRUSH_WORKER_LOG, IN_MODIFY);

  // Allow access to code files so $user can load them.
  exec('chmod -R o+r sites/all/modules');

  // Start timer to keep track of job duration.
  timer_start('worker_perform');

  // Start the worker process as $user and ensure that it is process -1.
  $process = &drupal_static('worker_execute_background');
  $process = -2;
  worker_execute_background('sudo -u ' . escapeshellarg($user) . ' drush --nocolor worker-perform --report --summary ' .
    escapeshellarg(DRUSH_WORKER_JOB) .' > ' . escapeshellarg(DRUSH_WORKER_RESULT) . ' 2> ' . escapeshellarg(DRUSH_WORKER_ERROR));

  // Wait for job.log to be modified signaling that the job process is loaded.
  inotify_read($monitor_log);
  fclose($monitor_log);

  // Restrict access to code files since $user has preloaded required files.
  exec('chmod -R o-r sites/all/modules');

  if ($monitor) drush_log(dt('Job process loaded and running.'), 'success');

  // Continously scan for event notifications until:
  // 1) The error.log file is written and closed signaling the worker crashed
  // 2) The worker perform process (process.-1) file is deleted signaling the
  //    job has finished or if an empty job.result then terminated early without
  //    an error (possible intential attack/command).
  // 3) The previous two events are never signaled and the DRUSH_WORKER_TIMEOUT
  //    is exceeded.
  while (timer_read('worker_perform') < DRUSH_WORKER_TIMEOUT) {
    if ($monitor) drush_log(dt('Waiting for events.'), 'success');

    // Read from the status monitoring stream for a maximum of 60 seconds. If
    // events are found them check to see if they signal condition 1 or 2,
    // otherwise loop again.
    if (stream_select($read = array($monitor_status), $write = array(), $except = array(), 60) && ($events = inotify_read($monitor_status))) {
      foreach ($events as $event) {
        if ($event['wd'] == $watch_error && ($error = trim(file_get_contents(DRUSH_WORKER_ERROR)))) {
          if ($monitor) drush_log(dt('Job crashed.'), 'error');
          drush_worker_deamon_error(dt('Worker crashed'), "\n\n" . str_repeat('=', 80) . "\n\n" . $error);
          fclose($monitor_status);
          return;
        }
        elseif ($event['wd'] == $watch_process) {
          if ($result = file_get_contents(DRUSH_WORKER_RESULT)) {
            if ($monitor) drush_log(dt('Job finished.'), 'success');
          }
          else {
            if ($monitor) drush_log(dt('Job process terminated prematurely!'), 'error');
            drush_worker_deamon_error(dt('Job process terminated prematurely'));
          }
          fclose($monitor_status);
          return;
        }
      }
    }
  }

  // Cases 1 and 2 never occured and the DRUSH_WORKER_TIMEOUT has been exceeded
  // so report the job such.
  if ($monitor) drush_log(dt('Job timed out.'), 'error');
  drush_worker_deamon_error(dt('Job timeout of @timeout exceeded', array('@timeout' => format_interval(DRUSH_WORKER_TIMEOUT / 1000))));
  fclose($monitor_status);
}

/**
 * Report an error for the current job.
 *
 * @param $result
 *   The result summary to report.
 * @param $log
 *   (Optional) Additional information to append to the log.
 */
function drush_worker_deamon_error($result, $log = '') {
  $properties = drush_worker_perform_load(DRUSH_WORKER_JOB);
  $result = array(
    'status' => WORKER_STATUS_SETUP,
    'result' => $result,
    'log' => trim(file_get_contents(DRUSH_WORKER_LOG)) . $log,
    'duration' => ceil(timer_read('worker_perform') / 1000),
  );
  worker_rest_result($properties['item'], $result);
}

/**
 * Ensure the input file exists and is readable.
 */
function drush_worker_perform_validate($file = '') {
  if (!file_exists($file) || !is_readable($file)) {
    return drush_set_error('file', dt('The file must exist and be readable.'));
  }
}

/**
 * Perform a job as defined in the specified file.
 *
 * @param $file
 *   A PHP file containing the job properties array defined as $properties in
 *   the global namespace.
 */
function drush_worker_perform($file) {
  // Load includes before file access is disabled.
  worker_include('job');
  worker_include('rest');

  // Start a timer to keep track of job duration.
  timer_start('worker_perform');

  // Load the file and access the $properties array.
  $properties = drush_worker_perform_load($file);

  // Perform job.
  $result = worker_perform($properties);
  $result['duration'] = ceil(timer_read('worker_perform') / 1000);

  if (drush_get_option('report')) {
    // Report the result to the conduit server.
    worker_rest_result($properties['item'], $result);
  }

  if (drush_get_option('summary')) {
    drush_print($result['status']);
  }
  else {
    drush_print_r($properties);
    drush_print_r($result);
  }
}

/**
 * Load a properties file.
 *
 * @param $file
 *   The path to properties file.
 * @return
 *   An array of properties.
 */
function drush_worker_perform_load($file) {
  global $properties;
  require $file;
  $properties['item'] = (object) $properties['item'];
  return $properties;
}

/**
 * Check the status of the worker requirements.
 */
function drush_worker_requirements() {
  worker_include('job');
  worker_include('rest');

  // Basic requirements.
  $requirements = array(
    'PHP executable path found (%s).' => worker_php_path(),
    'Drush is configured properly.' => TRUE,
    'Variable worker_url found.' => variable_get('worker_url'),
    'Variable worker_login found.' => variable_get('worker_login'),
    'Variable worker_category found.' => variable_get('worker_category'),
    'Successfully authenticated with conduit server.' => worker_rest_login(),
  );

  // Check database setup.
  global $databases;
  foreach (array_keys($databases) as $database) {
    if (preg_match('/^(?P<key>(?P<base>.*)_(?P<driver>.*))_stub$/m', $database, $match) && isset($database[$match['key']])) {
      $requirements["Database pair found for [{$match['base']}] with driver [{$match['driver']}]."] = TRUE;
    }
  }

  // Iterate over requirements to determine overall status and output messages.
  $pass = TRUE;
  foreach ($requirements as $message => $value) {
    $pass = $pass && $value;
    if (!drush_get_option('summary')) {
      drush_log(sprintf($message, $value), $value ? 'success' : 'error');
    }
  }

  // If only the summary was requested the output passed or failed.
  if (drush_get_option('summary')) {
    drush_print($pass ? 'passed' : 'failed');
  }
}

<?php
/**
 * @file
 * Provide convenience functions and environment setup for jobs.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Absolute path to file directory.
 */
define('FILE_ROOT', DRUPAL_ROOT . '/' . file_directory_path());

/**
 * Root directory for job.
 */
define('JOB_ROOT', FILE_ROOT . '/job');

/**
 * Absolute URL to the job directory.
 */
define('JOB_URL', 'http://worker.loc');

/**
 * Status of job: failed during setup.
 */
define('WORKER_STATUS_SETUP', 1);

/**
 * Status of job: plugin encountered a failure.
 */
define('WORKER_STATUS_FAIL', 2);

/**
 * Status of job: pluging passed.
 */
define('WORKER_STATUS_PASS', 3);

/**
 * Perform the given job.
 *
 * Inital cleanup from last job run -- delete job dir, prepare new directory
 * Setup commands -- bzr launchpad is more like a config command, it does not login
 * Code checkout, patch apply
 *
 * for svn, checkout from subclipse
 * check log for all array keys, content will vary
 *
 * @param $properties
 *   Associative array of job properties applicable to the current job.
 *   Supported keys and values are:
 *   - plugin: The name of the plugin to load. A valid plugin with the
 *       same name should exist in the worker/plugins/worker directory.
 *   - verbose: A boolean indicating whether to return verbose debug information.
 *       Debug information is [written to log|included in results returned by worker].
 *   - command: The command string to execute. Only applies if plugin = 'execute'.
 *
 *     Scope parameters used to determine the list of files to perform a job with.
 *   - path: Array of directory and file paths relative to the JOB_ROOT directory.
 *       Enumerated files are always included regardless of the mask and nomask values.
 *       Directories are recursed by default. [can this be changed except by listing files?]
 *   - mask: The preg_match() regular expression of the files to find.
 *   - nomask: The preg_match() regular expression of the files to ignore.
 *       Defaults to '/(\.\.?|CVS)$/'.
 *
 *   - setup: Array of command strings to execute during the job setup phase.
 *
 *   - newline: The type of newline characters [in the files the job will be executed against].
 *       Possible values are: 'unix', 'mac', 'windows', 'any'.
 *
 *     Code repository parameters.
 *   - vcs: Associative array of code repository paths whose keys are directory
 *       paths relative to the JOB_ROOT directory. A key of '' refers to the
 *       JOB_ROOT directory.
 *       [strings (matching the values in the patch array).]
 *   - patch: Associative array of file paths whose keys are patch files to be
 *       applied to the files in the path. The file path values must match a
 *       file path key in the vcs property array.
 *     [Why not have key=directory path and value=array of patch file URLs???]
 *
 *     Login properties for code repositories.
 *   - ssh: Array of associative arrays containing ssh keys for connecting to a
 *       code repository. The keys of each associative array are:
 *       - key: Public key for validating with the host.
 *       - host: The repository [URL|???].
 *       - user: the user name [associated with the key|to login to the repository with].
 *       - port: The IP port to connect to the repository server.
 *
 *   - item: Object added by conduit server containing row ids for current job.
 *   - context: Integer indicating the number of items (assertions|lines)
 *       surrounding an error (failed assertion|parse error) to return.
 *
 * @return
 *   Associative array containing the job result (status, result, log).
 */
function worker_perform(array $properties) {
  // Preload all plugins before access to code is revoked.
  worker_plugins_get_all('database');
  worker_plugins_get_all('vcs');
  worker_plugins_get_all('worker');

  // Allow other modules to preload any additional files.
  module_invoke_all('worker_preload');

  // Initialize result so that the default status returned is failure unless
  // the plugin was found, the worker successfully prepared, and the plugin
  // executed with a success return value.
  $result = array(
    'status' => WORKER_STATUS_SETUP,
    'result' => NULL,
  );

  // Initialize log and set vebose reporting mode.
  $data = $properties['item']->data;
  worker_log('Start job ' . number_format($data['nid']) . '.' . number_format($data['vid']) . '.' . number_format($data['delta']) . '.');
  worker_verbose($properties['verbose']);
  worker_log_version();

  // Ensure that the job plugin can be loaded, prepare the environment, and
  // invoke the plugin to perform.
  if (($plugin = worker_plugins_get('worker', $properties['plugin'])) && worker_prepare($properties)) {
    list($pass, $result['result']) = $plugin['perform']($properties);
    $result['status'] += $pass ? 2 : 1;

    // In case worker_prepare() or $plugin['perform']() crash ensure the
    // working directory is reset so Drupal functions properly.
    chdir(DRUPAL_ROOT);
  }
  elseif (!$plugin) {
    worker_log('Invalid plugin ' . $properties['plugin'] . '.');
  }
  worker_log('Exit job.');

  // Load log file contents as an array so the second to last line can be
  // accessed if necessary.
  $log = file(file_directory_path() . '/job.log', FILE_IGNORE_NEW_LINES);
  $result['log'] = implode("\n", $log);

  // If the job was a failure then use the second to last line from the log as
  // the error message after trimming off the time offset.
  if ($result['status'] == WORKER_STATUS_SETUP) {
    array_pop($log);
    $result['result'] = worker_log_last($log);
  }
  return $result;
}

/**
 * Perform preparation needed before job runs.
 *
 * @param $properties
 *   Associative array of job properties.
 * @return
 *   TRUE if successfull, otherwise FALSE.
 *
 * @see worker_perform() for a description of the properties array.
 */
function worker_prepare(array $properties) {
  if (!worker_directory_prepare()) {
    return FALSE;
  }

  foreach (array('setup', 'ssh', 'vcs', 'patch', 'build') as $action) {
    $function  = 'worker_' . $action;
    if (!$function($properties)) {
      chdir(DRUPAL_ROOT);
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Prepare the job root directory.
 *
 * @return
 *   TRUE if successfull, otherwise FALSE.
 */
function worker_directory_prepare($directory = JOB_ROOT) {
  // If directory exists then clear it out.
  if (file_exists($directory)) {
    worker_execute('chmod -R 755 ' . escapeshellarg($directory));
    worker_delete_recursive($directory);
  }

  // Create directory.
  if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
    worker_log('Failed to prepare job root directory.');
    return FALSE;
  }
  return TRUE;
}

/**
 * Allow commands to be run during setup phase.
 *
 * @param $properties
 *   Associative array of job properties.
 * @return
 *   TRUE if successfull, otherwise FALSE.
 *
 * @see worker_perform() for a description of the properties array.
 */
function worker_setup(array $properties, $type = 'setup') {
  worker_chdir();
  $success = TRUE;
  foreach ($properties[$type] as $command) {
    // Append < /dev/null as input stream to ensure commands do not block
    // waiting for user input.
    worker_log(ucfirst($type) . ': `' . $command . '`.');
    $success = worker_execute(escapeshellcmd($command) . ' < /dev/null') && $success;
  }
  chdir(DRUPAL_ROOT);

  if (!$success) worker_log("Failed to complete execution of $type commands.");
  return $success;
}

/**
 * Allow commands to be run after source has been setup, but before plugin.
 *
 * @param $properties
 *   Associative array of job properties.
 * @return
 *   TRUE if successfull, otherwise FALSE.
 */
function worker_build(array $properties) {
  return worker_setup($properties, 'build');
}

/**
 * Prepare ssh keys.
 *
 * @param $properties
 *   Associative array of job properties.
 * @return
 *   TRUE if successfull, otherwise FALSE.
 *
 * @see worker_perform() for a description of the properties array.
 */
function worker_ssh(array $properties) {
  // Determine the web user home directory.
  if (($home = current(worker_execute_output('eval echo ~$(whoami)'))) && is_dir($home)) {
    // Clean out .ssh directory and recreate it.
    worker_directory_prepare($home . '/.ssh');

    // Initialize ssh config file.
    file_put_contents($home . '/.ssh/config', "# Auto generated by worker. Do not modify this file manually.\n\n");

    // Cycle through ssh key information and process.
    $delta = 0;
    foreach ($properties['ssh'] as $info) {
      worker_log('Adding ssh key for host ' . $info['host'] . '.');
      // Write the ssh key to a file and set the appropriate permissions.
      $file = $home . '/.ssh/id_rsa.' . ++$delta;
      file_put_contents($file, $info['key']);
      chmod($file, 0600);

      // Add an entry in the ssh config file for the key.
      $config = "Host {$info['host']}\n" .
        "\tIdentityFile $file\n" .
        "\tUser {$info['user']}\n" .
        "\tPort {$info['port']}\n\n";
      file_put_contents($home . '/.ssh/config', $config, FILE_APPEND);

      // Add the host to the known_hosts file.
      $line = current(worker_execute_output('ssh -o StrictHostKeyChecking=no ' . escapeshellarg($info['host']) . ' exit'));
      if (preg_match('/Warning: Permanently added .*? to the list of known hosts./', $line)) {
        // Coninue with next ssh key.
        continue;
      }
      worker_log('Failed to setup ssh key for ' . $info['host'] . '.');
      return FALSE;
    }
    return TRUE;
  }
  worker_log('Failed to determine the webuser home directory path.');
  return FALSE;
}

/**
 * Process version control repositories.
 *
 * @param $properties
 *   Associative array of job properties.
 * @return
 *   TRUE if successfull, otherwise FALSE.
 *
 * @see worker_perform() for a description of the properties array.
 */
function worker_vcs(array $properties) {
  // Cycle through all repositories until complete or an error is encountered.
  foreach ($properties['vcs'] as $path => $url) {
    // Load plugin appropriate for handling the scheme.
    if ($vcs = worker_plugins_get_vcs($url)) {
      // Ensure the path specified exists.
      worker_directory_prepare(JOB_ROOT . '/' . $path);
      worker_chdir($path);

      // Checkout from the repository.
      worker_log('Checkout ' . $url . ' into /' . $path . '.');
      if (!$vcs['checkout']($url)) {
        worker_log('Failed to checkout ' . $url . '.');
        return FALSE;
      }

      // Return to the Drupal root after processing a repository successfully
      // to ensure that the next plugin is loaded properly.
      chdir(DRUPAL_ROOT);
    }
    else {
      worker_log('Unsupported scheme in ' . $url . '.');
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Apply a list of patches.
 *
 * @param $properties
 *   Associative array of job properties.
 * @return
 *   TRUE if successfull, otherwise FALSE.
 *
 * @see worker_perform() for a description of the properties array.
 */
function worker_patch(array $properties) {
  // Prepare patch storage directory.
  worker_directory_prepare($directory = DRUPAL_ROOT . '/' . file_directory_path() . '/patch');

  // Cycle through all the patches, download them and apply them.
  $delta = 0;
  foreach ($properties['patch'] as $url => $path) {
    if (!empty($properties['vcs'][$path]) && ($vcs = worker_plugins_get_vcs($properties['vcs'][$path]))) {
      // Use the path specified as the root the patch is to be applied to. Use
      // a delta to ensure patch file names do not conflict.
      worker_chdir($path);
      if (!($local = worker_fetch($url, $directory, $delta++))) {
        worker_log('Failed to fetch patch file from ' . $url);
        return FALSE;
      }
      $name = basename($url);

      // If requested check for non-unix newline characters.
      if ($properties['newline'] == 'unix' && strpos(file_get_contents($local), "\r") !== FALSE) {
        worker_log('Non-unix new line character(s) found in ' . $name . '.');
        return FALSE;
      }

      // Ensure that path the patch is to be applied to is readable.
      worker_log('Apply ' . $name . ' to /' . $path . '.');
      if (!$vcs['apply']($local)) {
        worker_log('Failed to apply patch ' . $name . '.');
        return FALSE;
      }
      worker_log_list('Modified files:', $vcs['modified files']($local));
    }
    else {
      worker_log('No repository corresponds to path /' . $path . ' for ' . basename($url) . '.');
      return FALSE;
    }
  }
  chdir(DRUPAL_ROOT);
  return TRUE;
}

/**
 * Fetch a file and place in directory.
 *
 * @param $url
 *   URL of file to fetch.
 * @param $directory
 *   Directory to place file in.
 * @param $suffix
 *   (Optional) Suffix to append to the local name of the file. (defaults to '')
 * @return
 *   Local path to file, or FALSE if retrieval/storage failed.
 */
function worker_fetch($url, $directory, $suffix = '') {
  // Override error handler so error_get_last() works.
  set_error_handler(function() { return false; });
  $contents = @file_get_contents($url);
  restore_error_handler();

  if ($contents === FALSE) {
    // Load error message and skip the function call portion.
    $error = error_get_last();
    worker_log('Unable to fetch ' . $url . ' due to [' . substr($error['message'], strpos($error['message'], ':') + 2) . '].');
    return FALSE;
  }

  // Save the file contents in the given directory.
  $local = $directory . '/' . basename($url) . $suffix;
  if (file_put_contents($local, $contents) === FALSE) {
    worker_log('Failed to save file data to ' . $local . '.');
    return FALSE;
  }
  return $local;
}

/**
 * Apply a patch in the working directory.
 *
 * @param $patch
 *   Path to patch file.
 * @return
 *   TRUE if successfull, otherwise FALSE.
 */
function worker_patch_apply($patch) {
  return worker_execute('patch -p0 -i ' . escapeshellarg($patch));
}

/**
 * Get a list of all files modified by a patch.
 *
 * @param
 *   Path to patch file, which must be in the unified format.
 * @return
 *   List of files modified by the patch.
 */
function worker_patch_modified_files($patch) {
  // Load the patch file and use regular expression to find all files modified
  // by the patch.
  $contents = file_get_contents($patch);
  preg_match_all('/^[\+-]{3} (.*?)\s/m', $contents, $matches, PREG_SET_ORDER);

  $files = array();
  foreach ($matches as $match) {
    // Ignore reference to /dev/null which will occur when a file is being
    // added or removed.
    if ($match[1] != '/dev/null') {
      $files[$match[1]] = $match[1];
    }
  }
  return array_values($files);
}

/**
 * Evaluate scope properties to generate a list of files.
 *
 * @param $properties
 *   Associative array of properties containing keys: path, mask, and nomask.
 * @return
 *   List of files relative to the JOB_ROOT.
 *
 * @see worker_perform() for a description of the properties array.
 */
function worker_file_list(array $properties) {
  // Cycle through all paths, determine if path points to a file or directory
  // and process accordingly.
  $files = array();
  foreach ($properties['path'] as $path) {
    $name = basename($path);
    $full_path = rtrim(JOB_ROOT . '/' . $path, '/');

    // Ensure path exists.
    if (!file_exists($full_path)) {
      worker_log('Invalid path /' . $path . '.');
      return FALSE;
    }

    // If path points to a directory then scan all the files within using the
    // mask and no mask properties.
    if (is_dir($full_path)) {
      $scan = file_scan_directory($full_path, $properties['mask'], array('nomask' => $properties['nomask']));

      // Cycle through resulting files and create an associative array of
      // relative paths.
      foreach ($scan as $uri => $info) {
        $path = str_replace(JOB_ROOT . '/', '', $uri);
        $files[$path] = $path;
      }
    }
    else {
      // Always add files regardless of mask and nomask.
      $files[$path] = $path;
    }
  }

  // Sort files and return a list of relative paths.
  ksort($files);
  return array_keys($files);
}

/**
 * Check for syntax errors.
 *
 * @param $properties
 *   Associative array of properties defining the job.
 * @param $callback
 *   (Optional) Function to call in order to check the syntax of a file. The
 *   only parameter passed the function will be the file path relative to the
 *   current working directory. The function is expected to return TRUE if the
 *   file passed the syntax check, an array containing a message and line
 *   number of the error (or FALSE), or FALSE if the syntax check failed to run
 *   properly. The default callback checks PHP syntax.
 * @return
 *   Associative array of syntax errors keyed by path and containing 'messaage'
 *   and 'context' (an array of lines), or FALSE if an error occured.
 *
 * @see worker_perform() for a description of the properties array.
 */
function worker_syntax(array $properties, $callback = 'worker_syntax_php') {
  worker_log('Assemble list of files to syntax check.');
  if (!($files = worker_file_list($properties))) {
    return FALSE;
  }
  worker_log('Syntax check ' . number_format(count($files)) . ' file(s).');

  worker_chdir();
  $length = $properties['context'] * 2 + 1;
  $errors = array();
  foreach ($files as $file) {
    if (worker_verbose()) worker_log('> Check ' . $file . '.');
    if ($error = $callback($file)) {
      // Continue if the file passed the syntax check.
      if ($error === TRUE) {
        continue;
      }

      $errors[$file] = array(
        'message' => $error['message'],
        'context' => array(),
      );

      // If the line referenced by the message was determined then pull the
      // relevant lines from the file as context.
      if ($error['line']) {
        $lines = file($file, FILE_IGNORE_NEW_LINES);
        $errors[$file]['context'] = array_slice($lines, $error['line'] - $properties['context'] - 1, $length, TRUE);
      }
    }
    else {
      worker_log('Syntax check did not respond properly.');
      chdir(DRUPAL_ROOT);
      return FALSE;
    }
  }
  chdir(DRUPAL_ROOT);

  if (!empty($errors)) {
    worker_log_list('Syntax errors found in the following file(s):', array_keys($errors));
  }
  return $errors;
}

/**
 * Check the syntax of a PHP file.
 *
 * @param $file
 *   Relative path to file.
 * @return
 *   See worker_syntax() for details on the return value
 *
 * @see worker_syntax()
 */
function worker_syntax_php($file) {
  if ($output = worker_execute_output('php -l -f ' . escapeshellarg($file))) {
    $output = $output[0];
    if (strpos($output, 'No syntax errors detected') === FALSE) {
      $error = array(
        'message' => $output,
        'line' => FALSE,
      );

      // If the messages follows the standard and the line number can be
      // determined with a the regular expression then return it.
      if (preg_match('/line (?P<line>\d+)$/m', $output, $match)) {
        $error['line'] = $match['line'];
      }
      return $error;
    }
    return TRUE;
  }
  return FALSE;
}

/**
 * Prepare a database and return the connection object.
 *
 * For each $prefix/$driver pair there should exist two database connections in
 * the $databases array found in settings.php. The keys should follow the
 * format: [PREFIX]_[DRIVER], [PREFIX]_[DRIVER]_stub. The stub connection
 * should point to an existing database of the proper type that is empty while
 * the primary should not point to an existing database, but the user should
 * have permission to create and drop the database it points to.
 *
 * @param $prefix
 *   The prefix used in the $databases array.
 * @param $driver
 *   The database driver which the connection should use.
 * @return
 *   A database connection object on success, otherwise FALSE.
 */
function worker_database($prefix, $driver) {
  static $prepared = array();

  // Generate database key strings.
  $name = $prefix . '_' . $driver;
  $stub = $name . '_stub';

  // If the database connection has already been prepared then simply return it.
  if (!empty($prepared[$name])) {
    return Database::getConnection('default', $name);
  }

  // Attempt to prepare the database connection using the stub connection.
  global $databases;
  if ($connection = Database::getConnection('default', $stub)) {
    // Attempt to load the worker database plugin for the specified driver.
    if ($plugin = worker_plugins_get('database', $driver)) {
      // Ensure the database connection information is available for the primary database.
      if (!empty($databases[$name]['default']['database'])) {
        // Attempt to drop/create the primary database using the stub
        // connection with the appropriate driver.
        $database = $databases[$name]['default']['database'];
        if ($plugin['drop']($connection, $database) && $plugin['create']($connection, $database)) {
          $prepared[$name] = TRUE;
          return worker_database($prefix, $driver);
        }
        worker_log('Failed to prepare (drop/create) the primary database.');
      }
      else {
        worker_log('Failed to find the primary database information.');
      }
    }
    else {
      worker_log('Failed to load database driver for ' . $driver . '.');
    }
  }
  else {
    worker_log('Failed to load stub database connection.');
  }
  return FALSE;
}

/**
 * Get the database information for the primary database.
 *
 * @param $prefix
 *   The prefix used in the $databases array.
 * @param $driver
 *   The database driver which the connection should use.
 * @return
 *   Database information from $databases array in settings.php.
 */
function worker_database_info($prefix, $driver) {
  global $databases;
  return $databases[$prefix . '_' . $driver]['default'];
}

/**
 * Determine the location of the PHP executable from the whereis command.
 *
 * @return
 *   Absolute path to the PHP executable.
 */
function worker_php_path() {
  // Determine the location of the PHP executable from whereis command output.
  if (!(($php = current(worker_execute_output('whereis php'))) && ($php = explode(' ', $php)) && !empty($php[1]))) {
    worker_log('Could not determine the location of the PHP executable.');
    return FALSE;
  }
  return $php[1];
}

/**
 * Wrapper for PHP exec() function.
 *
 * All commands executed on the worker should be run through this command or
 * worker_execute_output(). If the exit status is non-zero or verbose mode is
 * enabled, then the exit status and output will be logged.
 *
 * @param $command
 *   Command to execute.
 * @param $return_output
 *   (Optional) Return the output of the command instead of TRUE or FALSE.
 * @param $capture_stderr
 *   (Optional) Capture the stderr as part of the output.
 * @return
 *   If successfull then TRUE or the output of the command, otherwise FALSE.
 */
function worker_execute($command, $return_output = FALSE, $capture_stderr = TRUE) {
  // Redirect the stderr to stdout so it is also captured.
  if ($capture_stderr) {
    $command .= ' 2>&1';
  }

  // Execute the command and capture the output and status.
  exec($command, $output, $status);

  if ($status != 0 || worker_verbose()) {
    // Log the exit status.
    worker_log("The following command generated an exit status of $status:\n" .
      "$command\n" .
      ($output ? trim(implode("\n", $output)) : '<<no output>>'));
  }
  return $return_output ? $output : $status == 0;
}

/**
 * Wrapper for PHP exec() function that returns the output.
 *
 * @param $command
 *   Command to execute.
 * @param $capture_stderr
 *   Capture the stderr as part of the output.
 * @return
 *   If successfull then TRUE or the output of the command, otherwise FALSE.
 * @see worker_execute()
 */
function worker_execute_output($command, $capture_stderr = TRUE) {
  return worker_execute($command, TRUE, $capture_stderr);
}

/**
 * Execute a task in the background so the calling function does not block.
 *
 * @param $command
 *   Script to be placed in file and executed in the background. The script may
 *   span multiple lines since it will be written to a file.
 * @param $done
 *   (Optional) Command(s) to be executed after $command has been completed.
 *   Note that the background process will be considered done by
 *   worker_execute_background_done() and the related script file will have
 *   been deleted before any commands in $done are executed.
 * @return
 *   The process number to use with worker_execute_background_done().
 * @see worker_execute_background_done()
 * @see worker_execute_concurrent()
 */
function worker_execute_background($command, $done = NULL) {
  $number = &drupal_static(__FUNCTION__, 0);
  $file = FILE_ROOT . '/process.' . ++$number;
  // Add command to delete the script file after $command finishes so we can
  // test the file's existence in worker_execute_background_done().
  file_put_contents($file, "$command\nrm $file\n$done");
  shell_exec("sh $file > /dev/null 2> /dev/null &");
  return $number;
}

/**
 * Determine if the background process has completed.
 *
 * @param $number
 *   Process number.
 * @return
 *   TRUE if the process has completed, otherwise FALSE.
 * @see worker_execute_background()
 * @see worker_execute_concurrent()
 */
function worker_execute_background_done($number) {
  return !file_exists(FILE_ROOT . '/process.' . $number);
}

/**
 * Execute a list of commands concurrently.
 *
 * @param $commands
 *   List of commands to be executed, see worker_execute_background().
 * @param $sleep
 *   (Optional) Time in micro seconds to sleep between checking the state of
 *   processes. For tasks with longer execution times you can set the sleep
 *   higher. The default is one second.
 * @param $concurrency
 *   (Optional) Number of concurrent processes to keep running at the same
 *   time. The default is two, but can be overridden using
 *   $conf['worker_concurrency'] in settings.php.
 * @return
 *   Associative array keyed by the keys found in $commands and containing the
 *   length of time, in seconds, that the task took to complete.
 * @see worker_execute_background()
 * @see worker_execute_background_done()
 */
function worker_execute_concurrent(array $commands, $sleep = 1000000, $concurrency = NULL) {
  if (!$concurrency) {
    $concurrency = variable_get('worker_concurrency', 2);
  }

  // Loop until all commands have executed and all processes have completed.
  $processes = array();
  $log = array();
  while ($commands || $processes) {
    // Prune list of active processes.
    foreach ($processes as $key => $process) {
      list($process, $start) = $process;
      if (worker_execute_background_done($process)) {
        unset($processes[$key]);
        $log[$key] = microtime(TRUE) - $start;
        worker_log('> Finished ' . $key . ' in ' . number_format($log[$key], 2) . ' seconds.');
      }
    }

    // Ensure that the maximum number of processes are running.
    while ($commands && count($processes) < $concurrency) {
      list($key, $command) = each($commands);
      $process = worker_execute_background($command);
      $processes[$key] = array($process, microtime(TRUE));
      unset($commands[$key]);
    }

    usleep($sleep);
  }
  return $log;
}

/**
 * Wrapper for PHP chdir() function, that starts from JOB_ROOT.
 *
 * @param $directory
 *   Directory within the job root to switch to.
 */
function worker_chdir($directory = '') {
  chdir(JOB_ROOT . '/' . $directory);
}

/**
 * Delete a path recursively.
 *
 * Replacement for file_unmanaged_delete_recursive() since the latter will not
 * always delete symbolic links which may be contained in a directory.
 *
 * @param $path
 *   Path to directory or file that will be recursively deleted.
 * @return
 *   TRUE if successfull, otherwise FALSE.
 */
function worker_delete_recursive($path) {
  if (is_dir($path)) {
    // Cycle through all entries in directory and recursively remove them.
    $files = scandir($path);
    $status = TRUE;
    foreach ($files as $file) {
      if ($file != '.' && $file != '..') {
        $status = worker_delete_recursive($path . '/' . $file) && $status;
      }
    }

    // Remove the directory itself.
    return rmdir($path) && $status;
  }
  // Do not check if the file exists since the check may fail if the file is
  // a symbolic link and the file should always exist.
  return unlink($path);
}

/**
 * Add the string to the log.
 *
 * @param $string
 *   Message string to log.
 */
function worker_log($string) {
  static $start, $log_file;

  // First time function is called then initialze the $start to the current
  // float version of microtime(), generate the $log_file uri and create a
  // blank log file.
  if (!$start) {
    $start = microtime(TRUE);
    $log_file = DRUPAL_ROOT . '/' . file_directory_path() . '/job.log';
    file_put_contents($log_file, '');
  }

  // Write the log message with length of time since logging was started and
  // the message followed by a newline character.
  file_put_contents($log_file, sprintf("[%10s] %s\n", number_format(microtime(TRUE) - $start, 6), $string), FILE_APPEND);
}

/**
 * Wrapper for worker_log() that formats a list of values.
 *
 * @param $string
 *   Message string to log.
 * @param $list
 *   List of values to be printed in log.
 */
function worker_log_list($string, $list) {
  worker_log($string . "\n > " . implode("\n > ", $list));
}

/**
 * Get the last message from the log.
 *
 * @param $log
 *   (Optional) Array containing the lines of log file instead of loading.
 * @return
 *   Last message in log.
 */
function worker_log_last($log = NULL) {
  // If no log was passed then attempt to load log from file.
  if (!$log) $log = file(file_directory_path() . '/job.log', FILE_IGNORE_NEW_LINES);

  // Return last message from log after trimming off the time offset.
  if ($log) return ltrim(substr(array_pop($log), 13), '> ');

  return 'Failed to extract last log message.';
}

/**
 * Log the worker module versions either from `git describe` or .info version.
 */
function worker_log_version() {
  $plugins = worker_plugins_get_all('worker');
  $versions = array();
  foreach ($plugins as $plugin) {
    $module = $plugin['module'];
    if (!isset($versions[$module])) {
      $path = drupal_get_path('module', $plugin['module']);
      if (file_exists($path . '/.git')) {
        $version = current(worker_execute_output('cd ' . escapeshellarg($path) . '; git describe --always --tags --long --dirty="*"'));
      }
      else {
        $info = system_get_info('module', $module);
        $version = $info['version'];
      }

      $versions[$module] = $module . ' ' . $version;
    }
  }
  ksort($versions);
  worker_log('Version ' . implode(', ', $versions));
}

/**
 * Check if worker is in verbose mode.
 *
 * @param
 *   (Optional) Verbose mode to use, this should only be set by worker init.
 * @return
 *   TRUE if in verbose mode, otherwise FALSE.
 */
function worker_verbose($verbose = NULL) {
  static $_verbose;
  if ($verbose) {
    $_verbose = (bool) $verbose;
  }
  return $_verbose;
}

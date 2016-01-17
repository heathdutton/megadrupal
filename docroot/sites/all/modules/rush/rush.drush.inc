<?php
/**
 * @file
 * Drush Rush commands.
 * Drush tools to enhance and improve Drupal development operations and work
 * flow.
 * The purpose of Rush is to allow Drupal Developers to:
 *   1. Reduce typical Drupal development work flows down to one step.
 *   2. Create re-usable and share-able Drupal development operations
 *     & work flows.
 *   3. To provide basic auto build & re-build capabilities using "jobs"
 *     similar to Jenkins.
 * You can copy this file to any of the following:
 *   1. A .drush folder in your HOME folder.
 *   2. Anywhere in a folder tree below an active module on your site.
 *   3. /usr/share/drush/commands (configurable)
 *   4. In an arbitrary folder specified with the --include option.
 *   5. Drupal's sites/all/drush folder.
 */

require dirname(__FILE__) . '/rush.inc';

/**
 * @addtogroup bootstrap Bootstrap
 * @brief Functions necessary for Rush to run.
 * @{
 */

/**
 * @addtogroup drushhooks Drush Hooks
 * @brief Drush hooks implemented by Rush.
 * @{
 */

/**
 * Implements hook_drush_command().
 */
function rush_drush_command() {

  $items = array();

  // The 'rush' command.
  $items['rush'] = array(
    'description' => "Run a rush file with rush operations in it.",
    'arguments' => array(
      'job' => 'The name of the job directory within the Rush Application folder.  No argument will result in a select list of jobs.',
    ),
    'options' => array(
      'show' => array(
        'description' => 'Show mode. Utility to inspect operations or params without actually running the job.',
        'example-value' => 'ops,params, See drush rush-show -h for more examples.',
      ),
      'select' => array(
        'description' => 'Flag to tell rush to list jobs from another location.  Only --select=jt or --select=job-templates are used at this time to list jobs from the job templates folder.',
        'example-value' => 'job-templates,jt',
      ),
      'domain' => array(
        'description' => 'Domain to use for jobs that use the [build][domain] parameter.',
        'example-value' => 'mydomain.local',
      ),
      'rush' => array(
        'description' => 'Rush prefix to select a different rush.ini or rush.php to run.',
        'example-value' => 'un,pre-build,post-build',
      ),
      'branch' => array(
        'description' => 'Branch to use for jobs that use the [repo][branch] parameter.  This will affect jobs that run the rush clone operation.  Passing this parameter at runtime is a good way to clone a different git branch to run the rush job against.',
        'example-value' => 'develop,master,sprint1,new-feature',
      ),
      'new-branch' => array(
        'description' => 'New branch name to use for jobs that use the [repo][new_branch] parameter.  This will affect jobs that run the rush branch operation.  Passing this parameter at runtime is a good way to create new feature branches that start from the branch defined in the rush job.',
        'example-value' => 'new~feature,experiment',
      ),
      'create-branch' => array(
        'description' => 'Create a new branch with the specified name, after all rush operations have run.  This option runs `git branch` after the rush job.  It differs from the `new-branch` option in that this option runs at the end of the job and does not need the rush branch operation to be present in the rush file.',
        'example-value' => 'new~feature,experiment',
      ),

    ),
    'examples' => array(
      'drush rush jobName' => 'Run the rush files within the job directory named "jobName"',
      'drush rush jobName.subJob' => 'Run the rush files within the job directory named "jobName/subJob"',
      'drush rush jobName --show=ops' => 'Parse rush files and output the rush operations that would occur if the rush file were run.',
      'drush rush jobName --show=params' => 'Parse rush param files and output all params (rush jobName plus global directory) for inspection.',
      'drush rush jobName --rush=clean' => 'Run the rush job within the "jobName" and use the rush file `clean.rush.ini` or `clean.rush.php`',
      'drush rush jobName --branch=develop' => 'Run the rush job and clone from the `develop` branch, overriding the branch set in the job ini files.',
      'drush rush jobName --new-branch=experiment' => 'Run the rush job and create a new branch called `experiment` when the `git_branch` rush operation is called.',
    ),
    'aliases' => array('dr'),
    // No bootstrap at all.
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  $items['rush-list'] = array(
    'description' => "List the rush jobs in the jobs directory.",
    'options' => array(
      'show' => array(
        'description' => 'Show mode. Utility to inspect operations or params without actually running the job.',
        'example-value' => 'job-templates,jt',
      ),
    ),
    'examples' => array(
      'drush rush-list' => 'List all the jobs in the job directory.',
      'drush rush-list --show=jt' => 'List all the jobs in the job templates directory.',
    ),
    'aliases' => array('drl'),
    // No bootstrap at all.
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  $items['rush-show'] = array(
    'description' => 'A utility to inspect operations or params without actually running a job.  This is a convenience command which does the same thing as running rush with the --show option.',
    'arguments' => array(
      'job' => 'Required. The name of the job directory within the Rush Application folder.',
      'show_mode' => 'Required. The name of the job directory within the Rush Application folder.',
    ),
    'options' => array(
      'domain' => array(
        'description' => 'Domain to use for jobs that use the [build][domain] parameter.',
        'example-value' => 'mydomain.local',
      ),
      'rush' => array(
        'description' => 'Rush prefix to select a different rush.ini or rush.php to run.',
        'example-value' => 'un,pre-build,post-build',
      ),
    ),
    'examples' => array(
      'drush rush-show jobName params' => 'Show all parameters the job may use.',
      'drush rush-show jobName ops' => 'Show all ops the job will run.',
      'drush rush-show jobName help' => 'Show the job description and help text.',
      'drush rush-show jobName h' => 'Alias for --show=help',
      'drush rush-show jobName derived ' => 'Show the parameters which rush derived for the job.',
      'drush rush-show jobName si' => 'Show the parameters which the si operation would use if run.',
      'drush rush-show jobName db' => 'Show the parameters which db operations would use if run.',
      'drush rush-show jobName repo' => 'Show the parameters which repo operations would use if run.',
      'drush rush-show jobName env' => 'Show the environment parameters.',
      'drush rush-show jobName build' => 'Show all build parameters the job may use.',
      'drush rush-show jobName paths' => 'Show all paths the job may use.',
    ),
    'aliases' => array('drs'),
    // No bootstrap at all.
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  // Add docs topic.
  $docs_dir = rush_self_path() . 'operations/';

  $items['rush-docs'] = array(
    'description' => 'Rush: Topics List',
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => '_rush_docs_selection',
  );

  $ops = array(
    'db',
    'repo',
    'site',
    'utilities',
    'filesystem',
    'host',
  );

  $items['rush-ops-tables'] = array(
    'description' => 'Rush: Display a summary tables of rush operations.',
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => '_rush_print_ops_tables',
    'callback arguments' => array($ops),
  );

  $items['docs-rush-ops-db'] = array(
    'description' => 'Rush: DB operations.',
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'rush_print_function_blocks',
    'callback arguments' => array($docs_dir . 'rush.db.inc'),
  );

  $items['docs-rush-ops-repo'] = array(
    'description' => 'Rush: Repo operations.',
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'rush_print_function_blocks',
    'callback arguments' => array($docs_dir . 'rush.repo.inc'),
  );

  $items['docs-rush-ops-site'] = array(
    'description' => 'Rush: Site operations.',
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'rush_print_function_blocks',
    'callback arguments' => array($docs_dir . 'rush.site.inc'),
  );

  $items['docs-rush-ops-host'] = array(
    'description' => 'Rush: Host operations.',
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'rush_print_function_blocks',
    'callback arguments' => array($docs_dir . 'rush.host.inc'),
  );

  $items['docs-rush-ops-filesystem'] = array(
    'description' => 'Rush: Filesystem operations.',
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'rush_print_function_blocks',
    'callback arguments' => array($docs_dir . 'rush.filesystem.inc'),
  );

  $items['docs-rush-ops-utility'] = array(
    'description' => 'Rush: Utility operations.',
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'rush_print_function_blocks',
    'callback arguments' => array($docs_dir . 'rush.utilities.inc'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function rush_drush_help($section) {

  switch ($section) {
    case 'drush:rush':
      return dt('Run rush operations defined in Rush job files. Drush rush is a tool to enhance and improve Drupal development operations and work flow. Run `drush rush-docs` for further help.');

    case 'drush:rush-list':
      return dt('List all the jobs in the Rush Application folder jobs directory.');

  }
}

/**
 * Implements drush hook_COMMAND_validate().
 *
 * @backlog Error check for the presence of the rush file in the directory.
 */
function drush_rush_validate() {

  $args = drush_get_arguments();
  if (isset($args[2])) {
    return drush_set_error('EXTRA_ARGUMENT', dt('It looks like you added a second argument.  Did you mean to add a parameter instead, such as --show=params?'));
  }
}

/**
 * Implements drush hook_COMMAND_validate().
 *
 * @backlog Error check for the presence of the rush file in the directory.
 */
function drush_rush_list_validate() {

  $args = drush_get_arguments();
  if (isset($args[1])) {
    return drush_set_error('EXTRA_ARGUMENT', dt('rush-list does not use arguments.  Perhaps you meant to pass a parameter such as --show=jt?'));
  }
}

/** @}*/

/**
 * @addtogroup commandcallbacks Drush Command Callbacks
 * @brief Callback functions for each Drush Rush command.
 * @{
 */

/**
 * Drush rush command callback.
 *
 * Run a rush file with commands in it.
 *
 * @see drush_invoke()
 * @see drush.api.php
 * @backlog This does not seem to stop on all operation errors.
 * @backlog Refactor switch case statements.
 * @return bool
 *   FALSE on failure.
 */
function drush_rush() {
  $args = drush_get_arguments();
  // Check for the job argument.
  if (!isset($args[1])) {
    $global_locations = rush_get_global_locations();
    // Set default values.
    $directory_path = $global_locations['RUSH_JOBS_DIRECTORY_PATH'];
    $directory_name = $global_locations['RUSH_JOBS_DIRECTORY_NAME'];
    $rush_dir_name = $global_locations['RUSH_HOME_DIRECTORY_NAME'];

    if (drush_get_option('select', FALSE)) {
      $select = drush_get_option('select');
      switch ($select) {
        case 'job-templates':
        case 'jt':
          $directory_path = $global_locations['RUSH_JOB_TEMPLATES_DIRECTORY_PATH'];
          $directory_name = $global_locations['RUSH_JOB_TEMPLATES_DIRECTORY_NAME'];
          $rush_dir_name = trim($global_locations['RUSH_MODULE_DIRECTORY_PATH'], '/');
          break;
      }
    }

    // Get the jobs list.
    $jobs = _rush_list_jobs($directory_path, $directory_name, $rush_dir_name);
    $jobs = drush_map_assoc($jobs);
    // Present a list of jobs as options to the user.
    $job = drush_choice($jobs, 'Enter a number to choose which job to run.', '!key');
    if ($job !== FALSE) {
      $args[1] = $jobs[$job];
      drush_set_arguments($args);
    }
    else {
      $msg = dt('[Rush]: !message', array('!message' => 'Canceled'));
      drush_log($msg, 'status');

      return;
    }
  }

  $params = rush_get_params();
  $rush_operations = rush_get_operations($params);
  if (drush_get_option('create-branch', FALSE)) {
    $rush_operations[]['f'] = 'git_create_branch';

  }
  if (drush_get_option('show', FALSE)) {
    $show_mode = drush_get_option('show');

    return _rush_show($params, $show_mode, $rush_operations);
  }

  $msg = dt('[ERROR]: An error occurred while running this drush operation: ');
  // @backlog Make a progress meter based on number of operations to run.
  // @backlog Have a finished message.
  foreach ($rush_operations as $op_array) {
    foreach ($op_array as $op_type => $op) {
      switch ($op_type) {
        // The op_type is a function.
        case 'f':
          if (rush_function_exec($op, $params) === FALSE) {
            return drush_set_error('DRUSH_RUSH_OP_ERROR', $msg . $op);
          }
          break;

        // The op_type is a command.
        case 'c':
          if (rush_command_exec($op, $params) === FALSE) {
            return drush_set_error('DRUSH_RUSH_OP_ERROR', $msg . $op);
          }
          break;

        // The op_type is a job to run.
        case 'j':
          if (rush_job_exec($op, $params) === FALSE) {
            return drush_set_error('DRUSH_RUSH_OP_ERROR', $msg . $op);
          }
          break;

        // The op_type is a message.
        case 'm':
          if (rush_message_exec($op, $params) === FALSE) {
            return drush_set_error('DRUSH_RUSH_OP_ERROR', $msg . $op);
          }
          break;
      }
    }
  }
}

/**
 * Drush rush-list command callback.
 *
 * List all the jobs in the Rush Application folder jobs directory.
 *
 * @see drush_invoke()
 * @see drush.api.php
 *
 * @return bool
 *   TRUE on success, FALSE on failure.
 */
function drush_rush_list() {

  $global_locations = rush_get_global_locations();
  // Set default values.
  $msg = rush_message_wrapper(dt('JOBS LIST'), 0);
  $directory_path = $global_locations['RUSH_JOBS_DIRECTORY_PATH'];
  $directory_name = $global_locations['RUSH_JOBS_DIRECTORY_NAME'];
  $rush_dir_name = $global_locations['RUSH_HOME_DIRECTORY_NAME'];

  // If the show option is set to jt then list jobs in the job_templates
  // directory.
  if (drush_get_option('show', FALSE)) {
    $show_mode = drush_get_option('show');
    switch ($show_mode) {
      case 'job-templates':
      case 'jt':
        $msg = rush_message_wrapper(dt('JOB TEMPLATES LIST'), 0);
        $directory_path = $global_locations['RUSH_JOB_TEMPLATES_DIRECTORY_PATH'];
        $directory_name = $global_locations['RUSH_JOB_TEMPLATES_DIRECTORY_NAME'];
        $rush_dir_name = trim($global_locations['RUSH_MODULE_DIRECTORY_PATH'], '/');
        break;

      case 'locations':
      case 'l':
        $msg = rush_message_wrapper(dt('GLOBAL LOCATIONS'), 0);
        drush_print($msg);
        ksort($global_locations);
        drush_print_r($global_locations);

        return TRUE;

      default:
        $msg = dt('[ERROR]: *** The --show option "!option" is not a valid value. Check the spelling of the value. ***', array('!option' => $show_mode));

        return drush_set_error('DRUSH_RUSH_INVALID_OPTION', $msg);
    }
  }

  // Tell the user what list they will get.
  drush_print($msg);
  // Get the jobs list.
  $jobs = _rush_list_jobs($directory_path, $directory_name, $rush_dir_name);
  foreach ($jobs as $job) {
    drush_print($job);
  }
  $msg = rush_message_wrapper(dt('END LIST'), 0);
  drush_print($msg);

  return TRUE;
}

/**
 * Drush rush-show command callback.
 *
 * A utility to inspect operations or params without actually running a job.
 *
 * @see drush_invoke()
 * @see drush.api.php
 */
function drush_rush_show() {

  $params = rush_get_params();
  $rush_operations = rush_get_operations($params);
  $args = drush_get_arguments();
  if (isset($args[2])) {
    $show_mode = $args[2];

    return _rush_show($params, $show_mode, $rush_operations);
  }

  return drush_set_error('RUSH COMMAND ERROR', 'rush-show missing show argument.');

}

/** @}*/

/**
 * @addtogroup command_execution Rush Command Executions
 * @brief Functions used by the `drush rush` command to execute operations.
 * @{
 */

/**
 * Run a command through drush_shell_exec().
 *
 * @param string $command
 *   Required.  A string with the command to run through drush_shell_exec.
 * @param array $params
 *   (Optional) An associative array of all rush parameters.  Not used yet.
 *
 * @return bool
 *   TRUE on success.  FALSE on fail via drush_set_error.
 * @backlog Look for a better way to add url to drush functions.
 * It may not always be accurate this way.
 * @backlog Would like an away to abort if an operation failed.
 */
function rush_command_exec($command, $params = array()) {

  // If this is a drush command then run it in the docroot directory.
  $pos = strpos($command, 'drush');
  // Using != would not work as expected
  // because a position can be 0. The statement (0 != false) evaluates
  // to false.
  if ($pos !== FALSE) {
    $command = 'cd ' . $params['build']['docroot_path'] . '; ' . $command . ' --uri=' . $params['build']['site_uri'];
  }
  // If it's not a drush command, then run it in the build directory.
  else {
    $command = 'cd ' . $params['build']['directory_path'] . '; ' . $command;
  }
  if (drush_shell_exec($command)) {
    $msg = dt('[RUSH COMMAND EXEC]: !command', array('!command' => $command));
    drush_log($msg, 'success');

    return TRUE;
  }
  else {
    $msg = dt('!command', array('!command' => $command));

    return drush_set_error('RUSH_COMMAND_EXEC_FAILED', $msg);

  }
}

/**
 * Run another Rush Job via drush_shell_exec().
 *
 * @param string $job
 *   Required.  A string with the job to run through drush_shell_exec.
 *
 * @return bool
 *   TRUE on success.  FALSE on fail via drush_set_error.
 */
function rush_job_exec($job) {

  $command = 'drush rush ' . $job;
  if (drush_shell_exec($command)) {
    $result = drush_shell_exec_output();
    drush_print($result);

    $msg = dt('[RUSH JOB EXEC]: !command', array('!command' => $command));
    drush_log($msg, 'success');

    return TRUE;
  }
  else {
    $msg = dt('!command', array('!command' => $command));

    return drush_set_error('RUSH_JOB_EXEC_FAILED', $msg);

  }
}

/**
 * Print a rush message through drush_log().
 *
 * @param string $message
 *   Required. A string with the message to print through drush_log().
 *
 * @return bool
 *   TRUE.
 * @note No real error checking needed right now.
 */
function rush_message_exec($message) {

  $msg = dt('[RUSH MESSAGE]: !message', array('!message' => $message));
  drush_log($msg, 'status');

  return TRUE;
}

/**
 * Execute the defined rush function.
 *
 * @param string $function
 *   Required. A string with the rush function to run.
 * @param array $params
 *   Required.  An associative array of all rush parameters.
 *
 * @return bool
 *   TRUE on success, FALSE on fail via drush_set_error.
 * @backlog Check the return of the function for errors.
 */
function rush_function_exec($function, $params) {

  $operation = 'rush_op_' . $function;
  if (function_exists($operation)) {
    $msg = dt('[RUSH OPERATION CALL]: !function', array('!function' => $function));
    drush_log($msg, 'success');

    return $operation($params);
  }
  else {
    $msg = dt('!function does not exist or has not been included.', array('!function' => $operation));

    return drush_set_error('RUSH_FUNCTION_EXEC_FAILED', $msg);
  }
}

/** @}*/
/** @}*/

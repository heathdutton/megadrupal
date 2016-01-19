<?php

/**
 * @file
 * @todo
 */

include_once 'drake.context.inc';
include_once 'drake.files.inc';
include_once 'drake.actions.inc';

define('DRAKE_API_VERSION', 1);
define('DRAKE_RCFILE_REGEX', '{^(.*\.)?drakefile.drush(' . DRUSH_MAJOR_VERSION . '|)rc.php$}');
/**
 * Implements hook_drush_command().
 */
function drake_drush_command() {
  $items['drake'] = array(
    'description' => 'Do something.',
    'arguments' => array(
      'target' => 'Task to make.',
    ),
    'options' => array(
      'file' => 'Use specified file.',
      'show-tasks' => 'List available tasks.',
      'show-files' => 'List used drakefiles.',
      'show-context' => 'List the context for the given (or default) task.',
      'ignore-current-dir' => 'Ignore drakefile.php in the current directory.',
      'all' => '(with --show-tasks) list all tasks, not just those with description.',
      'skip-tasks' => 'Skip these tasks (comma separated).',
      'no-task-ok' => "Don't fail if the task is not found.",
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  // Hidden command to make a single task.
  $items['drake-process'] = array(
    'hidden' => TRUE,
    'arguments' => array(
      'target' => 'The target to build.',
    ),
    'options' => array(
      'drake' => 'The collected tasks and actions.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function drake_drush_help($section) {
  switch ($section) {
    case 'meta:drake:title':
      return dt('Drake');

    case 'meta:drake:summary':
      return dt('Do stuff.');

    case 'drush:drake':
      return dt("Does stuff...");
  }
}

/**
 * Command callback.
 *
 * Executes the main drake command.
 */
function drush_drake() {
  // Drake files to include.
  $include_files = array();
  // Drake files to load.
  $files = array();

  $target = NULL;
  // Sort out arguments and find a target, if specified.
  $arguments = func_get_args();
  $global_context = new DrakeContext();
  $i = 1;
  $first_argument = TRUE;
  // Create context from command line arguments.
  foreach ($arguments as $argument) {
    // If the argument contains an =, it's a key=value argument.
    $arg = preg_split('/(?<!\\\)=/', $argument, 2);
    if (!isset($arg[1])) {
      // Skip the first argument. We assume its the target.
      if (!$target) {
        $target = $arg[0];
        continue;
      }
      else {
        // If no = was found, name it sequentially.
        $arg[1] = $arg[0];
        $arg[0] = 'arg' . $i++;
        $first_argument = FALSE;
      }
    }
    // Remove = escaping.
    $arg = preg_replace('/\\\=/', '=', $arg);
    $global_context->force($arg[0], $arg[1]);
  }

  // Include common system wide drakefiles and drakefiles in .drush.
  $dirs = array();
  // In the system wide configuration folder.
  $dirs[] = drush_get_context('DRUSH_SITE_WIDE_CONFIGURATION');

  // User .drush.
  $per_user_config_dir = drush_get_context('DRUSH_PER_USER_CONFIGURATION');
  if (!empty($per_user_config_dir)) {
    $dirs[] = $per_user_config_dir;
  }

  foreach ($dirs as $dir) {
    $include_files = array_merge($include_files, array_keys(drush_scan_directory($dir, DRAKE_RCFILE_REGEX)));
  }

  $file = drush_get_option('file', NULL);
  if (empty($file)) {
    // Look around if no file specified.
    // Check current directory if alias was not defined on the command line.
    if (!drush_get_context('DRUSH_TARGET_SITE_ALIAS')) {
      // Use a drakefile file in the current directory.
      if (file_exists('drakefile.php') && !drush_get_option('ignore-current-dir', FALSE)) {
        $files[] = 'drakefile.php';
      }
    }
    // Add sites/all/drush if we have a boostrapped site.
    $drupal_root = drush_get_context('DRUSH_SELECTED_DRUPAL_ROOT');
    if (!empty($drupal_root)) {
      $paths = array(
        $drupal_root . '/drush',
        $drupal_root . '/sites/all/drush',
      );
      foreach ($paths as $path) {
        $all_sites_file = $path . '/drakefile.php';
        if (file_exists($all_sites_file)) {
          // If there aren't a drakefile in the current directory, consider this
          // the main one, else use it as an include.
          if (empty($files)) {
            $files[] = $all_sites_file;
          }
          else {
            $include_files[] = $all_sites_file;
          }
        }
        // Add any drushrc drakefiles from any commands in sites/all/drush.
        $include_files = array_merge($include_files, array_keys(drush_scan_directory($path, DRAKE_RCFILE_REGEX)));
      }

      // @todo: consider to load sites/<site>/drakefile.php. Check out what
      //   _drush_config_file does.
    }
    // If the above two didn't find anything, make a last ditch effort of
    // looking in the site profiles.
    if (empty($files) && !empty($drupal_root)) {
      $exclude = array('.', '..', 'CVS');
      $profiles = array_keys(drush_scan_directory($drupal_root . '/profiles', '{.*}', $exclude, 0, FALSE));
      foreach ($profiles as $profile) {
        $drakefile = $profile . '/drakefile.php';
        if (file_exists($drakefile)) {
          $files[] = $drakefile;
        }
      }
      // Error out if there's more than one candidate.
      if (count($files) > 1) {
        return drush_set_error('MULTIPLE_DRAKEFILES', dt('Multiple profile drakefile.phps found, please use --file to specify which to use.'));
      }
    }
  }
  else {
    // A file was specified, use that.
    $url = parse_url($file);
    if (!empty($url['scheme'])) {
      // Remote file, save it to a temp file so we can include it.
      $data = file_get_contents($file);
      if ($data === FALSE) {
        return drush_set_error('FETCH_ERROR', dt('Could not fetch drakefile from @uri.', array('@uri' => $file)));
      }
      else {
        $file = drush_save_data_to_temp_file($data, '.php');
      }
    }
    elseif (!file_exists($file)) {
      // Check that the file exists.
      return drush_set_error('MISSING_FILE', dt('File @file not found.', array('@file' => $file)));
    }
    $files[] = $file;
  }

  // Bail out if no non-include files and no target was specified, or there's no
  // drakefiles at all.
  if (empty($files) && (empty($target) || empty($include_files))) {
    return drush_set_error('NO_DRAKEFILE_FILE', dt('No drakefile.php files found or specified.'));
  }

  // Merge in reverse order as we reverse the list afterwards.
  $files = array_merge($include_files, $files);

  // @todo make this a separate command?
  if (drush_get_option('show-files', FALSE)) {
    drush_print(dt('Loaded drakefiles:'));
    foreach ($files as $file) {
      drush_print($file);
    }
    return;
  }

  $tasks = array();
  $actions = drake_standard_actions();

  // Set up global context.
  $self_record = drush_sitealias_get_record('@self');
  if (!empty($self_record)) {
    // Oddly, it might be '@self'.
    if ($self_record['#name'] == '@self') {
      $global_context['@self'] = '@self';
    }
    else {
      $global_context['@self'] = '@' . $self_record['#name'];
    }
  }

  $realpaths = array();
  $start_task = NULL;
  foreach ($files as $file) {
    $realpath = realpath($file);
    if (isset($realpaths[$realpath])) {
      continue;
    }
    $realpaths[$realpath] = TRUE;
    $context = new DrakeContext($global_context);
    $res = _drake_load_file($file, $context);
    if ($res === FALSE) {
      return $res;
    }
    if (!empty($res['tasks'])) {
      $tasks = array_merge($tasks, $res['tasks']);
      // The first task in non-include file is the default task to run.
      if (!$start_task && !in_array($file, $include_files)) {
        $task_names = array_keys($res['tasks']);
        $start_task = $task_names[0];
      }
    }
    if (!empty($res['actions'])) {
      $actions = array_merge($actions, $res['actions']);
    }
    if (!empty($res['filesets'])) {
      foreach ($res['filesets'] as $name => $def) {
        fileset_create($name, $def);
      }
    }
  }

  // Add array key as name in sub-arrays..
  _drake_index_array($tasks, 'name');
  _drake_index_array($actions, 'name');

  // @todo make this a separate command?
  if (drush_get_option('show-tasks', FALSE)) {
    $all = drush_get_option('all', FALSE);
    ksort($tasks);
    $rows = array(array(dt('Task'), dt('Description')));
    foreach ($tasks as $task) {
      $help = NULL;
      if (!empty($task['help'])) {
        $help = $task['help'];
      }
      elseif ($all) {
        $help = "";
      }
      if (!is_null($help)) {
        if ($task['depends']) {
          if ($help) {
            $help .= "\n";
          }
          $help .= "(Depends on: " . implode(', ', $task['depends']) . ")";
        }

        $rows[] = array($task['name'], $help);
      }
    }
    if (count($rows) > 1) {
      drush_print(dt('Available tasks:'));
      drush_print_table($rows, TRUE);
    }
    elseif ($all) {
      drush_log(dt('No tasks found.'), 'error');
    }
    else {
      drush_log(dt('No tasks with descriptions found.'), 'status');
    }
    return;
  }

  // Cleanup tasks. Move this to a dedicated function when it gets too
  // complicated.
  foreach ($tasks as $name => $task) {
    // Allow single dependencies to be specified as a string.
    if (isset($task['depends']) && is_string($task['depends'])) {
      $task['depends'] = array($task['depends']);
    }
    $tasks[$name] = $task;
  }

  // Find the main task we're doing, if specified on the command line.
  if (!empty($target)) {
    if (!isset($tasks[$target])) {
      if (drush_get_option('no-task-ok', FALSE)) {
        // Just return quietly.
        return;
      }
      else {
        return drush_set_error('UNKNOWN_TASK', dt('Unknown task @task', array('@task' => $target)));
      }
    }
    $start_task = $target;
  }

  // Use a task array instead of task name.
  $start_task = $tasks[$start_task];

  if ($skip_tasks = drush_get_option('skip-tasks', '')) {
    drush_set_context('DRAKE_SKIP_TASKS', explode(',', $skip_tasks));
  }

  // Add in dependent tasks.
  if (!($run_tasks = drake_resolve_dependencies($start_task, $tasks))) {
    return FALSE;
  }

  // Check that we can run the given tasks.
  try {
    foreach ($run_tasks as &$task) {
      if (!_drake_prepare_task($task, $actions)) {
        return FALSE;
      }
    }
  }
  catch (DrakeContextException $e) {
    drush_set_error('CONTEXT_EXCEPTION', dt('Task @task: @message', array('@task' => $task['name'], '@message' => $e->getMessage())));
    return FALSE;
  }

  // Unset reference, else the following foreach loop will munge the last task.
  unset($task);

  if (drush_get_option('show-context', FALSE)) {
    $context = $start_task['#context']->toArray();
    if (!empty($context)) {
      drush_print(dt('Context for the "@task" task:', array('@task' => $start_task['name'])));
      $rows = array(array(dt('Name'), dt('Value')));
      foreach ($context as $name => $value) {
        $rows[] = array($name, (string) $value);
      }
      drush_print_table($rows, TRUE);
    }
    else {
      drush_print(dt('No context variables for "@task" task.', array('@task' => $start_task['name'])));
    }
    return;
  }

  $total_tasks = 0;
  foreach ($run_tasks as $task) {
    if (!empty($task['action'])) {
      $total_tasks++;
    }
  }
  $task_num = 1;
  $dir = getcwd();
  foreach ($run_tasks as $task) {
    try {
      $message_suffix = "\n";
      if (!empty($task['action'])) {
        $message_suffix = ' [' . $task_num++ . '/' . $total_tasks . ']';
      }

      foreach (_drake_task_messages($task, $message_suffix) as $message) {
        drush_log($message . "\n", 'status');
      }

      // We need drush_invoke_process to use post for redispatching, as using
      // get will loose our drake command option (as it is an array).
      $backend_options = array(
        'method' => 'POST',
        '#output-label' => $task['name'] . ': ',
      );
      $command_options = drush_redispatch_get_options();
      // $command_options['yes'] = TRUE;
      $command_options['drake'] = array(
        'task' => serialize($task),
        'files' => $files,
      );
      $result = drush_invoke_process('@self', 'drake-process', array($task['name']), $command_options, $backend_options);

      // Make, situs and other build tools might pull the rug out from under our
      // feet. Reenter the directory in case someone moved a new version in.
      chdir($dir);

      if ($result['error_status'] !== 0) {
        // We assume that _drake_run_task already output a message.
        return FALSE;
      }
      if (!empty($task['action'])) {
        drush_log("Done\n", 'ok');
      }
    }
    catch (Exception $e) {
      drush_set_error('EXCEPTION_CAUGHT', dt('Task @task threw exception: @message', array('@task' => $task['name'], '@message' => $e->getMessage())));
      return FALSE;
    }
  }
}

/**
 * Command callback to process a single target.
 */
function drush_drake_process($target) {
  $drake = drush_get_option('drake', NULL);

  if (empty($drake)) {
    return drush_set_error('NO_TASK', dt('Task not specified for drake-process.'));
  }
  $drake['task'] = unserialize($drake['task']);

  if (!empty($drake['files'])) {
    $realpaths = array();
    // Load the drakefiles to define the functions they contain.
    foreach ($drake['files'] as $file) {
      $realpath = realpath($file);
      if (isset($realpaths[$realpath])) {
        continue;
      }
      $realpaths[$realpath] = TRUE;
      // @todo investigate whether this is the most kosher way to include file.
      require $file;
    }
  }
  if (_drake_run_task($drake['task']) === FALSE) {
    // If error code isn't set, set a generic error message to ensure that our
    // return code is failure..
    if (drush_get_error() == DRUSH_SUCCESS) {
      return drush_set_error('UNKNOWN_ERROR', dt('Task failed with an unknown error.'));
    }
    // Else assume that the task set the message.
    return FALSE;
  }
}

/**
 * Load a drakefile, adding context.
 *
 * Adds context objects to tasks and actions and saves file scope context.
 */
function _drake_load_file($_drake_file, $_drake_context) {
  require $_drake_file;

  if ($api != 1) {
    return drush_set_error('BAD_API', dt("No api version in file. Add in \$api = 1;"));
  }

  // Catch common typos.
  if (isset($task)) {
    drush_log(dt('Variable named $task found in @file, did you mean $tasks?', array('@file' => $_drake_file)), 'warning');
  }
  if (isset($action)) {
    drush_log(dt('Variable named $action found in @file, did you mean $actions?', array('@file' => $_drake_file)), 'warning');
  }
  if (isset($contexts)) {
    drush_log(dt('Variable named $contexts found in @file, did you mean $context?', array('@file' => $_drake_file)), 'warning');
  }

  $return = array();
  if (!empty($tasks)) {
    foreach ($tasks as &$task) {
      $task['#context'] = $_drake_context;
    }
    $return['tasks'] = $tasks;
  }
  if (!empty($actions)) {
    foreach ($actions as &$action) {
      $action['#context'] = $_drake_context;
    }
    $return['actions'] = $actions;
  }
  if (!empty($filesets)) {
    foreach ($filesets as &$fileset) {
      $fileset['#context'] = $_drake_context;
    }
    $return['filesets'] = $filesets;
  }
  if (!empty($context)) {
    foreach ($context as $key => $value) {
      $_drake_context[$key] = $value;
    }
  }
  return $return;
}

/**
 * Return the message for the task.
 */
function _drake_task_messages($task, $suffix = '') {
  static $printed_parents = array();
  $return = array();
  if (!empty($task['#parent']['depends']) && !isset($printed_parents[$task['#parent']['name']])) {
    $return = _drake_task_messages($task['#parent']);
    $printed_parents[$task['#parent']['name']] = TRUE;
  }
  if (!empty($task['message'])) {
    $message = $task['message'];
    if (is_array($message) && function_exists($message[0])) {
      $function = $message[0];
      $message[0] = $task;
      $message = trim(call_user_func_array($function, $message));
    }
    $return[] = $message . $suffix;
  }
  elseif (!empty($task['action']['default_message'])) {
    $return[] = $task['action']['default_message'] . $suffix;
  }
  else {
    if (!isset($printed_parents[$task['name']]) || !empty($task['action'])) {
      $return[] = dt('Doing "@task_name" task @suffix', array('@task_name' => $task['name'], '@suffix' => $suffix));
    }
    else {
      $return[] = dt('Finished "@task_name".', array('@task_name' => $task['name']));
    }
  }
  return $return;
}

/**
 * Check and prepare a task.
 *
 * Ensures that actions are properly configured, handles multiple actions and
 * action parameters.
 */
function _drake_prepare_task(&$task, &$actions) {
  // Resolve contexts. Doing it now allows us to have optional contexts for
  // optional parameters, which will proporly default the parameter when the
  // context is unset.
  $task['#context']->resolve($task);
  if (!empty($task['action'])) {
    if (!isset($actions[$task['action']])) {
      return drush_set_error("UNKNOWN_ACTION", dt('Unknown action "@action".', array('@action' => $task['action'])));
    }

    $action = $actions[$task['action']];
    $task['action'] = $action;

    if (empty($action['callback']) ||
      !function_exists($action['callback'])) {
      return drush_set_error("UNKNOWN_ACTION_CALLBACK", dt('Callback for action "@action" does not exist.', array('@action' => $action['name'])));
    }

    $action += array(
      'multiple' => '',
      'parameters' => array(),
    );

    // Compile default parameters.
    $action_defaults = array();
    foreach ($action['parameters'] as $param_name => $param_settings) {
      if (is_array($param_settings) && (array_key_exists('default', $param_settings))) {
        $action_defaults[$param_name] = $param_settings['default'];
      }
    }
    // Check the action parameters and add in the action(s) on the task.
    $muliple_parm = $action['multiple'];
    if ($muliple_parm && isset($task[$muliple_parm . 's'])) {
      if (!empty($task[$muliple_parm])) {
        return drush_set_error('AMBIGUOUS_PARAM', dt('Action "@action" failed: Both "@singular_parm" and "@plural_parm" cannot be specified.', array(
              '@action' => $action['name'],
              '@singular_parm' => $muliple_parm,
              '@plural_parm' => $muliple_parm . 's',
            )));
      }
      $task_actions = array();
      // Explode the multiple parameter to multiple actions.
      foreach ($task[$muliple_parm . 's'] as $key => $params) {
        // If the key is int, use the value as command.
        if (is_int($key)) {
          // If the params isn't an array, assume it's a simple command string.
          if (!is_array($params)) {
            $params = array($muliple_parm => $params);
          }
        }
        else {
          // Else use the key as the command.
          $params += array($muliple_parm => $key);
        }
        $task_actions[] = $params + $task + $action_defaults;
      }
    }
    else {
      $task_actions = array($task + $action_defaults);
    }

    // Check that required parameters is present.
    foreach ($task_actions as $task_action) {
      $diff = array_diff(array_keys($action['parameters']), array_keys($task_action));
      if (!empty($diff)) {
        return drush_set_error('MISSING_PARAM', dt('Action "@action" failed: Required param(s) not supplied: "@params".', array(
              '@action' => $action['name'],
              '@params' => implode(', ', $diff),
            )));
      }
    }

    $task['#actions'] = $task_actions;
  }
  return TRUE;
}

/**
 * Add the key of the array items as the given value in the sub-array.
 */
function _drake_index_array(&$array, $prop) {
  foreach ($array as $key => $item) {
    $array[$key][$prop] = $key;
  }
}

/**
 * Runs a single task.
 */
function _drake_run_task($task) {
  if (isset($task['#actions'])) {
    foreach ($task['#actions'] as $run_action) {
      $function = $run_action['action']['callback'];
      $context = &drush_get_context('DRAKE_RUN');
      $context['action'] = $run_action['action'];
      $context['task'] = $task;
      if ($function($run_action) === FALSE) {
        $context = array();
        return drush_set_error('TASK_FAILED', dt('Task "@task" failed, aborting.', array('@task' => $task['name'])));
      }
    }
  }
}

/**
 * Resolve task dependencies by doing a depth first search.
 */
function drake_resolve_dependencies(&$task, &$tasks) {
  $resolution = array();
  $seen = array();
  if (!_drake_visit($task, $resolution, $tasks, $seen, array())) {
    return FALSE;
  }
  return $resolution;
}

/**
 * Helper for drake_resolve_dependencies().
 */
function _drake_visit(&$task, &$resolution, &$all_tasks, &$seen, $stack) {
  $skip_tasks = drush_get_context('DRAKE_SKIP_TASKS', array());
  $seen_id = $task['name'] . $task['#context']->md5();
  if (!isset($seen[$seen_id])) {
    $seen[$seen_id] = TRUE;
    $context = $task['#context'];
    if (isset($task['context'])) {
      $context = new DrakeContext($context);
      foreach ($task['context'] as $name => $value) {
        $context[$name] = $value;
      }
      // Set the new context.
      $task['#context'] = $context;
    }
    if (!empty($task['depends'])) {
      // Resolve contexts in dependencies.
      try {
        $task['#context']->resolve($task['depends']);
      }
      catch (DrakeContextException $e) {
        drush_set_error('CONTEXT_MISSING', dt('In @task dependencies: @message', array('@task' => $task['name'], '@message' => $e->getMessage())));
        return FALSE;
      }

      foreach ($task['depends'] as $dependency_name) {
        // Skip tasks from --skip-tasks.
        if (in_array($dependency_name, $skip_tasks)) {
          continue;
        }
        // Circular detection.
        if (in_array($dependency_name, $stack)) {
          $chain = implode(' -> ', array_merge($stack, array($task['name'], $dependency_name)));
          drush_log(dt('Failing dependency chain: @chain.', array('@chain' => $chain)), 'notice');
          return drush_set_error('RECURSIVE_DEPENDENCY', dt('Recursive dependency'));
        }
        if (!isset($all_tasks[$dependency_name])) {
          return drush_set_error('UNKNOWN_TASK', dt('Unknown task @task', array('@task' => $dependency_name)));
        }
        // Let context follow dependencies.
        $dependency = $all_tasks[$dependency_name];
        $dependency['#parent'] = $task;
        $dependency['#context'] = new DrakeContext(array($dependency['#context'], $context));
        $res = _drake_visit($dependency, $resolution, $all_tasks, $seen, array_merge($stack, array($task['name'])));
        if (!$res) {
          return $res;
        }
      }
    }
    $resolution[] = $task;
  }
  return TRUE;
}

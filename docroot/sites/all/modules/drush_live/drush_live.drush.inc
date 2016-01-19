<?php

/**
 * @file A drush command that maintains a bootstrapped drupal indefinitely
 * listening for commands to do.
 */

// Various reasons the drush live backend has for exiting.
define('DRUSH_LIVE_EXIT_TIMEOUT', 0);
// These probably should be unique accross all of Drush and drush modules.
define('DRUSH_LIVE_EXIT_COMMAND', 121);
define('DRUSH_LIVE_EXIT_ERROR', 122);

/**
 * Implements hook_drush_command().
 */
function drush_live_drush_command() {
  $items = array();

  // A wrapper command which runs at a low bootstrap level.
  // This allows it to kill and recreate the bootstrapped version.
  $items['live'] = array(
    'description' => "Bootstrap drupal and wait for commands. Run 'exit' to exit and 'reload' to re-bootstrap Drupal. Prefix commands with '$' to send them directly to bash and '>' to run them in a new drush instance.",
    'options' => array(
      'reload-timeout' => dt("Drush live will force reload itself this often (in seconds). Defaults to half an hour."),
      'auto-reload' => dt("Causes Drush live to reload after every command. Set to a positive integer to reload every Nth command."),
      'ignore-command-reloads' => dt("Drush live will honour any other drush command's reload instruction, unless this option is passed."),
      'prompt-text' => dt('(Internal) A string to show in the live prompt.'),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_ROOT,
  );

  // The backend command that is controlled by the wrapper and fully bootstraps
  // Drupal.
  $items['live-backend'] = array(
    'description' => "Bootstrap drupal and wait for commands. Run 'exit' or 'reload' to exit.",
    'hidden' => TRUE,
    'arguments' => array(
      'timeout' => dt('How long to keep running for in seconds before exiting.'),
    ),
    'options' => array(
      'prompt-text' => dt('A string to show in the live prompt.'),
      'die-after' => dt("How many commands to run before exiting. Omit or pass 0 to indicate 'no limit'."),
      'ignore-command-reloads' => dt("Do not check the command to see if it mandates a reload."),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  return $items;
}

/**
 * The command
 */
function drush_drush_live_live() {

  // Number of seconds we maintain fully bootstrapped Drush/Drupal
  // before reloading it.
  $backend_timeout = drush_get_option('reload-timeout', 1800);
  $die_after = max(0, (int) drush_get_option('auto-reload', FALSE));
  $ignore_command_reloads = (boolean) drush_get_option('ignore-command-reloads', FALSE);

  echo _drush_live_welcome_message() . "\n\n";

  $command = 'live-backend';
  $args = array($backend_timeout);
  $options = array(
    'die-after' => $die_after,
    'ignore-command-reloads' => $ignore_command_reloads,
    'prompt-text' => drush_get_option('prompt-text', ''),
  );
  if (!drush_get_option('prompt-text', FALSE) && _drush_live_get_current_aliasname()) {
    $options['prompt-text'] = _drush_live_get_current_aliasname();
  }
  while (TRUE) {
    $exit_status = _drush_live_backend_invoke(_drush_live_get_current_aliasname(), $command, $args, $options, TRUE);
    switch ($exit_status) {
      // Do nothing if we just timed-out - we'll respawn a new one.
      case DRUSH_LIVE_EXIT_TIMEOUT:
        echo "...reloading...\n";
        break;
      // Completely exit if user supplied 'exit' command or error
      case DRUSH_LIVE_EXIT_COMMAND:
      case DRUSH_LIVE_EXIT_ERROR:
      default:
        break 2;
    }
  }

}

/**
 * Helper function to wrap drush_backend_invoke family of functions.
 * We choose the right one depending on the current situation.
 */
function _drush_live_backend_invoke_augment_prompt($alias_name, $command, $args, $options) {
  $options += array(
    'prompt-text' => _drush_live_prompt_text() . '>' . (!empty($alias_name) ? $alias_name : '-'),
  );
  return _drush_live_backend_invoke($alias_name, $command, $args, $options);
}


/**
 * Helper function to wrap drush_backend_invoke family of functions.
 * We choose the right one depending on the current situation.
 */
function _drush_live_backend_invoke($alias_name, $command, $args, $options) {
  $backend_options = array(
    'interactive' => TRUE,
  );
  drush_set_context('drush_live_alias', $alias_name);
  $exit_status = drush_invoke_process($alias_name, $command, $args, $options, $backend_options);
  return $exit_status;
}

/**
 * Waits in a loop for input and dispatches drush commands.
 */
function drush_drush_live_live_backend($timeout_seconds) {

  $timer_started = microtime(TRUE);
  $exit_condition = NULL;
  $command_limit = max(0, (int) drush_get_option('die-after', FALSE));
  $ignore_command_reloads = (boolean) drush_get_option('ignore-command-reloads', FALSE);

  # note for php4 or lower STDIN isn't a defined "constant".
  if (version_compare(phpversion(), '5.0.0', '<') && !defined('STDIN')) {
   define('STDIN', fopen('php://stdin', 'r'));
  }

  //stream_set_blocking(STDIN, 0);
  $error_code = $error_message = NULL;
  $timer_diff = 0;
  $command_count = 0;
  //stream_set_read_buffer(STDIN, 0);
  _drush_live_prompt();
  while (TRUE) {

    $input_line = '';

    // Test to see if there's anything waiting for us in stdin.
    // This only returns TRUE when there's a whole line to read.
    $read = array(STDIN);
    $null = NULL;
    $timeout = max(0, min(200000, $timeout_seconds - $timer_diff));
    $num_changed_sockets = stream_select($read, $null, $null, $timeout);

    // Read in what there is, if any, handle errors and make sure we put
    // aside whole lines for processing.
    if ($num_changed_sockets === FALSE) {
      $error_code = socket_last_error(STDIN);
      $error_message = socket_strerror($error_code);
      break;
    }
    elseif ($num_changed_sockets > 0) {
      $stdin_line = fread(STDIN, 2094);
      if ($stdin_line === FALSE) {
        $error_code = '0000';
        $error_message = 'Could not fread STDIN stream';
        break;
      }
      // It is possible for us to read a line without it ending at a newline
      // character (e.g. use of CTRL+D). In this case we print a newline
      // ourselves to make the prompt look OK.
      if (!preg_match('@\r?\n$@', $stdin_line)) {
        echo "\n";
      }
      // If the user *just* pressed CTRL+D we advise them how to actually exit.
      if ($stdin_line === '') {
        echo "To exit type 'exit'.\n";
      }
      $input_line = trim($stdin_line);
    }

    // Process a line of input
    if (!empty($input_line)) {
      // Accept the 'exit' keyword as instruction to exit.
      if (strtolower($input_line) == 'exit') {
        $exit_condition = DRUSH_LIVE_EXIT_COMMAND;
        break;
      }
      // Accept the 'restart' keyword as instruction to reload.
      if (strtolower($input_line) == 'reload') {
        $exit_condition = DRUSH_LIVE_EXIT_TIMEOUT;
        break;
      }
      // Print help message.
      if (strtolower($input_line) == '?') {
        echo _drush_live_help_message() . "\n";
      }
      else {
        _drush_live_handle_command($input_line, $drush_command);
        $command_count++;
      }
      // Allow commands to specify that things should always reload after them.
      if (isset($drush_command) && !$ignore_command_reloads && _drush_live_command_requests_reload($drush_command)) {
        $exit_condition = DRUSH_LIVE_EXIT_TIMEOUT;
        break;
      }
      // Restart if we've reached our command limit.
      if (!empty($command_limit) && ($command_count >= $command_limit)) {
        $exit_condition = DRUSH_LIVE_EXIT_TIMEOUT;
        break;
      }
    }
    _drush_live_prompt();

    // Sleep for quarter of a second.
    // usleep(250000);

    // Abort if we're over our timeout
    $timer_diff = microtime(TRUE) - $timer_started;
    if ($timer_diff > $timeout_seconds) {
      $exit_condition = DRUSH_LIVE_EXIT_TIMEOUT;
      break;
    }
  }

  // If there was an error exit.
  if (isset($error_code)) {
    drush_log(dt("Error reading from stdin (!error_code): !error_message", array('!error_code' => $error_code, '!error_message' => $error_message)), 'error');
    $exit_condition = DRUSH_LIVE_EXIT_ERROR;
  }

  // Attempt to get drush to return some sort of error code.
  if (!empty($exit_condition)) {
    drush_set_context('DRUSH_ERROR_CODE', $exit_condition);
  }
}

/**
 * Echo out the prompt.
 */
function _drush_live_prompt() {
  $aliasname = _drush_live_get_current_aliasname();
  $prompttext = _drush_live_prompt_text();
  echo "DrushLive{$aliasname}{$prompttext}> ";
}

/**
 * Helper function which retrieves which prompt text we have.
 */
function _drush_live_prompt_text() {
  static $prompt = '';
  if ($new_prompt = drush_get_option('prompt-text', '')) {
    $prompt = $new_prompt;
  }
  return $prompt;
}

function _drush_live_get_current_aliasname($with_ampersand_prefix = FALSE) {
  $aliasname = NULL;
  $alias_context = drush_get_context('alias');
  if (!empty($alias_context) && !empty($alias_context['#name'])) {
    $aliasname = $alias_context['#name'];
  }
  if (!empty($aliasname) && $aliasname[0] !== '@') {
    $aliasname = '@' . $aliasname;
  }
  return $aliasname;
}

/**
 * Helper function which executes a drush command in as similar a way to
 * drush as possible.
 *
 * @param $command
 *  The full string to interpret as a command.
 * @param &$command_array
 *  The drush command loaded for it. Will be NULL if not found.
 *
 * Copied from drush_main().
 */
function _drush_live_handle_command($command, &$drush_command = NULL) {

  $drush_command = NULL;

  // Strip out and remember any modifier flag prefixes we support.
  $mods = array();
  $mod_symbols = array(
    'backend_invoke' => preg_quote('>', '/'),
    'shell_command' => preg_quote('$', '/'),
  );
  foreach ($mod_symbols as $name => $char) {
    $pattern = '/^([ ' . implode('', array_intersect_key($mod_symbols, array($name => 1))) . ']*)' . $mod_symbols[$name] . '/';
    $command = preg_replace($pattern, '$1', $command, 1, $mods[$name]);
    $mods[$name] = (boolean) $mods[$name];
  }
  if ($mods['backend_invoke'] && $mods['shell_command']) {
    drush_log(dt("Drush live only supports the use of one modifier ('$' or '>') at a time."), 'error');
    return FALSE;
  }

  if ($mods['shell_command']) {
    echo "[Direct shell command..]\n";
    return drush_op_system($command);
  }

  // Overwrite global context for our new command.
  $command_array = _drush_live_parse_arg_string($command);

  // Strip 'drush' from the front of commands.
  if (strtolower(trim(reset($command_array))) == 'drush') {
    drush_log(dt("It is unnecessary to prefix commands with 'drush' when at the Drush live prompt."), 'warning');
    $command_array = array_slice($command_array, 1);
  }

  // Strip @aliasname from the front of commands.
  $alias_name = NULL;
  $first_arg = strtolower(trim(reset($command_array)));
  if ($first_arg[0] == '@') {
    $alias_name = $first_arg;
    $command_array = array_slice($command_array, 1);
  }

  // Allow the notation '>@aliasname' as shorthand for '>@aliasname live'
  if (!empty($alias_name) && empty($command_array) && $mods['backend_invoke']) {
    $command_array = array('live');
  }

  // Remember all Drush context so we can restore it later.
  $old_context = drush_get_context();

  $new_argv = array_merge(array(reset($GLOBALS['argv'])), $command_array);
  drush_set_context('argv', $new_argv);
  drush_set_context('argc', count($new_argv));

  // Process and run the command
  drush_parse_args();

  // If there was a '>' prefix then we backend invoke this.
  // This makes it possible to call commands to other @aliases without having
  // to leave Drush live.
  if ($mods['backend_invoke']) {
    if (empty($alias_name)) {
      echo "[Bootstrapping drush..]\n";
    }
    else {
      echo "[Bootstrapping drush at {$alias_name}..]\n";
    }
    _drush_live_backend_invoke_augment_prompt($alias_name, reset($command_array), array_slice(drush_get_arguments(), 1), drush_redispatch_get_options(), TRUE);
  }

  else {

    // Error if an @aliasname is at the front of commands. We do not support them.
    if (!empty($alias_name)) {
      drush_log(dt("Drush aliases are not valid at the Drush live prompt. To open Drush live for that site run:\n\n  drush !aliasname live", array('!aliasname' => $alias_name)), 'error');
      return FALSE;
    }

    // Make Drush dispatch the command in as much the normal way as possible.
    $return = _drush_live_dispatch_command();

  }

  // Restore old context
  drush_set_context(NULL, $old_context);

  return $return;
}

/**
 * Copy-n-pasted code from Drush to make it execute commands in as normal
 * a way as possible.
 *
 * All global context should have been set by this point to make it think that
 * it's the active command.
 */
function _drush_live_dispatch_command() {

  $command = drush_parse_command();
  $return = '';
  $command_found = FALSE;

  // Command could be handled early.
  $command_handled = drush_preflight_command_dispatch();
  if ($command_handled) {
    return $return;
  }

  // Otherwise despatch it in the normal way.
  if (is_array($command)) {
    //$bootstrap_result = drush_bootstrap_to_phase($command['bootstrap']);
    $bootstrap_result = TRUE;
    //drush_enforce_requirement_bootstrap_phase($command);
    drush_enforce_requirement_core($command);
    drush_enforce_requirement_drupal_dependencies($command);
    drush_enforce_requirement_drush_dependencies($command);

    if ($bootstrap_result && empty($command['bootstrap_errors'])) {
      drush_log(dt("Found command: !command (commandfile=!commandfile)", array('!command' => $command['command'], '!commandfile' => $command['commandfile'])), 'bootstrap');

      $command_found = TRUE;
      // Dispatch the command(s).
      $return = drush_dispatch($command);

      // prevent a '1' at the end of the output
      if ($return === TRUE) {
        $return = '';
      }

      if (drush_get_context('DRUSH_DEBUG') && !drush_get_context('DRUSH_QUIET')) {
        drush_print_timers();
      }
      drush_log(dt('Peak memory usage was !peak', array('!peak' => drush_format_size(memory_get_peak_usage()))), 'memory');
    }
  }

  if (!$command_found) {
    // If we reach this point, command doesn't fit requirements or we have not
    // found either a valid or matching command.

    // If no command was found check if it belongs to a disabled module.
    if (!$command) {
      $command = drush_command_belongs_to_disabled_module();
    }

    // Set errors related to this command.
    $args = implode(' ', drush_get_arguments());
    if (isset($command) && is_array($command)) {
      foreach ($command['bootstrap_errors'] as $key => $error) {
        drush_set_error($key, $error);
      }
      drush_set_error('DRUSH_COMMAND_NOT_EXECUTABLE', dt("The drush command '!args' could not be executed.", array('!args' => $args)));
    }
    elseif (!empty($args)) {
      drush_set_error('DRUSH_COMMAND_NOT_FOUND', dt("The drush command '!args' could not be found.  Run `drush cache-clear drush` to clear the commandfile cache if you have installed new extensions.", array('!args' => $args)));
    }
    // Set errors that occurred in the bootstrap phases.
    $errors = drush_get_context('DRUSH_BOOTSTRAP_ERRORS', array());
    foreach ($errors as $code => $message) {
      drush_set_error($code, $message);
    }
  }

  return $return;
}

/**
 * Helper function which examines a fully loaded drush command and determines
 * whether it mandates that drush live should reload after running it.
 *
 * Commands can require this by adding the following entry in their definition
 * (in hook_drush_command()):
 *
 *   'drush_live_reload' => TRUE
 */
function _drush_live_command_requests_reload($drush_command) {
  return (!empty($drush_command['drush_live_reload']));
}

/**
 * Helper function which takes a string and tries to split it up into arguments
 * the way a bash shell would.
 * Sigh - a bit annoying we need to do this.. but we never go through bash so..
 */
function _drush_live_parse_arg_string($str_args) {
  $chr_escape = array('\\' => '\\');
  $chr_quote = array("'" => "'", '"' => '"');
  $chr_space = array(' ' => ' ');

  $chr_special = $chr_escape + $chr_quote + $chr_space;

  $arr_args = array();
  $str_args = str_split($str_args);

  $is_escaped = FALSE;
  $open_quote = FALSE;
  $_arg = '';
  while ($next = array_shift($str_args)) {

    // ESCAPE CHAR
    if (in_array($next, $chr_escape)) {
      if ($is_escaped) {
        $_arg .= $next;
        $is_escaped = FALSE;
      }
      else {
        $is_escaped = TRUE;
      }
    }

    //STRING QUOTE CHAR
    elseif (in_array($next, $chr_quote)) {
      if (!$is_escaped) {
        if ($open_quote === FALSE) {
          $open_quote = $next;
        }
        elseif ($next === $open_quote) {
          $open_quote = FALSE;
        }
        else {
          $_arg .= $next;
        }
      }
      else {
        $_arg .= $next;
      }
      $is_escaped = FALSE;
    }

    // SPACE CHAR
    elseif (in_array($next, $chr_space)) {
      if (!$open_quote && !$is_escaped) {
        // Start new arg
        if (strlen(trim($_arg))) {
          $arr_args[] = $_arg;
          $_arg = '';
        }
      }
      else {
        $_arg .= $next;
      }
      $is_escaped = FALSE;
    }

    // NORMAL CHAR
    else {
      $_arg .= $next;
      $is_escaped = FALSE;
    }

  }

  if (strlen(trim($_arg))) {
    $arr_args[] = $_arg;
  }

  if (!empty($arr_args)) {
    $arr_args = array_filter(array_map('trim', $arr_args));
  }

  return $arr_args;
}

function _drush_live_welcome_message() {
  return <<<EOT
Entering the Drush live CLI. Commands are: 'exit' and 'reload'.
Type '?' for help.
EOT;
}

function _drush_live_help_message() {
  return <<<EOT

  DRUSH LIVE CLI - USAGE

  The Drush live CLI allows you to run Drush commands on an already fully
  bootstrapped Drush/Drupal site. Any drush command which works normally should
  work in here with the following additions:

    'exit'      exits the Drush live CLI
    'reload'    re-bootstraps Drush/Drupal in the current CLI
    '?'         prints this help message

  Additionally any drush command may be prefixed with one modifier character:

    '$'         (Direct shell command) executes the line in a system shell
                directly (rather than interpretting it as a drush command)
    '>'         (Backend invoke) runs the line as a drush command but in a
                seperately spawned process.

  Note, in general @aliasnames are meaningless at the start of commands since
  Drush live maintains the fully bootstrapped site as indicated when initially
  called. However, through use of the 'Backend invoke' syntax, it is possible to
  run commands at seperate sites. For example:

    # Start Drush live CLI at @example.co.uk site
    drush @example.co.uk live
    ...
    # Run command at @mywebsite.com alias despite being in another CLI
    DrushLive@example.co.uk> >@mywebsite.com status
    ...

  It is even possible to temporarily move over to a different @alias by invoking
  a nested Drush live CLI instance using a simplified 'Backend invoke' syntax:

    # Start Drush live CLI at @example.co.uk site
    drush @example.co.uk live
    ...
    # Move over to @mywebsite.com and run a few commands
    DrushLive@example.co.uk> >@mywebsite.com
    DrushLive@example.co.uk>@mywebsite.com> status
    DrushLive@example.co.uk>@mywebsite.com> rl

    # Move back to where I was
    DrushLive@example.co.uk>@mywebsite.com> exit
    DrushLive@example.co.uk>

EOT;
}

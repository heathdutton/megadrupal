<?php

/**
 * @file
 * This is an include file with Drush Rush helper functions.
 * @see drushrush.drush.inc
 * @backlog Refactor to use lazy includes and auto loaders.
 */

require dirname(__FILE__) . '/includes/includes.inc';
require dirname(__FILE__) . '/operations/operations.inc';

/**
 * @addtogroup helpers Helper Functions
 * @brief Helper functions for Drush Rush commands.
 * @details These helper functions include functions to:
 * - provide wrappers to Drush functions and commands
 * - provide service functions for Rush commands
 *
 * Though not strictly internal or private, these functions are mainly used by
 * other Rush functions, and are not structured like operations functions which
 * may be called directly by other code with parameters passed at call time.
 * @{
 */


/**
 * @addtogroup command_helpers Command Helpers
 * @brief Functions to help with Drush Rush Commands.
 * @{
 */


/**
 * A wrapper for drush_invoke_process().
 *
 * See: http://api.drush.org/api/
 *
 * @param array $alias
 *   The site record to execute the command on.
 *   Use '@self' to run on the current site.
 * @param string $command
 *   The command to invoke.
 * @param array $args
 *   The arguments to pass to the command.
 * @param array $options
 *   The options (e.g. --select) to provide to the command.
 * @param string $function
 *   The name of the function which called, rush_invoke().
 * @param string $type
 *   The type of Drush log message to use.
 *
 * @return bool
 *   Returns TRUE on success or FALSE on fail (via drush_set_error).
 */
function rush_invoke($alias, $command, $args, $options, $function, $type = 'message') {

  $cmd_string = $command . ' ' . implode(' ', $args) . ' ' . implode(' ', $options);
  if (drush_invoke_process($alias, $command, $args, $options, TRUE)) {

    $msg = dt('[RUN DRUSH]: !cmd_string', array('!cmd_string' => $cmd_string));
    drush_log($msg, $type);

    return TRUE;
  }
  else {

    $msg = dt('[ERROR]: *** !operation failed to run: !cmd_string ***', array(
      '!cmd_string' => $cmd_string,
      '!operation' => $function,
    ));

    return drush_set_error('RUSH_INVOKE_ERROR', $msg);

  }
}

/**
 * A wrapper for drush_shell_exec() .
 *
 * See: http://api.drush.org/api/
 *
 * @param string $cmd
 *   Required. The command to execute. May include placeholders.
 * @param string $msg
 *   Required.  String containing the message to log upon success.
 * @param string $error
 *   Required.  The type of error to log on fail.
 * @param string $function
 *   The name of the function which called, rush_shell_exec().
 * @param string $type
 *   The type of Drush log message to use.
 *
 * @return bool
 *   TRUE on success, FALSE on failure via drush_set_error().
 */
function rush_shell_exec($cmd, $msg, $error, $function, $type = 'message') {

  if (drush_shell_exec($cmd)) {
    drush_log($msg, $type);

    return TRUE;
  }
  else {
    $error_msg = dt('[ERROR]: *** Failed to run: !cmd in !function ***', array(
      '!cmd' => $cmd,
      '!function' => $function,
    ));

    return drush_set_error($error, $error_msg);
  }

}

/**
 * Helper function to return the path to the Drush Rush module directory.
 *
 * @return string
 *   A string containing the path to the Rush module directory.
 */
function rush_self_path() {

  return realpath(dirname(__FILE__)) . '/';
}

/**
 * Helper function to print the params associated with the defined show mode.
 *
 * @param array $params
 *   Required. An associative array of all rush parameters.
 * @param string $mode
 *   Required. A string containing the name of the mode to show.
 * @param array $operations
 *   An associative array of all rush operations.
 *
 * @return bool
 *   TRUE on success, FALSE on fail via drush_set_error.
 */
function _rush_show($params, $mode, $operations) {

  switch ($mode) {
    case 'params':
      unset($params['derived_params']);
      unset($params['job']);
      ksort($params);
      $msg = rush_message_wrapper(dt(" PARAMS "), 0);
      drush_print_r($msg);
      drush_print_r($params);
      $msg = rush_message_wrapper(dt(" END PARAMS "), 0);
      drush_print_r($msg);

      return TRUE;

    case 'ops':
      $msg = rush_message_wrapper(dt(" OPERATIONS "), 0);
      drush_print_r($msg);
      drush_print_r($operations);
      $msg = rush_message_wrapper(dt(" END OPERATIONS "), 0);
      drush_print_r($msg);

      return TRUE;

    case 'help':
    case 'h':
      $msg = rush_message_wrapper(dt(" HELP "), 0);
      drush_print_r($msg);
      drush_print_r($params['job']['description']);
      // Create a line space.
      drush_print('');
      drush_print_r($params['job']['help']);
      $msg = rush_message_wrapper(dt(" END HELP "), 0);
      drush_print_r($msg);

      return TRUE;

    case 'derived':
      drush_print_r($params['derived_params']);

      return TRUE;

    case 'paths':
      $msg = rush_message_wrapper(dt(" PATHS "), 0);
      drush_print_r($msg);
      $paths['build_paths']['root_directory_path'] = $params['build']['root_directory_path'];
      $paths['build_paths']['directory_path'] = $params['build']['directory_path'];
      $paths['build_paths']['docroot_path'] = $params['build']['docroot_path'];
      $paths['db_paths']['directory_path'] = $params['db']['directory_path'];
      $paths['global_paths'] = $params['global_locations'];
      drush_print_r($paths);
      $msg = rush_message_wrapper(dt(" END PATHS "), 0);
      drush_print_r($msg);

      return TRUE;

    default:
      if (!empty($params[$mode])) {
        ksort($params[$mode]);
        drush_print_r($params[$mode]);

        return TRUE;
      }

      $msg = dt('[ERROR]: *** The --show option "!option" is not a valid value. Check the spelling of the value. ***', array('!option' => $mode));

      return drush_set_error('DRUSH_RUSH_INVALID_OPTION', $msg);
  }
}

/**
 * Help function to select which docs to display information about.
 *
 * @return mixed
 *   FALSE on error.  Array on drush_invoke_process success.
 */
function _rush_docs_selection() {

  $topics = array(
    'rush-ops-tables',
    'docs-rush-ops-db',
    'docs-rush-ops-repo',
    'docs-rush-ops-site',
    'docs-rush-ops-utility',
    'docs-rush-ops-filesystem',
    'docs-rush-ops-host',
  );
  $topics = drush_map_assoc($topics);
  // Present a list of topics as options to the user.
  $topic = drush_choice($topics, 'Enter a number to choose which topic to read.', '!key');
  if ($topic !== FALSE) {
    $args = array($topics[$topic]);
    return drush_invoke_process('@self', 'topic', $args);
  }

  return FALSE;

}


/** @}*/
/** @}*/

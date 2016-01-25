<?php

/**
 * @file
 * Common functions used throughout Drush Vagrant
 */

// Include all the sub-commands
$includes = scandir(dirname(__FILE__));
foreach ($includes as $key => $file) {
  if (@strpos($file, '.vagrant.inc', strlen($file) - 12) !== FALSE) {
    include_once($file);
  }
}

/**
 * Run a command, log the result and return the output
 */
function drush_vagrant_exec_and_log(array $command) {
  // Default to output of drush_exec
  if (!isset($command['output'])) {
    if ($output = drush_shell_exec_output()) {
      $command['output'] = drush_shell_exec_output();
    }
    else {
      $command['output'] = $command['command'];
    }
  }
  // Failure
  if (!$command['command']) {
    drush_log($command['failure'], "error");
    if (!empty($command['output'])) {
      foreach ($command['output'] as $line) {
        drush_print($line);
      }
    }
    exit(1);
  }
  // Success
  else {
    drush_log($command['success'], "ok");
  }
  // Return our output, in case it's needed
  return $command['output'];
}
/**
 * Executes a shell command at a new working directory in interactive mode.
 * The old cwd is restored on exit.
 *
 * TODO: Follow http://drupal.org/node/1793694 to see if this gets added to Drush core.
 *
 * @param $effective_wd
 *   The new working directory to execute the shell command at.
 * @param $cmd
 *   The command to execute. May include placeholders used for sprintf.
 * @param ...
 *   Values for the placeholders specified in $cmd. Each of these will be passed
 *   through escapeshellarg() to ensure they are safe to use on the command line.
 * @return
 *   TRUE on success, FALSE on failure
 */
function drush_vagrant_shell_cd_and_exec_interactive($effective_wd, $cmd) {
  $args = func_get_args();

  $effective_wd = array_shift($args);
  $cwd = getcwd();
  drush_op('chdir', $effective_wd);
  $result = call_user_func_array('drush_shell_exec_interactive', $args);
  drush_op('chdir', $cwd);
  return $result;
}

/**
 * Generate help text
 */
function _drush_vagrant_help_text() {
  $args = drush_get_arguments();
  if (isset($args[1])) {
    // Drop the 0th argument, which should always be 'vagrant', but allow for
    // sub-sub-commands
    array_shift($args);
    $current_subcommand = $args;
  }
  // Build an array of our drush-vagrant commands
  $dv_commands = vagrant_drush_command();
  // Generate a list of drush-vagrant sub-commands
  $all_subcommands = str_replace('vagrant-', '', array_keys($dv_commands));

  if (!isset($current_subcommand)) {
    // No sub-command was given, so we'll build our lists of subcommands
    return _drush_vagrant_full_help_text();
  }
  elseif (in_array($current_subcommand[0], $all_subcommands)) {
    // We've matched one of our sub-commands, so we'll pass along its help-text
    drush_shell_exec('drush vagrant-' . $current_subcommand[0] . ' --help');
    return implode("\n", drush_shell_exec_output());
  }
  else {
    // We've either passed a native Vagrant command or a non-existant command
    // so we'll provide explanatory text, and let Vagrant handle the rest
    $vg_subcommand = implode(' ', $current_subcommand);
    $output[] = dt('Drush Vagrant Integration will pass through native Vagrant commands. However, it allows the use of Drush aliases to run commands on projects and VMs remotely. Here is the help output from Vagrant itself on the "!command" sub-command:\n', array('!command' => $vg_subcommand));
    $vg_command = strtr("vagrant !sub-commands --help", array('!sub-commands' => $vg_subcommand));
    drush_shell_exec($vg_command);
    $output[] = implode("\n", drush_shell_exec_output());
    return implode("\n", $output);
  }
}

/**
 * Generate full help text
 */
function _drush_vagrant_full_help_text() {
  $args = drush_get_arguments();
  $short_help = dt('Run a command on your Vagrant projects or VMs.');
  if ($args[0] == 'help') {
    // We're in a command list
    return $short_help;
  }
  else {
    $cache = drush_cache_get('help_text', 'vagrant');
    // Check for cached help text
    if (!isset($cache->data)) {
      // Generate help text
      $variables = array();
      $variables["short_help"] = $short_help . "\n";
      // Generate a list of native Vagrant commands.
      drush_shell_exec('vagrant');
      $output = drush_shell_exec_output();
      if ($output) {
        // Drop first 6 lines and last 2 as those are not needed.
        $output = array_splice($output, 6, -2);
        $variables["vagrant_commands"] = _drush_vagrant_help_table($output);
      }

      // Generate a list of commands from Drush extensions that implement a
      // blueprint.
      $blueprints = drush_vagrant_blueprint_info();
      foreach ($blueprints as $blueprint => $info) {
        $extensions[] = $info['extension'];
      }
      $drush_commands = drush_get_context('DRUSH_COMMANDS');
      $sub_commands = array();
      foreach ($drush_commands as $command => $info) {
        if (in_array($info['commandfile'], $extensions)
            && !isset($info['topics'][0])
            && (isset($info['hidden']) ? $info['hidden'] != 1 : TRUE)
            && !isset($info['is_alias'])
            && $command != 'vagrant') {
          $vg_prefix = 'vagrant-';
          if (substr($command, 0, strlen($vg_prefix)) == $vg_prefix) {
            $command = substr($command, strlen($vg_prefix));
          }
          $sub_commands[] = '     ' . $command;
        }
      }
      $variables["sub_commands"] = _drush_vagrant_help_table($sub_commands, 2);

      $help_text = _drush_vagrant_render_template($variables, $template = "help.tpl.php");

      // Save our generated help text to the cache
      drush_cache_set('help_text', $help_text, 'vagrant', DRUSH_CACHE_TEMPORARY);
    }

    $help_text = isset($cache->data) ? $cache->data : $help_text;

    return $help_text;
  }
}

function _drush_vagrant_help_table($list, $cols = 3) {
  // If we aren't passed anything, just return.
  if (!count($list)) return '';
  // Sort alphabetically
  sort($list, SORT_STRING);
  // Calculate the number of rows
  $rows = ceil(count($list)/$cols);
  // Re-arrange the list for presentation in column that run alphabetically
  // down, rather than across.
  $i = 0;
  for ($col = 0; $col < $cols; $col++) {
    for ($row = 0; $row < $rows; $row++) {
      $table[$row][$col] = isset($list[$i]) ? $list[$i] : '';
      $i++;
    }
  }
  // Since drush_print_table() actually calls drush_print(), grab the output,
  // rather than sending it to the terminal
  ob_start();
  drush_print_table($table);
  return ob_get_clean();
}

/**
 * Ensure that there is a user context
 */
function _drush_vagrant_check_user_context() {
  $user_settings = drush_get_context('user');
  if (!isset($user_settings['vagrant']['username'])) {
    drush_print(dt('This appears to be your first time running Drush Vagrant or your user settings (~/.drushrc.php) have been deleted.'));
    drush_print(dt('Drush Vagrant Integration requires those settings in order to function properly.'));
    if (drush_confirm(dt('Proceed?'))) {
      drush_shell_exec_interactive('drush vagrant-user');
      // PROJECTS_PATH won't have been defined in hook_drush_init() yet, so we
      // have to do it here for the first run of vagrant-build
      $home = getenv('HOME');

      // Re-include the updated .drushrc.php file
      include($home . '/.drushrc.php');
      define("PROJECTS_PATH", $options['vagrant']['project_root']);
      drush_set_context('user', $options);
    }
    else {
      drush_user_abort();
    }
  }
}

/**
 * Ensure that a given project exists
 */
function drush_vagrant_check_project($project_path, $set_error = FALSE) {
  if (!is_dir($project_path)) {
    if ($set_error) {
      drush_set_error(dt('ERROR: Could not find a project at "!project".', array('!project' => $project_path)));
    }
    return FALSE;
  }
  if (!file_exists($project_path . '/Vagrantfile')) {
    if ($set_error) {
      drush_set_error(dt('ERROR: Could not find a Vagrantfile in "!project".', array('!project' => $project_path)));
    }
    return FALSE;
  }
  return $project_path;
}

/**
 * Validate and return the current project's path.
 */
function drush_vagrant_get_project_path() {
  if ($project_path = drush_get_option('project-path', getcwd())) {
    return _drush_vagrant_get_project_path($project_path);
  }
}

/**
 * Return the validated root of a project path
 */
function _drush_vagrant_get_project_path($project_path = PROJECT_PATH) {
  // Turn our paths into arrays for easier comparison
  $root = explode(DIRECTORY_SEPARATOR, PROJECTS_PATH);
  $project = explode(DIRECTORY_SEPARATOR, $project_path);
  // Check if our $project_path is under PROJECTS_PATH
  if ($root === array_slice($project, 0, count($root), TRUE)) {
    // We're somewhere under PROJECTS_PATH, so trim the project_path to one
    // level deeper than that, so we can check if we're in a project or not
    array_splice($project, count($root) + 1);
    $new_project_path = implode(DIRECTORY_SEPARATOR, $project);
    if (drush_vagrant_check_project($new_project_path)) {
      // Yep, it's a project
      return $new_project_path;
    }
    else {
      // Nope, it's not a project, so return our original path
      return $project_path;
    }
  }
  else {
    // We aren't under PROJECTS_PATH, so return our original path
    return $project_path;
  }
}

/**
 * Return the name of a project based on its path
 *
 * @project_path
 *   The absolute path to a project directory.
 */
function drush_vagrant_get_project_name($project_path) {
  $project_path_root = _drush_vagrant_get_project_path($project_path);
  return basename($project_path_root);
}


/**
 * Return the subnet of a given project
 */
function _drush_vagrant_get_subnet($project_path) {
  $subnet = '';
  $match = '';
  $config_file = $project_path . '/.config/config.rb';

  // Read each line of the config file to find the line that contains the subnet
  if (is_file($config_file) && $file = @fopen($config_file, "r")) {
    while (($line = fgets($file, 4096)) !== FALSE) {
      if (strpos($line , 'Subnet')) {
        $match = $line;
        break;
      }
    }
    if ($match == '' && !feof($file)) {
      drush_set_error(dt('ERROR: unexpected fgets() fail in _drush_vagrant_get_subnet() reading !config_file.', array('!config_file' => $config_file)));
    }
    fclose($file);
  }
  else {
    drush_set_error(dt('ERROR: Could not open !config_file in _drush_vagrant_get_subnet().', array('!config_file' => $config_file)));
  }

  // The Subnet's value is double-quoted
  $i = explode('"' , $match);
  if (is_numeric($i[1])) {
    return $i[1];
  }
  else {
    drush_set_error(dt('ERROR: Could not parse !config_file in _drush_vagrant_get_subnet().', array('!config_file' => $config_file)));
  }
}

/**
 * Clear caches internal to drush-vagrant.
 */
function vagrant_cache_clear_all() {
  drush_cache_clear_all(NULL, 'vagrant');
}

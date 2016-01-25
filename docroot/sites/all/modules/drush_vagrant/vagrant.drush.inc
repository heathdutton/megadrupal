<?php

/**
 * @file
 *   Vagrant family of Drush commands for managing Vagrant-based VMs.
 */

define("DRUSH_VAGRANT_ROOT", drush_normalize_path(dirname(__FILE__)));

// Include all command and helper functions
include_once('includes/vagrant.inc');

// Include hooks for our default blueprint
include_once('vagrant.blueprints.inc');

/**
 * Implements hook_drush_help().
 */
function vagrant_drush_help($section) {
  switch ($section) {
    case 'meta:vagrant:title':
      return dt('Vagrant integration');
    case 'meta:vagrant:summary':
      return dt('Manage Vagrant-based networks of VMs.');
  }
}

/**
 * Implements hook_drush_init().
 */
function vagrant_drush_init() {
  $command = drush_get_command();
  if (isset($command['requires_vagrant'])) {
    // Load our cached data
    $cache_ids = array('vagrant_path', 'project_root');
    $cache = drush_cache_get_multiple($cache_ids, 'vagrant');

    // Check if Vagrant is installed
    if (!isset($cache['vagrant_path'])) {
      $vagrant_path = drush_shell_exec_output(drush_shell_exec('which vagrant'));
      drush_cache_set('vagrant_path', $vagrant_path[0], 'vagrant', DRUSH_CACHE_TEMPORARY);
    }
    if (!isset($cache['vagrant_path']->data) && !isset($vagrant_path)) {
      drush_log(dt('*** You have installed Drush Vagrant Integration, but Vagrant does not appear to be installed.
*** Please install Vagrant (http://vagrantup.com/) before continuing.
'), 'warning');
    }

    // Set our defines, which provide context for commands
    if (!isset($cache['project_root'])) {
      $user_settings = drush_get_context('user');
      if (isset($user_settings['vagrant']['project_root'])) {
        $project_root = $user_settings['vagrant']['project_root'];
        drush_cache_set('project_root', $project_root, 'vagrant', DRUSH_CACHE_TEMPORARY);
      }
    }
    if (isset($cache['project_root']->data) || isset($project_root)) {
      $project_root = isset($cache['project_root']->data) ? $cache['project_root']->data : $project_root;
      define("PROJECTS_PATH", $project_root);
    }
  }

  if (isset($command['requires_vagrant_project'])) {

    define("VM_NAME", drush_get_option('vm-name', NULL));
    define("PROJECT_PATH", drush_vagrant_get_project_path());

    // Load an alias if none was provided, but we're in a project for which
    // one exists.
    if (!drush_get_context('alias', FALSE) && drush_vagrant_check_project(PROJECT_PATH, TRUE)) {
      $project_name = drush_vagrant_get_project_name(PROJECT_PATH);
      if (file_exists(drush_server_home() . '/.drush/' . $project_name . '.aliases.drushrc.php')) {
        $alias = drush_sitealias_get_record('@' . $project_name);
        drush_set_context('alias', $alias);
      }
    }
  }

  // Set our help text
  drush_set_context('vagrant_help_text', _drush_vagrant_help_text());

}

/**
 * Implements hook_drush_command().
 *
 * Special attributes:
 *   'requires_vagrant' => If this command requires the vagrant binaries.
 *   'requires_vagrant_project' => If this command requires a valid vagrant project directory.
 */
function vagrant_drush_command() {
  $items = array();

  $items['vagrant'] = array(
    'description' => drush_get_context('vagrant_help_text', dt('Run a command on your Vagrant projects or VMs.')),
    'options' => array(
      'project-path' => array(
        'description' => dt('The relative or absolute path to the project on which to run the command.'),
      ),
      'vm-name' => array(
        'description' => dt('The name of the Vagrant virtual machine on which to run the command.'),
      ),
    ),
    'strict-option-handling' => TRUE,
    'allow-additional-options' => TRUE,
    'aliases' => array('vg'),
    'requires_vagrant' => TRUE,
    'requires_vagrant_project' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['vagrant-alias-config'] = array(
    'description' => dt('Convert the output of `vagrant ssh-config` into a Drush remote alias.'),
    'requires_vagrant' => TRUE,
    'requires_vagrant_project' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['vagrant-alias'] = array(
    'description' => dt('Generate an alias for a site running in a VM.'),
    'options' => array(
      'alias' => dt('Alias.'),
      'uri' => dt('URI.'),
      'docroot' => dt('Docroot location.'),
      'vm-name' => dt('Name of the VM.'),
    ),
    'strict-option-handling' => TRUE,
    'allow-additional-options' => TRUE,
    'aliases' => array('val'),
    'requires_vagrant' => TRUE,
    'requires_vagrant_project' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['vagrant-aliases'] = array(
    'description' => dt('Generate aliases for a project and its VMs.'),
    'options' => array(
      'delete' => dt('Delete aliases for a project.'),
      ),
    'strict-option-handling' => TRUE,
    'allow-additional-options' => TRUE,
    'aliases' => array('vals'),
    'requires_vagrant' => TRUE,
    'requires_vagrant_project' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['vagrant-list'] = array(
    'description' => dt('List current Vagrant projects, VMs and statuses.'),
    'options' => array(
      'all' => dt('When in a project directory, list all projects instead of just the current one.'),
      'projects' => dt('Only list project names.'),
      'vms' => dt('Only list VM names.'),
      ),
    'strict-option-handling' => TRUE,
    'aliases' => array('vls'),
    'requires_vagrant' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['vagrant-blueprints'] = array(
    'description' => dt('List all blueprints.'),
    'strict-option-handling' => TRUE,
    'aliases' => array('vbl'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['vagrant-build'] = array(
    'description' => dt('Build a new Drush Vagrant project.'),
    'options' => array(
      'project-name' => array(
        'description' => dt('The short name of the project.')),
      'blueprint' => array(
        'description' => dt('Specify the blueprint to use.')),
      'git-repo' => array(
        'description' => dt('Clone a Git repo, instead of using a blueprint.')),
      'git-branch' => array(
        'description' => dt('The branch of the git-repo, defaults to \'master\'.')),
      'aliases' => array(
        'description' => dt('Generate aliases for the project and its VMs.')),
      'hosts' => array(
        'description' => dt('Add entries to /etc/hosts.')),
      'up' => array(
        'description' => dt('Start up the Vagrant project VM(s) immediately.')),
    ),
    'examples' => array(
      'drush vb --project-name=project1 --blueprint=aegir' => dt('Initialize a new Vagrant project using the "aegir" blueprint.'),
    ),
    'drush dependencies' => array('hosts'),
    'strict-option-handling' => TRUE,
    'aliases' => array('vb'),
    'requires_vagrant' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['vagrant-delete'] = array(
    'description' => dt('Delete a Vagrant project.'),
    'arguments' => array(
      'project' => dt('The project to delete.'),
    ),
    'examples' => array(
      'drush vagrant-delete project1' => dt('Delete the "project1" project.'),
    ),
    'drush dependencies' => array('hosts'),
    'strict-option-handling' => TRUE,
    'aliases' => array('vdl'),
    'requires_vagrant' => TRUE,
    'requires_vagrant_project' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['vagrant-shell'] = array(
    'description' => dt('Log into a Drush Vagrant VM via SSH.'),
    'arguments' => array(
      'remote-command' => dt('(OPTIONAL) Instead of logging into a VM, run this command in the VM. The command should be quoted if there are parameters. Returns the output of the command.'),
    ),
    'options' => array(
      'ssh-user' => 'The username to login under. Defaults to the username in the user-specific config file (~/drushrc.php).',
      'ssh-id' => 'The identity file from which the private key is read. Defaults to the private key in the user-specific config file (~/drushrc.php).',
      'ssh-connection-string' => 'Instead of running SSH, return the a string of the command that would otherwise be run.',
      'default-user' => 'Use the default "vagrant" user, and ignore other username or identity files, whether specified in the user-specific config file, or on the command line.',
      'vm-name' => 'The name of the VM to SSH into.'
    ),
    'examples' => array(
      'drush vagrant shell' => dt('Log into the VM in a single VM project.'),
      'drush @example vsh "hostname -f"' => dt('Return the FQDN of the VM/project specified in the @example alias.'),
    ),
    'strict-option-handling' => TRUE,
    'allow-additional-options' => TRUE,
    'aliases' => array('vsh'),
    'requires_vagrant' => TRUE,
    'requires_vagrant_project' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['vagrant-user'] = array(
    'description' => dt('Initialize or update user-specific settings for Drush Vagrant.'),
    'options' => array(
      'reset' => 'Reset user settings to defaults.',
    ),
    'strict-option-handling' => TRUE,
    'aliases' => array('vuser'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['vagrant-rebuild'] = array(
    'description' => dt('Re-build a Vagrant VM.'),
    'examples' => array(
      'drush vrb' => dt('Destroy the VM and re-build it'),
    ),
    'strict-option-handling' => TRUE,
    'aliases' => array('vrb'),
    'requires_vagrant' => TRUE,
    'requires_vagrant_project' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['vagrant-root'] = array(
    'description' => dt('Print the path to a Vagrant VM.'),
    'options' => array(
      'root' => 'Print the root projects path.',
    ),
    'examples' => array(
      'drush @project-alias vr' => dt('Print the path to the project represented by @project-alias.'),
    ),
    'strict-option-handling' => TRUE,
    'aliases' => array('vr'),
    'requires_vagrant' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );

  // Drush topics
  $items['docs-vagrant-readme'] = array(
    'description' => dt('Drush Vagrant README.'),
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'drush_print_file',
    'callback arguments' => array(DRUSH_VAGRANT_ROOT . '/README.md'),
  );
  $items['docs-vagrant-architecture'] = array(
    'description' => dt('Drush Vagrant architecture documentation.'),
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'drush_print_file',
    'callback arguments' => array(DRUSH_VAGRANT_ROOT . '/docs/architecture.md'),
  );
  $items['docs-vagrant-blueprints'] = array(
    'description' => dt('Drush Vagrant blueprints documentation.'),
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'drush_print_file',
    'callback arguments' => array(DRUSH_VAGRANT_ROOT . '/docs/blueprints.md'),
  );
  $items['docs-vagrant-projects'] = array(
    'description' => dt('Drush Vagrant projects documentation.'),
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'drush_print_file',
    'callback arguments' => array(DRUSH_VAGRANT_ROOT . '/docs/projects.md'),
  );
  $items['docs-vagrant-api'] = array(
    'description' => dt('Drush Vagrant API documentation.'),
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'drush_print_file',
    'callback arguments' => array(DRUSH_VAGRANT_ROOT . '/docs/drush-vagrant.api.php'),
  );

  return $items;
}

/**
 * Command callback for `drush vagrant`.
 */
function drush_vagrant() {
  // hook_drush_command() will pass additional command-line arguments to the
  // specified hook function, so we can recuperate them here.
  $args = func_get_args();
  // Since Drush will parse aliases and options itself, the first argument will
  // be a sub-command
  $command = array_shift($args);
  // To determine whether we should pass along sub-command to Vagrant, we first
  // check that there is no such command implemented in Drush.
  $commands = drush_get_commands();
  if (!isset($commands['vagrant-' . $command])) {
    // If there's no sub-command, provide some guidance (i.e., help text)
    if (empty($command)) {
      drush_shell_exec_interactive('drush vagrant --help');
    }
    // TODO: does this special-case even make sense anymore?
    elseif ($command == 'init' && !is_dir(PROJECT_PATH)) {
      drush_mkdir(PROJECT_PATH);
    }
    else {
      // Get the command as passed on the CLI
      $vg_command = drush_get_context('argv');
      if ($key = array_search('vg', $vg_command)) {
        $vg_command[$key] = 'vagrant';
      }
      $vg_command = implode(' ', array_splice($vg_command, array_search('vagrant', $vg_command)));
      $vg_command = strtr('cd !path && !command', array(
        '!path' => PROJECT_PATH,
        '!command' => $vg_command,
        )
      );

      drush_shell_exec_interactive($vg_command);
    }
  }
  else {
    // Re-dispatch the drush-vagrant sub-command using original CLI arguments
    drush_shell_exec_interactive('drush vagrant-' . implode(' ', drush_get_original_cli_args_and_options()));
  }
}

/**
 * Implements hook_drush_help_alter().
 */
function vagrant_drush_help_alter(&$command) {
  if ($command['command'] === 'vagrant') {
    $args = drush_get_arguments();
    if (isset($args[1])) {
      // Remove the 'drush vagrant' options and aliases for sub-commands
      $command['options'] = array();
      $command['aliases'] = array();
    }
  }
}

/**
 * Return a list of all extensions that implement blueprints
 */
function drush_vagrant_extension_info() {
  $all_extensions = drush_commandfile_list();
  $extensions = drush_command_implements('vagrant_blueprints');
  foreach ($extensions as $key => $value) {
    $blueprints = array_keys(call_user_func($value . '_vagrant_blueprints'));
    $extensions[$value]['blueprints'] = $blueprints;
    $extensions[$value]['full_path'] = dirname($all_extensions[$value]);
    unset($extensions[$key]);
  }

  return $extensions;
}

/**
 * Implements hook_drush_cache_clear().
 */
function vagrant_drush_cache_clear(&$types) {
    $types['vagrant'] = 'vagrant_cache_clear_all';
}

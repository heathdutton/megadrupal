<?php

/**
 * @file
 *
 * Hooks related to the Default blueprint.
 */

/**
 * Implemention of hook_vagrant_blueprints().
 */
function vagrant_vagrant_blueprints() {
  $blueprints = array(
    'default' => array(
      'name' => dt('Default'),
      'description' => dt('The default blueprint.'),
      'path' => 'blueprints/default',
      ),
    );
  return $blueprints;
}

/**
 * Implementation of drush_hook_pre_COMMAND().
 *
 * Set SSH username and identity file options
 */
function drush_vagrant_pre_vagrant_shell() {
  include_once(PROJECT_PATH . '/.config/blueprint.inc');
  if ($options['blueprint']['blueprint'] == 'default') {
    $user_settings = drush_get_context('user');
    // TODO: 'vsh' is already defaulting to the user's user, so this is now useless. vsh probably shouldn't though.
    // TODO: It would be cool for aegir-up to use this technique to become 'aegir' user on login.
    drush_set_option('ssh-user', drush_get_option('ssh-user', $user_settings['vagrant']['username']));
    drush_set_option('ssh-id', drush_get_option('ssh-id', $user_settings['vagrant']['ssh_keysprivate']));
  }
}

/**
 * Implemention of COMMANDFILE_EXTENSION_build().
 */
function vagrant_default_build() {
  vagrant_default_build_steps(vagrant_default_build_vars());
}

function vagrant_default_build_vars() {
  $build = array();
  $build['project_path'] = drush_get_option('project-path');
  $build['project_name'] = drush_get_option('project-name');
  $build['blueprint'] = drush_get_option('blueprint');
  $build['blueprint_info'] = drush_vagrant_blueprint_info();
  $build['git_repo'] = drush_get_option('git-repo');
  $build['git_branch'] = drush_get_option('git-branch', 'master');

  return $build;
}

function vagrant_default_build_steps($build) {
  vagrant_default_build_project_dir($build);
  vagrant_default_build_project_setup($build);
  vagrant_default_build_config_dir($build);
  vagrant_default_build_record_blueprint($build);
  vagrant_default_build_user_data($build);
  vagrant_default_build_user_dotfiles($build);
  vagrant_default_build_git_init($build);
}

function vagrant_default_build_project_dir($build) {
  // Create the project directory from the blueprint, or the git repo
  if ($build['git_repo']) {
    if (drush_shell_exec_interactive('git clone --branch %s %s %s', $build['git_branch'], $build['git_repo'], $build['project_path'])) {
      drush_print(dt('Cloned the git repo at !git_repo to !project_path.', array('!git_repo' => $build['git_repo'], '!project_path' => $build['project_path'])));
    }
    else {
      drush_print(dt('An error occurred when running "git clone !git_repo !project_path".', array('!git_repo' => $build['git_repo'], '!project_path' => $build['project_path'])));
    }
  }
  else {
    drush_vagrant_exec_and_log(array(
      'command' => drush_copy_dir($build['blueprint_info'][$build['blueprint']]['full_path'], $build['project_path']),
      'success' => dt('Copied the "!blueprint" blueprint directory to !project_path.', array('!blueprint' => $build['blueprint'], '!project_path' => $build['project_path'])),
      'failure' => dt('The following errors occurred when running "drush_copy_dir()" in "vagrant_default_build_project_dir()".'),
    ));
  }
}

function vagrant_default_build_project_setup($build) {
  // Set up the new project
  $target = $build['project_path'] . '/Vagrantfile';
  if (!file_exists($target)) {
    drush_vagrant_exec_and_log(array(
      'command' => symlink(DRUSH_VAGRANT_ROOT . '/lib/Vagrantfile', $target),
      'success' => dt('Created symlink to Vagrantfile.'),
      'failure' => dt('Errors occurred when running "symlink()" in "!function".', array('!function' => __FUNCTION__)),
    ));
  }
  $target = $build['project_path'] . '/.gitignore';
  if (!file_exists($target)) {
    drush_vagrant_exec_and_log(array(
      'command' => symlink(DRUSH_VAGRANT_ROOT . '/lib/gitignore', $target),
      'success' => dt('Created symlink to .gitignore.'),
      'failure' => dt('Errors occurred when running "drush_copy_dir()" in "vagrant_default_build_project_setup()".'),
    ));
  }
}

function vagrant_default_build_config_dir(&$build) {
  $build['config_path'] = $build['project_path'] . '/.config';
  drush_vagrant_exec_and_log(array(
    'command' => drush_mkdir($build['config_path']),
    'success' => dt('Created the ".config" directory in !project_path.', array('!project_path' => $build['project_path'])),
    'failure' => dt('Errors occurred when running "drush_mkdir()" in "!function".', array('!function' => __FUNCTION__)),
  ));
}

function vagrant_default_build_record_blueprint($build) {
  if (!isset($build['git_repo'])) {
    // Record the blueprint that was used
    $blueprint_vars['extension'] = $build['blueprint_info'][$build['blueprint']]['extension'];
    $blueprint_vars['blueprint'] = $build['blueprint'];
    $blueprint_file = _drush_vagrant_render_template($blueprint_vars, "blueprint.tpl.php");
    drush_vagrant_exec_and_log(array(
      'command' => file_put_contents($build['config_path'] . '/blueprint.inc', $blueprint_file),
      'success' => dt('Wrote "blueprint.inc" to !config_path.', array('!config_path' => $build['config_path'])),
      'failure' => dt('Errors occurred when running "file_put_contents()" in "!function".', array('!function' => __FUNCTION__)),
    ));
  }
}

function vagrant_default_build_user_data($build) {
  $user_settings = drush_get_context('user');

  $config_vars = array();
  $config_vars["global_path"] =  DRUSH_VAGRANT_ROOT . '/lib/global.rb';
  $config_vars["project_name"] = $build['project_name'];
  $config_vars["subnet"] = drush_get_option('next_subnet', '10');
  # Factor facts
  $config_vars["facts"]["fqdn"] = '$hostname';
  $config_vars["facts"]["username"] = '"' . $user_settings['vagrant']['username'] . '"';
  $config_vars["facts"]["git_name"] = '"' . $user_settings['vagrant']['git_name'] . '"';
  $config_vars["facts"]["git_email"] = '"' . $user_settings['vagrant']['git_email'] . '"';
  $config_vars["facts"]["uid"] = '"' . $user_settings['vagrant']['uid'] . '"';
  $config_vars["facts"]["gid"] = '"' . $user_settings['vagrant']['gid'] . '"';
  if (isset($build["facts"])) {
    foreach ($build["facts"] as $name => $fact) {
      $config_vars["facts"][$name] = $fact;
    }
  }
  # Puppet module directories
  $config_vars["modules"]["project-specific modules"] = "modules/";
  $config_vars["modules"]["drush-vagrant default modules"] = dirname(__FILE__). "/lib/puppet-modules/";
  if (isset($build["modules"])) {
    foreach ($build["modules"] as $name => $dir) {
      $config_vars["modules"][$name] = $dir;
    }
  }
  $config_file = _drush_vagrant_render_template($config_vars, $template = "config.tpl.php");
  if (file_put_contents($build['config_path'] . '/config.rb', $config_file)) {
    drush_log(dt('Successfully wrote config.rb'), 'ok');
  }
  else {
    drush_log(dt('Error writing config.rb'), 'error');
  }
}

function vagrant_default_build_user_dotfiles($build) {
  // Create a directory to contain our files, if it doesn't already exist
  $dotfiles_dir = $build['config_path'] . '/files';
  if (!is_dir($dotfiles_dir)) {
    drush_mkdir($dotfiles_dir);
  }
  // Iterate over our list of files, ensure they exist, and copy them
  $user_settings = drush_get_context('user');
  foreach ($user_settings['vagrant'] as $key => $dotfile_source) {
    if (strpos($key, 'dotfiles') !== FALSE) {
      if (is_file($dotfile_source)) {
        if (copy($dotfile_source, $dotfiles_dir . '/' . basename($dotfile_source))) {
          drush_log(dt('Copied your !dotfile into the project', array('!dotfile' => basename($dotfile_source))), 'ok');
        }
        else {
          drush_log(dt('Error copying !dotfile to !dir in !function.',
            array(
              '!dotfile' => $dotfile_source,
              '!dir' => $dotfiles_dir,
              '!function' => __FUNCTION__,
            )), 'warning');
        }
      }
      else {
        drush_log(dt('Tried to copy !dotfile to your project, as found in your drushrc.php, but it does not appear to be a file.',
          array('!dotfile' => $dotfile_source)), 'warning');
      }
    }
  }
  if (isset($user_settings['vagrant']['ssh_keyspublic'])) {
    copy($user_settings['vagrant']['ssh_keyspublic'], $dotfiles_dir . '/authorized_keys');
  }
}

function vagrant_default_build_git_init($build) {
  if (!file_exists($build['project_path'] . '/.git')) {
    drush_vagrant_shell_cd_and_exec_interactive($build['project_path'], "git init && git add . && git commit -m'Initial commit.'");
  }
}


/**
 * Implemention of COMMANDFILE_EXTENSION_update().
 */
function vagrant_default_update() {
  // TODO: Figure out how we want to handle updates
}

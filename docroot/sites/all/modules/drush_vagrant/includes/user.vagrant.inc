<?php

/**
 * @file
 * Save some user-specific settings in a Drush context
 */

/**
 * Create or update user-specific settings.
 */
function drush_vagrant_user() {

  $user_settings = drush_get_context('user', array());
  // Build new settings if there aren't any saved yet, or we explicitely want
  // to reset them to defaults.
  if (count($user_settings['vagrant']) == 0 || drush_get_option('reset')) {
    $data = _drush_vagrant_generate_user_settings();

    // Build our confirmation prompt
    $defaults = "The following settings will be added to your ~/.drushrc.php:\n";
    $defaults .= _drush_vagrant_format_user_settings($data);
    $defaults .= 'Are these settings correct?';
    $correct = drush_confirm($defaults);

    // Build our user context
    if ($correct) {
      foreach ($data as $key => $datum) {
        $user_settings['vagrant'][$key] = $datum['default'];
      }
    }
    // Or prompt for everything
    else {
      $prompt = 'What !label would you like to use? ';
      foreach ($data as $key => $datum) {
        $user_settings['vagrant'][$key] = drush_prompt(dt($prompt, array( '!label' => $datum['label'])), $datum['default']);
      }
    }
  }
  else {
    // Just list the current settings
    drush_print(_drush_vagrant_format_user_settings($user_settings['vagrant']));
    return;
  }

  // Prompt to create project-root, if it doesn't already exist
  if (!is_dir($user_settings['vagrant']['project_root'])) {
    if (drush_prompt(dt('The directory you specified for your projects (!dir) does not appear to exist. Create it now?', array('!dir' => $user_settings['vagrant']['project_root'])))) {
      drush_mkdir($user_settings['vagrant']['project_root']);
    }
  }

  // Unset global user-settings
  unset($user_setting['config-file']);
  unset($user_setting['context-path']);

  //Save our settings
  drush_set_context('user', $user_settings);
  drush_save_config('user');
}

/**
 * Generate default user settings
 */
function _drush_vagrant_generate_user_settings() {

  $data = array();

  // Environment variables
  // TODO: allow other env vars to be passed through an option or hook?
  $data['username'] = array( 'command' => 'whoami', 'label' => 'username');
  if (!drush_is_windows()) {
    $data['uid'] = array( 'command' => 'id -u', 'label' => 'user id');
    $data['gid'] = array( 'command' => 'id -g', 'label' => 'group id');
  }
  // TODO: else {provide some default} ?
  drush_shell_exec('git --version');
  $result = drush_shell_exec_output();
  $result = explode(' ', $result[0]);
  if ($result[0] == 'git' && $result[1] == 'version') {
    $data['git_name'] = array( 'command' => 'git config --global --get user.name', 'label' => 'Git username');
    $data['git_email'] = array( 'command' => 'git config --global --get user.email', 'label' => 'Git email');
  }
  // Iterate over our list of commands, and get their output.
  foreach ($data as $key => $datum) {
    drush_shell_exec($datum['command']);
    $default = drush_shell_exec_output();
    $data[$key]['default'] = $default[0];
  }

  $data['home'] = array('default' => drush_server_home(), 'label' => "User's home directory");
  $data['project_root'] = array( 'default' => $data['home']['default'] . '/vagrant/projects', 'label' => 'Projects directory');

  // Files
  // TODO: allow other files & filesets to be passed through an option or hook?
  $file_sets = array('dotfiles', 'ssh_keys' );
  $dotfiles = array('.profile', '.bashrc', '.bash_aliases', '.vimrc', );
  if (file_exists($data['home']['default'] . '/.ssh/id_rsa')) {
    $ssh_keys = array('public' => '.ssh/id_rsa.pub', 'private' => '.ssh/id_rsa');
  }
  elseif (file_exists($data['home']['default'] . '/.ssh/id_dsa')) {
    $ssh_keys = array('public' => '.ssh/id_dsa.pub', 'private' => '.ssh/id_dsa');
  }

  // Get file defaults
  foreach ($file_sets as $set) {
    foreach ($$set as $key => $file) {
      if (is_numeric($key)) {
        $name = $file;
      }
      else {
        $name = $key;
      }
      if (file_exists($data['home']['default'] . '/' . $file)) {
        $data[$set . $name] = array('default' => $data['home']['default'] . '/' . $file, 'label' => $name . '(' . $set . ')');
      }
    }
  }

  return $data;
}

/**
 * Format list of user settings
 */
function _drush_vagrant_format_user_settings($data) {
  $output = array();
  // Normalize generated and saved data
  foreach ($data as $key => $value) {
    if (isset($value['label'])) {
      $output[$value['label']] = $value['default'];
    }
    else {
      $output[$key] = $value;
    }
  }

  // find the longest label so we can align values
  $i = 0;
  foreach ($output as $key => $value) {
    if (strlen($key) > $i) {
      $i = strlen($key);
    }
  }

  // Build our list
  $list = '';
  foreach ($output as $key => $value) {
    $list .= '  ' . $key . ' ' . str_repeat('.', $i + 1 - strlen($key)) . ' ' . $value . "\n";
  }

  return $list;
}

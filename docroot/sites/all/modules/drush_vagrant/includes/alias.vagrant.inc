<?php

/**
 * @file
 * Generate Drush aliases.
 */

/**
 * Parse the output of 'vagrant ssh-config'
 */
function drush_vagrant_parse_ssh_config($vm_name = VM_NAME) {
  // 'vagrant ssh-config' will fail if passed a VM name in a single VM project.
  // So we have to count the VMs, and unset $vm_name if there's only one.
  $output = drush_vagrant_exec_and_log(array(
    'command' => drush_shell_cd_and_exec(PROJECT_PATH, "drush vagrant-list --vms"),
    'success' => dt('Listed VMs in this project.'),
    'failure' => dt('The following errors occurred when running "drush vagrant-list --vms":'),
  ));
  if (count($output) == 1) {
    unset($vm_name);
  }

  $output = drush_vagrant_exec_and_log(array(
    'command' => drush_shell_cd_and_exec(PROJECT_PATH, "vagrant ssh-config " . $vm_name),
    'success' => dt('Determined the Vagrant SSH configuration.'),
    'failure' => dt('The following errors occurred when running "vagrant ssh-config". Are you in the directory with the Vagrantfile?'),
  ));

  $ssh_options = array();
  foreach ($output as $line) {
    $split_line = explode(" ", trim($line));
    $ssh_options[array_shift($split_line)] = implode(" ", $split_line);
  }

  return $ssh_options;
}

/**
 * Render an array of SSH options
 */
function drush_vagrant_render_ssh_options($ssh_options) {
  foreach ($ssh_options as $key => $value) {
    $ssh_options[$key] = $key . ' ' . $value;
  }
  return array_reduce($ssh_options, "_drush_vagrant_join_ssh_commands");
}


/**
 * Command callback for `drush vagrant-alias-config`.
 */
function drush_vagrant_alias_config() {
  $args = func_get_args();

  $ssh_options = drush_vagrant_parse_ssh_config();

  // These do not need to go into $alias['ssh-options'].
  unset($ssh_options["User"]);
  unset($ssh_options["Host"]);

  // Set up variables to send to the template.
  $variables = array();
  $variables["ssh_options"] = drush_vagrant_render_ssh_options($ssh_options);

  drush_print(dt('Please enter the information about the Drupal site inside the VM.'));
  $variables["alias"] = drush_prompt(dt('Alias'), "drush-vagrant");
  $variables["uri"] = drush_prompt(dt('URI'), "http://vagrantvm.dev/");
  $variables["docroot"] = drush_prompt(dt('Docroot location'), "/var/www/html/");

  drush_print(dt('Rendering the remote alias entry.'));

  $output = _drush_vagrant_render_template($variables);
  if ($output === NULL) {
    drush_log(dt('Could not render the remote alias entry.'), "error");
  }
  else {
    drush_log(dt('The remote alias entry is below. Please paste it into an aliases.drushrc.php file.'), "ok");
    drush_print();
    drush_print($output);
  }
}

/**
 * Command callback for `drush vagrant-alias`.
 */
function drush_vagrant_alias() {
  $drush_home = drush_server_home() . '/.drush/';
  if (!is_dir($drush_home)) {
    drush_mkdir($drush_home);
  }

  // TODO: Check for a '--delete' option as in 'vals'

  // Set up variables to send to the template.
  $variables = array();

  drush_print(dt('Please enter the information about the Drupal site inside the VM.'));
  if (!($variables["alias"] = drush_get_option('alias'))) {
    $variables["alias"] = drush_prompt(dt('Alias'), "drush-vagrant");
  }
  if (!($variables["uri"] = drush_get_option('uri'))) {
    $variables["uri"] = drush_prompt(dt('URI'), drush_get_option('fqdn', "http://vagrantvm.dev/"));
  }
  // TODO: Validate that this is a valid FQDN
  if (!($variables["docroot"] = drush_get_option('docroot'))) {
    $variables["docroot"] = drush_prompt(dt('Docroot location'), "/var/www/html/");
  }

  // TODO: Instead of "default" we could prefill the name of an actual vm...
  //       maybe skip the question if there is only one in the current project
  if (!($vm_name = drush_get_option('vm-name'))) {
    $vm_name = drush_prompt(dt('Name of the VM'), "default");
  }
  $ssh_options = drush_vagrant_parse_ssh_config();
  if (!($ssh_options["HostName"] = drush_get_option('ip'))) {
    $output = drush_vagrant_exec_and_log(array(
      # TODO: Does this need further processing to get the 2nd IP address?
      # Like: $vm_vars['ip_addr'] = array_pop(explode(' ', $output[1]));
      'command' => drush_shell_cd_and_exec(PROJECT_PATH, 'drush vagrant-shell --default-user --vm-name=' . $vm_name . ' "hostname -I"'),
      'success' => dt('Retrieved the IP address for the !vm_name VM.', array('!vm_name' => $vm_name)),
      'failure' => dt('The following errors occurred when attempting to retrieve the IP address for the !vm_name VM.', array('!vm_name' => $vm_name)),
    ));
    // TODO: Validate that this is a valid IP address
    $ssh_options["HostName"] = array_pop(explode(' ', $output[1]));
  }

  // These do not need to go into $alias['ssh-options'].
  unset($ssh_options["User"]);
  unset($ssh_options["Host"]);
  unset($ssh_options["Port"]);

  $variables["ssh_options"] = drush_vagrant_render_ssh_options($ssh_options);

  drush_print(dt('Rendering the remote alias entry.'));

  $output = _drush_vagrant_render_template($variables);
  if ($output === NULL) {
    drush_log(dt('Could not render the remote alias entry.'), "error");
  }
  else {

    $alias_filename = $drush_home . $variables['alias'] . '.alias.drushrc.php';

    if (!file_exists($alias_filename)) {
      drush_vagrant_exec_and_log(array(
        'command' => file_put_contents($alias_filename, _drush_vagrant_render_template($variables, 'alias.tpl.php')),
        'success' => dt('Wrote project aliases file to !alias_filename.', array('!alias_filename' => $alias_filename)),
        'failure' => dt('Errors occurred when attempting to write project aliases file to !alias_filename.', array('!alias_filename' => $alias_filename)),
      ));
    }
    else {
      # TODO: Check for this as soon as we have an alias, rather than wait for the end. And prompt to continue.
      drush_log(dt('There appears to already be an alias at !alias_name.', array('!alias_name' => $alias_filename)), "error");
    }

  }
}

/**
 * Reduce the ssh options to one long line.
 */
function _drush_vagrant_join_ssh_commands($x, $acc) {
  $x .= "-o'{$acc}' ";
  return $x;
}

/**
 * Run all the variables through a template file.
 */
function _drush_vagrant_render_template($variables, $template = "alias.tpl.php") {
  $template_path = DRUSH_VAGRANT_ROOT . '/templates/' . $template;

  try {
    extract($variables);
    ob_start();
    include $template_path;
    $content = ob_get_clean();
    return $content;
  }
  catch (Exception $e) {
    return FALSE;
  }
}

/**
 * Generate aliases for a project and its VMs.
 */
function drush_vagrant_aliases() {
  $drush_home = drush_server_home() . '/.drush/';
  if (!is_dir($drush_home)) {
    drush_mkdir($drush_home);
  }

  $alias_filename = $drush_home . drush_vagrant_get_project_name(PROJECT_PATH) . '.aliases.drushrc.php';

  if ($delete = drush_get_option('delete')) {
    if (is_file($alias_filename)) {
      if (!drush_confirm(dt('Delete the aliases for this project?'))) {
        return drush_user_abort();
      }
      $status = drush_shell_exec('rm %s', $alias_filename);
      return $status;
    }
    else {
      drush_log(dt('ERROR: There do not appear to be any aliases for this project.'), 'warning');
    }
  }
  else {

    if (is_file($alias_filename)) {
      drush_log(dt('WARNING: There appear to already be aliases for this project.'), 'warning');
      if (!drush_confirm(dt('Regenerate the aliases for this project?'))) {
        return drush_user_abort();
      }
    }

    drush_shell_cd_and_exec(PROJECT_PATH, 'vagrant up');

    $project_vars["project_path"] = PROJECT_PATH;

    $vms_info = drush_vagrant_vms_info(PROJECT_PATH);
    foreach ($vms_info as $vm_name => $data) {
      $vm_vars = $project_vars;
      $vm_vars['vm_alias'] = $vm_name;
      $output = drush_vagrant_exec_and_log(array(
        'command' => drush_shell_cd_and_exec(PROJECT_PATH, 'drush vagrant-shell --default-user --vm-name=' . $vm_name . ' "hostname -f"'),
        'success' => dt('Retrieved the FQDN for the !vm_name VM.', array('!vm_name' => $vm_name)),
        'failure' => dt('The following errors occurred when attempting to retrieve the FQDN for the !vm_name VM.', array('!vm_name' => $vm_name)),
      ));
      // TODO: Validate that this is a valid FQDN
      $vm_vars['fqdn'] = $output[2];
      $output = drush_vagrant_exec_and_log(array(
        'command' => drush_shell_cd_and_exec(PROJECT_PATH, 'drush vagrant-shell --default-user --vm-name=' . $vm_name . ' "hostname -I"'),
        'success' => dt('Retrieved the IP address for the !vm_name VM.', array('!vm_name' => $vm_name)),
        'failure' => dt('The following errors occurred when attempting to retrieve the IP address for the !vm_name VM.', array('!vm_name' => $vm_name)),
      ));
      // TODO: Validate that this is a valid IP address
      $vm_vars['ip_addr'] = array_pop(explode(' ', $output[2]));
      $vms[] = _drush_vagrant_render_template($vm_vars, 'vm_alias.tpl.php');
    }
    $project_vars["vms"] = $vms;

    $aliases = _drush_vagrant_render_template($project_vars, 'project_alias.tpl.php');

    drush_vagrant_exec_and_log(array(
      'command' => file_put_contents($alias_filename, _drush_vagrant_render_template($project_vars, 'project_alias.tpl.php')),
      'success' => dt('Wrote project aliases file to !alias_filename.', array('!alias_filename' => $alias_filename)),
      'failure' => dt('Errors occurred when attempting to write project aliases file to !alias_filename.', array('!alias_filename' => $alias_filename)),
    ));
  }
}

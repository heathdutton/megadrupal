<?php

/**
 * @file
 * Delete a project directory after destroying VMs, and removing hosts and
 * aliases.
 */

/**
 * Validate the project to delete
 */
function drush_vagrant_delete_validate() {
  drush_vagrant_get_project_path();
}

/**
 * Delete an existing project.
 */
function drush_vagrant_delete() {

  $project_alias = drush_get_context('alias');

  if (isset($project_alias['#name'])) {
    $project_alias = $project_alias['#name'];
  }
  else {
    $project_alias = '@' . drush_vagrant_get_project_name(PROJECT_PATH);
  }

  drush_print(dt('!!! WARNING: This operation cannot be undone. !!!'));
  drush_print(dt('This operation will delete the "!project" project, and destroy all of its VMs', array('!project' => drush_vagrant_get_project_name(PROJECT_PATH))));

  if (drush_confirm(dt('Are you certain that you want to proceed?'))) {
    $vms = drush_vagrant_exec_and_log(array(
      'command' => drush_shell_cd_and_exec(PROJECT_PATH, 'drush vagrant-list --vms'),
      'success' => dt('Listed all VMs of the project at !project_path.', array('!project_path' => PROJECT_PATH)),
      'failure' => dt('The following errors occurred when running "drush vagrant-list --vms".'),
    ));
    foreach ($vms as $vm_name) {
      drush_vagrant_exec_and_log(array(
        'command' => drush_shell_exec_interactive('drush @%s.%s hosts --remove', str_replace('.project', '', $project_alias) , $vm_name),
        'success' => dt('Removed the entry in /etc/hosts for "!vm_name".', array('!vm_name' => $vm_name)),
        'failure' => dt('The following errors occurred when running "drush [alias] hosts --remove".'),
      ));
    }
    drush_vagrant_exec_and_log(array(
      'command'  => drush_vagrant_shell_cd_and_exec_interactive(PROJECT_PATH, 'vagrant destroy --force'),
      'success' => dt('Destroyed all VMs of the project at !project_path.', array('!project_path' => PROJECT_PATH)),
      'failure' => dt('The following errors occurred when running "vagrant destroy --force".'),
    ));
    // Check that the VMs were destroyed
    if (!is_file(PROJECT_PATH . '/.vagrant')) {
      drush_vagrant_exec_and_log(array(
        'command'  => drush_vagrant_shell_cd_and_exec_interactive(PROJECT_PATH, 'drush -y vagrant-aliases --delete'),
        'success' => dt('Deleted all aliases for the project at !project_path.', array('!project_path' => PROJECT_PATH)),
        'failure' => dt('The following errors occurred when running "drush vagrant-aliases --delete".'),
      ));
      drush_vagrant_exec_and_log(array(
        'command'  => drush_shell_exec('rm -rf %s', PROJECT_PATH),
        'success' => dt('Removed the project directory at !project_path.', array('!project_path' => PROJECT_PATH)),
        'failure' => dt('The following errors occurred when running "rm -rf".'),
      ));
      if (getcwd() === FALSE) {
        drush_print(dt("Your current working directory was deleted, please cd into another directory. E.g. 'cd ..'"));
      }
    }
    else {
      drush_set_error(dt('Failure during "vagrant destroy"'));
    }
  }
  else {
    drush_user_abort();
  }
}

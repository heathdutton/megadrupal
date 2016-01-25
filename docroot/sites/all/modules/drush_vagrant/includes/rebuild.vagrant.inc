<?php

/**
 * Re-build the VMs in a project.
 *
 * WARNING: this destroys the current VMs in the vagrant project.
 */
function drush_vagrant_rebuild() {

  drush_print(dt('!!! WARNING: This operation cannot be undone. !!!'));
  drush_print(dt('This operation will destroy all VMs in "!project". And build a fresh set afterwards.', array('!project' => drush_vagrant_get_project_name(PROJECT_PATH))));

  if (drush_confirm(dt('Are you certain that you want to proceed?'))) {
    $cwd = getcwd();
    drush_op('chdir', PROJECT_PATH);
    drush_shell_exec_interactive("vagrant destroy -f && vagrant up");
    drush_op('chdir', $cwd);
  }
}

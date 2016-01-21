<?php

/**
 * Implementation of hook_post_hosting_TASK_TYPE_task() for delete tasks.
 */
function aegir_ssh_post_hosting_verify_task($task, $data)
{

  // When a server is being verified...
  if ($task->ref->type == 'server') {

    // Check for authorized_keys.
    if (empty($task->ref->authorized_keys) || count($task->ref->authorized_keys) < 1) {
      return;
    }

    drush_log('[aegir_ssh] server verify...', 'ok');

    // Queue site deletion for each environment.
    $count = count($task->ref->authorized_keys);
    drush_log('[aegir_ssh] ' . $count . ' ssh keys found', 'ok');

    // Write to "authorized_keys".
    // TODO: We should probably backup the existing authorized key file!
    $authorized_keys = implode("\n", $task->ref->authorized_keys);

    if (file_put_contents('/var/aegir/.ssh/authorized_keys', $authorized_keys)) {
      drush_log('[aegir_ssh] Wrote /var/aegir/.ssh/authorized_keys!', 'ok');
      chmod('/var/aegir/.ssh/authorized_keys', 0600);
      drush_log('[aegir_ssh] Changed Permissions of /var/aegir/.ssh/authorized_keys to 600', 'ok');

    }
    else {
      return drush_set_error('AEGIR_ERROR', '[aegir_ssh] COULD NOT write ~/.ssh/authorized_keys!');
    }
  }
}
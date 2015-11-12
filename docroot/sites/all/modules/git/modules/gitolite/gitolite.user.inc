<?php
/**
 * @file
 *  User integration
 */

/**
 * Adds a user to tto Gitolite.
 */
function gitolite_add_user($variables) {
  try {
    $drupal_user = $variables['account'];
    $git_server = $variables['server'];
    $gitolite = gitolite_client($git_server->user, $git_server->host);

    if ($drupal_user->uid == 0) {
      return TRUE;
    }

    // Load all the sshkey entities for this user and create them
    $keys = sshkey_load_multiple(FALSE, array('entity_type' => 'user', 'entity_id' => $drupal_user->uid));
    $user = $gitolite->loadUser($drupal_user->mail);

    foreach ($keys as $key) {
      $user->addKey($key->key_id, $key->value);
    }

    foreach ($drupal_user->roles as $rid => $role) {
      if ($role = user_role_load($rid)) {
        $group = $gitolite->loadGroup($role->name, 'User');
        $group->addItem($user);
        $gitolite->save($group);
      }
    }

    $gitolite->save($user);
    if (empty($variables['dont_commit'])) {
      $gitolite->getAdminRepository()->commit('Added user ' . $user->getName());
      $gitolite->push();
    }
  }
  catch (Exception $e) {
    watchdog_exception('gitolite', $e);
    $message = theme('gitolite_message', array('message' => 'Could not add user. To recover you may want to !synchronise-link.'));
    drupal_set_message($message, 'error');
    return FALSE;
  }
}

/**
 * Deletes a user from Gitolite
 */
function gitolite_delete_user($variables) {
  try {
    $drupal_user = $variables['account'];
    $git_server = $variables['server'];
    $gitolite = gitolite_client($git_server->user, $git_server->host);

    $user = $gitolite->loadUser($drupal_user->mail);
    $groups = $gitolite->loadGroups('User');

    foreach ($groups as $group) {
      $group->removeItem($user);
    }
    $gitolite->save($groups);
    $gitolite->delete($user);

    if (empty($variables['dont_commit'])) {
      $gitolite->getAdminRepository()->commit('Deleted user ' . $user->getName());
      $gitolite->push();
    }
  }
  catch (Exception $e) {
    watchdog_exception('gitolite', $e);
    $message = theme('gitolite_message', array('message' => 'Could not delete user. To recover you may want to !synchronise-link.'));
    drupal_set_message($message, 'error');
    return FALSE;
  }
}

/**
 * Deletes and recreates a user in gitolite.
 */
function gitolite_update_user($variables) {
  try {
    $new_user = $variables['account'];
    $original_user = $new_user->original;
    $git_server = $variables['server'];
    $gitolite = gitolite_client($git_server->user, $git_server->host);

    gitolite_delete_user(array('account' => $original_user, 'server' => $git_server, 'dont_commit' => TRUE));
    gitolite_add_user(array('account' => $new_user, 'server' => $git_server, 'dont_commit' => TRUE));

    $repository = $gitolite->getAdminRepository();
    if ($repository->hasChanges()) {
      $repository->commit("Updated user {$original_user->mail} => {$new_user->mail}");
      $repository->push();
    }
  }
  catch (Exception $e) {
    watchdog_exception('gitolite', $e);
    $message = theme('gitolite_message', array('message' => 'Could not update user. To recover you may want to !synchronise-link.'));
    drupal_set_message($message, 'error');
    return FALSE;
  }
}

/**
 * Synchronises all users
 */
function gitolite_server_sync($git_server) {
  try {
    $gitolite = gitolite_client($git_server->user, $git_server->host);
    $repository = $gitolite->getAdminRepository();

    $admin_users = array();
    foreach ($git_server->field_admin_users[LANGUAGE_NONE] as $admin_user) {
      $admin_users[] = $admin_user['safe_value'];
    }

    $users = $gitolite->loadUsers();
    $groups = $gitolite->loadGroups('User');
    foreach ($users as $user) {
      if (in_array($user->getName(), $admin_users)) {
        continue;
      }
      // Remove all users.
      foreach ($groups as $group) {
        $group->removeItem($user);
      }
      $gitolite->delete($user);
    }
    $gitolite->delete($groups);

    foreach (entity_load('user') as $drupal_user) {
      gitolite_add_user(array('account' => $drupal_user, 'server' => $git_server, 'dont_commit' => TRUE));
    }

    if ($repository->hasChanges()) {
      $repository->commit("Resynchronised all users");
      $gitolite->push();
      drupal_set_message('Successfully synchronised all users to gitolite server.');
    }
    else {
      drupal_set_message('No changes needed synchronising.');
    }
  }
  catch (Exception $e) {
    watchdog_exception('gitolite', $e);
    drupal_set_message(t('Failed to synchronise server'), 'error');
    return FALSE;
  }
}

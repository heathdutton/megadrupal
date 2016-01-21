<?php
/**
 * @file
 *  SSH Key module integration
 *
 * @see gitolite.rules.inc
 */

/**
 * Implements hook_sshkey_insert().
 */
function gitolite_sshkey_insert($key) {
  if ($key->entity_type == 'user') {
    rules_invoke_event('gitolite_user_sshkey_insert', $key);
  }
}

/**
 * Implements hook_sshkey_update().
 */
function gitolite_sshkey_update($key) {
  if ($key->entity_type == 'user') {
    rules_invoke_event('gitolite_user_sshkey_update', $key);
  }
}

/**
 * Implements hook_sshkey_delete().
 */
function gitolite_sshkey_delete($key) {
  if ($key->entity_type == 'user') {
    rules_invoke_event('gitolite_user_sshkey_delete', $key);
  }
}


/**
 * Adds a public key to the gitolite server.
 *
 * @param object $key The key object.
 * @param object $git_server The git_server object.
 */
function gitolite_add_user_public_key($variables) {
  try {
    $key = $variables['sshkey'];
    $git_server = $variables['server'];

    $drupal_user = reset(entity_load('user', array($key->entity_id)));
    $gitolite = gitolite_client($git_server->user, $git_server->host);

    $user = $gitolite->loadUser($drupal_user->mail);
    $user->addKey($key->key_id, $key->value);
    $gitolite->save($user, "Added key for " . $user->getName());
    if (empty($variables['dont_push'])) {
      $gitolite->push();
    }

    return TRUE;
  }
  catch (Exception $e) {
    watchdog_exception('gitolite', $e);
    $message = theme('gitolite_message', array(
      'message' => 'Could not create/update key with fingerprint %fingerprint in gitolite for user @user_id. To recover you may want to !synchronise-link.',
      'key' => $key,
    ));
    drupal_set_message($message, 'error');
    return FALSE;
  }
}

/**
 * Adds a public key to the gitolite server.
 *
 * @param object $key The key object.
 * @param object $git_server The git_server object.
 * @see gitolite_sshkey_delete()
 */
function gitolite_delete_user_public_key($variables) {
  try {
    $key = $variables['sshkey'];
    $git_server = $variables['server'];

    $user = reset(entity_load('user', array($key->entity_id)));
    $gitolite = gitolite_client($git_server->user, $git_server->host);

    $user = $gitolite->loadUser($user->mail);
    $user->deleteKey($key->key_id);
    $gitolite->save($user, "Delete key for " . $user->getName());
    if (empty($variables['dont_push'])) {
      $gitolite->push();
    }

    return TRUE;
  }
  catch (Exception $e) {
    watchdog_exception('gitolite', $e);
    $message = theme('gitolite_message', array(
      'message' => 'Could not remove key with fingerprint %fingerprint from gitolite for user @user_id. To recover you may want to !synchronise-link.',
      'key' => $key,
    ));
    drupal_set_message($message, 'error');
    return FALSE;
  }
}

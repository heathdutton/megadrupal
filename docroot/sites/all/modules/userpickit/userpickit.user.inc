<?php

/**
 * @file
 *
 * Support Drupal core' user pictures.
 *
 * If the picture chosen by the user is stored locally, userpickit replaces
 * the user picture file associated with a user account ($user->picture).
 *
 * This file includes additional support for stashing aside uploaded pictures
 * when they are replaced by userpickit when a user chooses a different
 * picture type.
 */

/**
 * Implements hook_userpickit_info().
 */
function userpickit_userpickit_info() {
  $picture_types = array();

  $t = array('@sitename' => variable_get('site_name', ''));
  $picture_types['user'] = array(
    'title' => t('Picture uploaded to @sitename', $t),
    'description' => t('Image uploaded using the standard Drupal interface.'),
    'callback' => 'userpickit_userpickit_picture',
    'default callback' => 'userpickit_userpickit_picture_default',
  );

  return $picture_types;
}

/**
 * Callback.
 *
 * @see userpickit_userpickit_info().
 */
function userpickit_userpickit_picture($picture_type, $account) {
  $upload = userpickit_user_picture_upload_get($account->uid);
  if ($upload) {
    return array(
      'fid' => $upload->fid,
      'message' => t('Your uploaded picture. Use the form below to replace.'),
      // Cache lifetime is managed by userpickit_user_picture_upload_set() when
      // user overrides his picture. This must be set or the picture may get
      // overridden by the selected picture type if it expires on the same
      // request. See _userpickit_entity_update_user() for behaviour.
      'cache lifetime' => USERPICKIT_CACHE_EXPIRE_NEVER,
    );
  }

  return array(
    'uri' => variable_get('user_picture_default', ''),
    'message' => t('No picture has been uploaded yet, so the site default picture is displayed. Use the form below to upload your own picture.'),
  );
}

/**
 * @see userpickit_userpickit_info()
 */
function userpickit_userpickit_picture_default() {
  return variable_get('user_picture_default', '');
}

/**
 * If after save $user->picture is not known to userpickit, then assume
 * user replaced the image manually.
 *
 * @param object $entity
 * @param string $entity_type
 *
 * @see userpickit_entity_update()
 */
function _userpickit_entity_update_user($entity, $entity_type) {
  if ($entity_type == 'user' && !empty($entity->picture)) {
    $picture = is_numeric($entity->picture) ? file_load($entity->picture) : $entity->picture;
    if (!empty($picture->fid)) {
      $count = db_select('userpickit_pictures', 'f')
        ->fields('f')
        ->condition('fid', $picture->fid)
        ->condition('uid', $entity->uid)
        ->addTag(__FUNCTION__)
        ->countQuery()
        ->execute()
        ->fetchField();

      if (is_numeric($count) && !$count) {
        userpickit_user_picture_upload_set($entity->uid, $picture->fid);
      }
    }
  }
}

/**
 * Get file associated with users picture upload.
 *
 * @param int $uid
 *   User ID.
 */
function userpickit_user_picture_upload_get($uid) {
  return db_select('userpickit_user_upload', 'up')
    ->fields('up', array('fid'))
    ->condition('uid', $uid)
    ->addTag(__FUNCTION__)
    ->execute()
    ->fetchObject();
}

/**
 * Set a new file to use as a users picture.
 *
 * Disassociates and deletes previous pictures if available.
 *
 * @param int $uid
 *   User ID.
 * @param int $fid
 *   File ID.
 */
function userpickit_user_picture_upload_set($uid, $fid) {
  $upload = userpickit_user_picture_upload_get($uid);

  if ($upload && $fid != $upload->fid) {
    $picture_previous = file_load($upload->fid);
    if ($picture_previous) {
      file_usage_delete($picture_previous, 'userpickit_user', 'user', $uid, 0);
      file_delete($picture_previous);
    }
  }

  if ($fid) {
    $picture_new = file_load($fid);
    if ($picture_new) {
      $row = array(
        'fid' => $fid,
      );

      db_merge('userpickit_user_upload')
        ->key(array(
          'uid' => $uid,
        ))
        ->fields($row)
        ->execute();

      file_usage_add($picture_new, 'userpickit_user', 'user', $uid);
    }
  }
  else {
    db_delete('userpickit_user_upload')
      ->condition('uid', $uid)
      ->execute();
  }

  $account = user_load($uid);
  userpickit_cache_rebuild('user', $account);
}

/**
 * Set picture type preferences for users who have uploaded a picture.
 *
 * Creates a new batch.
 *
 * @see userpickit_maintenance_form()
 */
function userpickit_maintenance_migrate_user_preferences() {
  $users_per_operation = 50;
  $operations = array();

  $query = new EntityFieldQuery();
  $user_count = $query
    ->entityCondition('entity_type', 'user')
    ->propertyCondition('uid', '0', '>')
    ->propertyCondition('picture', '0', '>')
    ->count()
    ->execute();

  $operations_count = ceil($user_count / $users_per_operation);

  for ($i = 0; $i < $operations_count; $i++) {
    $operations[] = array('userpickit_maintenance_migrate_user_preferences_operation', array($users_per_operation));
  }

  $batch = array(
    'operations' => $operations,
    'finished' => 'userpickit_maintenance_migrate_user_preferences_finish'
  );
  batch_set($batch);
}

/**
 * Process preferences for a set of users.
 *
 * @param int $users_to_process
 *   Number of users to process in this operation.
 */
function userpickit_maintenance_migrate_user_preferences_operation($users_to_process) {
  $uid = variable_get(USERPICKIT_VARIABLE_MIGRATION_USER_PREFERENCES_UID, 0);

  $query = new EntityFieldQuery();
  $entity_ids = $query
    ->entityCondition('entity_type', 'user')
    ->propertyCondition('uid', $uid, '>')
    ->propertyCondition('picture', '0', '>')
    ->propertyOrderBy('uid', 'ASC')
    ->range(0, $users_to_process)
    ->execute();

  $users = entity_load('user', array_keys($entity_ids['user']));
  foreach ($users as $user) {
    $picture_type = userpickit_picture_type_user_get($user, FALSE);
    if (!$picture_type) {
      userpickit_picture_type_user_set('user', $user->uid);
      $picture = is_numeric($user->picture) ? file_load($user->picture) : $user->picture;
      userpickit_user_picture_upload_set($user->uid, $picture->fid);
    }
    $uid = $user->uid + 1;
  }

  variable_set(USERPICKIT_VARIABLE_MIGRATION_USER_PREFERENCES_UID, $uid);
}

function userpickit_maintenance_migrate_user_preferences_finish() {
  variable_del(USERPICKIT_VARIABLE_MIGRATION_USER_PREFERENCES_NOT_DONE);
  drupal_set_message('Finished migrating user preferences.');
}
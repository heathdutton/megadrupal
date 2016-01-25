<?php

/**
 * @file
 * Drush commands for the responsesys module.
 */

/**
 * Implements hook_drush_help().
 */
function responsys_drush_help($command) {
  switch ($command) {
    case 'drush:responsys-list-folders':
      return dt('Lists folders');
  }
}

/**
 * Implements hook_drush_command().
 */
function responsys_drush_command() {

  $items = array();

  $items['responsys-list-folders'] = array(
    'description' => dt('List folders'),
    'aliases' => array('rlf'),
  );

  $items['responsys-createf'] = array(
    'description' => dt('Create folder'),
    'aliases' => array('resp-cf'),
    'arguments' => array(
      'folder' => 'The name of the folder to create',
    ),
  );

  $items['responsys-get-member'] = array(
    'description' => dt('Get details of a user from responsys'),
    'aliases' => array('rgm'),
    'arguments' => array(
      'email' => 'The members email address',
      'folder' => '(optional) The name of the folder, defaults to system default.',
      'list' => '(optional) The name of the list, defaults to the default.',
    ),
  );

  $items['responsys-saved'] = array(
    'description' => dt('Create or update a document'),
    'aliases' => array('resp-sd'),
    'arguments' => array(
      'folder' => 'The name of the folder the document will be in',
      'document_name' => 'The name of the document',
      'document_contents' => 'The data to put into the document',
    ),
  );

  $items['responsys-sync-user'] = array(
    'description' => dt('Sync a users details with responsys'),
    'aliases' => array('resp-su'),
    'arguments' => array(
      'uid' => 'The user uid to sync',
    ),
    'options' => array(
      'folder' => 'The folder to sync the list in. Defaults to the system default one',
      'list' => 'The mailing list to sync. Defaults to the system default one',
      'match_column_1' => 'The first column to match by, defaults to the system default one.',
      'match_column_2' => 'The second column to match by, defaults to the system default one.',
      'match_column_3' => 'The third column to match by, defaults to the system default one.',
    ),
  );

  return $items;
}

/**
 * Create a new folder on responsys.
 *
 * @param string $folder_name
 *   The name of the folder to create
 */
function drush_responsys_createf($folder_name = NULL) {

  if (!$folder_name) {
    drush_set_error('Please specify a folder name');
    return;
  }

  try {
    $ws = wsif_get_api('responsys');

    $errors = array();
    $folders = $ws->CreateFolder($folder_name, $errors);
    if ($errors) {
      if (is_array($errors)) {
        $errors = print_r($errors, 1);
      }
      drush_set_error('Errors creating folder: ' . $errors);
    }
    else {
      drush_print("Created folder {$folder_name}");
    }

  }
  catch (Exception $e) {
    drush_set_error($e->getMessage());
  }

}

/**
 * List folders on the responsys account.
 */
function drush_responsys_list_folders() {
  try {
    foreach (responsys_get_folders() as $folder) {
      drush_print($folder);
    }
  }
  catch (Exception $e) {
    drush_set_error($e->getMessage());
  }
}

/**
 * Get a member based on their email.
 */
function drush_responsys_get_member($email, $folder = NULL, $list = NULL) {
  try {
    $info = responsys_get_memberinfo($email, $folder, $list);
    drush_print_r($info);
  }
  catch (Exception $e) {
    drush_set_error($e->getMessage());
  }
}

/**
 * Create or update a specified document.
 */
function drush_responsys_saved($folder, $document_name, $document_content = 'some test content') {
  if (empty($folder) || empty($document_name)) {
    drush_set_error('You must supply a folder name and a document name');
    return;
  }

  if (!drush_confirm("Are you sure you would like to create or update {$folder}/{$document_name}?")) {
    drush_print('bye');
    return;
  }

  try {
    $api = wsif_get_api('responsys');

    $success = $api->saveDocument($folder, $document_name, $document_content);

    if ($success) {
      drush_print($document_name . ' saved');
    }
    else {
      drush_set_error('Could not save the document: ' . $api->getLastError());
    }
  }
  catch (Exception $e) {
    drush_set_error($e->getMessage());
  }
}

/**
 * Sync a user with responsys.
 *
 * @param int $uid
 *   The user uid to sync
 */
function drush_responsys_sync_user($uid) {
  if (empty($uid) || !is_numeric($uid) || $uid < 1) {
    drush_set_error('User uid must be a positive integer');
    return;
  }

  $folder = drush_get_option('folder', '');
  $list = drush_get_option('list', '');
  $user = user_load($uid);

  if (empty($user)) {
    drush_set_error($uid . ' is an invalid user');
    return;
  }

  // Get an array of Responsys values for the user, for this list.
  $merge_vars = responsys_get_user_fields($user, $folder, $list);

  try {
    // Send the sync command to Responsys.
    $response = responsys_update_user($user->mail, $merge_vars, $folder, $list, TRUE);
    drush_print_r($response);
  }
  catch (Exception $e) {
    drush_set_error($e->getMessage());
  }
}

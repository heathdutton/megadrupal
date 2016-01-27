<?php
/**
 * @file
 * Enables users to be exported and imported.
 */

/**
 * Callback function to export users.
 */
function data_export_import_callback_export_users() {

  return drupal_get_form('data_export_import_export_users_form');
}

/**
 * Function to create form to export users.
 */
function data_export_import_export_users_form($form_state) {

  $form['export_users'] = array(
    '#type' => 'fieldset',
    '#title' => t('Export users'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['export_users']['description'] = array(
    '#type' => 'item',
    '#title' => t('Export all users to a dataset file'),
  );

  // Adds a simple submit button that refreshes the form and clears its
  // contents. This is the default behavior for forms.
  $form['export_users']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create dataset file'),
  );

  return $form;
}

/**
 * Function to process form to export users.
 */
function data_export_import_export_users_form_submit($form, &$form_state) {

  // This is the function which will carry out the exporting of the
  // users to a dataset file.
  $dataset_file_created = data_export_import_export_users_to_file();

  drupal_set_message(t("The following dataset file was created:"));
  drupal_set_message(check_plain($dataset_file_created));

  return TRUE;
}

/**
 * Export the required dataset file.
 */
function data_export_import_export_users_to_file() {

  // This will be the main array which will hold the data which will
  // be output to the dataset file.
  $dataset = array();

  // Here will add the list of roles to the dataset - this is so we can check
  // that the receiving instance has a matching set of roles.
  $dataset['roles'] = user_roles();

  $result = db_query("SELECT uid FROM {users}");

  foreach ($result as $user) {

    // We will not put the anonymous or admin users into the file.  These are
    // expected to exist as standard.
    if ($user->uid > 1) {

      // Attach the data for a user to the array.
      $dataset['users'][$user->uid] = user_load($user->uid);
    }
  }

  // Here we will serialize the array to convert it to a string which
  // can then be output to a file.
  $dataset_serialized = serialize($dataset);

  // Create the default directory to hold the datasets.
  $dataset_directory_parent_directory = variable_get('file_public_path', conf_path() . '/files') . "/data_export_import";
  file_prepare_directory($dataset_directory_parent_directory, $mode = FILE_CREATE_DIRECTORY);

  $dataset_directory = variable_get('file_public_path', conf_path() . '/files') . "/data_export_import/users/";
  file_prepare_directory($dataset_directory, $mode = FILE_CREATE_DIRECTORY);

  // Save the string as a file. This is the dataset data file.
  $file_name = format_date(REQUEST_TIME, 'custom', 'Ymd_His') . "_users.dataset";
  $file_path_and_name = $dataset_directory . "/" . $file_name;

  file_unmanaged_save_data($dataset_serialized, $file_path_and_name, FILE_EXISTS_REPLACE);

  return $file_name;
}

/**
 * Callback function to import users.
 */
function data_export_import_callback_import_users() {

  return drupal_get_form('data_export_import_import_users_form');
}

/**
 * Function to create form to import users.
 */
function data_export_import_import_users_form($form_state) {

  $form = array();

  $form['import_users'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import users'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // Get the contents of the dataset directory and create a list of
  // links to dataset files.
  $directory = variable_get('file_public_path', conf_path() . '/files') . "/data_export_import/users";

  $mask = '/.dataset/';

  $files = file_scan_directory($directory, $mask);

  // Sort them by the filename which is used as the key.  Since the
  // files are named using datetime stamps they will be listed in
  // date/time order.
  ksort($files);

  $options = array();
  $options['none'] = "None";

  foreach ($files as $file) {
    $options[$file->filename] = check_plain($file->filename);
  }

  $form['import_users']['dataset_file'] = array(
    '#type' => 'radios',
    '#title' => t('Select file to import - NB You might want to switch off account activation email notification at /admin/config/people/accounts.'),
    '#default_value' => 'none',
    '#options' => $options,
  );

  $form['import_users']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import dataset files'),
  );

  return $form;
}

/**
 * Function to process form to import users.
 */
function data_export_import_import_users_form_submit($form, &$form_state) {

  if ($form_state['values']['dataset_file'] != 'none') {

    $result = data_export_import_import_users($form_state['values']['dataset_file']);

    if ($result) {
      drupal_set_message(t('The Users dataset file %dataset_file was imported.', array('%dataset_file' => $form_state['values']['dataset_file'])));
      drupal_set_message(t('Any required role settings for the users will now have to be set by using the users administration screen.'));
    }
    else {
      drupal_set_message(t('The Users dataset file %dataset_file was not imported.', array('%dataset_file' => $form_state['values']['dataset_file'])), 'error');
    }
  }
}

/**
 * Make users list match the users from a dataset file.
 *
 * This function is to effectively 'import'  all the users in the dataset file.
 *
 * We will check that the roles on the receiving instance match the roles from
 * the dataset.
 *
 * Then we will bring in the users from the dataset file.
 *
 * NB - When this import has finished the users in the receiving
 * instance should be an exact match with the users in the imported
 * dataset file.  Think in terms of rsync with the '--delete'
 * option. This means that as well as importing new users and updating
 * existing users we need to delete users from the receiving instance
 * which are not in the imported dataset.
 *
 * This synchronisation will be carried out by two passes.
 *
 * First we will loop through the users in the receiving instance  and
 * check against the the imported dataset. If the user exists in the
 * dataset then it will be updated in the receiving instance.  If it
 * doesn't exist in the dataset then it will be deleted from the
 * receiving Drupal instance.
 *
 * The second pass will be a loop through the dataset - any users
 * which are in the dataset but are not in the receiving Drupal
 * instance will be created.
 *
 * This will effectively mean that the users have been sychronised
 * completely.
 *
 * NB - This function will create new users with the same ID as they had
 * originally when they were exported to the dataset file.
 *
 * @param string $file
 *   The dataset file which is being imported.
 *
 * @return bool
 *   TRUE if the import process ran without errors.
 */
function data_export_import_import_users($file) {

  // Load the dataset file into a variable.
  $file_content = file_get_contents(variable_get('file_public_path', conf_path() . '/files') . "/data_export_import/users/" . $file);

  // Decode the serialized data and store it in an array of objects.
  $file_content_as_array = unserialize($file_content);

  // Here we will pick up if the file contains no users.
  if (empty($file_content_as_array['users'])) {
    drupal_set_message(t("There are no users in the dataset file"), 'error');

    // Here we need to delete all the users apart from the admin user
    // account and the anonymous account.  The logic is a bit
    // complicated - but as we are going to remove any users which are
    // not in the dataset file it will be simpler here to catch this
    // special case where the dataset file contains no users.
    $result = db_query("SELECT uid FROM {users}");

    foreach ($result as $user) {

      // Ignore the anonymous and default admin users.
      if ($user->uid != 0 && $user->uid != 1) {

        // Here we will delete the user account and delete all content
        // owned by that user.
        user_cancel(array(), $user->uid, 'user_cancel_delete');
      }
    }
    return TRUE;
  }

  // This will check for differences between the roles which were in
  // the exporting instance and the roles in the receiving instance.
  // Currently the import will not set roles for the users imported
  // and the roles will have to be manually set after the import.  An
  // enhancement will be to put a tick box in the options box to say
  // to set the roles to be the same as the roles which were
  // exported. This would require a strict check to be in place RE
  // the roles. This check would have to check that the roles needed
  // exist on the receiving instance and that they have the same ID
  // numbers. If the ID numbers do not match then importing the users
  // will mean that users would be assigned to the wrong roles.
  $current_roles = user_roles();

  // Loop through the roles from the dataset and check there is a corresponding
  // role in the receiving instance.
  foreach ($file_content_as_array['roles'] as $dataset_role_key => $dataset_role_value) {

    if (!isset($current_roles[$dataset_role_key])) {

      drupal_set_message(t('The receiving instance does not have a role with ID of %id and name of %name.', array('%id' => $dataset_role_key, '%name' => $dataset_role_value)), 'warning');
    }

    elseif ($current_roles[$dataset_role_key] != $dataset_role_value) {

      drupal_set_message(
        t('The dataset contains a role with ID of %id and name of %name but the current role with that ID has a name of %current_role_name.',
          array(
            '%id' => $dataset_role_key,
            '%name' => $dataset_role_value,
            '%current_role_name' => $current_roles[$dataset_role_key],
          )
         ),
        'warning');
    }
  }

  // Here we will loop through the users in the receiving instance.
  // For each user we will look into the dataset file being imported.
  // If the user exists in the dataset file then we will check to see
  // if it matches and update it if it doesn't - if it does not exist
  // then we will delete it.
  $result = db_query("SELECT uid FROM {users}");

  foreach ($result as $user) {

    // Ignore the anonymous and default admin users.
    if ($user->uid == 0 || $user->uid == 1) {

      // Go to the start of the while loop.
      continue;
    }

    // See if a user with this ID exists in the dataset file.
    if (!isset($file_content_as_array['users'][$user->uid])) {

      // No user with this ID exists in the dataset file.  Therefore we will
      // delete the current user as we want the current user list to exactly
      // match the list in the dataset.  The current user could well be a test
      // user which was created during development. Here we will
      // delete the user account and delete all content owned by that
      // user.
      user_cancel(array(), $user->uid, 'user_cancel_delete');
    }

    else {

      // Else if it does see if it is the same and if not then update
      // the current user with the data from the dataset file.
      $current_user = user_load($user->uid);
      $user_from_dataset_file = $file_content_as_array['users'][$user->uid];

      // Here we are going to set the roles back to the basic value of
      // having the authenticated user role only.  This allows for the
      // receiving instance to have different roles and these may be
      // used for different purposes.
      $current_user->roles = array(DRUPAL_AUTHENTICATED_RID => 'authenticated user');
      $user_from_dataset_file->roles = array(DRUPAL_AUTHENTICATED_RID => 'authenticated user');

      if ($current_user != $user_from_dataset_file) {

        // Current user needs to be updated from the dataset file.
        user_save((object) array('uid' => $user_from_dataset_file->uid), (array) $user_from_dataset_file);

        // Here we will update the password to match what the was
        // originally output.
        $num_updated = db_update('users')
          ->fields(array(
              'pass' => $user_from_dataset_file->pass,
            ))
          ->condition('uid', $user_from_dataset_file->uid, '=')
          ->execute();
      }
    }
  }

  // Here we will loop through all of the users in the dataset file
  // and create any users which are in the dataset file but not in the
  // current users.
  foreach ($file_content_as_array['users'] as $dataset_file_user_value) {

    if (!user_load($dataset_file_user_value->uid)) {

      user_save(NULL, (array) $dataset_file_user_value);

      // Here we will update the password to match what the was
      // originally output.
      $num_updated = db_update('users')
        ->fields(array(
            'pass' => $dataset_file_user_value->pass,
          ))
        ->condition('uid', $dataset_file_user_value->uid, '=')
        ->execute();
    }
  }

  return TRUE;
}

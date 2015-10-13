<?php
/**
 * @file
 * Drush refactor project command.
 */

/**
 * Implements hook_drush_command().
 */
function refactor_project_drush_command() {
  $items = array();

  $items['refactor-project'] = array(
    'callback' => 'refactor_project_refactor',
    'description' => 'Refactor a project/module/theme by renaming its folders, files and function namespaces with a search/replace algorithm.',
    'arguments' => array(
      'oldname' => 'Namespace of project that must be renamed',
      'newname' => 'New namespace',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'options' => array(
      'oldname' => 'Namespace of project that must be renamed',
      'newname' => 'New namespace',
      'target-directory' => 'Directory relative to Drupal root where to place the renamed project.',
    ),
    'aliases' => array('rp'),
    'examples' => array(
      'drush refactor-project bootstrap_test bootstrap_final' => 'Renames project "bootstrap_test" to "bootstrap_final" and places the folder "bootstrap_final as sibling of "bootstrap_test".',
      'drush rp --oldname="project_to_refactor" --newname="project_refactored" --target-directory="sites/all/modules/custom"' => 'Renames project "project_to_refactor" to "project_refactored" and places the folder "project_refactored in "sites/all/modules/custom".',
    ),
  );

  return $items;
}

/**
 * Callback function.
 */
function refactor_project_refactor() {
  $args = func_get_args();

  // Extract function also prompts user for confirmation of variables.
  $variables = _refactor_project_refactor_variables_extract($args);


  // Copy the project to the new location.
  $oldname_path_full = $variables['drupal_root'] . "/" . $variables['oldname_path'];
  $newname_path_full = $variables['drupal_root'] . "/" . $variables['newname_path'];
  _refactor_project_refactor_copy_folder($oldname_path_full, $newname_path_full);


  // Rename all it's files and directories where $oldname acts as replacement
  // needle.
  $oldname = $variables['oldname'];
  $newname = $variables['newname'];
  _refactor_project_refactor_rename_files_and_folders_recursively($newname_path_full, $oldname, $newname);


  // Finished.
  drush_print("New project created in " . $newname_path_full);
  drush_print("Drush command finished successfully.");
}

/**
 * Helper that extract variables to work with.
 *
 * @return mixed
 *   - Array with variables to work with
 *   - Error if mandatory variables are missing.
 */
function _refactor_project_refactor_variables_extract($args) {
  $variables = array(
    'oldname' => '',
    'oldname_path' => '',
    'newname' => '',
    'newname_path' => '',
    'drupal_root' => DRUPAL_ROOT,
  );


  // Get oldname.
  $oldname = array_shift(drush_get_option_list('oldname'));
  if (empty($oldname)) {
    $oldname = array_shift($args);
  }
  if (empty($oldname)) {
    return drush_set_error(dt('You forgot to define the "oldname" of the project'));
  }
  $variables['oldname'] = $oldname;


  // Get oldname_path.
  $valid_types = array('module', 'theme', 'profile', 'theme_engine', 'feature');
  foreach ($valid_types as $valid_type) {
    if (drupal_get_path($valid_type, $oldname)) {
      $variables['oldname_path'] = drupal_get_path($valid_type, $oldname);
    }
  }
  if (!$variables['oldname_path']) {
    drush_set_error(dt('Drupal could not find location of "' . $oldname . '": command aborted'));
    exit;
  }


  // Test for newname.
  $newname = array_shift(drush_get_option_list('newname'));
  if (empty($newname)) {
    $newname = array_shift($args);
  }
  if (empty($newname)) {
    return drush_set_error(dt('You forgot to define the "newname" of the project'));
  }
  $variables['newname'] = $newname;


  // Get newname_path.
  $target_directory = array_shift(drush_get_option_list('target-directory'));
  if ($target_directory) {
    $variables['newname_path'] = $target_directory . "/" . $newname;
  }
  else {
    // Remove last part of directory and replace with newname.
    $variables['newname_path'] = substr($variables['oldname_path'], 0, -strlen($oldname)) . $newname;
  }


  // Print variables to screen and prompt user for confirmation of variables.
  drush_print('oldname      = ' . $variables['oldname']);
  drush_print('newname      = ' . $variables['newname']);
  drush_print('oldname_path = ' . $variables['oldname_path']);
  drush_print('newname_path = ' . $variables['newname_path']);
  drush_print('drupal_root  = ' . $variables['drupal_root'] . PHP_EOL);

  if (!drush_confirm('Please check above variables. Do you want to continue?')) {
    drush_user_abort();
    exit;
  }


  return $variables;
}

/**
 * Copy a folder to a new location recursively.
 *
 * @param string $oldname_path_full
 *   Full system path of original project folder.
 * @param string $newname_path_full
 *   Full system path of new project folder.
 */
function _refactor_project_refactor_copy_folder($oldname_path_full, $newname_path_full) {

  // Confirm replacement if the target directory already exists.
  if (is_dir($newname_path_full)) {
    if (!drush_confirm('Target location ' . $newname_path_full . ' already exists. Do you want to replace it?')) {
      drush_user_abort();
      exit;
    }
  }


  // Create new folder.
  $exec = "mkdir " . $newname_path_full;
  drush_shell_exec($exec);

  // Copy old folder to new folder.
  $exec = "cp -R " . $oldname_path_full . "/* " . $newname_path_full;
  drush_shell_exec($exec);

}


/**
 * Rename files and folder recursively.
 *
 * @param string $path
 *   Full system path of directory to start the replacement in. This path
 *   itself will not be replaced.
 * @param string $needle
 *   String that must be replaced inside file.
 * @param string $replacement
 *   String to replace needle with.
 */
function _refactor_project_refactor_rename_files_and_folders_recursively($path, $needle, $replacement) {
  $files_and_folders = scandir($path);

  // Check if $needle is found, if so rename file or folder.
  foreach ($files_and_folders as $file_or_folder) {
    $find = strpos($file_or_folder, $needle);
    if ($find === 0 || $find > 0) {
      $old_path = $path . "/" . $file_or_folder;
      $new_path = $path . "/" . str_replace($needle, $replacement, $file_or_folder);

      rename($old_path, $new_path);
    }
  }

  // If there are folders, repeat this process.
  $renamed_files_and_folders = scandir($path);


  foreach ($renamed_files_and_folders as $file_or_folder) {
    // Skip current directory.
    if ($file_or_folder == ".") {
    }
    // Skip parent directory.
    elseif ($file_or_folder == "..") {
    }
    else {
      $deep_path = $path . "/" . $file_or_folder;
      // Check if Dir.
      if (is_dir($deep_path)) {
        // Call this function again.
        _refactor_project_refactor_rename_files_and_folders_recursively($deep_path, $needle, $replacement);
      }
      // If its a file, replace its content.
      else {
        _refactor_project_refactor_replace_file_content($deep_path, $needle, $replacement);
      }
    }
  }
}

/**
 * Replace content within a file.
 *
 * @param string $file
 *   Full system path of file.
 * @param string $needle
 *   String that must be replaced inside file.
 * @param string $replacement
 *   String to replace needle with.
 *
 * @throws Exception
 *   If passed in file is a directory.
 */
function _refactor_project_refactor_replace_file_content($file, $needle, $replacement) {
  // Abort if the file is a directory.
  if (is_dir($file)) {
    throw new Exception("Could not replace inside file " . $file . " :is a directory.");
  }

  // Replace needle with replacement inside file.
  file_put_contents($file, str_replace($needle, $replacement, file_get_contents($file)));
}

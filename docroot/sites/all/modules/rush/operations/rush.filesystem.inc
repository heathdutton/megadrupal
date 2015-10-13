<?php
/**
 * @file
 * Drush Rush filesystem operation functions.
 * Functions for file and directory operation management.
 * @see operations
 */

/**
 * @addtogroup operations Operations
 * @{
 */

/**
 * @addtogroup filesystem-operations File System Operations
 * @brief File and directory operation management functions for Drush Rush.
 * @details Includes functions to:
 * - Create, backup, and remove fies and directories
 * - Add git ignore entries to a git ignore file
 * @{
 */

/**
 * Create directories within the specified directory.
 *
 * @backlog Write tests.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['directory_path'] string
 *     Required. A string containing the path to the build's directory.
 *   - $params['build']['directory_list'] array
 *     Required. An array list of directories to create.
 *
 * @return bool
 *   TRUE on success. FALSE on error.
 */
function rush_op_create_directories($params) {

  if (empty($params['build']['directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'directory_path\']', __FUNCTION__);
  }

  if (empty($params['build']['directory_list'])) {
    return rush_empty_error('$params[\'build\'][\'directory_list\']', __FUNCTION__);
  }

  foreach ($params['build']['directory_list'] as $directory) {
    $path = $params['build']['directory_path'] . $directory;
    drush_mkdir($path, TRUE);
    if (!is_dir($path)) {
      return drush_set_error('RUSH_FILESYSTEM_OP_ERROR', dt('[ERROR]: *** Unable to create directory !path ***', array('!path' => $path)));
    }
    else {
      $msg = dt('[CREATE DIRECTORY]: !path.', array('!path' => $path));
      drush_log($msg, 'info');
    }
  }

  $msg = dt('[CREATE DIRECTORIES]: In !path.', array('!path' => $params['build']['directory_path']));
  drush_log($msg, 'success');

  return TRUE;
}

/**
 * Create the build directory at the specified path.
 *
 * @backlog Write tests.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['directory_path'] string
 *     Required. A string containing the path to the build's directory.
 *   - $params['build']['backups_directory_path'] string
 *     Required. A string containing the path to the backups directory.
 *   - $params['build']['key'] string
 *     Required. A string containing a unique job id.
 *   - $params['build']['directory_name'] string
 *     Required. A string containing the build directory name. 
 *
 * @return bool
 *   TRUE on success. FALSE on error.
 */
function rush_op_create_build_directory($params) {

  if (empty($params['build']['directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'directory_path\']', __FUNCTION__);
  }

  if (empty($params['build']['backups_directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'backups_directory_path\']', __FUNCTION__);
  }

  if (empty($params['build']['key'])) {
    return rush_empty_error('$params[\'build\'][\'key\']', __FUNCTION__);
  }

  if (empty($params['build']['directory_name'])) {
    return rush_empty_error('$params[\'build\'][\'directory_name\']', __FUNCTION__);
  }

  $build_directory_path = $params['build']['directory_path'];
  // If no previous build directory exists, create one.
  if (!is_dir($build_directory_path)) {
    drush_mkdir($build_directory_path, TRUE);
    // Pause for a second to get directory made before other operations need it.
    sleep(1);
  }
  else {
    // A previous directory does exist, alert the user.
    $msg = dt('[NOTICE]: *** Directory !path already exists. Consider adding the operation `destroy_build_directory` to your rush file. ***', array(
      '!path' => $build_directory_path,
    ));
    drush_log($msg, 'notice');

    // Strip the directory forward slash off to prepare the rename action.
    $old_dir = rtrim($build_directory_path, '/');
    $new_dir = $params['build']['backups_directory_path'] . $params['build']['directory_name'] . '.' . date("Y-m-d-H-i-s") . '.' . $params['build']['key'];

    if (!is_dir($params['build']['backups_directory_path'])) {
      drush_mkdir($params['build']['backups_directory_path'], TRUE);
    }

    if (!rename($old_dir, $new_dir)) {
      // If the directory could not be successfully renamed then return error.
      return drush_set_error('RUSH_FILESYSTEM_OP_ERROR', dt('[ERROR]: *** Unable to rename directory !path ***', array('!path' => $build_directory_path)));
    }
    else {
      $msg = dt('[RENAME DIRECTORY]: !path to !new_dir.', array(
        '!path'    => $build_directory_path,
        '!new_dir' => $new_dir,
      ));
      drush_log($msg, 'info');

    }
    // Now create the directory.
    drush_mkdir($build_directory_path, TRUE);
    // Pause for a second to get directory made before other operations need it.
    sleep(1);
  }

  // Error checking.  Confirm the new directory exists.
  if (!is_dir($build_directory_path)) {
    return drush_set_error('RUSH_FILESYSTEM_OP_ERROR', dt('[ERROR]: *** Unable to create directory !path ***', array('!path' => $build_directory_path)));
  }
  else {
    $msg = dt('[CREATE DIRECTORY]: !path.', array('!path' => $build_directory_path));
    drush_log($msg, 'success');

    return TRUE;

  }
}

/**
 * Destroy the build directory at the specified path.
 *
 * @backlog Write tests.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['directory_path'] string
 *     Required. A string containing the path to the build's directory.
 *
 * @return bool
 *   TRUE on success. FALSE on error.
 * @backlog May have to refactor to make sure symlinks get deleted.
 */
function rush_op_destroy_build_directory($params) {

  if (empty($params['build']['directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'directory_path\']', __FUNCTION__);
  }
  $build_directory_path = $params['build']['directory_path'];
  if (!is_dir($build_directory_path)) {
    $msg = dt('[DELETE DIRECTORY]: !path does not exist. No need to delete it.', array('!path' => $build_directory_path));
    drush_log($msg, 'info');

    return TRUE;
  };

  if (drush_delete_dir($build_directory_path, TRUE, FALSE)) {
    $msg = dt('[DESTROY DIRECTORY]: !path', array('!path' => $build_directory_path));
    drush_log($msg, 'success');
  }
  else {
    return drush_set_error('RUSH_FILESYSTEM_OP_ERROR', dt('[ERROR]: *** Unable to destroy !dir ***', array('!dir' => $build_directory_path)));
  }

  return TRUE;
}

/**
 * Create files within the specified directory.
 *
 * @backlog Write tests.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['directory_path'] string
 *     Required. A string containing the path to the build's directory.
 *   - $params['build']['file_list'] array
 *     Required. An array containing a list of files to create.
 *   - $params['build']['file_list_data'] string
 *     Required. A string to write into the files when created.
 *
 * @return bool
 *   TRUE on success.  FALSE on error.
 */
function rush_op_create_files($params) {

  if (empty($params['build']['directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'directory_path\']', __FUNCTION__);
  }

  if (empty($params['build']['file_list'])) {
    return rush_empty_error('$params[\'build\'][\'file_list\']', __FUNCTION__);
  }

  if (empty($params['build']['file_list_data'])) {
    return rush_empty_error('$params[\'build\'][\'file_list_data\']', __FUNCTION__);
  }

  foreach ($params['build']['file_list'] as $file) {
    $file = $params['build']['directory_path'] . $file;
    if (drush_file_append_data($file, $params['build']['file_list_data'])) {
      $msg = dt('[CREATE FILE]: !file', array('!file' => $file));
      drush_log($msg, 'info');
    }
    else {
      return drush_set_error('RUSH_FILESYSTEM_OP_ERROR', dt('[ERROR]: *** Unable to create file !file ***', array('!file' => $file)));
    }
  }
  $msg = dt('[CREATE FILES]: In !dir', array('!dir' => $params['build']['directory_path']));
  drush_log($msg, 'success');

  return TRUE;
}

/**
 * Create a .gitignore file within the specified directory.
 *
 * @backlog Write tests.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['directory_path'] string
 *     Required. A string containing the path to the build's directory.
 *   - $params['build']['gitignore_list'] array
 *     An array containing a list of git ignore patterns.
 *
 * @return bool
 *   TRUE on success.  FALSE on error.
 */
function rush_op_create_git_ignore($params) {

  if (empty($params['build']['directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'directory_path\']', __FUNCTION__);
  }

  if (empty($params['build']['gitignore_list'])) {
    return rush_empty_error('$params[\'build\'][\'gitignore_list\']', __FUNCTION__);
  }

  $data = implode("\n", $params['build']['gitignore_list']);
  // Prefix the data with a new line in case the file exists with previous
  // entries.
  $data = "\n" . $data;
  $file = $params['build']['directory_path'] . '.gitignore';
  if (drush_file_append_data($file, $data)) {
    $msg = dt('[CREATE GIT IGNORE]: !file', array('!file' => $file));
    drush_log($msg, 'success');
  }
  else {
    return drush_set_error('RUSH_FILESYSTEM_OP_ERROR', dt('[ERROR]: *** Unable to create file !file ***', array('!file' => $file)));
  }

  return TRUE;
}

/** @}*/
/** @}*/

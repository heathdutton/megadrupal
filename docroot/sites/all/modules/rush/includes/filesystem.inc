<?php
/**
 * @file
 * Functions to get, set, and manage file and directory locations.
 */

/**
 * @addtogroup helpers Helper Functions
 * @{
 */

/**
 * @addtogroup filesystem File System Helpers
 * @brief Functions to get, set, and manage file and directory locations.
 * @{
 */

/**
 * Function to confirm important directories exists on the system.
 *
 * This function is currently not in use.
 *
 * @return bool
 *   Returns TRUE on success or FALSE on fail (via drush_set_error).
 */
function rush_check_directories() {

  $global_locations = rush_get_global_locations();
  if (!is_dir($global_locations['RUSH_HOME_DIRECTORY_PATH'])) {
    return drush_set_error('DRUSH_RUSH_NO_DIRECTORY', dt('[ERROR]: *** Cannot find the rush home directory !path ***', array('!path' => $global_locations['RUSH_HOME_DIRECTORY_PATH'])));
  }
  if (!is_dir($global_locations['RUSH_ENVIRONMENT_DIRECTORY_PATH'])) {
    return drush_set_error('DRUSH_RUSH_NO_DIRECTORY', dt('[ERROR]: *** Cannot find the rush global directory !path ***', array('!path' => $global_locations['RUSH_ENVIRONMENT_DIRECTORY_PATH'])));
  }
  if (!is_dir($global_locations['RUSH_JOBS_DIRECTORY_PATH'])) {
    return drush_set_error('DRUSH_RUSH_NO_DIRECTORY', dt('[ERROR]: *** Cannot find the rush jobs directory !path ***', array('!path' => $global_locations['RUSH_JOBS_DIRECTORY_PATH'])));
  }
  if (!is_dir($global_locations['RUSH_JOB_DIRECTORY_PATH'])) {
    return drush_set_error('DRUSH_RUSH_NO_DIRECTORY', dt('[ERROR]: *** Cannot find the rush job directory !path ***', array('!path' => $global_locations['RUSH_JOB_DIRECTORY_PATH'])));
  }

  return TRUE;
}

/**
 * Extract Rush application folder locations.
 *
 * The following locations are derived in this function:
 *   1.  The Rush "application" folder.
 *     - The Rush folder should be in the user's home directory and should be
 *       named "Rush".
 *     - Uses drush_server_home()  to find the home directory.
 *     - $global_locations['_RUSH_HOME_DIRECTORY_PATH']
 *   2.  The global environment directory within the Rush application folder.
 *     - This is where global (system environment) params should be placed.
 *     - Using global params allows job folders to inherit system level
 *       information without the need to define the information more than once.
 *     - $global_locations['RUSH_ENVIRONMENT_DIRECTORY_PATH_']
 *   3.  The jobs directory within the Rush application folder.
 *     - This is where single job folders may be placed.
 *     - $global_locations['_RUSH_JOBS_DIRECTORY_PATH_']
 *   4.  The specific job folder defined in the job argument at run time.
 *     - The params and rush files in here define build specific parameters and
 *       operations.
 *     - `drush rush jobOne` will run the params and operations included in
 *        the directory ~/Rush/jobs/jobOne/ .
 *     -  $global_locations['_RUSH_JOB_DIRECTORY_PATH_']
 *
 * @see rush_drush_command
 * @see drush_rush_rush
 * @return array
 *   An associative array with the path locations.
 */
function rush_get_global_locations() {

  $global_locations['RUSH_HOME_DIRECTORY_NAME'] = 'Rush';
  $global_locations['RUSH_MODULE_DIRECTORY_PATH'] = rush_self_path();
  $global_locations['RUSH_ENVIRONMENT_DIRECTORY_NAME'] = 'environment';
  $global_locations['RUSH_JOBS_DIRECTORY_NAME'] = 'jobs';
  $global_locations['RUSH_JOB_TEMPLATES_DIRECTORY_NAME'] = 'RushTemplates';
  $global_locations['RUSH_SERVER_HOME'] = drush_server_home() . '/';
  $global_locations['RUSH_HOME_DIRECTORY_PATH'] = $global_locations['RUSH_SERVER_HOME'] . $global_locations['RUSH_HOME_DIRECTORY_NAME'] . '/';
  $global_locations['RUSH_ENVIRONMENT_DIRECTORY_PATH'] = $global_locations['RUSH_HOME_DIRECTORY_PATH'] . $global_locations['RUSH_ENVIRONMENT_DIRECTORY_NAME'] . '/';
  $global_locations['RUSH_JOBS_DIRECTORY_PATH'] = $global_locations['RUSH_HOME_DIRECTORY_PATH'] . $global_locations['RUSH_JOBS_DIRECTORY_NAME'] . '/';
  $global_locations['RUSH_JOB_TEMPLATES_DIRECTORY_PATH'] = $global_locations['RUSH_MODULE_DIRECTORY_PATH'] . $global_locations['RUSH_JOB_TEMPLATES_DIRECTORY_NAME'] . '/';
  $args = drush_get_arguments();
  $job_arg = $args[1];
  $job = str_replace('.', "/", $job_arg);
  $global_locations['RUSH_JOB_DIRECTORY_PATH'] = $global_locations['RUSH_JOBS_DIRECTORY_PATH'] . $job . '/';
  // Check whether the job directory exists.
  if (!is_dir($global_locations['RUSH_JOB_DIRECTORY_PATH'])) {
    // Don't set an error at this time.
    // drush_rush_validate() will handle error checking.
    // Instead switch to the job_templates directory.
    // This is mainly a convenience for Job Template developers to keep template
    // jobs separate from their own jobs and for people who want to update
    // template jobs with a simple replacement of the previous directory.
    $global_locations['RUSH_JOBS_DIRECTORY_PATH'] = $global_locations['RUSH_JOB_TEMPLATES_DIRECTORY_PATH'];
    $global_locations['RUSH_JOB_DIRECTORY_PATH'] = $global_locations['RUSH_JOBS_DIRECTORY_PATH'] . $job . '/';
  }
  // Check whether this is a sub job and if so, identify the top job directory.
  $job_directories = explode('.', $job_arg);
  if (count($job_directories) > 1) {
    $global_locations['RUSH_JOB_TOP_DIRECTORY_PATH'] = $global_locations['RUSH_JOBS_DIRECTORY_PATH'] . $job_directories[0] . '/';
  }

  return $global_locations;
}

/**
 * Return an array of files and directories in a given directory path.
 *
 * @param string $root
 *   Required. The root directory path to start running through.
 *
 * @return array
 *   An multi-dimensional array of files and directories.
 *   `$files = $files['files']` and `$directories = $files['dirs']`
 */
function _rush_inspect_directory($root) {

  $files = array('files' => array(), 'dirs' => array());
  $directories = array();
  $last_letter = $root[strlen($root) - 1];
  $root = ($last_letter == '\\' || $last_letter == '/') ? $root : $root . DIRECTORY_SEPARATOR;

  $directories[] = $root;

  while (count($directories)) {
    $dir = array_pop($directories);
    $handle = opendir($dir);
    if ($handle) {
      while (FALSE !== ($file = readdir($handle))) {
        if ($file == '.' || $file == '..' || $file == '.git') {
          continue;
        }
        $file = $dir . $file;
        if (is_dir($file)) {
          $directory_path = $file . DIRECTORY_SEPARATOR;
          array_push($directories, $directory_path);
          $files['dirs'][] = $directory_path;
        }
        elseif (is_file($file)) {
          $files['files'][] = $file;
        }
      }
      closedir($handle);
    }
  }

  return $files;
}


/**
 * Return a list of Rush jobs.
 *
 * @param string $root
 *   Required. The root directory path to start running through.
 * @param string $directory_name
 *   Required.  The name of the jobs directory.
 * @param string $rush_dir_name
 *   Required.  The Rush home directory, usually `~/Rush`.
 *
 * @return array
 *   An array of Rush jobs.
 */
function _rush_list_jobs($root, $directory_name, $rush_dir_name) {

  $files = _rush_inspect_directory($root);
  // Turn the directories list into a list of jobs.
  $jobs_list = $files['dirs'];
  // Sort the directory names by alpha order.
  natcasesort($jobs_list);
  $jobs = array();
  foreach ($jobs_list as $job) {
    $job = explode("/$rush_dir_name/$directory_name/", $job);
    $job = trim($job[1], '/');
    $job = str_replace('/', '.', $job);
    $jobs[] = $job;
  }

  return $jobs;
}

/**
 * Return a make file location if it can be found.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 * @param string $make_file
 *   A string containing the name of the make file.
 *   Defaults to: 'default.make'.
 *
 * @return bool|string
 *   FALSE if no file is found.
 *   A string containing the path to the make file if found.
 */
function _rush_derive_make_file_path($params, $make_file = 'default.make') {

  // Look for the make file in the build directory.
  // This first directory will not exist if it is a fresh build.  However, this
  // function will be called again when drush make is called, so a make file in
  // a build directory can be used.
  $make_file_path = $params['build']['directory_path'] . $make_file;
  if (file_exists($make_file_path)) {
    return $make_file_path;
  }

  // Look for the make file in the job directory.
  $make_file_path = $params['global_locations']['RUSH_JOB_DIRECTORY_PATH'] . $make_file;
  if (file_exists($make_file_path)) {
    return $make_file_path;
  }
  // Look for the make file in the top job directory.
  $make_file_path = $params['global_locations']['RUSH_JOB_TOP_DIRECTORY_PATH'] . $make_file;
  if (file_exists($make_file_path)) {
    return $make_file_path;
  }
  // Look for the make file in the Rush global directory.
  $make_file_path = $params['global_locations']['RUSH_ENVIRONMENT_DIRECTORY_PATH'] . $make_file;
  if (file_exists($make_file_path)) {
    return $make_file_path;
  }

  return FALSE;
}

/** @}*/
/** @}*/

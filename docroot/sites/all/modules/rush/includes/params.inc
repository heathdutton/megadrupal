<?php
/**
 * @file
 * Functions to parse and derive Drush Rush Job and environment parameters.
 */

/**
 * @addtogroup helpers Helper Functions
 * @{
 */

/**
 * @addtogroup get_params Parameters Helpers
 * @brief Functions to parse and derive Rush Job and environment parameters.
 * @{
 */

/**
 * Parse rush parameter files and return an array of parameters.
 *
 * The following is the order of parsing:
 *   1. Get params from the global params ini file if it exists.
 *   2. Get params from the global params php file if it exists.
 *     - Params in the php file will override the ini file.
 *   3. Get params from the job params ini file if it exists.
 *   4. Merge global params with job params, letting job params override global.
 *   5. Get params from the job params php file if it exists.
 *     - Params in the job php file will override all params to this point.
 *
 * @see doc-parameters-list
 * @return array
 *   An associative array of all rush parameters.
 */
function rush_get_params() {

  $global_locations = rush_get_global_locations();
  $params           = array();
  // IMPORT GLOBAL PARAMETERS
  // Get params from the global params ini file if it exists.
  $params_ini_file = $global_locations['RUSH_ENVIRONMENT_DIRECTORY_PATH'] . 'params.ini';
  if (file_exists($params_ini_file)) {
    $msg = $msg = dt('[LOAD PARAMS FILE]: !file.', array('!file' => $params_ini_file));
    drush_log($msg, 'notice');
    $params = rush_parse_ini($params_ini_file);
  }
  // Get params from the global params php file if it exists.
  // Params in the php file will override the ini file.
  $params_php_file = $global_locations['RUSH_ENVIRONMENT_DIRECTORY_PATH'] . 'params.php';
  if (file_exists($params_php_file)) {
    $msg = $msg = dt('[LOAD PARAMS FILE]: !file.', array('!file' => $params_php_file));
    drush_log($msg, 'notice');

    include_once "$params_php_file";
  }
  unset($params_ini_file, $params_php_file);
  // IMPORT TOP JOB PARAMETERS IF THERE IS A TOP JOB
  // If the job called is a sub job, then pull parameters from the top
  // job first.
  if (!empty($global_locations['RUSH_JOB_TOP_DIRECTORY_PATH'])) {
    // Get params from the top job params ini file if it exists.
    $params_ini_file = $global_locations['RUSH_JOB_TOP_DIRECTORY_PATH'] . 'params.ini';
    if (file_exists($params_ini_file)) {
      $msg = $msg = dt('[LOAD PARAMS FILE]: !file.', array('!file' => $params_ini_file));
      drush_log($msg, 'notice');
      $top_job_params = rush_parse_ini($params_ini_file);
      if (!empty($top_job_params)) {
        // Merge top job params with params set to this point, letting top job
        // params override.
        $params = _rush_array_merge_recursive_distinct($params, $top_job_params);
      }
    }
    // Get params from the top job params php file if it exists.
    // Params in the php file will override all params to this point.
    $params_php_file = $global_locations['RUSH_JOB_TOP_DIRECTORY_PATH'] . 'params.php';
    if (file_exists($params_php_file)) {
      $msg = $msg = dt('[LOAD PARAMS FILE]: !file.', array('!file' => $params_php_file));
      drush_log($msg, 'notice');

      include_once "$params_php_file";
    }
  }
  unset($params_ini_file, $params_php_file);
  // IMPORT JOB PARAMETERS
  // Get params from the job params ini file if it exists.
  $params_ini_file = $global_locations['RUSH_JOB_DIRECTORY_PATH'] . 'params.ini';
  if (file_exists($params_ini_file)) {
    $msg = $msg = dt('[LOAD PARAMS FILE]: !file.', array('!file' => $params_ini_file));
    drush_log($msg, 'notice');
    $job_params = rush_parse_ini($params_ini_file);
    // Merge job params with params set to this point, letting job params
    // override.
    if (!empty($job_params)) {
      $params = _rush_array_merge_recursive_distinct($params, $job_params);
    }
  }
  // Get params from the job params php file if it exists.
  // Params in the php file will override all params to this point.
  $params_php_file = $global_locations['RUSH_JOB_DIRECTORY_PATH'] . 'params.php';
  if (file_exists($params_php_file)) {
    $msg = $msg = dt('[LOAD PARAMS FILE]: !file.', array('!file' => $params_php_file));
    drush_log($msg, 'notice');

    include_once "$params_php_file";
  }
  $command_line = array();
  // Add in options passed on the command line.
  if (drush_get_option('domain', FALSE)) {
    $command_line['domain'] = $params['build']['domain'] = drush_get_option('domain');
  }
  if (drush_get_option('branch', FALSE)) {
    $command_line['branch'] = $params['repo']['branch'] = drush_get_option('branch');
  }
  if (drush_get_option('new-branch', FALSE)) {
    $command_line['new-branch'] = $params['repo']['new_branch'] = drush_get_option('new-branch');
  }
  if (drush_get_option('create-branch', FALSE)) {
    $command_line['create-branch'] = $params['repo']['create_branch'] = drush_get_option('create-branch');
  }

  $params['command_line_params'] = $command_line;
  // PREP AND RETURN PARAMS
  // Add global locations to the params array to inspect in the --show=params
  // output.
  // @backlog Change to allow job name override.
  $params['global_locations'] = $global_locations;
  $args                       = drush_get_arguments();
  $params['build']['job']     = $args[1];
  $params['build']['key']     = substr(str_shuffle(md5(microtime())), 0, 5);
  _rush_derive_parameters($params);

  return $params;
}

/**
 * Parse a params.ini file.
 *
 * Wrapper function for parse_ini_file.
 *   - http://www.php.net/manual/en/function.parse-ini-file.php
 *
 * @param string $ini_file
 *   Required. A string with the path of the ini file to parse.
 *
 * @return array
 *   An associative array of parameters on success.  False on fail.
 */
function rush_parse_ini($ini_file) {

  return parse_ini_file($ini_file, TRUE);
}

/**
 * Derive missing parameters.
 *
 * To see a list of all derived params add --show=derived to the rush command.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *
 * @backlog Make some of these detections smarter by system type &
 * directory tests.
 * @backlog Add inline comments when needed.
 */
function _rush_derive_parameters(&$params) {

  // Set up an array to track derived params to output during --show-derived.
  $derived = array();
  // Derive parameters for each param type.
  // Use helper function just to break up the code.
  _rush_derive_parameters_env($params, $derived);
  _rush_derive_parameters_build($params, $derived);
  _rush_derive_parameters_db($params, $derived);
  _rush_derive_parameters_repo($params, $derived);
  _rush_derive_parameters_si($params, $derived);

  // Add the derived params to the params array.
  $params['derived_params'] = $derived;
}

/**
 * Derive missing build parameters.
 *
 * To see a list of all derived params add --show=derived to the rush command.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 * @param array $derived
 *   An array to track derived params to output during --show-derived.
 *
 * @backlog Make some of these detections smarter by system type &
 * directory tests.
 * @backlog Add inline comments when needed.
 * @backlog Create a makefile name param to change just the name but not path.
 */
function _rush_derive_parameters_build(&$params, &$derived) {

  // To avoid requiring a domain in each build ini file, use the
  // build job folder name by default plus the env tld.
  if (empty($params['build']['domain'])) {
    $args          = drush_get_arguments();
    $job_full_name = $args[1];

    $job_name                   = $job_full_name;
    $job_name                   = str_replace(' ', '_', $job_name);
    $domain                     = $job_name . $params['env']['tld'];
    $derived['build']['domain'] = $params['build']['domain'] = strtolower($domain);
  }

  if (empty($params['build']['machine_name'])) {
    $machine_name                     = str_replace('.', '_', $params['build']['domain']);
    $derived['build']['machine_name'] = $params['build']['machine_name'] = $machine_name;
  }

  if (empty($params['build']['site_uri'])) {
    $derived['build']['site_uri'] = $params['build']['site_uri'] = 'http://' . $params['build']['domain'];
  }
  // MAMP default port is not set to 80.
  // Any host port not set to 80 will have the port added to the site url.
  if ($params['env']['host_port'] != '80') {
    $derived['build']['site_uri'] = $params['build']['site_uri'] = $params['build']['site_uri'] . ':' . $params['env']['host_port'];
  }

  // The build_root_directory_name is usually the name of webroot directory on
  // the server. Example settings include: 'www', 'websites', and 'Sites'.
  // If the directory name is not set in the global.ini file then set it to
  // 'Sites' as a default; which is kind of OSX-centric.
  if (empty($params['build']['root_directory_name'])) {
    $derived['build']['root_directory_name'] = $params['build']['root_directory_name'] = 'Sites';
  }
  // The build_root_directory_path is usually the path to the webroot directory
  // on the server. Usually it's something like '~/webroot'.
  // Note, it does not have to be.  A particular Drush Rush job may be just to
  // download a bunch of code libraries to a user directory for the use of code
  // completion via an IDE include libraries option. With special cases, the
  // build_root_directory_path can be changed in a job ini file.
  if (empty($params['build']['root_directory_path'])) {
    $derived['build']['root_directory_path'] = $params['build']['root_directory_path'] = $params['global_locations']['RUSH_SERVER_HOME'] . $params['build']['root_directory_name'] . '/';
  }

  if (empty($params['build']['backups_directory_name'])) {
    $derived['build']['backups_directory_name'] = $params['build']['backups_directory_name'] = 'backups';
  }

  if (empty($params['build']['backups_directory_path'])) {
    $derived['build']['backups_directory_path'] = $params['build']['backups_directory_path'] = $derived['build']['root_directory_path'] . $params['build']['backups_directory_name'] . '/';
  }

  // If there is no build logs path, then try to find one in the root
  // directory path.
  if (empty($params['build']['logs_path'])) {
    $derived['build']['logs_path'] = $params['build']['logs_path'] = $derived['build']['root_directory_path'] . 'logs/';
    if (!is_dir($derived['build']['logs_path'])) {
      // If the directory does not exist, then unset the param.
      unset($derived['build']['logs_path']);
      unset($params['build']['logs_path']);
    }
  }

  // The build_directory_name is automatically derived based on the
  // build_machine_name.  However, setting this value in a job params file will
  // override the name. This is useful if you want a sub job to use the same
  // path name as the parent job.
  if (empty($params['build']['directory_name'])) {
    $derived['build']['directory_name'] = $params['build']['directory_name'] = $params['build']['machine_name'];
  }

  // The build_directory_path is the sum of the build_root_directory_path and
  // the build_directory_name.  It's automatically derived.  However, setting
  // this value in a job params file will override the path.  In practice, this
  // would be the only value needed to be overwritten, rather than the
  // build_root_directory_path and the build_machine_name.
  if (empty($params['build']['directory_path'])) {
    $derived['build']['directory_path'] = $params['build']['directory_path'] = $params['build']['root_directory_path'] . $params['build']['directory_name'] . '/';
  }
  // A Drupal project stored in version control may have it's docroot
  // (Drupal codebase) in a sub-directory of the overall project. Drush Rush
  // must know where the core Drupal code is.  Example values include: 'htdocs',
  // 'www', and 'docroot'.  Drush Rush defaults to 'docroot'.
  if (empty($params['build']['docroot_name'])) {
    $derived['build']['docroot_name'] = $params['build']['docroot_name'] = 'docroot';
  }
  // This is the docroot_path used by Drush and Apache Vhosts.
  if (empty($params['build']['docroot_path'])) {
    $derived['build']['docroot_path'] = $params['build']['docroot_path'] = $params['build']['directory_path'] . $params['build']['docroot_name'] . '/';
  }
  // Build code_directory_path path is typically an alias for the docroot_path.
  // However, it can be changed if the codebase is located somewhere else.
  if (empty($params['build']['code_directory_path'])) {
    $derived['build']['code_directory_path'] = $params['build']['code_directory_path'] = $params['build']['docroot_path'];
  }
  // The name of the Drupal site folder, inside the sites directory.
  if (empty($params['build']['site_folder_name'])) {
    $derived['build']['site_folder_name'] = $params['build']['site_folder_name'] = 'default';
  }
  // Site specification used by Drush.
  if (empty($params['build']['site_specification'])) {
    $derived['build']['site_specification'] = $params['build']['site_specification'] = $params['build']['code_directory_path'] . '?uri=' . $params['build']['site_uri'] . '#' . $params['build']['site_folder_name'];
  }
  // Site alias record used by Drush.
  if (empty($params['build']['site_alias_record'])) {
    $derived['build']['site_alias_record'] = $params['build']['site_alias_record'] = drush_sitealias_get_record($params['build']['site_specification']);
  }
  // Site profile used by Drush Site Install.
  if (empty($params['build']['site_profile'])) {
    $derived['build']['site_profile'] = $params['build']['site_profile'] = 'standard';
  }

  // Make file name.
  if (empty($params['build']['make_file'])) {
    $derived['build']['make_file'] = $params['build']['make_file'] = 'default.make';
  }

  // There may not always be a make file.
  // This will auto set a make file if it can find one.
  if (empty($params['build']['make_file_path'])) {
    $make_file_path = _rush_derive_make_file_path($params, $params['build']['make_file']);
    if ($make_file_path != FALSE) {
      $derived['build']['make_file_path'] = $params['build']['make_file_path'] = $make_file_path;
    }
  }
  // This is not an important parameter, so it's derived by default.
  // It's used to drop place-holder text into a file when auto-created by
  // Drush Rush.  In keeping with allowing lot's of configuration options, this
  // setting can also be changed in a global params file or build params file.
  if (empty($params['build']['file_list_data'])) {
    $derived['build']['file_list_data'] = $params['build']['file_list_data'] = 'File added by Drush Rush.';
  }
}

/**
 * Derive missing db parameters.
 *
 * To see a list of all derived params add --show=derived to the rush command.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 * @param array $derived
 *   An array to track derived params to output during --show-derived.
 *
 * @backlog Make some of these detections smarter by system type &
 * directory tests.
 * @backlog Add inline comments when needed.
 */
function _rush_derive_parameters_db(&$params, &$derived) {

  // Default to the db type of mysql.
  if (empty($params['db']['type'])) {
    $derived['db']['type'] = $params['db']['type'] = 'mysql';
  }
  // Default to the server of localhost.
  if (empty($params['db']['server'])) {
    $derived['db']['server'] = $params['db']['server'] = 'localhost';
  }
  // Default to the server of 3306.
  // Note: MAMP runs mysql by default on port: 8889.
  if (empty($params['db']['port'])) {
    $derived['db']['port'] = $params['db']['port'] = '3306';
  }
  // Default to root.  Should be changed in params to a user with just the
  // appropriate amount of permissions and no more.
  if (empty($params['db']['user'])) {
    $derived['db']['user'] = $params['db']['user'] = 'root';
  }
  // Default to root. Rarely safe, unless behind an excellent firewall on a
  // secure box.  Change in params as needed.
  if (empty($params['db']['password'])) {
    $derived['db']['password'] = $params['db']['password'] = 'root';
  }
  // The db name will usually be set in the project params file.
  // If not, set it to the project machine name.
  if (empty($params['db']['name'])) {
    $derived['db']['name'] = $params['db']['name'] = $params['build']['machine_name'];
  }
  // The db directory name will usually be set in the project params file.
  // If not, set it 'sql'.
  // This is where db files will be stored. SQL dumps or sqlite files.
  // The directory is assumed to be within the ['build']['directory_path'] but
  // it doesn't have to be.
  if (empty($params['db']['directory_name'])) {
    $derived['db']['directory_name'] = $params['db']['directory_name'] = 'sql';
  }
  // Set the default location of the db directory path to be within the build
  // directory.  Change this if the job's db files are located somewhere else
  // on the system.
  if (empty($params['db']['directory_path'])) {
    $derived['db']['directory_path'] = $params['db']['directory_path'] = $params['build']['directory_path'] . $params['db']['directory_name'] . '/';
  }

  if (empty($params['db']['file_ext'])) {
    $derived['db']['file_ext'] = $params['db']['file_ext'] = 'sql';
  }

  if (empty($params['db']['file_name'])) {
    $derived['db']['file_name'] = $params['db']['file_name'] = $params['db']['name'] . '.' . $params['db']['file_ext'];
  }

  if (empty($params['db']['file_path'])) {
    $derived['db']['file_path'] = $params['db']['file_path'] = $params['db']['directory_path'] . $params['db']['file_name'];
  }

  // Build the db url based on the database info above.
  // Override at the project level.
  if (empty($params['db']['url'])) {
    // Extract variable names from the array to make the DB URLs easier to read.
    // extract($params['db']);
    // Extract scares code inspections, so fo now:
    $type      = $params['db']['type'];
    $user      = $params['db']['user'];
    $password  = $params['db']['password'];
    $port      = $params['db']['port'];
    $server    = $params['db']['server'];
    $name      = $params['db']['name'];
    $file_path = $params['db']['file_path'];
    switch ($type) {
      case 'mysql':
        $derived['db']['url'] = $params['db']['url'] = "$type://$user:$password@$server:$port/$name";
        break;

      case 'sqlite':
        $derived['db']['url'] = $params['db']['url'] = "$type://$file_path";
        break;
    }
  }
}

/**
 * Derive missing repo parameters.
 *
 * To see a list of all derived params add --show=derived to the rush command.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 * @param array $derived
 *   An array to track derived params to output during --show-derived.
 *
 * @backlog Make some of these detections smarter by system type &
 * directory tests.
 * @backlog Add inline comments when needed.
 */
function _rush_derive_parameters_repo(&$params, &$derived) {

  if (empty($params['repo']['service'])) {
    $derived['repo']['service'] = $params['repo']['service'] = 'github';
  }

  if (empty($params['repo']['api_url'])) {
    $derived['repo']['api_url'] = $params['repo']['api_url'] = 'https://api.github.com';
  }

  if (empty($params['repo']['api_account'])) {
    $derived['repo']['api_account'] = $params['repo']['api_account'] = '/user/';
  }

  if (empty($params['repo']['private'])) {
    $derived['repo']['private'] = $params['repo']['private'] = TRUE;
  }

  // Set the default repo name.
  if (empty($params['repo']['name'])) {
    $derived['repo']['name'] = $params['repo']['name'] = $params['build']['machine_name'];
  }

  if (empty($params['repo']['url'])) {
    if (!empty($params['repo']['url_prefix']) && !empty($params['repo']['url_suffix'])) {
      $derived['repo']['url'] = $params['repo']['url'] = $params['repo']['url_prefix'] . $params['repo']['name'] . $params['repo']['url_suffix'];
    }
  }

  if (empty($params['repo']['branch'])) {
    $derived['repo']['branch'] = $params['repo']['branch'] = 'master';
  }

  if (empty($params['repo']['remote'])) {
    $derived['repo']['remote'] = $params['repo']['remote'] = 'origin';
  }

  // Set the default repo commit message, which may be changed in param files.
  if (empty($params['repo']['commit_message'])) {
    $derived['repo']['commit_message'] = $params['repo']['commit_message'] = 'Automatic commit made by Drush Rush.';
  }
}

/**
 * Derive missing env parameters.
 *
 * To see a list of all derived params add --show=derived to the rush command.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 * @param array $derived
 *   An array to track derived params to output during --show-derived.
 *
 * @backlog Make some of these detections smarter by system type &
 * directory tests.
 * @backlog Add inline comments when needed.
 */
function _rush_derive_parameters_env(&$params, &$derived) {

  // Use .dev rather than .local as Mac Bonjour may interfere with .local.
  if (empty($params['env']['tld'])) {
    $derived['env']['tld'] = $params['env']['tld'] = '.dev';
  }
  if (empty($params['env']['hosts_file_path'])) {
    $derived['env']['hosts_file_path'] = $params['env']['hosts_file_path'] = '/etc/hosts';
  }
  if (empty($params['env']['vhosts_dir_path'])) {
    $derived['env']['vhosts_dir_path'] = $params['env']['vhosts_dir_path'] = '/etc/apache2/sites-available/';
  }
  if (empty($params['env']['host_ip'])) {
    $derived['env']['host_ip'] = $params['env']['host_ip'] = '127.0.0.1';
  }
  if (empty($params['env']['host_port'])) {
    $derived['env']['host_port'] = $params['env']['host_port'] = '80';
  }
}

/**
 * Set some default si parameters if they are not present in a params file.
 *
 * To see a list of all derived params add --show=derived to the rush command.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 * @param array $derived
 *   An array to track derived params to output during --show-derived.
 *
 * @backlog Make some of these detections smarter by system type &
 * directory tests.
 * @backlog Add inline comments when needed.
 */
function _rush_derive_parameters_si(&$params, &$derived) {

  if (empty($params['si']['account-name'])) {
    $derived['si']['account-name'] = $params['si']['account-name'] = 'admin';
  }
  if (empty($params['si']['account-pass'])) {
    $derived['si']['account-pass'] = $params['si']['account-pass'] = 'admin';
  }
  if (empty($params['si']['site-name'])) {
    $derived['si']['site-name'] = $params['si']['site-name'] = $params['build']['machine_name'];
  }
}

/**
 * An array merge function which does not change data types during merge.
 *
 * The array_merge_recursive function does merge arrays, but it converts values
 * with duplicate keys to arrays rather than overwriting the value in the first
 * array with the duplicate value in the second array, as array_merge does.
 * I.e., with array_merge_recursive, this happens (documented behavior):
 *
 * array_merge_recursive(array('key' => 'org value'),
 * array('key' => 'new value'));
 *     => array('key' => array('org value', 'new value'));
 *
 * _rush_array_merge_recursive_distinct does not change the datatypes of values
 * in the arrays. Matching keys' values in the second array overwrite those in
 * the first array, as is the case with array_merge, i.e.:
 *
 * _rush_array_merge_recursive_distinct(array('key' => 'org value'),
 * array('key' => 'new value'));
 *     => array('key' => array('new value'));
 *
 * Parameters are passed by reference, though only for performance reasons.
 * They're not
 * altered by this function.
 *
 * @param array $array1
 *   Array one of two to be merged.
 * @param array $array2
 *   Array two of two to be merged.
 *
 * @return array
 *   An array of merged arrays.
 */
function _rush_array_merge_recursive_distinct(array &$array1, array &$array2) {

  $merged = $array1;

  foreach ($array2 as $key => &$value) {
    if (is_array($value) && isset($merged[$key]) && is_array($merged[$key])) {
      $merged[$key] = _rush_array_merge_recursive_distinct($merged[$key], $value);
    }
    else {
      $merged[$key] = $value;
    }
  }

  return $merged;
}

/** @}*/
/** @}*/

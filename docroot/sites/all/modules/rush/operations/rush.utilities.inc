<?php
/**
 * @file
 * Drush Rush utility functions.
 * Functions for miscellaneous operations which may not fit into another group.
 * @see operations
 * @backlog Implement a hook help type system.
 */

/**
 * @addtogroup operations Operations
 * @{
 */
/**
 * @addtogroup utility-operations Utility Operations
 * @brief Miscellaneous operations which may not fit into another group yet.
 * Note: Utility Operations may be refactored or removed in the future.
 * @{
 */

/**
 * This is the virtual hosts template file for the apache functions.
 */
define('RUSH_SETTINGS_LOCAL_TEMPLATE', '<?php
$databases = array (
  \'default\' =>
  array (
    \'default\' =>
    array (
      \'driver\' => \'#TYPE#\',
      \'database\' => \'#NAME#\',
      \'username\' => \'#USER#\',
      \'password\' => \'#PASSWORD#\',
      \'host\' => \'#SERVER#\',
      \'port\' => \'#PORT#\',
    ),
  ),
);');

/**
 * Create a settings.local.php file.
 *
 * Some developers and teams will add an entry like this:
 *
 * @code
 * if (file_exists(dirname(__FILE__) . '/settings.local.php')) {
 *    require_once(dirname(__FILE__) .'/settings.local.php');
 * }
 * @endcode
 * to allow for local overrides if a file named settings.local.php exists.
 * The `settings.local.php` will then be excluded from version control.
 *
 * This operation creates a `settings.local.php` file and adds the db connection
 * information from the Rush user's environment or job parameters.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *
 * @return bool
 *   TRUE on success, FALSE on failure via drush_set_error().
 */
function rush_op_create_settings_local($params) {

  if (!_rush_settings_local_params($params)) {
    return drush_set_error('RUSH_FILESYSTEM_OP_ERROR', dt('[ERROR]: *** Unable to create file local settings file. ***'));
  }

  // Get template.
  $str_config = RUSH_SETTINGS_LOCAL_TEMPLATE;
  $str_config = str_replace("#TYPE#", $params['db']['type'], $str_config);
  $str_config = str_replace("#NAME#", $params['db']['name'], $str_config);
  $str_config = str_replace("#USER#", $params['db']['user'], $str_config);
  $str_config = str_replace("#PASSWORD#", $params['db']['password'], $str_config);
  $str_config = str_replace("#SERVER#", $params['db']['server'], $str_config);
  $str_config = str_replace("#PORT#", $params['db']['port'], $str_config);

  $settings_file = $params['build']['code_directory_path'] . 'sites/' . $params['build']['site_folder_name'] . '/settings.local.php';

  if (drush_file_append_data($settings_file, $str_config)) {
    // If execution has made it this far, then the operation is a success.
    $msg = dt('[WRITE SETTINGS.LOCAL]: !file ', array(
      '!file' => $settings_file,
    ));
    drush_log($msg, 'success');
  }
  else {
    return drush_set_error('RUSH_FILESYSTEM_OP_ERROR', dt('[ERROR]: *** Unable to create file !file ***', array('!file' => $settings_file)));
  }

  return TRUE;
}

/**
 * Function to verify required params exist for rush_op_create_settings_local().
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *
 * @return bool
 *   TRUE on success, FALSE on failure via drush_set_error().
 */
function _rush_settings_local_params($params) {

  // Yep.  That's a lot of if statements.
  if (empty($params['build']['code_directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'code_directory_path\']', __FUNCTION__);
  }
  if (empty($params['build']['site_folder_name'])) {
    return rush_empty_error('$params[\'build\'][\'site_folder_name\']', __FUNCTION__);
  }
  if (empty($params['db']['type'])) {
    return rush_empty_error('$params[\'db\'][\'type\']', __FUNCTION__);
  }
  if (empty($params['db']['name'])) {
    return rush_empty_error('$params[\'db\'][\'name\']', __FUNCTION__);
  }
  if (empty($params['db']['user'])) {
    return rush_empty_error('$params[\'db\'][\'user\']', __FUNCTION__);
  }
  if (empty($params['db']['password'])) {
    return rush_empty_error('$params[\'db\'][\'password\']', __FUNCTION__);
  }
  if (empty($params['db']['server'])) {
    return rush_empty_error('$params[\'db\'][\'server\']', __FUNCTION__);
  }
  if (empty($params['db']['port'])) {
    return rush_empty_error('$params[\'db\'][\'port\']', __FUNCTION__);
  }

  return TRUE;
}

/**
 * Used to test parameter inheritance.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 */
function rush_op_test_params($params) {

  drush_print_r($params['test']);
  drush_print($params['build']['machine_name']);

}

/** @}*/
/** @}*/

<?php
/**
 * @file
 * Drush Rush site functions.
 * @see operations
 * @backlog Provide method to handle existing databases and dbs.
 * Delete? Rename?
 */

/**
 * @addtogroup operations Operations
 * @{
 */
/**
 * @addtogroup site-operations Site Operations
 * @brief Site operation functions for Drush Rush.
 * @details Includes functions to:
 * - Run drush make
 * - Run drush site-install
 * - Derive and set site-install parameters
 * - Open a site in a browser
 * @{
 */

/**
 * Wrapper for `drush make`.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['code_directory_path'] string
 *   Required. A string containing the path to the build make file.
 *   - $params['build']['make_file_path'] string
 *   Required. A string containing the path to the build code directory path.
 *   For Drupal this is the docroot directory.
 *
 * @return bool
 *   TRUE on success.  FALSE on fail.
 */
function rush_op_make($params) {

  if (empty($params['build']['code_directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'code_directory_path\']', __FUNCTION__);
  }

  $params['build']['make_file_path'] = _rush_derive_make_file_path($params, $params['build']['make_file']);

  if (empty($params['build']['make_file_path'])) {
    return rush_empty_error('$params[\'build\'][\'make_file_path\']', __FUNCTION__);
  }

  $alias     = drush_sitealias_get_record('@self');
  $args      = array();
  $args[]    = $params['build']['make_file_path'];
  $args[]    = $params['build']['code_directory_path'];
  $options   = array();
  $options[] = '--prepare-install';
  $options[] = '-y';

  if (!empty($params['build']['contrib_destination'])) {
    $options[] = '--contrib-destination=' . $params['build']['contrib_destination'];
  }

  return rush_invoke($alias, 'make', $args, $options, __FUNCTION__);

}

/**
 * Wrapper for drush variable-set.
 *
 * This rush command takes an array of Drupal variable names and their values
 * and sets them with "drush variable-set"
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['site_alias_record'] array
 *   Required. A drush site alias record.
 *   - $params['drupal_variables'] array
 *   Required. A flat array with variable names as keys and their values as
 *   the array values. For example:
 *   $params['drupal_variables'] = array(
 *     'site_name' => 'My Great Blog Site',
 *     'clean_url' => '1',
 *   );
 *
 * @return bool
 *   TRUE on success.  FALSE with errors.
 */
function rush_op_set_drupal_variables($params) {

  if (empty($params['drupal_variables'])) {
    return rush_empty_error('$params[\'drupal_variables\']', __FUNCTION__);
  }

  if (empty($params['build']['site_alias_record'])) {
    return rush_empty_error('$params[\'build\'][\'site_alias_record\']', __FUNCTION__);
  }

  // These two variables will be passed to rush_invoke inside of the foreach
  // loop below. These two variables do not change based on the loop.
  $alias = $params['build']['site_alias_record'];
  // Use the exact flag because the parameters file should only use exact
  // variable names.
  $options = array(
    '--exact',
  );

  foreach ($params['drupal_variables'] as $variable_name => $variable_value) {
    // The variable name and value are passed to "drush variable-set"
    $args = array(
      $variable_name,
      $variable_value,
    );

    // If a variable-set attempt fails (rush_invoke returns TRUE or FALSE)
    // then return FALSE immediately, even if the foreach looping has not
    // reached it's last item.
    if (FALSE === rush_invoke($alias, 'variable-set', $args, $options, __FUNCTION__)) {
      return FALSE;
    }
  }

  // If the foreach loop was completed without any fails, return TRUE because
  // all variable-sets have succeeded.
  return TRUE;
}

/**
 * Wrapper for `drush site-install`.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['site_alias_record'] array
 *   Required. A drush site alias record.
 *   - $params['build']['site_profile'] string
 *   A string containing the name of a Drupal site profile.
 *   - $params['build']['code_directory_path'] string
 *   Required. A string containing the path to the build code directory path.
 *   For Drupal this is the docroot directory.
 *
 * @return bool
 *   TRUE on success.  FALSE with errors.
 */
function rush_op_si($params) {

  if (empty($params['build']['site_alias_record'])) {
    return rush_empty_error('$params[\'build\'][\'site_alias_record\']', __FUNCTION__);
  }

  if (empty($params['build']['site_profile'])) {
    return rush_empty_error('$params[\'build\'][\'site_profile\']', __FUNCTION__);
  }

  if (empty($params['build']['code_directory_path'])) {
    return rush_empty_error('$params[\'build\'][\'code_directory_path\']', __FUNCTION__);
  }

  $alias     = $params['build']['site_alias_record'];
  $args      = array();
  $args[]    = $params['build']['site_profile'];
  $options   = _rush_get_si_options($params);
  $options[] = '-y';

  return rush_invoke($alias, 'site-install', $args, $options, __FUNCTION__);
}

/**
 * Open the defined site URL in the default browser.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['site_uri'] string
 *   Required. A string containing the URI of the site to open.  For example:
 *   http://sitename.local
 *
 * @return bool
 *   TRUE on success. FALSE on fail.
 */
function rush_op_open_uri($params) {

  if (empty($params['build']['site_uri'])) {
    return rush_empty_error('$params[\'build\'][\'site_uri\']', __FUNCTION__);
  }
  $command = 'open ' . $params['build']['site_uri'];

  return rush_command_exec($command);

}

/**
 * Prepare options for rush_op_si(); a wrapper for `drush site-install`.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['docroot_path'] string
 *   Required.
 *   - $params['db']['url'] string
 *   Required.
 *
 * @backlog Refactor si params to be set in a for loop.
 *
 * @return array
 *   An associative array of site options.
 */
function _rush_get_si_options($params) {

  if (empty($params['build']['docroot_path'])) {
    return rush_empty_error('$params[\'build\'][\'docroot_path\']', __FUNCTION__);
  }

  if (empty($params['db']['url'])) {
    return rush_empty_error('$params[\'db\'][\'url\']', __FUNCTION__);
  }

  $opts   = array();
  $opts[] = "--root=" . $params['build']['docroot_path'];
  $opts[] = "--db-url=" . $params['db']['url'];

  // Optional parameters which may be set in param files.
  if (isset($params['si']['db-prefix'])) {
    $opts[] = "--db-prefix=" . $params['si']['db-prefix'];
  }
  if (isset($params['si']['account-name'])) {
    $opts[] = "--account-name=" . $params['si']['account-name'];
  }
  if (isset($params['si']['account-pass'])) {
    $opts[] = "--account-pass=" . $params['si']['account-pass'];
  }
  if (isset($params['si']['db-prefix'])) {
    $opts[] = "--db-prefix=" . $params['si']['db-prefix'];
  }
  if (isset($params['si']['account-mail'])) {
    $opts[] = "--account-mail=" . $params['si']['account-mail'];
  }
  if (isset($params['si']['locale'])) {
    $opts[] = "--locale=" . $params['si']['locale'];
  }
  if (isset($params['si']['clean-url'])) {
    $opts[] = "--clean-url=" . $params['si']['clean-url'];
  }
  if (isset($params['si']['site-name'])) {
    $opts[] = "--site-name=" . $params['si']['site-name'];
  }
  if (isset($params['si']['site-mail'])) {
    $opts[] = "--site-mail=" . $params['si']['site-mail'];
  }
  if (isset($params['si']['sites-subdir'])) {
    $opts[] = "--sites-subdir=" . $params['si']['sites-subdir'];
  }

  return $opts;
}
/** @}*/
/** @}*/

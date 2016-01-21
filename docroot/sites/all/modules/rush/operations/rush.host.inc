<?php
/**
 * @file
 * Host operation functions for Drush Rush.
 * @see operations
 */

/**
 * @addtogroup operations Operations
 * @{
 */
/**
 * @addtogroup host-operations Host Operations
 * @brief Host operation functions for Drush Rush.
 * @details Includes functions to:
 * - Create and remove dns entries
 * - Create and remove vhost files
 * - Restart Apache
 * @{
 */
/**
 * This is the virtual hosts template file for the apache functions.
 */
define('RUSH_APACHE_TEMPLATE', "
<VirtualHost *:#HOSTPORT#>
  ServerName #DOMAIN#
  ServerAlias *.#DOMAIN#
  DocumentRoot #DOCROOT#
  ErrorLog #ERRORLOG#
  <Directory #DOCROOT#>
    Options Indexes FollowSymLinks MultiViews
    AllowOverride All
    Order allow,deny
    allow from all
  </Directory>
</VirtualHost>");

/**
 * Create the DNS entry.
 *
 * Edit hosts file on local machine. Usually /etc/hosts.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['domain'] string
 *   Required. A string containing the site domain to add to the hosts file,
 *   such as 'mysite.dev'.
 *   - $params['env']['hosts_file_path'] string
 *   Required. A string containing the path to hosts file to edit, such
 *   as '/etc/hosts'.
 *   - $params['env']['host_ip'] string
 *   Required. A string containing the host ip address to add to the hosts file,
 *   such as '127.0.0.1'.
 *
 * @return bool
 *   TRUE on success. FALSE on fail (via drush_set_error).
 */
function rush_op_create_dns($params) {

  $dns_entry = '';
  $file      = '';
  // Check to make sure the domain is not already in the hosts file.
  if (!_rush_dns_exists($params, $dns_entry, $file)) {
    // The entry does not already exist so write it.
    $cmd = "echo '$dns_entry' | sudo tee -a " . $params['env']['hosts_file_path'];
  }
  // If the domain is already in the host file, make sure it matches $dns_entry.
  elseif (!strpos($file, $dns_entry)) {
    // Since it does not match, then we have an entry with the same domain but
    // possibly not with the same ip address.
    return drush_set_error('RUSH_HOST_OP_ERROR', dt('[ERROR]: *** A different entry for the !domain already exists in !hosts_file ***', array(
      '!domain'     => $params['build']['domain'],
      '!hosts_file' => $params['env']['hosts_file_path'],
    )));
  }
  else {
    $msg = dt('[CREATE DNS]: The dns entry for !domain in !hosts_file already existed. No changes made.', array(
      '!domain'     => $params['build']['domain'],
      '!hosts_file' => $params['env']['hosts_file_path'],
    ));
    drush_log($msg, 'info');

    return TRUE;
  }

  $msg = dt('[CREATE DNS]: Add !domain to !hosts_file', array(
    '!domain'     => $params['build']['domain'],
    '!hosts_file' => $params['env']['hosts_file_path'],
  ));

  return rush_shell_exec($cmd, $msg, 'RUSH_HOST_OP_ERROR', __FUNCTION__);

}

/**
 * Delete the DNS entry.
 *
 * Edit hosts file on local machine to remove the domain's dns entry.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['domain'] string
 *   Required. A string containing the site domain to add to the hosts file,
 *   such as 'mysite.dev'.
 *   - $params['env']['hosts_file_path'] string
 *   Required. A string containing the path to hosts file to edit, such
 *   as '/etc/hosts'.
 *   - $params['env']['host_ip'] string
 *   Required. A string containing the host ip address to add to the hosts file,
 *   such as '127.0.0.1'.
 *
 * @return bool
 *   TRUE on success, FALSE on failure via drush_set_error().
 */
function rush_op_delete_dns($params) {

  $dns_entry = '';
  $file      = '';
  if (!_rush_dns_exists($params, $dns_entry, $file)) {
    $msg = dt('[DELETE DNS]: The dns entry !dns_entry was not in !hosts_file', array(
      '!dns_entry'  => $dns_entry,
      '!hosts_file' => $params['env']['hosts_file_path'],
    ));
    drush_log($msg, 'info');

    return TRUE;
  }

  $cmd = "sudo sed -i.bak '/$dns_entry/d' " . $params['env']['hosts_file_path'];
  $msg = dt('[DELETE DNS]: Delete !domain from !hosts_file', array(
    '!domain'     => $params['build']['domain'],
    '!hosts_file' => $params['env']['hosts_file_path'],
  ));

  return rush_shell_exec($cmd, $msg, 'RUSH_HOST_OP_ERROR', __FUNCTION__);

}

/**
 * A helper function to check if the dns entry exists.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['domain'] string
 *   Required. A string containing the site domain to add to the hosts file,
 *   such as 'mysite.dev'.
 *   - $params['env']['hosts_file_path'] string
 *   Required. A string containing the path to hosts file to edit, such
 *   as '/etc/hosts'.
 *   - $params['env']['host_ip'] string
 *   Required. A string containing the host ip address to add to the hosts file,
 *   such as '127.0.0.1'.
 * @param string $dns_entry
 *   A string to hold the dns entry.
 * @param string $file
 *   A string to hold the dns file contents.
 *
 * @return bool
 *   TRUE if the dns entry exists.  False if the entry does not exist.
 */
function _rush_dns_exists($params, &$dns_entry, &$file) {

  $dns_entry = $params['env']['host_ip'] . ' ' . $params['build']['domain'] . ' #drushrush';
  $file      = file_get_contents($params['env']['hosts_file_path']);

  if (empty($params['build']['domain'])) {
    return rush_empty_error('$params[\'build\'][\'domain\']', __FUNCTION__);
  }

  if (empty($params['env']['hosts_file_path'])) {
    return rush_empty_error('$params[\'env\'][\'hosts_file_path\']', __FUNCTION__);
  }

  if (empty($params['env']['host_ip'])) {
    return rush_empty_error('$params[\'env\'][\'host_ip\']', __FUNCTION__);
  }

  if (!file_exists($params['env']['hosts_file_path'])) {
    return drush_set_error('RUSH_HOST_OP_ERROR', dt(array(
      '!file'     => $params['env']['hosts_file_path'],
      '!function' => __FUNCTION__,
    ), '[ERROR]: *** !file does not exist.  Called in !function. ***'));
  }

  if (!strpos($file, $params['build']['domain'])) {
    return FALSE;
  }

  return TRUE;
}

/**
 * Configure apache virtual host file using the template.
 *
 * Typically files put in /etc/apache2/sites-enabled/$build_domain .
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['domain'] string
 *   Required. A string containing the site domain to remove from the hosts
 *   file, such as 'mysite.dev'.
 *   - $params['build']['docroot_path'] string
 *   Required. A string containing the path to the build's docroot.
 *   - $params['env']['vhosts_dir_path'] string
 *   Required. A string containing the path to vhosts directory where vhost
 *   files go.
 *   - $params['env']['host_port'] string
 *   Required. A string containing the host port.  Usually '80' or '8888'
 *   for MAMP.
 *
 * @return bool
 *   TRUE on success, FALSE on failure via drush_set_error().
 * @backlog Tweak to use for MAMP and single vhost file.
 * @backlog Add to documentation about creating a logs directory.
 */
function rush_op_create_vhost($params) {

  if (empty($params['build']['domain'])) {
    return rush_empty_error('$params[\'build\'][\'domain\']', __FUNCTION__);
  }

  if (empty($params['build']['docroot_path'])) {
    return rush_empty_error('$params[\'build\'][\'docroot_path\']', __FUNCTION__);
  }

  if (empty($params['env']['vhosts_dir_path'])) {
    return rush_empty_error('$params[\'env\'][\'vhosts_dir_path\']', __FUNCTION__);
  }

  if (empty($params['env']['host_port'])) {
    return rush_empty_error('$params[\'env\'][\'host_port\']', __FUNCTION__);
  }

  // Remove any old vhost -may be same domain but could be different config.
  rush_op_delete_vhost($params);
  // Get template.
  $str_config = RUSH_APACHE_TEMPLATE;
  $str_config = str_replace("#DOMAIN#", $params['build']['domain'], $str_config);
  $str_config = str_replace("#DOCROOT#", $params['build']['docroot_path'], $str_config);
  $str_config = str_replace("#HOSTPORT#", $params['env']['host_port'], $str_config);
  // If an error logs path is set then create an error log entry.
  if (isset($params['build']['logs_path'])) {
    $env_logs_path = $params['build']['logs_path'] . $params['build']['machine_name'] . '.log';
    $str_config    = str_replace("#ERRORLOG#", $env_logs_path, $str_config);
  }
  // If the path is not set, then clear the error log line from the string.
  else {
    $str_config = str_replace("ErrorLog #ERRORLOG#", '', $str_config);
  }

  $cmd = "echo '$str_config' | sudo tee " . $params['env']['vhosts_dir_path'] . $params['build']['domain'];
  $msg = dt('[CREATE VHOST ENTRY]: For !domain in !vhost', array(
    '!domain' => $params['build']['domain'],
    '!vhost'  => $params['env']['vhosts_dir_path'],
  ));

  $config_result = rush_shell_exec($cmd, $msg, 'RUSH_HOST_OP_ERROR', __FUNCTION__);

  if ($config_result && rush_op_restart_apache($params)) {
    return TRUE;
  }
  else {
    return drush_set_error('RUSH_HOST_OP_ERROR', dt('[ERROR]: *** Could not restart apache in !function. ***', array('!function' => __FUNCTION__)));

  }

}

/**
 * Delete the apache virtual hosts config.
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['build']['domain'] string
 *   Required. A string containing the site domain to remove from the hosts
 *   file, such as 'mysite.dev'.
 *   - $params['env']['vhosts_dir_path'] string
 *   Required. A string containing the path to vhosts directory where vhost
 *   files go.
 *
 * @return bool
 *   TRUE on success, FALSE on failure via drush_set_error().
 * @backlog Tweak to use for MAMP and single vhost file.
 * @backlog Add to documentation about creating a logs directory.
 */
function rush_op_delete_vhost($params) {

  if (empty($params['build']['domain'])) {
    return rush_empty_error('$params[\'build\'][\'domain\']', __FUNCTION__);
  }

  if (empty($params['env']['vhosts_dir_path'])) {
    return rush_empty_error('$params[\'env\'][\'vhosts_dir_path\']', __FUNCTION__);
  }

  $file_name = $params['env']['vhosts_dir_path'] . $params['build']['domain'];
  if (file_exists($file_name)) {
    $cmd = 'sudo rm -f ' . $params['env']['vhosts_dir_path'] . $params['build']['domain'];
    $msg = dt('[DELETE VHOST ENTRY]: For !domain in !vhost', array(
      '!domain' => $params['build']['domain'],
      '!vhost'  => $params['env']['vhosts_dir_path'],
    ));

    $config_result = rush_shell_exec($cmd, $msg, 'RUSH_HOST_OP_ERROR', __FUNCTION__);

    if ($config_result && rush_op_restart_apache($params)) {
      return TRUE;
    }
    else {
      return drush_set_error('RUSH_HOST_OP_ERROR', dt('[ERROR]: *** Could not restart apache in !function. ***', array('!function' => __FUNCTION__)));

    }
  }
  // If the file is not gone, return an error.
  if (file_exists($file_name)) {
    return drush_set_error('RUSH_HOST_OP_ERROR', dt('[ERROR]: *** Could not delete vhost file in !function. ***', array('!function' => __FUNCTION__)));
  }

  return TRUE;

}

/**
 * A helper function to restart apache.
 *
 * Runs: apache2 restart .
 *
 * @param array $params
 *   An associative array of all rush parameters.
 *   - $params['env']['apache_bin_path'] string
 *   Optional. A string containing the path an alternative apache bin.
 *
 * @return bool
 *   TRUE on success, FALSE on failure via drush_set_error().
 */
function rush_op_restart_apache($params = array()) {

  // For now just assuming Ubuntu Linux or Darwin OSX.
  $os = PHP_OS;
  switch ($os) {
    case 'Linux':
      // Restart apache.
      return drush_shell_exec("sudo /etc/init.d/apache2 restart");

    case 'Darwin':
      // OSX.
      // Check for a different bin path.
      if (!isset($params['env']['apache_bin_path'])) {
        // Restart Apache at default path.
        return drush_shell_exec("sudo apachectl restart");
      }
      else {
        // Restart Apache at custom path.
        $cmd = $params['env']['apache_bin_path'] . '/apachectl restart';

        return drush_shell_exec("sudo $cmd");
      }

    default:
      return FALSE;
  }
}


/** @}*/
/** @}*/

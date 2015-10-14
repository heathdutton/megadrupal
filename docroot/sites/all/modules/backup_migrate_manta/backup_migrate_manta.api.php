<?php
/**
 * @file
 *
 * This file contains sample hook implementations specific to this module
 */

/**
 * hook_backup_migrate_manta_curl_options_alter
 *
 * This hook allows modules to alter the curl options used to connnect to the
 * Manta endpoint.  Some required options will be overriden by the
 * Manta library if required to have certain values, but this might come
 * in handy for proxy settings, etc.  See the following for various options:
 *
 * http://php.net/manual/en/function.curl-setopt.php
 *
 * NOTE that values may also be set in the settings.php file as an array in
 * the form (same as used by curl_setopt_array):
 *
 * $conf['backup_migrate_manta_curl_options'] = array(
 *   CURLOPT_HTTPPROXYTUNNEL => TRUE,
 *   CURLOPT_PROXYAUTH => CURLAUTH_BASIC,
 *   CURLOPT_PROXYTYPE => CURLPROXY_HTTP,
 *   CURLOPT_PROXY => 'http://127.0.0.1:3128',
 * );
 *
 * These values will then be amended by any alter hook implementations.
 *
 * @param $curl_options
 */
function hook_backup_migrate_manta_curl_options_alter(&$curl_options) {
  if (!isset($curl_options['CURLOPT_PROXY'])) {
    $curl_options['CURLOPT_PROXY'] = 'http://127.0.0.1:3128';
  }
}

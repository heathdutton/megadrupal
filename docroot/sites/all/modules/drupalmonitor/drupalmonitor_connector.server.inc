<?php

/**
 * @file
 * This file handles server metrics
 */

/**
 * Returns server metrics.
 */
function drupalmonitor_connector_get_serverdata() {
  $info['php_uname'] = php_uname('s');
  $info['system_load'] = sys_getloadavg();
  $info['php_version'] = phpversion();
  $info['getrusage'] = getrusage();
  $info['sys_get_temp_dir'] = sys_get_temp_dir();
  $info['PHP_SERVER'] = $_SERVER;
  $info['phpinfo'] = drupalmonitor_connector_phpinfo_array();
  return $info;
}

/**
 * Returns phpinfo() data as array.
 */
function drupalmonitor_connector_phpinfo_array() {
  // Code from http://www.php.net/manual/de/function.phpinfo.php#106862.
  ob_start();
  phpinfo();
  $info_arr = array();
  $info_lines = explode("\n", strip_tags(ob_get_clean(), "<tr><td><h2>"));
  $cat = "General";
  foreach ($info_lines as $line) {
    // Check if a new category is shown on line.
    preg_match("~<h2>(.*)</h2>~", $line, $title) ? $cat = $title[1] : NULL;
    if (preg_match("~<tr><td[^>]+>([^<]*)</td><td[^>]+>([^<]*)</td></tr>~", $line, $val)) {
      $info_arr[$cat][$val[1]] = $val[2];
    }
    elseif (preg_match("~<tr><td[^>]+>([^<]*)</td><td[^>]+>([^<]*)</td><td[^>]+>([^<]*)</td></tr>~", $line, $val)) {
      $info_arr[$cat][$val[1]] = array("local" => $val[2], "master" => $val[3]);
    }
  }
  return $info_arr;
}

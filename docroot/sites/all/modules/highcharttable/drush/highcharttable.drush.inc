<?php

/**
 * @file
 *   Drush integration for HighchartTable.
 */

/**
 * Two JS libraries required for this module.
 */
define('HIGHCHARTS_JS', 'http://code.highcharts.com/highcharts.js');
define('HIGHCHARTTABLE_ZIP', 'https://github.com/highchartTable/jquery-highchartTable-plugin/archive/master.zip');

/**
 * Implements hook_drush_command().
 */
function highcharttable_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['dl-highcharts'] = array(
    'callback' => 'highcharttable_drush_download_libraries',
    'description' => dt('Download and install the Highcharts and HighchartTable JS libraries.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'path' => dt('Optional path to download Highcharts libraries to. Defaults to "sites/all/libraries"'),
    ),
    'aliases' => array('dl-hc'),
  );
  return $items;
}

/**
 * Implementd hook_drush_help().
 *
 * This function is called in response to: 'drush help dl-datatables'
 */
function highcharttable_drush_help($section) {
  switch ($section) {
    case 'drush:dl-highcharts':
      return dt('Download the Highcharts and HighchartTable JS libraries to the appropriate location on your system.');
  }
}

/**
 * Command to download the Highcharts and HighchartTable libraries.
 */
function highcharttable_drush_download_libraries() {
  $args = func_get_args();
  $lib_path = empty($args[0]) ? 'sites/all/libraries' : trim($args[0]);

  // Create the libraries directory if it does not exist.
  if (!is_dir($lib_path)) {
    drush_op('mkdir', $lib_path);
    drush_log(dt('Directory @lib_path created.', array('@lib_path' => $lib_path)), 'notice');
  }
  drush_log(dt('Starting downloads to @lib_path. Please wait...', array('@lib_path' => $lib_path)), 'success');

  // jQuery.highchartTable first (for no partiuclar reason.
  if ($zip = highcharttable_download_and_extract_zip(HIGHCHARTTABLE_ZIP, $lib_path)) {
    if (highcharttable_rename_dir("$lib_path/jquery-highchartTable-plugin-master", "$lib_path/highcharttable")) {
      drush_log(dt('Directory renamed to @lib_path.', array('@lib_path' => "$lib_path/highcharttable")), 'success');
    }
  }

  $lib_highcharts = "$lib_path/highcharts";
  if (!is_dir($lib_highcharts)) {
    drush_op('mkdir', $lib_highcharts);
    drush_log(dt('Directory @lib_path created.', array('@lib_path' => $lib_highcharts)), 'notice');
  }
  $lib_highcharts_js = "$lib_highcharts/js";
  $highcharts_js = "$lib_highcharts/js/highcharts.js";

  if (file_exists($highcharts_js)) {
    drush_log(dt('@highcharts_js already exists. You could try the existing file. Or you could rename it and run this Drush command again to download a fresh copy.', array('@highcharts_js' => $highcharts_js)), 'warning');
    return;
  }
  elseif (!is_dir($lib_highcharts_js)) {
    drush_op('mkdir', $lib_highcharts_js);
    drush_log(dt('Directory @lib_path created.', array('@lib_path' => $lib_highcharts_js)), 'notice');
  }
  // drush_download_file() [without leading underscore] does not work here.
  if (_drush_download_file(HIGHCHARTS_JS, $highcharts_js, TRUE)) {
    drush_log(dt('@url downloaded.', array('@url' => HIGHCHARTS_JS)), 'success');
  }
}

/**
 * Download a zipped library from the specified URL to a destination directory.
 *
 * @param string $url
 *   The url to the .zip to be downloaded.
 * @param string $dest_path
 *   The path relative to the Drupal root to put the extracted .zip
 *   Defaults to sites/all/libraries
 *
 * @return boolean
 *   TRUE when the .zip could be downloaded AND extracted successfully
 */
function highcharttable_download_and_extract_zip($url, $dest_path = 'sites/all/libraries') {
  $zip = drush_download_file($url);
  if ($zip) {
    if (drush_tarball_extract($zip, $dest_path)) {
      drush_log(dt('@url downloaded and extracted.', array('@url' => $url)), 'success');
    }
    else {
      drush_log(dt('@zip was downloaded, but could not be extracted.', array('@zip' => $zip)), 'error');
      return FALSE;
    }
  }
  else {
    drush_log(dt('Drush could not download @url', array('@url' => $url)), 'error');
  }
  return $zip;
}

/**
 * Rename a directory.
 *
 * @param string $old_name
 *   Relative the Drupal root.
 * @param string $new_name
 *   Relative to the Drupal root.
 *
 * @return boolean
 *   TRUE on success, FALSE otherwise
 */
function highcharttable_rename_dir($old_name, $new_name) {
  if (is_dir($new_name) && drush_delete_dir($new_name, TRUE)) {
    drush_log(dt('The existing library directory @new_name was deleted.', array('@new_name' => $new_name)), 'notice');
  }
  if (drush_move_dir($old_name, $new_name, TRUE)) {
    drush_log(dt('The library directory was renamed to @new_name', array('@new_name' => $new_name)), 'notice');
    return TRUE;
  }
  drush_log(dt('The temporary library directory could not be renamed from @old_name to @new_name', array('@old_name' => $old_name, '@new_name' => $new_name)), 'error');
  return FALSE;
}
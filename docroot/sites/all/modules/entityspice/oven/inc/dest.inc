<?php

/**
 * @file
 * Copied from drush's core to find propert location to put generated module
 * in.
 */

function _oven_pm_download_destination_lookup($type, $drupal_root, $sitepath, $create = FALSE) {
  if ($type == 'theme engine') {
    $destination = 'themes/engines';
  }
  // Profiles in Drupal < 8
  elseif (($type == 'profile') && (drush_drupal_major_version() < 8)) {
    $destination = 'profiles';
  }
  // Type: module, theme or profile.
  else {
    $destination = $type . 's';
    // Prefer /contrib if it exists.
    if ($sitepath) {
      $destination = $sitepath . '/' . $destination;
    }
    $contrib = $destination . '/contrib';
    if (is_dir($contrib)) {
      $destination = $contrib;
    }
  }
  if ($create) {
    drush_log(dt('Attempting to create destination directory at !dir', array('!dir' => $destination)));
    drush_mkdir($destination, TRUE);
  }
  if (is_dir($destination)) {
    drush_log(dt('Using destination directory !dir', array('!dir' => $destination)));
    return $destination;
  }
  drush_log(dt('Could not find destination directory at !dir', array('!dir' => $destination)));
  return FALSE;
}

/**
 * Returns the best destination for a particular download type we can find.
 *
 * It is based on the project type and drupal and site contexts.
 */
function _oven_pm_download_destination($type) {
  $drupal_root = drush_get_context('DRUSH_DRUPAL_ROOT');
  $site_root = drush_get_context('DRUSH_DRUPAL_SITE_ROOT');
  $full_site_root = empty($drupal_root) ? '' : $drupal_root .'/'. $site_root;
  $sitewide = $drupal_root . '/' . drush_drupal_sitewide_directory();

  $in_site_directory = FALSE;
  // Check if we are running within the site directory.
  if (strpos(realpath(drush_cwd()), realpath($full_site_root)) !== FALSE || (drush_get_option('use-site-dir', FALSE))) {
    $in_site_directory = TRUE;
  }

  $destination = '';
  if ($type != 'core') {
    // Attempt 1: If we are in a specific site directory, and the destination
    // directory already exists, then we use that.
    if (empty($destination) && $site_root && $in_site_directory) {
      $create_dir = drush_get_option('use-site-dir', FALSE);
      $destination = _oven_pm_download_destination_lookup($type, $drupal_root, $full_site_root, $create_dir);
    }
    // Attempt 2: If the destination directory already exists for
    // the sitewide directory, use that.
    if (empty($destination) && $drupal_root) {
      $destination = _oven_pm_download_destination_lookup($type, $drupal_root, $sitewide);
    }
    // Attempt 3: If a specific (non default) site directory exists and
    // the sitewide directory does not exist, then create destination
    // in the site specific directory.
    if (empty($destination) && $site_root && $site_root !== 'sites/default' && is_dir($full_site_root) && !is_dir($sitewide)) {
      $destination = _oven_pm_download_destination_lookup($type, $drupal_root, $full_site_root, TRUE);
    }
    // Attempt 4: If sitewide directory exists, then create destination there.
    if (empty($destination) && is_dir($sitewide)) {
      $destination = _oven_pm_download_destination_lookup($type, $drupal_root, $sitewide, TRUE);
    }
    // Attempt 5: If site directory exists (even default), then create
    // destination in that directory.
    if (empty($destination) && $site_root && is_dir($full_site_root)) {
      $destination = _oven_pm_download_destination_lookup($type, $drupal_root, $full_site_root, TRUE);
    }
  }
  // Attempt 6: If we didn't find a valid directory yet (or we somehow found
  // one that doesn't exist) we always fall back to the current directory.
  if (empty($destination) || !is_dir($destination)) {
    $destination = drush_cwd();
  }

  return $destination;
}

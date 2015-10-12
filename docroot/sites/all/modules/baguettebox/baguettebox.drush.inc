<?php

/**
 * @file
 * Drush integration for BaguetteBox module.
 */

/**
 * The BaguetteBox library URLs.
 */
define('BAGUETTEBOX_DOWNLOAD_JS_URL', 'https://raw.githubusercontent.com/feimosi/baguetteBox.js/master/dist/baguetteBox.min.js');
define('BAGUETTEBOX_DOWNLOAD_CSS_URL', 'https://raw.githubusercontent.com/feimosi/baguetteBox.js/master/dist/baguetteBox.min.css');

/**
 * Implements hook_drush_command().
 */
function baguettebox_drush_command() {
  $items['baguettebox-download'] = array(
    'description' => dt('Download and install baguetteBox library.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'aliases' => array('bb-d'),
  );
  return $items;
}

/**
 * Command to download the BaguetteBox library.
 */
function drush_baguettebox_download() {

  $libraries_dir = 'sites/all/libraries';

  if (!is_dir($libraries_dir)) {
    return drush_set_error('BAGUETTEBOX', dt('Directory !libraries_dir does not exitst.', array('!libraries_dir' => $libraries_dir)));
  }

  if (is_dir(BAGUETTEBOX_LIBRARY_DIR)) {
    if (drush_confirm(dt('Install location !dir already exists. Do you want to overwrite it?', array('!dir' => BAGUETTEBOX_LIBRARY_DIR)))) {
      drush_delete_dir(BAGUETTEBOX_LIBRARY_DIR, TRUE);
    }
    else {
      return drush_log(dt('Aborting.'));
    }
  }

  drush_mkdir(BAGUETTEBOX_LIBRARY_DIR);

  drush_shell_exec('wget --timeout=15 -O %s %s', BAGUETTEBOX_LIBRARY_DIR . '/baguetteBox.min.js', BAGUETTEBOX_DOWNLOAD_JS_URL);
  drush_shell_exec('wget --timeout=15 -O %s %s', BAGUETTEBOX_LIBRARY_DIR . '/baguetteBox.min.css', BAGUETTEBOX_DOWNLOAD_CSS_URL);

  $js_file_found = drush_file_not_empty(BAGUETTEBOX_LIBRARY_DIR . '/baguetteBox.min.js');
  $css_file_found = drush_file_not_empty(BAGUETTEBOX_LIBRARY_DIR . '/baguetteBox.min.css');
  if (!$js_file_found || !$css_file_found) {
    return drush_set_error('BAGUETTEBOX', dt('Could not download baguetteBox library from GitHub.'));
  }
  else {
    drush_log(dt('BaguetteBox library has been installed in @path', array('@path' => BAGUETTEBOX_LIBRARY_DIR)), 'success');
  }

}

<?php
/**
 * @file
 * Download pdfcrowd library with drush.
 */

/**
 * Implements hook_drush_command().
 */
function pdfcrowd_drush_command() {
  $items['dl-pdfcrowd'] = array(
    'callback' => 'pdfcrowd_drush_download',
    'description' => dt('Downloads required pdfcrowd library.'),
  );

  return $items;
}

/**
 * Pdfcrowd PHP API download callback.
 */
function pdfcrowd_drush_download() {
  $library = libraries_load('pdfcrowd');
  $path = 'sites/all/libraries/pdfcrowd';
  $url = $library['download url'];

  // Download and extract library.
  if (($tmp = drush_download_file($url)) && drush_tarball_extract($tmp, $path)) {
    drush_log(dt('Pdfcrowd PHP API has been downloaded to @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download Pdfcrowd PHP API to @path', array('@path' => $path)), 'error');
  }
}

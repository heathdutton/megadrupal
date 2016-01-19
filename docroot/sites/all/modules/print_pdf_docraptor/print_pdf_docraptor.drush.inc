<?php

/**
 * @file
 * Drush integration for print_pdf_docraptor module PDF libraries download.
 */

/**
 * The PDF project download URL.
 *
 * URI to the the latest Docraptor version.. Hardcoded version unfortunately.
 */
define('DOCRAPTOR_DOWNLOAD_URI', 'https://github.com/apmsooner/docraptor/archive/master.zip');

/**
 * Implements hook_drush_command().
 */
function print_pdf_docraptor_drush_pdf_libs_alter(&$pdf_libs) {
  $pdf_libs['docraptor'] = array(
    'callback' => '_print_pdf_docraptor_drush_download_url',
  );
}

/**
 * Discover the correct URL of the package to download.
 *
 * @return string
 *   URL of the file to download, FALSE if not known
 */
function _print_pdf_docraptor_drush_download_url() {
  return DOCRAPTOR_DOWNLOAD_URI;
}

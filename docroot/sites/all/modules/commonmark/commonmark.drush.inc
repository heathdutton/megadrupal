<?php
/**
 * @file
 * Drush commandfile for CommonMark module.
 */

/**
 * Implements hook_drush_command().
 */
function commonmark_drush_command() {
  $items['commonmark-download'] = array(
    'description' => dt('Downloads the necessary CommonMark PHP library and optional extensions.'),
    'options' => array(
      'force' => dt('Will force re-installation of libraries if they already exist.'),
      'no-extensions' => dt('Will not download the extra extension libraries.'),
      'path' => dt('The path, relative to the Drupal root directory, to download into. Defaults to: sites/all/libraries.')
    ),
  );
  return $items;
}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_commonmark_download() {
  include_once 'commonmark.module';

  $drupal_root = drush_get_context('DRUSH_DRUPAL_ROOT');
  $no_extensions = drush_get_option('no-extensions', FALSE);
  $force = drush_get_option('force', FALSE);
  $path = drush_get_option('path', 'sites/all/libraries');

  $libraries = [
    [
      'vendor' => 'thephpleague',
      'package' => 'commonmark',
      'version' => '0.10.0',
    ],
    [
      'vendor' => 'webuni',
      'package' => 'commonmark-attributes-extension',
      'version' => '0.2.0',
      'extension' => TRUE,
    ],
    [
      'vendor' => 'webuni',
      'package' => 'commonmark-table-extension',
      'version' => '0.3.0',
      'extension' => TRUE,
    ],
  ];

  // Keep track of installed libraries.
  $installed = [];

  // Iterate over all the libraries.
  foreach ($libraries as $library) {
    // Skip downloading extensions if option was provided.
    if ($no_extensions && !empty($library['extension'])) {
      continue;
    }

    $library_path = "$drupal_root/$path/" . $library['vendor'] . '/' . $library['package'];
    $tokens = [
      '@library' => $library['vendor'] . '/' . $library['package'],
      '@library_path' => str_replace($drupal_root . '/', '', $library_path),
    ];

    // Check for an existing installation.
    if ($installed_library_path = commonmark_get_library_path($library['vendor'], $library['package'])) {
      $installed[] = $installed_library_path;
      if (!$force) {
        $tokens['@installed_path'] = str_replace($drupal_root . '/', '', $installed_library_path);
        drush_log(dt('The @library library is already installed at: @installed_path.', $tokens), 'ok');
        continue;
      }
      if (is_dir($installed_library_path)) {
        drush_delete_dir($installed_library_path, TRUE);
      }
    }

    // Download the archive file.
    $url = 'https://github.com/' . $library['vendor'] . '/' . $library['package'] . '/archive/' . $library['version'] . '.tar.gz';
    if ($archive_file = drush_download_file($url, drush_find_tmp() . '/' . $library['vendor'] . '-' . $library['package'] . '.zip')) {
      // Ensure the base directory exists.
      $base_dir = dirname($library_path);
      if (!is_dir($base_dir)) {
        drush_mkdir($base_dir);
      }

      // Remove any existing library directory.
      if (is_dir($library_path)) {
        drush_delete_dir($library_path, TRUE);
      }
      make_download_file_unpack($archive_file, $library_path, $library['package'], $library['package'] . '-' . $library['version']);
    }

    if (is_dir($library_path)) {
      drush_log(dt('The @library library has been installed in @library_path.', $tokens), 'success');
    }
    else {
      drush_log(dt('Drush was unable to install the @library in @library_path', $tokens), 'error');
    }
  }

  if (!$force && !empty($installed)) {
    drush_print("\n" . dt('Use the --force option to reinstall the libraries.'));
  }
}

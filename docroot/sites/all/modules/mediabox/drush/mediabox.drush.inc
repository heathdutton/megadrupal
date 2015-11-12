<?php

/**
 * @file
 *   drush integration for mediabox.
 */

/**
 * Implementation of hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * See `drush topic docs-commands` for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function mediabox_drush_command() {
  return array(
    'mediabox-jcrop' => array(
      'callback' => 'drush_mediabox_jcrop',
      'description' => dt('Download and install the jquery Jcrop plugin.'),
      'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
      'arguments' => array(
        'path' => dt('Optional. A path where to install the jquery Jcrop plugin. If omitted Drush will use the default location.'),
      ),
      'aliases' => array('mediaboxjcrop'),
    ),
  );
}

/**
 * Implementation of hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function mediabox_drush_help($section) {
  switch ($section) {
    case 'drush:mediabox-jcrop':
      return dt('Download and install the jquery Jcrop plugin from github.com/tapmodo/Jcrop, default location is sites/all/libraries.');
  }
}

/**
 * Command to download the jquery Jcrop plugin.
 */
function drush_mediabox_jcrop() {
  $args = func_get_args();
  if (!empty($args[0])) {
    $path = $args[0];
  }
  else {
    $path = 'sites/all/libraries';
  }

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);

  // Download the zip archive.
  if ($filepath = drush_download_file('https://github.com/tapmodo/Jcrop/archive/v0.9.12.tar.gz')) {
    $filename = basename($filepath);
    $dirname = 'Jcrop-' . substr(basename($filepath, '.tar.gz'), 1);

    // Remove any existing plugin directory.
    if (is_dir($dirname) || is_dir('Jcrop')) {
      drush_delete_dir($dirname, TRUE);
      drush_delete_dir('Jcrop', TRUE);
      drush_log(dt('An existing jquery Jcrop plugin was deleted from @path', array('@path' => $path)), 'notice');
    }

    // Decompress the zip archive.
    drush_tarball_extract($filename);

    // Change the directory name to "Jcrop" if needed.
    if ($dirname != 'Jcrop') {
      drush_move_dir($dirname, 'Jcrop', TRUE);
      $dirname = 'Jcrop';
    }
  }

  if (is_dir($dirname)) {
    drush_log(dt('jquery Jcrop plugin has been installed in @path', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to install the jquery Jcrop plugin plugin to @path', array('@path' => $path)), 'error');
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);
}

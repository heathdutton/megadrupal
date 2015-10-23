<?php
/**
 * @file
 * Drush integration for Libraries API.
 *
 * @author Sergii Bondarenko, <sb@firstvector.org>
 */

// Prevent conflict with https://www.drupal.org/node/1884246.
if (!function_exists('_drush_libraries_download')) {
  /**
   * Implements hook_drush_command().
   */
  function libapi_drush_command() {
    $commands = array();

    $commands['libraries-list'] = array(
      'description' => dt('Lists registered library information.'),
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
      'aliases' => array('libl', 'liblist'),
    );

    $commands['libraries-download'] = array(
      'description' => dt('Download all non-existent libraries from "hook_libraries_info()". You able to specify necessary libraries separating their names by spaces or download all non-existent libraries.'),
      'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
      'aliases' => array('libd', 'libget'),
      'options' => array(
        'profile' => dt('Specify this empty option if you want to download libraries into profile installation folder. In this case the "--uri" parameter will be ignored.'),
        'all' => dt('Download all defined, non-existent libraries.'),
      ),
      'examples' => array(
        'drush ld <NAME...> [--uri|-l <SUBSITE>]' => dt('Download defined libraries.'),
      ),
    );

    return $commands;
  }

  /**
   * Implements hook_drush_cache_clear().
   *
   * @see drush_cache_clear_types()
   */
  function libapi_drush_cache_clear(array &$types) {
    $types['libraries'] = 'libapi_drush_invalidate_cache';
  }

  /**
   * Clears the library cache.
   */
  function libapi_drush_invalidate_cache() {
    // @see drupal_flush_all_caches()
    foreach (libraries_flush_caches() as $table) {
      cache_clear_all('*', $table, TRUE);
    }
  }

  /**
   * Implements drush_COMPONENT_COMMAND().
   */
  function drush_libapi_libraries_list() {
    $libraries = array();

    foreach (libraries_info() as $name => $info) {
      $libraries[$name] = libraries_detect($name);
    }

    ksort($libraries);

    if (empty($libraries)) {
      drush_print('There are no registered libraries.');
    }
    else {
      $rows = array();
      // drush_print_table() automatically treats the first row as the
      // header, if $header is TRUE.
      $rows[] = array(
        dt('Name'),
        dt('Status'),
        dt('Version'),
        dt('Variants'),
        dt('Dependencies'),
      );

      foreach ($libraries as $name => $library) {
        // Only list installed variants.
        $variants = array();

        foreach ($library['variants'] as $variant_name => $variant) {
          if (!empty($variant['installed'])) {
            $variants[] = $variant_name;
          }
        }

        $rows[] = array(
          $name,
          $library['installed'] ? dt('OK') : drupal_ucfirst($library['error']),
          $library['installed'] || empty($library['version']) ? '-' : $library['version'],
          empty($variants) ? '-' : implode(', ', $variants),
          empty($library['dependencies']) ? '-' : implode(', ', $library['dependencies']),
        );
      }

      // Make the possible values for the 'Status' column and
      // the 'Version' header wrap nicely.
      drush_print_table($rows, TRUE, array(0, 12, 7, 0, 0));
    }
  }

  /**
   * Implements drush_COMPONENT_COMMAND().
   */
  function drush_libapi_libraries_download() {
    $info = libraries_info();

    if (!drush_get_option('all', FALSE)) {
      $info = array_intersect_key($info, array_flip(array_slice(drush_get_arguments(), 1)));
    }

    if (empty($info)) {
      drush_log(dt('You are not specified any existing library, defined by "hook_libraries_info()".'), 'error');
    }
    else {
      $path = sprintf('%s/%s/libraries', drush_locate_root(), _drush_libraries_download_path());
      $libraries = libraries_get_libraries();
      $downloaded = _drush_libraries_download($path, $libraries, $info);

      if (!empty($downloaded) && drush_confirm(dt('The next libraries were already downloaded: "!libs". Do you want re-download them? Old data will be erased!', array('!libs' => implode('", "', $downloaded))))) {
        // Reset an array with items for downloading.
        $download = array();

        // Set affirmative context to "TRUE", because a user gave his consent
        // for re-downloading.
        drush_set_context('DRUSH_AFFIRMATIVE', TRUE);

        foreach ($downloaded as $machine_name => $name) {
          if (drush_delete_dir($libraries[$machine_name], TRUE)) {
            // Allow to re-download successfully deleted libraries only.
            $download[$machine_name] = $info[$machine_name];
          }
          else {
            drush_log(dt('Cannot remove the "@dir" directory.', array('@dir' => $libraries[$machine_name])), 'error');
          }
        }

        if (!empty($download)) {
          _drush_libraries_download($path, array(), $download);
        }
      }
    }
  }

  /**
   * Download libraries.
   *
   * @internal
   *
   * @param string $path
   *   Destination path.
   * @param array $existing
   *   An empty or returned by {@link libraries_get_libraries()} array.
   * @param array $download
   *   Structured array from {@link libraries_info()} function.
   *
   * @return array
   *   An empty array if nothing has been downloaded or filled by names of
   *   downloaded libraries.
   */
  function _drush_libraries_download($path, array $existing, array $download) {
    $downloaded = array();

    foreach ($download as $name => $library) {
      $filename = basename($library['download url']);
      // Path to file that will be downloaded.
      $file = "$path/$filename";
      // Temporary path needed for extracting archives content
      // for future moving of contents to destination folder.
      $tmp = "$file-tmp";

      if (empty($existing[$name])) {
        // Keep going, if library does not exist and user allows to download it.
        if (drush_confirm(dt('Are you want download the "@lib" library?', array('@lib' => $library['name'])))) {
          // Download an archive into "*/libraries" folder.
          if (!empty($library['download url']) && _drush_download_file($library['download url'], $file)) {
            $destination = "$path/$name";
            // Re-assign variable because we need to remove directory later.
            $source = $file;

            if (drush_file_is_tarball($file)) {
              // Extract data into "*/libraries/LIBRARY-tmp" directory and get
              // the listing of the whole structure of an archive.
              $listing = drush_tarball_extract($file, $tmp, TRUE);

              if (!empty($listing)) {
                $source = $tmp . '/' . reset($listing);
              }
            }
            // Process single file.
            elseif (drush_mkdir($destination)) {
              $destination .= "/$filename";
            }

            // Move the downloaded data into a "*/libraries/LIBRARY" directory.
            if (drush_copy_dir($source, $destination)) {
              drush_log(dt('The library was downloaded to "@dir".', array('@dir' => "$path/$name")), 'success');
            }
            else {
              drush_log(dt('Library could not be moved from temporary folder.'), 'error');
            }

            array_map('drush_delete_dir', array($file, $tmp));
          }
          else {
            drush_log(dt('To download a library, the "download url" parameter shall point to file that can be downloaded.'), 'error');
          }
        }
      }
      else {
        $downloaded[$name] = $library['name'];
      }
    }

    return $downloaded;
  }

  /**
   * Determine downloading destination.
   *
   * @internal
   *
   * @return string
   *   Relative path for downloading.
   */
  function _drush_libraries_download_path() {
    if (drush_get_option('profile', FALSE)) {
      return drupal_get_path('profile', drupal_get_profile());
    }

    $uri = drush_get_option('uri');
    $path = drush_conf_path($uri);

    // If "--uri" or "-l" parameter specified, then check that configuration
    // path exists or user specially chosen "default" directory.
    if (!isset($path) || ('sites/default' === $path && 'default' !== $uri)) {
      return drush_drupal_sitewide_directory();
    }

    return $path;
  }
}

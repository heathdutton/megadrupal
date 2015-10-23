<?php

/**
 * @file
 * Drush integration for Rijsoverheid Cookie Opt-in module.
 */

/**
 * Implements hook_drush_command().
 */
function ro_cookie_opt_in_drush_command() {
  $items = array();

  $items['dl-ro-cookie-opt-in'] = array(
    'callback' => 'ro_cookie_opt_in_drush_library_download',
    'description' => dt('Downloads the required Rijksoverheid Cookie Opt-in library.'),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function ro_cookie_opt_in_drush_help($section) {
  switch ($section) {
    case 'drush:dl-ro-cookie-opt-in':
      return dt("Downloads the required Rijksoverheid Cookie Opt-in library.");
  }
}

/**
 * Download library callback.
 */
function ro_cookie_opt_in_drush_library_download() {
  $remove_files = drush_confirm(dt("Do you want Drush to remove unnecessary files from the library?"));
  if (module_exists('libraries')) {
    $libraries_path = 'sites/all/libraries';

    // Check if the libraries path exists and if not, create it.
    file_prepare_directory($libraries_path, FILE_CREATE_DIRECTORY);

    $path = $libraries_path . '/rijksoverheid-cookie-opt-in';
    // Check if the library is already installed.
    $library = libraries_detect('rijksoverheid-cookie-opt-in');
    if ($library['installed'] === TRUE) {
      // Remove the exisiting library folder.
      file_unmanaged_delete_recursive($path);
    }
    // Download the library zip file.
    $request = drupal_http_request($library['download url']);
    if ($request->code == '200') {
      // Save the library zip file.
      $bytes = file_put_contents($libraries_path . '/ro_cookie_opt_in.zip', $request->data);
      if ($bytes !== FALSE) {
        // Open the library zip file.
        $zip = new ZipArchive();
        $res = $zip->open($libraries_path . '/ro_cookie_opt_in.zip');
        if ($res === TRUE) {
          // Extract the library zip file.
          if ($zip->extractTo($libraries_path) === TRUE) {
            $zip->close();
            // Remove the library zip file.
            if (file_unmanaged_delete($libraries_path . '/ro_cookie_opt_in.zip') === TRUE) {
              // Rename the library folder.
              if (rename($libraries_path . '/rijksoverheid-cookie-opt-in-v1.1', $libraries_path . '/rijksoverheid-cookie-opt-in') === TRUE) {
                if ($remove_files) {
                  file_unmanaged_delete_recursive($path . '/content');
                  file_unmanaged_delete($path . '/behaviour/cookies.js');
                  file_unmanaged_delete($path . '/behaviour/cookiebar-init.js');
                  file_unmanaged_delete($path . '/behaviour/jquery-1.8.1-ui-1.8.23.custom.min.js');
                  file_unmanaged_delete($path . '/presentation/screen.css');
                  file_unmanaged_delete($path . '/presentation/screen-cookies.css');
                  file_unmanaged_delete($path . '/presentation/screen-ie6.css');
                  file_unmanaged_delete($path . '/presentation/screen-ie7.css');
                  file_unmanaged_delete($path . '/voorbeeld.html');
                  file_unmanaged_delete($path . '/cookie overzicht Voorbeeldtekst v5.doc');
                  file_unmanaged_delete($path . '/cookie overzicht Voorbeeldtekst v5 Engels.doc');
                  file_unmanaged_delete($path . '/Documentatie-Rijksoverheid-Cookie-opt-in-v1.1.pdf');
                  file_unmanaged_delete($path . '/Documentatie-Rijksoverheid-Cookie-opt-in-v1.1.pdf');
                  file_unmanaged_delete($path . '/handleiding technische implementatie.txt');
                  drush_log(dt('Unnecessary files were removed from the library.'), 'success');
                }
                drush_log(dt('The Rijksoverheid Cookie Opt-in library has been successfully downloaded.'), 'success');
              }
              else {
                drush_log(dt('Drush was unable to rename the Rijksoverheid Cookie Opt-in library folder.'), 'error');
              }
            }
            else {
              drush_log(dt('Drush was unable to remove the Rijksoverheid Cookie Opt-in zip file.'), 'error');
            }
          }
          else {
            drush_log(dt('Drush was unable to extract the Rijksoverheid Cookie Opt-in library.'), 'error');
          }
        }
        else {
          drush_log(dt('Drush was unable to open the Rijksoverheid Cookie Opt-in library zip file.'), 'error');
        }
      }
      else {
        drush_log(dt('Drush was unable to save the Rijksoverheid Cookie Opt-in library zip file.'), 'error');
      }
    }
    else {
      drush_log(dt('Drush was unable to download the Rijksoverheid Cookie Opt-in library zip file.'), 'error');
    }
  }
  else {
    drush_log(dt('You need to install the libraries (7.x-.2.x branch) module to use the Rijksoverheid Cookie Opt-in module.'), 'error');
  }
}

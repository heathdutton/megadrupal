<?php
/**
 * @file
 * Mobile Detect class import functions.
 */

/**
 * Helper function to update the Mobile Detect class file.
 *
 * @param $cron
 *   Optional import environment. If false, display status messages to
 *   the user in addition to logging information with the watchdog.
 */
function _mobile_detect_import_import($cron = TRUE) {
  $make_import = FALSE;

  // The local imported Mobile Detect class version number.
  // This is the version in the files directory.
  $local_files_version = variable_get('mobile_detect_import_version', 0);

  // Retrieve the current Mobile Detect class version number using HTTPS.
  $current_version = $current_file = drupal_http_request(MOBILE_SWITCH_LIBRARY_DOWNLOAD_URL);

  // Log an error if the Mobile Detect class version number
  // could not be retrieved.
  if (isset($current_version->error)) {
    // Log a message with the watchdog.
    watchdog('Mobile Detect Import', "Couldn't check version: %error", array('%error' => $current_version->error), WATCHDOG_ERROR);

    // Display a message to the user if the update process
    // was triggered manually.
    if ($cron == FALSE) {
      drupal_set_message(t("Couldn't check Mobile Detect class version: %error", array('%error' => $current_version->error)), 'error');
    }

    return;
  }

  $current_version = str_replace(array("\r\n", "\n", "\r"), '|', $current_version->data);
  $current_version_arr = explode("|", $current_version);

  foreach ($current_version_arr as $val) {
    if (preg_match(MOBILE_SWITCH_LIBRARY_VERSION_PATTERN, $val, $matches)) {
      $current_version = $matches[1];
      break;
    }
  }

  // Compare the current and local imported version numbers to determine if the
  // Mobile Detect class requires import.
  if ($current_version == $local_files_version) {
    watchdog('Mobile Detect Import', 'The import of a new version of the Mobile Detect class is not necessary.');

    // Display a message to the user if the update process was triggered
    // manually.
    if ($cron === FALSE) {
      drupal_set_message(t('The import of a new version of the Mobile Detect class is not necessary.'));
    }

    return;
  }
  elseif ($current_version > $local_files_version) {
    $make_import = TRUE;
  }

  // Import a new Mobile Detect class file to the files directory.
  if ($make_import === TRUE && $current_file->data) {
    $path = 'public://' . MOBILE_SWITCH_LIBRARY_NAME;
    $uri = $path . '/' . MOBILE_SWITCH_LIBRARY_FILE_NAME;

    file_prepare_directory($path, FILE_CREATE_DIRECTORY);

    // Check the local imported version.
    $import_message = '';
    if (file_exists($uri)) {
      // Re-check the version to avoid conflicts.
      $local_files_version = mobile_switch_mobile_detect_get_version($uri);

      // An imported local version is not available or
      // is a smaller version than the currently available.
      if ($local_files_version < $current_version) {
        file_unmanaged_save_data($current_file->data, $uri, FILE_EXISTS_REPLACE);

        // Update the local imported Mobile Detect class version.
        // Note: This is the imported, not the installed library version.
        variable_set('mobile_detect_import_version', $current_version);

        watchdog('Mobile Detect Import', 'The new version %version of the Mobile Detect class imported to the files directory.', array('%version' => $current_version));

        $import_message = 'imported';
      }
      // The imported local version exist and is up to date.
      elseif ($local_files_version >= $current_version) {
        $import_message = 'exist';
      }
    }

    if (!file_exists($uri) && !file_unmanaged_save_data($current_file->data, $uri, FILE_EXISTS_REPLACE)) {
      return FALSE;
    }

    // Rewrite the imported file URI.
    if (variable_get('mobile_detect_import_file_uri', FALSE) == FALSE) {
      variable_set('mobile_detect_import_file_uri', drupal_realpath($uri));
    }

    // Display a message to the user if the update process
    // was triggered manually.
    if ($cron === FALSE) {
      switch ($import_message) {
        case 'imported':
          drupal_set_message(t('The new version %version of the Mobile Detect class imported to the files directory.', array('%version' => $current_version)));
          break;
        case 'exist':
          drupal_set_message(t('The new version %version is available in the files directory.', array('%version' => $current_version)));
          break;
      }
    }
  }
}

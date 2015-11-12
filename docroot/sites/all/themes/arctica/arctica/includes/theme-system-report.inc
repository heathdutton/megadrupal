<?php

if (user_access('administer site configuration')) {
  $directory = 'public://';
  $is_writable = is_writable($directory);
  $is_directory = is_dir($directory);
  if (!$is_writable || !$is_directory) {
    if (!$is_directory) {
      $error = t('the files directory does not exist.') . ' ';
    }
    else {
      $error = t('the files directory is not writable.') . ' ';
    }
    // The files directory requirement check is done only during install and runtime.

    drupal_set_message(t('@theme cannot function correctly because ', array('@theme' => $theme)) . $error . t('You may need to set the correct directory at the <a href="@admin-file-system">file system settings page</a> or change the current directory\'s permissions so that it is writable.', array('@admin-file-system' => url('admin/config/media/file-system'))), 'error');
  }
}

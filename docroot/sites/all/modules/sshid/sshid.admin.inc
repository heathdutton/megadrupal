<?php
/**
 * @file
 *  SSH Module admin functions
 */

/**
 * Page callback for admin/config/development/ssh
 */
function sshid_settings_page() {
  return '<p>No settings yet.</p>';
}

/**
 * Page callback for admin/config/development/ssh/config
 */
function sshid_config_file_download() {
  $config_file = sshid_config_file_contents();

  $file = new stdClass();
  $file->filename = 'config';
  $file->filemime = 'text/plain';
  $file->filesize = strlen($config_file);

  $headers = file_get_content_headers($file);
  $headers['Content-Disposition'] = 'attachment; filename="' . $file->filename . '"';
  drupal_send_headers($headers);

  echo $config_file;
  exit;
}
<?php
/**
 * @file
 * Drush integration for File Redirect.
 */
/**
 * Implements hook_drush_command().
 */
function file_redirect_drush_command() {
  $items['file-redirect'] = array(
    'description' => dt('Set the production base URL to redirect missing file requests.'),
    'arguments' => array('url' => 'The production URL'),
  );

  return $items;
}

/**
 * Command Callback: file-redirect
 */
function drush_file_redirect($url = '') {
  variable_set('file_redirect_production_host', rtrim($url, '/'));

  // Check directory existence first.
  $dir = variable_get('file_public_path');
  if (drush_file_redirect_check_directory($dir)) {
    file_redirect_create_htaccess();
  }
}

/**
 * Implements drush_hook_COMMAND_validate().
 */
function drush_file_redirect_validate($url = '') {
  if (empty($url)) {
    return drush_set_error('INVALID_URL', dt('You must specify a URL.'));
  }

  if (!preg_match('#^http(s)?://#', $url)) {
    return drush_set_error('INVALID_URL', dt('URL must begin with a valid protocol (ex. http://).', array('!url' => $url)));
  }

  if (!valid_url($url)) {
    return drush_set_error('INVALID_URL', dt('URL !url is not valid.', array('!url' => $url)));
  }
}

/**
 * Checks the existence of the directory specified in $form_element.
 *
 * @param $form_element
 *   The form element containing the name of the directory to check.
 */
function drush_file_redirect_check_directory($directory) {
  if (strlen($directory) == 0) {
    return FALSE;
  }

  if (!@is_dir($directory) && !@drupal_mkdir($directory, NULL, TRUE)) {
    // If the directory does not exists and cannot be created.
    return drush_set_error('UNWRITABLE_DIR', dt('The directory !directory does not exist and could not be created.', array('!directory' => $directory)));
  }

  if (@is_dir($directory) && !@is_writable($directory) && !@drupal_chmod($directory)) {
    // If the directory is not writable and cannot be made so.
    return drush_set_error('UNWRITABLE_DIR', dt('The directory !directory exists but is not writable and could not be made writable.', array('!directory' => $directory)));
  }
  elseif (is_dir($directory)) {
    // Create public .htaccess file.
    @file_create_htaccess($directory, FALSE);
    return TRUE;
  }

  return TRUE;
}

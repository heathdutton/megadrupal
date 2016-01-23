<?php

/**
 * @file
 * Admin page callbacks for the responsive_favicons module.
 */

/**
 * Menu callback; Provide the administration overview page.
 */
function responsive_favicons_config_page() {
  $form['responsive_favicons_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to responsive favicon files'),
    '#description' => t('A local file system path where favicon files will be stored. This directory must exist and be writable by Drupal.'),
    '#field_prefix' => file_create_url('public://'),
    '#default_value' => variable_get('responsive_favicons_path', 'favicons'),
  );
  $form['responsive_favicons_tags'] = array(
    '#type' => 'textarea',
    '#title' => t('Favicon tags'),
    '#description' => t('Paste the code provided by <a href="http://realfavicongenerator.net/" target="_blank">http://realfavicongenerator.net/</a>. Make sure each link is on a separate line. It is fine to paste links with paths like "/apple-touch-icon-57x57.png" as these will be converted to the correct paths automatically.'),
    '#default_value' => implode(PHP_EOL, variable_get('responsive_favicons_tags', array())),
    '#rows' => 16,
  );
  $form['responsive_favicons_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload a zip file from realfavicongenerator.net to install'),
    '#description' => t('For example: %filename from your local computer. This only needs to be done once.', array('%filename' => 'favicons.zip')),
  );
  $form['responsive_favicons_remove_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove default favicon from Drupal'),
    '#description' => t('It is recommended to remove default favicon as it can cause issues'),
    '#default_value' => variable_get('responsive_favicons_remove_default', 0),
  );

  $form['#submit'][] = 'responsive_favicons_config_page_submit';

  return system_settings_form($form);
}

/**
 * Implements additional submit logic for responsive_favicons_settings_form().
 */
function responsive_favicons_config_page_submit($form, &$form_state) {
  // We want to save tags as an array.
  if (isset($form_state['values']['responsive_favicons_tags'])) {
    $tags = explode(PHP_EOL, $form_state['values']['responsive_favicons_tags']);
    $tags = array_filter($tags);
    foreach ($tags as $pos => $tag) {
      $tags[$pos] = trim($tag);
    }
    variable_set('responsive_favicons_tags', $tags);
    unset($form_state['values']['responsive_favicons_tags']);
  }

  // Remove trailing slash on responsive_favicons_path.
  $form_state['values']['responsive_favicons_path'] = rtrim($form_state['values']['responsive_favicons_path'], '/');

  // Attempt the upload and extraction of the zip file. This code is largely
  // based on the code in Drupal core.
  //
  // @see update_manager_install_form_submit().
  if ($_FILES['files']['name']['responsive_favicons_upload']) {
    $validators = array('file_validate_extensions' => array(archiver_get_extensions()));
    $field = 'responsive_favicons_upload';
    if (!($finfo = file_save_upload($field, $validators, NULL, FILE_EXISTS_REPLACE))) {
      // Failed to upload the file. file_save_upload() calls form_set_error() on
      // failure.
      return;
    }
    $local_cache = $finfo->uri;

    $directory = _responsive_favicons_extract_directory();
    try {
      $archive = _responsive_favicons_archive_extract($local_cache, $directory);
    }
    catch (Exception $e) {
      form_set_error($field, $e->getMessage());
      return;
    }

    $files = $archive->listContents();
    if (!$files) {
      form_set_error($field, t('Provided archive contains no files.'));
      return;
    }

    // Create the destination directory.
    $destination = 'public://' . variable_get('responsive_favicons_path', 'favicons');
    file_prepare_directory($destination, FILE_CREATE_DIRECTORY);

    // Copy the files to the correct location.
    $success_count = 0;
    foreach ($files as $file) {
      $success = file_unmanaged_copy($directory . '/' . $file, $destination, FILE_EXISTS_REPLACE);
      $uri = $destination . '/' . $file;
      if ($success) {
        $success_count++;

        // Rewrite the paths of the JSON files.
        if (preg_match('/\.json$/', $file)) {
          $file_contents = file_get_contents(drupal_realpath($uri));
          $find = preg_quote('"\/android-chrome', '/');
          $replace = '"' . str_replace('/', '\/', _responsive_favicons_normalise_path('/android-chrome'));
          $file_contents = preg_replace('/' . $find . '/', $replace, $file_contents);
          file_unmanaged_save_data($file_contents, $uri, FILE_EXISTS_REPLACE);
        }
        // Rewrite the paths of the XML files.
        else if (preg_match('/\.xml$/', $file)) {
          $file_contents = file_get_contents(drupal_realpath($uri));
          $find = preg_quote('"/mstile', '/');
          $replace = '"' . _responsive_favicons_normalise_path('/mstile');
          $file_contents = preg_replace('/' . $find . '/', $replace, $file_contents);
          file_unmanaged_save_data($file_contents, $uri, FILE_EXISTS_REPLACE);
        }
      }
    }

    if ($success_count > 0) {
      drupal_set_message(format_plural($success_count, 'Uploaded 1 favicon successfully.', 'Uploaded @count favicons successfully.'));
    }
  }
}

/**
 * Returns a short unique identifier for this Drupal installation.
 *
 * @return
 *   An eight character string uniquely identifying this Drupal installation.
 */
function _responsive_favicons_unique_identifier() {
  $id = &drupal_static(__FUNCTION__, '');
  if (empty($id)) {
    $id = substr(hash('sha256', drupal_get_hash_salt()), 0, 8);
  }
  return $id;
}

/**
 * Returns the directory where update archive files should be extracted.
 *
 * @param $create
 *   (optional) Whether to attempt to create the directory if it does not
 *   already exist. Defaults to TRUE.
 *
 * @return
 *   The full path to the temporary directory where update file archives should
 *   be extracted.
 */
function _responsive_favicons_extract_directory($create = TRUE) {
  $directory = &drupal_static(__FUNCTION__, '');
  if (empty($directory)) {
    $directory = 'temporary://responsive-favicons .' . _responsive_favicons_unique_identifier();
    if ($create && !file_exists($directory)) {
      mkdir($directory);
    }
  }
  return $directory;
}

/**
 * Unpacks a downloaded archive file.
 *
 * @param string $file
 *   The filename of the archive you wish to extract.
 * @param string $directory
 *   The directory you wish to extract the archive into.
 *
 * @return Archiver
 *   The Archiver object used to extract the archive.
 *
 * @throws Exception
 */
function _responsive_favicons_archive_extract($file, $directory) {
  $archiver = archiver_get_archiver($file);
  if (!$archiver) {
    throw new Exception(t('Cannot extract %file, not a valid archive.', array ('%file' => $file)));
  }

  if (file_exists($directory)) {
    file_unmanaged_delete_recursive($directory);
  }

  $archiver->extract($directory);
  return $archiver;
}

<?php

/**
 * @file
 * Common functionality for entermedia file handling CCK field modules.
 */

/**
 * The #value_callback for a managed_file type element.
 */
function embridge_field_managed_file_value(&$element, $input = FALSE, $form_state = NULL, $instance = array()) {
  $aid = 0;
  $catalog_id = $instance['settings']['asset_catalog'];

  // Find the current value of this field from the form state.
  $form_state_aid = $form_state['values'];
  foreach ($element['#parents'] as $parent) {
    $form_state_aid = isset($form_state_aid[$parent]) ? $form_state_aid[$parent] : 0;
  }

  if ($element['#extended'] && isset($form_state_aid['aid'])) {
    $aid = $form_state_aid['aid'];
  }
  elseif (is_numeric($form_state_aid)) {
    $aid = $form_state_aid;
  }

  // Process any input and save new uploads.
  if ($input !== FALSE) {
    $return = $input;

    // Uploads take priority over all other values.
    if ($file = embridge_field_managed_file_save_upload($element, $catalog_id, $instance)) {
      $aid = $file->aid;
    }
    else {
      // Check for #filefield_value_callback values.
      // Because FAPI does not allow multiple #value_callback values like it
      // does for #element_validate and #process, this fills the missing
      // functionality to allow File fields to be extended through FAPI.
      if (isset($element['#file_value_callbacks'])) {
        foreach ($element['#file_value_callbacks'] as $callback) {
          $callback($element, $input, $form_state);
        }
      }
      // Load file if the FID has changed to confirm it exists.
      if (isset($input['aid']) && $file = embridge_field_asset_load($input['aid'])) {
        $aid = $file->aid;

        // Attach new thumbail to the existing asset.
        $source = implode('_', $element['#parents']);
        if (!empty($_FILES['thumbnails']['name'][$source])) {
          embridge_field_managed_file_attach_thumbnail($aid, $element, $catalog_id, $instance);
        }
      }
    }
  }
  else {
    // If there is no input, set the default value.
    if ($element['#extended']) {
      $default_aid = isset($element['#default_value']['aid']) ? $element['#default_value']['aid'] : 0;
      $return = isset($element['#default_value']) ? $element['#default_value'] : array('aid' => 0);
    }
    else {
      $default_aid = isset($element['#default_value']) ? $element['#default_value'] : 0;
      $return = array('aid' => 0);
    }

    // Confirm that the file exists when used as a default value.
    if ($default_aid && $file = embridge_field_asset_load($default_aid, $catalog_id)) {
      $aid = $file->aid;
    }
  }
  $return['filesize'] = empty($file->filesize) ? 0 : $file->filesize;
  $return['filename'] = empty($file->sourcepath) ? '' : _embridge_get_file_name_w_extension($file->sourcepath);

  $return['aid'] = $aid;

  return $return;
}

/**
 * Given a managed_file element, save any files that have been uploaded into it.
 *
 * @param array $element
 *   The FAPI element whose values are being saved.
 * @param string $catalog_id
 *   Catalog ID.
 * @param array $instance
 *   Instance array.
 *
 * @return Object
 *   The file object representing the file that was saved, or FALSE if no file
 *   was saved.
 */
function embridge_field_managed_file_save_upload($element, $catalog_id, $instance) {
  global $user;
  $source = implode('_', $element['#parents']);
  if (empty($_FILES['files']['name'][$source])) {
    return FALSE;
  }

  $validators = $element['#upload_validators'];
  // Check for file upload errors and return FALSE if a lower level system
  // error occurred. For a complete list of errors:
  // See http://php.net/manual/en/features.file-upload.errors.php.
  switch ($_FILES['files']['error'][$source]) {
    case UPLOAD_ERR_INI_SIZE:
    case UPLOAD_ERR_FORM_SIZE:
      drupal_set_message(t('The file %file could not be saved, because it exceeds %maxsize, the maximum allowed size for uploads.', array('%file' => $_FILES['files']['name'][$source], '%maxsize' => format_size(file_upload_max_size()))), 'error');
      return FALSE;

    case UPLOAD_ERR_PARTIAL:
    case UPLOAD_ERR_NO_FILE:
      drupal_set_message(t('The file %file could not be saved, because the upload did not complete.', array('%file' => $_FILES['files']['name'][$source])), 'error');
      return FALSE;

    case UPLOAD_ERR_OK:
      // Final check that this is a valid upload, if it isn't, use the
      // default error handler.
      if (is_uploaded_file($_FILES['files']['tmp_name'][$source])) {
        break;
      }
    default:
      // Unknown error.
      drupal_set_message(t('The file %file could not be saved. An unknown error has occurred.', array('%file' => $_FILES['files']['name'][$source])), 'error');
      return FALSE;
  }

  // Begin building file object.
  $file = new stdClass();
  $file->uid      = $user->uid;
  $file->status   = 0;
  $file->filename = trim(basename($_FILES['files']['name'][$source]), '.');
  $file->uri      = $_FILES['files']['tmp_name'][$source];
  $file->filemime = file_get_mimetype($file->filename);
  $file->filesize = $_FILES['files']['size'][$source];

  $extensions = '';
  if (isset($validators['file_validate_extensions'])) {
    if (isset($validators['file_validate_extensions'][0])) {
      // Build the list of non-munged extensions if the caller provided them.
      $extensions = $validators['file_validate_extensions'][0];
    }
    else {
      // If 'file_validate_extensions' is set and the list is empty then the
      // caller wants to allow any extension. In this case we have to remove
      // the validator or else it will reject all extensions.
      unset($validators['file_validate_extensions']);
    }
  }
  else {
    // No validator was provided, so add one using the default list.
    // Build a default non-munged safe list for file_munge_filename().
    $extensions = 'jpg jpeg gif png txt doc xls pdf ppt pps odt ods odp';
    $validators['file_validate_extensions'] = array();
    $validators['file_validate_extensions'][0] = $extensions;
  }

  if (!empty($extensions)) {
    // Munge the filename to protect against possible malicious extension
    // hiding within an unknown file type (ie: filename.html.foo).
    $file->filename = file_munge_filename($file->filename, $extensions);
  }

  // Rename potentially executable files, to help prevent exploits (i.e. will
  // rename filename.php.foo and filename.php to filename.php.foo.txt and
  // filename.php.txt, respectively). Don't rename if 'allow_insecure_uploads'
  // evaluates to TRUE.
  if (!variable_get('allow_insecure_uploads', 0) && preg_match('/\.(php|pl|py|cgi|asp|js)(\.|$)/i', $file->filename) && (drupal_substr($file->filename, -4) != '.txt')) {
    $file->filemime = 'text/plain';
    $file->uri .= '.txt';
    $file->filename .= '.txt';
    // The .txt extension may not be in the allowed list of extensions. We
    // have to add it here or else the file upload will fail.
    if (!empty($extensions)) {
      $validators['file_validate_extensions'][0] .= ' txt';
      drupal_set_message(t('For security reasons, your upload has been renamed to %filename.', array('%filename' => $file->filename)));
    }
  }

  // Add in our check of the the file name length.
  $validators['file_validate_name_length'] = array();

  // Call the validation functions specified by this function's caller.
  $errors = file_validate($file, $validators);

  // Check for errors.
  if (!empty($errors)) {
    $message = t('The specified file %name could not be uploaded.', array('%name' => $file->filename));
    if (count($errors) > 1) {
      $message .= theme('item_list', array('items' => $errors));
    }
    else {
      $message .= ' ' . array_pop($errors);
    }
    form_set_error($source, $message);
    return FALSE;
  }

  $catalogs = variable_get('embridge_catalogs', array());
  $catalog_name = $catalogs[$catalog_id];
  $save_path = getcwd() . '/' . variable_get('file_public_path', conf_path() . '/files') . '/';
  $cust_file_name = _embridge_get_file_name($_FILES['files']['name'][$source]);
  $fname = _embridge_get_friendly_name($cust_file_name) . '-' . round(_embridge_microtime_float());
  $file_name = $fname . '.' . _embridge_get_file_extension($_FILES['files']['name'][$source]);

  if ($_FILES['files']['tmp_name'][$source]) {
    $upload_file = $_FILES['files']['tmp_name'][$source];
    if (!@move_uploaded_file($upload_file, $save_path . $file_name)) {
      if (!file_exists($upload_file)) {
        watchdog('embridge_field', 'The file %file does not exist.', array('%file' => $upload_file), WATCHDOG_ERROR);
      }
      if (!is_dir($save_path)) {
        watchdog('embridge_field', 'The directory %directory does not exist.', array('%directory' => $save_path), WATCHDOG_ERROR);
      }
      form_set_error($source, t('File upload error. Could not move uploaded file.'));
      return FALSE;
    }
  }

  // Attach thumbnail.
  if (isset($_FILES['thumbnails']) && $_FILES['thumbnails']['name'][$source]) {
    $thumbnail_file_name = $fname . '.' . _embridge_get_file_extension($_FILES['thumbnails']['name'][$source]);
    if (!@move_uploaded_file($_FILES['thumbnails']['tmp_name'][$source], $save_path . $thumbnail_file_name)) {
      if (!file_exists($_FILES['thumbnails']['tmp_name'][$source])) {
        watchdog('embridge_field', 'The thumbnail %file does not exist.', array('%file' => $_FILES['thumbnails']['name'][$source]), WATCHDOG_ERROR);
      }
      if (!is_dir($save_path)) {
        watchdog('embridge_field', 'The directory %directory does not exist.', array('%directory' => $save_path), WATCHDOG_ERROR);
      }
      form_set_error($source, t('Thumbnail upload error. Could not move uploaded thumbnail.'));
    }
    else {
      $attachment = TRUE;
    }
    unset($_FILES['thumbnails']);
  }

  $asset_path = trim($instance['settings']['asset_path']);
  $asset_path = token_replace($asset_path, array('dam' => NULL));
  $asset_directory = $asset_path;

  // Source path will be returned from method.
  $source_path = '';
  $response = _embridge_upload_file($save_path . $file_name, $asset_directory, $catalog_id, $source_path);
  $xmlobj = simplexml_load_string($response);
  $xmlarr = (array) $xmlobj;

  if (!empty($xmlarr['@attributes']['stat']) && $xmlarr['@attributes']['stat'] == 'ok') {
    $asset = (array) $xmlarr['asset'];
  }
  else {
    watchdog('embridge_field', 'Failed to upload EnterMedia asset %source - %message', array('%source' => $source_path, '%message' => $response), WATCHDOG_ERROR);
    form_set_error($source, t('Failed to upload EnterMedia asset - EnterMedia service error.'));
    return 0;
  }

  if (isset($attachment)) {
    $response = _embridge_upload_attachment($source_path, $catalog_id, $save_path . $thumbnail_file_name);
    $xmlobj = simplexml_load_string($response);
    $xmlarr = (array) $xmlobj;
    if (empty($xmlarr['@attributes']['stat']) || $xmlarr['@attributes']['stat'] != 'ok') {
      watchdog('embridge_field', 'Failed to upload atachment %source - %message', array('%source' => $source_path, '%message' => $response), WATCHDOG_ERROR);
      drupal_set_message(t('Failed to upload atachment - EnterMedia service error.'), 'error');
    }
  }

  $catalog_settings = _embridge_get_catalog_settings_by_name(drupal_strtolower($catalog_name));
  $xmlarr = _embridge_get_search_result(array(array('field' => 'id', 'value' => $asset['@attributes']['id'])), $catalog_settings);
  $xmlarr = (array) $xmlarr;
  if (!$xmlarr['hit']) {
    watchdog('embridge_field', 'Failed to get uploaded EnterMedia asset %source - %message', array('%source' => $source_path, '%message' => $response), WATCHDOG_ERROR);
    form_set_error($source, t('Failed to get uploaded EnterMedia asset - EnterMedia service error.'));
    return FALSE;
  }

  $asset = (array) $xmlarr['hit'];

  $server_url = variable_get('embridge_server_url', 'http://entermedia.databasepublish.com');
  $server_port = variable_get('embridge_server_port', '8080');

  $file = array();
  $file['aid'] = NULL;
  $file['asset_id'] = $asset['@attributes']['id'];
  $file['title'] = isset($asset['@attributes']['assettitle']) ? $asset['@attributes']['assettitle'] : $asset['@attributes']['name'];;
  $file['filesize'] = $asset['@attributes']['filesize'];
  $file['filemime'] = $asset['@attributes']['fileformat'];
  $file['thumbnail'] = $server_url . ':' . $server_port . $asset['thumb'];
  $file['preview'] = $server_url . ':' . $server_port . $asset['preview'];
  $file['width'] = isset($asset['@attributes']['width']) ? $asset['@attributes']['width'] : 0;
  $file['height'] = isset($asset['@attributes']['height']) ? $asset['@attributes']['height'] : 0;
  $file['embedcode'] = isset($embedcode) ? $embedcode : '';
  $file['sourcepath'] = $source_path;
  // Get all renditions for asset.
  _embridge_generate_renditions($asset['@attributes']['id'], $catalog_id, $asset['@attributes']['sourcepath']);
  $renditions = _embridge_get_rendition_list($asset['@attributes']['id'], $catalog_id, $asset['@attributes']['sourcepath'], $asset['@attributes']['fileformat'], TRUE);
  if (!empty($renditions['thumb'])) {
    $file['thumbnail'] = $renditions['thumb'];
  }
  
  if (!empty($renditions['medium'])) {
    $file['preview'] = $renditions['medium'];
  }

  // Adding entermedia asset data to entermedia table.
  $file['aid'] = _embridge_asset_insert($file, $catalog_id);

  return (object) $file;
}

/**
 * Save a file upload to entermedia.
 */
function embridge_field_managed_file_attach_thumbnail($aid, $element, $catalog_id, $instance) {
  $source = implode('_', $element['#parents']);
  if ($aid) {
    // Get asset data from entermedia table.
    $asset = _embridge_get_asset_array($aid);
    if (!empty($asset)) {
      $source_path = $asset['sourcepath'];
    }
  }
  $save_path = getcwd() . '/' . variable_get('file_public_path', conf_path() . '/files') . '/';
  $fname = round(_embridge_microtime_float());

  // Attach thumbnail.
  if ($_FILES['thumbnails']['name'][$source]) {
    $thumbnail_file_name = $fname . '.' . _embridge_get_file_extension($_FILES['thumbnails']['name'][$source]);
    if (!@move_uploaded_file($_FILES['thumbnails']['tmp_name'][$source], $save_path . $thumbnail_file_name)) {
      if (!file_exists($_FILES['thumbnails']['tmp_name'][$source])) {
        watchdog('embridge_field', 'The thumbnail %file does not exist.', array('%file' => $_FILES['thumbnails']['name'][$source]), WATCHDOG_ERROR);
      }
      if (!is_dir($save_path)) {
        watchdog('embridge_field', 'The directory %directory does not exist.', array('%directory' => $save_path), WATCHDOG_ERROR);
      }
      form_set_error($source, t('Thumbnail upload error. Could not move uploaded thumbnail.'));
    }
    else {
      $response = _embridge_upload_attachment($source_path, $catalog_id, $save_path . $thumbnail_file_name);
      $xmlobj = simplexml_load_string($response);
      $xmlarr = (array) $xmlobj;
      if (empty($xmlarr['@attributes']['stat']) || $xmlarr['@attributes']['stat'] != 'ok') {
        watchdog('embridge_field', 'Failed to upload atachment %source - %message', array('%source' => $source_path, '%message' => $response), WATCHDOG_ERROR);
        form_set_error($source, t('Failed to upload atachment - EnterMedia service error.'));
      }
    }
    unset($_FILES['thumbnails']);
  }
}

/**
 * Save embed code to entermedia.
 */
function embridge_field_managed_file_update_embed_code($aid, $catalog_id, $embed_code) {
  $asset = _embridge_get_asset_array($aid);
  if (!empty($asset) && $asset['embedcode'] != $embed_code) {
    $fields_values = array();
    $fields_values['embedcode'] = $embed_code;
    // Update default metadata to entermedia asset.
    _embridge_update_asset($asset['asset_id'], $catalog_id, $fields_values);
    $num_updated = db_update('entermedia')
      ->fields(array(
        'embedcode' => $embed_code,
      ))
      ->condition('aid', $aid)
      ->execute();
  }
}

/**
 * Load an asset object from the database.
 */
function embridge_field_asset_load($aid) {
  if ($aid) {
    // Get asset data from entermedia table.
    return _embridge_get_asset_object($aid);
  }
}

/**
 * Load multiple asset objects from the database.
 */
function embridge_field_asset_load_multiple($aids = array()) {
  $assets = array();
  foreach ($aids as $aid) {
    $asset = embridge_field_asset_load($aid);
    $assets[$asset->aid] = $asset;
  }
  return $assets;
}

/**
 * Get the upload validators for a file field.
 *
 * @param array $field
 *   A field array.
 * @param array $instance
 *   An instance array.
 *
 * @return array
 *   An array suitable for passing to file_save_upload() or the file field
 *   element's '#upload_validators' property.
 */
function embridge_field_widget_upload_validators($field, $instance) {
  // Cap the upload size according to the PHP limit.
  $max_filesize = parse_size(file_upload_max_size());
  if (!empty($instance['settings']['max_filesize']) && parse_size($instance['settings']['max_filesize']) < $max_filesize) {
    $max_filesize = parse_size($instance['settings']['max_filesize']);
  }

  $validators = array();

  // There is always a file size limit due to the PHP server limit.
  $validators['file_validate_size'] = array($max_filesize);

  // Add the extension check if necessary.
  if (!empty($instance['settings']['file_extensions'])) {
    $validators['file_validate_extensions'] = array($instance['settings']['file_extensions']);
  }
    
  // Add min filesize check if necessary.
  if (!empty($instance['settings']['min_filesize'])) {
    $validators['embridge_field_file_validate_min_filesize'] = array(parse_size($instance['settings']['min_filesize']));
  }

  return $validators;
}

/**
 * Determine the URI for a file field instance.
 *
 * @param array $field
 *   A field array.
 * @param array $instance
 *   A field instance array.
 * @param array $account
 *   An account array.
 *
 * @return string
 *   A file directory URI with tokens replaced.
 */
function embridge_field_widget_uri($field, $instance, $account = NULL) {
  $destination = trim($instance['settings']['file_directory'], '/');

  // Replace tokens.
  $data = array('user' => isset($account) ? $account : $GLOBALS['user']);
  $destination = token_replace($destination, $data);

  return $field['settings']['uri_scheme'] . '://' . $destination;
}

/**
 * Remove field.
 */
function embridge_field_delete(stdClass $file, $force = FALSE) {
  return TRUE;
}

/**
 * TODO: Add description.
 */
function embridge_field_usage_list(stdClass $file) {
  $references = array();
  return $references;
}

/**
 * Placeholder function.
 *
 * TODO: This is just placeholder. We can't remove file on EnterMedia server.
 *       This function should be removed or replaced with real function.
 * Remove file on the server.
 */
function embridge_field_delete_file($item, $field, $entity_type, $id, $count = 1) {
  return TRUE;
}

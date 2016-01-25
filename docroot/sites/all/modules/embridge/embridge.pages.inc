<?php

/**
 * @file
 * Defines page callbacks for embridge module.
 */
function embridge_asset_upload_form($form, $form_state, $catalog_name) {
  drupal_add_js(drupal_get_path('module', 'embridge') . '/js/embridge.js');
  drupal_add_css(drupal_get_path('module', 'embridge') . '/css/embridge.css');
  $form = array();
  $form_state['catalog'] = $catalog_name;
  if (!empty($form_state['asset'])) {
    $asset = $form_state['asset'];
    $image = theme('image',
      array(
            'path' => $asset['thumbnail'],
            'alt' => isset($asset['title']) ? $asset['title'] : '',
            'title' => isset($asset['title']) ? $asset['title'] : '',
            'attributes' => array(),
      )
    );

    $output = '<div class="uploaded-asset">';
    $output .= $image;
    $output .= '<div class="field-name">'. $asset['title'] . '</div>';
    $output .= '<input type="hidden" name="asset_id" value="' . $asset['asset_id'] . '" />';
    $output .= '<input type="hidden" name="aid" value="' . $asset['aid'] . '" />';
    $output .= '<input type="hidden" name="thumbnail" value="' . $asset['thumbnail'] . '" disabled="true"/>';
    $output .= '<input type="hidden" name="preview" value="' . $asset['renditions']['large'] . '" disabled="true"/>';
    
    if (!empty($asset['renditions'])) {
      foreach ($asset['renditions'] as $key => $value) {
        $output .= '<input type="hidden" name="' . $key . '" value="' . $value . '" disabled="true"/>';
      }
    }
    $output .= '</div>';
    $output .= '<div id="preview-dialog" style="display:none;"></div>';
    $form['upload_asset'] = array('#markup' => $output);
  }
  else {
    $form['upload_file'] = array(
      '#type' => 'file',
    );
    $form['upload'] = array(
      '#type' => 'submit',
      '#value' => t('Upload'),
      '#attributes' => array('class' => array('upload-action')),
      '#prefix' => '<div class="form-item form-item-files-upload-button">',
      '#suffix' => '</div>',
    );
  }
  // We will hide buttons if the search form is embed.
  if (isset($_REQUEST['embed'])) {
    $form['embed'] = array(
      '#type' => 'hidden',
      '#value' => 'true',
    );
    drupal_set_title('');
  }
  return $form;
}

function embridge_asset_upload_form_submit(&$form, &$form_state) {
  global $user;
  $source = 'upload_file';
  $catalog_name = $form_state['catalog'];
  $catalog_settings = _embridge_get_catalog_settings_by_name($catalog_name);
  $catalog_id = $catalog_settings['id'];
  
  if (empty($_FILES['files']['name'][$source])) {
    return FALSE;
  }

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

  // No validator was provided, so add one using the default list.
  // Build a default non-munged safe list for file_munge_filename().
  $extensions = 'jpg jpeg png';
  $validators['file_validate_extensions'] = array();
  $validators['file_validate_extensions'][0] = $extensions;
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

  // Get asset path patterns.
  $asset_path = variable_get('embridge_server_asset_path', '[dam:yyyy]/[dam:mm]');
  // Replace token in the path.
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
  $file['title'] = isset($asset['@attributes']['assettitle']) ? $asset['@attributes']['assettitle'] : $asset['@attributes']['name'];
  $file['filesize'] = $asset['@attributes']['filesize'];
  $file['filemime'] = $asset['@attributes']['fileformat'];
  $file['thumbnail'] = $server_url . ':' . $server_port . $asset['thumb'];
  $file['preview'] = $server_url . ':' . $server_port . $asset['preview'];
  $file['width'] = isset($asset['@attributes']['width']) ? $asset['@attributes']['width'] : 0;
  $file['height'] = isset($asset['@attributes']['height']) ? $asset['@attributes']['height'] : 0;
  $file['embedcode'] = isset($embedcode) ? $embedcode : '';
  $file['sourcepath'] = $source_path;
  // Get all renditions for asset.
  _embridge_generate_renditions($asset['@attributes']['id'], $catalog_id, $asset['@attributes']['sourcepath'], $asset['@attributes']['fileformat']);
  $renditions = _embridge_get_rendition_list($asset['@attributes']['id'], $catalog_id, $asset['@attributes']['sourcepath'], $asset['@attributes']['fileformat'], TRUE);
  $server_name = $server_url . ($server_port ? ':' . $server_port : '');
  $embridge_cloud_server_url = variable_get('embridge_cloud_server_url', '');
  $use_cloud_urls = variable_get('embridge_cloud_use_cloud_urls', FALSE);
  
  if (!empty($renditions)) {
    // Replace EnterMedia urls by Cloud urls.
    foreach ($renditions as $key => $value) {
      $file['renditions'][$key] = $use_cloud_urls ? str_replace($server_name, $embridge_cloud_server_url, $value) : $value;
    }

    if (!empty($renditions['thumb'])) {
      $file['thumbnail'] = $renditions['thumb'];
    }

    if (!empty($renditions['medium'])) {
      $file['preview'] = $renditions['medium'];
    }
  }

  // Adding entermedia asset data to entermedia table.
  $file['aid'] = _embridge_asset_insert($file, $catalog_id);
  $form_state['asset'] = $file;
  $form_state['rebuild'] = TRUE;
}

/**
 * Create EnterMedia search page for the current catalog.
 *
 * @param string $catalog_name
 *   The name for EnterMedia catalog. i.e photo, video.
 *
 * @return string
 *   Search form for the catalog specified.
 */
function embridge_search_asset($catalog_name) {
  // Add JS for entermedia.
  drupal_add_js(drupal_get_path('module', 'embridge') . '/js/embridge.js');
  drupal_add_css(drupal_get_path('module', 'embridge') . '/css/embridge.css');

  $catalog_settings = _embridge_get_catalog_settings_by_name($catalog_name);
  $search_result = FALSE;
  if (isset($_GET['ent_search']) || isset($_GET['hitssessionid'])) {
    $search_params = array();
    // Initial Search.
    if (isset($_GET['ent_search']) && !isset($_GET['hitssessionid'])) {
      foreach ($_GET['ent_search'] as $k => $v) {
        if (!empty($v)) {
          if ($k == 'assetmodificationdate') {

            $search_params[] = array(
              'field' => $k,
              'operation' => 'betweendates',
              'after' => (date('n/d/Y', strtotime('-' . drupal_substr($v, 6) . ' day'))),
              'before' => (date('n/d/Y', strtotime('+1 day'))),
            );
          }
          elseif ($k == 'description') {
            // Full-text search will use starts with operation.
            $search_params[] = array(
              'field' => $k,
              'operation' => 'matches',
              'value' => $v . '*',
            );
          }
          else {
            $search_params[] = array('field' => $k, 'value' => $v);
          }
        }
      }
      // Apply default search filters.
      if (!empty($catalog_settings['default_filters'])) {
        foreach ($catalog_settings['default_filters'] as $field_id => $field) {
          $search_params[] = array('field' => $field_id, 'value' => $field['value']);
        }
      }
      $search_result = _embridge_get_search_result($search_params, $catalog_settings);
    }
    elseif (isset($_GET['hitssessionid']) && isset($_GET['current_page'])) {
      // Next page search.
      $search_result = _embridge_get_search_page_result($catalog_settings, $_GET['hitssessionid'], $_GET['current_page']);
    }
  }
  $form_output = drupal_get_form('embridge_search_asset_form', $search_result, $catalog_settings);
  return $form_output;
}

/**
 * Menu callback page for media playing.
 *
 * @param string $catalog_name
 *   The name of current catalog.
 * @param int $aid
 *   The asset id for the EnterMedia asset.
 *
 * @return string
 *   Rendering html.
 */
function embridge_media_play($catalog_name, $aid) {
  $head = FALSE;
  // $catalog_settings = _embridge_get_catalog_settings_by_name($catalog_name);
  // $catalog_id = $catalog_settings['id'];
  $module_path = drupal_get_path('module', 'embridge');
  $head = '<link href="/' . $module_path . '/css/default.css" rel="stylesheet" type="text/css" />';
  $output = '';

  if ($aid) {
    // Get asset data from entermedia table.
    $asset = _embridge_get_asset_array($aid);
    if (!empty($asset) && !empty($asset['embedcode'])) {
      $output .= '<div class="entermedia-embedcode">' . $asset['embedcode'] . '</div>';
    }
  }

  if (empty($output)) {
    $output .= '<div class="empty-message">' . t('There is no valid embedcode to play for this asset.') . '</div>';
  }

  $output = theme('embridge_iframe', array('content' => $output, 'scripts' => $head));
  print $output;
  exit();
}

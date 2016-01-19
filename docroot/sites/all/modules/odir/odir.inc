<?php
/**
 * @file
 * Pages of odir module
 */

/**
 * Main odir-page listing images.
 */
function odir_filelist($path = '') {
  odir_current($path);
  $text = theme('odir_filelist_header', array('splitted_path' => odir_split_path($path)));
  $text .= _odir_render_file_output();
  _odir_js_set_filecounter();
  return $text;
}

/**
 * Ajax callback for deleting a file.
 */
function odir_ajax_file_removal() {
  $recursive = TRUE;
  $path = odir_decode($_POST['odir_path']);
  $uri = 'odir://' . $path;
  $force = TRUE;

  $file = odir_load_file($uri);

  if ($file) {
    file_delete($file, $force);
    $result = t('@file deleted (!fid)', array('@file' => $uri, '!fid' => $fid));
  }
  else {
    drupal_unlink($uri);
    $result = t('@file unlinked (!fid)', array('@file' => $uri, '!fid' => $fid));
  }

  print $result;
}

/**
 * Ajax callback for deleting a directory.
 */
function odir_ajax_directory_removal() {
  $path = odir_decode($_POST['odir_path']);
  $recursive = TRUE;
  $uri = 'odir://' . $path;
  $wrapper = new OdirStreamWrapper($uri);
  $path = $wrapper->getPublicLocalPath($uri);
  file_unmanaged_delete_recursive($path);
  db_delete('file_managed')
    ->condition(db_or()
        ->condition('uri', $uri . '%', 'LIKE')
      )
    ->execute();

  $result = t('@file rmdired', array('@file' => $uri));
  print $result;
}

/**
 * Ajax callback for creating a directory.
 */
function odir_ajax_create_directory($dir = "/") {
  $recursive = TRUE;
  odir_folder_filecounter_set($_POST['odir_div_filecounter']);
  $newdir = $_POST['odir_new_dir'];
  $path = odir_decode($dir) . '/' . $newdir;
  $uri = 'odir://' . $path;
  $mode = NULL;
  if (!file_prepare_directory($uri, FILE_CREATE_DIRECTORY)) {
    drupal_set_message(t("@dir could not be created!", array('@dir' => $uri)), 'error');
  }
  print theme('odir_folder_list_directory_listing_item', array(
                  'path' => odir_encode($path),
                  'filename' => $newdir,
                  'mayenter' => TRUE,
                  'filecounter' => odir_folder_filecounter(1),
                  'href' => url('dir/' . odir_encode($path)),
                )
              );
  _odir_js_set_filecounter();
}

/**
 * Ajax callback returning information of directory.
 */
function odir_ajax_check_file_info($dir = '/') {
  $dir = odir_decode($dir);
  $system_dir = odir_system_path($dir);

  $new_path = $system_dir . '/' . $_POST['odir_new_dir'] . '/';
  $result = array();
  if (file_exists($new_path)) {
    $result['file_exists'] = 1;
  }
  else {
    $result['file_exists'] = -1;
  }

  drupal_json_output($result);
}

/**
 * Callback for saving multiple files in iframe.
 * Javascript will be executed in parent frame.
 */
function odir_upload_files_in_iframe($relpath = '') {
  $res = _odir_upload_files_helper($relpath);
  if (is_array($res)) {
    $html  = '<script language="javascript" type="text/javascript">';
    $html .= 'var my_JSON_object = ' . drupal_json_encode($res) . ';';
    $html .= 'parent.odir.show_uploaded_files(my_JSON_object);';
    $html .= '</script>';
    print $html;
  }
}
/**
 * Callback for saving multiple files in iframe.
 */
function odir_upload_files_ajax($relpath = '') {
  $res = _odir_upload_files_helper($relpath);
  if (is_array($res)) {
    drupal_json_output($res);
  }
}

/**
 * Callback for saving multiple files in iframe.
 */
function _odir_upload_files_helper($relpath = '') {
  global $user;
  $html = array();
  $html['main'] = '';
  $html['block'] = '';
  $upload_validators = array('file_validate_extensions' => array()); /* TODO */
  $relpath = odir_decode($relpath);
  $file_names = $_FILES['files']['name'];
  $destination_folder = 'odir://' . $relpath;
  $singlefilemode = FALSE;

  if (!is_array($file_names)) {
    $singlefilemode = TRUE;
    $file_names = array($file_names);
  }

  $i = 0;
  $uploaded = 0;
  foreach ($file_names as $file_name) {
    if ($singlefilemode) {
      $_FILES['files']['error'] = array($file_name => $_FILES['files']['error']);
      $_FILES['files']['name'] = array($file_name => $_FILES['files']['name']);
      $_FILES['files']['tmp_name'] = array($file_name => $_FILES['files']['tmp_name']);
      $_FILES['files']['size'] = array($file_name => $_FILES['files']['size']);
    }

    if (!file_prepare_directory($destination_folder, FILE_CREATE_DIRECTORY)) {
      drupal_set_message(t("Directory @uri could not be created!", array('@uri' => $destination_folder)), 'error');
      drupal_add_http_header('Content-Typs:', 'text/html; charset: utf-8');
      watchdog('file', 'The upload directory %directory could not be created or is not accessible, ' .
                'so a newly uploaded file could not be saved to this directory, ' .
                'and the upload was canceled.', array('%directory' => $relpath));

      $html_error  = '<script language="javascript" type="text/javascript">';
      $html_error .= 'alert(\'' . t('The upload directory @relpath could not be created!', array('@relpath' => $destination_folder)) . '\');';
      $html_error .= 'window.top.window.odir_uploadFinished("-20")';
      $html_error .= '</script>';
      print $html_error;
      return;
    }

    if ($singlefilemode) {
      $source = $file_name;
    }
    else {
      $source = $i;
    }
    $file = file_save_upload($source, $upload_validators, $destination_folder);
    $file = odir_save_new_file_record($destination_folder, $file_name);
    if (!$file) {
      drupal_add_http_header('Content-Typs:', 'text/html; charset: utf-8');
      watchdog('file', 'The file upload failed. %upload', array('%upload' => $file_name));

      $html_error  = '<script language="javascript" type="text/javascript">';
      $html_error .= 'alert(\'' . t('A file was unable to be uploaded.') . '\');';
      $html_error .= 'window.top.window.odir_uploadFinished("-10")';
      $html_error .= '</script>';
      print $html_error;
      return;
    }
    $i++;

    if (preg_match(variable_get('odir_img_filter_regex', ODIR_IMG_FILTER_REGEX), $file_name)) {
      $html["main"] .= _odir_filelist_details_file("main", $relpath, $file_name);
    }
    else {
      $html["block"] .= _odir_filelist_details_file("block", $relpath, $file_name);
    }
  }
  $html["filecounter"] = odir_folder_filecounter();
  return $html;
}

/**
 * Sets a JS file counter for Ajax applications
 * The variable may be used with Drupal.settings.odir.odir_div_filecounter
 */
function _odir_js_set_filecounter() {
  drupal_add_js(array('odir' => array("odir_div_filecounter" => odir_folder_filecounter())), 'setting');
}

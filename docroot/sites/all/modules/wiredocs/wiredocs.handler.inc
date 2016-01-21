<?php 
/**
 * @file wiredocs.handler.inc
 * Handlers for menu callbacks.
 */

/**
 * Download a file.
 * @param $file
 *   file object
 */
function wiredocs_download($file) {
  // set correct HTTP download headers
  $headers = array(
    'Content-Type'              => 'force-download',
    'Content-Disposition'       => 'attachment; filename="' . $file->filename . '"',
    'Content-Length'            => $file->filesize,
    'Content-Transfer-Encoding' => 'binary',
    'Pragma'                    => 'no-cache',
    'Cache-Control'             => 'must-revalidate, post-check=0, pre-check=0',
    'Expires'                   => '0',
    'Accept-Ranges'             => 'bytes'
  );

  file_transfer($file->uri, $headers);
}

 /**
 * Post (upload) a file.
 * @TODO: We may want to support revisions by invoking a hook for client modules.
 * @param $file
 *   file object
 */
function wiredocs_upload($file) {
  global $user;  
  
  // get uploaded file
  $tmp_file_name = $_FILES['wiredocs-file']['tmp_name'];
 
  // Save the new file back to its uri.
  // Function drupal_move_uploaded_file() has been introduced in Drupal version 7.8, that's
  // why we want to provide backwards compatiblity here.
  $success = FALSE;
  if (function_exists('drupal_move_uploaded_file')) {
    $success = drupal_move_uploaded_file($tmp_file_name, $file->uri);
  }
  else {
    $success = move_uploaded_file($tmp_file_name, $file->uri);
  }
  file_save($file); // update file info in database, e. g. file size
  // Return a message
  if ($success) {
    echo t('File %filename was uploaded successfully!', array('%filename' => $file->filename));
  }
  else { // Indicate a server side error
    drupal_add_http_header("Status", "500 Internal Server Error");
    echo t('File %filename couldn\'t be updated!', array('%filename' => $file->filename));
  }
}

/**
 * Callback for downloading a Jar archive.
 * 
 * @param $filename
 *   Name of the Jar file without extension
 */
function wiredocs_get_jar($filename) {
  //retrieve jar file in jar folder
  $filename = drupal_get_path('module', 'wiredocs') . '/jar/' . $filename . '.jar';
  
  // read and output the file
  if (file_exists($filename) && readfile($filename)) {    
    // set http headers
    $headers = array(
      'Content-Type'              => 'force-download',
      'Content-Disposition'       => 'attachment; filename="' . $filename . '"',
      'Content-Transfer-Encoding' => 'binary',
      'Pragma'                    => 'no-cache',
      'Cache-Control'             => 'must-revalidate, post-check=0, pre-check=0',
      'Expires'                   => '0',
      'Accept-Ranges'             => 'bytes'
    );
    foreach ($headers as $name => $value) {
      drupal_add_http_header($name, $value);
    }
  }    
  else { // If error occurs, return 404
    drupal_not_found();
  }
}
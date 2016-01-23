<?php
function imageeditor_inline_save() {
  $alert = '';
  if (!_imageeditor_inline_access_check($_POST['fullurl'])) {
    $alert .= 'You don\'t have permission to edit this image.' . "\n";
    drupal_json_output($alert);
  }
  else {

  $ds = file_default_scheme();
  // Save backup to default stream wrapper imageeditor/backup.
  $directory = $ds. '://imageeditor/backup';

  // Original image was from this site.
  if ($fullurl = imageeditor_inline_urltouri($_POST['fullurl'])) {
    $local = TRUE;
    $alert .= 'Original image was from this website.' . "\n";
    $directory .= '/' . dirname(file_uri_target($fullurl));
    if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
      watchdog('imageeditor_inline', 'Directory %directory for image backups could not be created.', array('%directory' => $directory), WATCHDOG_WARNING);
    }
    elseif ($backup = file_unmanaged_copy($fullurl, $directory)) {
      watchdog('imageeditor_inline', 'Backed up original image %fullurl to %backup.', array('%fullurl' => $fullurl, '%backup' => $backup), WATCHDOG_INFO);
      $alert .= 'Original image was backed up.' . "\n";
    }
    else {
      watchdog('imageeditor_inline', 'Could not back up original image %fullurl to %directory.', array('%fullurl' => $fullurl, '%directory' => $directory), WATCHDOG_WARNING);
      $alert .= 'Original image backup couldn\'t be created.' . "\n";
    }
  }
  // Original image was from external site.
  else {
    $local = FALSE;
    $alert .= 'Original image was from external website.' . "\n";
    $url_info = parse_url($_POST['fullurl']);
    $directory .= '/' . $url_info['host'] . '/' . dirname($url_info['path']);
    if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
      watchdog('imageeditor_inline', 'Directory %directory for image backups could not be created.', array('%directory' => $directory), WATCHDOG_WARNING);
    }
    elseif ($data = imageeditor_inline_save_file_get_contents($_POST['fullurl'])) {
      if ($backup = file_unmanaged_save_data($data, $directory . '/' . basename($_POST['fullurl']))) {
        watchdog('imageeditor_inline', 'Backed up original image %fullurl to %backup.', array('%fullurl' => $_POST['fullurl'], '%backup' => $backup), WATCHDOG_INFO);
        $alert .= 'Original image was backed up.' . "\n";
      }
      else {
        watchdog('imageeditor_inline', 'Could not back up original image %fullurl to %directory.', array('%fullurl' => $_POST['fullurl'], '%directory' => $directory), WATCHDOG_WARNING);
        $alert .= 'Original image backup couldn\'t be created.' . "\n";
      }
    }
    else {
      watchdog('imageeditor_inline', 'Could not get image from %url.', array('%url' => $_POST['fullurl']), WATCHDOG_WARNING);
      $alert .= 'Original image backup couldn\'t be created.' . "\n";
    }
  }

  // Replace original image with edited image.
  if ($image = imageeditor_inline_urltouri($_POST['image'])) { //edited image is from this site
    $alert .= 'Edited image comes from this website.' . "\n";
    if ($local) { //original image was from local site - rewrite it using the edited image
      if ($file = file_unmanaged_move($image, $fullurl, FILE_EXISTS_REPLACE)) {
        watchdog('imageeditor_inline', 'Saved edited image %image to %file.', array('%image' => $image, '%file' => $file), WATCHDOG_INFO);
        $alert .= 'Original image was successfully replaced by edited image.' . "\n";
      }
      else {
        watchdog('imageeditor_inline', 'Could not save edited image %image to %fullurl.', array('%image' => $image, '%fullurl' => $fullurl), WATCHDOG_WARNING);
        $alert .= 'Original image couldn\'t be replaced by edited image.' . "\n";
      }
    }
    else { //original image was from external site - save edited image to imageeditor/external
      $directory = $ds . '://imageeditor/external';
      if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
        watchdog('imageeditor_inline', 'Directory %directory could not be created.', array('%directory' => $directory), WATCHDOG_WARNING);
      }
      elseif ($file = file_unmanaged_move($image, $directory . '/' . basename($image), FILE_EXISTS_RENAME)) {
        watchdog('imageeditor_inline', 'Saved edited image %image to %file.', array('%image' => $image, '%file' => $file), WATCHDOG_INFO);
        $alert .= 'Edited image was successfully saved.' . "\n";
      }
      else {
        watchdog('imageeditor_inline', 'Could not save edited image %image to %directory.', array('%image' => $image, '%directory' => $directory . '/' . basename($image)), WATCHDOG_WARNING);
        $alert .= 'Edited image couldn\'t be saved.' . "\n";
      }
    }
  }
  else { //edited image is from external site
    $alert .= 'Edited image comes from external website.' . "\n";
    if ($data = imageeditor_inline_save_file_get_contents($_POST['image'])) {
      if ($local) { //original image was from local site - rewrite it using the edited image
        if ($file = file_unmanaged_save_data($data, $fullurl, FILE_EXISTS_REPLACE)) {
          watchdog('imageeditor_inline', 'Saved edited image %image to %file.', array('%image' => $_POST['image'], '%file' => $file), WATCHDOG_INFO);
          $alert .= 'Original image was successfully replaced by edited image.' . "\n";
        }
        else {
          watchdog('imageeditor_inline', 'Could not save edited image %image to %fullurl.', array('%image' => $_POST['image'], '%fullurl' => $fullurl), WATCHDOG_WARNING);
          $alert .= 'Original image couldn\'t be replaced by edited image.' . "\n";
        }
      }
      else { //original image was from external site - save edited image to imageeditor/external
        $directory = $ds . '://imageeditor/external';
        if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
          watchdog('imageeditor_inline', 'Directory %directory could not be created.', array('%directory' => $directory), WATCHDOG_WARNING);
        }
        elseif ($file = file_unmanaged_save_data($data, $directory . '/' . basename($_POST['image']), FILE_EXISTS_RENAME)) {
          watchdog('imageeditor_inline', 'Saved edited image %image to %file.', array('%image' => $_POST['image'], '%file' => $file), WATCHDOG_INFO);
          $alert .= 'Edited image was successfully saved.' . "\n";
        }
        else {
          watchdog('imageeditor_inline', 'Could not save edited image %image to %directory.', array('%image' => $_POST['image'], '%directory' => $directory . '/' . basename($_POST['image'])), WATCHDOG_WARNING);
          $alert .= 'Edited image couldn\'t be saved.' . "\n";
        }
      }
    }
    else {
      watchdog('imageeditor_inline', 'Could not get image from %url.', array('%url' => $_POST['image']), WATCHDOG_WARNING);
    }
  }

  // Clear styles cache and update files_managed DB table if original image was local one.
  if ($local) {
    // Clear styles cache.
    // TODO - check if it is symbolic link (collageformatter).
    image_path_flush($fullurl);
    $alert .= 'Image styles of original image were flushed.' . "\n";

    // Check if this uri is registered in DB and update its record.
    $existing_files = file_load_multiple(array(), array('uri' => $fullurl));
    global $user;
    foreach ($existing_files as $file) {
      if (variable_get('imageeditor_inline_file_ownership', 0)) {
        $file->uid = $user->uid;
        $alert .= 'Image file ownership has been changed to you.' . "\n";
      }
      file_save($file);
      // TODO: update image dimensions in image fields - see file_entity module.
    }
  }

  drupal_json_output($alert);
  }
}

function imageeditor_inline_save_file_get_contents($url) {
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 15);
  // Causes a warning if PHP safe mode is on.
  @curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);

  // Verbose logging.
  /*curl_setopt($ch, CURLOPT_VERBOSE, true);
  $verbose = fopen('php://temp', 'rw+');
  curl_setopt($ch, CURLOPT_STDERR, $verbose);*/

  $data = curl_exec($ch);

  /*rewind($verbose);
  $verboseLog = stream_get_contents($verbose);*/

  curl_close($ch);

  return $data;
}

function imageeditor_inline_urltouri($url) {
  // Allow other modules to recognize url and provide uri.
  foreach (module_invoke_all('imageeditor_inline_urltouri', $url) as $value) {
    if ($value) {
      return rawurldecode($value);
    }
  }
}

/**
 * Implements hook_imageeditor_inline_urltouri().
 */
function imageeditor_inline_imageeditor_inline_urltouri($url) {
  global $base_url;
  $ds = file_default_scheme();
  $sw = file_stream_wrapper_get_instance_by_scheme($ds);
  // Check that url is from this site and from default stream wrapper.
  if (preg_match('`^' . preg_quote($base_url . '/' . $sw->getDirectoryPath() . '/') . '`', $url)) {
    $uri = $ds . '://' . preg_replace('`^' . preg_quote($base_url . '/' . $sw->getDirectoryPath() . '/') . '`', '', $url);
    return $uri;
  }
  else {
    return FALSE;
  }
}

function imageeditor_inline_access_check() {
  $access = array();
  foreach (explode(',', $_POST['images']) as $image) {
    $access[] = _imageeditor_inline_access_check($image);
  }
  drupal_json_output($access);
}

function _imageeditor_inline_access_check($image) {
  global $user;
  if (!variable_get('imageeditor_inline_access_check', 0)) {
    return TRUE;
  }
  else {
    if (user_access('edit all images')) {
      return TRUE;
    }
    elseif (user_access('edit own images')) {
      if ($fullurl = imageeditor_inline_urltouri($image)) {
        $existing_files = file_load_multiple(array(), array('uri' => $fullurl));
        foreach ($existing_files as $file) {
          if ($file->uid == $user->uid) {
            return TRUE;
          }
          else {
            return FALSE;
          }
        }
      }
      else {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }
}

function imageeditor_inline_revert() {
  if ($fullurl = imageeditor_inline_urltouri($_GET['image'])) {
    $ds = file_default_scheme();
    $pathinfo = pathinfo(file_uri_target($fullurl));
    $directory = $ds. '://imageeditor/backup' . '/' . $pathinfo['dirname'];
    $filename = substr($pathinfo['basename'], 0, strlen($pathinfo['basename']) - strlen($pathinfo['extension']) - 1);
    $mask = '/' . $filename . '(_[0-9]+)?\.' . $pathinfo['extension'] . '/';
    $result = array();
    foreach (file_scan_directory($directory, $mask) as $key => $file) {
      $result[] = theme('image_style', array('style_name' => variable_get('imageeditor_inline_revert_image_style', 'thumbnail'), 'path' => $file->uri, 'attributes' => array('class' => 'imageeditor-inline-revert')));
    }
    if (!empty($result)) {
      $output = 'Click on the image thumbnail to revert to it.';
      $output .= '<div class="imageeditor-inline-revert-images">';
      foreach ($result as $image) {
        $output .= $image;
      }
      $output .= '</div>';
      
      $path = drupal_get_path('module', 'imageeditor_inline');
      drupal_add_js($path . '/js/imageeditor_inline_revert.js');
      drupal_add_js(array('imageeditor_inline' => array('imageeditor_inline_revert' => $_GET['image'])), 'setting');
      drupal_add_css($path . '/css/imageeditor_inline.css');
      return $output;
    }
    else {
      return 'There are no image backups to revert to.';
    }
  }
  else {
    return 'This image is from external site.';
  }
}

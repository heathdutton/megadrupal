<?php
/**
 * @file
 * Utility and API functions for zoomify.module
 */

/**
 * Get image file information from given node
 * @param object $node: The node object
 * @return array: image information array in the form file ID => file path
 */
function _zoomify_images($node) {
  $images = array();
  foreach (module_implements('zoomify_images') as $module) {
    $these_images = module_invoke($module, 'zoomify_images', $node);
    if (!empty($these_images)) {
      foreach ($these_images as $fid => $filepath) {
        if ($fid && $filepath) {
          $images[$fid] = drupal_realpath($filepath);
        }
      }
    }
  }
  return $images;
}

/**
 * Helper function to define the Zoomify files basepath
 */
function _zoomify_basepath() {
  $basepath = variable_get('file_public_path', conf_path() . '/files') . '/zoomify';
  if (!file_prepare_directory($basepath, FILE_CREATE_DIRECTORY)) {
    drupal_set_message(t('Could not create path %path.', array('%path' => $basepath)), 'error');
    watchdog('zoomify', 'Could not create path %path.', array('%path' => $basepath), WATCHDOG_ERROR);
  }
  return $basepath;
}

/**
 * Helper function to retrieve the node specific Zoomify path
 */
function _zoomify_nodepath($node) {
  $path = _zoomify_basepath() . '/' . $node->nid;
  if (file_prepare_directory($path, FILE_CREATE_DIRECTORY)) {
    return $path;
  }
  return FALSE;
}

/**
 * Helper function to retrieve the file specific Zoomify path
 */
function _zoomify_filepath($node, $fid) {
  $path = _zoomify_nodepath($node) . '/' . $fid;
  if (file_prepare_directory($path, FILE_CREATE_DIRECTORY)) {
    return $path;
  }
  return FALSE;
}

/**
 * Check if a given fid path is identical to a given filepath
 * @param object $node: the node object to retrieve the first path from
 * @param int $fid: the file ID to retrieve the first path from
 * @param string $filepath: the second path
 * @return bool: TRUE if the first and second image have identical hashes, FALSE if not
 */
function _zoomify_identical($node, $fid, $filepath) {
  $md5 = md5_file($filepath);
  $md5_path = _zoomify_filepath($node, $fid) . '/Image.md5';
  if (!file_exists($md5_path)) {
    return FALSE;
  }
  $md5_existing = file_get_contents($md5_path);
  return $md5_existing == $md5;
}

/**
 * Process zoomify images when a node gets inserted
 * @param object $node: the node to retrieve the images from for processing
 */
function _zoomify_insert_node($node) {
  $images = _zoomify_images($node);
  if (!empty($images)) {
    foreach ($images as $fid => $filepath) {
      _zoomify_process($node, $fid, $filepath);
    }
  }
}

/**
 * Helper function to process zoomify files for added images
 * @param object $node: The node to process images from
 * @param int $fid: the file ID to process images from
 * @param string $filepath: the path of the image file to process
 */
function _zoomify_process($node, $fid, $filepath) {
  // @TODO: Let user adjust maximum time for scripts or provide better mechanism.
  drupal_set_time_limit(0);

  $info = image_get_info($filepath);
  if ($info === FALSE) {
    watchdog('zoomify', 'Could not find image info for file %path.', array('%path' => $filepath), WATCHDOG_ERROR);
    drupal_set_message(t('Could not find image info for file %path.', array('%path' => $filepath)), 'error');
    return;
  }
  if (($info['width'] < variable_get('zoomify_minimum_width', 1024))
    && ($info['height'] < variable_get('zoomify_minimum_height', 768))) {
    return;
  }

  $zoomify_nodepath = _zoomify_nodepath($node);
  if (!file_prepare_directory($zoomify_nodepath, FILE_CREATE_DIRECTORY)) {
    drupal_set_message(t('Could not create path %path.', array('%path' => _zoomify_nodepath($node))), 'error');
    watchdog('zoomify', 'Could not create path %path.', array('%path' => _zoomify_nodepath($node)), WATCHDOG_ERROR);
    return;
  }

  // Change $filepath variable from streamwrapper to real path
  $filepath = drupal_realpath($filepath);

  $backend = variable_get('zoomify_backend', ZOOMIFY_BACKEND_PHP);
  if ($backend == ZOOMIFY_BACKEND_MANUAL && !isset($node->zoomify[$fid]) && isset($node->zoomify['new'])) { // the new file is taken for the first unknown fid
    $node->zoomify[$fid] = $node->zoomify['new'];
    unset($node->zoomify['new']);
  }

  if ($backend == ZOOMIFY_BACKEND_PHP) {
    require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'zoomify') . '/ZoomifyFileProcessor.php';
    $zoomify = new ZoomifyFileProcessor();
    $zoomify->ZoomifyProcess($filepath);
    rename($zoomify->_v_saveToLocation, _zoomify_filepath($node, $fid));
  }
  elseif ($backend == ZOOMIFY_BACKEND_PYTHON) {
    $pathinfo = pathinfo_filename($filepath);
    $python_library_path = DRUPAL_ROOT . libraries_get_path('zoomify', TRUE) . '/ZoomifyImage';
    $cmd = 'cd ' . $pathinfo['dirname'] . ' && python ' . $python_library_path . '/ZoomifyFileProcessor.py' . ' "' . $pathinfo['basename'] . '"' . ' 2>&1';
    $return = 0;
    $output = array();
    exec($cmd, $output, $return);
    if ($return) {
      watchdog('zoomify', 'ZoomifyFileProcessor.py returned an error:<br />!output', array('!output' => implode('<br />', $output)), WATCHDOG_ERROR);
      drupal_set_message(t('ZoomifyFileProcessor.py returned an error:<br />!output', array('!output' => implode('<br />', $output))), 'error');
      return;
    }
    rename($pathinfo['dirname'] . '/' . $pathinfo['filename'], _zoomify_filepath($node, $fid));
  }
  elseif ($backend == ZOOMIFY_BACKEND_MANUAL && isset($node->zoomify[$fid]) ) {
    $zip = new ZipArchive;
    $res = $zip->open($node->zoomify[$fid]);
    if ($res !== TRUE) {
      watchdog('zoomify', 'Could not open Zip archive, error code: !output', array('!output' => $res), WATCHDOG_ERROR);
      drupal_set_message(t('Could not open Zip archive, error code: !output', array('!output' => $res)), 'error');
      return;
    }
    $root = $zip->locateName('ImageProperties.xml', ZIPARCHIVE::FL_NOCASE | ZIPARCHIVE::FL_NODIR);
    if ($root === FALSE) {
      watchdog('zoomify', 'Could not locate ImageProperties.xml in Zip archive.', array(), WATCHDOG_ERROR);
      drupal_set_message(t('Could not locate ImageProperties.xml in Zip archive.', array()), 'error');
      return;
    }
    $path = dirname($zip->getNameIndex($root));
    if ($path == '.') {
      $zip->extractTo(_zoomify_filepath($node, $fid));
    }
    else {
      $tmp = _zoomify_nodepath($node);
      $zip->extractTo($tmp);
      rename($tmp . '/' . $path, _zoomify_filepath($node, $fid));
      $path = dirname($path);
      if ($path != '.') {
        rrmdir($tmp . '/' . $path);
      }
    }
    $zip->close();
  }
  else {
    drupal_set_message(t('No tiles have been created.'));
    return;
  }

  $md5 = md5_file($filepath);
  $md5_path = _zoomify_filepath($node, $fid) . '/Image.md5';
  file_put_contents($md5_path, $md5);

  drupal_set_message(t('Created tiles for image %path.', array('%path' => $filepath)), 'status');
  watchdog('zoomify', 'Created tiles for image %path.', array('%path' => $filepath), WATCHDOG_NOTICE);
}

/**
 * Delete zoomify images when a node gets deleted
 * @param object $node: the node to delete the Zoomify images from
 */
function _zoomify_delete_node($node) {
  if (is_dir(_zoomify_nodepath($node))) {
    rrmdir(_zoomify_nodepath($node));
    watchdog('zoomify', 'Tiles deleted for node %nid.', array('%nid' => $node->nid), WATCHDOG_NOTICE);
    drupal_set_message(t('Tiles deleted for node %nid.', array('%nid' => $node->nid)), 'status');
  }
}

/**
 * Process zoomify images when a node gets updated
 * @param object $node: the node to retrieve the images from for processing
 */
function _zoomify_update_node($node) {
  $old_images = file_scan_directory(_zoomify_nodepath($node), '/.*/', array('nomask' => '/(\.|\.\.)/', 'recurse' => FALSE));
  $images = _zoomify_images($node);
  if (!empty($images)) {
    foreach ($images as $fid => $filepath) {
      if (!_zoomify_identical($node, $fid, $filepath) ||
        (variable_get('zoomify_backend', ZOOMIFY_BACKEND_PHP) == ZOOMIFY_BACKEND_MANUAL && isset($node->zoomify[$fid]))
      ) {
        rrmdir(_zoomify_filepath($node, $fid));
        _zoomify_process($node, $fid, $filepath);
      }
    }
  }
  if (!empty($old_images)) {
    foreach ($old_images as $old_image) {
      if (!array_key_exists($old_image->filename, $images) && is_dir($old_image->filename)) {
        rrmdir($old_image->filename);
        watchdog('zoomify', 'Tiles deleted for obsolete file %fid.', array('%fid' => $old_image->basename), WATCHDOG_NOTICE);
        drupal_set_message(t('Tiles deleted for obsolete file %fid.', array('%fid' => $old_image->basename)), 'status');
      }
    }
  }
}
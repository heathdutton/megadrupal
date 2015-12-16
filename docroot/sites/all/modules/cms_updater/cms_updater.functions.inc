<?php
/**
 * @file
 * Helper functions definition
 */


/**
 * Check if apache or the current php user can write to all
 * drupal files
 *
 * @param $src
 * @param array $exclude
 * @param array $include
 * @return mixed
 */
function _cms_updater_check_fileperms($src, $exclude = array(), $include = array()) {
  $source = _cms_updater_sanitizePath($src);
  $dir = opendir($src);

  static $retarray = array(
    'success'        => TRUE,
    'erronfiles'     => array(),
    'successonfiles' => array(),
  );

  while (FALSE !== ($file = readdir($dir))) {

    // Do only include provided files if set
    if (!empty($include) && !in_array($file, $include)) {
      continue;
    }
    // Exclude certain files
    if (!in_array($file, $exclude)) {
      if (($file != '.') && ($file != '..')) {
        if (is_dir($src . '/' . $file)) {
          $ret = _cms_updater_check_fileperms($source . '/' . $file);
          // If we receive some errors - stop and return
          if (sizeof($ret['erronfiles']) > 0) {
            closedir($dir);
            return $ret;
          }
        }
        else {
          $ecp = is_writable($source . '/' . $file);
          if ($ecp === FALSE) {
            // Try chmod:
            $orig_perms = _cms_updater_getFilePermission($source . '/' . $file);
            if (@chmod($source . '/' . $file, octdec('775'))) {
              $ecp = is_writable($source . '/' . $file);
              chmod($source . '/' . $file, octdec($orig_perms));
            }
          }
          $fileownerid = fileowner($source . '/' . $file);
          if (function_exists('posix_getpwuid')) {
            $fileownerinfo = posix_getpwuid($fileownerid);
          }
          else {
            // File owner of current script
            $fileownerinfo = array('name' => 'not determindable');
          }

          if ($ecp === FALSE) {
            $error_case_file_perms = _cms_updater_getFilePermission($source . '/' . $file);
            $retarray['success'] = FALSE;
            $retarray['erronfiles'][] = array(
              'filename'  => $source . '/' . $file,
              'perms'     => $error_case_file_perms,
              'fileowner' => $fileownerinfo['name'],
            );

          }
          else {
            $success_case_file_perms = _cms_updater_getFilePermission($source . '/' . $file);
            $retarray['successonfiles'][] = array(
              'filename'  => $source . '/' . $file,
              'perms'     => $success_case_file_perms,
              'fileowner' => $fileownerinfo['name'],
            );
          }
        }
      }
    }
  }
  closedir($dir);
  return $retarray;
}

/**
 * Get file permissions
 *
 * @param $file
 * @return string - last 3 diggits i.e. 777, 755
 */
function _cms_updater_getFilePermission($file) {
  $length = strlen(decoct(fileperms($file))) - 3;
  return substr(decoct(fileperms($file)), $length);
}

/**
 * Sanitize path
 *
 * @param $path
 * @return mixed|string
 */
function _cms_updater_sanitizePath($path) {
  $path = str_replace('\\', '/', $path); // Windows path sanitization.
  if (substr($path, -1) == '/') {
    $path = substr($path, 0, -1);
  }
  return $path;
}

/**
 * check if a writable temp-dir is determinable. this is needed for extracting
 * the downloaded new CMS/Drupal core files to.
 *
 * @return bool
 */
function _cms_updater_check_get_writable_temp_dir($doc_root) {
  $dir = file_directory_temp();
  if (file_exists($dir) && is_writable($dir)) {
    return TRUE;
  }
  elseif (file_exists($doc_root . '/' . $dir) && is_writable($doc_root . '/' . $dir)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * List some files
 *
 * @param $array
 * @return string
 */
function _cms_updater_files_array_to_list($array) {
  static $output = '</br>';
  static $i = 1;
  foreach ($array as $key => $item) {
    if (is_array($item)) {
      _cms_updater_files_array_to_list($item);
    }
    else {
      if (($key == 'filename') && ($i <= 10)) {
        $output .= $item . '</br>';
        $i++;
      }
    }
  }
  return $output;
}

/**
 * Recursive add files from source to destination zip Archive
 *
 * @param $src
 * @param $dst
 * @param $exclude
 */
function _cms_updater_recurse_zip($src, $dst, $exclude = array(), $include = array(), ZipArchive $zip) {
  if (!file_exists($dst)) {
    $zip->addEmptyDir($dst);
  }
  $dir = opendir($src);
  while (FALSE !== ($file = readdir($dir))) {
    // Do only include provided files if set
    if (!empty($include) && !in_array($file, $include)) {
      continue;
    }
    // Exclude certain files
    if (!in_array($file, $exclude)) {
      if (($file != '.') && ($file != '..')) {
        if (is_dir($src . '/' . $file)) {
          $zip->addEmptyDir($dst . '/' . $file . '/');
          $ret = _cms_updater_recurse_zip($src . '/' . $file, $dst . '/' . $file, array(), array(), $zip);
          if (is_array($ret)) {
            return $ret;
          }
        }
        else {
          if (!$zip->addFile($src . '/' . $file, $dst . '/' . $file)) {
            return array(
              'success'     => FALSE,
              'error'       => 'could not add file to zip',
              'file'        => $file,
              'src'         => $src,
              'destination' => $dst . '/' . $file,
              'base'        => basename($dst),
              'strrplc'     => str_replace($src . '/', '', $file),
              'zipstatus'   => $zip->getStatusString(),
            );
          }
        }
      }
    }
  }
  closedir($dir);
  return TRUE;
}

/**
 * Recursive copy files from source to destination
 *
 * @param $src
 * @param $dst
 * @param $exclude
 * @param $include
 */
function _cms_updater_recurse_copy($src, $dst, $exclude = array(), $include = array()) {
  // Create dir if not exist
  if (!file_exists($dst)) {
    @mkdir($dst);

  }

  $dir = opendir($src);
  while (FALSE !== ($file = readdir($dir))) {

    // Do only include provided files if set
    if (!empty($include) && !in_array($file, $include)) {
      continue;
    }
    // Exclude certain files
    if (!in_array($file, $exclude)) {
      if (($file != '.') && ($file != '..')) {
        if (is_dir($src . '/' . $file)) {
          $ret = _cms_updater_recurse_copy($src . '/' . $file, $dst . '/' . $file);
          // If we receive an error array - stop and return
          if (is_array($ret)) {
            return $ret;
          }
        }
        else {
          $ecp = copy($src . '/' . $file, $dst . '/' . $file);
          if ($ecp === FALSE) {
            return array(
              'success'     => FALSE,
              'error'       => 'could not copy to backup',
              'file'        => $file,
              'destination' => $dst . '/' . $file,
            );
          }
        }
      }
    }
  }
  closedir($dir);
  return TRUE;
}

/**
 * Get a List of all relevant Drupal 7 root files and dirs
 *
 * @return array
 */
function _cms_updater_get_drupal_root_file_structure() {
  // Include all Drupal 7 core files without the sites folder
  // We do this to ensure that we only use Drupal core files and not some other dirs in the Drupal root.
  $include = array(
    '.htaccess',
    'authorize.php',
    'cron.php',
    'includes',
    'index.php',
    'install.php',
    'misc',
    'modules',
    'robots.txt',
    'profiles',
    'scripts',
    'themes',
    'update.php',
    'web.config',
    'xmlrpc.php',
  );

  return $include;
}
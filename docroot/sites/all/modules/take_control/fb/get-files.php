<?php


require_once (drupal_get_path('module', 'take_control') . '/includes/security.inc');
require_once (drupal_get_path('module', 'take_control') . '/includes/filesystem.inc');

function take_control_fb_getfiles() {
  // set the headers
  header("Content-type: text/html");
  header("Expires: Wed, 29 Jan 1975 04:15:00 GMT");
  header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
  header("Cache-Control: no-cache, must-revalidate");
  header("Pragma: no-cache");

  // grab and validate the custom params
  $op = NULL;
  if (isset($_REQUEST['op'])) {
    $op = $_REQUEST['op'];
  }
  else {
    take_control_security_error();
  }

  $method = strtolower($_SERVER['REQUEST_METHOD']);
  if($method != 'post') {
    $getOps = array('download-file');
    if(!in_array($op, $getOps)) {
      take_control_security_error();
    }
  }

  $validationString = $_REQUEST['validationString'];
  $validationToken = $_REQUEST['validationToken'];
  if (!drupal_valid_token($validationToken, $validationString)) {
    take_control_security_error();
  }
  
  try {
    // perform the actual operation
    switch ($op) {
      case 'list-subdirs':
        take_control_fb_list_subdirs();
        break;
      case 'list-files':
        take_control_fb_list_files();
        break;
      case 'create-folder':
        take_control_fb_create_folder();
        break;
      case 'create-file':
        take_control_fb_create_file();
        break;
      case 'copy':
        take_control_fb_copy();
        break;
      case 'move':
        take_control_fb_move();
        break;
      case 'upload-file':
        take_control_fb_file_upload();
        break;
      case 'download-file':
        take_control_fb_file_download();
        break;
      case 'delete':
        take_control_fb_delete();
        break;
      case 'extract-archive':
        take_control_fb_extract_archive();
        break;
      case 'create-archive':
        take_control_fb_create_archive();
        break;
      case 'rename':
        take_control_fb_rename();
        break;
      case 'change-perm':
        take_control_fb_change_perm();
        break;
      case 'view-file':
        take_control_fb_view_file();
        break;
      default:
        take_control_security_error();
    }
  }
  catch (Exception $e) {
    die($e->getMessage() + "\n" + $e->getTraceAsString());
  }

  die();
}

///////////////////////////////////////////////////////////////////////////////////////////////////


// op type handlers
function take_control_fb_list_subdirs() {
  $node = $_REQUEST['node'];
  _take_control_fb_validate_path($node);

  _take_control_fb_scan_dir($node, $subdirs, $files);
  echo json_encode($subdirs);
}

function take_control_fb_list_files() {
  $node = $_REQUEST['node'];
  _take_control_fb_validate_path($node);

  _take_control_fb_scan_dir($node, $subdirs, $files);
  $files = array_merge($subdirs, $files);
  echo json_encode($files);
}

function take_control_fb_create_folder() {
  if(variable_get('take_control_fb_enable_demo_mode', FALSE)) {
    take_control_security_error(t('Action disabled in demo mode.'));
  }
  
  $parent = $_REQUEST['parent'];
  $name = $_REQUEST['name'];
  _take_control_fb_validate_path($parent);
  _take_control_fb_validate_name($name);

  $dir = $parent . DIRECTORY_SEPARATOR . $name;

  if (file_exists($dir)) {
    echo 'Folder already exists.';
  }
  else {
    mkdir($dir, 0755);
  }
}

function take_control_fb_create_file() {
  if(variable_get('take_control_fb_enable_demo_mode', FALSE)) {
    take_control_security_error(t('Action disabled in demo mode.'));
  }
  
  $parent = $_REQUEST['parent'];
  $name = $_REQUEST['name'];
  _take_control_fb_validate_path($parent);
  _take_control_fb_validate_name($name);

  $file = $parent . DIRECTORY_SEPARATOR . $name;

  if (file_exists($file)) {
    echo 'File already exists.';
  }
  else {
    file_put_contents($file, '');
  }
}

function take_control_fb_copy() {
  if(variable_get('take_control_fb_enable_demo_mode', FALSE)) {
    take_control_security_error(t('Action disabled in demo mode.'));
  }
  
  $nodes = $_REQUEST['nodes'];
  $path = $_REQUEST['path'];
  $nodes = json_decode($nodes);

  take_control_fb_validate_paths($nodes);
  _take_control_fb_validate_path($path);

  $parent = dirname($nodes[0]);
  if (strcmp($parent, $path) == 0) {
    die('The source and destination directory cannot be same.');
  }
  foreach ($nodes as $node) {
    take_control_copy($node, $path . DIRECTORY_SEPARATOR . basename($node));
  }
}

function take_control_fb_move() {
  if(variable_get('take_control_fb_enable_demo_mode', FALSE)) {
    take_control_security_error(t('Action disabled in demo mode.'));
  }
  
  $nodes = $_REQUEST['nodes'];
  $path = $_REQUEST['path'];
  $nodes = json_decode($nodes);

  take_control_fb_validate_paths($nodes);
  _take_control_fb_validate_path($path);

  $parent = dirname($nodes[0]);
  if (strcmp($parent, $path) == 0) {
    die('The source and destination directory cannot be same.');
  }
  foreach ($nodes as $node) {
    take_control_move($node, $path . DIRECTORY_SEPARATOR . basename($node));
  }
}

function take_control_fb_file_download() {
  $file = $_REQUEST['file'];
  _take_control_fb_validate_path($file);

  if (file_exists($file)) {
    header('Content-Description: File Transfer');
    header('Content-Type: ' . file_get_mimetype($file));
    header('Content-Disposition: attachment; filename=' . basename($file));
    header('Content-Transfer-Encoding: binary');
    header('Expires: 0');
    header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
    header('Pragma: public');
    header('Content-Length: ' . filesize($file));
    ob_clean();
    flush();
    readfile($file);
    die();
  }
  else {
    echo 'Invalid File name.';
  }
}

function take_control_fb_file_upload() {
  if(variable_get('take_control_fb_enable_demo_mode', FALSE)) {
    take_control_security_error(t('Action disabled in demo mode.'));
  }
  
  $parent = $_REQUEST['parent'];
  _take_control_fb_validate_path($parent);

  $success = true;
  $failed = array();
  $succeded = array();
  foreach ($_FILES as $key => &$file) {
    $target_path = $parent . DIRECTORY_SEPARATOR . basename($file['name']);
    $partial = move_uploaded_file($file['tmp_name'], $target_path);
    if ($partial) {
      $succeded[] = $file['name'];
    }
    else {
      $failed[] = $file['name'];
    }
    $success = $success && $partial;
  }
  echo '{success:true, succeded:' . json_encode($succeded) . ', failed:' . json_encode($failed) . '}';
}

function take_control_fb_delete() {
  if(variable_get('take_control_fb_enable_demo_mode', FALSE)) {
    take_control_security_error(t('Action disabled in demo mode.'));
  }
  
  $nodes = $_REQUEST['nodes'];
  $nodes = json_decode($nodes);

  take_control_fb_validate_paths($nodes);

  foreach ($nodes as $path) {
    if (is_file($path)) {
      unlink($path);
    }
    else {
      if (!is_dir($path)) {
        die('Error: File not found');
      }
      take_control_delete_directory($path);
    }
  }
}

function take_control_fb_extract_archive() {
  if(variable_get('take_control_fb_enable_demo_mode', FALSE)) {
    take_control_security_error(t('Action disabled in demo mode.'));
  }
  
  $path = $_REQUEST['path'];
  $file = $_REQUEST['file'];
  _take_control_fb_validate_path($path);
  _take_control_fb_validate_path($file);

  if (!in_array(file_get_mimetype($file), take_control_zip_mimes())) {
    take_control_security_error('The file is not an archive file.');
  }

  //Create the directory if it does not exist.
  if (!file_exists($path)) {
    if (is_file($path)) {
      die('A file already exists for the specified directory path and name.');
    }
    mkdir($path, 0755, true);
  }

  $zip = new ZipArchive();
  if ($zip->open($file) === TRUE) {
    $zip->extractTo($path);
    $zip->close();
  }
  else {
    echo 'Could not open Archive File';
  }
}

function take_control_fb_create_archive() {
  if(variable_get('take_control_fb_enable_demo_mode', FALSE)) {
    take_control_security_error(t('Action disabled in demo mode.'));
  }
  
  $nodes = $_REQUEST['nodes'];
  $file = $_REQUEST['file'];
  $curDir = $_REQUEST['curDir'];
  $nodes = json_decode($nodes);

  take_control_fb_validate_paths($nodes);
  _take_control_fb_validate_path($file);
  _take_control_fb_validate_path($curDir);

  $ext = '.zip';
  if (substr_compare($file, $ext, -strlen($ext), strlen($ext)) !== 0) {
    $file = $file . $ext;
  }

  $zip = new ZipArchive();
  if ($zip->open($file, file_exists($file) ? ZIPARCHIVE::OVERWRITE : ZIPARCHIVE::CREATE) !== true) {
    die('Could not create zip file.');
  }

  foreach ($nodes as $path) {
    if (is_file($path)) {
      _take_control_fb_zip_addFile($zip, $path, $curDir);
    }
    else {
      if (!is_dir($path)) {
        $zip->close();
        unlink($file);
        die('Error: File not found');
      }
      _take_control_fb_recurse_zip($zip, $path, $curDir);
    }
  }
  $zip->close();
}

function take_control_fb_rename() {
  if(variable_get('take_control_fb_enable_demo_mode', FALSE)) {
    take_control_security_error(t('Action disabled in demo mode.'));
  }
  
  $path = $_REQUEST['path'];
  $new = $_REQUEST['name'];
  _take_control_fb_validate_path($path);
  _take_control_fb_validate_name($new);

  $new = dirname($path) . DIRECTORY_SEPARATOR . $new;

  $success = rename($path, $new);
  if (!$success) {
    echo 'Operation failed';
  }
}

function take_control_fb_change_perm() {
  if(variable_get('take_control_fb_enable_demo_mode', FALSE)) {
    take_control_security_error(t('Action disabled in demo mode.'));
  }
  
  $nodes = $_REQUEST['nodes'];
  $perm = $_REQUEST['perm'];
  $nodes = json_decode($nodes);

  take_control_fb_validate_paths($nodes);
  _take_control_fb_validate_perm($perm);

  $perm = octdec($perm);
  $errs = '';

  foreach ($nodes as $path) {
    $success = chmod($path, $perm);
    if ($success) {
      $errs .= 'Failed to change permission for: <b>' . $path . "</b>\n";
    }
  }

  echo ($errs);
}

function take_control_fb_view_file() {
  $file = $_REQUEST['file'];
  _take_control_fb_validate_path($file);

  readfile($file);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// helper functions
function _take_control_fb_validate_path($path) {
  if (!isset($path)) {
    take_control_security_error();
  }
  if (strpos($path, '..') !== false) {
    take_control_security_error();
  }
  $rootFolder = _take_control_get_user_root_accessible_dir();
  $docRoot = $_SERVER['DOCUMENT_ROOT'];

  $len = strlen($docRoot);
  if ($docRoot[$len - 1] == DIRECTORY_SEPARATOR) {
    $docRoot = substr($docRoot, 0, $len - 1);
  }

  if (!isset($rootFolder) || strpos($rootFolder, $docRoot) !== 0 || strpos($path, $rootFolder) !== 0) {
    take_control_security_error();
  }
}

function _take_control_fb_validate_paths($paths) {
  foreach ($paths as $path) {
    _take_control_fb_validate_path($path);
  }
}

function _take_control_fb_validate_name($name) {
  if (strpos($name, '/') !== FALSE || strpos($name, '\\') !== FALSE) {
    take_control_security_error('Invalid name.');
  }
}

function _take_control_fb_validate_perm($perm) {
  $perm = (int) $perm;

  if ($perm < 600 || $perm > 777) {
    take_control_security_error('Permission should be between 600 and 777.');
  }
}

function _take_control_fb_scan_dir($base, &$subdirs, &$files) {
  $subdirs = array();
  $files = array();

  if (is_dir($base)) {
    $d = dir($base);
    while ($f = $d->read()) {
      if ($f == '.' || $f == '..') // Uncomment this if system files are not to be served || substr($f, 0, 1) == '.')
        continue;

      $filename = $base . '/' . $f;
      $lastmod = date('M j, Y, g:i a', filemtime($filename));

      if (is_dir($filename)) {
        $qtip = 'Type: Folder<br />Last Modified: ' . $lastmod;
        $subdirs[] = array(
            'id' => $filename,
            'text' => $f,
            'cls' => 'folder',
            'modified' => $lastmod,
            'owner' => fileowner($filename),
            'perm' => substr(decoct(fileperms($filename)), 1),
            'mime' => 'folder',
            'qtip' => $qtip);
      }
      else {
        $files[] = array(
            'id' => $filename,
            'text' => $f,
            'cls' => 'file',
            'modified' => $lastmod,
            'owner' => fileowner($filename),
            'perm' => substr(decoct(fileperms($filename)), 2),
            'mime' => file_get_mimetype($filename),
            'size' => _take_control_fb_format_bytes(filesize($filename), 2));
      }
    }

    $d->close();

    usort($subdirs, '_take_control_fb_file_comparer');
    usort($files, '_take_control_fb_file_comparer');
  }
  else {
    take_control_security_error();
  }
}

function _take_control_fb_recurse_zip(&$zip, $path, $basePath) {
  //The line below ensures that empty directories also get added to the zip file.
  _take_control_fb_zip_addDir($zip, $path, $basePath);
  $dir = opendir($path);
  while (($file = readdir($dir)) !== false) {
    if (($file != '.') && ($file != '..')) {
      if (is_dir($file)) {
        _take_control_fb_recurse_zip($zip, $file);
      }
      else {
        _take_control_fb_zip_addFile($zip, $path . DIRECTORY_SEPARATOR . $file, $basePath);
      }
    }
  }
  closedir($dir);
}

function _take_control_fb_zip_addFile(&$zip, $path, $basePath) {
  $name = substr($path, strlen($basePath) + 1);
  $zip->addFile($path, $name);
}

function _take_control_fb_zip_addDir(&$zip, $path, $basePath) {
  $name = substr($path, strlen($basePath) + 1);
  $zip->addEmptyDir($name);
}

function _take_control_fb_file_comparer($a, $b) {
  return strcmp($a["text"], $b["text"]);
}

function _take_control_fb_format_bytes($val, $digits = 3, $mode = 'SI', $bB = 'B') { //$mode == 'SI'|'IEC', $bB == 'b'|'B'
  $si = array('', 'K', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y');
  $iec = array('', 'Ki', 'Mi', 'Gi', 'Ti', 'Pi', 'Ei', 'Zi', 'Yi');
  switch (strtoupper($mode)) {
    case 'SI':
      $factor = 1000;
      $symbols = $si;
      break;
    case 'IEC':
      $factor = 1024;
      $symbols = $iec;
      break;
    default:
      $factor = 1000;
      $symbols = $si;
      break;
  }
  switch ($bB) {
    case 'b':
      $val *= 8;
      break;
    default:
      $bB = 'B';
      break;
  }
  for ($i = 0; $i < count($symbols) - 1 && $val >= $factor; $i++)
    $val /= $factor;
  $p = strpos($val, '.');
  if ($p !== false && $p > $digits)
    $val = round($val);
  elseif ($p !== false)
    $val = round($val, $digits - $p);
  return round($val, $digits) . ' ' . $symbols[$i] . $bB;
}

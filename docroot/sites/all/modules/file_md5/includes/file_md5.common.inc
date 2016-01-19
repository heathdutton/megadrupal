<?php
/**
 * @file
 * Auxiliary functionality.
 */

/**
 * Scan a given path for files and set MD5 hash for each of them.
 *
 * @param string $directory
 *   Directory for scanning.
 * @param string[] $extensions
 *   Allowed file extensions.
 *
 * @return \stdClass[]
 *   An array with Drupal file objects.
 */
function file_md5_scan($directory = 'public://', $extensions = ['jpg', 'jpeg', 'png', 'gif', 'svg', 'csv']) {
  $files = [];

  /* @var \SplFileInfo $object */
  foreach (new \RecursiveIteratorIterator(new \RecursiveDirectoryIterator($directory)) as $path => $object) {
    if ($object->isFile() && in_array($object->getExtension(), $extensions)) {
      $file = file_md5_create_file($object);
      $files[$file->fid] = $file;
    }
  }

  return $files;
}

/**
 * Create Drupal file object from SplFileInfo.
 *
 * @see file_save_upload()
 *
 * @param \SplFileInfo $object
 *   File object.
 *
 * @throws \RuntimeException
 *   When file doesn't exists on a file system.
 *
 * @return \stdClass
 *   Drupal file object.
 */
function file_md5_create_file(\SplFileInfo $object) {
  global $user;

  $uri = $object->getPathname();

  if (!file_exists($uri)) {
    throw new \RuntimeException(t('File does not exist in at @uri.', [
      '@uri' => $uri,
    ]));
  }

  // Try to find an entry about file in the database (by URI and MD5 hash).
  $file = file_md5_load_by('md5', file_md5_hash($uri)) ?: file_md5_load_by('uri', $uri);

  // Create a new entry if file does not exist.
  if (empty($file)) {
    $file = new \stdClass();
    $file->uri = $uri;
    $file->uid = $user->uid;
    $file->status = FILE_STATUS_PERMANENT;
    $file->filename = $object->getFilename();
    $file->filemime = file_get_mimetype($uri);
    $file->filesize = filesize($uri);
  }

  // Set "md5" property if it is empty.
  if (empty($file->md5)) {
    // The "md5" property will be added by hook_file_presave().
    // @see file_md5_file_presave()
    file_save($file);
  }

  return $file;
}

/**
 * Generate a hash value using the contents of a given file.
 *
 * @param string $uri
 *   Path to file.
 *
 * @return string
 *   MD5 hash of a file.
 */
function file_md5_hash($uri) {
  return md5_file($uri);
}

/**
 * Load file using one of columns of "file_managed" table.
 *
 * @see file_md5_hash()
 * @see system_schema()
 *
 * @param string $field
 *   Any field, defined in "file_managed" schema.
 * @param string $value
 *   Value of a field.
 *
 * @return \stdClass
 *   Drupal file object.
 */
function file_md5_load_by($field, $value) {
  // If user tries to load file using "md5" hash, but it is not
  // correct, then try to recognize an argument as file ID.
  if ('md5' === $field && strlen($value) !== 32) {
    return file_load($value);
  }

  $files = file_load_multiple([], [$field => $value]);

  return reset($files);
}

/**
 * Delete file using one of columns of "file_managed" table.
 *
 * @see file_md5_load_by()
 *
 * @param string $field
 *   Any field, defined in "file_managed" schema.
 * @param string $value
 *   Value of a field.
 *
 * @return bool
 *   Deleting state.
 */
function file_md5_delete_by($field, $value) {
  return file_delete(file_md5_load_by($field, $value));
}

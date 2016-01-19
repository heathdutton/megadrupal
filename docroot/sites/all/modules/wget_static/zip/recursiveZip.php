<?php
/**
 * @file
 * Recursively ZIP / Compress a file / folder.
 */

/**
 * Recursivezip class.
 */
class WgetStaticRecursiveZip {
  /**
   * Recursively reads a directory and compress files.
   */
  private function dorecursivezip($src, &$zip, $path) {
    // Open file/directory.
    $dir = opendir($src);
    // Loop through the directory.
    while (FALSE !== ($file = readdir($dir))) {
      // Skip parent (..) and root (.) directory.
      if (($file != '.') && ($file != '..')) {
        // If directory found again, call dorecursivezip() function again.
        if (is_dir($src . '/' . $file)) {
          $this->dorecursivezip($src . '/' . $file, $zip, $path);
        }
        else {
          // Add files to zip.
          $zip->addFile($src . '/' . $file, substr($src . '/' . $file, $path));
        }
      }
    }
    closedir($dir);
  }

  /**
   * Perform compression.
   */
  public function compress($src, $dst, $filename) {
    // Check zip extension loaded or not and file existence.
    if (!extension_loaded('zip') || !file_exists($src)) {
      return FALSE;
    }

    // Remove last slash (/) from source directory / destination directory.
    if (substr($src, -1) === '/') {
      $src = substr($src, 0, -1);
    }
    if (substr($dst, -1) === '/') {
      $dst = substr($dst, 0, -1);
    }
    $path = strlen(dirname($src) . '/');
    $dst = empty($dst) ? $filename : $dst . '/' . $filename;
    @unlink($dst);

    // Create zip.
    $zip = new ZipArchive();
    $res = $zip->open($dst, ZipArchive::CREATE);
    if ($res !== TRUE) {
      watchdog('wget_static', 'Error: Unable to create zip file', array(), WATCHDOG_NOTICE, 'link');
      return FALSE;
    }
    if (is_file($src)) {
      $zip->addFile($src, substr($src, $path));
    }
    else {
      if (!is_dir($src)) {
        $zip->close();
        @unlink($dst);
        watchdog('wget_static', 'Error: File not found', array(), WATCHDOG_NOTICE, 'link');
        return FALSE;
      }
      $this->dorecursivezip($src, $zip, $path);
    }
    $zip->close();
    return $dst;
  }

}

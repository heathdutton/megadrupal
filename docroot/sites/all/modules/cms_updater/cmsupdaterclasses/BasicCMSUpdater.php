<?php
/**
 * @file
 * Contains BasicCMSUpdater
 */

require_once 'CMSUpdateManager.php';

/**
 * Class BasicCMSUpdater
 */
abstract class BasicCMSUpdater {
  private $download_url; // URL where to download new version of project/cms from.
  private $extract_path; // Path where to extract the zip to (probably somewhere in temp).
  private $source_root_backup_path; // Root path where to start backup from.
  private $destination_backup_file; // Where to store the backup-zip-file.
  private $overwrite_target_root; // Where to copy the downloaded archives files (usually the web root of the cms).
  private $update_manager = NULL; // Holds the object/instance of the CMSUpdateManager
  private $temppath = ''; // Holds the temporary path
  private $files_path; // Relative path to i.e. sites/default/files/private
  private $root_dir;
  private $client_enabled = TRUE; //TRUE or FALSE, default TRUE
  private $update_success = FALSE;

  /**
   * @param $file
   * @return string - last 3 digits i.e. 777, 755
   */
  function getFilePermission($file) {
    $length = strlen(decoct(fileperms($file))) - 3;
    return substr(decoct(fileperms($file)), $length);
  }

  /**
   * @param $path
   * @return mixed|string
   */
  public function sanitizePath($path) {
    $path = str_replace('\\', '/', $path); // Windows path sanitization.
    if (substr($path, -1) == '/') {
      $path = substr($path, 0, -1);
    }
    return $path;
  }

  /**
   * @param $md5_1
   * @param $md5_2
   */
  public function md5_check($md5_1, $md5_2) {
    if ($md5_1 <> $md5_2) {
      //md5 match failed failed:
      $this->get_update_manager()->log['CMSUPDATER_DOWNLOAD_NEW_CORE'] = array(
        'success'       => FALSE,
        'error_message' => 'could not download Archive',
        'error'         => 'md5hashes did not match',
      );
      $this->get_update_manager()->write_log();
      $this->get_update_manager()->send_site_update_end(0, $this->get_update_manager()->log);
      EXIT("END");
    }
  }

  /**
   * @return mixed
   */
  abstract function update_db();

  /**
   * @param $extract_path
   * @param $dl_files
   * @return mixed
   */
  abstract function get_cms_core($extract_path, $dl_files);

  /**
   * @param $source
   * @param $destination
   * @param $justcheck
   * @return mixed
   */
  abstract function move_core_files($source, $destination, $justcheck = FALSE);

  /**
   * @param $source
   * @param $destination
   * @return mixed
   */
  abstract function backup_files($source, $destination);

  /**
   * @return null
   */
  public function get_update_manager() {
    return $this->update_manager;
  }

  /**
   * @param CMSUpdateManager $update_manager
   */
  public function set_update_manager(CMSUpdateManager $update_manager) {
    $this->update_manager = $update_manager;
  }

  /**
   * @return mixed
   */
  public function get_overwrite_target_root() {
    return $this->overwrite_target_root;
  }

  /**
   * @param $overwrite_target_root
   */
  public function set_overwrite_target_root($overwrite_target_root) {
    $this->overwrite_target_root = $overwrite_target_root;
  }

  /**
   * @return mixed
   */
  public function get_download_url() {
    return $this->download_url;
  }

  /**
   * @return mixed
   */
  public function get_extract_path() {
    return $this->extract_path;
  }

  /**
   * @return mixed
   */
  public function get_source_root_backup_path() {
    return $this->source_root_backup_path;
  }

  /**
   * @return mixed
   */
  public function get_destination_backup_file() {
    return $this->destination_backup_file;
  }

  /**
   * @param $download_url
   */
  public function set_download_url($download_url) {
    $this->download_url = $download_url;
  }

  /**
   * @param $extract_path
   */
  public function set_extract_path($extract_path) {
    $this->extract_path = $extract_path;
  }

  /**
   * @param $source_root_backup_path
   */
  public function set_source_root_backup_path($source_root_backup_path) {
    $this->source_root_backup_path = $source_root_backup_path;
  }

  /**
   * @param $destination_backup_file
   */
  public function set_destination_backup_file($destination_backup_file) {
    $this->destination_backup_file = $destination_backup_file;
  }

  /**
   * @return string
   */
  public function get_temppath() {
    return $this->temppath;
  }

  /**
   * @param $temppath
   */
  public function set_temppath($temppath) {
    $this->temppath = $temppath;
  }

  /**
   * @return mixed
   */
  public function get_files_path() {
    return $this->files_path;
  }

  /**
   * @param $files_path
   */
  public function set_files_path($files_path) {
    $this->files_path = $files_path;
  }

  /**
   * @return mixed
   */
  public function get_root_dir() {
    return $this->root_dir;
  }

  /**
   * @param $root_dir
   */
  public function set_root_dir($root_dir) {
    $this->root_dir = $root_dir;
  }

  /**
   * @return bool
   */
  public function get_client_enabled() {
    return $this->client_enabled;
  }

  /**
   * @param $client_enabled
   */
  public function set_client_enabled($client_enabled) {
    $this->client_enabled = $client_enabled;
  }

  /**
   * @return bool
   */
  public function get_update_success() {
    return $this->update_success;
  }

  /**
   * @param $update_success
   */
  public function set_update_success($update_success) {
    $this->update_success = $update_success;
  }

  /**
   * Recursively delete dir and its contents:
   *
   * @param $dir
   */
  public function rrmdir($dir) {
    if (is_dir($dir)) {
      $objects = scandir($dir);
      foreach ($objects as $object) {
        if ($object != "." && $object != "..") {
          if (filetype($dir . "/" . $object) == "dir") {
            $this->rrmdir($dir . "/" . $object);
          }
          else {
            unlink($dir . "/" . $object);
          }
        }
      }
      reset($objects);
      rmdir($dir);
    }
  }

  /**
   * Function is_writable_chmod checks if $filename is writable and if not,
   * tries to adjust permission and checks again.
   *
   * @param  $filename
   * @return bool
   */
  public function is_writable_chmod($filename) {
    if (!file_exists($filename)) {
      return FALSE;
    }

    $ecp = FALSE;
    if (is_dir($filename)) {
      $orig_perms = $this->getFilePermission($filename);
      if (chmod($filename, octdec('775'))) {
        $ecp = is_writable($filename);
        chmod($filename, octdec($orig_perms));
      }
    }
    else {
      $orig_perms = $this->getFilePermission($filename);
      if (chmod($filename, octdec('664'))) {
        $ecp = is_writable($filename);
        chmod($filename, octdec($orig_perms));
      }
    }
    return $ecp;
  }

  /**
   * Function copy_chmod, tries to copy a file from source to destination
   * on failure it tries changing chmod of target and tries again.
   * again.
   *
   * @param $source
   * @param $target
   * @return bool
   */
  public function copy_chmod($source, $target) {
    $ret = FALSE;
    if (copy($source, $target)) {
      $ret = TRUE;
    }
    else {
      if (file_exists($target) && (!is_dir($target))) {
        $orig_perms = $this->getFilePermission($target);

        if (chmod($target, octdec('664'))) {
          if (copy($source, $target)) {
            $ret = TRUE;
          }
          else {
            $ret = FALSE;
          }
          chmod($target, octdec($orig_perms));
        }
        else {
          $ret = FALSE;
        }
      }
    }
    return $ret;
  }

}

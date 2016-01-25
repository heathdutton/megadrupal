<?php

/**
 * @file
 * This file is modified copy of Drupal Updater Class
 * and specfically used for SN Quick Multisite creation 
 * for making a clone of existing site,
 * which is simply used for copying the modules and themes 
 * from the source site, 
 * to destination site(the new site).
 */

/**
 * Base class for Copying directories
 */
class SnQuickMultisiteCopyDir {

  /**
   * @var string $source Directory to copy from.
   * @var string $destination destination Directory
   */
  public $source;
  public $destination;

  /**
   * Store the parameters for source and destination.
   * 
   * @param string $source
   *   String for holding source.
   * @param string $destination
   *   String for holding the destination for copying.
   */
  public function __construct($source, $destination) {
    $this->source = $source;
    $this->destination = $destination;
  }

  /**
   * Store the default parameters for copying.
   *
   * @param array $overrides
   *   An array of overrides for the default parameters.
   *
   * @return array
   *   An array of configuration parameters for copying.
   */
  protected function getInstallArgs($overrides = array()) {
    $args = array(
      'make_backup' => FALSE,
      'copy_dir' => $this->destination,
    );
    return array_merge($args, $overrides);
  }

  /**
   * Copy directory, returns a list of next actions.
   *
   * @param FileTransfer $filetransfer
   *   Object that is a child of FileTransfer.
   * @param array $overrides
   *   An array of settings to override defaults; see self::getInstallArgs().
   */
  public function copyDir(&$filetransfer, $overrides = array()) {
    try {
      // Establish arguments with possible overrides.
      $args = $this->getInstallArgs($overrides);

      // Make sure the installation parent directory exists and is writable.
      $this->prepareCopyDirectory($filetransfer, $args['copy_dir']);

      // Copy the directory in place.
      $filetransfer->copyDirectory($this->source, $args['copy_dir']);

      // Make sure what we just copied is readable by the web server.
      $this->makeWorldReadable($filetransfer, $args['copy_dir'] . '/' . $this->name);

    }
    catch (FileTransferException $e) {
      throw new UpdaterFileTransferException(t('File Transfer failed, reason: !reason', array('!reason' => strtr($e->getMessage(), $e->arguments))));
    }
  }

  /**
   * Make sure the installation parent directory exists and is writable.
   *
   * @param FileTransfer $filetransfer
   *   Object which is a child of FileTransfer.
   * @param string $directory
   *   The directory to prepare.
   */
  public function prepareCopyDirectory(&$filetransfer, $directory) {
    // Make the parent dir writable if need be and create the dir.
    if (!is_dir($directory)) {
      $parent_dir = dirname($directory);
      if (!is_writable($parent_dir)) {
        @chmod($parent_dir, 0755);
        // It is expected that this will fail if the directory is owned by the
        // FTP user. If the FTP user == web server, it will succeed.
        try {
          $filetransfer->createDirectory($directory);
          $this->makeWorldReadable($filetransfer, $directory);
        }
        catch (FileTransferException $e) {
          drupal_set_message($e->getMessage(), 'error');
        }
        // Put the parent directory back.
        @chmod($parent_dir, 0555);
      }
    }
  }

  /**
   * Ensure that a given directory is world readable.
   *
   * @param FileTransfer $filetransfer
   *   Object which is a child of FileTransfer.
   * @param string $path
   *   The file path to make world readable.
   * @param bool $recursive
   *   If the chmod should be applied recursively.
   */
  public function makeWorldReadable(&$filetransfer, $path, $recursive = TRUE) {
    if (!is_executable($path)) {
      // Set it to read + execute.
      $new_perms = substr(sprintf('%o', fileperms($path)), -4, -1) . "5";
      $filetransfer->chmod($path, intval($new_perms, 8), $recursive);
    }
  }

}

<?php
/**
 * @file
 * TODO.
 */

/**
 * Class that represents a translation job response.
 */
class TmgmtXConnectResponse {
  /**
   * The path to the archive file.
   *
   * @var string
   */
  private $filePath;

  /**
   * The Zip file container.
   *
   * @var TmgmtXConnectResponseFile
   */
  private $file;


  /**
   * Constructor.
   *
   * @param string $file_path
   *   The file path to the zip archive.
   */
  public function __construct($file_path) {
    $this->filePath = $file_path;
  }

  /**
   * Get the archive file container.
   *
   * @return TmgmtXConnectResponseFile
   *   The response file wrapper.
   *
   * @throws TmgmtXConnectResponseCantOpenFileException
   *   When the Zip file can not be opened.
   */
  public function getFile() {
    if (!$this->file) {
      $zip = new ZipArchive();
      if (!$zip->open(drupal_realpath($this->filePath))) {
        throw new TmgmtXConnectResponseCantOpenFileException(
          'Could not open the archive file.'
        );
      }

      $this->file = new TmgmtXConnectResponseFile($zip);
    }

    return $this->file;
  }

  /**
   * Get the remote file path.
   *
   * @return string
   *   The remote file path.
   */
  public function getFilePath() {
    return $this->filePath;
  }

}

<?php
/**
 * @file
 * Class that wraps around the response Zip File.
 */

/**
 * Class that represents a translation job response file (zip archive).
 */
class TmgmtXConnectResponseFile {
  /**
   * The file with the translation response.
   *
   * @var ZipArchive
   */
  private $file;

  /**
   * The info file within the response.
   *
   * @var TmgmtXConnectResponseDelivery
   */
  private $delivery;


  /**
   * Constructor.
   *
   * @param ZipArchive $file
   *   The local zip file with the translation response.
   */
  public function __construct(ZipArchive $file) {
    $this->file = $file;
  }

  /**
   * Cleanup before destroy.
   */
  public function __destruct() {
    $this->file->close();
  }

  /**
   * Get the response information.
   *
   * @return TmgmtXConnectResponseDelivery
   *   The info based on the Delivery XML.
   */
  public function getDeliveryInfo() {
    if (!$this->delivery) {
      $info_file = basename($this->file->filename, '.zip') . '.xml';
      $this->delivery = new TmgmtXConnectResponseDelivery(
        $this->getFileContent($info_file)
      );
    }

    return $this->delivery;
  }

  /**
   * Get the translations from the zip file.
   *
   * @return TmgmtXConnectResponseTranslations
   *   Collection of translations.
   */
  public function getTranslations() {
    return new TmgmtXConnectResponseTranslations($this);
  }

  /**
   * Helper to get the content of a file within the zip archive.
   *
   * @param string $path
   *   The file path within the zip archive of the file we need to open.
   *
   * @return string
   *   The file content if file exists.
   *
   * @throws TmgmtXConnectResponseFileDoesNotExistsException
   *   When the requested file does not exists.
   */
  public function getFileContent($path) {
    $content = '';
    $fp = $this->file->getStream($path);
    if (!$fp) {
      throw new TmgmtXConnectResponseFileDoesNotExistsException(
        sprintf('File "%s" not found in archive.', $path)
      );
    }

    while (!feof($fp)) {
      $content .= fread($fp, 2);
    }
    fclose($fp);

    return $content;
  }

}

<?php
/**
 * @file
 * Zip-file version of the request.
 */

/**
 * Zip File representation of the request.
 */
class TmgmtXConnectRequestFile {
  /**
   * The request order object.
   *
   * @var TmgmtXConnectRequestOrder
   */
  private $order;

  /**
   * The directory where to store the temporary archive file in.
   *
   * @var string
   */
  private $directory;

  /**
   * The zip archive object.
   *
   * @var ZipArchive
   */
  private $file;

  /**
   * The zip archive file path.
   *
   * @var string
   */
  private $path;

  /**
   * Is the file finished?
   *
   * @var bool
   */
  private $finished = FALSE;


  /**
   * Constructor.
   *
   * @param TmgmtXConnectRequestOrder $order
   *   The request order object.
   * @param string $directory
   *   The directory where to store the temporary archive file in.
   */
  public function __construct(TmgmtXConnectRequestOrder $order, $directory) {
    $this->order = $order;
    $this->directory = $directory;
  }

  /**
   * Cleanup during destroy.
   */
  public function __destruct() {
    if ($this->path && file_exists($this->path)) {
      unlink($this->path);
    }
  }

  /**
   * Finish the file.
   *
   * This will close the file to protect against further adding extra data.
   */
  public function finish() {
    $this->addOrder();
    $file = $this->getFile();
    $file->close();
    $this->finished = TRUE;
  }

  /**
   * Get the zip archive.
   *
   * @return ZipArchive
   *   The zip archive file.
   */
  public function getFile() {
    return $this->file;
  }

  /**
   * Get the file path.
   *
   * @return string
   *   The path to the zip file.
   */
  public function getPath() {
    if (empty($this->path)) {
      $this->setPath();
    }

    return $this->path;
  }

  /**
   * Set the path based on the request.
   */
  protected function setPath() {
    // Create the microseconds string to add to the order.
    $microseconds = floor((substr((string) microtime(), 1, 8) * 1000));
    // Make sure that there are always 3 digits.
    $microseconds = str_pad($microseconds, 3, '0', STR_PAD_LEFT);

    $timestamp = date('YmdHis') . $microseconds;
    $this->path = sprintf(
      '%s/drupal_order_%s.zip',
      $this->directory,
      $timestamp
    );
  }

  /**
   * Add a file to translate the archive.
   *
   * @param string $path
   *   The path to the file.
   */
  public function addFile($path) {
    $file_name = basename($path);
    $this->createFile()->addFile($path, 'Input/' . $path);
    $this->order->addInputFile($file_name);
  }

  /**
   * Add a file as string (raw content) to the archive.
   *
   * @param string $file_name
   *   The file name to use when adding the string as file to the archive.
   * @param string $content
   *   The content to add to the archive.
   */
  public function addFromString($file_name, $content) {
    $this->createFile()->addFromString('Input/' . $file_name, $content);
    $this->order->addInputFile($file_name);
  }

  /**
   * Add the order to the archive.
   */
  protected function addOrder() {
    $this->createFile()->addFromString('order.xml', $this->order->getXml());
  }

  /**
   * Create a new zip archive on disk.
   *
   * @return ZipArchive
   *   The created zip archive file.
   *
   * @throws TmgmtXConnectRequestFileException
   *   When the file can not be created on disk.
   */
  protected function createFile() {
    if (empty($this->file)) {
      $path = $this->getPath();

      // Create the archive on disk.
      $this->file = new ZipArchive();
      $result = $this->file->open($path, ZipArchive::CREATE);
      if (!$result) {
        throw new TmgmtXConnectRequestFileException(
          sprintf('Can\'t create the zip archive "%s"', $path)
        );
      }
    }

    return $this->file;
  }

}

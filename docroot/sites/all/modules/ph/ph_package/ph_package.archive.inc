<?php
/**
 * @file
 * Package archive implementation.
 */

/**
 * Archive utility class. This class uses Drupal core archivers to package.
 */
class PHPackageArchive {
  /**
   * Temporary working directory.
   * @var string
   */
  protected $workingDirectory;

  /**
   * Package directory.
   * @var string
   */
  protected $directory;

  /**
   * Package metadata.
   * @var array
   */
  protected $metadata;

  /**
   * Constructs an archive.
   *
   * @param $directory
   *   Exported package directory.
   * @param array $metadata
   *   Package metadata. See ph_package_stamp().
   *
   * @throws InvalidArgumentException
   *   If directory is not valid or package metadata is incomplete.
   * @throws PHPackageArchiveException
   *   If temporary working directory could not be initialized.
   *
   * @see ph_package_stamp()
   */
  public function __construct($directory, array $metadata) {
    if (!file_exists($directory) || !is_dir($directory) || !is_readable($directory)) {
      throw new InvalidArgumentException('Archive directory is not valid');
    }
    if (empty($metadata['project']) || empty($metadata['version'])) {
      throw new InvalidArgumentException('Package metadata is incomplete for archiving');
    }

    // Copy given directory into a named temporary directory.
    $this->workingDirectory = $this->generateTemporaryFile('', 'temporary://');
    if (!file_prepare_directory($this->workingDirectory, FILE_CREATE_DIRECTORY)) {
      throw new PHPackageArchiveException('Temporary directory could not be created for archiving.');
    }
    $this->directory = $this->workingDirectory . '/' . $metadata['project'];
    $transfer = FileTransferLocal::factory(drupal_realpath($this->workingDirectory), array());
    $transfer->copyDirectory(drupal_realpath($directory), drupal_realpath($this->directory));

    $this->metadata = $metadata;
  }

  /**
   * Creates an archive file.
   */
  public function createArchive($extension, array $options = array()) {
    // Add default options.
    $options += array(
      // Replace existing file by default.
      'replace' => FILE_EXISTS_ERROR,
    );

    // Create archive file.
    $archiveFile = $this->createDirectoryArchive($this->directory, $extension);
    $destination = $this->getFilePath($extension);
    if (file_unmanaged_move($archiveFile, $destination, $options['replace'])) {
      $file = file_uri_to_object($destination);
      return $file;
    }
    else {
      return NULL;
    }
  }

  /**
   * Gets the file name.
   */
  public function getFileName($extension) {
    $fileName = $this->metadata['project'] . '-' . ph_version_format($this->metadata['version'], 'standard');
    return $fileName . '.' . $extension;
  }

  /**
   * Gets the file path.
   */
  public function getFilePath($extension) {
    $destination = variable_get('ph_package_archive_directory', 'public://');
    $destination = substr($destination, -1) == '/' ? $destination : "$destination/";
    $fileName = $this->getFileName($extension);
    $filePath = $destination . $fileName;
    return $filePath;
  }

  /**
   * Archives a directory relative to its parent directory into a file.
   */
  protected function createDirectoryArchive($directory, $extension) {
    // Look up archiver in extension map.
    $extensions = ph_package_get_archive_extensions();
    $archiverInfo = archiver_get_info();
    if (!isset($extensions[$extension]) || !isset($archiverInfo[$extensions[$extension]['archiver']])) {
      throw new InvalidArgumentException('Unsupported archive extension');
    }
    $implementation = $archiverInfo[$extensions[$extension]['archiver']];

    // Create archive for directory.
    $filePath = $this->workingDirectory . '/' . $this->getFileName($extension);
    /** @var $archiver ArchiverInterface */
    $realFilePath = drupal_realpath($filePath);
    try {
      $archiver = new $implementation['class']($realFilePath);
    }
    catch (Exception $e) {
      // Try creating the file before creating an archiver.
      touch($realFilePath);
      $archiver = new $implementation['class']($realFilePath);
    }
    $this->archiveDirectory($archiver, $directory);
    unset($archiver);

    // Ensure archive exists for completeness.
    if (!file_exists($realFilePath)) {
      touch($realFilePath);
    }

    return $filePath;
  }

  /**
   * Adds a directory to an archive relative to its parent directory.
   */
  protected function archiveDirectory(ArchiverInterface $archiver, $directory) {
    // Prepare working directories.
    $originalBase = getcwd();
    $archiveBase = drupal_dirname($directory);

    // Add files in directory recursively.
    chdir(drupal_realpath($archiveBase));
    $directoryBase = drupal_basename($directory);
    $files = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($directoryBase), RecursiveIteratorIterator::SELF_FIRST);
    foreach ($files as $file) {
      /** @var $file SplFileInfo */
      if ($file->isFile()) {
        $archiver->add($file->getPathname());
      }
    }
    chdir($originalBase);
  }

  /**
   * Generates a temporary file name.
   */
  protected function generateTemporaryFile($extension = '', $destination = 'temporary://') {
    $destination = substr($destination, -1) == '/' ? $destination : "$destination/";
    $extension = strlen($extension) ? ".$extension" : '';
    $filePath = NULL;
    do {
      $filePath = $destination . uniqid('ph_package_archive_') . $extension;
    }
    while (file_exists($filePath));
    return $filePath;
  }

  /**
   * Removes the temporary folder on destruction.
   */
  public function __destruct() {
    if (file_exists($this->workingDirectory)) {
      file_unmanaged_delete_recursive($this->workingDirectory);
    }
  }
}

/**
 * Exception thrown in the archiver.
 */
class PHPackageArchiveException extends Exception {}

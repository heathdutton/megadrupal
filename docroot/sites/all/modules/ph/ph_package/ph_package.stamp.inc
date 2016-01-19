<?php
/**
 * @file
 * Stamp implementation.
 */

/**
 * Stamp factory interface.
 */
interface PHPackageStampFactoryInterface {
  /**
   * Creates a stamp.
   *
   * @param string $directory
   *   Path to the directory containing an exported package.
   * @return PHPackageStampInterface
   *   Created stamp.
   */
  public function createStamp($directory);
}

/**
 * Default stamp factory.
 */
class PHPackageDefaultStampFactory implements PHPackageStampFactoryInterface {
  /**
   * Creates a stamp.
   *
   * @param string $directory
   *   Path to the directory containing an exported package.
   * @return PHPackageStampInterface
   *   Created stamp.
   */
  public function createStamp($directory) {
    return new PHPackageDefaultStamp($directory);
  }
}

/**
 * Stamp interface.
 */
interface PHPackageStampInterface {
  /**
   * Stamps a package.
   *
   * @param array $metadata
   *   Package metadata. Specific implementations may choose to extend this. See
   *   ph_package_stamp() for supported values.
   *
   * @see ph_package_stamp()
   */
  public function stamp(array $metadata);
}

/**
 * Default package stamp.
 */
class PHPackageDefaultStamp implements PHPackageStampInterface {
  /**
   * Package directory.
   * @var string
   */
  protected $directory;

  /**
   * Constructs a stamp for a directory.
   */
  public function __construct($directory) {
    $this->directory = $directory;
  }

  /**
   * Stamps a package.
   *
   * @param array $metadata
   *   Package metadata. Specific implementations may choose to extend this. See
   *   ph_package_stamp() for supported values.
   *
   * @see ph_package_stamp()
   */
  public function stamp(array $metadata) {
    // Prepare for stamping.
    $metadata = $this->prepareMetadata($metadata);

    // List info files.
    $files = $this->listInfoFiles($this->directory);

    // Stamp info files.
    foreach ($files as $file) {
      $this->stampInfoFile($file, $metadata);
    }
  }

  /**
   * Preprocesses metadata.
   */
  protected function prepareMetadata(array $metadata) {
    // Set default status URL if updates module is also enabled.
    if (!isset($metadata['project_status_url']) && module_exists('ph_updates')) {
      $feed_path = variable_get('ph_updates_feed_path');
      if (!empty($feed_path) && ph_updates_valid_feed_path($feed_path)) {
        $metadata['project_status_url'] = url($feed_path, array('absolute' => TRUE));
      }
    }

    return $metadata;
  }

  /**
   * Lists info files.
   *
   * @param $directory
   *   Directory to search in.
   * @return array
   *   List of file paths.
   */
  protected function listInfoFiles($directory) {
    $info_pattern = '/^' . DRUPAL_PHP_FUNCTION_PATTERN . '\.info$/';
    return array_keys(file_scan_directory($directory, $info_pattern));
  }

  /**
   * Stamps an info file.
   */
  protected function stampInfoFile($file, array $metadata) {
    if ($f = fopen($file, 'a')) {
      fwrite($f, "\n");
      $formattedMetadata = $this->formatMetadata($metadata);
      fwrite($f, $formattedMetadata);
      fclose($f);
    }
  }

  /**
   * Formats metadata for insertion into info file.
   */
  protected function formatMetadata(array $metadata) {
    $text = $this->getMetadataHeader($metadata);
    $text .= "\n";
    $text .= $this->getMetadataContent($metadata);
    $text .= "\n";
    return $text;
  }

  /**
   * Formats metadata header.
   */
  protected function getMetadataHeader(array $metadata) {
    $header = ph_package_get_stamp_header();
    if ($header) {
      $lines = preg_split('/\r?\n/', $header);
      foreach ($lines as &$line) {
        $line = '; ' . $line;
      }
      unset($line);
      $header = implode("\n", $lines);
    }
    return rtrim($header);
  }

  /**
   * Formats metadata text.
   */
  protected function getMetadataContent(array $metadata) {
    $content = $this->prepareMetadataContent($metadata);
    $lines = array();
    foreach ($content as $key => $value) {
      $lines[] = sprintf('%s = "%s"', $key, $value);
    }
    return implode("\n", $lines);
  }

  /**
   * Prepares metadata content for formatting.
   */
  protected function prepareMetadataContent(array $metadata) {
    $content = array();
    // Add version information.
    if (isset($metadata['version'])) {
      $version = $metadata['version'];
      $content['version'] = ph_version_format($version);
      if (!ph_version_is_core($version)) {
        $content['core'] = ph_version_format($version, 'core_api');
      }
    }
    // Add project name.
    if (isset($metadata['project'])) {
      $content['project'] = $metadata['project'];
    }
    // Add project update feed URL.
    if (isset($metadata['project_status_url'])) {
      $content['project status url'] = $metadata['project_status_url'];
    }
    // TODO Add datestamp.
    return $content;
  }
}

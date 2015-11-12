<?php

/**
 * @file
 * Apple News article preview generator class.
 */

/**
 * This class generate downloadable Apple News Native formatted
 * documents with it's assets.
 */
class ApplenewsPreview {

  /**
   * URI to individual entity export directory.
   *
   * @var string
   */
  private $entity_dir;

  /**
   * Real path to individual entity export directory.
   *
   * @var string
   */
  private $entity_realpath;

  /**
   * Apple News article assets.
   *
   * @var array
   */
  private $files = [];

  /**
   * Apple News Native document formatted JSON string.
   *
   * @var int|string
   */
  private $article_json;

  /**
   * Real path to the batch preview export archive file.
   *
   * @var string
   */
  private $archive_file;

  /**
   * URL to the archive file.
   *
   * @var int|string
   */
  private $archive_url;

  /**
   * Real path to the batch preview directory.
   *
   * @var string
   */
  private $archive_realpath;

  /**
   * Entity ID of the current preview object.
   *
   * @var int
   */
  private $entity_id;

  /**
   * Indicates whether this is an individual entity export or not.
   *
   * @var boolean
   */
  private $entity_archive;

  /**
   * URI to the main Apple News export directory.
   *
   * @var string
   */
  private $directory;

  /**
   * Batch preview export archive file name.
   *
   * @var string
   */
  private $archive = '';

  /**
   * Implements __construct().
   * Initialize all required variables.
   */
  public function __construct($entity_id, $filename, $entity_archive = FALSE, array $data = []) {

    $this->directory = variable_get('applenews_export_directory', 'applenews/');
    $this->archive = !empty($filename) ? 'applenews-' . $filename . '.zip' : 'applenews.zip';
    $this->archive_realpath = drupal_realpath(file_build_uri($this->directory));
    $this->archive_file = drupal_realpath(file_build_uri($this->directory . $this->archive));
    $this->archive_url = file_create_url(file_build_uri($this->directory . $this->archive));

    if ($entity_id) {
      $drupal_entity_dir = file_build_uri($this->directory . $entity_id);
      $this->entity_dir = $drupal_entity_dir;
      $this->entity_realpath = drupal_realpath($drupal_entity_dir);
      // Boolean value that indicated if we should create tmp archive file for an entity.
      $this->entity_archive = $entity_archive;
      $this->entity_id = $entity_id;
      if (count($data)) {
        $this->files = $data['files'];
        $this->article_json = $data['json'];
      }
      if ($entity_archive) {
        $this->removeDirectories([$this->entity_id]);
      }
      file_prepare_directory($drupal_entity_dir, FILE_CREATE_DIRECTORY);
    }

  }

  /**
   * Save JSON string into article.json file.
   */
  private function saveArticleJSON() {
    file_unmanaged_save_data($this->article_json, $this->entity_dir . '/article.json');
  }

  /**
   * Save article assets into article directory.
   */
  private function saveArticleAssets() {
    foreach ($this->files as $url => $path) {
      $contents = file_get_contents($path);
      file_unmanaged_save_data($contents, $this->entity_dir . '/' . basename($url));
    }
  }

  /**
   * Scan a directory and return a list of file names and directories.
   */
  private function scanDirectory($path) {
    $items = array_values(array_filter(scandir($path), function($file) {
      return !is_dir($file);
    }));
    return $items;
  }

  /**
   * Convert \ZipArchive::open() error code to message.
   *
   * @see http://php.net/manual/en/ziparchive.open.php
   */
  public static function zipErrorMsg($error_code) {
    switch ($error_code) {

      case \ZipArchive::ER_EXISTS:
        return 'File already exists.';

      case \ZipArchive::ER_INCONS:
        return 'Zip archive inconsistent.';

      case \ZipArchive::ER_INVAL:
        return 'Invalid argument.';

      case \ZipArchive::ER_MEMORY:
        return 'Malloc failure.';

      case \ZipArchive::ER_NOENT:
        return 'No such file.';

      case \ZipArchive::ER_NOZIP:
        return 'Not a zip archive.';

      case \ZipArchive::ER_OPEN:
        return 'Can\'t open file.';

      case \ZipArchive::ER_READ:
        return 'Read error.';

      case \ZipArchive::ER_SEEK:
        return 'Seek error.';

    }
    return 'Unknown error.';
  }

  /**
   * Create [article-id].zip file archive.
   */
  private function createArchive($entity_ids = []) {

    // Start creating a new archive file.
    if (!class_exists('\ZipArchive')) {
      throw new Exception('Feature requires PHP Zip extension.');
    }
    $zip = new \ZipArchive();

    if ($this->entity_archive) {

      $entity_archive_realpath = $this->archive_realpath . '/' . $this->entity_id;
      $entity_archive = $entity_archive_realpath . '.zip';

      // Make sure to remove archive file first.
      if (file_exists($entity_archive)) {
        file_unmanaged_delete($entity_archive);
      }

      // Open archive.
      $result = $zip->open($entity_archive, \ZipArchive::OVERWRITE);
      if ($result !== TRUE) {
        throw new Exception('Could not open archive file: ' .
          $this::zipErrorMsg($result));
      }
      // Create an archive of an article assets and content.
      foreach ($this->scanDirectory($entity_archive_realpath) as $item) {
        $zip->addFile($entity_archive_realpath . '/' . $item, $this->entity_id . '/' . $item);
      }

    }
    else {

      // Open archive.
      $result = $zip->open($this->archive_file, \ZipArchive::OVERWRITE);
      if ($result !== TRUE) {
        throw new Exception('Could not open archive file: ' .
          $this::zipErrorMsg($result));
      }

      // Scan through all entity directories and add each file to an archive.
      foreach ($this->scanDirectory($this->archive_realpath) as $item) {
        $dir = $this->archive_realpath . '/' . $item;
        if (is_dir($dir) && in_array($item, $entity_ids)) {
          $zip->addEmptyDir($item);
          $files = $this->scanDirectory($this->archive_realpath . '/' . $item);
          foreach ($files as $file) {
            $zip->addFile($this->archive_realpath . '/' . $item . '/' . $file, $item . '/' . $file);
          }
        }
      }

    }

    // Close and save archive.
    $zip->close();

  }

  /**
   * Removes directories.
   */
  public function removeDirectories($entity_ids = []) {
    if (is_dir($this->archive_realpath)) {
      foreach ($this->scanDirectory($this->archive_realpath) as $file) {
        $dir = $this->archive_realpath . '/' . $file;
        if (is_dir($dir) && in_array($file, $entity_ids)) {
          file_unmanaged_delete_recursive($dir);
        }
      }
    }
  }

  /**
   * Delete individual entity archive.
   */
  public function entityArchiveDelete($entity_id) {
    $archive_file = $this->archive_realpath . '/' . $entity_id . '.zip';
    file_unmanaged_delete_recursive($archive_file);
  }

  /**
   * Returns URL path to an archive file.
   */
  public function getArchiveFilePath() {
    return $this->archive_url;
  }

  /**
   * Get path to the individula entity archive.
   */
  public function getEntityArchivePath($entity_id) {
    return $this->archive_realpath . '/' . $entity_id;
  }

  /**
   * Export entities to files.
   */
  public function toFile() {
    $this->saveArticleJSON();
    $this->saveArticleAssets();
  }

  /**
   * Generate downloadable zip file.
   */
  public function archive($entity_ids = []) {
    $this->createArchive($entity_ids);
  }

}

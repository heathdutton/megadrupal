<?php

/**
 * @file
 * Contains \Drupal\flysystem\Flysystem\Local.
 */

namespace Drupal\flysystem\Flysystem;

use Drupal\flysystem\Flysystem\Adapter\MissingAdapter;
use Drupal\flysystem\Plugin\FlysystemPluginBase;
use League\Flysystem\Adapter\Local as LocalAdapter;

/**
 * Drupal plugin for the "Local" Flysystem adapter.
 */
class Local extends FlysystemPluginBase {

  /**
   * Whether the directory was recently created.
   *
   * @var bool
   */
  protected $created = FALSE;

  /**
   * The permissions to create directories with.
   *
   * @var int
   */
  protected $directoryPerm;

  /**
   * The root directory as it was supplied by the user.
   *
   * @var string
   */
  protected $originalRoot;

  /**
   * Whether the root is in the public path.
   *
   * @var string|false
   */
  protected $publicPath = FALSE;

  /**
   * The root of the local adapter.
   *
   * @var string
   */
  protected $root;

  /**
   * Constructs a Local object.
   *
   * @param string $public_filepath
   *   The path to the public files directory.
   * @param string $root
   *   The of the adapter's filesystem.
   * @param bool $is_public
   *   (optional) Whether this is a public file system. Defaults to false.
   * @param int $directory_permission
   *   (optional) The permissions to create directories with.
   */
  public function __construct($public_filepath, $root, $is_public = FALSE, $directory_permission = 0775) {
    $this->originalRoot = $root;
    $this->directoryPerm = $directory_permission;
    // ensureDirectory() sets the created flag.
    if (!$this->root = $this->ensureDirectory($root)) {
      return;
    }

    if ($is_public) {
      $this->publicPath = $this->pathIsPublic($public_filepath, $this->root);
    }

    // If the directory was recently created, write the .htaccess file.
    if ($this->created && $this->publicPath === FALSE) {
      $this->writeHtaccess($this->root);
    }
  }

  /**
   * {@inheritdoc}
   */
  public static function create(array $configuration) {
    return new static(
      variable_get('file_public_path', conf_path() . '/files'),
      $configuration['root'],
      !empty($configuration['public']),
      variable_get('file_chmod_directory', 0775)
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getAdapter() {
    if ($this->root) {
      return new LocalAdapter($this->root);
    }

    return new MissingAdapter();
  }

  /**
   * {@inheritdoc}
   */
  public function getExternalUrl($uri) {
    if ($this->publicPath === FALSE) {
      return parent::getExternalUrl($uri);
    }

    $path = str_replace('\\', '/', $this->publicPath . '/' . $this->getTarget($uri));

    return $GLOBALS['base_url'] . '/' . drupal_encode_path($path);
  }

  /**
   * {@inheritdoc}
   */
  public function ensure($force = FALSE) {
    if (!$this->root) {
      return array(array(
        'severity' => WATCHDOG_ERROR,
        'message' => 'The %root directory either does not exist or is not readable and attempts to create it have failed.',
        'context' => array('%root' => $this->originalRoot),
      ));
    }

    if ($this->publicPath !== FALSE || $this->writeHtaccess($this->root, $force)) {
      return array();
    }

    return array(array(
      'severity' => WATCHDOG_ERROR,
      'message' => 'See <a href="@url">@url</a> for information about the recommended .htaccess file which should be added to the %directory directory to help protect against arbitrary code execution.',
      'context' => array(
        '%directory' => $this->root,
        '@url' => 'https://www.drupal.org/SA-CORE-2013-003',
      ),
    ));
  }

  /**
   * Returns the public path of the adapter, if present.
   *
   * @return string|false
   *   The public path, or false.
   */
  public function getPublicPath() {
    return $this->publicPath;
  }

  /**
   * Checks that the directory exists and is readable.
   *
   * This will attempt to create the directory if it doesn't exist.
   *
   * @param string $directory
   *   The directory.
   *
   * @return string|false
   *   The path of the directory, or false on failure.
   */
  protected function ensureDirectory($directory) {
    // Go for the success case first.
    if (is_dir($directory) && is_readable($directory)) {
      return $directory;
    }

    if (!file_exists($directory)) {
      mkdir($directory, $this->directoryPerm, TRUE);
    }

    if (is_dir($directory) && chmod($directory, $this->directoryPerm)) {
      clearstatcache(TRUE, $directory);
      $this->created = TRUE;
      return $directory;
    }

    return FALSE;
  }

  /**
   * Writes an .htaccess file.
   *
   * @param string $directory
   *   The directory to write the .htaccess file.
   * @param bool $force
   *   Whether to overwrite an existing file.
   *
   * @return bool
   *   True on success, false on failure.
   */
  protected function writeHtaccess($directory, $force = FALSE) {
    $htaccess_path = $directory . '/.htaccess';

    if (file_exists($htaccess_path) && !$force) {
      // Short circuit if the .htaccess file already exists.
      return TRUE;
    }

    // Write the .htaccess file.
    if (is_dir($directory) && is_writable($directory)) {
      return file_put_contents($htaccess_path, file_htaccess_lines()) && chmod($htaccess_path, 0444);
    }

    return FALSE;
  }

  /**
   * Determines if the path is inside the public path.
   *
   * @param string $public_filepath
   *   The path to the public files directory.
   * @param string $root
   *   The root path.
   *
   * @return string|false
   *   The public path, or false.
   */
  protected function pathIsPublic($public_filepath, $root) {
    $root = realpath($root);

    if (!$public = realpath($public_filepath)) {
      return FALSE;
    }

    if (strpos($root . DIRECTORY_SEPARATOR, $public . DIRECTORY_SEPARATOR) === 0) {
      return $public_filepath . substr($root, strlen($public));
    }

    return FALSE;
  }

}

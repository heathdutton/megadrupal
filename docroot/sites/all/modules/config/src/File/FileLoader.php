<?php

/**
 * @file
 * Contains a FileLoader
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

namespace Drupal\config\File;

use Drupal\config\Exception\ConfigException;

/**
 * Class FileLoader
 * @package Drupal\config\File
 */
class FileLoader {

  /**
   * Lazy constructor.
   *
   * @return static
   *   An instance of FileLoader
   * @static
   */
  static public function create() {

    return new static();
  }

  /**
   * Constructor.
   */
  public function __construct() {
    // Nothing to do.
  }

  /**
   * Construct a path to a folder.
   *
   * @param string $config_module
   *   Module holding the config.
   * @param string $config_path
   *   Path from the folder root
   *
   * @return string
   *   A full system path to the config folder.
   */
  public function getFolderPath($config_module, $config_path) {

    $module_path = drupal_get_path('module', $config_module);
    $config_full_path = $module_path . '/' . $config_path;

    return $config_full_path;
  }

  /**
   * Get the full file path to a config file.
   *
   * @param string $config_full_path
   *   Full path to the config folder
   * @param string $filename
   *   File name
   *
   * @return string
   *   Full path to a file.
   */
  public function getFilePath($config_full_path, $filename = 'config.json') {

    $base_config_path = $config_full_path . '/' . $filename;

    return $base_config_path;
  }

  /**
   * Load a config file.
   *
   * @param string $path
   *   Path to load
   *
   * @return string
   *   Contents of the file.
   *
   * @throws \Drupal\config\Exception\ConfigException
   */
  public function loadFile($path) {

    if (empty($path)) {
      throw new ConfigException('No path provided for Config file.');
    }

    if (!file_exists($path)) {
      watchdog('Config', 'Invalid file path supplied to parser.');
      throw new ConfigException('Invalid file path supplied to parser.');
    }

    $base_config_file = file_get_contents($path);

    return $base_config_file;
  }
}

<?php

/**
 * @file
 * Contains a ParserBase
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2014 Christopher Skene
 */

namespace Drupal\config\Parser;

use Drupal\config\File\FileLoader;

/**
 * Class ParserBase
 * @package Drupal\fabricator\Parser
 */
abstract class ParserBase implements ParserInterface {

  /**
   * File Loader
   *
   * @var FileLoader
   */
  public $fileLoader;

  /**
   * {@inheritdoc}
   */
  static public function create() {
    $file_loader = FileLoader::create();

    return new static($file_loader);
  }

  /**
   * Constructor.
   *
   * @param \Drupal\config\File\FileLoader $file_loader
   *   A file loader instance.
   */
  public function __construct(FileLoader $file_loader) {
    $this->fileLoader = $file_loader;

  }

  /**
   * {@inheritdoc}
   */
  public function parseConfig($module, $path, $filename) {

    $config_full_path = $this->fileLoader->getFolderPath($module, $path);

    $base_config_path = $this->fileLoader->getFilePath($config_full_path, $filename);

    return $this->getContent($base_config_path);
  }
}

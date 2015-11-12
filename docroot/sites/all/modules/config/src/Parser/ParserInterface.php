<?php

/**
 * @file
 * Contains a ParserInterface
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2014 Christopher Skene
 */

namespace Drupal\config\Parser;

/**
 * Interface ParserInterface
 * @package Drupal\config\Parser
 */
interface ParserInterface {

  /**
   * Lazy factory.
   *
   * @return static
   *   An instance of ParserInterface
   * @static
   */
  public static function create();

  /**
   * Get the content.
   *
   * @param string $path
   *   The path to load.
   *
   * @return array
   *   An array of defined vars
   */
  public function getContent($path);

  /**
   * Parse a config file
   *
   * @param string $module
   *   Name of the module providing the configuration
   * @param string $path
   *   [optional] The directory path within the module. Defaults to 'config'
   * @param string $filename
   *   [optional] Name of the config file to load. Defaults to config.json
   *
   * @return array
   *   An array of configuration.
   */
  public function parseConfig($module, $path, $filename);
}

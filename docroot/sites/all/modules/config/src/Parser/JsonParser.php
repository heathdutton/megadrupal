<?php

/**
 * @file
 * Contains a JsonParser
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

namespace Drupal\config\Parser;

/**
 * Class JsonParser
 * @package Drupal\config\Parser
 */
class JsonParser extends ParserBase implements ParserInterface {

  /**
   * Get the content.
   *
   * @param string $path
   *   The path to load.
   *
   * @return array
   *   An array of defined vars
   */
  public function getContent($path) {

    $base_config_file = $this->fileLoader->loadFile($path);

    return json_decode($base_config_file, TRUE);
  }

}

<?php

/**
 * @file
 * Contains a PhpParser.
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2014 Christopher Skene
 */

namespace Drupal\config\Parser;

/**
 * Class PhpParser
 * @package Drupal\config\Parser
 */
class PhpParser extends ParserBase implements ParserInterface {

  /**
   * {@inheritdoc}
   */
  public function getContent($config_file_path) {

    ob_start();
    include(DRUPAL_ROOT . '/' . $config_file_path);
    ob_end_clean();

    unset($config_file_path);

    $vars = get_defined_vars();

    return $vars;
  }
}

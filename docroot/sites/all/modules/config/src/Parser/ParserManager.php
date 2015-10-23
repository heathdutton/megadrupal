<?php

/**
 * @file
 * Contains a ParserManager
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2014 Christopher Skene
 */

namespace Drupal\config\Parser;

use Drupal\config\Exception\ConfigException;
use Drupal\config\Parser\ParserInterface;

/**
 * Class ParserManager
 * @package Drupal\config\Parser
 */
class ParserManager {

  /**
   * Constructor.
   */
  public function __construct() {

  }

  /**
   * Lazy constructor.
   *
   * @return ParserManager
   *   This object.
   */
  static public function init() {
    return new static();
  }

  /**
   * Get Parser information.
   *
   * @todo: Cache
   *
   * @return array
   *   An array of parsers.
   */
  public function getAllParserInfo() {

    $implementers = $this->getParserImplementers();
    if (empty($implementers)) {
      return array();
    }

    $parsers = array();
    foreach ($implementers as $module_name) {

      $parser_configs = $this->getParserModuleConfig($module_name);
      if (empty($parser_configs)) {
        continue;
      }

      foreach ($parser_configs as $parser_name => $parser_details) {
        $defaults = array(
          'name' => '',
          'class' => NULL,
          'provider' => $module_name,
          'machine_name' => $parser_name,
          'extension' => '',
        );

        $parsers[$parser_name] = array_merge($defaults, $parser_details);
      }
    }

    return $parsers;
  }

  /**
   * Fetch information about an individual parser, without loading it.
   *
   * @param string $parser_name
   *   The parser name
   *
   * @return array
   *   An array of parser information
   * @throws \Drupal\config\Exception\ConfigException
   *   Throws a ConfigException if not found.
   */
  public function getParserInfo($parser_name) {

    $parsers = $this->getAllParserInfo();

    if (array_key_exists($parser_name, $parsers)) {
      return $parsers[$parser_name];
    }

    throw new ConfigException(sprintf('Could not find parser matching %s', $parser_name));
  }

  /**
   * Get an individual parser.
   *
   * @param string $name
   *   Name of the parser.
   *
   * @return ParserInterface
   *   A Parser.
   *
   * @throws ConfigException
   */
  public function getParser($name) {

    $parsers = $this->getAllParserInfo();

    if (array_key_exists($name, $parsers)) {
      if (class_exists($parsers[$name]['class'])) {

        /* @var $class ParserInterface */
        $class = $parsers[$name]['class'];
        $parser = $class::create();

        if ($parser instanceof ParserInterface) {
          return $parser;
        }
      }
    }

    throw new ConfigException(sprintf('Invalid Parser: %s', $name));
  }

  /**
   * Wrapper around module_implements for config_parsers.
   *
   * @return array
   *   An array of parser implementations by module.
   */
  protected function getParserImplementers() {

    $implementers = module_implements('config_parsers');
    return $implementers;
  }

  /**
   * Wrapper around module_invoke for config_parsers.
   *
   * @param string $module_name
   *   Module to invoke.
   *
   * @return array
   *   Array of settings for parsers.
   */
  protected function getParserModuleConfig($module_name) {

    $module_factories = module_invoke($module_name, 'config_parsers');
    return $module_factories;
  }
}

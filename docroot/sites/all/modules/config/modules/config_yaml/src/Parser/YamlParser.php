<?php

/**
 * @file
 * Contains a YamlParser
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

namespace Drupal\config_yaml\Parser;

use Drupal\config\Exception\ConfigException;
use Drupal\config\Parser\ParserBase;
use Drupal\config\Parser\ParserInterface;
use Symfony\Component\Yaml\Yaml;

/**
 * Class YamlParser
 * @package Drupal\config\Parser
 */
class YamlParser extends ParserBase implements ParserInterface {

  /**
   * {@inheritdoc}
   */
  public function getContent($path) {

    if (!class_exists('Symfony\Component\Yaml\Yaml')) {
      watchdog('config_yaml', 'You may need to run Composer to install the Symfony Yaml component', WATCHDOG_DEBUG);
      throw new ConfigException('Symfony Yaml component not found.');
    }

    $yaml = Yaml::parse($path);

    return $yaml;
  }

}

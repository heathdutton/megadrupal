<?php

/**
 * @file
 * Contains a
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

namespace Drupal\config\Tests\Parser;

use Drupal\config\Exception\ConfigException;
use Drupal\config\Tests\ConfigTestBase;
use Drupal\config\Parser\ParserInterface;

/**
 * Class ParserManagerTest
 * @package Drupal\config\Tests\Parser
 */
class ParserManagerTest extends ConfigTestBase {

  /**
   * The Parser manager
   *
   * @var MockParserManager
   */
  protected $parserManager;

  public function setup() {
    $this->parserManager = MockParserManager::init();
  }

  public function testGetParsers() {
    $parsers = $this->parserManager->getAllParserInfo();

    $this->assertArrayHasKey(CONFIG_PARSER_JSON, $parsers);
  }

  public function testGetParser() {

    try {
      $parser = $this->parserManager->getParser(CONFIG_PARSER_JSON);
    }
    catch(ConfigException $e) {
      $this->fail($e->getMessage());
    }

    $this->assertTrue($parser instanceof ParserInterface);
  }
}

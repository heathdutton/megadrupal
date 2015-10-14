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

use Drupal\config\File\FileLoader;
use Drupal\config\Tests\ConfigTestBase;
use Drupal\config\Parser\ParserInterface;
use Drupal\config\Parser\JsonParser;
use Drupal\config\Tests\Parser\MockParserManager as ParserManager;

/**
 * Class JsonParserTest
 * @package Drupal\config\Tests\Parser
 */
class JsonParserTest extends ConfigTestBase{

  /**
   * The Parser manager
   *
   * @var ParserManager
   */
  protected $parserManager;

  /**
   * The JsonParser
   *
   * @var JsonParser
   */
  protected $jsonParser;

  public function setup() {
    $this->parserManager = ParserManager::init();
  }

  public function testGetContent() {
    $this->jsonParser = $this->parserManager->getParser(CONFIG_PARSER_JSON);

    $this->assertTrue($this->jsonParser instanceof ParserInterface);
    $this->assertInstanceOf('Drupal\config\File\FileLoader', $this->jsonParser->fileLoader);

    $contents = $this->jsonParser->getContent(DRUPAL_ROOT . '/sites/all/modules/config/src/Tests/config/config.json');

    $this->assertArrayHasKey('example', $contents);
  }

}

<?php

/**
 * @file
 * Tests for smartling.
 */

namespace Drupal\smartling\Alters;

use Drupal\smartling\Alters\SmartlingContentProcessorInterface;
use Drupal\smartling\Alters\SmartlingContentImageUrlParser;

/**
 * Class MockedSmartlingContentImageUrlProcessor.
 */
class MockedSmartlingContentImageUrlProcessor implements SmartlingContentProcessorInterface {
  protected $processFunc;

  /**
   * Init.
   *
   * @param mixed $callable
   *   Callable.
   */
  public function __construct($callable) {
    $this->processFunc = $callable;
  }

  /**
   * Process.
   *
   * @param string $item
   *   Item.
   * @param mixed $context
   *   Context.
   * @param string $lang
   *   Language.
   * @param string $field_name
   *   File name.
   * @param object $entity
   *   Entity object.
   */
  public function process(&$item, $context, $lang, $field_name, $entity) {
    // A hack that allows to pass a parameter by reference to call_user_func.
    // http://stackoverflow.com/questions/295016/is-it-possible-to-pass-
    // parameters-by-reference-using-call-user-func-array
    $tmp = array(&$item);
    call_user_func($this->processFunc, $tmp, $context, $lang, $field_name, $entity);
    $item = $tmp[0];
  }
}

/**
 * SmartlingContentImageUrlParserTest.
 */
class SmartlingContentImageUrlParserTest extends \PHPUnit_Framework_TestCase {
  protected $context;
  protected $entity;

  /**
   * Get info.
   *
   * @return array
   *   Return info array.
   */
  public static function getInfo() {
    return array(
      'name' => 'Content - Smartling Content Urls/image Parser',
      'description' => 'Unit tests for url parser',
      'group' => 'Smartling UnitTests',
    );
  }

  /**
   * Setup.
   */
  protected function setUp() {
    parent::setUp();

    $this->entity = new \stdClass();
  }

  /**
   * Test should convert links.
   */
  public function testShouldConvertLinks() {
    $content = 'cool <a href="http://google.com.ua">Home page</a> wow';
    $content_res = 'cool <a href="http://hello.wrld/">Home page</a> wow';
    $proc = new MockedSmartlingContentImageUrlProcessor(function($item, $context, $lang, $field_name, $entity){
      // Please see the process method above
      // for explanation of first index: [0].
      $item[0][2] = 'http://hello.wrld/';
    });
    $parse = new SmartlingContentImageUrlParser(array($proc));
    $content = $parse->parse($content, 'en', 'field_body', $this->entity);

    $this->assertEquals($content, $content_res, 'Test should convert links');
  }
}

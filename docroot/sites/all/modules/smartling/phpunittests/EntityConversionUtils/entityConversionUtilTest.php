<?php
require_once(dirname(__FILE__) . '/../../lib/Drupal/smartling/Settings/SmartlingSettingsHandler.php');
require_once(dirname(__FILE__) . '/../../lib/Drupal/smartling/Wrappers/FieldAPIWrapper.php');
require_once(dirname(__FILE__) . '/../../lib/Drupal/smartling/Wrappers/DrupalAPIWrapper.php');
require_once(dirname(__FILE__) . '/../../lib/Drupal/smartling/Wrappers/SmartlingUtils.php');
require_once(dirname(__FILE__) . '/../../lib/Drupal/smartling/EntityConversionUtils/EntityConversionInterface.php');
require_once(dirname(__FILE__) . '/../../lib/Drupal/smartling/EntityConversionUtils/EntityConversionUtil.php');
require_once(dirname(__FILE__) . '/../../lib/Drupal/smartling/EntityConversionUtils/NodeConversionUtil.php');

define('LANGUAGE_NONE', 'und');
/**
 * @file
 * Tests for smartling.
 */
use Drupal\smartling\EntityConversionUtils;

/**
 * SmartlingFileCleanTest.
 */
class entityConversionUtilTest extends \PHPUnit_Framework_TestCase {
  public static function randomName($length = 8) {
    $values = array_merge(range(65, 90), range(97, 122), range(48, 57));
    $max = count($values) - 1;
    $str = chr(mt_rand(97, 122));
    for ($i = 1; $i < $length; $i++) {
      $str .= chr($values[mt_rand(0, $max)]);
    }
    return $str;
  }

  protected function customCreateNode($settings = array(), $node_lang = LANGUAGE_NONE, $field_lang = LANGUAGE_NONE) {
    // Populate defaults array.
    $settings += array(
      'body'      => array($field_lang => array(array())),
      'title'     => $this->randomName(8),
      'comment'   => 2,
      'changed'   => REQUEST_TIME,
      'moderate'  => 0,
      'promote'   => 0,
      'revision'  => 1,
      'log'       => '',
      'status'    => 1,
      'sticky'    => 0,
      'type'      => 'page',
      'revisions' => NULL,
      'language'  => $node_lang,
    );

    // Use the original node's created time for existing nodes.
    if (isset($settings['created']) && !isset($settings['date'])) {
      $settings['date'] = format_date($settings['created'], 'custom', 'Y-m-d H:i:s O');
    }

    // If the node's user uid is not specified manually, use the currently
    // logged in user if available, or else the user running the test.
    if (!isset($settings['uid'])) {
      $settings['uid'] = 1;
    }

    // Merge body field value and format separately.
    $body = array(
      'value' => $this->randomName(32),
      'format' => 'filtered_html',
    );
    $settings['body'][$field_lang][0] += $body;

    $node = (object) $settings;

    return $node;
  }

  public function setUp() {
    $this->settings = $this->getMockBuilder('\Drupal\smartling\Settings\SmartlingSettingsHandler')
      ->disableOriginalConstructor()
      ->getMock();

    $this->entity_api_wrapper = $this->getMockBuilder('\Drupal\smartling\Wrappers\EntityAPIWrapper')
      ->disableOriginalConstructor()
      ->getMock();

    $this->field_api_wrapper = $this->getMockBuilder('\Drupal\smartling\Wrappers\FieldAPIWrapper')
      ->disableOriginalConstructor()
      ->getMock();
    $this->drupal_api_wrapper = $this->getMockBuilder('\Drupal\smartling\Wrappers\DrupalAPIWrapper')
      ->disableOriginalConstructor()
      ->getMock();
    $this->smartling_utils = $this->getMockBuilder('\Drupal\smartling\Wrappers\SmartlingUtils')
      ->disableOriginalConstructor()
      ->getMock();
  }


  public function testConvertDefaultLangNeutral() {
    $this->drupal_api_wrapper->expects($this->once())
      ->method('getDefaultLanguage')
      ->will($this->returnValue(LANGUAGE_NONE));

    $obj = new EntityConversionUtils\EntityConversionUtil($this->settings, $this->entity_api_wrapper, $this->field_api_wrapper, $this->drupal_api_wrapper, $this->smartling_utils);


    $node = $this->customCreateNode();
    $res = $obj->convert($node, 'node');

    $this->assertEquals(FALSE, $res);
  }


  public function testConvertNoAllowedLanguage() {
    $this->drupal_api_wrapper->expects($this->once())
      ->method('getDefaultLanguage')
      ->will($this->returnValue('en'));

    $this->entity_api_wrapper->expects($this->once())
      ->method('getBundle')
      ->will($this->returnValue('article'));

    $this->settings->expects($this->once())
      ->method('getFieldsSettingsByBundle')
      ->will($this->returnValue(array()));

    $obj = new EntityConversionUtils\EntityConversionUtil($this->settings, $this->entity_api_wrapper, $this->field_api_wrapper, $this->drupal_api_wrapper, $this->smartling_utils);


    $node = $this->customCreateNode();
    $res = $obj->convert($node, 'node');

    $this->assertEquals(FALSE, $res);
  }


  public function testConvertFieldLangNeutral() {
    $this->drupal_api_wrapper->expects($this->once())
      ->method('getDefaultLanguage')
      ->will($this->returnValue('en'));

    $this->entity_api_wrapper->expects($this->once())
      ->method('getBundle')
      ->will($this->returnValue('article'));

    $this->settings->expects($this->once())
      ->method('getFieldsSettingsByBundle')
      ->will($this->returnValue(array('article' => array('title', 'body'))));



    $this->field_api_wrapper->expects($this->once())
      ->method('fieldLanguage')
      ->will($this->returnValue(array('uk')));

    $obj = new EntityConversionUtils\EntityConversionUtil($this->settings, $this->entity_api_wrapper, $this->field_api_wrapper, $this->drupal_api_wrapper, $this->smartling_utils);


    $node = $this->customCreateNode();
    $res = $obj->convert($node, 'node');

    $this->assertEquals(FALSE, $res);
  }

  public function testConvertToFieldsMethod() {
    $this->settings->expects($this->once())
      ->method('getFieldsSettingsByBundle')
      ->will($this->returnValue(array('article' => array('title', 'body'))));

    $this->drupal_api_wrapper->expects($this->once())
      ->method('getDefaultLanguage')
      ->will($this->returnValue('en'));

    $obj = $this->getMockBuilder('\Drupal\smartling\EntityConversionUtils\EntityConversionUtil')
      ->setConstructorArgs(array($this->settings, $this->entity_api_wrapper, $this->field_api_wrapper, $this->drupal_api_wrapper, $this->smartling_utils))
      ->setMethods(array('updateToNodeTranslateMethod', 'updateToFieldsTranslateMethod'))
      ->getMock();

    $obj->expects($this->once())->method('updateToFieldsTranslateMethod');


    $node = $this->customCreateNode();
    $obj->convert($node, 'node');
  }

  /**
   * Test correct copy fields when translated node not exist.
   */
  public function testPrepareForByFieldsTranslationUndUnd() {
    $settings = array('language' => LANGUAGE_NONE);
    $node = $this->customCreateNode($settings, LANGUAGE_NONE, LANGUAGE_NONE);

    $node_res = clone $node;
    $node_res->language = 'en';
    $node_res->body['en'] = $node_res->body[LANGUAGE_NONE];
    unset($node_res->body[LANGUAGE_NONE]);

    $this->field_api_wrapper->expects($this->once())
      ->method('fieldLanguage')
      ->will($this->returnValue(array('body'=>LANGUAGE_NONE)));

    $this->field_api_wrapper->expects($this->once())
      ->method('fieldGetItems')
      ->will($this->returnValue($node->body[LANGUAGE_NONE]));

    $obj = new EntityConversionUtils\EntityConversionUtil($this->settings, $this->entity_api_wrapper, $this->field_api_wrapper, $this->drupal_api_wrapper, $this->smartling_utils);
    $obj->updateToFieldsTranslateMethod($node, 'node', 'en', array('body'));

    $this->assertEquals($node_res->body, $node->body);
    $this->assertEquals($node->language, 'en');
  }


  public function testPrepareForByFieldsTranslationUndEn() {
    $settings = array('language' => LANGUAGE_NONE);
    $node = $this->customCreateNode($settings, LANGUAGE_NONE, 'en');

    $node_res = clone $node;
    $node_res->language = 'en';

    $this->field_api_wrapper->expects($this->once())
      ->method('fieldLanguage')
      ->will($this->returnValue(array('body'=>'en')));

    $obj = new EntityConversionUtils\EntityConversionUtil($this->settings, $this->entity_api_wrapper, $this->field_api_wrapper, $this->drupal_api_wrapper, $this->smartling_utils);
    $obj->updateToFieldsTranslateMethod($node, 'node', 'en', array('body'));

    $this->assertEquals($node_res->body, $node->body);
    $this->assertEquals($node->language, 'en');
  }

  public function testPrepareForByFieldsTranslationEnUnd() {
    $settings = array('language' => LANGUAGE_NONE);
    $node = $this->customCreateNode($settings, 'en', LANGUAGE_NONE);

    $node_res = clone $node;
    $node_res->body['en'] = $node_res->body[LANGUAGE_NONE];
    unset($node_res->body[LANGUAGE_NONE]);

    $this->field_api_wrapper->expects($this->once())
      ->method('fieldLanguage')
      ->will($this->returnValue(array('body'=>LANGUAGE_NONE)));

    $this->field_api_wrapper->expects($this->once())
      ->method('fieldGetItems')
      ->will($this->returnValue($node->body[LANGUAGE_NONE]));

    $obj = new EntityConversionUtils\EntityConversionUtil($this->settings, $this->entity_api_wrapper, $this->field_api_wrapper, $this->drupal_api_wrapper, $this->smartling_utils);
    $obj->updateToFieldsTranslateMethod($node, 'node', 'en', array('body'));

    $this->assertEquals($node_res->body, $node->body);
    $this->assertEquals($node->language, 'en');
  }

  public function testPrepareForByFieldsTranslationEnEn() {
    $settings = array('language' => LANGUAGE_NONE);
    $node = $this->customCreateNode($settings, 'en', 'en');

    $node_res = clone $node;

    $this->field_api_wrapper->expects($this->once())
      ->method('fieldLanguage')
      ->will($this->returnValue(array('body'=>'en')));

    $obj = new EntityConversionUtils\EntityConversionUtil($this->settings, $this->entity_api_wrapper, $this->field_api_wrapper, $this->drupal_api_wrapper, $this->smartling_utils);
    $obj->updateToFieldsTranslateMethod($node, 'node', 'en', array('body'));

    $this->assertEquals($node_res->body, $node->body);
    $this->assertEquals($node->language, 'en');
  }
}

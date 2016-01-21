<?php

/**
 * @file
 * Contains \Drupal\google_dfp\Tests\KeywordPluginTest.
 */

namespace Drupal\google_dfp\Tests;

/**
 * A PHP Unit test for Keyword plugins
 */
class KeywordPluginTest extends PluginTestBase {

  /**
   * Sets up the test.
   */
  public function setUp() {

  }

  /**
   * Tests the getKeywords method.
   *
   * @param string $keywords
   *   The expected return.
   */
  public function doTestGetKeywords($keywords) {
    $this->assertEquals($keywords, $this->plugin->getKeywords());
  }


  /**
   * Tests id, title and data.
   *
   * @param string $class
   *   Keyword class to test.
   * @param string $id
   *   Keyword id.
   * @param string $title
   *   Keyword title.
   * @param array $configuration
   *   Keyword configuration.
   * @param string $keywords
   *   Expected keyword return.
   *
   * @dataProvider getKeywords
   */
  public function testKeywords($class, $id, $title, $configuration, $keywords) {
    $this->plugin = new $class($id, $configuration);
    $this->doTestCreateInstance();
    $this->doTestGetId($id);
    $this->doTestGetTitle($title);
    $this->doTestGetKeywords($keywords);
    $this->doTestSetConfiguration();
  }

  /**
   * Data provider for testKeywords().
   *
   * @return array
   *   Array of tests cases.
   */
  public function getKeywords() {
    return array(
      array(
        '\Drupal\google_dfp\Plugin\GoogleDfp\Keyword\TestNodeTerm',
        'google_dfp_node_term_keywords',
        'Node term keyword',
        array('weight' => 0, 'fields' => array('field_foo' => 'field_foo')),
        array('field_foo'),
      ),
      array(
        '\Drupal\google_dfp\Plugin\GoogleDfp\Keyword\TestNodeTerm',
        'google_dfp_node_term_keywords',
        'Node term keyword',
        array(
          'weight' => 0,
          'fields' => array('field_baz' => 'field_baz'),
        ),
        array(),
      ),
      array(
        '\Drupal\google_dfp\Plugin\GoogleDfp\Keyword\TestNodeTerm',
        'google_dfp_node_term_keywords',
        'Node term keyword',
        array(
          'weight' => 0,
          'fields' => array(
            'field_foo' => 'field_foo',
            'field_bar' => 'field_bar',
          ),
        ),
        array('field_foo', 'field_bar'),
      ),
    );
  }
}

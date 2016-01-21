<?php

/**
 * @file
 * Contains \Drupal\google_dfp\Tests\TierPluginTest.
 */

namespace Drupal\google_dfp\Tests;

/**
 * A PHP Unit test for Tier plugins
 */
class TierPluginTest extends PluginTestBase {

  /**
   * Sets up the test.
   */
  public function setUp() {

  }

  /**
   * Tests the getTier method.
   *
   * @param string $tier
   *   The expected return.
   */
  public function doTestGetTier($tier) {
    $this->assertEquals($tier, $this->plugin->getTier());
  }


  /**
   * Tests id, title and data.
   *
   * @param string $class
   *   Tier class to test.
   * @param string $id
   *   Tier id.
   * @param string $title
   *   Tier title.
   * @param array $configuration
   *   Tier configuration.
   * @param string $tier
   *   Expected tier return.
   * @param array $submitted
   *   Form state array.
   *
   * @dataProvider getTiers
   */
  public function testTiers($class, $id, $title, $configuration, $tier, $submitted) {
    $this->plugin = new $class($id, $configuration);
    $this->doTestCreateInstance();
    $this->doTestGetId($id);
    $this->doTestGetTitle($title);
    $form = array();
    $this->doTestSettingsFormSubmit($form, $submitted);
    $this->doTestGetTier($tier);
    $this->doTestSetConfiguration();
  }

  /**
   * Data provider for testTiers().
   *
   * @return array
   *   Array of tests cases.
   */
  public function getTiers() {
    return array(
      array(
        '\Drupal\google_dfp\Plugin\GoogleDfp\Tier\TestSiteWide',
        'google_dfp_site',
        'Site wide tier',
        array('weight' => 0, 'value' => 'Foo'),
        'Foo',
        array(),
      ),
      array(
        '\Drupal\google_dfp\Plugin\GoogleDfp\Tier\TestPageTitle',
        'google_dfp_page_title',
        'Page title tier',
        array('weight' => 0),
        'Drupal',
        array(),
      ),
      array(
        '\Drupal\google_dfp\Plugin\GoogleDfp\Tier\TestPageTitle',
        'google_dfp_page_title',
        'Page title tier',
        array('weight' => 0, 'fallback' => 'Bar'),
        'Bar',
        array(),
      ),
      array(
        '\Drupal\google_dfp\Plugin\GoogleDfp\Tier\TestPlacement',
        'google_dfp_placement',
        'Placement id tier',
        array('weight' => 0, 'value' => ''),
        'myid',
        array(
          'values' => array(
            'tiers' => array(
              'google_dfp_placement' => array('weight' => 0, 'enabled' => 1),
            ),
          ),
          'item' => (object) array('placement' => 'myid'),
        ),
      ),
      array(
        '\Drupal\google_dfp\Plugin\GoogleDfp\Tier\TestNodeType',
        'google_dfp_node_type',
        'Node type tier',
        array('weight' => 0),
        'foo',
        array(),
      ),
      array(
        '\Drupal\google_dfp\Plugin\GoogleDfp\Tier\TestNodeType',
        'google_dfp_node_type',
        'Node type tier',
        array('weight' => 0, 'fallback' => 'bar'),
        'bar',
        array(),
      ),
      array(
        '\Drupal\google_dfp\Plugin\GoogleDfp\Tier\TestNodeTerm',
        'google_dfp_node_term',
        'Node term tier',
        array('weight' => 0, 'fields' => array('field_foo' => 'field_foo')),
        'foo',
        array(),
      ),
      array(
        '\Drupal\google_dfp\Plugin\GoogleDfp\Tier\TestNodeTerm',
        'google_dfp_node_term',
        'Node term tier',
        array(
          'weight' => 0,
          'fields' => array('field_baz' => 'field_baz'),
          'fallback' => 'baz',
        ),
        'baz',
        array(),
      ),
      array(
        '\Drupal\google_dfp\Plugin\GoogleDfp\Tier\TestNodeTerm',
        'google_dfp_node_term',
        'Node term tier',
        array(
          'weight' => 0,
          'concatenate' => 1,
          'fields' => array(
            'field_foo' => 'field_foo',
            'field_bar' => 'field_bar',
          ),
        ),
        'foo-bar',
        array(),
      ),
      array(
        '\Drupal\google_dfp\Plugin\GoogleDfp\Tier\TestNodeTerm',
        'google_dfp_node_term',
        'Node term tier',
        array(
          'weight' => 0,
          'concatenate' => 0,
          'fields' => array(
            'field_foo' => 'field_foo',
            'field_bar' => 'field_bar',
          ),
        ),
        'foo',
        array(),
      ),
      array(
        '\Drupal\google_dfp\Plugin\GoogleDfp\Tier\TestNodeTerm',
        'google_dfp_node_term',
        'Node term tier',
        array('weight' => 0, 'fallback' => 'foo', 'fields' => array()),
        'foo',
        array(),
      ),
    );
  }
}

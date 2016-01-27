<?php

/**
 * @file
 * Contains \Drupal\google_dfp\Tests\PluginTestBase.
 */

namespace Drupal\google_dfp\Tests;

/**
 * A PHP Unit test base for Google Dfp plugins
 */
class PluginTestBase extends \PHPUnit_Framework_TestCase {

  /**
   * Object under test.
   *
   * @var \Drupal\google_dfp\PluginInterface
   */
  protected $plugin;

  /**
   * Tests Drupal\google_dfp\PluginBase::createInstance.
   */
  public function doTestCreateInstance() {
    $class = get_class($this->plugin);
    $id = $this->plugin->getId();
    $configuration = $this->plugin->getConfiguration();

    $new = $class::createInstance($id, $configuration);
    $new_values = get_object_vars($new);
    $original = get_object_vars($this->plugin);
    $this->assertEquals($original, $new_values);
    $this->assertEquals(get_class($new), $class);
  }

  /**
   * Tests Drupal\google_dfp\PluginBase::setConfiguration.
   */
  public function doTestSetConfiguration() {
    $configuration = array('foo' => 'bar');
    $this->plugin->setConfiguration($configuration);
    $out = $this->plugin->getConfiguration();
    $this->assertEquals($configuration, $out);
    // Test getting a single value.
    $this->assertEquals('bar', $this->plugin->getConfiguration('foo'));
    // Set the configuration to an empty value.
    $this->plugin->setConfiguration(array());
    // Default configuration should now be returned.
    $this->assertEquals($this->plugin->getConfiguration('weight'), 0);
    // Test for non-existent key.
    $this->assertFalse($this->plugin->getConfiguration('foo'));
  }

  /**
   * Tests Drupal\google_dfp\PluginBase::getId.
   *
   * @param string $id
   *   Tier id.
   */
  public function doTestGetId($id) {
    $this->assertEquals($this->plugin->getId(), $id);
  }

  /**
   * Tests Drupal\google_dfp\PluginBase::getTitle.
   *
   * @param string $title
   *   Tier title.
   */
  public function doTestGetTitle($title) {
    $this->assertEquals($this->plugin->getTitle(), $title);
  }

  /**
   * Tests Drupal\google_dfp\PluginBase::settingsFormSubmit.
   *
   * @param array $form
   *   Form array.
   * @param array $form_state
   *   Form state array.
   */
  public function doTestSettingsFormSubmit($form, $form_state) {
    $this->plugin->settingsFormSubmit($form, $form_state);
  }

}

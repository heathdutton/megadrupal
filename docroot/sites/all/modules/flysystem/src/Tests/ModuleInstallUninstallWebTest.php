<?php

/**
 * @file
 * Contains \Drupal\flysystem\Tests\ModuleInstallUninstallWebTest.
 */

namespace Drupal\flysystem\Tests;

/**
 * Tests module installation and uninstallation.
 */
class ModuleInstallUninstallWebTest extends \DrupalWebTestCase {

  public static function getInfo() {
    return array(
      'name' => 'Install/uninstall',
      'description' => 'Tests that the module installs and uninstalls.',
      'group' => 'Flysystem',
    );
  }

  public function setUp() {
    parent::setUp(array('flysystem'));
  }

  /**
   * Tests installation and uninstallation.
   */
  protected function testInstallationAndUninstallation() {
    $this->assertTrue(module_exists('flysystem'));

    module_disable(array('flysystem'));
    $this->assertFalse(module_exists('flysystem'));

    $this->assertTrue(drupal_uninstall_modules(array('flysystem')));
  }

}

<?php

namespace Drupal\go1_base\Tests\Unit;

use Drupal\go1_base\Helper\Test\UnitTestCase;

/**
 * Test service container
 *
 *  drush test-run --dirty 'Drupal\go1_base\Tests\Unit\ContainerTest'
 */
class ContainerTest extends UnitTestCase {
  public function getInfo() {
    return array('name' => 'GO1 Unit: Container') + parent::getInfo();
  }

  /**
   * Test for go1_container().
   */
  public function testServiceContainer() {
    // Simple service
    $service_1 = go1_container('helper.content_render');
    $this->assertEqual('Drupal\go1_base\Helper\ContentRender', get_class($service_1));

    // Service with factory
    $service_2 = go1_container('twig.core');
    $this->assertEqual('Twig_Environment', get_class($service_2));

    // Service depends on others
    $service_3 = go1_container('twig_string');
    $this->assertEqual('Twig_Environment', get_class($service_3));

    $service_4 = go1_container('twig');
    $this->assertEqual('Twig_Environment', get_class($service_4));
  }

  public function testIncludingFile() {
    $service = go1_container('go1test_base.include_me');
    $this->assertEqual('GO1Test_Base_Include_Me', get_class($service));
  }

  public function testDynamicArguments() {
    $service = go1_container('go1test_base.dynamic_arguments');
    $this->assertEqual('Drupal\go1test_base\DynamicArguments', get_class($service));
    $this->assertEqual('go1test_base', $service->getDynParam());
    $this->assertEqual('Drupal\go1test_base\Service_1', get_class($service->getDynService()));
  }

  public function testAutoloadPSR0() {
    $service = go1_container('go1test_base.psr0_me');
    $this->assertEqual('GO1\go1test_load\PSR0Me', get_class($service));
  }

  public function testAutoloadPSR4() {
    $service = go1_container('go1test_base.psr4_me');
    $this->assertEqual('GO1Test\go1test_base\PSR4Me', get_class($service));
  }

  public function testTaggedServices() {
    // With weight
    $expected = array('cache.warmer.view', 'cache.warmer.entity', 'cache.warmer.simple');
    $actual = go1_container('container')->find('cache.warmer');
    $this->assertEqual(implode(', ', $expected), implode(', ', $actual));

    // Return services instead of services name
    foreach (go1_container('container')->find('cache.warmer', $return = 'service') as $name => $service) {
      $expected = get_class(go1_container($name));
      $actual = get_class($service);
      $this->assertEqual($expected, $actual);
    }
  }
}

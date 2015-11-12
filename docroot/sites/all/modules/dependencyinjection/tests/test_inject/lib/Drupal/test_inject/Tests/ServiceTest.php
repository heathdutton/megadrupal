<?php

namespace Drupal\test_inject\Tests;

use Inject\Test\DummyManager;
use Drupal\test_inject\Dummy;

class ServiceTest extends \DrupalWebTestCase {

  public static function getInfo() {
    return array(
      'name' => 'Inject',
      'description' => 'Test Inject Service.',
      'group' => 'Inject',
    );
  }

  function setUp() {
    parent::setUp('test_inject');
  }

  function testServicesInjection() {
    $container = drupal_container();
    $manager = $container->get('dummy_manager');
    $this->assertTrue($manager instanceof DummyManager, 'Dummy Manager correctly instantiated.');
    $this->assertTrue($manager->getDummy() instanceof Dummy, 'Dependency correctly resolved.');
    $this->assertTrue($manager->getDummy()->getParameter() == 'dummy', 'Parameter correctly passed.');

    $this->assertTrue($container->hasParameter('dummy.build'), 'Parameter has been overriden.');
  }

}

<?php

namespace Drupal\go1_base\Tests\Unit;

use Drupal\go1_base\Helper\Test\UnitTestCase;

/**
 * drush test-run --dirty 'Drupal\go1_base\Tests\Unit\ControllerResolverTest'
 */
class ControllerResolverTest extends UnitTestCase {
  private $resolver;

  public function getInfo() {
    return array('name' => 'GO1 Unit: Controller Resolver') + parent::getInfo();
  }

  public function setUp() {
    $this->resolver = go1_container('helper.controller.resolver');
    parent::setUp();
  }

  public function testObjectMethodPair() {
    $obj = new \Go1_Base_Test_Class();
    $definition = array($obj, 'foo');
    $expected = array($obj, 'foo');
    $actual = $this->resolver->get($definition);
    $this->assertEqual($expected, $actual);
  }

  public function testStaticMethod() {
    $definition = 'Go1_Base_Test_Class::foo';
    $expected = array('Go1_Base_Test_Class', 'foo');
    $actual = $this->resolver->get($definition);
    $this->assertEqual($expected, $actual);
  }

  public function testTwigTemplate() {
    $definition = "{{ 'Hello ' ~ 'GO1' }}";
    $expected = 'Hello GO1';
    $actual = $this->resolver->get($definition);
    $actual = trim(call_user_func($actual));
    $this->assertEqual($expected, $actual);
  }

  public function testFunctionString() {
    $definition = 'time';
    $expected = 'time';
    $actual = $this->resolver->get($definition);
    $this->assertEqual($expected, $actual);
  }

  public function testObjectInvoke() {
    $obj = new \Go1_Base_Test_Class();
    $definition = $obj;
    $expected = $obj;
    $actual = $this->resolver->get($definition);
    $this->assertEqual($expected, $actual);
  }

  public function testClassStringInvoke() {
    $definition = 'Go1_Base_Test_Class';
    $expected = 'Go1_Base_Test_Class';
    $actual = $this->resolver->get($definition);
    $this->assertEqual($expected, get_class($actual));
  }
}

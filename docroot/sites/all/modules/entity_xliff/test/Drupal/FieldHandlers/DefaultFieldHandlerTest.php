<?php

namespace EntityXliff\Drupal\Tests\FieldHandlers;


use EntityXliff\Drupal\FieldHandlers\DefaultFieldHandler;

class DefaultFieldHandlerTest extends \PHPUnit_Framework_TestCase {

  /**
   * Tests that the default field handler merely returns $wrapper->value().
   *
   * @test
   */
  public function getValue() {
    $expectedResponse = 'value';

    $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('value'));
    $observerWrapper->expects($this->once())
      ->method('value')
      ->willReturn($expectedResponse);

    $handler = new DefaultFieldHandler();
    $this->assertEquals($expectedResponse, $handler->getValue($observerWrapper));
  }

  /**
   * Tests that the default field handler merely calls $wrapper->set() with the
   * given value.
   *
   * @test
   */
  public function setValue() {
    $mockValue = 'value';

    $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('set'));
    $observerWrapper->expects($this->once())
      ->method('set')
      ->with($this->equalTo($mockValue));

    $handler = new DefaultFieldHandler();
    $handler->setValue($observerWrapper, $mockValue);
  }

}

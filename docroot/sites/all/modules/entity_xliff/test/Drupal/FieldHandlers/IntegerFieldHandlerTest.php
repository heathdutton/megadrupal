<?php

namespace EntityXliff\Drupal\Tests\FieldHandlers;


use EntityXliff\Drupal\FieldHandlers\IntegerFieldHandler;

class IntegerFieldHandlerTest extends \PHPUnit_Framework_TestCase {

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

    $handler = new IntegerFieldHandler();
    $this->assertEquals($expectedResponse, $handler->getValue($observerWrapper));
  }

  /**
   * Tests that the integer field handler calls $wrapper->set() with the intval
   * of the given value.
   *
   * @test
   */
  public function setValue() {
    $mockValue = '123';
    $expectedValue = intval($mockValue);

    $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('set'));
    $observerWrapper->expects($this->once())
      ->method('set')
      ->with($this->identicalTo($expectedValue));

    $handler = new IntegerFieldHandler();
    $handler->setValue($observerWrapper, $mockValue);
  }

}

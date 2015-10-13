<?php

namespace EntityXliff\Drupal\Tests\FieldHandlers;


use EntityXliff\Drupal\FieldHandlers\FormattedFieldHandler;

class FormattedFieldHandlerTest extends \PHPUnit_Framework_TestCase {

  /**
   * Tests that the formatted text field handler returns the "value" key on the
   * array returned by $wrapper->value();
   *
   * @test
   */
  public function getValue() {
    $expectedResponse = 'value';

    $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('value'));
    $observerWrapper->expects($this->once())
      ->method('value')
      ->willReturn(array('value' => $expectedResponse));

    $handler = new FormattedFieldHandler();
    $this->assertEquals($expectedResponse, $handler->getValue($observerWrapper));
  }

  /**
   * Tests that the formatted text field handler calls $wrapper->set() with the
   * given value on the "value" key.
   *
   * @test
   */
  public function setValue() {
    $mockValue = 'value';

    $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('set', 'value'));
    $observerWrapper->expects($this->once())
      ->method('value')
      ->willReturn(array('value' => 'not value'));
    $observerWrapper->expects($this->once())
      ->method('set')
      ->with($this->equalTo(array('value' => $mockValue)));

    $handler = new FormattedFieldHandler();
    $handler->setValue($observerWrapper, $mockValue);
  }

}

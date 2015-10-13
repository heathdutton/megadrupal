<?php

namespace EntityXliff\Drupal\Tests\FieldHandlers;


use EntityXliff\Drupal\FieldHandlers\LinkFieldHandler;

class LinkFieldHandlerTest extends \PHPUnit_Framework_TestCase {

  /**
   * Tests that the link field handler returns the "title" key on the array
   * returned by $wrapper->value();
   *
   * @test
   */
  public function getValue() {
    $expectedResponse = 'value';

    $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('value'));
    $observerWrapper->expects($this->once())
      ->method('value')
      ->willReturn(array('title' => $expectedResponse));

    $handler = new LinkFieldHandler();
    $this->assertEquals($expectedResponse, $handler->getValue($observerWrapper));
  }

  /**
   * Tests that the link field handler calls $wrapper->set() with the given
   * value on the "title" key.
   *
   * @test
   */
  public function setValue() {
    $mockValue = 'value';

    $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('set', 'value'));
    $observerWrapper->expects($this->once())
      ->method('value')
      ->willReturn(array('title' => 'not title'));
    $observerWrapper->expects($this->once())
      ->method('set')
      ->with($this->equalTo(array('title' => $mockValue)));

    $handler = new LinkFieldHandler();
    $handler->setValue($observerWrapper, $mockValue);
  }

}

<?php

namespace EntityXliff\Drupal\Tests\FieldHandlers;


use EntityXliff\Drupal\FieldHandlers\ImageFieldHandler;

class ImageFieldHandlerTest extends \PHPUnit_Framework_TestCase {

  /**
   * Tests that the image field handler returns an empty array when the value
   * pulled from the wrapper contains no applicable properties.
   *
   * @test
   */
  public function getValueNoApplicableProperties() {
    $expectedResponse = array();

    $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('value'));
    $observerWrapper->expects($this->once())
      ->method('value')
      ->willReturn(array('notAlt' => TRUE, 'notTitle' => TRUE));

    $handler = new ImageFieldHandler();
    $this->assertSame($expectedResponse, $handler->getValue($observerWrapper));
  }

  /**
   * Tests that the image field handler returns only the applicable properties
   * pulled from the wrapper.
   *
   * @test
   */
  public function getValueAllApplicableProperties() {
    $willedResponse = array(
      'alt' => 'Alt value',
      'title' => 'Title value',
      'irrelevant' => TRUE,
    );
    $expectedResponse = array(
      'alt' => array('#label' => 'Alternate text', '#text' => 'Alt value'),
      'title' => array('#label' => 'Title text', '#text' => 'Title value'),
    );

    $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('value'));
    $observerWrapper->expects($this->once())
      ->method('value')
      ->willReturn($willedResponse);

    $handler = new ImageFieldHandler();
    $this->assertSame($expectedResponse, $handler->getValue($observerWrapper));
  }

  /**
   * Tests that the image field handler calls $wrapper->set() with the same
   * value that $wrapper->value() returned if no applicable properties are
   * passed in.
   *
   * @test
   */
  public function setValueNoApplicableProperties() {
    $expectedResponse = 'value';
    $inapplicableProps = array(
      'notAlt' => TRUE,
      'notTitle' => TRUE,
    );

    $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('set', 'value'));
    $observerWrapper->expects($this->once())
      ->method('value')
      ->willReturn($expectedResponse);
    $observerWrapper->expects($this->once())
      ->method('set')
      ->with($this->equalTo($expectedResponse));

    $handler = new ImageFieldHandler();
    $handler->setValue($observerWrapper, $inapplicableProps);
  }

  /**
   * Tests that the image field handler calls $wrapper->set() with the expected
   * values for applicable properties only.
   *
   * @test
   */
  public function setValueAllApplicableProperties() {
    $willedResponse = array();
    $expectedResponse = array(
      'alt' => TRUE,
      'title' => TRUE,
    );

    $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('set', 'value'));
    $observerWrapper->expects($this->once())
      ->method('value')
      ->willReturn($willedResponse);
    $observerWrapper->expects($this->once())
      ->method('set')
      ->with($this->equalTo($expectedResponse));

    $handler = new ImageFieldHandler();
    $handler->setValue($observerWrapper, $expectedResponse);
  }

}

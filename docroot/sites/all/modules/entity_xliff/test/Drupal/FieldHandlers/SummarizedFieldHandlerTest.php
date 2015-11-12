<?php

namespace EntityXliff\Drupal\Tests\FieldHandlers;


use EntityXliff\Drupal\FieldHandlers\SummarizedFieldHandler;

class SummarizedFieldHandlerTest extends \PHPUnit_Framework_TestCase {

  /**
   * Tests that the summarized text field handler returns an empty array when
   * the value pulled from the wrapper contains no applicable properties.
   *
   * @test
   */
  public function getValueNoApplicableProperties() {
    $expectedResponse = array();

    $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('value', 'info'));
    $observerWrapper->expects($this->once())
      ->method('info')
      ->willReturn(array());
    $observerWrapper->expects($this->once())
      ->method('value')
      ->willReturn(array('notValue' => TRUE, 'notSummary' => TRUE));

    $handler = new SummarizedFieldHandler();
    $this->assertSame($expectedResponse, $handler->getValue($observerWrapper));
  }

  /**
   * Tests that the summarized text field handler returns only the applicable
   * properties pulled from the wrapper. Also ensures the field label is pulled
   * from the wrapper info.
   *
   * @test
   */
  public function getValueAllApplicableProperties() {
    $willedInfo = array('label' => 'Field label');
    $willedResponse = array(
      'summary' => 'Summary value',
      'value' => 'Value',
      'irrelevant' => TRUE,
    );
    $expectedResponse = array(
      'value' => array('#label' => $willedInfo['label'] . ' (value)', '#text' => 'Value'),
      'summary' => array('#label' => $willedInfo['label'] . ' (summary)', '#text' => 'Summary value'),
    );

    $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('value', 'info'));
    $observerWrapper->expects($this->once())
      ->method('info')
      ->willReturn($willedInfo);
    $observerWrapper->expects($this->once())
      ->method('value')
      ->willReturn($willedResponse);

    $handler = new SummarizedFieldHandler();
    $this->assertSame($expectedResponse, $handler->getValue($observerWrapper));
  }

  /**
   * Tests that the summarized text field handler calls $wrapper->set() with the
   * same value that $wrapper->value() returned if no applicable properties are
   * passed in.
   *
   * @test
   */
  public function setValueNoApplicableProperties() {
    $expectedResponse = 'value';
    $inapplicableProps = array(
      'notSummary' => TRUE,
      'notValue' => TRUE,
    );

    $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('set', 'value'));
    $observerWrapper->expects($this->once())
      ->method('value')
      ->willReturn($expectedResponse);
    $observerWrapper->expects($this->once())
      ->method('set')
      ->with($this->equalTo($expectedResponse));

    $handler = new SummarizedFieldHandler();
    $handler->setValue($observerWrapper, $inapplicableProps);
  }

  /**
   * Tests that the summarized text field handler calls $wrapper->set() with the
   * expected values for applicable properties only.
   *
   * @test
   */
  public function setValueAllApplicableProperties() {
    $willedResponse = array();
    $expectedResponse = array(
      'value' => TRUE,
      'summary' => TRUE,
    );

    $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('set', 'value'));
    $observerWrapper->expects($this->once())
      ->method('value')
      ->willReturn($willedResponse);
    $observerWrapper->expects($this->once())
      ->method('set')
      ->with($this->equalTo($expectedResponse));

    $handler = new SummarizedFieldHandler();
    $handler->setValue($observerWrapper, $expectedResponse);
  }

}

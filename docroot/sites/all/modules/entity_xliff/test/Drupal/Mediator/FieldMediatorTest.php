<?php

namespace EntityXliff\Drupal\Tests\Mediator;


use EntityXliff\Drupal\Mediator\FieldMediator;

class FieldMediatorTest extends \PHPUnit_Framework_TestCase {

  /**
   * Tests that the constructor will instantiate a DrupalHandler if none is
   * provided.
   *
   * @test
   */
  public function constructorWithNoArgs() {
    $fieldMediator = new MockFieldMediatorForConstructorOnly();
    $this->assertInstanceOf('EntityXliff\Drupal\Utils\DrupalHandler', $fieldMediator->getHandler());
  }

  /**
   * Tests that FieldMediator::buildMap() builds the internal class map from the
   * DrupalHandler as expected (both via the constructor and directly).
   *
   * @test
   */
  public function buildMap() {
    $entityKey = 'foo';
    $expectedClass = 'bar';
    $classMap = array($entityKey => array('class' => $expectedClass));
    $expectedResult = array($entityKey => $expectedClass);

    $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler');
    $observerDrupal->expects($this->any())
      ->method('entityXliffGetFieldHandlers')
      ->willReturn($classMap);

    // Ensure the constructor calls buildMap() and stores the expected results.
    $fieldMediator = new MockFieldMediator($observerDrupal);
    $this->assertSame($expectedResult, $fieldMediator->getClassMap());

    // Also ensure that manually calling FieldMediator::buildMap() works.
    $fieldMediator->setClassMap(array());
    $fieldMediator->buildMap();
    $this->assertSame($expectedResult, $fieldMediator->getClassMap());
  }

  /**
   * Tests that FieldMediator::getInstance() returns NULL when the given wrapper
   * is not known to be translatable.
   *
   * @test
   */
  public function getInstanceNotTranslatable() {
    $mockWrapper = $this->getMock('\EntityMetadataWrapper');

    $mockFieldMediator = $this->getMockBuilder('EntityXliff\Drupal\Mediator\FieldMediator')
      ->disableOriginalConstructor()
      ->setMethods(array('getClass'))
      ->getMock();
    $mockFieldMediator->expects($this->once())
      ->method('getClass')
      ->with($this->equalTo($mockWrapper))
      ->willReturn(FALSE);

    $this->assertSame(NULL, $mockFieldMediator->getInstance($mockWrapper));
  }

  /**
   * Tests that FieldMediator::getInstance() returns an instance of the expected
   * class from the internal classMap when the given wrapper IS known to be
   * translatable.
   *
   * @test
   */
  public function getInstanceIsTranslatable() {
    $entityKey = 'node';
    $expectedClass = 'stdClass';
    $classMap = array($entityKey => $expectedClass);

    $mockWrapper = $this->getMock('\EntityMetadataWrapper');

    $mockFieldMediator = $this->getMockBuilder('EntityXliff\Drupal\Tests\Mediator\MockFieldMediator')
      ->disableOriginalConstructor()
      ->setMethods(array('getClass'))
      ->getMock();
    $mockFieldMediator->expects($this->once())
      ->method('getClass')
      ->with($this->equalTo($mockWrapper))
      ->willReturn($expectedClass);

    $mockFieldMediator->setClassMap($classMap);
    $this->assertEquals($expectedClass, get_class($mockFieldMediator->getInstance($mockWrapper)));
  }

  /**
   * Tests that FieldMediator::canBeTranslated returns the correct response
   * according to the provided wrapper's info and the known classMap.
   *
   * @test
   */
  public function canBeTranslated() {
    $entityKey = 'node';
    $classMap = array($entityKey => 'anything');
    $willedInfo = array('type' => $entityKey);

    $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('info'));
    $observerWrapper->expects($this->once())
      ->method('info')
      ->willReturn($willedInfo);

    $fieldMediator = $this->getFieldMediatorWithClassMap($classMap);
    $this->assertTrue($fieldMediator->canBeTranslated($observerWrapper));
  }

  /**
   * Tests that FieldMediator::getClass() returns FALSE when the given wrapper
   * is not known to be translatable.
   *
   * @test
   */
  public function getClassNotTranslatable() {
    $mockWrapper = $this->getMock('\EntityMetadataWrapper');

    $mockFieldMediator = $this->getMockBuilder('EntityXliff\Drupal\Mediator\FieldMediator')
      ->disableOriginalConstructor()
      ->setMethods(array('canBeTranslated'))
      ->getMock();
    $mockFieldMediator->expects($this->once())
      ->method('canBeTranslated')
      ->with($this->equalTo($mockWrapper))
      ->willReturn(FALSE);

    $this->assertFalse($mockFieldMediator->getClass($mockWrapper));
  }

  /**
   * Tests that FieldMediator::getClass() returns the expected class name from
   * the internal classMap when the given wrapper IS known to be translatable.
   *
   * @test
   */
  public function getClassIsTranslatable() {
    $entityKey = 'node';
    $expectedClass = 'ExpectedClassName';
    $classMap = array($entityKey => $expectedClass);

    $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('info'));
    $observerWrapper->expects($this->once())
      ->method('info')
      ->willReturn(array('type' => $entityKey));

    $mockFieldMediator = $this->getMockBuilder('EntityXliff\Drupal\Tests\Mediator\MockFieldMediator')
      ->disableOriginalConstructor()
      ->setMethods(array('canBeTranslated'))
      ->getMock();
    $mockFieldMediator->expects($this->once())
      ->method('canBeTranslated')
      ->with($this->equalTo($observerWrapper))
      ->willReturn(TRUE);

    $mockFieldMediator->setClassMap($classMap);
    $this->assertEquals($expectedClass, $mockFieldMediator->getClass($observerWrapper));
  }

  /**
   * Returns a field mediator that will use the given classMap internally.
   *
   * @param array $classMap
   *   The desired internal classMap.
   * @return FieldMediator
   */
  public function getFieldMediatorWithClassMap(array $classMap) {
    foreach ($classMap as $entity => $class) {
      $classMap[$entity] = array('class' => $class);
    }

    $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler');
    $observerDrupal->expects($this->any())
      ->method('entityXliffGetFieldHandlers')
      ->willReturn($classMap);

    return new FieldMediator($observerDrupal);
  }
}

/**
 * Adds helper methods for simpler testing of the field mediator.
 */
class MockFieldMediator extends FieldMediator {

  /**
   * Manually sets the internal classMap property.
   *
   * @param array $classMap
   *   An associative array mapping entity types to class names.
   */
  public function setClassMap(array $classMap) {
    $this->classMap = $classMap;
  }

  /**
   * Returns the internal classMap property.
   * @return array
   */
  public function getClassMap() {
    return $this->classMap;
  }
}

/**
 * Overrides the buildMap method so the constructor does not depend on the
 * handler.
 */
class MockFieldMediatorForConstructorOnly extends FieldMediator {

  /**
   * Makes the buildMap method a no-op for testing the parent constructor.
   */
  public function buildMap() {}

  /**
   * Returns the internal DrupalHandler property.
   * @return \EntityXliff\Drupal\Utils\DrupalHandler
   */
  public function getHandler() {
    return $this->drupal;
  }

}

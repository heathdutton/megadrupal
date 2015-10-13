<?php

namespace EntityXliff\Drupal\Tests\Factories;

use EntityXliff\Drupal\Factories\EntityTranslatableFactory;


class EntityTranslatableFactoryTest extends \PHPUnit_Framework_TestCase {

  /**
   * Tests that the EntityTranslatableFactory::getInstance method returns an
   * instance of the translatable factory and builds an internal class map with
   * all expected values.
   *
   * @test
   */
  public function getInstanceAndClassMap() {
    $expectedType = 'node';
    $willedEntityInfo = array(
      $expectedType => array(
        'entity xliff translatable classes' => array(
          EntityTranslatableFactory::ENTITYFIELD => 'EntityFieldClassName',
          EntityTranslatableFactory::CONTENT => 'ContentClassName',
          EntityTranslatableFactory::UNKNOWN => 'CustomClassName',
        ),
      ),
    );
    $expectedClassMap = array(
      $expectedType => $willedEntityInfo[$expectedType]['entity xliff translatable classes'],
    );

    $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler', array('entityGetInfo'));
    $observerDrupal->expects($this->any())
      ->method('entityGetInfo')
      ->willReturn($willedEntityInfo);

    $factory = EntityTranslatableFactoryInstance::getInstance($observerDrupal);
    $this->assertSame($expectedClassMap, $factory->getClassMap());
  }

  /**
   * Tests that EntityTranslatableFactory::getTranslatable pulls translatables
   * from its static cache with a given key.
   *
   * @test
   */
  public function getTranslatableFromStaticCache() {
    $expectedType = 'node';
    $expectedIdentifier = 123;
    $transKey = $expectedType . ':' . $expectedIdentifier;
    $expectedTranslatables = array($transKey => 'foobar');

    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('type', 'getIdentifier'));
    $observerWrapper->expects($this->once())
      ->method('type')
      ->willReturn($expectedType);
    $observerWrapper->expects($this->once())
      ->method('getIdentifier')
      ->willReturn($expectedIdentifier);

    $mockDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler');

    $factory = EntityTranslatableFactoryInstance::getInstance($mockDrupal);
    $factory->setTranslatables($expectedTranslatables);
    $translatable = $factory->getTranslatable($observerWrapper);
    $this->assertSame($expectedTranslatables[$transKey], $translatable);
  }

  /**
   * Tests that EntityTranslatableFactory::getTranslatable will instantiate a
   * new Translatable as specified in the factory's internal classMap.
   * 
   * @test
   */
  public function getTranslatable() {
    $expectedType = 'node';
    $expectedIdentifier = 123;
    $expectedParadigm = EntityTranslatableFactory::UNKNOWN;
    $classMap = array(
      $expectedType => array(
        $expectedParadigm => 'EntityXliff\Drupal\Tests\Factories\NotReallyTranslatable',
      ),
    );

    $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler', array('entityGetInfo'));

    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('type', 'getIdentifier'));
    $observerWrapper->expects($this->once())
      ->method('type')
      ->willReturn($expectedType);
    $observerWrapper->expects($this->once())
      ->method('getIdentifier')
      ->willReturn($expectedIdentifier);

    $factory = $this->getMockBuilder('EntityXliff\Drupal\Tests\Factories\EntityTranslatableFactoryInstance')
      ->disableOriginalConstructor()
      ->setMethods(array('getTranslationParadigm'))
      ->getMock();
    $factory->expects($this->once())
      ->method('getTranslationParadigm')
      ->willReturn($expectedParadigm);

    $factory->setDrupalHandler($observerDrupal);
    $factory->setClassMap($classMap);
    $translatable = $factory->getTranslatable($observerWrapper);
    $this->assertSame($observerWrapper, $translatable->entity);
  }

  /**
   * Tests that EntityTranslatableFactory::getTranslationParadigm returns the
   * expected paradigm given a certain set of circumstances.
   *
   * @test
   */
  public function getTranslationParadigmEntityField() {
    $expectedType = 'node';
    $expectedBundle = 'page';

    $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler', array(
      'entityGetInfo',
      'moduleExists',
      'entityTranslationEnabled',
    ));
    $observerDrupal->expects($this->any())
      ->method('entityGetInfo')
      ->willReturn(array());

    $observerDrupal->expects($this->exactly(2))
      ->method('moduleExists')
      ->withConsecutive(
        array($this->equalTo('entity_translation')),
        array($this->equalTo('translation'))
      )
      ->will($this->onConsecutiveCalls(TRUE, FALSE));
    $observerDrupal->expects($this->once())
      ->method('entityTranslationEnabled')
      ->with($this->equalTo($expectedType), $this->equalTo($expectedBundle))
      ->willReturn(TRUE);

    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('type', 'getBundle'));
    $observerWrapper->expects($this->once())
      ->method('type')
      ->willReturn($expectedType);
    $observerWrapper->expects($this->once())
      ->method('getBundle')
      ->willReturn($expectedBundle);

    $factory = EntityTranslatableFactoryInstance::getInstance($observerDrupal);
    $factory->setDrupalHandler($observerDrupal);
    $paradigm = $factory->getTranslationParadigm($observerWrapper);
    $this->assertEquals(EntityTranslatableFactory::ENTITYFIELD, $paradigm);
  }

  /**
   * Tests that EntityTranslatableFactory::getTranslationParadigm returns the
   * expected paradigm given a certain set of circumstances.
   *
   * @test
   */
  public function getTranslationParadigmContent() {
    $expectedType = 'node';
    $expectedBundle = 'page';

    $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler', array(
      'entityGetInfo',
      'moduleExists',
      'translationSupportedType',
    ));
    $observerDrupal->expects($this->any())
      ->method('entityGetInfo')
      ->willReturn(array());

    $observerDrupal->expects($this->exactly(2))
      ->method('moduleExists')
      ->withConsecutive(
        array($this->equalTo('entity_translation')),
        array($this->equalTo('translation'))
      )
      ->will($this->onConsecutiveCalls(FALSE, TRUE));
    $observerDrupal->expects($this->once())
      ->method('translationSupportedType')
      ->with($this->equalTo($expectedBundle))
      ->willReturn(TRUE);

    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('type', 'getBundle'));
    $observerWrapper->expects($this->once())
      ->method('type')
      ->willReturn($expectedType);
    $observerWrapper->expects($this->once())
      ->method('getBundle')
      ->willReturn($expectedBundle);

    $factory = EntityTranslatableFactoryInstance::getInstance($observerDrupal);
    $factory->setDrupalHandler($observerDrupal);
    $paradigm = $factory->getTranslationParadigm($observerWrapper);
    $this->assertEquals(EntityTranslatableFactory::CONTENT, $paradigm);
  }


  /**
   * Tests that EntityTranslatableFactory::getTranslationParadigm returns the
   * expected paradigm given a certain set of circumstances.
   *
   * @test
   */
  public function getTranslationParadigmUnknown() {
    $expectedType = 'node';
    $expectedBundle = 'page';

    $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler', array(
      'entityGetInfo',
      'moduleExists',
      'translationSupportedType',
      'entityTranslationEnabled',
    ));
    $observerDrupal->expects($this->any())
      ->method('entityGetInfo')
      ->willReturn(array());

    $observerDrupal->expects($this->exactly(2))
      ->method('moduleExists')
      ->withConsecutive(
        array($this->equalTo('entity_translation')),
        array($this->equalTo('translation'))
      )
      ->will($this->onConsecutiveCalls(TRUE, TRUE));
    $observerDrupal->expects($this->once())
      ->method('translationSupportedType')
      ->with($this->equalTo($expectedBundle))
      ->willReturn(FALSE);
    $observerDrupal->expects($this->once())
      ->method('entityTranslationEnabled')
      ->with($this->equalTo($expectedType), $this->equalTo($expectedBundle))
      ->willReturn(FALSE);

    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('type', 'getBundle'));
    $observerWrapper->expects($this->once())
      ->method('type')
      ->willReturn($expectedType);
    $observerWrapper->expects($this->once())
      ->method('getBundle')
      ->willReturn($expectedBundle);

    $factory = EntityTranslatableFactoryInstance::getInstance($observerDrupal);
    $factory->setDrupalHandler($observerDrupal);
    $paradigm = $factory->getTranslationParadigm($observerWrapper);
    $this->assertEquals(EntityTranslatableFactory::UNKNOWN, $paradigm);
  }

}

/**
 * Class EntityTranslatableFactoryInstance
 * @package EntityXliff\Drupal\Tests\Factories
 */
class EntityTranslatableFactoryInstance extends EntityTranslatableFactory {

  /**
   * Helper method to set the internal translatables property in order to make
   * testing simpler.
   *
   * @param array $translatables
   */
  public function setTranslatables(array $translatables) {
    $this->translatables = $translatables;
  }

  /**
   * Helper method to set the internal classMap property in order to simplify
   * testing.
   *
   * @param array $classMap
   */
  public function setClassMap(array $classMap) {
    $this->classMap = $classMap;
  }

  /**
   * Helper method to set the internal Drupal handler in order to make testing
   * simpler.
   *
   * @param \EntityXliff\Drupal\Utils\DrupalHandler $handler
   */
  public function setDrupalHandler($handler) {
    $this->drupal = $handler;
  }

}

/**
 * A stub class that will be instantiated as a "Translatable" when the
 * EntityTranslatableFactory::getTranslatable method is called above.
 */
class NotReallyTranslatable {

  /**
   * @var \EntityDrupalWrapper
   */
  public $entity;

  /**
   * @param \EntityDrupalWrapper $wrapper
   */
  public function __construct(\EntityDrupalWrapper $wrapper) {
    $this->entity = $wrapper;
  }

}

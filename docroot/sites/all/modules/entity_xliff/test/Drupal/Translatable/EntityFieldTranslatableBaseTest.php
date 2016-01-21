<?php

namespace EntityXliff\Drupal\Tests\Translatable;


use EntityXliff\Drupal\Translatable\EntityFieldTranslatableBase;
use EntityXliff\Drupal\Utils\DrupalHandler;

class EntityFieldTranslatableBaseTest extends \PHPUnit_Framework_TestCase {

  /**
   * Tests that EntityFieldTranslatableBase::getSourceLanguage() returns the
   * language code of the wrapped entity as expected.
   *
   * @test
   */
  public function getSourceLanguage() {
    $mockEntityType = 'node';
    $mockEntityRaw = (object) array('nid' => 123);
    $expectedLanguage = 'fr';

    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('raw', 'type'));
    $observerWrapper->expects($this->once())
      ->method('raw')
      ->willReturn($mockEntityRaw);
    $observerWrapper->expects($this->once())
      ->method('type')
      ->willReturn($mockEntityType);

    $observerHandler = $this->getMock('\EntityTranslationHandlerInterface', array('getLanguage'));
    $observerHandler->expects($this->once())
      ->method('getLanguage')
      ->willReturn($expectedLanguage);

    $observerDrupal = $this->getMockHandler();
    $observerDrupal->expects($this->once())
      ->method('entityTranslationGetHandler')
      ->with($this->equalTo($mockEntityType), $this->equalTo($mockEntityRaw))
      ->willReturn($observerHandler);

    $translatable = new EntityFieldTranslatableBaseInstance($observerWrapper, $observerDrupal, NULL, $this->getMockMediator());
    $this->assertSame($expectedLanguage, $translatable->getSourceLanguage());
  }

  /**
   * Tests that EntityFieldTranslatableBase::getTranslatableFields returns an
   * empty array when the wrapped entity is not translatable.
   *
   * @test
   */
  public function getTranslatableFieldsNotTranslatable() {
    $observerWrapper = $this->getMock('\EntityDrupalWrapper');
    $observerWrapper->expects($this->never())
      ->method('getPropertyInfo');

    $translatable = $this->getTranslatableOrNotInstance(FALSE, $observerWrapper);
    $this->assertSame(array(), $translatable->getTranslatableFields());
  }

  /**
   * Tests that EntityFieldTranslatableBase::getTranslatableFields returns as
   * expected when the wrapped entity is translatable.
   *
   * @test
   * @dataProvider getTranslatableFieldsData
   */
  public function getTranslatableFieldsIsTranslatable($entityInfo, $expectedResponse) {
    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('getPropertyInfo'));
    $observerWrapper->expects($this->once())
      ->method('getPropertyInfo')
      ->willReturn($entityInfo);

    $translatable = $this->getTranslatableOrNotInstance(TRUE, $observerWrapper);
    $this->assertSame($expectedResponse, $translatable->getTranslatableFields());
  }

  /**
   * Tests that EntityFieldTranslatableBase::isTranslatable performs the
   * expected check in the expected way.
   *
   * @test
   */
  public function isTranslatable() {
    $expectedType = 'node';
    $expectedBundle = 'page';
    $expectedResponse = TRUE;

    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('type', 'getBundle', 'entityGetInfo'));
    $observerWrapper->expects($this->once())
      ->method('type')
      ->willReturn($expectedType);
    $observerWrapper->expects($this->once())
      ->method('getBundle')
      ->willReturn($expectedBundle);
    $observerWrapper->expects($this->any())
      ->method('entityGetInfo')
      ->willReturn(array());

    $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler');
    $observerDrupal->expects($this->once())
      ->method('entityTranslationEnabled')
      ->with($this->equalTo($expectedType), $this->equalTo($expectedBundle))
      ->willReturn($expectedResponse);

    $mockFactory = $this->getMockFactory();
    $mockMediator = $this->getMockMediator();

    $translatable = new EntityFieldTranslatableBaseInstance($observerWrapper, $observerDrupal, $mockFactory, $mockMediator);
    $this->assertEquals($expectedResponse, $translatable->isTranslatable());
  }

  /**
   * Tests that EntityFieldTranslatableBase::getTargetEntity will pull a target
   * from its internal static cache if available.
   *
   * @test
   */
  public function getTargetEntityFromStaticCache() {
    $targetLanguage = 'de';
    $targetEntities = array(
      $targetLanguage => 'value',
    );
    $observerWrapper = $this->getMock('\EntityDrupalWrapper');

    $translatable = $this->getTranslatableOrNotInstance(TRUE, $observerWrapper);
    $translatable->setTargetEntities($targetEntities);
    $this->assertSame($targetEntities[$targetLanguage], $translatable->getTargetEntity($targetLanguage));
  }

  /**
   * Tests that EntityFieldTranslatableBase::getTargetEntity will call the
   * expected methods on the wrapped entity and set the value on static cache.
   *
   * @test
   */
  public function getTargetEntity() {
    $sourceLanguage = 'fr';
    $targetLanguage = 'de';
    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('language', 'raw', 'type'));
    $observerWrapper->language = $this->getMock('\EntityMetadataWrapper', array('set'));

    $observerWrapper->language->expects($this->once())
      ->method('set')
      ->with($this->equalTo($sourceLanguage));

    $observerWrapper->expects($this->once())
      ->method('language')
      ->with($this->equalTo($targetLanguage));

    $translatable = $this->getTranslatableOrNotInstance(TRUE, $observerWrapper, NULL, NULL, NULL, $sourceLanguage);
    $translatable->getTargetEntity($targetLanguage);

    // Ensure that the result was saved to static cache.
    $targetEntities = $translatable->getTargetEntities();
    $this->assertTrue(isset($targetEntities[$targetLanguage]));
  }

  /**
   * Tests that EntityFieldTranslatableBase::initializeTranslation is a no-op in
   * cases where the language value for the wrapped entity is already anything
   * but LANGUAGE_NONE.
   *
   * @test
   */
  public function initializeTranslationNoNeed() {
    $observerWrapper = $this->getMock('\EntityDrupalWrapper');
    $observerWrapper->language = $this->getMock('\EntityMetadataWrapper', array('value'));
    $observerWrapper->language->expects($this->once())
      ->method('value')
      ->willReturn('not' . DrupalHandler::LANGUAGE_NONE);

    $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler', array(
      'entityTranslationGetHandler',
      'entityGetInfo'
    ));
    $observerDrupal->expects($this->never())
      ->method('entityTranslationGetHandler');
    $observerDrupal->expects($this->any())
      ->method('entityGetInfo')
      ->willReturn(array());

    $translatable = $this->getTranslatableOrNotInstance(TRUE, $observerWrapper, $observerDrupal);
    $translatable->initializeTranslation();
  }

  /**
   * Tests that translation initialization for Entity Field translatables is
   * performed exactly as expected...
   *
   * @test
   */
  public function initializeTranslation() {
    $expectedSourceLanguage = 'fr';
    $expectedType = 'node';
    $expectedRawEntity = (object) array('nid' => 1234);
    $expectedHandlerEntity = (object) array('nid' => 1234, 'handled' => TRUE);
    $expectedFinalEntity = 'final, static entity';

    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('language', 'type'));
    $observerWrapper->language = $this->getMock('\EntityMetadataWrapper', array('value'));
    $observerWrapper->expects($this->once())
      ->method('language')
      ->with($this->equalTo($expectedSourceLanguage));
    $observerWrapper->expects($this->once())
      ->method('type')
      ->willReturn($expectedType);
    $observerWrapper->language->expects($this->once())
      ->method('value')
      ->willReturn(DrupalHandler::LANGUAGE_NONE);

    $observerTranslationHandler = $this->getMock('\EntityTranslationHandlerInterface', array(
      'setOriginalLanguage',
      'initOriginalTranslation',
      'saveTranslations',
      'getEntity',
    ));
    $observerTranslationHandler->expects($this->once())
      ->method('setOriginalLanguage')
      ->with($this->equalTo($expectedSourceLanguage));
    $observerTranslationHandler->expects($this->once())
      ->method('initOriginalTranslation');
    $observerTranslationHandler->expects($this->once())
      ->method('saveTranslations');
    $observerTranslationHandler->expects($this->once())
      ->method('getEntity')
      ->willReturn($expectedHandlerEntity);

    $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler', array(
      'entityTranslationGetHandler',
      'entityGetInfo',
      'entityMetadataWrapper',
    ));
    $observerDrupal->expects($this->once())
      ->method('entityTranslationGetHandler')
      ->with($this->equalTo($expectedType), $this->equalTo($expectedRawEntity))
      ->willReturn($observerTranslationHandler);
    $observerDrupal->expects($this->any())
      ->method('entityGetInfo')
      ->willReturn(array());
    $observerDrupal->expects($this->once())
      ->method('entityMetadataWrapper')
      ->with($this->equalTo($expectedType), $this->equalTo($expectedHandlerEntity))
      ->willReturn($expectedFinalEntity);

    $mockFactory = $this->getMockFactory();
    $mockMediator = $this->getMockMediator();

    $translatable = $this->getMockBuilder('EntityXliff\Drupal\Tests\Translatable\EntityFieldTranslatableBaseInstance')
      ->setConstructorArgs(array($observerWrapper, $observerDrupal, $mockFactory, $mockMediator))
      ->setMethods(array('getRawEntity', 'getSourceLanguage'))
      ->getMock();
    $translatable->expects($this->once())
      ->method('getSourceLanguage')
      ->willReturn($expectedSourceLanguage);
    $translatable->expects($this->once())
      ->method('getRawEntity')
      ->with($this->equalTo($observerWrapper))
      ->willReturn($expectedRawEntity);

    $translatable->initializeTranslation();
    $this->assertSame($expectedFinalEntity, $translatable->getEntity());
  }

  /**
   * Tests that EntityFieldTranslatableBase::saveWrappper performs everything as
   * expected...
   *
   * @test
   */
  public function saveWrapper() {
    $expectedSourceLanguage = 'de';
    $expectedType = 'node';
    $expectedRawEntity = (object) array('nid' => 1234);
    $targetLanguage = 'de';
    $expectedTranslation = array(
      'translate' => 0,
      'status' => TRUE,
      'language' => $targetLanguage,
      'source' => $expectedSourceLanguage,
    );

    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('type', 'save'));
    $observerWrapper->expects($this->once())
      ->method('type')
      ->willReturn($expectedType);
    $observerWrapper->expects($this->once())
      ->method('save');

    $observerTranslationHandler = $this->getMock('\EntityTranslationHandlerInterface', array(
      'setFormLanguage',
      'setTranslation',
      'saveTranslations',
    ));
    $observerTranslationHandler->expects($this->once())
      ->method('setFormLanguage')
      ->with($this->equalTo($targetLanguage));
    $observerTranslationHandler->expects($this->once())
      ->method('setTranslation')
      ->with($this->equalTo($expectedTranslation));
    $observerTranslationHandler->expects($this->once())
      ->method('saveTranslations');

    $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler', array(
      'entityTranslationGetHandler',
      'entityGetInfo',
    ));
    $observerDrupal->expects($this->once())
      ->method('entityTranslationGetHandler')
      ->with($this->equalTo($expectedType), $this->equalTo($expectedRawEntity))
      ->willReturn($observerTranslationHandler);
    $observerDrupal->expects($this->any())
      ->method('entityGetInfo')
      ->willReturn(array());

    $mockWrapper = $this->getMockWrapper();
    $mockFactory = $this->getMockFactory();
    $mockMediator = $this->getMockMediator();

    $translatable = $this->getMockBuilder('EntityXliff\Drupal\Tests\Translatable\EntityFieldTranslatableBaseInstance')
      ->setConstructorArgs(array($mockWrapper, $observerDrupal, $mockFactory, $mockMediator))
      ->setMethods(array('getRawEntity', 'getSourceLanguage'))
      ->getMock();
    $translatable->expects($this->once())
      ->method('getSourceLanguage')
      ->willReturn($expectedSourceLanguage);
    $translatable->expects($this->once())
      ->method('getRawEntity')
      ->with($this->equalTo($observerWrapper))
      ->willReturn($expectedRawEntity);

    $translatable->saveWrapper($observerWrapper, $targetLanguage);
  }

  /**
   * Returns data to test EntityFieldTranslatableBase::getTranslatableFields in
   * the case that the wrapped entity is translatable.
   *
   * @return array
   *   Each contained array consists of the data returned by the entityGetInfo
   *   method on EntityDrupalWrapper, followed by the expected response from the
   *   method.
   */
  public function getTranslatableFieldsData() {
    return array(
      // No "field" set.
      array(array('prop' => array('notField' => FALSE)), array()),
      // Field set, but not TRUE.
      array(array('prop' => array('field' => FALSE)), array()),
      // Field set and TRUE, but not translatable.
      array(array('prop' => array('field' => TRUE)), array()),
      // Field set and TRUE, translatable set, but not TRUE.
      array(array('prop' => array('field' => TRUE, 'translatable' => FALSE)), array()),
      // Field set and TRUE, translatable set and TRUE.
      array(array('prop' => array('field' => TRUE, 'translatable' => TRUE)), array('prop')),
    );
  }

  /**
   * Returns an instance of EntityFieldTranslatableBase with the isTranslatable
   * method stubbed out to return the provided $isTranslatable value.
   *
   * @param bool $isTranslatable
   *   Whether or not the translatable instance should return as translatable.
   *
   * @return \EntityXliff\Drupal\Interfaces\EntityTranslatableInterface
   */
  protected function getTranslatableOrNotInstance($isTranslatable, $wrapper, $handler = NULL, $factory = NULL, $mediator = NULL, $sourceLang = 'en') {
    $handler = $handler ?: $this->getMockHandler();
    $factory = $factory ?: $this->getMockFactory();
    $mediator = $mediator ?: $this->getMockMediator();

    $translatable = $this->getMockBuilder('EntityXliff\Drupal\Tests\Translatable\EntityFieldTranslatableBaseInstance')
      ->setConstructorArgs(array($wrapper, $handler, $factory, $mediator))
      ->setMethods(array('isTranslatable', 'getSourceLanguage'))
      ->getMock();

    $translatable->expects($this->any())
      ->method('isTranslatable')
      ->willReturn($isTranslatable);

    $translatable->expects($this->any())
      ->method('getSourceLanguage')
      ->willReturn($sourceLang);

    return $translatable;
  }

  /**
   * Returns a mock Entity wrapper, suitable for cases where the wrapped entity
   * does not need to be used for observation.
   *
   * @return \PHPUnit_Framework_MockObject_MockObject
   */
  protected function getMockWrapper() {
    return $this->getMock('\EntityDrupalWrapper');
  }

  /**
   * Returns a mock DrupalHandler instance, suitable for cases where the Drupal
   * Handler does not need to be used for observation.
   *
   * @return \PHPUnit_Framework_MockObject_MockObject
   */
  protected function getMockHandler() {
    return $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler');
  }

  /**
   * Returns a mock EntityTranslatableFactory instance, suitable for cases where
   * the factory does not need to be used for observation.
   *
   * @return \PHPUnit_Framework_MockObject_MockObject
   */
  protected function getMockFactory() {
    return $this->getMockBuilder('EntityXliff\Drupal\Factories\EntityTranslatableFactory')
      ->disableOriginalConstructor()
      ->getMock();
  }

  /**
   * Returns a mock FieldMediator instance, suitable for cases where the field
   * mediator does not need to be used for observation.
   *
   * @return \PHPUnit_Framework_MockObject_MockObject
   */
  protected function getMockMediator() {
    return $this->getMockBuilder('EntityXliff\Drupal\Mediator\FieldMediator')
      ->disableOriginalConstructor()
      ->getMock();
  }

}

/**
 * Class EntityFieldTranslatableBaseInstance
 * @package EntityXliff\Drupal\Tests\Translatable
 *
 * Concrete, testable instance extending EntityFieldTranslatableBase.
 */
class EntityFieldTranslatableBaseInstance extends EntityFieldTranslatableBase {

  /**
   * Helper method to return the internal targetEntities property for testing.
   *
   * @return \EntityDrupalWrapper[]
   */
  public function getTargetEntities() {
    return $this->targetEntities;
  }

  /**
   * Helper method to set the internal targetEntities property for testing.
   *
   * @param \EntityDrupalWrapper[] $targetEntities
   */
  public function setTargetEntities($targetEntities) {
    $this->targetEntities = $targetEntities;
  }

  /**
   * Helper method to get the internal, wrapped entity represented by this
   * translatable.
   *
   * @return \EntityDrupalWrapper
   */
  public function getEntity() {
    return $this->entity;
  }

}

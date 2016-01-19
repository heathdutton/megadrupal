<?php

namespace EntityXliff\Drupal\Tests\Translatable\Content {

  use EntityXliff\Drupal\Factories\EntityTranslatableFactory;
  use EntityXliff\Drupal\Translatable\Content\FieldCollectionTranslatable;
  use EntityXliff\Drupal\Utils\DrupalHandler;


  class FieldCollectionTranslatableTest extends \PHPUnit_Framework_TestCase {

    /**
     * {@inheritdoc}
     */
    protected function setUp() {
      $GLOBALS['user'] = (object) array('uid' => 123);
    }

    /**
     * {@inheritdoc}
     */
    protected function tearDown() {
      unset($GLOBALS['user']);
    }

    /**
     * Tests that the isTranslatable method just returns TRUE.
     *
     * @test
     */
    public function isTranslatable() {
      $observerWrapper = $this->getMock('\EntityDrupalWrapper');
      $mockDrupal = $this->getMockDrupalHandlerForConstructor();

      $translatable = new FieldCollectionTranslatable($observerWrapper, $mockDrupal);
      $this->assertSame(TRUE, $translatable->isTranslatable());
    }

    /**
     * Tests that the saveWrapper method calls the save method on the encapsulated
     * Field Collection itself, and preventing the save from affecting the host.
     *
     * @test
     */
    public function saveWrapper() {
      $mockDrupal = $this->getMockDrupalHandlerForConstructor();

      // FieldCollectionItemEntity::save() should be called once with arg "TRUE"
      $observerFieldCollection = $this->getMock('\FieldCollectionItemEntity', array('save'));
      $observerFieldCollection->expects($this->once())
        ->method('save')
        ->with($this->equalTo(TRUE));

      // EntityDrupalWrapper::value() should be called once and return the raw
      // field collection item.
      $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('value'));
      $observerWrapper->expects($this->once())
        ->method('value')
        ->willReturn($observerFieldCollection);

      // Instantiate the Field Collection Translatable and call saveWrapper().
      $translatable = new FieldCollectionTranslatable($observerWrapper, $mockDrupal);
      $translatable->saveWrapper($observerWrapper, 'whatever');
    }

    /**
     * Tests that the FieldCollectionTranslatable::getHostEntity() method pulls
     * the host entity from the wrapped field collection in the expected way.
     *
     * @test
     */
    public function getHostEntity() {
      $untouchedUser = $GLOBALS['user'];
      $expectedHostEntity = 'Host entity value pulled from the field collection';
      $mockWrapper = $this->getMock('\EntityDrupalWrapper');

      // FieldCollectionItemEntity::hostEntity() should be called and return the
      // expected host entity.
      $observerFieldCollection = $this->getMock('\FieldCollectionItemEntity', array('hostEntity'));
      $observerFieldCollection->expects($this->once())
        ->method('hostEntity')
        ->willReturn($expectedHostEntity);

      // DrupalHandler::saveSession() should be called twice with args FALSE and
      // TRUE in that order.
      $observerDrupal = $this->getMockDrupalHandlerForConstructor();
      $observerDrupal->expects($this->exactly(2))
        ->method('saveSession')
        ->withConsecutive(array($this->equalTo(FALSE)), array($this->equalTo(TRUE)));

      // Instantiate the FieldCollection translatable and call getHostEntity().
      $translatable = new FieldCollectionTranslatable($mockWrapper, $observerDrupal);
      $returnedHostEntity = $translatable->getHostEntity($observerFieldCollection);

      // The method call should return the expected host entity and the global
      // user should be remain as it was before the method call.
      $this->assertEquals($expectedHostEntity, $returnedHostEntity);
      $this->assertEquals($untouchedUser, $GLOBALS['user']);
    }

    /**
     * Tests that the source language is pulled from the host entity and its
     * associated translatable.
     *
     * @test
     */
    public function getSourceLanguageFromHost() {
      $expectedLanguage = 'en';
      $expectedHostEntity = 'Host entity value pulled from the field collection';
      $expectedHostEntityType = 'node';
      $expectedHostWrapper = $this->getMock('\EntityDrupalWrapper');

      // Ensure we pull the host entity type from the field collection.
      $observerFieldCollection = $this->getMock('\FieldCollectionItemEntity', array('hostEntityType'));
      $observerFieldCollection->expects($this->once())
        ->method('hostEntityType')
        ->willReturn($expectedHostEntityType);

      $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('value'));
      $observerWrapper->expects($this->atLeastOnce())
        ->method('value')
        ->willReturn($observerFieldCollection);

      // Ensure we wrap the host entity in a metadata wrapper.
      $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler');
      $observerDrupal->expects($this->once())
        ->method('entityMetadataWrapper')
        ->with($this->equalTo($expectedHostEntityType), $this->equalTo($expectedHostEntity))
        ->willReturn($expectedHostWrapper);

      // Ensure the source language is returned from the host entity.
      $observerTranslatable = $this->getMockBuilder('EntityXliff\Drupal\Translatable\EntityTranslatableBase')
        ->disableOriginalConstructor()
        ->getMock();
      $observerTranslatable->expects($this->once())
        ->method('getSourceLanguage')
        ->willReturn($expectedLanguage);

      // Ensure "getTranslatable" is called on with the expected host wrapper.
      $observerFactory = $this->getMockBuilder('EntityXliff\Drupal\Factories\EntityTranslatableFactory')
        ->disableOriginalConstructor()
        ->getMock();
      $observerFactory->expects($this->once())
        ->method('getTranslatable')
        ->with($expectedHostWrapper)
        ->willReturn($observerTranslatable);

      // Instantiate a field collection translatable and set the host entity.
      $translatable = new MockFieldCollectionTranslatableForSourceLangTest($observerWrapper, $observerDrupal, $observerFactory);
      $translatable->setExpectedHost($expectedHostEntity);

      // Ensure the returned language is the expected language.
      $this->assertEquals($expectedLanguage, $translatable->getSourceLanguage());
    }

    /**
     * Tests that the source language is pulled from the site default when the
     * host entity is language neutral.
     *
     * @test
     */
    public function getSourceLanguageWhenHostIsLanguageNeutral() {
      $givenLanguage = DrupalHandler::LANGUAGE_NONE;
      $expectedLanguage = 'en';
      $expectedHostEntity = 'Host entity value pulled from the field collection';
      $expectedHostEntityType = 'node';
      $expectedHostWrapper = $this->getMock('\EntityDrupalWrapper');

      // Ensure we pull the host entity type from the field collection.
      $observerFieldCollection = $this->getMock('\FieldCollectionItemEntity', array('hostEntityType'));
      $observerFieldCollection->expects($this->once())
        ->method('hostEntityType')
        ->willReturn($expectedHostEntityType);

      $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('value'));
      $observerWrapper->expects($this->atLeastOnce())
        ->method('value')
        ->willReturn($observerFieldCollection);

      // Ensure we wrap the host entity in a metadata wrapper.
      $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler');
      $observerDrupal->expects($this->once())
        ->method('entityMetadataWrapper')
        ->with($this->equalTo($expectedHostEntityType), $this->equalTo($expectedHostEntity))
        ->willReturn($expectedHostWrapper);
      // Ensures the default language is pulled from Drupal.
      $observerDrupal->expects($this->once())
        ->method('languageDefault')
        ->with('language')
        ->willReturn($expectedLanguage);

      // Ensure the source language is returned from the host entity.
      $observerTranslatable = $this->getMockBuilder('EntityXliff\Drupal\Translatable\EntityTranslatableBase')
        ->disableOriginalConstructor()
        ->getMock();
      $observerTranslatable->expects($this->once())
        ->method('getSourceLanguage')
        ->willReturn($givenLanguage);

      // Ensure "getTranslatable" is called on with the expected host wrapper.
      $observerFactory = $this->getMockBuilder('EntityXliff\Drupal\Factories\EntityTranslatableFactory')
        ->disableOriginalConstructor()
        ->getMock();
      $observerFactory->expects($this->once())
        ->method('getTranslatable')
        ->with($expectedHostWrapper)
        ->willReturn($observerTranslatable);

      // Instantiate a field collection translatable and set the host entity.
      $translatable = new MockFieldCollectionTranslatableForSourceLangTest($observerWrapper, $observerDrupal, $observerFactory);
      $translatable->setExpectedHost($expectedHostEntity);

      // Ensure the returned language is the expected language.
      $this->assertEquals($expectedLanguage, $translatable->getSourceLanguage());
    }

    /**
     * Tests that the getTargetEntity method will pull a response from its
     * internal static cache.
     *
     * @test
     */
    public function getTargetEntityFromStaticCache() {
      $targetLang = 'de';
      $expectedEntity = 'expected entity wrapper';
      $targetEntities = array($targetLang => $expectedEntity);

      $translatable = new MockFieldCollectionTranslatable();
      $translatable->setTargetEntities($targetEntities);
      $this->assertEquals($expectedEntity, $translatable->getTargetEntity($targetLang));
    }

    /**
     * Tests the getTargetEntity method in the case that the host language does
     * not match the target, and thus we need to create brand new field collection
     * item, cloning the wrapped collection.
     *
     * @test
     */
    public function getTargetEntityTranslationDoesNotExist() {
      $targetLang = 'de';
      $expectedHostEntityType = 'node';
      $expectedRawHost = 'Raw host entity object.';
      $mockWrapper = $this->getMock('\EntityDrupalWrapper');

      // FieldCollectionTranslatable::getTargetEntity() should attempt to access
      // the wrapped entity's host entity wrapper. From the host entity wrapper,
      // the language should be returned (and for testing, should not return the
      // expected target language), and the host's type should be returned.
      $observerHost = $this->getMock('\EntityDrupalWrapper', array('type'));
      $observerHost->expects($this->once())
        ->method('type')
        ->willReturn($expectedHostEntityType);
      $observerHost->language = $this->getMock('\EntityMetadataWrapper', array('value'));
      $observerHost->language->expects($this->once())
        ->method('value')
        ->willReturn('not-' . $targetLang);

      // The FieldCollectionTranslatable's DrupalHandler should be used to:
      // - Check if the Entity Translation module exists (@todo, still necessary?)
      // - Wrap two different entities:
      //   - The host entity (on the first go around),
      //   - The returned field collection entity (on the second go around).
      $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler');
      $observerDrupal->expects($this->exactly(2))
        ->method('entityMetadataWrapper')
        ->withConsecutive(array(
          $this->equalTo($expectedHostEntityType),
          $this->equalTo($expectedRawHost),
        ), array(
          $this->equalTo('field_collection_item')
        ))
        ->willReturnOnConsecutiveCalls($observerHost, $this->returnArgument(1));

      // FieldCollectionTranslatable should attempt to access the target entity's
      // host entity type, and set the host entity.
      $observerTarget = $this->getMock('\FieldCollectionItemEntity', array(
        'hostEntityType',
        'setHostEntity'
      ));
      $observerTarget->expects($this->once())
        ->method('hostEntityType')
        ->willReturn($expectedHostEntityType);
      $observerTarget->expects($this->once())
        ->method('setHostEntity')
        ->with($this->equalTo($expectedHostEntityType), $this->equalTo($expectedRawHost));

      // We expect that the following properties on the target field collection
      // will be removed.
      $observerTarget->item_id = 'should be unset';
      $observerTarget->revision_id = 'should also be unset';

      // Set up the FieldCollectionTranslatable for testing and return the target.
      $translatable = new MockFieldCollectionTranslatableForTargetEntityTest($mockWrapper, $observerDrupal);
      $translatable->setExpectedTarget($observerTarget);
      $translatable->setExpectedHost($expectedRawHost);
      $actualTarget = $translatable->getTargetEntity($targetLang);

      // Ensure that EntityTranslatableBase::getRawEntity() was called with the
      // wrapped entity injected in the constructor.
      $this->assertEquals($mockWrapper, $translatable->gotRawEntity);

      // Ensure that FieldCollectionTranslatable::getHostEntity() was called with
      // the target entity wrapper loaded from the prior getRawEntity() call.
      $this->assertEquals($observerTarget, $translatable->gotHostEntity);

      // Ensure the target entity returned is a FieldCollectionItemEntity.
      $this->assertInstanceOf('\FieldCollectionItemEntity', $actualTarget);

      // Ensure that the FieldCollectionItemEntity clone process set and unset all
      // properties as expected.
      $this->assertFalse(isset($actualTarget->item_id));
      $this->assertFalse(isset($actualTarget->revision_id));
      $this->assertEquals(TRUE, $actualTarget->is_new);
    }

    /**
     * Tests the getTargetEntity method in the case that the host language does
     * not match the target, and thus we need to create brand new field collection
     * item, cloning the wrapped collection.
     *
     * @test
     */
    public function getTargetEntityTranslationExists() {
      $targetLang = 'de';
      $expectedHostEntityType = 'node';
      $mockWrapper = $this->getMock('\EntityDrupalWrapper');

      // FieldCollectionTranslatable::getTargetEntity() should attempt to access
      // the wrapped entity's host entity wrapper. From the host entity wrapper,
      // the language should be returned (and for testing, should return the
      // expected target language), and the host's type should be returned.
      $observerHost = $this->getMock('\EntityDrupalWrapper', array('type'));
      $observerHost->expects($this->once())
        ->method('type')
        ->willReturn($expectedHostEntityType);
      $observerHost->language = $this->getMock('\EntityMetadataWrapper', array('value'));
      $observerHost->language->expects($this->once())
        ->method('value')
        ->willReturn($targetLang);

      // The FieldCollectionTranslatable's DrupalHandler should be used to:
      // - Check if the Entity Translation module exists (@todo, still necessary?)
      // - Wrap the field collection entity just prior to return.
      $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler');
      $observerDrupal->expects($this->once())
        ->method('entityMetadataWrapper')
        ->with($this->equalTo('field_collection_item'))
        ->willReturnArgument(1);

      // FieldCollectionTranslatable should attempt to set the host entity.
      $observerTarget = $this->getMock('\FieldCollectionItemEntity', array(
        'hostEntityType',
        'setHostEntity'
      ));
      $observerTarget->expects($this->once())
        ->method('setHostEntity')
        ->with($this->equalTo($expectedHostEntityType), $this->callback(function ($subject) use ($observerTarget) {
          return get_class($observerTarget) === get_class($subject);
        }));

      // Set up the FieldCollectionTranslatable for testing and return the target.
      $translatable = new MockFieldCollectionTranslatableForTargetEntityTest($mockWrapper, $observerDrupal);
      $translatable->setExpectedTarget($observerTarget);
      $translatable->setExpectedHost(FALSE);
      $translatable->setExpectedParent($observerHost);
      $actualTarget = $translatable->getTargetEntity($targetLang);

      // Ensure that the host entity is pulled from the wrapped entity injected
      // in the constructor.
      $this->assertEquals($mockWrapper, $translatable->gotParentEntity);

      // Ensure the target entity returned is a FieldCollectionItemEntity.
      $this->assertInstanceOf('\FieldCollectionItemEntity', $actualTarget);

      // Ensure that the FieldCollectionItemEntity clone process set and unset all
      // properties as expected.
      $this->assertEquals(TRUE, $actualTarget->default_revision);
      $this->assertEquals(FALSE, $actualTarget->is_new_revision);
      $this->assertEquals(FALSE, $actualTarget->is_new);
    }

    /**
     * Returns a mock DrupalHandler suitable for use in constructing an instance
     * of the FieldCollection translatable.
     *
     * @return \PHPUnit_Framework_MockObject_MockObject
     */
    protected function getMockDrupalHandlerForConstructor() {
      $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler');
      $observerDrupal->expects($this->once())
        ->method('entityXliffGetFieldHandlers')
        ->willReturn(array());

      return $observerDrupal;
    }

  }

  /**
   * Class MockFieldCollectionTranslatable
   * @package EntityXliff\Drupal\Tests\Translatable\Content
   *
   * Extension of the FieldCollectionTranslatable class that bypasses the real
   * constructor, but allows us to inject the components we need to test
   * associated methods.
   */
  class MockFieldCollectionTranslatable extends FieldCollectionTranslatable {

    /**
     * Overrides the parent constructor to just perform injection.
     *
     * @param \EntityDrupalWrapper $wrapper
     * @param DrupalHandler $handler
     */
    public function __construct(\EntityDrupalWrapper $wrapper = NULL, DrupalHandler $handler = NULL) {
      $this->entity = $wrapper;
      $this->drupal = $handler;
    }

    /**
     * Optionally inject specific target entity values.
     *
     * @param array $targetEntities
     */
    public function setTargetEntities(array $targetEntities) {
      $this->targetEntities = $targetEntities;
    }

  }

  /**
   * Class MockFieldCollectionTranslatableForSourceLangTest
   * @package EntityXliff\Drupal\Tests\Translatable\Content
   *
   * Extension of the FieldCollectionTranslatable class used to test the source
   * language getter.
   */
  class MockFieldCollectionTranslatableForSourceLangTest extends FieldCollectionTranslatable {

    protected $expectedHost;

    /**
     * Overrides the parent constructor to just perform injection.
     *
     * @param \EntityDrupalWrapper $wrapper
     * @param DrupalHandler $handler
     */
    public function __construct(\EntityDrupalWrapper $wrapper = NULL, DrupalHandler $handler = NULL, EntityTranslatableFactory $factory = NULL) {
      $this->entity = $wrapper;
      $this->drupal = $handler;
      $this->translatableFactory = $factory;
    }

    public function setExpectedHost($expectedHost) {
      $this->expectedHost = $expectedHost;
    }

    public function getHostEntity(\FieldCollectionItemEntity $fieldCollection) {
      $this->gotHostEntity = $fieldCollection;
      return $this->expectedHost;
    }

  }

  /**
   * Class MockFieldCollectionTranslatableForTargetEntityTest
   * @package EntityXliff\Drupal\Tests\Translatable\Content
   *
   * Extension of the FieldCollectionTranslatable class used to test the target
   * entity getter.
   */
  class MockFieldCollectionTranslatableForTargetEntityTest extends FieldCollectionTranslatable {

    protected $expectedTarget;
    protected $expectedHost;
    protected $expectedParent;

    public $gotRawEntity;
    public $gotHostEntity;
    public $gotParentEntity;

    /**
     * Overrides the parent constructor to just perform injection.
     *
     * @param \EntityDrupalWrapper $wrapper
     * @param DrupalHandler $handler
     */
    public function __construct(\EntityDrupalWrapper $wrapper = NULL, DrupalHandler $handler = NULL) {
      $this->entity = $wrapper;
      $this->drupal = $handler;
    }

    public function setExpectedTarget($expectedTarget) {
      $this->expectedTarget = $expectedTarget;
    }

    public function setExpectedHost($expectedHost) {
      $this->expectedHost = $expectedHost;
    }

    public function setExpectedParent($expectedParent) {
      $this->expectedParent = $expectedParent;
    }

    public function getRawEntity(\EntityDrupalWrapper $entity) {
      $this->gotRawEntity = $entity;
      return $this->expectedTarget;
    }

    public function getHostEntity(\FieldCollectionItemEntity $fieldCollection) {
      $this->gotHostEntity = $fieldCollection;
      return $this->expectedHost;
    }

    public function getParent(\EntityMetadataWrapper $entity) {
      $this->gotParentEntity = $entity;
      return $this->expectedParent;
    }

  }

}

// Only necessary because of MockFieldCollectionTranslatableForTargetEntityTest
namespace {
  class EntityMetadataWrapper {}
  class EntityDrupalWrapper extends EntityMetadataWrapper {}
}

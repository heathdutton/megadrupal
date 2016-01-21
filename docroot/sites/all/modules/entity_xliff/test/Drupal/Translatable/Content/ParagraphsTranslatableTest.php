<?php

namespace EntityXliff\Drupal\Tests\Translatable\Content;

use EntityXliff\Drupal\Factories\EntityTranslatableFactory;
use EntityXliff\Drupal\Translatable\Content\ParagraphsTranslatable;
use EntityXliff\Drupal\Utils\DrupalHandler;


class ParagraphsTranslatableTest extends \PHPUnit_Framework_TestCase {

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

    $translatable = new ParagraphsTranslatable($observerWrapper, $mockDrupal);
    $this->assertSame(TRUE, $translatable->isTranslatable());
  }

  /**
   * Tests that the saveWrapper method calls the save method on the encapsulated
   * Paragraphs Item itself, and preventing the save from affecting the host.
   *
   * @test
   */
  public function saveWrapper() {
    $mockDrupal = $this->getMockDrupalHandlerForConstructor();

    // FieldCollectionItemEntity::save() should be called once with arg "TRUE"
    $observerFieldCollection = $this->getMock('\ParagraphsItemEntity', array('save'));
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
    $translatable = new ParagraphsTranslatable($observerWrapper, $mockDrupal);
    $translatable->saveWrapper($observerWrapper, 'whatever');
  }

  /**
   * Tests that the ParagraphsTranslatable::getHostEntity() method pulls the
   * host entity from the wrapped field collection in the expected way.
   *
   * @test
   */
  public function getHostEntity() {
    $untouchedUser = $GLOBALS['user'];
    $expectedHostEntity = 'Host entity value pulled from the paragraphs item';
    $mockWrapper = $this->getMock('\EntityDrupalWrapper');

    // ParagraphsItemEntity::hostEntity() should be called and return the
    // expected host entity.
    $observerParagraph = $this->getMock('\ParagraphsItemEntity', array('hostEntity'));
    $observerParagraph->expects($this->once())
      ->method('hostEntity')
      ->willReturn($expectedHostEntity);

    // DrupalHandler::saveSession() should be called twice with args FALSE and
    // TRUE in that order.
    $observerDrupal = $this->getMockDrupalHandlerForConstructor();
    $observerDrupal->expects($this->exactly(2))
      ->method('saveSession')
      ->withConsecutive(array($this->equalTo(FALSE)), array($this->equalTo(TRUE)));

    // Instantiate the ParagraphsTranslatable and call getHostEntity().
    $translatable = new ParagraphsTranslatable($mockWrapper, $observerDrupal);
    $returnedHostEntity = $translatable->getHostEntity($observerParagraph);

    // The method call should return the expected host entity and the global
    // user should be remain as it was before the method call.
    $this->assertEquals($expectedHostEntity, $returnedHostEntity);
    $this->assertEquals($untouchedUser, $GLOBALS['user']);
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

    $translatable = new MockParagraphsTranslatable();
    $translatable->setTargetEntities($targetEntities);
    $this->assertEquals($expectedEntity, $translatable->getTargetEntity($targetLang));
  }

  /**
   * Tests that the getTargetEntity method returns the wrapped target entity.
   *
   * @test
   */
  public function getTargetEntityUncached() {
    $targetLang = 'de';
    $willedParagraph = 'The wrapped paragraph item.';
    $expectedTarget = 'The wrapped target paragraph.';

    $observerDrupal = $this->getMockDrupalHandlerForConstructor();
    $observerDrupal->expects($this->once())
      ->method('entityMetadataWrapper')
      ->with($this->equalTo('paragraphs_item'), $this->equalTo($willedParagraph))
      ->willReturn($expectedTarget);

    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('value'));
    $observerWrapper->expects($this->once())
      ->method('value')
      ->willReturn($willedParagraph);

    $translatable = new ParagraphsTranslatable($observerWrapper, $observerDrupal);
    $this->assertSame($expectedTarget, $translatable->getTargetEntity($targetLang));
  }


  /**
   * Tests that the source language is pulled from the host entity and its
   * associated translatable.
   *
   * @test
   */
  public function getSourceLanguageFromHost() {
    $expectedLanguage = 'en';
    $expectedHostEntity = 'Host entity value pulled from the paragraph';
    $expectedHostEntityType = 'node';
    $expectedHostWrapper = $this->getMock('\EntityDrupalWrapper');

    // Ensure we pull the host entity type from the paragraph.
    $observerParagraph = $this->getMock('\ParagraphsItemEntity', array('hostEntityType'));
    $observerParagraph->expects($this->once())
      ->method('hostEntityType')
      ->willReturn($expectedHostEntityType);

    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('value'));
    $observerWrapper->expects($this->atLeastOnce())
      ->method('value')
      ->willReturn($observerParagraph);

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
    $translatable = new MockParagraphsTranslatableForSourceLangTest($observerWrapper, $observerDrupal, $observerFactory);
    $translatable->setExpectedHost($expectedHostEntity);

    // Ensure the returned language is the expected language. Subsequent calls
    // should also bypass the calls above and pull from static cache.
    $this->assertEquals($expectedLanguage, $translatable->getSourceLanguage());
    $this->assertEquals($expectedLanguage, $translatable->getSourceLanguage());
  }

  /**
   * Tests that the source language is pulled from the site default when the
   * host entity is language neutral.
   *
   * @test
   */
  public function getSourceLanguageWhenHostIsLanguageNeutral() {
    $expectedLanguage = 'de';
    $observerDrupal =$this->getMock('EntityXliff\Drupal\Utils\DrupalHandler');
    $observerDrupal->expects($this->once())
      ->method('languageDefault')
      ->with($this->equalTo('language'))
      ->willReturn($expectedLanguage);

    // Instantiate the translatable and manually set the source language to UND.
    $translatable = new MockParagraphsTranslatableForSourceLangTest(NULL, $observerDrupal);
    $translatable->setSourceLanguage(DrupalHandler::LANGUAGE_NONE);
    $this->assertSame($expectedLanguage, $translatable->getSourceLanguage());

    // A subsequent call to get the source language should bypass the calls set
    // above and pull from static cache.
    $this->assertSame($expectedLanguage, $translatable->getSourceLanguage());
  }

  /**
   * Returns a mock DrupalHandler suitable for use in constructing an instance
   * of the Paragraphs translatable.
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
 * Class MockParagraphsTranslatable
 * @package EntityXliff\Drupal\Tests\Translatable\Content
 *
 * Extension of the ParagraphsTranslatable class that bypasses the real
 * constructor, but allows us to inject the components we need to test
 * associated methods.
 */
class MockParagraphsTranslatable extends ParagraphsTranslatable {

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
 * Class MockParagraphsTranslatableForSourceLangTest
 * @package EntityXliff\Drupal\Tests\Translatable\Content
 *
 * Extension of the ParagraphsTranslatable class used to test the source
 * language getter.
 */
class MockParagraphsTranslatableForSourceLangTest extends ParagraphsTranslatable {

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

  public function getHostEntity(\ParagraphsItemEntity $paragraph) {
    $this->gotHostEntity = $paragraph;
    return $this->expectedHost;
  }

  public function setSourceLanguage($sourceLanguage) {
    $this->sourceLanguage = $sourceLanguage;
  }

}

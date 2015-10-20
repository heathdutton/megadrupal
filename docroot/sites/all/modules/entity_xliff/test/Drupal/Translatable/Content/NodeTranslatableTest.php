<?php

namespace EntityXliff\Drupal\Tests\Translatable\Content;

use EntityXliff\Drupal\Translatable\Content\NodeTranslatable;
use EntityXliff\Drupal\Utils\DrupalHandler;


class NodeTranslatableTest extends \PHPUnit_Framework_TestCase {

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
   * Tests that constructor will pull the translation set for the wrapped entity
   * as expected, specifically in the case that the wrapped entity represents an
   * as-yet untranslated node.
   *
   * @test
   */
  public function constructTranslatableAsYetUntranslated() {
    $expectedNid = 123;

    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('getIdentifier', 'raw'));
    $observerWrapper->expects($this->once())
      ->method('getIdentifier')
      ->willReturn((string) $expectedNid);
    $observerWrapper->expects($this->once())
      ->method('raw')
      ->willReturn((object) array());

    $observerDrupal = $this->getMockHandlerForConstructor($expectedNid);

    $mockFactory = $this->getMockBuilder('EntityXliff\Drupal\Factories\EntityTranslatableFactory')
      ->disableOriginalConstructor()
      ->getMock();
    $mockMediator = $this->getMock('EntityXliff\Drupal\Mediator\FieldMediator');
    $translatable = new NodeTranslatable($observerWrapper, $observerDrupal, $mockFactory, $mockMediator);
  }

  /**
   * Tests that the constructor will pull the translation set from the wrapped
   * entity's translation source ID in the case that the wrapped entity is
   * already translated, and therefore already part of a translation set.
   *
   * @test
   */
  public function constructTranslatableAlreadyTranslated() {
    $expectedNid = 123;
    $expectedRawNode = (object) array(
      'tnid' => $expectedNid,
    );

    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('raw'));
    $observerWrapper->expects($this->once())
      ->method('raw')
      ->willReturn($expectedRawNode);

    $observerDrupal = $this->getMockHandlerForConstructor($expectedNid);

    $mockFactory = $this->getMockBuilder('EntityXliff\Drupal\Factories\EntityTranslatableFactory')
      ->disableOriginalConstructor()
      ->getMock();
    $mockMediator = $this->getMock('EntityXliff\Drupal\Mediator\FieldMediator');
    $translatable = new MockNodeTranslatableForConstructor($observerWrapper, $observerDrupal, $mockFactory, $mockMediator, $expectedRawNode);
  }

  /**
   * Tests that the getTranslatableFields method returns the "title" property
   * in the event that the wrapped entity is translatable.
   *
   * @test
   */
  public function getTranslatableFields() {
    $mockDrupal = $this->getMock('\EntityDrupalWrapper', array('getPropertyInfo'));
    $mockDrupal->expects($this->any())
      ->method('getPropertyInfo')
      ->willReturn(array());

    $translatable = $this->getMockBuilder('EntityXliff\Drupal\Tests\Translatable\Content\MockNodeTranslatable')
      ->setMethods(array('isTranslatable'))
      ->setConstructorArgs(array($mockDrupal))
      ->getMock();
    $translatable->expects($this->atLeastOnce())
      ->method('isTranslatable')
      ->willReturn(TRUE);

    $fields = $translatable->getTranslatableFields();
    $this->assertTrue(in_array('title', $fields));
  }

  /**
   * Tests that the isTranslatable method checks that the bundle of the wrapped
   * entity is enabled for Content translation.
   *
   * @test
   */
  public function isTranslatable() {
    $expectedBundle = 'article';
    $expectedTranslatable = TRUE;

    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('getBundle'));
    $observerWrapper->expects($this->once())
      ->method('getBundle')
      ->willReturn($expectedBundle);

    $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler');
    $observerDrupal->expects($this->once())
      ->method('translationSupportedType')
      ->with($this->equalTo($expectedBundle))
      ->willReturn($expectedTranslatable);

    $translatable = new MockNodeTranslatable($observerWrapper, $observerDrupal);
    $this->assertSame($expectedTranslatable, $translatable->isTranslatable());
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

    $translatable = new MockNodeTranslatable();
    $translatable->setTargetEntities($targetEntities);
    $this->assertEquals($expectedEntity, $translatable->getTargetEntity($targetLang));
  }

  /**
   * Tests the getTargetEntity method in the case that a translation set exists
   * for the wrapped entity and the translation for the given target language
   * already exists.
   *
   * @test
   */
  public function getTargetEntityTranslationExists() {
    $targetLang = 'de';
    $targetNid = 123;
    $tset = array($targetLang => (object) array(
      'nid' => $targetNid,
    ));
    $expectedNode = clone $tset[$targetLang];
    $expectedNode->revision = FALSE;
    $expectedEntity = 'expected entity wrapper';

    $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler', array('nodeLoad', 'entityMetadataWrapper'));
    $observerDrupal->expects($this->once())
      ->method('nodeLoad')
      ->with($this->equalTo($targetNid), $this->equalTo(NULL), $this->equalTo(TRUE))
      ->willReturn($tset[$targetLang]);
    $observerDrupal->expects($this->once())
      ->method('entityMetadataWrapper')
      ->with($this->equalTo('node'), $this->equalTo($expectedNode))
      ->willReturn($expectedEntity);

    $translatable = new MockNodeTranslatable(NULL, $observerDrupal);
    $translatable->setTranslationSet($tset);
    $this->assertEquals($expectedEntity, $translatable->getTargetEntity($targetLang));
  }

  /**
   * Tests that the getTargetEntity method will clone the original node and edit
   * properties on it to ensure that the translation will created successfully
   * in the translation set.
   *
   * @test
   */
  public function getTargetEntityNewTranslation() {
    $targetLang = 'de';
    $targetNid = 123;
    $willedRawNode = (object) array(
      'nid' => $targetNid,
      'vid' => 456,
    );
    $expectedRawnode = (object) array(
      'translation_source' => clone $willedRawNode,
      'is_new' => TRUE,
      'tnid' => $targetNid,
      'language' => $targetLang,
    );
    $expectedEntity = 'expected entity wrapper';

    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('getIdentifier'));
    $observerWrapper->expects($this->any())
      ->method('getIdentifier')
      ->willReturn($targetNid);

    $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler', array('entityMetadataWrapper', 'nodeLoad'));
    $observerDrupal->expects($this->once())
      ->method('entityMetadataWrapper')
      ->with($this->equalTo('node'), $this->equalTo($expectedRawnode))
      ->willReturn($expectedEntity);
    // Ensure translation initialization is triggered in this case.
    $observerDrupal->expects($this->atLeastOnce())
      ->method('nodeLoad')
      ->willReturn((object) array(
        'language' => 'fr',
        'tnid' => 1,
      ));

    $translatable = $this->getMockBuilder('EntityXliff\Drupal\Tests\Translatable\Content\MockNodeTranslatable')
      ->setMethods(array('getRawEntity'))
      ->setConstructorArgs(array($observerWrapper, $observerDrupal))
      ->getMock();
    $translatable->expects($this->atLeastOnce())
      ->method('getRawEntity')
      ->with($this->equalTo($observerWrapper))
      ->willReturn($willedRawNode);

    $this->assertEquals($expectedEntity, $translatable->getTargetEntity($targetLang));
  }

  /**
   * Tests that translation initialization runs through the expected steps under
   * the expected circumstances.
   *
   * @test
   */
  public function initializeTranslation() {
    $expectedNid = 123;
    $willedNode = (object) array(
      'nid' => $expectedNid,
      'language' => DrupalHandler::LANGUAGE_NONE,
      'tnid' => NULL,
    );
    $expectedNode = clone $willedNode;
    $expectedNode->language = 'en';
    $expectedNode->tnid = $expectedNid;

    $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('getIdentifier'));
    $observerWrapper->expects($this->once())
      ->method('getIdentifier')
      ->willReturn((string) $expectedNid);
    $observerWrapper->language = $this->getMock('\EntityMetadataWrapper', array('value'));
    $observerWrapper->language->expects($this->once())
      ->method('value')
      ->willReturn('en');

    $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler', array(
      'nodeLoad',
      'nodeSave',
      'entityMetadataWrapper',
      'staticReset',
      'translationNodeGetTranslations',
      'userLoad',
      'saveSession',
      ));
    $observerDrupal->expects($this->once())
      ->method('nodeLoad')
      ->with($this->equalTo($expectedNid), $this->equalTo(NULL), $this->equalTo(TRUE))
      ->willReturn($willedNode);
    $observerDrupal->expects($this->once())
      ->method('nodeSave')
      ->with($this->equalTo($expectedNode));
    $observerDrupal->expects($this->once())
      ->method('entityMetadataWrapper')
      ->with($this->equalTo('node'), $this->equalTo($expectedNode));
    $observerDrupal->expects($this->once())
      ->method('staticReset')
      ->with($this->equalTo('translation_node_get_translations'));
    // Ensures translation initialization re-populates the internal tset prop.
    $observerDrupal->expects($this->once())
      ->method('translationNodeGetTranslations')
      ->with($this->equalTo($willedNode->nid));
    $observerDrupal->expects($this->once())
      ->method('userLoad')
      ->with($this->equalTo(1));
    $observerDrupal->expects($this->exactly(2))
      ->method('saveSession')
      ->withConsecutive($this->equalTo(FALSE), $this->equalTo(TRUE));

    $translatable = new MockNodeTranslatable($observerWrapper, $observerDrupal);
    $translatable->initializeTranslation();
  }

  /**
   * Returns a mock DrupalHandler instance with all expectations for the main
   * NodeTranslatable constructor.
   *
   * @param int $expectedTnid
   * @return \PHPUnit_Framework_MockObject_MockObject
   */
  protected function getMockHandlerForConstructor($expectedTnid) {
    $drupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler', array(
      'staticReset',
      'saveSession',
      'userLoad',
      'translationNodeGetTranslations',
      'entityGetInfo',
    ));
    $drupal->expects($this->once())
      ->method('staticReset')
      ->with($this->equalTo('translation_node_get_translations'));
    $drupal->expects($this->exactly(2))
      ->method('saveSession')
      ->withConsecutive(array($this->equalTo(FALSE)), array($this->equalTo(TRUE)));
    $drupal->expects($this->once())
      ->method('userLoad')
      ->with($this->equalTo(1));
    $drupal->expects($this->once())
      ->method('translationNodeGetTranslations')
      ->with($this->equalTo($expectedTnid));
    $drupal->expects($this->any())
      ->method('entityGetInfo')
      ->willReturn(array());

    return $drupal;
  }
}

/**
 * Class MockNodeTranslatable
 * @package EntityXliff\Drupal\Tests\Translatable\Content
 *
 * Extension of the NodeTranslatable class that bypasses the real constructor,
 * but allows us to inject the components we need to test associated methods.
 */
class MockNodeTranslatable extends NodeTranslatable {

  /**
   * Overrides the parent constructor to just perform injection.
   *
   * @param \EntityDrupalWrapper $wrapper
   * @param DrupalHandler $handler
   */
  public function __construct(\EntityDrupalWrapper $wrapper = NULL, DrupalHandler$handler = NULL) {
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

  /**
   * Optionally inject translation set values.
   *
   * @param array $tset
   */
  public function setTranslationSet(array $tset) {
    $this->tset = $tset;
  }

}

/**
 * Class MockNodeTranslatableForConstructor
 * @package EntityXliff\Drupal\Tests\Translatable\Content
 *
 * Extension of the NodeTranslatable class that allows us to mock internal
 * methods that are called within the constructor.
 *
 * @see NodeTranslatableTest::constructTranslatableFromNonEnglish
 */
class MockNodeTranslatableForConstructor extends NodeTranslatable {

  /**
   * @var array
   */
  protected $expectedRawEntity;

  /**
   * {@inheritdoc}
   */
  public function __construct($wrapper, $handler, $factory, $mediator, $expectedRaw) {
    $this->expectedRawEntity = $expectedRaw;
    parent::__construct($wrapper, $handler, $factory, $mediator);
  }

  /**
   * Returns the mock raw entity value injected in the constructor.
   *
   * @param \EntityDrupalWrapper $wrapper
   * @return array
   */
  public function getRawEntity(\EntityDrupalWrapper $wrapper) {
    return $this->expectedRawEntity;
  }

}

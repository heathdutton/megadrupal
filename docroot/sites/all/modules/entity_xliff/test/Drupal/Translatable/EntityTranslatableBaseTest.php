<?php

namespace EntityXliff\Drupal\Tests\Translatable {


  use EntityXliff\Drupal\Translatable\EntityTranslatableBase;
  use EntityXliff\Drupal\Utils\DrupalHandler;

  class EntityTranslatableBaseTest extends \PHPUnit_Framework_TestCase {

    /**
     * Tests that EntityTranslatableBase::getIdentifier() returns the entity ID as
     * expected.
     *
     * @test
     */
    public function getIdentifier() {
      $expectedIdentifier = 123;

      $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('getIdentifier'));
      $observerWrapper->expects($this->once())
        ->method('getIdentifier')
        ->willReturn($expectedIdentifier);

      $translatable = $this->getTranslatableOrNotInstance(TRUE, $observerWrapper);
      $this->assertSame($expectedIdentifier, $translatable->getIdentifier());
    }

    /**
     * Tests that EntityTranslatableBase::getLabel() returns the entity label as
     * expected.
     *
     * @test
     */
    public function getLabel() {
      $expectedLabel = 123;

      $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('label'));
      $observerWrapper->expects($this->once())
        ->method('label')
        ->willReturn($expectedLabel);

      $translatable = $this->getTranslatableOrNotInstance(TRUE, $observerWrapper);
      $this->assertSame($expectedLabel, $translatable->getLabel());
    }

    /**
     * Tests that EntityTranslatableBase::getSourceLanguage() returns the lang
     * code of the wrapped entity, as expected.
     *
     * @test
     */
    public function getSourceLanguageReal() {
      $expectedLanguage = 'fr';

      $observerWrapper = $this->getMock('\EntityDrupalWrapper');
      $observerWrapper->language = $this->getMock('\EntityMetadataWrapper', array('value'));
      $observerWrapper->language->expects($this->once())
        ->method('value')
        ->willReturn($expectedLanguage);

      $translatable = $this->getTranslatableOrNotInstance(TRUE, $observerWrapper);
      $this->assertSame($expectedLanguage, $translatable->getSourceLanguage());
    }

    /**
     * @test
     */
    public function getSourceLanguageNeutral() {
      $expectedLanguage = 'en';

      $observerDrupal = $this->getMockHandler();
      $observerDrupal->expects($this->once())
        ->method('languageDefault')
        ->with($this->equalTo('language'))
        ->willReturn($expectedLanguage);

      $observerWrapper = $this->getMock('\EntityDrupalWrapper');
      $observerWrapper->language = $this->getMock('\EntityMetadataWrapper', array('value'));
      $observerWrapper->language->expects($this->once())
        ->method('value')
        ->willReturn(DrupalHandler::LANGUAGE_NONE);

      $translatable = $this->getTranslatableOrNotInstance(TRUE, $observerWrapper, $observerDrupal);
      $this->assertSame($expectedLanguage, $translatable->getSourceLanguage());
    }

    /**
     * Ensures that EntityTranslatableBase::getData() returns translatable data
     * as expected.
     *
     * @test
     */
    public function getData() {
      $mockEntity = $this->getMockWrapper();
      $mockMediator = $this->getMockMediator();
      $listenerDrupal = $this->getMockHandler();

      // Instantiate the translatable.
      $translatable = new EntityTranslatableMockForGetData($mockEntity, $listenerDrupal, NULL, $mockMediator);

      // Ensure translatable fields are alterable, with the entity (as context).
      $listenerDrupal->expects($this->once())
        ->method('entityXliffLoadModuleIncs');
      $listenerDrupal->expects($this->once())
        ->method('alter')
        ->with('entity_xliff_translatable_fields', array_keys($translatable->translatableFieldData), $mockEntity);

      // Ensure the translatable data returned matches expectations.
      $this->assertSame($translatable->translatableFieldData, $translatable->getData());
      $this->assertSame($translatable->gotFieldFromEntity, $mockEntity);
    }

    /**
     * Ensures that EntityTranslatableBase::setData() adds translated data
     * recursively, but does not initialize translation when the $saveData flag
     * is passed in with FALSE.
     *
     * @test
     */
    public function setDataDoNotSave() {
      $mockEntity = $this->getMockWrapper();
      $saveData = FALSE;
      $expectedTranslationData = array('foo' => 'bar', 'baz' => 'fizz');
      $expectedTargetLang = 'de';

      $translatable = new EntityTranslatableMockForSetData($mockEntity);
      $translatable->setData($expectedTranslationData, $expectedTargetLang, $saveData);
      $this->assertSame($expectedTranslationData, $translatable->translationData);
      $this->assertSame($expectedTargetLang, $translatable->targetLanguage);
      $this->assertSame(array(), $translatable->translationKey);
      $this->assertFalse($translatable->initialized);
    }

    /**
     * Ensures that EntityTranslatableBase::setData() adds translated data
     * recursively, then initializes translation and iterates through internally
     * saved entities and saves them.
     *
     * @test
     */
    public function setDataAndSave() {
      $saveData = TRUE;
      $expectedTranslationData = array('foo' => 'bar', 'baz' => 'fizz');
      $expectedTargetLang = 'de';
      $expectedEntityType = 'node';
      $mockMediator = $this->getMockMediator();

      $observerEntity = $this->getMock('\EntityDrupalWrapper', array('type'));
      $observerEntity->expects($this->once())
        ->method('type')
        ->willReturn($expectedEntityType);

      $observerTranslatable = $this->getMock('EntityXliff\Drupal\Interfaces\EntityTranslatableInterface');
      $observerTranslatable->expects($this->once())
        ->method('initializeTranslation');
      $observerTranslatable->expects($this->once())
        ->method('saveWrapper')
        ->with($this->equalTo($observerEntity), $this->equalTo($expectedTargetLang));

      $observerFactory = $this->getMockFactory();
      $observerFactory->expects($this->once())
        ->method('getTranslatable')
        ->with($this->equalTo($observerEntity))
        ->willReturn($observerTranslatable);

      $observerHandler = $this->getMockHandler();
      $observerHandler->expects($this->once())
        ->method('alter')
        ->with($this->equalTo('entity_xliff_presave'), $this->equalTo($observerEntity), $this->equalTo($expectedEntityType));

      $translatable = new EntityTranslatableMockForSetData($observerEntity, $observerHandler, $observerFactory, $mockMediator);
      $translatable->setEntitiesNeedSave(array($observerEntity));

      $translatable->setData($expectedTranslationData, $expectedTargetLang, $saveData);
      $this->assertSame($expectedTranslationData, $translatable->translationData);
      $this->assertSame($expectedTargetLang, $translatable->targetLanguage);
      $this->assertSame(array(), $translatable->translationKey);
      $this->assertTrue($translatable->initialized);
    }

    /**
     * Ensures that EntityTranslatableBase::getTranslatableFields() returns an
     * empty array in the event that the wrapped entity is not translatable.
     *
     * @test
     */
    public function getTranslatableFieldsNotTranslatable() {
      $mockWrapper = $this->getMockWrapper();
      $translatable = $this->getTranslatableOrNotInstance(FALSE, $mockWrapper);
      $this->assertSame(array(), $translatable->getTranslatableFields());
    }

    /**
     * Ensures that EntityTranslatableBase::getTranslatableFields() returns all
     * fields from the wrapped entity's property info.
     *
     * @test
     */
    public function getTranslatableFieldsIsTranslatable() {
      $expectedFields = array('foo', 'bar', 'baz');

      $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('getPropertyInfo'));
      $observerWrapper->expects($this->once())
        ->method('getPropertyInfo')
        ->willReturn(array(
          'foo' => array('field' => TRUE),
          'bar' => array('field' => TRUE),
          'baz' => array('field' => TRUE),
        ));

      $translatable = $this->getTranslatableOrNotInstance(TRUE, $observerWrapper);
      $this->assertSame($expectedFields, $translatable->getTranslatableFields());
    }

    /**
     * Tests that EntityTranslatableBase::saveWrapper() saves the provided wrapper
     * as expected.
     *
     * @test
     */
    public function saveWrapper() {
      $mockEntity = $this->getMockWrapper();

      $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('save'));
      $observerWrapper->expects($this->once())
        ->method('save');

      $translatable = $this->getTranslatableOrNotInstance(TRUE, $mockEntity);
      $translatable->saveWrapper($observerWrapper, 'whatever');
    }

    /**
     * Ensures that EntityTranslatableBase::addTranslatedData() sets data on the
     * wrapped target entity as expected in the base case.
     *
     * @test
     */
    public function addTranslatedDataBaseCase() {
      $givenTranslation = array('#text' => 'Translation text');
      $expectedTranslation = array('#translation' => $givenTranslation);
      $expectedKey = array('foo', 'bar', 'baz');
      $expectedTargetLang = 'de';
      $mockEntity = $this->getMockWrapper();

      $translatable = new EntityTranslatableMockForAddTranslatedData($mockEntity);
      $translatable->addTranslatedDataRecursive($givenTranslation, $expectedKey, $expectedTargetLang);

      $this->assertSame($expectedTranslation, $translatable->gotValues);
      $this->assertSame($expectedKey, $translatable->gotKey);
      $this->assertSame($expectedTargetLang, $translatable->gotTargetLang);
    }

    /**
     * Ensures that EntityTranslatableBase::addTranslatedData() sets data on the
     * wrapped target entity as expected in the recursive case.
     *
     * @test
     */
    public function addTranslatedDataRecursiveCase() {
      $recursiveKey = 0;
      $givenTranslation = array($recursiveKey => array('#text' => 'Translation text'));
      $givenKey = array('foo', 'bar', 'baz');
      $expectedTranslation = array('#translation' => $givenTranslation[$recursiveKey]);
      $expectedKey = array_merge($givenKey, array($recursiveKey));
      $expectedTargetLang = 'de';

      $mockEntity = $this->getMockWrapper();
      $mockMediator = $this->getMockMediator();

      $observerDrupal = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler');
      $observerDrupal->expects($this->once())
        ->method('elementChildren')
        ->with($this->equalTo($givenTranslation))
        ->willReturn(array($recursiveKey));

      $translatable = new EntityTranslatableMockForAddTranslatedData($mockEntity, $observerDrupal, NULL, $mockMediator);
      $translatable->addTranslatedDataRecursive($givenTranslation, $givenKey, $expectedTargetLang);

      $this->assertSame($expectedTranslation, $translatable->gotValues);
      $this->assertSame($expectedKey, $translatable->gotKey);
      $this->assertSame($expectedTargetLang, $translatable->gotTargetLang);
    }


    /**
     * Ensures EntityTranslatableBase::setItem() calls entitySetNestedValue with
     * all of the expected parameters.
     *
     * @test
     */
    public function setItemBaseCase() {
      $givenParents = array('foo', 0, 'bar', '#text');
      $givenValue = ' String value. ';
      $expectedTargetLang = 'de';
      $expectedParents = $givenParents;
      array_pop($expectedParents);
      $mockEntity = $this->getMockWrapper();
      $mockTargetEntity = $this->getMockWrapper();

      $translatable = new EntityTranslatableMockForSetItem($mockEntity);
      $translatable->setTargetEntity($mockTargetEntity);

      $translatable->setItem($givenParents, array($givenValue), $expectedTargetLang);
      $this->assertSame($mockTargetEntity, $translatable->gotTargetEntity);
      $this->assertSame($expectedParents, $translatable->gotParents);
      $this->assertSame(trim($givenValue), $translatable->gotValue);
      $this->assertSame($expectedTargetLang, $translatable->gotTargetLang);
    }

    /**
     * Ensures EntityTranslatableBase::setItem() calls itself with the expected
     * parameters in the recursive case.
     *
     * @test
     */
    public function setItemRecursiveCase() {
      $givenParents = array('foo', 'bar', 'baz');
      $givenValue = array('String value. ');
      $expectedTargetLang = 'de';
      $mockEntity = $this->getMockWrapper();
      $mockTargetEntity = $this->getMockWrapper();

      $translatable = new EntityTranslatableMockForSetItem($mockEntity);
      $translatable->setTargetEntity($mockTargetEntity);

      $translatable->setItem($givenParents, array('fizz' => $givenValue), $expectedTargetLang);
      $this->assertSame($mockTargetEntity, $translatable->gotTargetEntity);
      $this->assertSame($givenParents, $translatable->gotParents);
      $this->assertSame(trim($givenValue[0]), $translatable->gotValue);
      $this->assertSame($expectedTargetLang, $translatable->gotTargetLang);
    }

    /**
     * Ensures that EntityTranslatableBase::getFieldFromEntity() will return an
     * empty array if the field mediator doesn't find a valid handler.
     *
     * @test
     */
    public function getFieldFromEntityUnknownFieldType() {
      $mockEntity = $this->getMockWrapper();
      $field = 'title';
      $expectedFieldValue = array();

      $observerFieldWrapper = $this->getMock('\EntityMetadataWrapper', array('info'));
      $observerFieldWrapper->expects($this->once())
        ->method('info')
        ->willReturn(array('type' => 'unknown'));

      $observerWrapper = $this->getMock('\EntityDrupalWrapper');
      $observerWrapper->{$field} = $observerFieldWrapper;

      $observerHandler = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler');
      $observerHandler->expects($this->once())
        ->method('watchdog')
        ->with($this->equalTo('entity xliff'), $this->anything(), $this->anything(), $this->equalTo(DrupalHandler::WATCHDOG_WARNING));

      $observerMediator = $this->getMock('EntityXliff\Drupal\Mediator\FieldMediator');
      $observerMediator->expects($this->once())
        ->method('getInstance')
        ->with($this->equalTo($observerFieldWrapper))
        ->willReturn(FALSE);

      $translatable = $this->getTranslatableOrNotInstance(TRUE, $mockEntity, $observerHandler, NULL, $observerMediator);
      $actualFieldValue = $translatable->getFieldFromEntity($observerWrapper, $field);
      $this->assertSame($expectedFieldValue, $actualFieldValue);
    }

    /**
     * Ensures that EntityTranslatableBase::getFieldFromEntity() will return the
     * value from the field handler instance if given one.
     *
     * @test
     */
    public function getFieldFromEntityBaseCase() {
      $mockEntity = $this->getMockWrapper();
      $field = 'title';
      $willedFieldValue1 = 'text';
      $expectedFieldValue[0] = array('#label' => 'label1', '#text' => 'text1');
      $expectedFieldValue[1] = array(
        '#label' => 'label2',
        '#text' => $willedFieldValue1
      );

      $observerFieldWrapper = $this->getMock('\EntityMetadataWrapper', array('info'));
      $observerFieldWrapper->expects($this->exactly(2))
        ->method('info')
        ->willReturn(array(
          'type' => 'known',
          'label' => $expectedFieldValue[1]['#label']
        ));

      $observerWrapper = $this->getMock('\EntityDrupalWrapper');
      $observerWrapper->{$field} = $observerFieldWrapper;

      $observerFieldHandler = $this->getMock('EntityXliff\Drupal\Interfaces\FieldHandlerInterface');
      $observerFieldHandler->expects($this->exactly(2))
        ->method('getValue')
        ->with($observerFieldWrapper)
        ->willReturnOnConsecutiveCalls($expectedFieldValue[0], $willedFieldValue1);

      $observerMediator = $this->getMock('EntityXliff\Drupal\Mediator\FieldMediator');
      $observerMediator->expects($this->exactly(2))
        ->method('getInstance')
        ->with($this->equalTo($observerFieldWrapper))
        ->willReturn($observerFieldHandler);

      $translatable = $this->getTranslatableOrNotInstance(TRUE, $mockEntity, NULL, NULL, $observerMediator);
      $actualFieldValue = $translatable->getFieldFromEntity($observerWrapper, $field);
      $this->assertSame($expectedFieldValue[0], $actualFieldValue);

      $actualFieldValue = $translatable->getFieldFromEntity($observerWrapper, $field);
      $this->assertSame($expectedFieldValue[1], $actualFieldValue);
    }

    /**
     * Ensures that EntityTranslatableBase::getFieldFromEntity() will return the
     * full translatable data of the referenced entity, or an empty array if the
     * referenced entity has no translatable data.
     *
     * @test
     */
    public function getFieldFromEntityReference() {
      $mockEntity = $this->getMockWrapper();
      $field = 'entity_reference';
      $expectedFieldValue = array('#text' => 'text', '#label' => 'label');

      $observerFieldWrapper = $this->getMock('\EntityDrupalWrapper', array(
        'info',
        'getIdentifier'
      ));
      $observerFieldWrapper->expects($this->exactly(2))
        ->method('info')
        ->willReturn(array('type' => 'node'));
      $observerFieldWrapper->expects($this->exactly(2))
        ->method('getIdentifier')
        ->willReturnOnConsecutiveCalls(1, 0);

      $observerTranslatable = $this->getMock('EntityXliff\Drupal\Interfaces\EntityTranslatableInterface');
      $observerTranslatable->expects($this->once())
        ->method('getData')
        ->willReturn($expectedFieldValue);

      $observerTranslatableFactory = $this->getMockFactory();
      $observerTranslatableFactory->expects($this->once())
        ->method('getTranslatable')
        ->with($observerFieldWrapper)
        ->willReturn($observerTranslatable);

      $observerWrapper = $this->getMock('\EntityDrupalWrapper');
      $observerWrapper->{$field} = $observerFieldWrapper;

      $observerHandler = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler');
      $observerHandler->expects($this->once())
        ->method('entityGetInfo')
        ->willReturn(array('node' => array('node' => 'definition')));

      $observerMediator = $this->getMock('EntityXliff\Drupal\Mediator\FieldMediator');
      $observerMediator->expects($this->any())
        ->method('getInstance')
        ->with($this->equalTo($observerFieldWrapper))
        ->willReturn(FALSE);

      $translatable = $this->getTranslatableOrNotInstance(TRUE, $mockEntity, $observerHandler, $observerTranslatableFactory, $observerMediator);
      $actualFieldValue = $translatable->getFieldFromEntity($observerWrapper, $field);
      $this->assertSame($expectedFieldValue, $actualFieldValue);

      $actualFieldValue = $translatable->getFieldFromEntity($observerWrapper, $field);
      $this->assertSame(array(), $actualFieldValue);
    }

    /**
     * Ensures that EntityTranslatableBase::getFieldFromEntity() will call itself
     * recursively if the field values attempting to be loaded are in a list.
     *
     * @test
     */
    public function getFieldFromEntityFieldTypeList() {
      $mockEntity = $this->getMockWrapper();
      $field = 'bullet_points';
      $expectedFieldValue = array('#text' => 'text', '#label' => 'label');

      $observerFieldWrapper = $this->getMock('\EntityMetadataWrapper', array(
        'info',
        'getIterator'
      ));
      $observerFieldWrapper->expects($this->exactly(2))
        ->method('info')
        ->willReturn(array('type' => 'list<text>'));
      $observerFieldWrapper->expects($this->once())
        ->method('getIterator')
        ->willReturn(array($this->getMockWrapper()));

      $observerWrapper = new EntityDrupalWrapperMagicGetMock($observerFieldWrapper);

      $observerFieldHandler = $this->getMock('EntityXliff\Drupal\Interfaces\FieldHandlerInterface');
      $observerFieldHandler->expects($this->once())
        ->method('getValue')
        ->with($observerFieldWrapper)
        ->willReturn($expectedFieldValue);

      $observerMediator = $this->getMock('EntityXliff\Drupal\Mediator\FieldMediator');
      $observerMediator->expects($this->exactly(2))
        ->method('getInstance')
        ->with($this->equalTo($observerFieldWrapper))
        ->willReturnOnConsecutiveCalls(FALSE, $observerFieldHandler);

      $translatable = $this->getTranslatableOrNotInstance(TRUE, $mockEntity, NULL, NULL, $observerMediator);
      $actualFieldValue = $translatable->getFieldFromEntity($observerWrapper, $field);
      $this->assertSame(array($expectedFieldValue), $actualFieldValue);
    }

    /**
     * Ensures that EntityTranslatableBase::entitySetNestedValue() returns FALSE
     * in the case that the given field is not known to be translated.
     *
     * @test
     */
    public function entitySetNestedValueBaseCaseNoFieldHandler() {
      $givenField = 'title';
      $givenParents = array($givenField);
      $givenValue = 'Value';
      $givenTargetLang = 'de';

      $mockEntity = $this->getMockWrapper();

      $observerWrapper = $this->getMock('\EntityDrupalWrapper');
      $observerWrapper->{$givenField} = $this->getMock('\EntityMetadataWrapper');

      $observerMediator = $this->getMockMediator();
      $observerMediator->expects($this->once())
        ->method('getInstance')
        ->with($this->equalTo($observerWrapper->{$givenField}))
        ->willReturn(FALSE);

      $translatable = new EntityTranslatableMockForEntitySetNestedValue($mockEntity, NULL, NULL, $observerMediator);
      $this->assertFalse($translatable->entitySetNestedValue($observerWrapper, $givenParents, $givenValue, $givenTargetLang));
    }

    /**
     * Ensures that EntityTranslatableBase::entitySetNestedValue() returns TRUE
     * and sets the nested value as expected in the base case.
     *
     * @test
     */
    public function entitySetNestedValueBaseCaseWithFieldHandler() {
      $givenField = 'title';
      $givenParents = array($givenField);
      $givenValue = 'Value';
      $givenTargetLang = 'de';
      $expectedIdentifier = 123;
      $expectedType = 'node';
      $expectedKey = $expectedType . ':' . $expectedIdentifier;

      $mockEntity = $this->getMockWrapper();

      $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('getIdentifier', 'type'));
      $observerWrapper->expects($this->atLeast(2))
        ->method('getIdentifier')
        ->willReturnOnConsecutiveCalls(FALSE, $expectedIdentifier);
      $observerWrapper->expects($this->any())
        ->method('type')
        ->willReturn($expectedType);
      $observerWrapper->{$givenField} = $this->getMock('\EntityMetadataWrapper');

      $observerHandler = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler', array('alter'));
      $observerHandler->expects($this->once())
        ->method('alter')
        ->with($this->equalTo('entity_xliff_presave'), $this->equalTo($observerWrapper), $this->equalto($expectedType));

      $observerFieldHandler = $this->getMock('EntityXliff\Drupal\Interfaces\FieldHandlerInterface');
      $observerFieldHandler->expects($this->once())
        ->method('setValue')
        ->with($this->equalTo($observerWrapper->{$givenField}), $this->equalTo($givenValue));

      $observerMediator = $this->getMockMediator();
      $observerMediator->expects($this->once())
        ->method('getInstance')
        ->with($this->equalTo($observerWrapper->{$givenField}))
        ->willReturn($observerFieldHandler);

      $observerTranslatable = $this->getMock('EntityXliff\Drupal\Interfaces\EntityTranslatableInterface');
      $observerTranslatable->expects($this->once())
        ->method('saveWrapper')
        ->with($this->equalTo($observerWrapper), $givenTargetLang);

      $observerFactory = $this->getMockFactory();
      $observerFactory->expects($this->once())
        ->method('getTranslatable')
        ->with($this->equalTo($observerWrapper))
        ->willReturn($observerTranslatable);

      $translatable = new EntityTranslatableMockForEntitySetNestedValue($mockEntity, $observerHandler, $observerFactory, $observerMediator);

      $this->assertTrue($translatable->entitySetNestedValue($observerWrapper, $givenParents, $givenValue, $givenTargetLang));
      $needsSave = $translatable->getEntitiesNeedSave();
      $this->assertSame($observerWrapper, $needsSave[$expectedKey]);
    }

    /**
     * Ensures that EntityTranslatableBase::entitySetNestedValue() recursively
     * calls itself with the expected arguments when necessary.
     *
     * @test
     */
    public function entitySetNestedValueRecursiveCase() {
      $givenField = 'title';
      $givenBaseField = 'base';
      $givenParents = array($givenBaseField, $givenField);
      $givenValue = 'Value';
      $givenTargetLang = 'de';
      $expectedIdentifier = 123;
      $expectedType = 'node';
      $expectedKey = $expectedType . ':' . $expectedIdentifier;

      $mockEntity = $this->getMockWrapper();

      $observerWrapper = $this->getMock('\EntityMetadataWrapper');
      $observerWrapper->{$givenBaseField} = $this->getMock('\EntityDrupalWrapper', array('getIdentifier', 'type', 'set'));
      $observerWrapper->{$givenBaseField}->expects($this->exactly(4))
        ->method('getIdentifier')
        ->willReturnOnConsecutiveCalls(FALSE, $expectedIdentifier, $expectedIdentifier, $expectedIdentifier);
      $observerWrapper->{$givenBaseField}->expects($this->any())
        ->method('type')
        ->willReturn($expectedType);
      $observerWrapper->{$givenBaseField}->expects($this->once())
        ->method('set')
        ->with($this->equalTo($expectedIdentifier));

      $observerHandler = $this->getMock('EntityXliff\Drupal\Utils\DrupalHandler', array('alter'));
      $observerHandler->expects($this->once())
        ->method('alter')
        ->with($this->equalTo('entity_xliff_presave'), $this->equalTo($observerWrapper->{$givenBaseField}), $this->equalto($expectedType));

      $observerTranslatable = $this->getMock('EntityXliff\Drupal\Interfaces\EntityTranslatableInterface');
      $observerTranslatable->expects($this->once())
        ->method('saveWrapper')
        ->with($this->equalTo($observerWrapper->{$givenBaseField}), $givenTargetLang);
      $observerTranslatable->expects($this->once())
        ->method('getTargetEntity')
        ->with($this->equalTo($givenTargetLang))
        ->willReturn($observerWrapper->{$givenBaseField});

      $observerFactory = $this->getMockFactory();
      $observerFactory->expects($this->atLeastOnce())
        ->method('getTranslatable')
        ->with($this->equalTo($observerWrapper->{$givenBaseField}))
        ->willReturn($observerTranslatable);

      $translatable = new EntityTranslatableMockForEntitySetNestedValue($mockEntity, $observerHandler, $observerFactory);
      $translatable->setEntitiesNeedSave(array($expectedKey => $observerWrapper->{$givenBaseField}));

      $translatable->entitySetNestedValue($observerWrapper, $givenParents, $givenValue, $givenTargetLang);
      $needsSave = $translatable->getEntitiesNeedSave();
      $this->assertSame($observerWrapper->{$givenBaseField}, $needsSave[$expectedKey]);
    }

    /**
     * Ensures that EntityTranslatableBase::getParent() returns FALSE if the given
     * metadata wrapper has no parent info.
     *
     * @test
     */
    public function getParentNoParentInfo() {
      $mockEntity = $this->getMockWrapper();

      $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('info'));
      $observerWrapper->expects($this->once())
        ->method('info')
        ->willReturn(array());

      $translatable = $this->getTranslatableOrNotInstance(TRUE, $mockEntity);
      $this->assertSame(FALSE, $translatable->getParent($observerWrapper));
    }

    /**
     * Ensures that EntityTranslatableBase::getParent() returns the provided
     * entity in the parent info on the given entity.
     *
     * @test
     */
    public function getParentInfoHasDrupalWrapper() {
      $mockEntity = $this->getMockWrapper();

      $mockParentWrapper = $this->getMock('\EntityDrupalWrapper');
      $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('info'));
      $observerWrapper->expects($this->once())
        ->method('info')
        ->willReturn(array('parent' => $mockParentWrapper));

      $translatable = $this->getTranslatableOrNotInstance(TRUE, $mockEntity);
      $this->assertSame($mockParentWrapper, $translatable->getParent($observerWrapper));
    }

    /**
     * Ensures that EntityTranslatableBase::getParent() recursively iterates on
     * itself until it find's its parent's parent.
     *
     * @test
     */
    public function getParentParent() {
      $mockEntity = $this->getMockWrapper();

      $mockGrandparentWrapper = $this->getMock('\EntityDrupalWrapper');
      $observerParentWrapper = $this->getMock('\EntityMetadataWrapper', array('info'));
      $observerParentWrapper->expects($this->once())
        ->method('info')
        ->willReturn(array('parent' => $mockGrandparentWrapper));
      $observerWrapper = $this->getMock('\EntityMetadataWrapper', array('info'));
      $observerWrapper->expects($this->once())
        ->method('info')
        ->willReturn(array('parent' => $observerParentWrapper));

      $translatable = $this->getTranslatableOrNotInstance(TRUE, $mockEntity);
      $this->assertSame($mockGrandparentWrapper, $translatable->getParent($observerWrapper));
    }

    /**
     * Tests that EntityTranslatableBase::getRawEntity() returns the raw entity in
     * the simplest case.
     *
     * @test
     */
    public function getRawEntityIsObject() {
      $expectedRaw = (object) array('expected' => 'value');

      $mockEntity = $this->getMockWrapper();

      $observerWrapper = $this->getMock('\EntityDrupalWrapper', array('raw'));
      $observerWrapper->expects($this->once())
        ->method('raw')
        ->willReturn($expectedRaw);

      $translatable = $this->getTranslatableOrNotInstance(TRUE, $mockEntity);
      $this->assertEquals($expectedRaw, $translatable->getRawEntity($observerWrapper));
    }

    /**
     * Ensures that EntityTranslatableBase::getRawEntity() returns the raw entity
     * in the event that the underlying raw value is not the entity itself.
     *
     * @test
     */
    public function getRawEntityIsNotObject() {
      $expectedRawInitial = 123;
      $expectedRaw = (object) array('expected' => 'value');
      $expectedRawType = 'node';

      $mockEntity = $this->getMockWrapper();

      $observerDrupal = $this->getMockHandler();
      $observerDrupal->expects($this->once())
        ->method('entityLoad')
        ->with($this->equalTo($expectedRawType), $this->equalTo(array($expectedRawInitial)))
        ->willReturn(array(0 => $expectedRaw));

      $observerWrapper = $this->getMock('\EntityDrupalWrapper', array(
        'raw',
        'type'
      ));
      $observerWrapper->expects($this->once())
        ->method('raw')
        ->willReturn($expectedRawInitial);
      $observerWrapper->expects($this->once())
        ->method('type')
        ->willReturn($expectedRawType);

      $translatable = $this->getTranslatableOrNotInstance(TRUE, $mockEntity, $observerDrupal);
      $this->assertEquals($expectedRaw, $translatable->getRawEntity($observerWrapper));
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

    /**
     * Returns an instance of EntityTranslatableBase with the isTranslatable
     * method stubbed out to return the provided $isTranslatable value.
     *
     * @param bool $isTranslatable
     *   Whether or not the translatable instance should return as translatable.
     *
     * @return \EntityXliff\Drupal\Interfaces\EntityTranslatableInterface
     */
    protected function getTranslatableOrNotInstance($isTranslatable, $wrapper, $handler = NULL, $factory = NULL, $mediator = NULL) {
      $handler = $handler ?: $this->getMockHandler();
      $factory = $factory ?: $this->getMockFactory();
      $mediator = $mediator ?: $this->getMockMediator();

      $translatable = $this->getMockBuilder('EntityXliff\Drupal\Tests\Translatable\EntityTranslatableBaseInstance')
        ->setConstructorArgs(array($wrapper, $handler, $factory, $mediator))
        ->setMethods(array('isTranslatable'))
        ->getMock();

      $translatable->expects($this->any())
        ->method('isTranslatable')
        ->willReturn($isTranslatable);

      return $translatable;
    }

  }

  /**
   * Class EntityTranslatableBaseInstance
   * @package EntityXliff\Drupal\Tests\Translatable
   *
   * Concrete, testable instance extending EntityTranslatableBase.
   */
  class EntityTranslatableBaseInstance extends EntityTranslatableBase {

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

    public function getTargetEntity($targetLanguage) {}

    public function isTranslatable() {}

    public function initializeTranslation() {}

  }

  class EntityDrupalWrapperMagicGetMock extends \EntityDrupalWrapper {

    protected $count = 0;
    protected $fieldHandler = NULL;

    public function __construct($fieldHandler) {
      $this->fieldHandler = $fieldHandler;
    }

    public function __get($property) {
      $this->count++;

      if ($this->count === 1) {
        return $this->fieldHandler;
      }
      elseif ($this->count === 2) {
        return array($this->fieldHandler);
      }
      else {
        return parent::__get($property);
      }
    }

  }

  /**
   * Class EntityTranslatableMockForGetData
   * @package EntityXliff\Drupal\Tests\Translatable
   * @see EntityTranslatableBaseTest::getData()
   */
  class EntityTranslatableMockForGetData extends EntityTranslatableBaseInstance {

    public $gotFieldFromEntity = NULL;

    public $translatableFieldData = array(
      'title' => 'Title Value',
      'other_field' => 'Other Field Value',
    );

    public function getTranslatableFields() {
      return array_keys($this->translatableFieldData);
    }

    public function getFieldFromEntity(\EntityDrupalWrapper $wrapper, $field, $delta = NULL) {
      $this->gotFieldFromEntity = $wrapper;
      return $this->translatableFieldData[$field];
    }

  }

  /**
   * Class EntityTranslatableMockForSetData
   * @package EntityXliff\Drupal\Tests\Translatable
   * @see EntityTranslatableBaseTest::setDataDoNotSave()
   * @see EntityTranslatableBaseTest::setDataAndSave()
   */
  class EntityTranslatableMockForSetData extends EntityTranslatableBaseInstance {

    public $initialized = FALSE;
    public $translationData = array();
    public $translationKey = array();
    public $targetLanguage = '';

    public function addTranslatedDataRecursive($translation, array $key = array(), $targetLang) {
      $this->translationData = $translation;
      $this->translationKey = $key;
      $this->targetLanguage = $targetLang;
    }

    public function initializeTranslation() {
      $this->initialized = TRUE;
    }

    public function setEntitiesNeedSave(array $entities) {
      $this->entitiesNeedSave = $entities;
    }

  }

  /**
   * Class EntityTranslatableMockForAddTranslatedData
   * @package EntityXliff\Drupal\Tests\Translatable
   * @see EntityTranslatableBaseTest::addTranslatedDataBaseCase()
   * @see EntityTranslatableBaseTest::addTranslatedDataRecursiveCase()
   */
  class EntityTranslatableMockForAddTranslatedData extends EntityTranslatableBaseInstance {

    public $gotKey;
    public $gotValues;
    public $gotTargetLang;

    public function addTranslatedDataRecursive($translation, array $key = array(), $targetLang) {
      parent::addTranslatedDataRecursive($translation, $key, $targetLang);
    }

    public function setItem($key, $values = array(), $targetLang) {
      $this->gotKey = $key;
      $this->gotValues = $values;
      $this->gotTargetLang = $targetLang;
    }

  }

  /**
   * Class EntityTranslatableMockForSetItem
   * @package EntityXliff\Drupal\Tests\Translatable
   * @see EntityTranslatableBaseTest::setItemBaseCase()
   * @see EntityTranslatableBaseTest::setItemRecursiveCase()
   */
  class EntityTranslatableMockForSetItem extends EntityTranslatableBaseInstance {

    protected $targetEntity;
    public $gotTargetEntity;
    public $gotParents;
    public $gotValue;
    public $gotTargetLang;

    public function getTargetEntity($targetLang) {
      return $this->targetEntity;
    }

    protected function entitySetNestedValue(\EntityMetadataWrapper $wrapper, array $parents, $value, $targetLang) {
      $this->gotTargetEntity = $wrapper;
      $this->gotParents = $parents;
      $this->gotValue = $value;
      $this->gotTargetLang = $targetLang;
    }

    public function setTargetEntity(\EntityDrupalWrapper $wrapper) {
      $this->targetEntity = $wrapper;
    }

  }

  /**
   * Class EntityTranslatableMockForEntitySetNestedValue
   * @package EntityXliff\Drupal\Tests\Translatable
   * @see EntityTranslatableBaseTest::entitySetNestedValueBaseCaseNoFieldHandler()
   * @see EntityTranslatableBaseTest::entitySetNestedValueBaseCaseWithFieldHandler()
   * @see EntityTranslatableBaseTest::entitySetNestedValueRecursiveCase()
   */
  class EntityTranslatableMockForEntitySetNestedValue extends EntityTranslatableBaseInstance {

    protected $called = 0;

    public function entitySetNestedValue(\EntityMetadataWrapper $wrapper, array $parents, $value, $targetLang) {
      $this->called++;
      if ($this->called === 1) {
        return parent::entitySetNestedValue($wrapper, $parents, $value, $targetLang);
      }
      // Recursive case, we don't care to go a layer deeper.
      else {
        return TRUE;
      }
    }

    public function getEntitiesNeedSave() {
      return $this->entitiesNeedSave;
    }

    public function setEntitiesNeedSave($needSave) {
      $this->entitiesNeedSave = $needSave;
    }

  }

}

// Mostly necessary for full code coverage of the constructor.
// @see EntityTranslatableBaseTest::constructor()
namespace {
  if (!function_exists('entity_xliff_get_field_handlers')) {
    function entity_xliff_get_field_handlers() {
      return array();
    }
    function entity_get_info() {
      return array();
    }
  }
}

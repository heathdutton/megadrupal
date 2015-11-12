<?php

class FractionEntityTest extends Entity {
  function __construct($values = array()) {
    parent::__construct($values, 'fraction_entity_test');
  }
}
class FractionEntityTestController extends EntityAPIController {}
class FractionEntityTestMetadataController extends EntityDefaultMetadataController {}

class FractionEntityTestType extends Entity {
  function __construct($values = array()) {
    parent::__construct($values, 'fraction_entity_test_type');
  }
}
class FractionEntityTestTypeUIController extends EntityDefaultUIController {}

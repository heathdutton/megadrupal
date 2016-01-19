<?php

namespace Drupal\campaignion\CRM\Import\Field;
require_once dirname(__FILE__) . '/RedhenEntityTest.php';

use \Drupal\campaignion\CRM\Import\Source\ArraySource;

class FieldTest extends RedhenEntityTest {
  function testSimpleStringNoErrors() {
    $field = 'first_name'; $string = 'Somename';
    $importer = new Field($field);
    $data[$field] = $string;
    $entity = $this->newRedhenContact();
    $importer->import(new ArraySource($data), $entity, TRUE);
    $this->assertEqual($string, $entity->first_name->value());
  }

  function testCallPreprocess(){
    $field = 'first_name'; $string = 'Somename';
    $importer = $this->getMock(__NAMESPACE__ . '\\Field', array('preprocessField'), array($field));
    $data[$field] = $string;
    $entity = $this->newRedhenContact();
    $importer->expects($this->once())->method('preprocessField')->with($this->identicalTo($data[$field]));
    $importer->import(new ArraySource($data), $entity, TRUE);
  }

  function testNotExistingFieldIsIgnored() {
    $field = 'nofield_with_this_name'; $string = 'Somename';
    $importer = new Field($field);
    $data[$field] = $string;
    $entity = $this->newRedhenContact();
    $importer->import(new ArraySource($data), $entity, TRUE);
  }

  /**
   * @expectedException PHPUnit_Framework_Error
   */
  function testNonEntityGivesPHPError() {
    $field = 'first_name'; $string = 'Somename';
    $importer = new Field($field);
    $data[$field] = $string;
    $entity = NULL;
    $this->assertEqual(NULL, $importer->import(new ArraySource($data), $entity, TRUE));
  }
}
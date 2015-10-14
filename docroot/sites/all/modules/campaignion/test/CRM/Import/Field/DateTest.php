<?php

namespace Drupal\campaignion\CRM\Import\Field;
require_once dirname(__FILE__) . '/RedhenEntityTest.php';

use \Drupal\campaignion\CRM\Import\Source\ArraySource;

class DateTest extends RedhenEntityTest {
  function testValidBirthDate() {
    $field = 'field_date_of_birth'; $string = '1988-10-22';
    $importer = new Date($field);
    $data[$field] = $string;
    $entity = $this->newRedhenContact();
    $importer->import(new ArraySource($data), $entity, TRUE);
    $this->assertEqual($string, strftime('%Y-%m-%d', (int) $entity->{$field}->value()));
  }
  
  function testInValidBirthDate_IsSetAsNull() {
    $field = 'field_date_of_birth'; $string = '1988;55yz10-22';
    $importer = new Date($field);
    $data[$field] = $string;
    $entity = $this->newRedhenContact();
    $importer->import(new ArraySource($data), $entity, TRUE);
    $this->assertNull($entity->{$field}->value());
  }
}

<?php

namespace Drupal\commerce_entity_plus\Tests;

class CustomerProfileTest extends CommerceEntityTestBase {

  public static function getInfo() {
    return array(
      'name' => 'Customer profiles',
      'description' => 'Test the enhanced Customer Profile entity class.',
      'group' => self::TEST_GROUP,
    );
  }

  function testEntityCreate() {
    $entity1 = entity_create('commerce_customer_profile', array(
      'type' => 'billing',
      'uid' => 0,
    ));
    $this->assertTrue($entity1 instanceof \CommerceCustomerProfile,
      'Created Profile entity has enhanced entity class');

    $entity2 = commerce_customer_profile_new('billing');
    $this->assertTrue($entity2 instanceof \CommerceCustomerProfile,
      'Created Profile entity has enhanced entity class');
  }
}

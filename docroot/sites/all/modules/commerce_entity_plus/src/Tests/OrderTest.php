<?php

namespace Drupal\commerce_entity_plus\Tests;

class OrderTest extends CommerceEntityTestBase {

  public static function getInfo() {
    return array(
      'name' => 'Order test',
      'description' => 'Test the enhanced Order entity class.',
      'group' => self::TEST_GROUP,
    );
  }

  function testEntityCreate() {
    $entity1 = entity_create('commerce_order', array());
    $this->assertTrue($entity1 instanceof \CommerceOrder,
      'Order entity has enhanced entity class');

    $entity2 = commerce_order_new();
    $this->assertTrue($entity2 instanceof \CommerceOrder,
      'Order entity has enhanced entity class');
  }
}

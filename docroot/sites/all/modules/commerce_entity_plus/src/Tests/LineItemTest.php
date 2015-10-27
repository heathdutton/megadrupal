<?php

namespace Drupal\commerce_entity_plus\Tests;

class LineItemTest extends CommerceEntityTestBase {

  public static function getInfo() {
    return array(
      'name' => 'Line items',
      'description' => 'Test the enhanced Line Item entity class.',
      'group' => self::TEST_GROUP,
    );
  }

  function testEntityCreate() {
    $entity1 = entity_create('commerce_line_item', array(
      'type' => 'product',
      'order_id' => 0,
    ));
    $this->assertTrue($entity1 instanceof \CommerceLineItem,
      'Line Item entity has enhanced entity class');

    $entity2 = commerce_line_item_new('product');
    $this->assertTrue($entity2 instanceof \CommerceLineItem,
      'Line Item entity has enhanced entity class');
  }
}

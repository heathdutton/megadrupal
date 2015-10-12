<?php

namespace Drupal\commerce_entity_plus\Tests;

class ProductTest extends CommerceEntityTestBase {

  public static function getInfo() {
    return array(
      'name' => 'Product test',
      'description' => 'Test the enhanced Product entity class.',
      'group' => self::TEST_GROUP,
    );
  }

  function testEntityCreate() {
    $entity1 = entity_create('commerce_product', array(
      'type' => 'product',
    ));
    $this->assertTrue($entity1 instanceof \CommerceProduct,
      'Product entity has enhanced entity class');

    $entity2 = commerce_product_new('product');
    $this->assertTrue($entity2 instanceof \CommerceProduct,
      'Product entity has enhanced entity class');
  }
}

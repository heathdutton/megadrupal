<?php

namespace Drupal\doctrine\Tests\Mapping;

use Drupal\doctrine\Mapping\SchemaDriver;
use Doctrine\ORM\Mapping\ClassMetadataInfo;
/**
 * Tests OneToOne relationships.
 */
class OneToOneTest extends MappingBaseTest {

  public static function getInfo() {
    return array(
      'name' => 'OneToOne',
      'description' => 'Unit Tests OneToOne Relationship.',
      'group' => 'Doctrine',
    );
  }

  function testUnidirectional() {
    $this->createEntity('shipping', 'Shipping')
         ->setPrimaryKey('id');

    $this->createEntity('product', 'Product')
         ->setPrimaryKey('id')
         ->setForeignKey('shipping_id', 'shipping', 'id')
         ->setUnique('shipping_id');

    $metadata = new ClassMetadataInfo('Product');

    $driver = new SchemaDriver($this->schema, $this->entityInfo);
    $driver->loadMetadataForClass('Product', $metadata);

    $association = $metadata->getAssociationMapping('shipping');
    $this->assertEqual('Shipping', $association['targetEntity']);
    $this->assertEqual('id', $association['joinColumns'][0]['referencedColumnName']);
    $this->assertEqual(TRUE, $association['joinColumns'][0]['unique']);
    $this->assertNull($association['mappedBy']);
  }

  function testBidirectional() {
    $this->createEntity('customer', 'Customer')
         ->setPrimaryKey('id');

    $this->createEntity('cart', 'Cart')
         ->setPrimaryKey('id')
         ->setForeignKey('customer_id', 'customer', 'id');

    $driver = new SchemaDriver($this->schema, $this->entityInfo);

    $metadata = new ClassMetadataInfo('Cart');
    $driver->loadMetadataForClass('Cart', $metadata);

    $association = $metadata->getAssociationMapping('customer');
    $this->assertEqual('Customer', $association['targetEntity']);
    $this->assertEqual('id', $association['joinColumns'][0]['referencedColumnName']);
//     $this->assertEqual('customer', $association['mappedBy']);

    $metadata = new ClassMetadataInfo('Customer');
    $driver->loadMetadataForClass('Customer', $metadata);
    $debug = true;
  }

}

<?php

namespace Drupal\commerce_entity_plus\Tests;

class PaymentTransactionTest extends CommerceEntityTestBase {

  public static function getInfo() {
    return array(
      'name' => 'Payment transaction test',
      'description' => 'Test the enhanced Payment Transaction entity class.',
      'group' => self::TEST_GROUP,
    );
  }

  function testEntityCreate() {
    $entity1 = entity_create('commerce_payment_transaction', array(
      'payment_method' => 'commerce_payment_example',
    ));
    $this->assertTrue($entity1 instanceof \CommercePaymentTransaction,
      'Payment Transaction entity has enhanced entity class');

    $entity2 = commerce_payment_transaction_new('commerce_payment_example');
    $this->assertTrue($entity2 instanceof \CommercePaymentTransaction,
      'Payment Transaction entity has enhanced entity class');
  }
}

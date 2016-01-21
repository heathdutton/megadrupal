<?php

namespace Drupal\commerce_entity_plus\Tests;

abstract class CommerceEntityTestBase extends \DrupalWebTestCase {
  const TEST_GROUP = 'Commerce Entity+';

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    $modules = array(
      'commerce',
      'commerce_product',
      'commerce_price',
      'commerce_customer',
      'commerce_line_item',
      'commerce_order',
      'commerce_product_reference',
      'commerce_payment',
//      'commerce_tax',
//      'commerce_product_pricing',
//      'commerce_ui',
      'commerce_checkout',
      'commerce_cart',
//      'commerce_line_item_ui',
//      'commerce_order_ui',
//      'commerce_product_ui',
//      'commerce_customer_ui',
//      'commerce_payment_ui',
//      'commerce_product_pricing_ui',
//      'commerce_tax_ui',
      'commerce_payment_example',
    );

    $modules[] = 'commerce_entity_plus';
    parent::setUp($modules);
  }


}

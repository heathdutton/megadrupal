<?php

namespace Drupal\commerce_reports\Tests;

class CommerceReportsProductTestCase extends CommerceReportsBaseTestCase {

  public static function getInfo() {
    return array(
      'name' => 'Product reports',
      'description' => 'Test the product reports.',
      'group' => 'Commerce Reports',
    );
  }

  function setUp() {
    $this->resetAll();
    parent::setUp();
  }

  /**
   * Tests creating a single order, containing a single product with a variable quantity.
   * Then verifies if the reporting is correct.
   */
  function testSingleProduct() {
    $this->createOrders();

    $this->_test();
  }

  /**
   * Tests creating a single order, containing multiple products with a variable quantity.
   * Then verifies if the reporting is correct.
   */
  function testMultipleProducts() {
    $this->createProducts(10);
    $this->createOrders();

    $this->_test();
  }

  /**
   * Tests creating a multiple orders, containing multiple products with a variable quantity.
   * Then verifies if the reporting is correct.
   */
  function testMultipleOrdersProducts() {
    $this->createProducts(5);
    $this->createOrders(5);

    $this->_test();
  }

  protected function _test() {
    $products = array();

    foreach ($this->orders as $order) {
      foreach ($order['products'] as $product_id => $quantity) {
        $sku = $this->products[$product_id]->sku;

        if (empty($products[$sku])) {
          $products[$sku] = array(
            'quantity' => 0,
            'revenue' => 0,
          );
        }

        $products[$sku]['quantity'] += $quantity;
        $products[$sku]['revenue'] += $quantity * $this->products[$product_id]->commerce_price[LANGUAGE_NONE][0]['amount'];
      }
    }

    $report = views_get_view_result('commerce_reports_products', 'default');

    $this->assertEqual(count($report), min(count($products), 10), t('The amount of products (%reported) that is reported (%generated) upon is correct.', array('%reported' => count($report), '%generated' => count($products))));

    foreach ($report as $line) {
      $sku = $line->commerce_product_field_data_commerce_product_sku;
      $quantity = $line->commerce_line_item_quantity;
      $revenue = $line->field_data_commerce_total_commerce_total_amount;

      $this->assertFalse(empty($products[$sku]), t('The product %sku that is reported upon exists.', array('%sku' => $sku)));

      if (!empty($products[$sku])) {
        $this->assertEqual($products[$sku]['quantity'], $quantity, t('The reported quantity %reported matches the generated quantity %generated.', array('%sku' => $sku, '%reported' => $quantity, '%generated' => $products[$sku]['quantity'])));
        $this->assertEqual($products[$sku]['revenue'], $revenue, t('The reported revenue %reported matches the generated revenue %generated.', array('%sku' => $sku, '%reported' => $revenue, '%generated' => $products[$sku]['revenue'])));
      }
    }
  }

}

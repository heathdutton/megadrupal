<?php

namespace Drupal\commerce_reports\Tests;

class CommerceReportsCustomerTestCase extends CommerceReportsBaseTestCase {

  public static function getInfo() {
    return array(
      'name' => 'Customer reports',
      'description' => 'Test the customer reports.',
      'group' => 'Commerce Reports',
    );
  }

  function setUp() {
    $this->resetAll();
    parent::setUp();
  }

  /**
   * Tests creating a single order for a single customer, containing a single product with a variable quantity.
   * Then verifies if the reporting is correct.
   */
  function testSingleCustomer() {
    $this->createOrders();

    $this->_test();
  }

  /**
   * Make one customer perform multiple orders with multiple products.
   * Then verifies if the reporting is correct.
   */
  function testSingleCustomerMultipleProducts() {
    $this->createProducts(5);
    $this->createOrders();

    $this->_test();
  }

  /**
   * Make one customer perform multiple orders with multiple products.
   * Then verifies if the reporting is correct.
   */
  function testMultipleCustomers() {
    $this->createCustomers(3);
    $this->createProducts(2);
    $this->createOrders(10);

    $this->_test();
  }

  protected function _test() {
    $customers = array();

    foreach ($this->orders as $order) {
      $uid = $order['commerce_order']->uid;

      if (empty($customers[$uid])) {
        $customers[$uid] = array(
          'orders' => 0,
          'products' => 0,
          'revenue' => 0,
        );
      }

      $customers[$uid]['orders']++;

      foreach ($order['products'] as $product_id => $quantity) {
        $customers[$uid]['products'] += $quantity;
        $customers[$uid]['revenue'] += $quantity * $this->products[$product_id]->commerce_price[LANGUAGE_NONE][0]['amount'];
      }
    }

    $report = views_get_view_result('commerce_reports_customers', 'page');

    $this->assertEqual(count($report), min(count($customers), 10), t('The amount of customers (%reported) that is reported (%generated) upon is correct.', array('%reported' => count($report), '%generated' => count($customers))));

    foreach ($report as $line) {
      $uid = $line->uid;

      $orders = $line->commerce_order_users_order_id;
      $revenue = $line->commerce_order_users__field_data_commerce_order_total_commer;
      $average = $line->commerce_order_users__field_data_commerce_order_total_commer_2;

      $this->assertFalse(empty($customers[$uid]), t('The customer %uid that is reported upon exists.', array('%uid' => $uid)));

      if (!empty($customers[$uid])) {
        $this->assertEqual($customers[$uid]['orders'], $orders, t('The reported amount of orders %reported matches the generated amount of orders %generated.', array('%reported' => $orders, '%generated' => $customers[$uid]['orders'])));
        // $this->assertEqual($customers[$uid]['products'], $products, t('The reported amount of line items %reported matches the generated amount of line items %generated.', array('%reported' => $products, '%generated' => $customers[$uid]['products'])));
        $this->assertEqual($customers[$uid]['revenue'], $revenue, t('The reported revenue %reported matches the generated revenue %generated.', array('%reported' => $revenue, '%generated' => $customers[$uid]['revenue'])));
      }
    }
  }

}

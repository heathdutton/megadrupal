<?php

namespace Drupal\commerce_reports\Tests;

class CommerceReportsSalesTestCase extends CommerceReportsBaseTestCase {

  public static function getInfo() {
    return array(
      'name' => 'Sales reports',
      'description' => 'Test the sales reports.',
      'group' => 'Commerce Reports',
    );
  }

  public function testNoOrders() {
    $this->_getDates();

    $this->_test();
  }

  public function testSingleOrder() {
    $this->createOrders(1, FALSE, $this->_getDates());

    $this->_test();
  }

  public function testMultipleOrders() {
    $this->createCustomers(5);
    $this->createOrders(20, FALSE, $this->_getDates());

    $this->_test();
  }

  protected function _getDates() {
    $report = views_get_view_result('commerce_reports_sales', 'page');
    $possibleDates = array();

    foreach ($report as $line) {
      $possibleDates[] = $this->getOrderCreated($line);
    }

    return $possibleDates;
  }

  protected function getOrderCreated($line) {
    return $line->_field_data['order_id_1']['entity']->created;
  }

  protected function _test() {
    $sales = array();
    foreach ($this->orders as $order) {
      $created = $order['commerce_order']->created;

      if (empty($sales[$created])) {
        $sales[$created] = array(
          'orders' => 0,
          'revenue' => 0,
        );
      }

      $sales[$created]['orders'] ++;

      foreach ($order['products'] as $product_id => $quantity) {
        $sales[$created]['revenue'] += $quantity * $this->products[$product_id]->commerce_price[LANGUAGE_NONE][0]['amount'];
      }
    }

    $report = views_get_view_result('commerce_reports_sales', 'page');

    foreach ($report as $line) {
      $created = $this->getOrderCreated($line);

      if (empty($sales[$created])) {
        $this->assertTrue(empty($line->order_id) && empty($line->commerce_order_total) && empty($line->commerce_order_total_1), t('There was no unintented activity.'));
      }
      else {
        $orders = $sales[$created]['orders'];
        $revenue = $sales[$created]['revenue'];
        $average = (int) floor($revenue / $orders);

        $this->assertEqual($line->order_id, $orders, t('The right amount of orders was reported.'));
        $this->assertEqual($line->field_data_commerce_order_total_commerce_order_total_amount, $revenue, t('The right amount of revenue was reported.'));
        $this->assertEqual((int) floor($line->field_data_commerce_order_total_commerce_order_total_amount_1), $average, t('The right average of revenue was reported.'));
      }
    }
  }

}

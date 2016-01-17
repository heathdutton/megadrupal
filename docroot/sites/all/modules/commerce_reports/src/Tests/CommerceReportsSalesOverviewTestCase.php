<?php

namespace Drupal\commerce_reports\Tests;

class CommerceReportsSalesOverviewTestCase extends CommerceReportsBaseTestCase {

  public static function getInfo() {
    return array(
      'name' => 'Sales overview reports',
      'description' => 'Test the sales overview reports.',
      'group' => 'Commerce Reports',
    );
  }

  protected function _test($data = array()) {
    foreach ($data as $display => $information) {
      $report = views_get_view_result('commerce_reports_sales_overview', $display);
      $line = reset($report);

      foreach ($information as &$metric) {
        if (empty($metric)) {
          $metric = NULL;
        }
      }

      if (empty($line->field_data_commerce_order_total_commerce_order_total_amount)) {
        $amount = 0;
      }
      else {
        $amount = $line->field_data_commerce_order_total_commerce_order_total_amount;
      }

      if (empty($line->order_id)) {
        $sales = NULL;
      }
      else {
        $sales = $line->order_id;
      }

      $this->assertEqual($information['count'], $sales, t('The expected amount of orders is equal to the derived amount of orders.'));
      $this->assertEqual($information['amount'], $amount, t('The expected total amount is equal to the derived total amount.'));
    }
  }

  public function testTodayOverview() {
    $this->createCustomers(2);
    $this->createOrders(10, TRUE, array(time()));

    $total = $this->getTotal();

    $testData = array(
      'today' => array(
        'count' => 10,
        'amount' => $total,
      ),
      'yesterday' => array(
        'count' => 0,
        'amount' => 0,
      ),
      'month' => array(
        'count' => 10,
        'amount' => $total,
      ),
    );

    $this->_test($testData);
  }

  public function testYesterdayOverview() {
    $this->createCustomers(2);
    $this->createOrders(10, TRUE, array(time() - 86400));

    $total = $this->getTotal();

    $testData = array(
      'today' => array(
        'count' => 0,
        'amount' => 0,
      ),
      'yesterday' => array(
        'count' => 10,
        'amount' => $total,
      ),
      'month' => array(
        'count' => 10,
        'amount' => $total,
      ),
    );

    $this->_test($testData);
  }

  public function testMonthlyOverview() {
    $this->createCustomers(2);
    $this->createOrders(10, TRUE, array(time() - 3 * 86400, time() - 5 * 86400, time() - 15 * 86400, time() - 20 * 86400, time() - 30 * 86400));

    $total = $this->getTotal();

    $testData = array(
      'today' => array(
        'count' => 0,
        'amount' => 0,
      ),
      'yesterday' => array(
        'count' => 0,
        'amount' => 0,
      ),
      'month' => array(
        'count' => 10,
        'amount' => $total,
      ),
    );

    $this->_test($testData);
  }

  public function testOutsideOverviewScope() {
    $this->createCustomers(2);
    $this->createOrders(10, TRUE, array(time() - 31 * 86400));

    $total = $this->getTotal();

    $testData = array(
      'today' => array(
        'count' => 0,
        'amount' => 0,
      ),
      'yesterday' => array(
        'count' => 0,
        'amount' => 0,
      ),
      'month' => array(
        'count' => 0,
        'amount' => 0,
      ),
    );

    $this->_test($testData);
  }

}

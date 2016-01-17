<?php

namespace Drupal\commerce_reports\Tests;

class CommerceReportsPaymentMethodTestCase extends CommerceReportsBaseTestCase {

  public static function getInfo() {
    return array(
      'name' => 'Payment method reports',
      'description' => 'Test the payment method reports.',
      'group' => 'Commerce Reports',
    );
  }

  public function testExampleMethod() {
    $this->createOrders(10, TRUE);

    $transactions = 0;
    $revenue = 0;

    foreach ($this->orders as $order) {
      $transactions ++;
      $revenue += $order['commerce_transaction']->amount;
    }

    $report = views_get_view_result('commerce_reports_payment_methods', 'default');

    $this->assertEqual(count($report), 1, t('Exactly one payment method was reported upon.'));

    foreach ($report as $line) {
      $this->assertEqual($line->commerce_payment_transaction_payment_method, 'commerce_payment_example', t('The example payment method was used for this transaction.'));

      $this->assertEqual($line->transaction_id, $transactions, t('The right amount of transactions were reported.'));
      $this->assertEqual($line->commerce_payment_transaction_amount, $revenue, t('The right amount of revenue was reported.'));
    }
  }

}

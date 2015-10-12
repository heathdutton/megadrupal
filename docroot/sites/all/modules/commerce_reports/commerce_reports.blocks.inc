<?php
/**
 * @file
 * Provides all statistics and other features that are not powered by Views.
 */

/**
 * Implementation of table showing sales overview.
 */
function commerce_reports_sales() {
  $sales = array();

  if ($timestamp = strtotime('first day of this month midnight')) {
    $passed_days = format_date(REQUEST_TIME, 'custom', 'j');
    $month_raw = commerce_reports_sales_data($timestamp, $passed_days);

    $month = commerce_reports_sales_format($month_raw, t('Daily average for @month', array('@month' => format_date($timestamp, 'custom', 'F o'))));
    $sales = array_merge($sales, $month);

    $total_days = format_date($timestamp, 'custom', 't');
    $projected_raw = array();
    foreach ($month_raw as $currency => $row) {
      $projected_raw[$currency] = array(
        'count' => round($row['count'] * $total_days, 2),
        'sum' => round( $row['sum'] * $total_days),
      );
    }

    $projected = commerce_reports_sales_format($projected_raw, t('Projected totals for @month', array('@month' => format_date($timestamp, 'custom', 'F o'))));
    $sales = array_merge($sales, $projected);
  }

  return array(
    '#theme' => 'table',
    '#rows' => $sales,
    '#header' => array(array('data' => t('Sales Data')), array('data' => t('Number of Orders')), array('data' => t('Revenue'))),
  );
}

/**
 * Helper function that retrieves sales data from a certain starting point.
 */
function commerce_reports_sales_data($start = 0, $time_periods = 1) {
  $sales = array();

  $result = db_query('SELECT SUM(amount) AS sum, AVG(amount) AS average, COUNT(*) AS count, currency_code
    FROM {commerce_payment_transaction} cpt
    INNER JOIN {commerce_order} co ON cpt.order_id = co.order_id
    WHERE cpt.created >= :start AND cpt.status = :status
    GROUP by cpt.currency_code', array(':status' => 'success', ':start' => $start));

  while ($row = $result->fetchAssoc()) {
    $currency = $row['currency_code'];
    unset($row['currency_code']);

    $sales[$currency] = $row;
    $sales[$currency]['count'] = round($sales[$currency]['count'] / $time_periods, 2);
    $sales[$currency]['sum'] = round($sales[$currency]['sum'] / $time_periods);
  }

  return $sales;
}

/**
 * Helper function that formats the data retrieved from commerce_reports_sales_data() function.
 */
function commerce_reports_sales_format($data, $title) {
  $sales = array();

  foreach ($data as $currency => $row) {
    $sales[] = array($title, $row['count'], commerce_currency_format($row['sum'], $currency));
  }

  if (empty($sales)) {
    $sales[] = array($title, 0, commerce_currency_format(0, commerce_default_currency()));
  }

  return $sales;
}

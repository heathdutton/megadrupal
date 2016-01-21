<?php

/**
 * @file
 * Marketplace report administration menu items.
 */

/**
 * Display the reports overview
 */
function mp_reports_overview($uid) {
  drupal_set_title(t('Sales Reports'));

  return '<dl><dt>' . l(t("Product Reports"), 'user/' . arg(1) . '/selling/reports/product') . '</dt><dd>' . t("View reports for your products.") . '</dd><dt>' . l(t("Sales Per Year"), 'user/' . arg(1) . '/selling/reports/sales') . '</dt><dd>' . t("View reports of your sales per year and month.") . '</dd><dt>' . l(t("Sales Summary"), 'user/' . arg(1) . '/selling/reports/summary') . '</dt><dd>' . t("View reports of your overall and projected sales.") . '</dd></dl>';
}

/**
 * Returns a table of seller order statuses.
 */
function mp_reports_statuses() {
  drupal_set_title(t('Seller Order Status Reports'));

  $header = array(
    array(
      'data' => t('Order ID'),
      'field' => 'sos.order_id',
    ),
    array(
      'data' => t('Seller ID'),
      'field' => 'sos.uid',
    ),
    array(
      'data' => t('Order Created'),
      'field' => 'uco.created',
      'sort' => 'desc',
    ),
  );

  $query = db_select('mp_seller_order_statuses', 'sos');
  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query->leftJoin('uc_orders', 'uco', 'uco.order_id = sos.order_id');
  $query->addField('sos', 'uid');
  $query->addField('sos', 'order_id');
  $query->addField('uco', 'created');
  $query->condition('sos.order_status', 'not complete');
  $count_query = db_select('mp_seller_order_statuses', 'sos');
  $count_query->addExpression('COUNT(*)');
  $count_query->condition('order_status', 'not complete');
  $query->limit(20)->setCountQuery($count_query);
  $query->orderByHeader($header);

  // Build a table based on the sellers' order statuses.
  $result = $query->execute();
  foreach ($result as $sellerorder) {
    $link2 = l($sellerorder->uid, 'user/' . $sellerorder->uid);
    $link = l($sellerorder->order_id, 'admin/store/orders/' . $sellerorder->order_id);

    $rows[] = array(
      array('data' => $link),
      array('data' => $link2),
      array('data' => format_date($sellerorder->created, 'custom', variable_get('uc_date_format_default', 'm/d/Y'))),
    );
  }

  $build = array();
  $build['table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header
  );
  $build['pager'] = array('#theme' => 'pager');
  return $build;
  //return theme('table', array('header' => $header, 'rows' => $rows)) . theme('pager', array('tags' => NULL, 'element' => 0));
}

/**
 * Displays the product reports
 */
function mp_reports_products($user) {
  $uid = $user->uid;
  drupal_set_title(t('Sales Reports'));
  $count_views = module_exists('statistics') && variable_get('statistics_count_content_views', FALSE);

  $page = isset($_GET['page']) ? intval($_GET['page']) : 0;
  $page_size = isset($_GET['nopage']) ? UC_REPORTS_MAX_RECORDS : variable_get('uc_reports_table_size', 30);
  $order_statuses = array('complete');
  $product_types = uc_product_types();
  $csv_rows = array();
  
  $query = db_select('node', 'n', array('fetch' => PDO::FETCH_ASSOC))
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->limit($page_size);
  $query->addField('n', 'nid');
  $query->addField('n', 'title');
  $query->addExpression('(SELECT SUM(p.qty) FROM {uc_order_products} AS p 
    LEFT JOIN {mp_seller_order_statuses} AS o ON p.order_id = o.order_id 
    WHERE o.order_status IN (:statuses)
    AND p.nid = n.nid 
    AND o.uid = :uid)',
    'sold',
    array(
      ':statuses' => $order_statuses,
      ':uid' => $uid
    )); 
  $query->addExpression('(SELECT SUM(p.cost * p.qty) FROM {uc_order_products} AS p 
    LEFT JOIN {mp_seller_order_statuses} AS o ON p.order_id = o.order_id 
    WHERE o.order_status IN (:statuses)
    AND p.nid = n.nid 
    AND o.uid = :uid)',
    'total',
    array(
      ':statuses' => $order_statuses,
      ':uid' => $uid
    )); 

  $header = array(
    array('data' => t('#')),
    array('data' => t('Product'), 'field' => 'n.title'),
    array('data' => t('Sold'), 'field' => 'sold'),
    array('data' => t('Total'), 'field' => 'total', 'sort' => 'desc'),
  );
  $csv_rows[] = array(t('#'), t('Product'), t('Sold'), t('Total'));
  if ($count_views) {
    $query->leftJoin('node_counter', 'nc', 'n.nid = nc.nid');
    $query->addField('nc', 'totalcount');
    $header[] = array('data' => t('Views'), 'field' => 'nc.totalcount');
    $csv_rows[0][] = t('Views');
  }
  $query->orderByHeader($header);
  $query->condition('n.type', $product_types, 'IN')
  ->condition('n.uid', $uid)
  ->groupBy('n.nid')
  ->groupBy('n.title');
  //$sql_count = "SELECT COUNT(*) FROM {node} WHERE uid = %d AND type IN (" . db_placeholders($product_types, 'text') . ")";
  //$products = pager_query($sql . tablesort_sql($header), $page_size, 0, $sql_count, array_merge($order_statuses, array($uid), $order_statuses, array($uid, $uid), $product_types));

  $products = $query->execute();
  $rows = array();
  foreach ($products as $product) {
    $row_cell = ($page * variable_get('uc_reports_table_size', 30)) + count($rows) + 1;
    $product_cell = l($product['title'], 'node/' . ($product['nid']));
    $product_csv = $product['title'];
    $sold_cell = (empty($product['sold'])) ? 0 : $product['sold'];
    $sold_csv = $sold_cell;
    $total_cell = uc_currency_format((empty($product['total'])) ? 0 : $product['total']);
    $total_csv = $total_cell;

    if (module_exists('uc_attribute')) {
      $product_models = db_query("SELECT model FROM {uc_product_adjustments} WHERE nid = :nid", array(':nid' => $product['nid']));
      $models = array(db_query("SELECT model FROM {uc_products} WHERE nid = :nid", array(':nid' => $product['nid']))->fetchField());
      $breakdown_product = $breakdown_sold = $breakdown_total = '';
      foreach ($product_models as $product_model) {
        $models[] = $product_model->model;
      }
      foreach ($models as $model) {
        $sold = db_query("SELECT SUM(qty) FROM {uc_order_products} AS p 
          LEFT JOIN {mp_seller_order_statuses} AS o ON p.order_id = o.order_id 
          WHERE o.order_status IN (:statuses)
          AND p.model = :model
          AND p.nid = :nid 
          AND o.uid = :uid", 
          array(
          	':statuses' => $order_statuses,
          	':model' => $model, 
          	':nid' => $product['nid'], 
          	':uid' => $uid
          ))->fetchField();
        $total = db_query("SELECT SUM(p.cost * p.qty) FROM {uc_order_products} AS p 
          LEFT JOIN {mp_seller_order_statuses} AS o ON p.order_id = o.order_id 
          WHERE o.order_status IN (:statuses)
          AND p.model = :model
          AND p.nid = :nid 
          AND o.uid = :uid",
          array(
          	':statuses' => $order_statuses,
          	':model' => $model, 
          	':nid' => $product['nid'], 
          	':uid' => $uid
          ))->fetchField();
        $breakdown_product .= "<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$model";
        $product_csv .= "\n     $model";
        $breakdown_sold .= "<br/>" . ((!empty($sold)) ? $sold : 0);
        $sold_csv .= "\n     " . ((!empty($sold)) ? $sold : 0);
        $breakdown_total .= "<br/>" . (uc_currency_format((!empty($total)) ? $total : 0));
        $total_csv .= "\n     " . (uc_currency_format((!empty($total)) ? $total : 0));
      }
      $product_cell = $product_cell . $breakdown_product;
      $sold_cell = '<strong>' . $sold_cell . '</strong>' . $breakdown_sold;
      $total_cell = '<strong>' . $total_cell . '</strong>' . $breakdown_total;
    }
    $row = array(
      array('data' => $row_cell),
      array('data' => $product_cell),
      array('data' => $sold_cell),
      array('data' => $total_cell, 'nowrap' => 'nowrap'),
    );
    $csv_row = array($row_cell, $product_csv, $sold_csv, $total_csv);
    
    if ($count_views) {
      $views = (empty($product['totalcount'])) ? 0 : $product['totalcount'];
      $row[] = array('data' => $views);
      $csv_row[] = $views;
    }
    $rows[] = $row;
    $csv_rows[] = $csv_row;
  }

  if (empty($rows)) {
    $rows[] = array(array(
        'data' => t('No products found'),
        'colspan' => count($header),
      ));
  }
  module_load_include('inc', 'uc_reports', 'uc_reports.admin');
  $csv_data = uc_reports_store_csv('mp_seller_products', $csv_rows);

  $build['report'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('width' => '100%', 'class' => array('uc-sales-table')),
  );
  $build['pager'] = array('#theme' => 'pager');
  $build['links'] = array(
    '#prefix' => '<div class="uc-reports-links">',
    '#suffix' => '</div>',
    'csv' => array(
      '#markup' => l(t('Export to csv file.'), 'user/' . $uid . '/selling/reports/gettxt/' . $csv_data['report'] . '/' . $csv_data['user']),
    ),
  );
  return $build;
  
  //$output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('width' => '100%', 'class' => array('uc-sales-table'))));
  //$output .= theme_pager(NULL, $page_size);
  //$output .= '<div class="uc-reports-links">' . 
  //l(t('Export to csv file.'), 'user/' . $uid . '/selling/reports/gettxt/' . $csv_data['report'] . '/' . $csv_data['user']) . '</div>';
  //return $output;
}

/**
 * Displays the yearly sales report for sellers.
 */
function mp_reports_sales($user) {
  $uid = $user->uid;
  drupal_set_title(t('Sales Reports'));

  $timezone_offset = REQUEST_TIME + variable_get('date_default_timezone', 0);
  $order_statuses = "('complete')";

  // Get the year for the report from the URL.
  if (intval(arg(5)) == 0) {
    $year = format_date($timezone_offset, 'custom', 'Y');
  }
  else {
    $year = arg(5);
  }

  // Build the header for the report table.
  $header = array(t('Month'), t('Number of orders'), t('Total revenue'), t('Average order'));

  // Build the header to the csv export.
  $csv_rows = array(array(t('Month'), t('Number of orders'), t('Total revenue'), t('Average order')));

  // For each month of the year...
  for ($i = 1; $i <= 12; $i++) {
    // Calculate the start and end timestamps for the month in local time.
    $month_start = gmmktime(0, 0, 0, $i, 1, $year);
    $month_end = gmmktime(23, 59, 59, $i + 1, 0, $year);

    // Get the sales report for the month.
    $month_sales = mp_reports_get_sales($uid, $month_start, 'month');

    // Calculate the average order total for the month.
    if ($month_sales['total'] != 0) {
      $month_average = round($month_sales['income'] / $month_sales['total'], 2);
    }
    else {
      $month_average = 0;
    }

    // Add the month's row to the report table.
    $rows[] = array(
      gmdate('M Y', $month_start),
      $month_sales['total'],
      uc_currency_format($month_sales['income']),
      uc_currency_format($month_average),
    );

    // Add the data to the csv export.
    $csv_rows[] = array(
      gmdate('M Y', $month_start),
      $month_sales['total'],
      uc_currency_format($month_sales['income']),
      uc_currency_format($month_average),
    );
  }

  // Calculate the start and end timestamps for the year in local time.
  $year_start = gmmktime(0, 0, 0, 1, 1, $year);
  $year_end = gmmktime(23, 59, 59, 1, 0, $year + 1);

  // Get the sales report for the year.
  $year_sales = mp_reports_get_sales($uid, $year_start, 'year');

  // Calculate the average order total for the year.
  if ($year_sales['total'] != 0) {
    $year_average = round($year_sales['income'] / $year_sales['total'], 2);
  }
  else {
    $year_average = 0;
  }

  // Add the total row to the report table.
  $rows[] = array(
    t('Total @year', array('@year' => $year)),
    $year_sales['total'],
    uc_currency_format($year_sales['income']),
    uc_currency_format($year_average),
  );

  // Add the total data to the csv export.
  $csv_rows[] = array(
    t('Total @year', array('@year' => $year)),
    $year_sales['total'],
    uc_currency_format($year_sales['income']),
    uc_currency_format($year_average),
  );

  // Cache the csv export.
  module_load_include('inc', 'uc_reports', 'uc_reports.admin');
  $csv_data = uc_reports_store_csv('mp_seller_sales_yearly', $csv_rows);

  // Build the page output holding the form, table, and csv export link.
  $build['form'] = drupal_get_form('mp_reports_sales_year_form', $year);
  $build['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('width' => '100%', 'class' => array('uc-sales-table'))
  );
  $build['links'] = array(
    '#prefix' =>'<div class="uc-reports-links">',
    'csv' => array('#markup' => l(t('Export to csv file.'), 'user/' . $uid . '/selling/reports/gettxt/' . $csv_data['report'] . '/' . $csv_data['user'])),
    '#suffix' => '</div>',
  );

  return $build;
}

/**
 * Form to specify a year for the yearly sales report.
 */
function mp_reports_sales_year_form($form, $form_state, $year) {
  $form['year'] = array(
    '#type' => 'textfield',
    '#title' => t('Sales year'),
    '#default_value' => $year,
    '#maxlength' => 4,
    '#size' => 4,
    '#prefix' => '<div class="sales-year">',
    '#suffix' => '</div>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('View'),
    '#prefix' => '<div class="sales-year">',
    '#suffix' => '</div>',
  );

  return $form;
}

/**
 * Submit handler for custom year selection.
 */
function mp_reports_sales_year_form_submit($form, &$form_state) {
  drupal_goto('user/' . arg(1) . '/selling/reports/sales/' . $form_state['values']['year']);
}

/**
 * Displays summary report for seller's of all their sales.
 */
function mp_reports_summary($user) {
  $uid = $user->uid;
  drupal_set_title(t('Sales Reports'));

  $timezone_offset = variable_get('date_default_timezone', 0);
  $format = variable_get('uc_date_format_default', 'm/d/Y');

  $order_statuses = array('complete');

  $today_start = gmmktime(0, 0, 0, date('n'), date('j'), date('Y')) - $timezone_offset;
  $today_end = gmmktime(23, 59, 59, date('n'), date('j'), date('Y')) - $timezone_offset;

  // Build the report table header.
  $header = array(t('Sales data'), t('Number of orders'), t('Total revenue'), t('Average order'));

  // Calculate and add today's sales summary to the report table.
  $today = mp_reports_get_sales($uid, $today_start);

  $rows[] = array(
    t('Today, !date', array('!date' => format_date($today_start, 'custom', $format))),
    $today['total'],
    uc_currency_format($today['income']),
    uc_currency_format($today['average']),
  );

  // Calculate and add yesterday's sales summary to the report table.
  $yesterday = mp_reports_get_sales($uid, $today_start - 86400);

  $rows[] = array(
    t('Yesterday, !date', array('!date' => format_date($today_start - 86400, 'custom', $format))),
    $yesterday['total'],
    uc_currency_format($yesterday['income']),
    uc_currency_format($yesterday['average']),
  );

  // Get the sales report for the month.
  $month = mp_reports_get_sales($uid, $today_start, 'month');
  $month_title = format_date($today_start, 'custom', 'M Y');

  // Calculate the daily averages for the month.
  $daily_orders = round($month['total'] / format_date(REQUEST_TIME, 'custom', 'j'), 2);
  $daily_revenue = round($month['income'] / format_date(REQUEST_TIME, 'custom', 'j'), 2);

  if ($daily_orders > 0) {
    $daily_average = round($daily_revenue / $daily_orders, 2);
  }
  else {
    $daily_average = 0;
  }

  // Add the daily averages for the month to the report table.
  $rows[] = array(
    t('Daily average for @month', array('@month' => $month_title)),
    $daily_orders,
    uc_currency_format($daily_revenue),
    uc_currency_format($daily_average),
  );

  // Store the number of days remaining in the month.
  $remaining_days = format_date($today_start, 'custom', 't') - format_date($today_start, 'custom', 'j');

  // Add the projected totals for the month to the report table.
  $rows[] = array(
    t('Projected totals for @date', array('@date' => $month_title)),
    round($month['total'] + ($daily_orders * $remaining_days), 2),
    uc_currency_format(round($month['income'] + ($daily_revenue * $remaining_days), 2)),
    '',
  );

  // Add the sales data report table to the output.
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('uc-sales-table'))));

  // Build the header statistics table header.
  $header = array(array(
      'data' => t('Statistics'),
      'width' => '50%',
    ), '');

  $grand_total = db_query("SELECT SUM(p.cost * p.qty) 
  	FROM {mp_seller_order_statuses} AS sos 
  	LEFT JOIN {uc_orders} AS o ON sos.order_id = o.order_id 
  	LEFT JOIN {uc_order_products} AS p ON p.order_id = o.order_id 
  	LEFT JOIN {node} AS n ON n.nid = p.nid 
  	WHERE sos.order_status IN (:statuses)
  	AND n.uid = :uid 
  	AND sos.uid = :uid", 
    array(':statuses' => $order_statuses, ':uid' => $uid))
    ->fetchField();
  $rows = array(
    array(array('data' => t('Grand total sales')), array('data' => uc_currency_format($grand_total))),
  );

  // Add the statistics table to the output.
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('width' => '100%', 'class' => array('uc-sales-table'))));

  return $output;
}

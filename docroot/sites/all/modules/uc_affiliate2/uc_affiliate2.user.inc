<?php

/**
 * @file
 * User functions for the uc_affiliate2 module.
 *
 */

/*
 * Display dashboard for affiliate
 */
function uc_affiliate2_dashboard($account) {
  $commissions = array(
    'today' => 0,
    'week' => 0,
    'year' => 0,
  );
  $clicks = array(
    'today' => 0,
    'week' => 0,
    'year' => 0,
  );

  // get today's commissions
  $dates_today = _uc_affiliate2_predefined_range(7);

 /* $query = db_select('uc_affiliate2_commission','ac');
  $query->fields('ac',array('commission_id'));
  $query->innerJoin('uc_orders','uco','ac.order_id = uco.order_id');
  $query->condition('aid',$account->uid,'=');
  $query->condition('uco.created', array($dates_today['start'], $dates_today['end']), 'BETWEEN' );
  $commissions['today'] = $query->countQuery()->execute()->fetchField();
 */ $result = db_query('SELECT COUNT(ac.commission_id) as cnt FROM {uc_affiliate2_commission} ac INNER JOIN {uc_orders} uco on ac.order_id = uco.order_id WHERE aid = :aid AND uco.created BETWEEN :start AND :end', array(':aid' => $account->uid, ':start' => $dates_today['start'], ':end' => $dates_today['end']));

  foreach ($result as $row) {
     $commissions['today'] += $row->cnt;
  }

  /// get commissions from the last 7 days
  $dates_week = _uc_affiliate2_predefined_range(6);

  $sql = 'SELECT COUNT(ac.commission_id) as cnt
            FROM {uc_affiliate2_commission} ac
              INNER JOIN {uc_orders} uco on ac.order_id = uco.order_id
                WHERE aid = :aid AND uco.created BETWEEN :start AND :end';
  $result = db_query($sql, array(':aid' => $account->uid, ':start' => $dates_week['start'], ':end' => $dates_week['end']));

//   $query = db_select('uc_affiliate2_commission','ac');
//   $query->fields('ac',array('commission_id'));
//   $query->innerJoin('uc_orders','uco','ac.order_id = uco.order_id');
//   $query->condition('aid',$account->uid,'=');
//   $query->condition('uco.created', array($dates_today['start'], $dates_today['end']), 'BETWEEN' );
//   $commissions['week'] = $query->countQuery()->execute()->fetchField();
  foreach ($result as $row) {
    $commissions['week'] += $row->cnt;
  }

  // get commissions from the last year
  $dates_year = _uc_affiliate2_predefined_range(5);

  $sql = 'SELECT COUNT(ac.commission_id) as cnt
            FROM {uc_affiliate2_commission} ac
              INNER JOIN {uc_orders} uco on ac.order_id = uco.order_id
                WHERE aid = :aid AND uco.created BETWEEN :start AND :end';
  $result = db_query($sql, array(':aid' => $account->uid, ':start' => $dates_year['start'], ':end' => $dates_year['end']));

  foreach ($result as $row) {
    $commissions['year'] += $row->cnt;
  }

  // get today's clicks
  $sql = "SELECT count(click_id) as clicks from {uc_affiliate2_clicks}
            WHERE aid = :aid AND time BETWEEN :start AND :end
              GROUP BY aid";
  $result = db_query($sql, array(':aid' => $account->uid, ':start' => $dates_today['start'], ':end' => $dates_today['end']));

  foreach ($result as $row) {
    $clicks['today'] += $row->clicks;
  }

  // get commissions from the last 7 days
  $sql = "SELECT count(click_id) as clicks from {uc_affiliate2_clicks}
            WHERE aid = :aid AND time BETWEEN :start AND :end
              GROUP BY aid";
  $result = db_query($sql, array(':aid' => $account->uid, ':start' => $dates_week['start'], ':end' => $dates_week['end']));

  foreach ($result as $row) {
    $clicks['week'] += $row->clicks;
  }

  // get commissions from the last year
  $sql = "SELECT count(click_id) as clicks from {uc_affiliate2_clicks}
            WHERE aid = :aid AND time BETWEEN :start AND :end
              GROUP BY aid";
  $result = db_query($sql, array(':aid' => $account->uid, ':start' => $dates_year['start'], ':end' => $dates_year['end']));

  foreach ($result as $row) {
    $clicks['year'] += $row->clicks;
  }

  drupal_set_title(t('Affiliate Center'));

  $output = '';
  $output .= '<div id="affiliate-sales-header"><h2>' . t('Commission History') . '</h2> <br />';

  $output .=  t('Sales Today:') . " $commissions[today] <br />" .
              t('Sales last 7 Days:') . " $commissions[week] <br />" .
              t('Last 365 Days:') . " $commissions[year] <br /><br />";

  $commissions_url = join('/', array('user', $account->uid, 'affiliate', 'commissions'));
  $output .= l(t('Click here for more reports.'), $commissions_url, array('query' => NULL, 'fragment' => NULL, 'absolute' => TRUE, 'html' => TRUE));

  $output .= '</div>';

  $output .= '<br /><div id="affiliate-sales-click-count"><h2>' . t('Unique Click Count') . '</h2> <br />';

  $output .= t("Today's Clicks:") . " $clicks[today] <br />" .
             t("Last 7 Days:") . " $clicks[week] <br />" .
             t("Last 365 Days:") . " $clicks[year] <br /><br />";

  $clicks_url = join('/', array('user', $account->uid, 'affiliate', 'clicks'));
  $output .= l(t('Click here for customized click reporting.'), $clicks_url, array('query' => NULL, 'fragment' => NULL, 'absolute' => TRUE, 'html' => TRUE));

  $output .= '</div>';

  $output .= '<br /><div id="affiliate-links-header"><h2>' . t('Affiliate Links') . '</h2>';
  switch (variable_get('affiliate_default_link_type', 'path')) {
    case 'path':
      $output .= t('You can append a destination page to the end of the affiliate links to redirect to a specific page,
                  as well as track clicks to it, i.e.
                  @example', array('@example' => _uc_affiliate2_url('node/1', $account, array('alias' => TRUE, 'type' => 'path'))));
      break;
    case 'query':
      $output .= t('You can link to any page on this site by adding your query argument (?a=@uid) to the end of the url, i.e. @example', array('@uid' => $account->uid, '@example' => _uc_affiliate2_url('node/1', $account, array('alias' => TRUE, 'type' => 'query'))));
      break;
  }

  $text_link = variable_get('affiliate_text_link_template', '');
  $text_link = _uc_affiliate2_l($text_link, '<front>', $account, array('html' => TRUE));
  $text_link = array(
    '#type' => 'textarea',
    '#title' => t('Text Link'),
    '#value' => $text_link,
  );
  $output .= drupal_render($text_link);

  $output .= _uc_affiliate2_display_banners();

  $links_url = join('/', array('user', $account->uid, 'affiliate', 'links'));
  $output .= '<p>' . l(t('Click here for product links.'), $links_url, array('query' => NULL, 'fragment' => NULL, 'absolute' => TRUE, 'html' => TRUE)) . '</p>';

  $output .= '</div>';

  return $output;
}

function uc_affiliate2_links($account) {
  $output = '';

  $products = array('all' => t('Show All'));
  $products += _uc_affiliate2_get_product_list();

  if (count($products) > 1) {
    $output .= '<b>' . t('Export product feed') . '</b> <br />';
    $output .= t('Choose the products which you want included in your feed.') . '<br />';
    $output .= t('All product links will have your affiliate id included.') . '<br /><br /> ';

    $output .= drupal_render(drupal_get_form('_uc_affiliate2_product_form', $products, $account));
  }
  else {
    $output .= t('No products found.');
  }

  return $output;
}

// This function exports the products in html
function uc_affiliate2_links_export() {
  $ids = func_get_args();

  $output = '<br />';
  $output .= t('!image is the placeholder image in case none is present.',
                  array('!image' => '<img src="' . $GLOBALS['base_url'] . '/misc/watchdog-error.png' . '" />'));
  $output .= '<br />';

  if ($ids[0] == 'all') {
    $products = _uc_affiliate2_get_products('html');
  }
  else {
    $products = _uc_affiliate2_get_products('html', $ids);
  }

  $output .= _uc_affiliate2_display_products($products);
  $output .= theme('pager', array('tags' => NULL, 'element' => 0));

  return $output;
}

// Show the html products in a table
function _uc_affiliate2_display_products($products) {
  global $base_url;

  $header[] = array(
    'field' => 'product_title',
    'data' => t('Product Title'),
  );
  $header[] = array(
    'field' => 'preview',
    'data' => t('Preview'),
  );
  $header[] = array(
    'field' => 'code',
    'data' => t('Code'),
  );

  if (count($products) < 1) {
    return t('No products found.');
  }
  $rows=array();
  foreach ($products as $key => $product) {
    $line = array();

    $line[] = $key;
    $line[] = $product;
    $line[] = '<textarea cols="60" rows="4">' . $product . '</textarea>';

    $rows[] = $line;
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

function _uc_affiliate2_product_form($form, &$form_state, $products, $account) {

  drupal_add_js(drupal_get_path('module', 'uc_affiliate2') . '/uc_affiliate2.js');

  $form['uid'] = array(
    '#type' => 'value',
    '#value' =>  $account->uid,
  );

  $form['products'] = array(
    '#type' => 'checkboxes',
    '#columns' => 2,
    '#options' => $products,
  );

  $form['export_type'] = array(
    '#type' => 'radios',
    '#title' => t('Export in'),
    '#options' => array(
      'html' => 'HTML',
      'csv' => 'CSV',
    ),
    '#default_value' => 'csv',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Export'),
  );

  return $form;
}

function _uc_affiliate2_product_form_submit($form, &$form_state) {
  global $user;

  $show_these = array();

  // build the filter array for _uc_affiliate2_get_products from the form checkboxes
  foreach ($form_state['values']['products'] as $product_id => $status) {
    if ($status !== 0) {
      $show_these[] = $product_id;
    }
  }

  if (in_array('all', $show_these)) {
    $show_these = array('all'); // we don't need anything else
  }

  if (count($show_these) < 1) {
    drupal_set_message(t('You must select at least one product.'));

    return;
  }

  if ($form_state['values']['export_type'] == 'csv') {
    drupal_add_http_header('Content-Type',"application/csv; charset=\"utf-8\"\n");
    drupal_add_http_header('Content-Disposition','attachment; filename="export.csv"');

    if ($show_these[0] == 'all') {
      print _uc_affiliate2_get_products('csv');
    }
    else {
      print _uc_affiliate2_get_products('csv', $show_these);
    }

    //module_invoke_all('exit'); - this causes a header warning in the watchlog because the form tries to redirect to itself
    exit;
  }
  else { // html
    $ids = join('/', $show_these);
    $path = join('/', array('user', $form_state['values']['uid'], 'affiliate', 'links', 'export', $ids));

    drupal_goto($path);
  }
}

function _uc_affiliate2_display_banners() {
  global $base_url, $user;

  $banners = variable_get('affiliate_banner_template', '');

  $header[] = array(
    'field' => 'banner_preview',
    'data' => t('Banner Preview'),
  );
  $header[] = array(
    'field' => 'banner_code',
    'data' => t('Banner Code'),
  );

  $banners = explode('<', $banners);
  unset($banners[0]);

  if (count($banners) < 1) {
    return;
  }

  foreach ($banners as $banner) {
    $banner = '<' . $banner;
    $banner_link = _uc_affiliate2_l($banner, '<front>', $user, array('html' => TRUE));

    $line = array();
    $line[] = $banner;
    $line[] = '<textarea cols="45">' . $banner_link . '</textarea>';
    $rows[] = $line;
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/*
 * Display affiliate commission records
 */
function uc_affiliate2_commissions($account) {
  drupal_set_title(t('Commissions'));

  // setup the table
  $header = array(
    array('data' => t('Customer')),
    array(
      'data' => t('Order'),
      'field' => 'order_id',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Date'),
      'field' => 'order_date',
    ),
    array('data' => t('Notes')),
    array(
      'data' => t('Purchase'),
      'field' => 'order_total',
    ),
    array(
      'data' => t('Commission'),
      'field' => "commission",
    ),
  );

  // Create sql to affiliate's commissions
  $dates = _uc_affiliate2_predefined_range(4);

  /*  $sql = 'SELECT uco.uid, u.name, ac.order_id, uco.created as order_date, ac.commission_notes, uco.order_total,
   ac.commission FROM {uc_affiliate2_commission} ac
   INNER JOIN {uc_orders} uco ON ac.order_id = uco.order_id
   LEFT JOIN {users} u ON uco.uid = u.uid
   WHERE aid = %d AND uco.created BETWEEN %d AND %d';
   $sql .= tablesort_sql($header);
   */
  $query = db_select('uc_affiliate2_commission', 'ac');
  $query->extend('TableSort')
  ->orderByHeader($header)
  ->innerjoin( 'uc_orders', 'uco', 'ac.order_id = uco.order_id' );
  $query->leftjoin( 'users', 'u', 'uco.uid = u.uid');
  $query->fields( 'ac', array('order_id', 'commission_notes', 'commission'));
  $query->fields( 'uco', array('uid', 'created', 'order_total'));
  $query->fields( 'u', array('name'));
  $query->condition('aid', $account->uid, '=');
  $query->condition('uco.created', array($dates['start'], $dates['end']), 'BETWEEN' );

  $result = $query->execute();
  // $result = db_query($sql, $account->uid, $dates['start'], $dates['end']);

  $total_sales = 0;
  $total_commission = 0;
  $rows = array();
  foreach ($result as $commission) {
    if ($commission->uid > 0 && user_access('access user profiles')) {
      // $commission['name'] represents the name that you can see when visiting the profile
      $u = l($commission->name, 'user/' . $commission->uid);
    }
    else {
      // in this case, we only show a number
      if ($commission->uid == 0) {
        $u = t('Unknown');
      }
      else {
        $u = t('User #') . $commission->uid;
      }
    }
    $rows[] = array(
      $u,
      $commission->order_id,
      format_date($commission->created, 'custom', 'Y/m/d'),
      $commission->commission_notes,
      uc_currency_format($commission->order_total),
      uc_currency_format($commission->commission),
    );
    $total_sales += $commission->order_total;
    $total_commission += $commission->commission;
  }
  if ($total_commission > 0) {
    $rows[] = array('', '', '', t('Total:'), uc_currency_format($total_sales), uc_currency_format($total_commission));
  }
  $empty_message = t('No commision has been paid for the period starting');
  $empty_message .= ' ' . format_date($dates['start'], 'custom', 'F d, Y') . ' ';
  $empty_message .= t('and ending');
  $empty_message .= ' ' . format_date($dates['end'], 'custom', 'F d, Y');
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'empty' => $empty_message));

  return $output;
}

/*
 * Display affiliate clicks
 */
function uc_affiliate2_clicks($account) {
  drupal_set_title(t('Unique Click count'));
  $dates = _uc_affiliate2_predefined_range(arg(4));
  $query = db_select('uc_affiliate2_clicks')
  ->fields('uc_affiliate2_clicks', array('time'))
  ->condition('aid', $account->uid)
  ->condition('time', array($dates['start'], $dates['end']), 'BETWEEN');
  $header = array(
    array(
      'data' => t('Date'),
      'field' => "time",
    ),
    array(
      'data' => t('Unique Clicks'),
      'field' => "clicks",
    ),
  );
  $query->extend('TableSort')->orderByHeader($header);
  $result = $query->execute();
  $clicks = array();
  foreach ($result as $click) {
    $date = date('Y/m/d', $click->time);
    if(!isset($clicks[$date])) $clicks[$date]=0;
    $clicks[$date] += 1;
  }
  $total_clicks = 0;
  foreach ($clicks as $click_str => $clicks_count) {
    $click_date = strtotime($click_str);
    $click_date_end = $click_date + 86399;
    $link = l(date('Y/m/d', $click_date), 'user/' . $account->uid . '/affiliate/clicks/details/' . $click_date . '/' . $click_date_end);
    $rows[] = array($link, $clicks_count);
    $total_clicks += $clicks_count;
  }
  $rows[] = array(t('Total:'), $total_clicks);

  // Build the output

  $output = theme('table',array('header'=>$header, 'rows'=> $rows));

  return $output;

  /* OLD CODE FOR REFERENCE
   // Create sql to display level of affiliate specified
   $dates = _uc_affiliate2_predefined_range(arg(4));
   $sql = "SELECT time FROM {uc_affiliate2_clicks} WHERE aid = %d AND time BETWEEN %d AND %d";
   $result = db_query($sql, $account->uid, $dates['start'], $dates['end']);

   $clicks = array();
   while ($click_day = db_fetch_array($result)) {
   $click_day['time'] = (int)format_date($click_day['time'], 'custom', 'U');

   $date = date('Y/m/d', $click_day['time']);
   $date_unix = strtotime($date);

   if (isset($clicks[$date_unix])) {
   $clicks[$date_unix] += 1;
   }
   else {
   $clicks[$date_unix] = 1;
   }
   }

   /*
   * Since we are building the array by hand instad of grouping with date functions
   * (because mysql/postgresql have different ways to do this, and we need to support both),
   * we need to simulate the sorting

   $header = array(
   array('data' => t('Date'), 'field' => "date"),
   array('data' => t('Unique Clicks'), 'field' => "clicks"),
   );

   $sort = str_replace('ORDER BY', '', tablesort_sql($header));
   $sort_params = explode(' ', trim($sort));

   if ($sort_params[0] == 'date' && $sort_params[1] == 'ASC') {
   ksort($clicks);
   }
   elseif ($sort_params[0] == 'date' && $sort_params[1] == 'DESC') {
   krsort($clicks);
   }
   elseif ($sort_params[0] == 'clicks' && $sort_params[1] == 'ASC') {
   asort($clicks);
   }
   elseif ($sort_params[0] == 'clicks' && $sort_params[1] == 'DESC') {
   arsort($clicks);
   }

   $total_clicks = 0;
   foreach ($clicks as $click_date => $clicks_count) {
   $click_date_end = $click_date + 86399;
   $link = l(date('Y/m/d', $click_date), 'user/' . $account->uid . '/affiliate/clicks/details/' . $click_date . '/' . $click_date_end);
   $rows[] = array($link, $clicks_count);
   $total_clicks += $clicks_count;
   }
   $rows[] = array(t('Total:'), $total_clicks);

   // Build the output
   global $pager_total_items;

   $output = theme('table', $header, $rows);

   return $output; */
}

/*
 * Display a detailed clicks view for the selected date
 */
function uc_affiliate2_click_details($account) {
  global $user;

  drupal_set_title(t('Unique Clicks - Details'));

  $cookie_statuses = array(t('Not sent'), t('Sent (Unconfirmed)'), t('Sent (Confirmed)'));

  $output = t('Viewing clicks for:') . ' ' . date('Y/m/d', arg(5)) . '<br /> <br />';
  $output .= t('Cookie Status can be: Not sent, Sent (Unconfirmed), Sent (Confirmed).') . '<br />';

  $ip_masking = variable_get('affiliate_ip_masking', 'hide');

  // setup the table
  $header = array(
    array(
      'data' => t('Date/Time'),
      'field' => "time",
      'sort' => 'desc',
    ),
    array(
      'data' => t('Client IP'),
      'field' => "client_ip",
    ),
    array(
      'data' => t('Referrer'),
      'field' => "referrer",
    ),
    array(
      'data' => t('Destination'),
      'field' => "destination",
    ),
    array(
      'data' => t('Cookie Status'),
      'field' => "cookie_status",
    ),
  );

  if ($ip_masking == 'hide') {
    unset($header[1]);
  }

  // Create sql to display level of affiliate specified
  $dates = _uc_affiliate2_predefined_range(0);

  $sql = "SELECT * FROM {uc_affiliate2_clicks} WHERE aid = :aid AND time BETWEEN :start AND :end";

  //$sql .= tablesort_sql($header);
  $result = db_query($sql, array(':aid'=>$account->uid, ':start'=>$dates['start'],':end'=> $dates['end']));

  $total_clicks = 0;
  while ($click = $result->fetchAssoc()) {
    if ($click['referrer']) {
      $click['referrer'] = l(t('Referrer'), $click['referrer']); // make it a link
    }
    else {
      $click['referrer'] = t('N/A');
    }

    if ($click['destination']) {
      $click['destination'] = l(t('Destination'), $click['destination']); // make it a link
    }
    else {
      $click['destination'] = t('Frontpage');
    }

    if ($click['client_ip']) {
      $click['client_ip'] = _uc_affiliate2_mask_ip($click['client_ip']);
    }
    else {
      $click['client_ip'] = t('N/A');
    }

    $row = array(
      format_date($click['time'], 'custom', 'Y/m/d H:i:s'),
      $click['client_ip'],
      $click['referrer'],
      $click['destination'],
      ($click['cookie_status'] === NULL) ? 'Not specified' : $cookie_statuses[$click['cookie_status']],
    );

    if ($ip_masking == 'hide') {
      unset($row[1]);
    }

    $rows[] = $row;
    $total_clicks += 1;
  }
  $rows[] = array(
    array(
      'data' => t('Total:'),
      'colspan' => ($ip_masking == 'hide' ? 3 : 4),
      'align' => 'right',
    ),
    $total_clicks,
  );

  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  return $output;
}

/**
 * Mask the IP address per the defined setting.
 */
function _uc_affiliate2_mask_ip($ip) {
  global $user;

  if ($user->uid == 1) {
    return $ip;
  }

  $ip_masking = variable_get('affiliate_ip_masking', 'hide');
  $ip_numbers = explode('.', $ip);

  switch ($ip_masking) {
    default: // = case 'hide':
      $ip_numbers[3] = '*';
    case 'one':
      $ip_numbers[2] = '*';
    case 'two':
      $ip_numbers[1] = '*';
      $ip_numbers[0] = '*';
      break;

    case 'first':
      $ip_numbers[1] = '*';
      $ip_numbers[2] = '*';
      $ip_numbers[3] = '*';
      break;

    case 'full':
      break;
  }

  $ip = implode('.', $ip_numbers);

  return $ip;
}

/*
 * Display affiliate orders by status
 */
function uc_affiliate2_orders($account) {
  drupal_set_title(t('All Orders'));

  // setup the table
  $header = array(
    array('data' => t('Customer')),
    array(
      'data' => t('Order'),
      'field' => 'ao.order_id',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Order Status'),
      'field' => 'order_status',
    ),
    array(
      'data' => t('Date'),
      'field' => 'created',
    ),
    array(
      'data' => t('Purchase'),
      'field' => 'order_total',
    ),
  );

  // Create sql to affiliate's commissions
  $query = db_select('uc_affiliate2_orders', 'ao');
  $query->innerjoin('uc_orders', 'uco', 'ao.order_id = uco.order_id');
  $query->leftjoin('users', 'u', 'uco.uid = u.uid');
  $query->fields('uco')
  ->fields('ao', array('order_id'))
  ->fields('u', array('name'));
  $query
  ->extend('TableSort')
  ->orderByHeader($header);
  $result = $query->execute(); // db_query($sql, $account->uid);

  $total_sales = 0;
  foreach ($result as $order) {
    if ($order->uid > 0 && user_access('access user profiles')) {
      $u = l($order->name, 'user/' . $order->uid);
    }
    else {
      // in this case, we only show a number
      if ($order->uid == 0) {
        $u = t('Guest');
      }
      else {
        $u = t('User #') . $order->uid;
      }
    }
    $total_sales += $order->order_total;
    $rows[] = array(
      $u,
      $order->order_id,
      ucwords(str_replace('_', ' ', $order->order_status)),
      format_date($order->created, 'custom', 'm-d-Y'),
      uc_currency_format($order->order_total),
    );
  }
  $rows[] = array('', '', '', t('Total:'), uc_currency_format($total_sales));
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}

/*
 * Display affiliate's downline
 */
function uc_affiliate2_downline($account, $level = 1) {
  drupal_set_title(t('Downline'));

  //make sure user doesn't try to manipulate depth
  if ($level > variable_get('affiliate_hierarchy_depth', 5)) {
    $level = variable_get('affiliate_hierarchy_depth', 5);
  }

  // setup the table
  $header = array(
    array(
      'data' => t('Username'),
      'field' => "u$level.name",
    ),
    array(
      'data' => t('Affiliate'),
      'field' => "u$level.status",
    ),
    array(
      'data' => t('Member for'),
      'field' => "u$level.created",
      'sort' => 'desc',
    ),
    array(
      'data' => t('Last access'),
      'field' => "u$level.access",
    ),
    array('data' => t('Level')),
    array(
      'data' => t('Customers'),
      'field' => "uc.user_count",
    ),
  );

  // Create sql to display level of affiliate specified
  $query = db_select('users', 'u1')
  ->fields('u1', array('uid'));
  $next = 1;
  for ($i = 1; $i <= $level; $i++) {
    $next++;
    $query->innerjoin('uc_affiliate2_users', 'au' . $i, 'au' . $i . '.uid = u' . $i . '.uid');
    $query->innerjoin('users', 'u' . $next, 'au' . $i . '.aid = u' . $next . '.uid');
  }
  // user_count subquery
  $subquery = db_select('uc_affiliate2_users', 'au');
  $subquery->addField('au', 'aid');
  $subquery->addExpression('COUNT(au.aid)', 'user_count');
  $subquery->groupBy('au.aid');
  // Attach subquery & field
  $query->leftJoin($subquery, 'uc', 'uc.aid = u1.uid');
  $query->addField('uc', 'user_count');

  $query->condition('u' . $next . '.uid', $account->uid, '=');
  $query->extend('PagerDefault')->limit(25)->extend('TableSort')->orderByHeader($header);

  $result = $query->execute();

  // Create the data rows for our table
  $rows = array();
  foreach ($result as $affiliate) {
    $downline_user = user_load($affiliate->uid);
    $rows[] = array(
      theme('username', array('account' => $downline_user) ),
      user_access('act as affiliate', $downline_user) ? 'Yes' : 'No',
      format_interval(REQUEST_TIME - $downline_user->created),
      $downline_user->access ? t('%time ago', array('%time' => format_interval(REQUEST_TIME - $downline_user->access))) : t('never'),
      $level,
      $affiliate->user_count > 0 ? $affiliate->user_count : 0,
    );
  }

  // Build the output
  $output = '<ul class="tabs secondary">';
  for ($i = 1; $i <= variable_get('affiliate_hierarchy_depth', 5); $i++) {
    $output .= ($i == $level ? '<li class="active">' : '<li>');
    $output .= l(t("Level") . " " . $i, 'user/' . $account->uid . '/affiliate/downline/' . $i) . "</li>";
  }
  $output .= "</ul>";

  global $pager_total_items;
  $output .= '<h2>' . t('Total Level !level', array('!level' => $level)) . '</h2>';
  $output .= theme('table',
    array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t('Currently you have nobody in your downline.'),
  )
  );
  $output .= theme('pager', array('tags' => array()));
  return $output;
}

/*
 * Display affiliate's upline
 */
function uc_affiliate2_upline($account) {
  drupal_set_title(t('Upline'));

  // setup the table
  $header = array(
    array('data' => t('Level')),
    array('data' => t('Username')),
    array('data' => t('Member for')),
    array('data' => t('Last access')),
    array('data' => t('Customers')),
  );

  // Create sql to display user's upline
  $level = variable_get('affiliate_hierarchy_depth', 5);
  $rows = array();
  $next_uid = $account->uid;
  for ($i = 1; $i <= $level; $i++) {
    $upline = db_select('users', 'u')
    ->condition('au.uid', $next_uid, '=')
    ->fields('u', array('uid', 'name', 'status', 'access') );
    $upline->innerjoin('uc_affiliate2_users', 'au', 'au.uid = u.uid');
    $upline->execute();
    /*
     * OLD QUERY
     $sql = "SELECT u.uid, u.name, u.status, u.created, u.access, uc.user_count, ". $i ." AS level ";
     $sql .= "FROM {users} u INNER JOIN {uc_affiliate2_users} au ON au.aid = u.uid ";
     $sql .= "LEFT OUTER JOIN (SELECT au2.aid, COUNT(au2.uid) as user_count FROM {uc_affiliate2_users} au2
     * GROUP BY au2.aid) uc on uc.aid = u.uid ";
     $sql .= "WHERE au.uid = %d";
     */
    // create the data rows for our table
    $status = array(t('blocked'), t('active'));
    foreach ($upline as $affiliate) {
      $next_uid = $affiliate->uid;
      $user_count = db_select('uc_affiliate2_users', 'au')
      ->condition('au.uid', $next_uid, '=')
      ->countQuery()->execute()->fetchfield();
      $rows[] = array(
        $affiliate->level,
        theme('username', array('account'=>$affiliate)),
        format_interval(REQUEST_TIME - $affiliate->created),
        $affiliate->access ? t('%time ago', array('%time' => format_interval(REQUEST_TIME - $account->access))) : t('never'),
        $user_count ? $user_count : 0,
      );
    }
  }

  if (count($rows) > 0) {
    $output = theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t("You are a top level affiliate.  There are no affiliates in your upline.")));
  }
  else {
    $output = "<p>" . t("You are a top level affiliate.  There are no affiliates in your upline.") . "</p>";
  }
  return $output;
}

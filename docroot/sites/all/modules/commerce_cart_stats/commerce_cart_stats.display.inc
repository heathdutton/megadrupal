<?php
/**
 * @file
 * Display functions for JHUP Commerce Stats module.
 */

/**
 * Renders the stat display page.
 *
 * @return string
 *   Page contents to be printed.
 */
function commerce_cart_stats_display() {
  $build = array();
  $build['#theme']              = 'commerce_cart_stats';
  $build['#count_uniques']      = commerce_cart_stats_count_uniques();
  $build['#adds_removes']       = commerce_cart_stats_adds_removes();
  $build['#statuses']           = commerce_cart_stats_statuses();
  $build['#abandoned_products'] = commerce_cart_stats_abandoned_products();

  $content = '';
  $content .= theme('commerce_cart_stats_display', $build['#count_uniques']);
  $content .= theme('commerce_cart_stats_display', $build['#adds_removes']);
  $content .= theme('commerce_cart_stats_display', $build['#statuses']);
  $content .= theme('commerce_cart_stats_display', $build['#abandoned_products']);

  return $content;
}


/**
 * Generates the table that counts unique users, hosts, and orders.
 *
 * @return  string
 *   Table that counts up the number of unique users, hosts, and orders.
 */
function commerce_cart_stats_count_uniques() {
  $qres = db_query("SELECT COUNT(DISTINCT(uid)) as uidcount FROM {commerce_order}")->fetch();
  $more_qres = db_query("SELECT COUNT(DISTINCT(hostname)) as hostcount FROM {commerce_order} WHERE uid = :uidnum", array(':uidnum' => 0))->fetch();
  $num_unique_users = $qres->uidcount + $more_qres->hostcount - 1;

  $qres = db_query("SELECT COUNT(DISTINCT(hostname)) as hostcount FROM {commerce_order}")->fetch();
  $num_unique_hosts = $qres->hostcount;

  $qres = db_query("SELECT COUNT(DISTINCT(order_id)) as ordercount FROM {commerce_order}")->fetch();
  $num_unique_orders = $qres->ordercount;

  $variables = array();
  $variables = array(
    '#theme'   => 'table',
    '#header'  => array(
      t('Unique Users'),
      t('Unique Hosts'),
      t('Unique Orders'),
    ),
    '#rows'    => array(
      $num_unique_users,
      $num_unique_hosts,
      $num_unique_orders,
    ),
    '#caption' => t('Unique # of Users, Hosts, and Orders'),
  );

  return $variables;
}


/**
 * Generates the table counting up the number of cart adds/removes.
 *
 * @return string
 *   Table that counts up all the cart add/remove events.
 */
function commerce_cart_stats_adds_removes() {
  $query = db_select('commerce_cart_stats_cart_contents', 'ccs');
  $query->fields('ccs', array('product_id', 'event', 'quantity'));
  $result = $query->execute();

  $variables = array(
    '#theme'   => 'table',
    '#header'  => array(
      t('Product'),
      t('Adds'),
      t('Removes'),
      t('Delta'),
    ),
    '#rows'    => array(),
    '#caption' => t('Cart Adds/Removes Per Product'),
  );

  $rows = array();
  foreach ($result as $res) {
    if (!isset($rows[$res->product_id]['add'])) {
      $rows[$res->product_id]['add'] = 0;
    }
    if (!isset($rows[$res->product_id]['remove'])) {
      $rows[$res->product_id]['remove'] = 0;
    }
    $rows[$res->product_id][$res->event] += $res->quantity;
  }

  $total_adds        = 0;
  $total_removes     = 0;
  foreach ($rows as $key => $values) {
    $product_info = commerce_product_load($key);
    $variables['#rows'][] = array(
      check_plain($product_info->title),
      $values['add'],
      $values['remove'],
      ($values['add'] - $values['remove']),
    );
    $total_adds  += $values['add'];
    $total_removes += $values['remove'];
  }
  $variables['rows'][] = array(
    t('Totals'),
    $total_adds,
    $total_removes,
    ($total_adds - $total_removes),
  );

  return $variables;
}


/**
 * Generates the table counting up all the different cart statuses.
 *
 * @return string
 *   Table that counts up all the different cart statuses.
 */
function commerce_cart_stats_statuses() {
  $qres = db_query("SELECT status,COUNT(*) AS count FROM {commerce_order} GROUP BY status")->fetchAll();

  $variables = array(
    '#theme'   => 'table',
    '#header'  => array(
      t('Cart Status'),
      t('# of Orders'),
    ),
    '#caption' => t('Orders Per Order Status'),
  );
  foreach ($qres as $value) {
    $variables['#rows'][] = array(
      $value->status,
      $value->count,
    );
  }

  return $variables;
}


/**
 * Generates the table counting up the number of times a product was abandoned.
 *
 * @return string
 *   Table that contains the number of times each product was abandoned.
 */
function commerce_cart_stats_abandoned_products() {
  $query = db_select('commerce_order', 'co');
  $query->fields('co', array('order_id', 'status'));

  $cl_alias = $query->innerJoin('commerce_line_item', 'cl', '%alias.order_id = co.order_id');
  $query->addField($cl_alias, 'quantity', 'quantity');
  $query->addField($cl_alias, 'line_item_label', 'sku');

  $cp_alias = $query->innerJoin('commerce_product', 'cp', '%alias.sku = cl.line_item_label');
  $query->addField($cp_alias, 'title', 'title');

  $query->condition('co.created', REQUEST_TIME - 86400, "<");
  $query->condition('co.status', 'completed', '!=');
  $query->orderBy('status', 'ASC');
  $result = $query->execute();

  $variables = array(
    '#theme'   => 'table',
    '#header'  => array(),
    '#rows'    => array(),
    '#caption' => t('Abandoned Products Per Checkout Status'),
  );
  $variables['#header'][] = t('Product');

  $statuses = array();
  $products = array();
  foreach ($result as $res) {

    if (!isset($statuses[$res->status])) {
      $statuses[$res->status] = 0;
    }
    $statuses[$res->status]++;
    if (!isset($products[$res->title][$res->status])) {
      $products[$res->title][$res->status] = 0;
    }
    $products[$res->title][$res->status]++;
  }

  $product_count = 0;
  $total_total_abandoned = 0;
  foreach ($products as $title => $values) {
    $variables['#rows'][$product_count] = array(
      check_plain($title),
    );

    $total_abandoned = 0;
    foreach ($statuses as $status => $value) {
      if (!isset($values[$status])) {
        $values[$status] = 0;
      }
      array_push($variables['#rows'][$product_count], $values[$status]);
      $total_abandoned += $values[$status];
    }
    array_push($variables['#rows'][$product_count], $total_abandoned);
    $total_total_abandoned += $total_abandoned;
    ++$product_count;
  }

  $variables['#rows'][$product_count] = array(t('Totals'));
  foreach ($statuses as $status => $value) {
    $variables['#header'][] = $status;

    array_push($variables['#rows'][$product_count], $value);
  }
  $variables['#header'][] = t('Total');
  array_push($variables['#rows'][$product_count], $total_total_abandoned);

  return $variables;

}

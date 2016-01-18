<?php

/**
 * @file
 * Marketplace report administration menu items.
 */

/**
 * Returns a sortable table listing of the seller's products.
 *
 * @param $user
 *   The user object whose marketplace products to show.
 */
function mp_products_selling($user) {
  drupal_set_title(t('My Products'));
  $uid = $user->uid;

  $header = array(
    array(
      'data' => t('ID'),
      'field' => 'n.nid',
    ),
    array(
      'data' => t('Name'),
      'field' => 'n.title',
      'sort' => 'asc',
    ),
    array(
      'data' => t('Cost'),
      'field' => 'p.cost',
    ),
    array(
      'data' => t('Added'),
      'field' => 'n.created',
    ),
    array(
      'data' => t('Status'),
      'field' => 'n.status',
    ),
  );
  $product_types = array("'product'");
  $types = db_query("SELECT DISTINCT(pcid) FROM {uc_product_classes}")->fetchAll();
  foreach ($types as $type) {
    $product_types[] = "'" . $type->pcid . "'";
  }

  //TODO Add count query to pager.
  //$result = pager_query("SELECT n.nid, n.title, p.cost, n.created, n.status FROM {node} AS n INNER JOIN {uc_products} AS p USING (nid,vid) WHERE n.uid = %d"  . tablesort_sql($header), 20, 0, "SELECT COUNT(*) FROM {node} WHERE uid = %d AND type IN (" . implode(", ", $product_types) . ")", $uid);
  $query = db_select('node', 'n')->extend('PagerDefault')->extend('TableSort');
  $query->innerJoin('uc_products', 'p', 'n.nid = p.nid AND n.vid = p.vid');
  $result = $query->condition('n.uid', $uid)
    ->fields('n', array('nid', 'title', 'created', 'status'))
    ->fields('p', array('cost'))
    ->limit(20)
    ->orderByHeader($header)
    ->execute();

  // Build a table based on the seller's products.
  $rows = array();
  foreach ($result as $product) {
    $link = l($product->title, 'node/' . $product->nid);
    $link .= '<span class="order-admin-icons">' . l('<img src="' . base_path() . drupal_get_path('module', 'uc_store') . '/images/order_view.gif" alt="View Product" />', 'node/' . $product->nid, array('title' => 'View Product', 'html' => TRUE)) . l('<img src="' . base_path() . drupal_get_path('module', 'uc_store') . '/images/order_edit.gif" alt="Edit Product" />', 'node/' . $product->nid . '/edit', array('title' => 'Edit Product', 'html' => TRUE)) . '</span>';
    $listed = ($product->status == 0) ? t('Not Currently Listed') : t('For Sale');

    $rows[] = array(
      array('data' => $product->nid),
      array('data' => $link),
      array(
        'data' => uc_currency_format($product->cost),
        'align' => 'left',
      ),
      array('data' => format_date($product->created, 'custom', variable_get('uc_date_format_default', 'm/d/Y'))),
      array('data' => $listed),
    );
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows)) . theme('pager', array('tags' => NULL, 'element' => 0));

  return $output;
}

<?php

/**
 * @file
 * This file provides store management displays and features. It only loads
 * where the current user is a manager, and on store management pages.
 * (In contrast, store.admin.inc only loads for store configuration pages.)
 */

// -------------------------------------------------------------------------
// Transaction header form

/**
 * Valid sections are:
 *   'overview': changing workflow, timestamps, and totals (admin/store/transaction).
 *   'addresses': shipping and billing info (admin/store/transaction/address).
 *   'items': editing and adding items (admin/store/transaction/item).
 */
function ec_store_transaction_form($txnid) {
  if ($txn = ec_store_transaction_load($txnid)) {
    module_load_include('inc', 'ec_store', 'ec_store.admin');
    return _ec_store_transaction_form($txn);
  }
  else {
    drupal_not_found();
    exit();
  }
}

/**
 * Called by ec_store_transaction_form().
 */
function _ec_store_transaction_form($edit = NULL) {
  module_load_include('inc', 'ec_store', 'ec_store');
  $edit = is_array($edit) ? (object) $edit : $edit;

  $section = isset($edit->section) ? $edit->section : $_GET['section'];

  switch (drupal_strtoupper($section)) {
    case 'OVERVIEW':
      $form = ec_store_transaction_overview_form($edit);
      break;

    case 'ADDRESSES':
      $form = ec_store_transaction_addresses_form($edit);
      break;

    case 'ITEMS':
      $form = ec_store_transaction_item_form($edit);
      break;

    default:
      $form = array_merge(ec_store_transaction_overview_form($edit),
        ec_store_transaction_addresses_form($edit), ec_store_transaction_item_form($edit));
  }

  if ($edit->uid && is_numeric($edit->uid)) {
    $edit->uid = db_query('SELECT name FROM {users} WHERE uid = :uid', array(':uid' => $edit->uid))->fetchField();
  }

  if (!empty($edit->txnid)) {
    $form['txnid'] = array(
      '#type' => 'value',
      '#value' => $edit->txnid,
    );
    $form[] = array(
      '#type' => 'submit',
      '#value' => t('Update transaction'),
    );
    $form[] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }
  else {
    $form[] = array(
      '#type' => 'submit',
      '#value' => t('Create new transaction'),
    );
  }

  $form['section'] = array(
    '#type' => 'value',
    '#value' => $section,
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_transaction_form_validate(&$form, &$form_state) {
  ec_store_transaction_validate($form_state['values']);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_transaction_validate(&$edit) {
  $edit['section'] = isset($edit['section']) ? drupal_strtoupper($edit['section']) : 'ALL';

  switch ($edit['section']) {

    case 'OVERVIEW':
      ec_store_transaction_validate_overview($edit);
      break;

    case 'ADDRESSES':
      ec_store_transaction_validate_addresses($edit);
      break;

    case 'ITEMS':
      ec_store_transaction_validate_item($edit);
      break;

    default:
      ec_store_transaction_validate_overview($edit);
      ec_store_transaction_validate_addresses($edit);
      ec_store_transaction_validate_item($edit);
  }

  ec_store_invoke_ec_transaction($edit, 'validate', drupal_strtoupper($edit['section']));

  return ((form_get_errors()) ? FALSE : TRUE);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_transaction_form_submit(&$form, &$form_state) {
  if ($form_state['values']['op'] == t('Delete')) {
    $dest = drupal_get_destination();
    unset($_GET['destination'], $_REQUEST['edit']['destination']);
    $form_state['redirect'] = array('admin/store/transaction/delete/' . $form_state['values']['txnid'], $dest);
  }
  else {
    ec_store_transaction_save((object) $form_state['values']);
    $form_state['redirect'] = 'admin/store/transaction';
  }
}

// -------------------------------------------------------------------------
// Transaction items form

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_transaction_edit_items($txnid, $nid = NULL) {
  if (!is_numeric($txnid) || !($t = ec_store_transaction_load($txnid))) {
    return drupal_not_found();
  }

  if (is_null($nid) || !is_numeric($nid)) {
    return drupal_get_form('ec_store_transaction_add_items_form', $t);
  }
  else {
    $p = NULL;
    foreach (array_keys($t->items) as $k) {
      $p = & $t->items[$k];
      if ($p->nid == $nid) {
        break;
      }
    }

    if (is_null($p)) {
      return drupal_not_found();
    }

    return drupal_get_form('ec_store_transaction_edit_items_form', $t, $p);
  }
}

/**
 * Displays the form for editing a product already attached to a transaction.
 *
 * @todo: add hooks to this form for recurring etc
 *
 * @param $t
 *   Transaction object.
 * @param $p
 *   Product node object.
 *
 * @return
 *   HTML form for editing the given product as part of the given transaction.
 */
function ec_store_transaction_edit_items_form($form, &$t, &$p) {
  $form['txnid'] = array(
    '#type' => 'value',
    '#value' => $t->txnid,
  );
  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $p->nid,
  );
  $form['vid'] = array(
    '#type' => 'value',
    '#value' => $p->vid,
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $p->title,
    '#size' => 30,
    '#maxlength' => 128,
  );
  $form['qty'] = array(
    '#type' => 'textfield',
    '#title' => t('Quantity'),
    '#default_value' => $p->qty,
    '#size' => 15,
    '#maxlength' => 15,
  );
  $form['data'] = array(
    '#type' => 'hidden',
    '#value' => $p->data,
  );

  $interval = drupal_map_assoc(range(0, 31));
  $interval[0] = '--';
  $unit = array(
    0 => t('-please choose-'),
    'D' => t('Day(s)'),
    'W' => t('Week(s)'),
    'M' => t('Month(s)'),
    'Y' => t('Year(s)'),
  );

  $form[] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#weight' => 2,
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_transaction_edit_items_form_submit(&$form, &$form_state) {
  product_transaction_save_product((object) $form_state['values']);

  // Deleting the cache.
  ec_store_transaction_cache($form_state['values']['txnid'], NULL, TRUE);

  // Fix up Gross.
  $txn = ec_store_transaction_load($form_state['values']['txnid']);
  $txn->gross = ec_store_transaction_calc_gross($txn);
  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query('UPDATE {ec_transaction} SET gross = %f WHERE txnid = %d', $t->gross, $t->txnid) */
  db_update('ec_transaction')
  ->fields(array(
    'gross' => $t->gross,
  ))
  ->condition('txnid', $t->txnid)
  ->execute();
}


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_transaction_items($txnid) {
  $html = ec_store_quicklist_header();

  $header = array(
    array(
      'data' => t('nid'),
      'field' => 'p.nid',
    ),
    array(
      'data' => t('sku'),
      'field' => 'p.sku',
    ),
    array(
      'data' => t('title'),
      'field' => 'stp.title',
      'sort' => 'asc',
    ),
    array(
      'data' => t('qty'),
      'field' => 'stp.qty',
    ),
    array(
      'data' => t('type'),
      'field' => 'p.ptype',
    ),
  );

  $query = db_select('ec_transaction_product', 'stp')
    ->fields()
    ->join('ec_product', 'p', 'stp.vid = p.vid')
    ->extend('PagerDefault');

  $num_items = 0;
  foreach ($query as $data) {
    ++$num_items;
    $qty += $data->qty;

    $rows[] = array($data->nid, check_plain($data->sku), l($data->title, "node/$data->nid", array('target' => '_blank')), $data->qty, $data->ptype);
  }

  $r[] = array(
    array(
      'data' => 'Unique items:',
      'align' => 'right',
    ),
    array('data' => "<strong>$num_items</strong>"),
  );
  $r[] = array(
    array(
      'data' => 'Total quantity:',
      'align' => 'right',
    ),
    array('data' => "<strong>$qty</strong>"),
  );

  $table = theme('table', array('header' => NULL, 'rows' => $r, 'attributes' => array('border' => 0)));

  $rows[] = array(array(
      'data' => $table,
      'colspan' => 5,
      'align' => 'right',
    ));

  $pager = theme('pager', array('tags' => NULL, 'element' => 0));

  if (!empty($pager)) {
    $rows[] = array(array(
        'data' => $pager,
        'colspan' => 5,
      ));
  }

  echo $html;
  if (!empty($rows)) {
    echo theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    echo '<p>' . t('There are no transactions to view. You can manually <a href="!add_transaction">add transactions</a>, however.', array('!add_transaction' => 'admin/store/transaction/add')) . '</p>';
  }
  echo '</body></html>';
}

// -------------------------------------------------------------------------
// Transaction delete form

/**
 * Confirm users wants to delete transaction using confirm_form().
 *
 * @param $txnid
 *   Transaction ID of transaction to be deleted.
 */
function ec_store_transaction_confirm_delete($txnid) {
  $form['txnid'] = array(
    '#type' => 'value',
    '#value' => $txnid,
  );
  return confirm_form(
    $form,
    t('Are you sure you want to delete this transaction?'),
    'admin/store/transaction',
    t('Are you sure you want to delete transaction #%txnid?  This cannot be undone.',
    array('%txnid' => $txnid)),
    t('Confirm delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for ec_store_transaction_confirm_delete(). Performs delete of
 * transaction using ec_store_transaction_delete().
 */
function ec_store_transaction_confirm_delete_submit(&$form, &$form_state) {
  ec_store_transaction_delete($form_state['values']['txnid']);
  $form_state['redirect'] = 'admin/store/transaction';
}

/**
 * Delete a transaction from the database and trigger hook_ec_transaction_delete().
 *
 * @param $txnid
 *   Transaction id of the transaction to delete.
 */
function ec_store_transaction_delete($txnid) {
  if ($txn = ec_store_transaction_load($txnid)) {
    ec_store_invoke_ec_transaction($txn, 'delete');
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query('DELETE FROM {ec_transaction} WHERE txnid = %d', $txnid) */
    db_delete('ec_transaction')
  ->condition('txnid', $txnid)
  ->execute();

    // Purge the transactions cache so that the the next load won't find this
    // transactions that hase been loaded.
    ec_store_transaction_cache($txnid, NULL, TRUE);
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_transaction_add_items_form($form, &$txn) {
  $items = ec_store_transaction_item_list($txn);

  if (empty($items)) {
    $items = t('No items found for the transaction');
  }
  $form['items'] = array(
    '#type' => 'item',
    '#title' => t('Transaction items'),
    '#value' => $items,
  );
  $form['txnid'] = array(
    '#type' => 'value',
    '#value' => $txn->txnid,
  );
  $form['products'] = array(
    '#type' => 'fieldset',
    '#title' => t('Products'),
  );
  $form['products']['nids'] = array(
    '#type' => 'textfield',
    '#title' => t('Product IDs'),
    '#default_value' => $txn->nids,
    '#autocomplete_path' => 'product/autocomplete/all',
    '#size' => 60,
    '#maxlength' => 128,
    '#description' => t('Enter a comma separated list of product ids to add to this transaction. Here is a <a href="!product_quicklist" onclick="window.open(this.href, \'!product_quicklist\', \'width=480,height=480,scrollbars=yes,status=yes\'); return FALSE">list of all products</a>.', array('!product_quicklist' => url('admin/store/products/quicklist'))),
  );
  $form['products'][] = array(
    '#type' => 'submit',
    '#value' => t('Add products'),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_transaction_item_list(&$txn) {
  $rows = array();
  if (!empty($txn->items)) {
    foreach (array_keys($txn->items) as $key) {
      $p = & $txn->items[$key];
      $rows[] = array(
        $p->title,
        l(t('edit'), "admin/store/transaction/product/edit/$txn->txnid/$p->nid", array(), drupal_get_destination()),
      );
    }
  }

  if (!empty($rows)) {
    return theme('table', array('header' => array('product', 'op'), 'rows' => $rows));
  }

  return '';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_transaction_add_items_form_validate(&$form, &$form_state) {
  foreach (split(', ?', $form_state['values']['nids']) as $nid) {
    if (!is_numeric($nid) || !db_query('SELECT nid FROM {ec_product}
        WHERE nid = :nid', array(':nid' => $nid))->fetchField()) {
      form_set_error('nids', t('Invalid list of product IDs'));
      break;
    }
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_transaction_add_items_form_submit(&$form, &$form_state) {
  $form_state['values']['nids'] = drupal_str_replace(' ', '', $form_state['values']['nids']);
  ec_store_transaction_save_nids($form_values);
}

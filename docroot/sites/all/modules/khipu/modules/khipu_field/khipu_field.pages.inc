<?php

/**
 * @file
 * Page callbacks.
 */


/**
 * Lists all orders made on the site.
 */
function khipu_field_orders_page() {
  $orders = khipu_field_get_all_orders();
  $rows = array();
  foreach ($orders as $order) {
    $rows[] = array(
      $order->koid,
      $order->amount,
      format_date($order->created),
      khipu_field_status_name($order->status),
    );
  }
  $header = array(
    t('ID'),
    t('Amount'),
    t('Created'),
    t('Status'),
  );
  $variables = array(
    'rows' => $rows,
    'header' => $header,
    'empty' => t('No orders'),
  );
  $form = drupal_get_form('khipu_field_orders_page_form');
  $output = drupal_render($form);
  $output .= theme('table', $variables);
  $output .= theme('pager', array());

  return $output;
}


/**
 * Form to filter the orders by statuses.
 */
function khipu_field_orders_page_form($form, &$form_state) {
  $statuses = array(
    'all' => t('- All -'),
  );
  $statuses += khipu_field_get_statuses();
  $form['status'] = array(
    '#type' => 'select',
    '#title' => t('State'),
    '#options' => $statuses,
    '#default_value' => isset($_GET['status']) ? $_GET['status'] : 'all',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );
  return $form;
}


/**
 * Form submission handler for khipu_field_orders_page_form().
 */
function khipu_field_orders_page_form_submit($form, &$form_state) {
  $query = array();

  if (is_numeric($form_state['values']['status'])) {
    $query['status'] = $form_state['values']['status'];
  }
  $form_state['redirect'] = array($_GET['q'], array('query' => $query));
}


/**
 * Return page from Khipu.
 *
 * @param int $koid
 *   The order ID.
 */
function khipu_field_return_success($koid) {
  $korder = khipu_field_get_order($koid);
  if ($korder->status == KHIPU_FIELD_STATUS_CREATED) {
    khipu_field_order_update_status($koid, KHIPU_FIELD_STATUS_PENDING);
    $redirect = variable_get('khipu_return_url', '<front>');
    drupal_goto($redirect);
  }

  drupal_goto('<front>');
}


/**
 * List of khipu accounts.
 *
 * @param object $account
 *   The user account object.
 */
function khipu_field_khipu_accounts_by_user($account) {
  $rows = array();
  if (isset($account->khipu_accounts)) {
    foreach ($account->khipu_accounts as $khipu_account) {
      $rows[] = array(
        $khipu_account->receiver_name,
        l(t('Received'), 'user/' . $account->uid . '/khipu_account/' . $khipu_account->kaid),
      );
    }
  }
  $header = array(
    t('Receiver Name'),
    t('Received Link'),
  );
  $variables = array(
    'rows' => $rows,
    'header' => $header,
    'empty' => t('No Khipu Accounts'),
  );

  $output = '<p>' . t('Here you can see your <a title="Manage my Khipu Accounts" href="@url">khipu accounts</a> and received from Khipu Field system.', array('@url' => url('user/' . $account->uid . '/edit/khipu_account'))) . '</p>';

  $output .= theme('table', $variables);
  return $output;
}

/**
 * Received per user.
 *
 * @param object $account
 *   The user account object.
 * @param object $khipu_account
 *   The khipu account object.
 */
function khipu_field_user_page_received($account, $khipu_account) {
  $statusses = array(KHIPU_FIELD_STATUS_PENDING, KHIPU_FIELD_STATUS_COMPLETED);
  $orders = khipu_field_get_all_orders_by_kaid($khipu_account->kaid, $statusses);
  $rows = array();
  if (!empty($orders)) {
    foreach ($orders as $order) {
      $rows[] = array(
        $order->amount,
        format_date($order->created),
        khipu_field_status_name($order->status),
      );
    }
  }
  $header = array(
    t('Amount'),
    t('Created'),
    t('Status'),
  );
  $variables = array(
    'rows' => $rows,
    'header' => $header,
    'empty' => t('No orders'),
  );

  $output = t('This lists the amounts received by the module Khipu Field.');
  $output .= theme('table', $variables);
  $output .= theme('pager', array());
  return $output;
}

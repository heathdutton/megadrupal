<?php
/**
 * @file
 * Novalnet payment method module
 * This module is used for real time processing of
 * Novalnet transaction of customers.
 *
 * Copyright (c) Novalnet AG
 *
 * Released under the GNU General Public License
 * This free contribution made by request.
 * If you have found this script useful a small
 * recommendation as well as a comment on merchant form
 * would be greatly appreciated.
 *
 * Script : commerce_novalnet.views_default.inc
 *
 */
function commerce_novalnet_views_default_views() {
$view = new view();
$view->name = 'novalnet_orders';
$view->description = 'Display a list of orders for the store admin.';
$view->tag = 'commerce';
$view->base_table = 'commerce_order';
$view->human_name = 'Novalnet orders';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = 'Novalnet Orders';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'view any commerce_order entity';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['distinct'] = TRUE;
$handler->display->display_options['query']['options']['pure_distinct'] = TRUE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = 50;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'order_number' => 'order_number',
  'changed' => 'changed',
  'commerce_customer_address' => 'commerce_customer_address',
  'name' => 'name',
  'commerce_order_total' => 'commerce_order_total',
  'status' => 'status',
  'operations' => 'operations',
);
$handler->display->display_options['style_options']['default'] = 'changed';
$handler->display->display_options['style_options']['info'] = array(
  'order_number' => array(
    'sortable' => 0,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'changed' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'commerce_customer_address' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'name' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'commerce_order_total' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'status' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'operations' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['empty_table'] = TRUE;
/* Header: Global: View order form */
$handler->display->display_options['header']['view_order_form']['id'] = 'view_order_form';
$handler->display->display_options['header']['view_order_form']['table'] = 'views';
$handler->display->display_options['header']['view_order_form']['field'] = 'view_order_form';
/* No results behavior: Commerce Order: Empty text */
$handler->display->display_options['empty']['empty_text']['id'] = 'empty_text';
$handler->display->display_options['empty']['empty_text']['table'] = 'commerce_order';
$handler->display->display_options['empty']['empty_text']['field'] = 'empty_text';
$handler->display->display_options['empty']['empty_text']['add_path'] = 'admin/commerce/orders/add';
/* Relationship: Commerce Order: Owner */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'commerce_order';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
/* Relationship: Commerce Order: Referenced customer profile */
$handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['id'] = 'commerce_customer_billing_profile_id';
$handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['table'] = 'field_data_commerce_customer_billing';
$handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['field'] = 'commerce_customer_billing_profile_id';
/* Relationship: Commerce Order: Payment Transaction */
$handler->display->display_options['relationships']['payment_transaction']['id'] = 'payment_transaction';
$handler->display->display_options['relationships']['payment_transaction']['table'] = 'commerce_order';
$handler->display->display_options['relationships']['payment_transaction']['field'] = 'payment_transaction';
/* Field: Commerce Order: Order number */
$handler->display->display_options['fields']['order_number']['id'] = 'order_number';
$handler->display->display_options['fields']['order_number']['table'] = 'commerce_order';
$handler->display->display_options['fields']['order_number']['field'] = 'order_number';
$handler->display->display_options['fields']['order_number']['link_to_order'] = 'admin';
/* Field: Commerce Order: Updated date */
$handler->display->display_options['fields']['changed']['id'] = 'changed';
$handler->display->display_options['fields']['changed']['table'] = 'commerce_order';
$handler->display->display_options['fields']['changed']['field'] = 'changed';
$handler->display->display_options['fields']['changed']['label'] = t('Updated');
$handler->display->display_options['fields']['changed']['date_format'] = 'medium';
/* Field: Commerce Customer profile: Address */
$handler->display->display_options['fields']['commerce_customer_address']['id'] = 'commerce_customer_address';
$handler->display->display_options['fields']['commerce_customer_address']['table'] = 'field_data_commerce_customer_address';
$handler->display->display_options['fields']['commerce_customer_address']['field'] = 'commerce_customer_address';
$handler->display->display_options['fields']['commerce_customer_address']['relationship'] = 'commerce_customer_billing_profile_id';
$handler->display->display_options['fields']['commerce_customer_address']['label'] = t('Name');
$handler->display->display_options['fields']['commerce_customer_address']['empty'] = '-';
$handler->display->display_options['fields']['commerce_customer_address']['hide_alter_empty'] = FALSE;
$handler->display->display_options['fields']['commerce_customer_address']['click_sort_column'] = 'country';
$handler->display->display_options['fields']['commerce_customer_address']['settings'] = array(
  'use_widget_handlers' => 0,
  'format_handlers' => array(
    'name-oneline' => 'name-oneline',
  ),
);
/* Field: Commerce Order: Order total */
$handler->display->display_options['fields']['commerce_order_total']['id'] = 'commerce_order_total';
$handler->display->display_options['fields']['commerce_order_total']['table'] = 'field_data_commerce_order_total';
$handler->display->display_options['fields']['commerce_order_total']['field'] = 'commerce_order_total';
$handler->display->display_options['fields']['commerce_order_total']['label'] = t('Total');
$handler->display->display_options['fields']['commerce_order_total']['click_sort_column'] = 'amount';
$handler->display->display_options['fields']['commerce_order_total']['settings'] = array(
  'calculation' => FALSE,
);
/* Field: Commerce Order: Order status */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'commerce_order';
$handler->display->display_options['fields']['status']['field'] = 'status';
/* Field: Commerce Order: Operations links */
$handler->display->display_options['fields']['operations']['id'] = 'operations';
$handler->display->display_options['fields']['operations']['table'] = 'commerce_order';
$handler->display->display_options['fields']['operations']['field'] = 'operations';
$handler->display->display_options['fields']['operations']['label'] = 'Operations';
/* Field: Commerce Order: Link */
$handler->display->display_options['fields']['view_order']['id'] = 'view_order';
$handler->display->display_options['fields']['view_order']['table'] = 'commerce_order';
$handler->display->display_options['fields']['view_order']['field'] = 'view_order';
$handler->display->display_options['fields']['view_order']['label'] = 'Novalnet Operations';
$handler->display->display_options['fields']['view_order']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['view_order']['alter']['text'] = 'Click here to do operations';
$handler->display->display_options['fields']['view_order']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['view_order']['alter']['path'] = 'admin/commerce/orders/novalnet/[order_number]/process';
$handler->display->display_options['fields']['view_order']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['view_order']['empty'] = 'test';
/* Filter criterion: Commerce Order: Order state */
$handler->display->display_options['filters']['state']['id'] = 'state';
$handler->display->display_options['filters']['state']['table'] = 'commerce_order';
$handler->display->display_options['filters']['state']['field'] = 'state';
$handler->display->display_options['filters']['state']['operator'] = 'not in';
$handler->display->display_options['filters']['state']['value'] = array(
  'cart' => 'cart',
  'checkout' => 'checkout',
);
$handler->display->display_options['filters']['state']['group'] = 1;
$handler->display->display_options['filters']['state']['expose']['label'] = 'Order state';
$handler->display->display_options['filters']['state']['expose']['use_operator'] = TRUE;
$handler->display->display_options['filters']['state']['expose']['operator'] = 'state_op';
$handler->display->display_options['filters']['state']['expose']['identifier'] = 'state';
/* Filter criterion: Commerce Payment Transaction: Payment method */
$handler->display->display_options['filters']['payment_method']['id'] = 'payment_method';
$handler->display->display_options['filters']['payment_method']['table'] = 'commerce_payment_transaction';
$handler->display->display_options['filters']['payment_method']['field'] = 'payment_method';
$handler->display->display_options['filters']['payment_method']['relationship'] = 'payment_transaction';
$handler->display->display_options['filters']['payment_method']['value'] = array(
  'commerce_novalnet_prepayment' => 'commerce_novalnet_prepayment',
  'commerce_novalnet_invoice' => 'commerce_novalnet_invoice',
  'commerce_novalnet_cc' => 'commerce_novalnet_cc',
  'commerce_novalnet_sepa' => 'commerce_novalnet_sepa',
  'commerce_novalnet_paypal' => 'commerce_novalnet_paypal',
  'commerce_novalnet_instant' => 'commerce_novalnet_instant',
  'commerce_novalnet_ideal' => 'commerce_novalnet_ideal',
  'commerce_novalnet_eps' => 'commerce_novalnet_eps',
);

/* Display: Admin page */
$handler = $view->new_display('page', 'Admin page', 'admin_page');
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Commerce Order: Order number */
$handler->display->display_options['fields']['order_number']['id'] = 'order_number';
$handler->display->display_options['fields']['order_number']['table'] = 'commerce_order';
$handler->display->display_options['fields']['order_number']['field'] = 'order_number';
$handler->display->display_options['fields']['order_number']['link_to_order'] = 'admin';
/* Field: Commerce Order: Updated date */
$handler->display->display_options['fields']['changed']['id'] = 'changed';
$handler->display->display_options['fields']['changed']['table'] = 'commerce_order';
$handler->display->display_options['fields']['changed']['field'] = 'changed';
$handler->display->display_options['fields']['changed']['label'] = t('Updated');
$handler->display->display_options['fields']['changed']['date_format'] = 'medium';
/* Field: Commerce Customer profile: Address */
$handler->display->display_options['fields']['commerce_customer_address']['id'] = 'commerce_customer_address';
$handler->display->display_options['fields']['commerce_customer_address']['table'] = 'field_data_commerce_customer_address';
$handler->display->display_options['fields']['commerce_customer_address']['field'] = 'commerce_customer_address';
$handler->display->display_options['fields']['commerce_customer_address']['relationship'] = 'commerce_customer_billing_profile_id';
$handler->display->display_options['fields']['commerce_customer_address']['label'] = t('Name');
$handler->display->display_options['fields']['commerce_customer_address']['empty'] = '-';
$handler->display->display_options['fields']['commerce_customer_address']['hide_alter_empty'] = FALSE;
$handler->display->display_options['fields']['commerce_customer_address']['click_sort_column'] = 'country';
$handler->display->display_options['fields']['commerce_customer_address']['settings'] = array(
  'use_widget_handlers' => 0,
  'format_handlers' => array(
    'name-oneline' => 'name-oneline',
  ),
);
/* Field: Commerce Order: Order total */
$handler->display->display_options['fields']['commerce_order_total']['id'] = 'commerce_order_total';
$handler->display->display_options['fields']['commerce_order_total']['table'] = 'field_data_commerce_order_total';
$handler->display->display_options['fields']['commerce_order_total']['field'] = 'commerce_order_total';
$handler->display->display_options['fields']['commerce_order_total']['label'] = t('Total');
$handler->display->display_options['fields']['commerce_order_total']['click_sort_column'] = 'amount';
$handler->display->display_options['fields']['commerce_order_total']['settings'] = array(
  'calculation' => FALSE,
);
/* Field: Commerce Order: Order status */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'commerce_order';
$handler->display->display_options['fields']['status']['field'] = 'status';
/* Field: Commerce Payment Transaction: Payment method */
$handler->display->display_options['fields']['payment_method']['id'] = 'payment_method';
$handler->display->display_options['fields']['payment_method']['table'] = 'commerce_payment_transaction';
$handler->display->display_options['fields']['payment_method']['field'] = 'payment_method';
$handler->display->display_options['fields']['payment_method']['relationship'] = 'payment_transaction';
/* Field: Commerce Payment Transaction: Remote ID */
$handler->display->display_options['fields']['remote_id']['id'] = 'remote_id';
$handler->display->display_options['fields']['remote_id']['table'] = 'commerce_payment_transaction';
$handler->display->display_options['fields']['remote_id']['field'] = 'remote_id';
$handler->display->display_options['fields']['remote_id']['relationship'] = 'payment_transaction';
$handler->display->display_options['fields']['remote_id']['label'] = t('Transaction ID');
/* Field: Commerce Order: Operations links */
$handler->display->display_options['fields']['operations']['id'] = 'operations';
$handler->display->display_options['fields']['operations']['table'] = 'commerce_order';
$handler->display->display_options['fields']['operations']['field'] = 'operations';
$handler->display->display_options['fields']['operations']['label'] = 'Operations';
$handler->display->display_options['fields']['operations']['exclude'] = TRUE;
$handler->display->display_options['fields']['operations']['add_destination'] = 0;
/* Field: Commerce Order: Link */
$handler->display->display_options['fields']['view_order']['id'] = 'view_order';
$handler->display->display_options['fields']['view_order']['table'] = 'commerce_order';
$handler->display->display_options['fields']['view_order']['field'] = 'view_order';
$handler->display->display_options['fields']['view_order']['label'] = t('Order transactions');
$handler->display->display_options['fields']['view_order']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['view_order']['alter']['text'] = t('Click here');
$handler->display->display_options['fields']['view_order']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['view_order']['alter']['path'] = 'admin/commerce/orders/novalnet/[order_number]/process';
$handler->display->display_options['fields']['view_order']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['view_order']['empty'] = 'test';
$handler->display->display_options['path'] = 'admin/commerce/orders/novalnet';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Novalnet Orders';
$handler->display->display_options['menu']['weight'] = '';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;
$handler->display->display_options['tab_options']['type'] = 'normal';
$handler->display->display_options['tab_options']['title'] = 'Orders';
$handler->display->display_options['tab_options']['description'] = 'Manage orders in the store.';
$handler->display->display_options['tab_options']['weight'] = '';
$handler->display->display_options['tab_options']['name'] = 'management';

 $views[$view->name] = $view;
  return $views;
}

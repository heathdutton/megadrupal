<?php

/**
 * @file
 * Views 2 hooks and callback registries.
 */

/**
 * Implements hook_views_data().
 */
function uc_coupon_views_data() {
  $data['uc_coupons']['table']['group'] = t('Coupon');

  $data['uc_coupons']['table']['base'] = array(
    'field' => 'cid',
    'title' => t('Coupon'),
    'help' => t("Ubercart discount coupons."),
  );

  $data['uc_coupons']['cid'] = array(
    'title' => t('ID'),
    'help' => t('The unique ID of the coupon.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['uc_coupons']['name'] = array(
    'title' => t('Name'),
    'help' => t('The name of the coupon.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['uc_coupons']['code'] = array(
    'title' => t('Code'),
    'help' => t('The coupon code, or prefix for bulk coupons.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['uc_coupons']['bulk_codes'] = array(
    'title' => t('Codes'),
    'help' => t('All codes associated with this coupon.'),
    'field' => array(
      'handler' => 'uc_coupon_handler_field_codes',
      'field' => 'cid',
    ),
  );

  $data['uc_coupons']['bulk'] = array(
    'title' => t('Bulk'),
    'help' => t('Whether the coupon is a bulk coupon with multiple suffixes.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Is a bulk coupon'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['uc_coupons']['created'] = array(
    'title' => t('Creation date'),
    'help' => t('The date the coupon was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['uc_coupons']['valid_from'] = array(
    'title' => t('Valid from'),
    'help' => t('The date the coupon becomes valid.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['uc_coupons']['valid_until'] = array(
    'title' => t('Valid until'),
    'help' => t('The date the coupon expires.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['uc_coupons']['status'] = array(
    'title' => 'Active',
    'help' => t('Whether the coupon is available to use.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Active'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['uc_coupons']['type'] = array(
    'title' => 'Type',
    'help' => t('The discount type of the coupon.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['uc_coupons']['value'] = array(
    'title' => t('Value'),
    'help' => t('The value of the coupon.'),
    'field' => array(
      'handler' => 'uc_coupon_handler_field_value',
      'click sortable' => TRUE,
      'float' => TRUE,
      'additional fields' => array(
        'type' => 'type',
      ),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['uc_coupons']['minimum_order'] = array(
    'title' => t('Minimum order total'),
    'help' => t('The minimum order total that applies to the coupon.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['uc_coupons']['max_uses'] = array(
    'title' => t('Max uses per code'),
    'help' => t('The maximum number of times this coupon can be redeemed.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['uc_coupons']['actions'] = array(
    'title' => t('Actions'),
    'help' => t('A set of actions the current user can perform on the coupon.'),
    'field' => array(
      'handler' => 'uc_coupon_handler_field_actions',
      'field' => 'cid',
      'additional fields' => array(
        'name' => 'name',
        'bulk' => 'bulk',
      ),
    ),
  );

  $data['uc_coupons']['product_type'] = array(
    'title' => t('Product classes'),
    'help' => t('Product classes for which this coupon is valid.'),
    'field' => array(
      'field' => 'cid',
      'handler' => 'uc_coupon_handler_field_product_type',
      'label' => t('Product types'),
      'additional fields' => array(
        'data' => 'data',
      ),
    ),
    'filter' => array(
      'handler' => 'uc_coupon_handler_filter_product_type',
      'label' => 'Is a product type',
    ),
    'argument' => array(
      'handler' => 'uc_coupon_handler_argument_product_type',
      'label' => 'Is a product type',
    ),
  );

  $data['uc_coupons']['bulk_number'] = array(
    'title' => t('Bulk number'),
    'help' => t('Number of bulk codes based on this coupon.'),
    'field' => array(
      'handler' => 'uc_coupon_handler_field_bulk_number',
      'field' => 'data',
    ),
  );

  $data['uc_coupons']['all_orders_count'] = array(
     'title' => t('All orders count'),
    'help' => t('The number of orders to which this coupon was applied.'),
    'field' => array(
      'handler' => 'uc_coupon_handler_field_all_orders_count',
    ),
  );

  $data['uc_coupons']['table']['join']['uc_orders'] = array(
    'left_table' => 'uc_coupons_orders',
    'left_field' => 'oid',
    'field' => 'order_id',
  );

  $data['uc_coupons']['all_orders_total'] = array(
    'title' => t('All orders total'),
    'help' => t('The total bottom-line for all orders to which this coupon was applied.'),
    'field' => array(
      'handler' => 'uc_coupon_handler_field_all_orders_total',
      'float' => TRUE,
    ),
  );

  $data['uc_coupons']['all_orders_value'] = array(
    'title' => t('All orders total value'),
    'help' => t('The total value of this coupon for all orders to which it was applied.'),
    'field' => array(
      'handler' => 'uc_coupon_handler_field_all_orders_value',
      'float' => TRUE,
    ),
  );

  $data['uc_coupons']['all_orders_gross'] = array(
    'title' => t('All orders gross total'),
    'help' => t('The pre-discount total for all orders to which this coupon was applied.'),
    'field' => array(
      'handler' => 'uc_coupon_handler_field_all_orders_gross',
      'float' => TRUE,
    ),
  );

  //
  // Table: uc_coupons_orders
  //
  $data['uc_coupons_orders']['table']['group'] = t('Coupon order');
  $data['uc_coupons_orders']['table']['join']['uc_coupons'] = array(
    'left_field' => 'cid',
    'field' => 'cid',
  );

  // Join to the uc_orders table.
  $data['uc_coupons_orders']['table']['join']['uc_orders'] = array(
    'left_field' => 'order_id',
    'field' => 'oid',
  );

  $data['uc_orders']['table']['join']['uc_coupons'] = array(
    'left_table' => 'uc_coupons_orders',
    'left_field' => 'oid',
    'field' => 'order_id',
  );

  $data['uc_coupons_orders']['code'] = array(
    'title' => t('Code'),
    'help' => t('The coupon code used for this order'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['uc_coupons_orders']['value'] = array(
    'title' => t('Value'),
    'help' => t('The value of the coupon used for this order'),
    'field' => array(
      'handler' => 'uc_order_handler_field_money_amount',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['uc_coupons_orders']['gross'] = array(
    'title' => t('Gross'),
    'help' => t('The pre-discount bottom-line for this order'),
    'field' => array(
      'handler' => 'uc_coupon_handler_field_gross',
      'float' => TRUE,
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'uc_coupon_handler_filter_gross'
    ),
  );

  $data['uc_coupons_orders']['oid'] = array(
    'title' => t('Order ID'),
    'help' => t('The order ID of this coupon order'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function uc_coupon_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'uc_coupon') . '/views',
    ),
    'handlers' => array(
      'uc_coupon_handler_field_value' => array(
        'parent' => 'views_handler_field_numeric',
      ),
      'uc_coupon_handler_field_actions' => array(
        'parent' => 'views_handler_field',
      ),
      'uc_coupon_handler_field_bulk_number' => array(
        'parent' => 'views_handler_field_numeric',
      ),
      'uc_coupon_handler_field_codes' => array(
        'parent' => 'views_handler_field_prerender_list',
      ),
      'uc_coupon_handler_filter_product_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'uc_coupon_handler_argument_product_type' => array(
        'parent' => 'views_handler_argument',
      ),
      'uc_coupon_handler_field_product_type' => array(
        'parent' => 'views_handler_field_prerender_list',
      ),
      'uc_coupon_handler_field_all_orders_count' => array(
        'parent' => 'views_handler_field_numeric',
      ),
      'uc_coupon_handler_field_all_orders_total' => array(
          'parent' => 'uc_order_handler_field_money_amount',
      ),
      'uc_coupon_handler_field_all_orders_value' => array(
          'parent' => 'uc_coupon_handler_field_all_orders_total',
      ),
      'uc_coupon_handler_field_all_orders_gross' => array(
        'parent' => 'uc_coupon_handler_field_all_orders_total',
      ),
      'uc_coupon_handler_field_gross' => array(
        'parent' => 'uc_order_handler_field_money_amount',
      ),
      'uc_coupon_handler_filter_gross' => array(
          'parent' => 'views_handler_filter_numeric',
      ),
    ),
  );
}

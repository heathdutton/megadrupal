<?php

/**
 * @file
 * This file contains the views integeration for the wishlists.
 */

/**
 * Implements hook_views_data().
 */
function uc_wishlist_views_data() {

  // Wishlist table.
  $data['uc_wishlists']['table']['group'] = t('Wishlist');

  // We can use this as a base table for Views.
  $data['uc_wishlists']['table']['base'] = array(
    'field' => 'wid',
    'title' => t('Wishlist'),
  );

  // Define the joins.
  $data['uc_wishlists']['table']['join']['users'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
    'type' => 'INNER',
  );

  // Define the fields.
  $data['uc_wishlists']['wid'] = array(
    'title' => t('Wishlist id'),
    'help' => t('Unique id for wishlist.'),
    'group' => 'Wishlist',
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'float' => FALSE,
      'click sortable' => TRUE,
    ),

   // Information for accepting a wid as a filter.
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),

    // Information for sorting on a wid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),

    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['uc_wishlists']['uid'] = array(
    'title' => t('user id'),
    'help' => t('user id who created the wishlist.'),
    'group' => 'Wishlist',
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'float' => FALSE,
      'click sortable' => TRUE,
    ),

    'filter' => array(
      'handler' => 'views_handler_filter_equality',
    ),
  );

  $data['uc_wishlists']['title'] = array(
    'title' => t('wishlist title'),
    'help' => t('Title for wishlist'),
    'group' => 'Wishlist',
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

  $data['uc_wishlists']['expiration'] = array(
    'title' => t('Expiry date'),
    'help' => t('Timestamp for when the wish list expires.'),
    'group' => 'Wishlist',
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

  $data['uc_wishlists']['address'] = array(
    'title' => t('Shipping Address'),
    'help' => t('Address for shipping items on the wish list.'),
    'group' => 'Wishlist',
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

  $data['uc_wishlists']['private'] = array(
    'title' => t('Private'),
    'help' => t('Access level of the wishlist of the users.'),
    'group' => 'Wishlist',
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'float' => FALSE,
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_equality',
    ),
  );

  // Wishlist products.
  $data['uc_wishlist_products']['table']['group'] = t('Wishlist products');

  // Define the joins.
  $data['uc_wishlist_products']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );

  $data['uc_wishlist_products']['table']['join']['uc_wishlists'] = array(
    'left_field' => 'wid',
    'field' => 'wid',
    'type' => 'INNER',
  );

  // Define the fields.
  $data['uc_wishlist_products']['wpid'] = array(
    'title' => t('Wishlist product id'),
    'help' => t('The ID of the wish list product.'),
    'group' => 'Wishlist products',
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'float' => FALSE,
      'click sortable' => TRUE,
    ),

   // Information for accepting a wid as a filter.
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),

    // Information for sorting on a wid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),

    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['uc_wishlist_products']['wid'] = array(
    'title' => t('Wishlist id'),
    'help' => t('The unique id for the wish list this product is assigned to.'),
    'group' => 'Wishlist products',
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'float' => FALSE,
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'base' => 'uc_wishlists',
      'base field' => 'wid',
      'handler' => 'views_handler_relationship',
      'label' => t('Wishlist'),
    ),
   // Information for accepting a wid as a filter.
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a wid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['uc_wishlist_products']['nid'] = array(
    'title' => t('product id'),
    'help' => t('The Product id of the product.'),
    'group' => 'Wishlist products',
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'float' => FALSE,
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'base' => 'node',
      'base field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Product'),
    ),
   // Information for accepting a wid as a filter.
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a wid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['uc_wishlist_products']['qty'] = array(
    'title' => t('Product quantity'),
    'help' => t('The quantity of this product on the wish list.'),
    'group' => 'Wishlist products',
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'float' => FALSE,
      'click sortable' => TRUE,
    ),
   // Information for accepting a wid as a filter.
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a wid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['uc_wishlist_products']['data'] = array(
    'title' => t('product data'),
    'help' => t('The data array for the product.'),
    'group' => 'Wishlist products',
    'field' => array(
      'handler' => 'views_handler_field_serialized',
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

  $data['uc_wishlist_products']['purchase'] = array(
    'title' => t('product purchase data'),
    'help' => t('An array of purchase data for the product.'),
    'group' => 'Wishlist products',
    'field' => array(
      'handler' => 'views_handler_field_serialized',
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

  $data['uc_wishlist_products']['changed'] = array(
    'title' => t('Change time for product'),
    'help' => t('The timestamp of the last change to this wish list product.'),
    'group' => 'Wishlist products',
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

  return $data;
}

<?php

/**
 * @file
 * Affiliate store views integration.
 */

/**
 * Implements hook_views_data().
 */
function affiliate_store_views_data() {
  $data = array();
  $data['affiliate_store_product']['table']['group'] = t('Affiliate store');
  // Let product show up in content and content revision view type.
  $data['affiliate_store_product']['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'node_revision' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );
  $data['affiliate_store_product']['pid'] = array(
    'title' => t('Product ID'),
    'help' => t('Product unique ID assigned by merchant.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array('handler' => 'views_handler_filter_string'),
    'sort' => array('handler' => 'views_handler_sort'),
  );
  $data['affiliate_store_product']['price'] = array(
    'title' => t('Product price'),
    'help' => t('Product price.'),
    'field' => array(
      'handler' => 'affiliate_store_handler_field_price',
      'float' => TRUE,
      'click sortable' => TRUE,
    ),
    'filter' => array('handler' => 'views_handler_filter_numeric'),
    'sort' => array('handler' => 'views_handler_sort'),
  );
  $data['affiliate_store_product']['currency'] = array(
    'title' => t('Product currency'),
    'help' => t(
      'Product price currency symbol or standard 3-letter currency code.'
    ),
    'field' => array(
      'handler' => 'affiliate_store_handler_field_currency',
      'click sortable' => TRUE,
    ),
    'filter' => array('handler' => 'views_handler_filter_string'),
    'sort' => array('handler' => 'views_handler_sort'),
  );
  $data['affiliate_store_product']['url'] = array(
    'title' => t('Product link'),
    'help' => t('Product link that leads to its landing page.'),
    'field' => array(
      'handler' => 'affiliate_store_handler_field_product_link',
    ),
  );
  $data['affiliate_store_product']['thumb_url'] = array(
    'title' => t('Product thumbnail'),
    'help' => t('Product thumbnail.'),
    'field' => array(
      'handler' => 'affiliate_store_handler_field_product_image',
    ),
  );
  $data['affiliate_store_product']['image_url'] = array(
    'title' => t('Product image'),
    'help' => t('Product image.'),
    'field' => array(
      'handler' => 'affiliate_store_handler_field_product_image',
    ),
  );
  return $data;
}

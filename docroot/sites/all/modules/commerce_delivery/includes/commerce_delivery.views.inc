<?php

/**
 * @file
 * Export Drupal Commerce deliveries to Views.
 */

/**
 * Implements hook_views_data().
 */
function commerce_delivery_views_data() {
  $data = array();

  $data['commerce_delivery']['table']['group']  = t('Commerce Delivery');

  $data['commerce_delivery']['table']['base'] = array(
    'field' => 'delivery_id',
    'title' => t('Commerce Delivery'),
    'help' => t('Delivery.'),
  );

  // Expose the order ID.
  $data['commerce_delivery']['order_id'] = array(
    'title' => t('Order ID'),
    'help' => t('The unique internal identifier of the order.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'order_number',
      'numeric' => TRUE,
      'validate type' => 'order_id',
    ),
    'relationship' => array(
       'base' => 'commerce_order',
       'field' => 'order_id',
       'handler' => 'views_handler_relationship',
       'label' => t('Order ID'),
    ),
  );

  // Expose the delivery id.
  $data['commerce_delivery']['delivery_id'] = array(
    'title' => t('Delivery id'),
    'help' => t('The delive id.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the delivery status.
  $data['commerce_delivery']['status'] = array(
    'title' => t('Delivery status'),
    'help' => t('The workflow status of the delivery.'),
    'field' => array(
      'handler' => 'commerce_delivery_handler_field_delivery_status',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'commerce_delivery_handler_filter_delivery_status',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the created and changed timestamps.
  $data['commerce_delivery']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the delivery was created.'),
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

  $data['commerce_delivery']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the order was last updated.'),
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

  // Expose links to operate on the line item delivery.
  $data['commerce_line_item']['set_delivery'] = array(
    'field' => array(
      'title' => t('Move item to delivery'),
      'help' => t('Provide links to move each line items to a different delivery.'),
      'handler' => 'commerce_delivery_handler_line_item_set_delivery',
    ),
  );
  
  /* Relate a line item to the delivery */
  //$data['field_data_delivery_ref']['table']['group'] = t('Commerce Line Item');
  $data['commerce_line_item']['delivery_ref'] = array(
    'title' => t('Delivery ID'),
    'help' => t('The uniquement internal identifier of the associated delivery.'),
    'field' => array(
      'handler' => 'commerce_delivery_handler_field_line_item_delivery',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'commerce_delivery',
      'field' => 'delivery_id',
      'relationship table' => 'field_data_delivery_ref',
      'relationship field' => 'delivery_ref_value',
      'label' => t('Delivery', array(), array('context' => 'a drupal commerce delivery')),
    )
  );

  return $data;

}


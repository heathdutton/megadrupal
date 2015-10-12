<?php

/**
 * @file
 * Expose Shipwire shipments to Views.
 */

/**
 * Implements hook_views_data().
 */
function shipwire_views_data() {
  $data = array();

  $data['shipwire_shipments']['table']['group'] = t('Shipwire shipment');
  $data['shipwire_shipments']['table']['entity type'] = 'shipwire_shipment';
  $data['shipwire_shipments']['table']['base'] = array(
    'field' => 'shipwire_shipment_id',
    'title' => t('Shipwire shipment'),
    'help' => t('Shipment for order placed in the store.'),
    'access query tag' => 'shipwire_shipment_access',
  );

  $data['shipwire_shipments']['shipwire_shipment_id'] = array(
    'title' => t('Shipment ID', array(), array('context' => 'a shipwire shipment')),
    'help' => t('The unique internal identifier of the Shipwire shipment.'),
    'field' => array(
      'handler' => 'shipwire_shipment_handler_field_shipment_id',
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
      'name field' => 'shipwire_shipment_id',
      'numeric' => TRUE,
      'validate type' => 'shipwire_shipment',
    ),
  );

  $data['shipwire_shipments']['type'] = array(
    'title' => t('Order entity type', array(), array('context' => 'a drupal order entity')),
    'help' => t('The entity type of the order.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['shipwire_shipments']['order_id'] = array(
    'title' => t('Order ID', array(), array('context' => 'a drupal store order')),
    'help' => t('The id of the order linked to the shipment.'),
    'field' => array(
      'handler' => 'shipwire_shipment_handler_field_order_id',
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
    ),
  );

  $data['shipwire_shipments']['order_prefix'] = array(
    'title' => t('Order prefix code', array(), array('context' => 'an order prefix')),
    'help' => t('A prefix used to differntiate orders from differnt sources.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['shipwire_shipments']['status'] = array(
    'title' => t('Status', array(), array('context' => 'a shipment status')),
    'help' => t('Text indicating the status of the shipment.'),
    'field' => array(
      'handler' => 'shipwire_shipment_handler_field_status',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'shipwire_shipment_handler_filter_status',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['shipwire_shipments']['shipwire_id'] = array(
    'title' => t('Shipwire ID'),
    'help' => t("The shipment ID used remotely by Shipwire."),
    'field' => array(
      'handler' => 'shipwire_shipment_handler_field_shipwire_id',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['shipwire_shipments']['shipwire_uri'] = array(
    'title' => t('Shipwire URL'),
    'help' => t('The URL to the shipment on the Shipwire site.'),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['shipwire_shipments']['shipping_method'] = array(
    'title' => t('Shipping method'),
    'help' => t('The shipping method selected by the customer.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['shipwire_shipments']['carrier_code'] = array(
    'title' => t('Carrier code'),
    'help' => t('The Shipwire carrier code indicates the shipping company their shipping method.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['shipwire_shipments']['carrier_name'] = array(
    'title' => t('Carrier name'),
    'help' => t('The full text description of the shipping company and their shipping method.'),
    'field' => array(
      'handler' => 'shipwire_shipment_handler_field_carrier_name',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['shipwire_shipments']['warehouse_code'] = array(
    'title' => t('Warehouse code'),
    'help' => t('The code indicating the warehouse that the shipment originated from.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['shipwire_shipments']['messages'] = array(
    'title' => t('Messages'),
    'help' => t('Shipment message that indicate holds or warnings.'),
    'field' => array(
      'handler' => 'shipwire_shipment_handler_field_messages',
    ),
  );

  $data['shipwire_shipments']['date_submitted'] = array(
    'title' => t('Submitted date'),
    'help' => t('The date the shipment was submitted for fulfillment.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  $data['shipwire_shipments']['date_shipped'] = array(
    'title' => t('Shipped date'),
    'help' => t('The date the shipment was shipped.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  $data['shipwire_shipments']['date_expected'] = array(
    'title' => t('Expected date'),
    'help' => t('The date the shipment is expected to be delivered.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  $data['shipwire_shipments']['date_delivered'] = array(
    'title' => t('Delivered date'),
    'help' => t('The date the shipment was delivered.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  $data['shipwire_shipments']['tracking_number'] = array(
    'title' => t('Tracking number'),
    'help' => t('The shipment tracking number.'),
    'field' => array(
      'handler' => 'shipwire_shipment_handler_field_tracking_number',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['shipwire_shipments']['tracking_uri'] = array(
    'title' => t('Tracking URL'),
    'help' => t('The shipment tracking URL.'),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['shipwire_shipments']['currency'] = array(
    'title' => t('Currency code'),
    'help' => t('The currency code used for shipiment costs.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['shipwire_shipments']['cost_total'] = array(
    'title' => t('Total cost'),
    'help' => t('The total cost incurred for this shipment.'),
    'field' => array(
      'handler' => 'shipwire_shipment_handler_field_price',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['shipwire_shipments']['cost_freight'] = array(
    'title' => t('Shipping cost'),
    'help' => t('The shipping freight cost.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'shipwire_shipment_handler_field_price',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['shipwire_shipments']['cost_handling'] = array(
    'title' => t('Handling cost'),
    'help' => t('The shipment handling cost.'),
    'field' => array(
      'handler' => 'shipwire_shipment_handler_field_price',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['shipwire_shipments']['cost_packaging'] = array(
    'title' => t('Packaging cost'),
    'help' => t('The shipment packaging cost.'),
    'field' => array(
      'handler' => 'shipwire_shipment_handler_field_price',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['shipwire_shipments']['cost_insurance'] = array(
    'title' => t('Insurance cost'),
    'help' => t('The shipment insurance cost.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'shipwire_shipment_handler_field_price',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['shipwire_shipments']['manually_edited'] = array(
    'title' => t('Manually edited'),
    'help' => t('Indicates if teh shipment was manually edited in  Shipwire.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'type' => 'yes-no',
      'use equal' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_boolean',
    ),
  );

  $data['shipwire_shipments']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the order was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  $data['shipwire_shipments']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the order was last updated.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  // Create links to CRUD shipments.
  $data['shipwire_shipments']['view_shipment'] = array(
    'field' => array(
      'title' => t('View link'),
      'help' => t('Provide a link to the administrator view of the shipment.'),
      'handler' => 'shipwire_shipment_handler_field_link_view',
    ),
  );

  $data['shipwire_shipments']['edit_shipment'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the shipment.'),
      'handler' => 'shipwire_shipment_handler_field_link_edit',
    ),
  );

  $data['shipwire_shipments']['delete_shipment'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the shipment.'),
      'handler' => 'shipwire_shipment_handler_field_link_delete',
      'additional fields' => 'shipwire_shipment_id',
    ),
  );

  $data['shipwire_shipments']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all the available operations links for the shipment.'),
      'handler' => 'shipwire_shipment_handler_field_link_operations',
    ),
  );

  return $data;
}

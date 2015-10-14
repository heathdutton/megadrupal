<?php

/**
 * Defines function signatures for resources available to services.
 */
function commerce_drupalgap_services_resources() {
  $resources = array();

  // Define a resource for completing
  $resources['checkout_complete'] = array(
    '#api_version' => 3002,
    'operations' => array(
      'create' => array(
        'help' => 'Completes the checkout process for an order.',
        'callback' => '_commerce_drupalgap_resource_checkout_complete',
        'file' => array('type' => 'inc', 'module' => 'commerce_drupalgap', 'name' => 'resources/checkout_complete'),
        'access callback' => '_commerce_drupalgap_resource_checkout_complete_access',
        'args' => array(
          // The data array must include at least an order_id and type or it
          // will result in an error. Individual line item types may require
          // specific field data to be submitted as well, such as a
          // commerce_product value for product line items. If left empty, the
          // quantity is assumed to be 1.
          array(
            'name' => 'data',
            'type' => 'array',
            'description' => 'The order data to get the order id',
            'optional' => FALSE,
            'source' => 'data',
          ),
          array(
            'name' => 'flatten_fields',
            'type' => 'string',
            'description' => 'A string indicating whether or not field value arrays are flattened; accepts true or false.',
            'optional' => TRUE,
            'source' => array('param' => 'flatten_fields'),
            'default value' => 'true',
          ),
        ),
      ), 
    ),
  );
  return $resources;
}

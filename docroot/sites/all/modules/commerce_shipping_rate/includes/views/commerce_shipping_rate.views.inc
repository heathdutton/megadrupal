<?php

/**
 * @file
 * Export rate to views.
 */

/**
 * Implements hook_views_data().
 */
function commerce_shipping_rate_views_data() {
  $data = array();

  $data['commerce_shipping_rate']['table']['group'] = t('Commerce shipping rate');

  $data['commerce_shipping_rate']['table']['base'] = array(
    'field' => 'rate_id',
    'title' => t('Commerce shipping rate'),
    'help' => t('A Shipping rate relative to a shipping service.'),
    'access query tag' => 'commerce_entity_access',
  );
  $data['commerce_shipping_rate']['table']['entity type'] = 'commerce_shipping_rate';

  // Expose the rate_id column.
  $data['commerce_shipping_rate']['rate_id'] = array(
    'title' => t('Rate ID', array(), array('context' => 'a Shipping rate relative to a shipping service.')),
    'help' => t('Primary identifier for a rate.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
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

  // Expose the name column.
  $data['commerce_shipping_rate']['name'] = array(
    'title' => t('Shipping service name', array(), array('context' => 'a name relative to the shipping service.')),
    'help' => t('The shipping service name'),
    'field' => array(
      'handler' => 'commerce_shipping_rate_handler_field_service_name',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'commerce_shipping_rate_handler_filter_service_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the method column.
  $data['commerce_shipping_rate']['method'] = array(
    'title' => t('Shipping method name', array(), array('context' => 'a name relative to the shipping method.')),
    'help' => t('The shipping method name'),
    'field' => array(
      'handler' => 'commerce_shipping_rate_handler_field_method_id',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'commerce_shipping_rate_handler_filter_method_id',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  // Expose the type column.
  $data['commerce_shipping_rate']['type'] = array(
    'title' => t('Type'),
    'help' => t('The type of a commerce shipping rate.'),
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

  // Expose the created and changed timestamps.
  $data['commerce_shipping_rate']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the rate was created.'),
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

  $data['commerce_shipping_rate']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the rate was last updated.'),
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

  $data['commerce_shipping_rate']['edit_rate'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit a rate.'),
      'handler' => 'commerce_shipping_rate_handler_field_rate_link_edit',
    ),
  );

  $data['commerce_shipping_rate']['delete_rate'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete a rate.'),
      'handler' => 'commerce_shipping_rate_handler_field_rate_link_delete',
    ),
  );

  $data['commerce_shipping_rate']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all the available operations links for a rate.'),
      'handler' => 'commerce_shipping_rate_handler_field_rate_operations',
    ),
  );

  return $data;
}

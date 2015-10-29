<?php

/**
 * @file
 * Views integration for ec_customer.
 */

/**
 * Implements hook_views_data().
 */
function ec_customer_views_data() {
  $data = array();

  $data['ec_customer']['table']['group'] = t('Customer');

  $data['ec_customer']['table']['base'] = array(
    'field' => 'ecid',
    'title' => t('Customer id'),
    'help' => t('A unique identifier for each customer no matter what the type'),
  );

  $data['ec_customer']['table']['join'] = array(
    'ec_transaction' => array(
      'left_field' => 'ecid',
      'field' => 'ecid',
      'type' => 'INNER',
    ),
    'ec_receipt' => array(
      'left_field' => 'ecid',
      'field' => 'ecid',
      'type' => 'INNER',
    ),
    'ec_donate' => array(
      'left_field' => 'ecid',
      'field' => 'ecid',
      'type' => 'INNER',
    ),
  );

  $data['ec_customer']['ecid'] = array(
    'title' => t('Customer'),
    'help' => t('Provide the ability to filter by customer'),
    'argument' => array(
      'handler' => 'ec_customer_views_handler_argument_user_uid',
    ),
  );
  $data['ec_customer']['uid'] = array(
    'title' => t('User Id'),
    'help' => t('Provide the ability to filter by user'),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'label' => 'User',
    ),
  );
  $data['ec_customer']['customer_name'] = array(
    'field' => array(
      'title' => t('Customer name'),
      'help' => t('The name of the customer'),
      'handler' => 'ec_customer_views_handler_field_customer_name',
    ),
  );
  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function ec_customer_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'ec_customer') . '/views',
    ),
    'handlers' => array(
      'ec_customer_views_handler_argument_user_uid' => array(
        'parent' => 'views_handler_argument_user_uid',
      ),
      'ec_customer_views_handler_field_customer_name' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_customer_views_plugin_access_customer' => array(
        'parent' => 'views_plugin_access',
      ),
    ),
  );
}

/**
 * Implements hook_views_plugins().
 */
function ec_customer_views_plugins() {
  return array(
    'access' => array(
      'ec_customer' => array(
        'title' => t('Customer'),
        'help' => t('Will be available to only the customer or the store administrators'),
        'handler' => 'ec_customer_views_plugin_access_customer',
        // 'help topic' => 'access-customer',
        'path' => drupal_get_path('module', 'ec_customer') . '/views',
      ),
    ),
  );
}


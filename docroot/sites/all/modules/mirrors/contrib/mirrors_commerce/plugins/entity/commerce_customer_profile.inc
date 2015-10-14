<?php

/**
 * @file
 * Mirrors entity plugin declaration.
 */

/**
 * Entity Declaration: Commerce Customer Profile.
 */
$plugin = array(
  'title' => t('Commerce Customer Profile'),
  'category' => 'Entity',
  'views' => array(
    'base_table' => 'commerce_customer_profile',
    'relationships' => array(
      'uid' => 'commerce_customer_profile',
    ),
  ),
  'feeds' => array(
    'processor' => 'FeedsCommerceCustomerProfileProcessor',
  ),
  'properties' => array(
    'uid' => array(
      'type' => 'text',
      'views' => array(
        'table' => 'users',
        'relationship' => 'uid',
        'link_to_user' => FALSE,
      ),
    ),
    'type' => array(
      'type' => 'text',
    ),
    'status' => array(
      'type' => 'boolean',
    ),
  ),
);

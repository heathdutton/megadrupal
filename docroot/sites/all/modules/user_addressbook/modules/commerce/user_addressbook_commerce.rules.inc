<?php

/**
 * @file
 * Rules integration for the checkout process.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_event_info().
 */
function user_addressbook_commerce_rules_event_info() {
  $events = array();

  $events['user_addressbook_commerce_user_address_select'] = array(
    'label' => t('Select an user address to be used as Customer profile'),
    'group' => t('Commerce Checkout'),
    'variables' => array(
      'user_address' => array(
        'type' => 'user_address',
        'label' => t('User address', array(), array('context' => 'an user address entity')),
      ),
      'commerce_customer_profile' => array(
        'type' => 'commerce_customer_profile',
        'label' => t('Commerce Customer profile', array(), array('context' => 'a drupal commerce customer profile')),
      ),
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Completed order', array(), array('context' => 'a drupal commerce order')),
      ),
    ),
    'access callback' => 'commerce_order_rules_access',
  );

  $events['user_addressbook_commerce_add_new_customer_profile'] = array(
    'label' => t('Add new Customer profile on User addressbook checkout pane.'),
    'group' => t('Commerce Checkout'),
    'variables' => array(
      'user_address' => array(
        'type' => 'user_address',
        'label' => t('User address', array(), array('context' => 'an user address entity')),
      ),
      'commerce_customer_profile' => array(
        'type' => 'commerce_customer_profile',
        'label' => t('Commerce Customer profile', array(), array('context' => 'a drupal commerce customer profile')),
      ),
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Completed order', array(), array('context' => 'a drupal commerce order')),
      ),
    ),
    'access callback' => 'commerce_order_rules_access',
  );

  return $events;
}

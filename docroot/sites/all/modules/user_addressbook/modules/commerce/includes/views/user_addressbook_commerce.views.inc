<?php

/**
 * Export User addressbook commerce to Views.
 */

/**
 * Implements hook_views_data_alter()
 */
function user_addressbook_commerce_views_data_alter(&$data) {

  $data['user_address']['user_address_commerce_checkout_select_user_address'] = array(
    'title' => t('Checkout select user address button'),
    'help' => t('Select the user address to be used in the checkout.'),
    'field' => array(
      'handler' => 'user_addressbook_commerce_field_handler_checkout_select_user_address',
    ),
  );
}

<?php

/**
 * @file
 * Provide Views support
 */

/**
 * Implements hook_views_data().
 */
function ms_affiliates_views_data() {
  $data = array();

  // PayPal Email
  $data['users']['ms_affiliates_paypal_email'] = array(
    'title' => 'Affiliate PayPal Email',
    'help' => 'Affiliate PayPal Email',
    'field' => array(
        'handler' => 'ms_affiliates_paypal_handler',
        'click sortable' => TRUE,
        'field' => 'uid',
    ),
  );

  // Affiliate Balance
  $data['users']['ms_affiliates_balance'] = array(
    'title' => 'Affiliate Balance',
    'help' => 'Affiliate Balance',
    'field' => array(
        'handler' => 'ms_affiliates_balance_handler',
        'click sortable' => TRUE,
        'field' => 'uid',
    ),
  );

  // Affiliate Address
  $data['users']['ms_affiliates_address'] = array(
    'title' => 'Affiliate Address',
    'help' => 'Affiliate Mailing Address',
    'field' => array(
        'handler' => 'ms_affiliates_address_handler',
        'click sortable' => TRUE,
        'field' => 'uid',
    ),
  );

  // Referrer
  $data['users']['ms_affiliates_referrer'] = array(
    'title' => 'Affiliate Referrer',
    'help' => 'The username of the person who referred this user',
    'field' => array(
        'handler' => 'ms_affiliates_referrer_handler',
        'click sortable' => TRUE,
        'field' => 'uid',
    ),
  );

  return $data;
}


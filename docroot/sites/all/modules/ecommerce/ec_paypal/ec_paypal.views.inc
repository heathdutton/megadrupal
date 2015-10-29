<?php

/**
 * @file
 * Additional views fields for ec_paypal module.
 */

/**
 * Implements hook_views_data().
 */
function ec_paypal_views_data() {
  $data = array();

  $data['ec_receipt']['paypal_payer_email'] = array(
    'title' => t('PP: Payer email hash'),
    'help' => t('A hash of the customers PayPal email which they used to pay this money. This is a has to protect the privacy of the customer.'),
    'field' => array(
      'field' => 'paypal_payer_email',
      'click sortable' => TRUE,
    ),
  );

  $data['ec_receipt']['paypal_subscr_id'] = array(
    'title' => t('PP: Subscription Id'),
    'help' => t('If the payment is a part of a subscription, there will be a subscription id.'),
    'field' => array(
      'field' => 'paypal_subscr_id',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  return $data;
}

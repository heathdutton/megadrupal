<?php

/**
 * @file
 * Views integration.
 */

/**
 * Implements hook_views_data().
 */
function ideal_views_data() {
  $data['ideal_payment'] = array(
    'table' => array(
      'base' => array(
        'title' => t('iDEAL payments'),
      ),
      'group' => t('iDEAL payments'),
    ),
    // Field configuration.
    'consumer_account_number' => array(
      'title' => t('Account number'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument',
      ),
    ),
    'consumer_bic' => array(
      'title' => t('BIC'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument',
      ),
    ),
    'consumer_city' => array(
      'title' => t('City'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument',
      ),
    ),
    'consumer_iban' => array(
      'title' => t('IBAN'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument',
      ),
    ),
    'consumer_name' => array(
      'title' => t('Name'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument',
      ),
    ),
    'issuer_authentication_url' => array(
      'title' => t('Issuer authentication URL'),
      'field' => array(
        'handler' => 'views_handler_field_url',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument',
      ),
    ),
    'updated' => array(
      'title' => t('Last status update'),
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
      'argument' => array(
        'handler' => 'views_handler_argument_date',
      ),
    ),
    'pid' => array(
      'title' => t('Payment ID'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'relationship' => array(
        'title' => t('Payments'),
        'base' => 'payment',
        'field' => 'pid',
        'handler' => 'views_handler_relationship',
        'label' => t('Payments'),
      ),
    ),
    'transaction_id' => array(
      'title' => t('Transaction ID'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument',
      ),
    ),
  );
  $data['ideal_payment_method'] = array(
    'table' => array(
      'base' => array(
        'title' => t('iDEAL payment methods'),
      ),
      'group' => t('iDEAL payment methods'),
    ),
    // Field configuration.
    'expiration_period' => array(
      'title' => t('Expiration period'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
    ),
    'merchant_id' => array(
      'title' => t('Merchant ID'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument',
      ),
    ),
    'merchant_sub_id' => array(
      'title' => t('Sub ID'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
    ),
    'acquirer_url' => array(
      'title' => t('Acquirer URL'),
      'field' => array(
        'handler' => 'views_handler_field_url',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument',
      ),
    ),
    'pmid' => array(
      'title' => t('Payment method ID'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'relationship' => array(
        'title' => t('Payment methods'),
        'base' => 'payment_method',
        'field' => 'pmid',
        'handler' => 'views_handler_relationship',
        'label' => t('Payment methods'),
      ),
    ),
  );

  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function ideal_views_data_alter(&$data) {
  $data['payment']['ideal_payment_pid'] = array(
    'title' => t('iDEAL payments'),
    'real field' => 'pid',
    'relationship' => array(
      'title' => t('iDEAL payments'),
      'base' => 'ideal_payment',
      'base field' => 'pid',
      'handler' => 'views_handler_relationship',
      'label' => t('iDEAL payments'),
    ),
  );
  $data['payment_method']['ideal_payment_method_pmid'] = array(
    'title' => t('iDEAL payment methods'),
    'real field' => 'pmid',
    'relationship' => array(
      'title' => t('iDEAL payment methods'),
      'base' => 'ideal_payment_method',
      'base field' => 'pmid',
      'handler' => 'views_handler_relationship',
      'label' => t('iDEAL payment methods'),
    ),
  );
}
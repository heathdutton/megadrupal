<?php
/**
 * @file
 * Rules related functions.
 */

function commerce_cib_rules_event_info() {
  $defaults = array(
      'group' => t('Commerce CIB'),
      'module' => 'commerce_cib',
      'access callback' => 'commerce_order_rules_access',
  );

  return array(
    'commerce_cib_pre_redirect' => $defaults + array(
      'label' => t('A redirect to the CIB payment site is about to happen.'),
      'variables' => array(
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('Order'),
        ),
      ),
    ),
    'commerce_cib_timeout' => $defaults + array(
      'label' => t('CIB bank transaction timeout'),
      'variables' => array(
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('Timeouted order'),
        ),
      ),
    ),
    'commerce_cib_no_communication' => $defaults + array(
      'label' => t('CIB bank transaction communication failure'),
      'variables' => array(
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('Order'),
        ),
      ),
    ),
    'commerce_cib_initialization_fails' => $defaults + array(
      'label' => t('CIB bank transaction initialization failure'),
      'variables' => array(
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('Order'),
        ),
      ),
    ),
    'commerce_cib_transaction_fails' => $defaults + array(
      'label' => t('CIB bank unsuccessful transaction'),
      'variables' => array(
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('Order'),
        ),
      ),
    ),
  );
}

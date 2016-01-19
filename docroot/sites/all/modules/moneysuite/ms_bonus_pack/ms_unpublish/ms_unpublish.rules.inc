<?php

// ======================================

// TRIGGERS

// ======================================


/*
 * Triggers:
 * + Node was unpublished
 */

/**
 * Implements hook_rules_event_info()
().
 */
function ms_unpublish_rules_event_info() {
  $items = array(
    'ms_unpublish_event_pay_unpublish' => array(
      'label' => t('Payment Received to Unpublish Node'),
      'module' => 'Pay to Unpublish',
      'help' => t("Triggers when a user pays to unpublish content."),
      'arguments' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('User Account'),
        ),
        'node' => array(
          'type' => 'node',
          'label' => t('Node'),
        ),
        'order' => array(
          'type' => 'ms_order',
          'label' => t('Order', array(), array('context' => 'ms_core')),
        ),
        'payment' => array(
          'type' => 'ms_payment',
          'label' => t('Payment'),
        ),
      ),
    ),
  );

  return $items;
}

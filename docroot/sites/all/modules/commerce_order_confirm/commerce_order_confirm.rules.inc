<?php

/**
 * Implements hook_rules_event_info().
 */
function commerce_order_confirm_rules_event_info() {
  $events = array();

  $events['commerce_order_confirmed'] = array(
    'label' => t('The order confirmed'),
    'group' => t('Commerce Order'),
    'variables' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Completed order', array(), array('context' => 'a drupal commerce order')),
      ),
    ),
    'access callback' => 'commerce_order_rules_access',
  );

  return $events;
}

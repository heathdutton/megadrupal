<?php

/**
 * Implements hook_rules_action_info().
 */
function commerce_cartmessage_rules_action_info() {
  $actions = array();

  $actions['commerce_cartmessage_setmessage_action'] = array(
    'label' => t('Show message on shopping cart'),
    'group' => t('Commerce Cart'),
    'parameter' => array(
      'message' => array(
        'type' => 'text',
        'label' => t('Message'),
        'description' => t('The message to be displayed on the cart.'),
      ),
    ),
  );

  return $actions;
}

/**
 *
 */
function commerce_cartmessage_setmessage_action($message) {
  commerce_cartmessage_setmessage($message);
}

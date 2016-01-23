<?php

/**
 * @file
 * Rules integration for the Commerce Restrict Area module.
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_restrict_area_rules_event_info() {
  $events = array();

  $events['commerce_restrict_area_validation'] = array(
    'label' => t('Check if products can be sold (at the order address)'),
    'group' => t('Commerce Restrict Area'),
    'variables' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order', array(), array('context' => 'a drupal commerce order')),
        'skip save' => TRUE,
      ),
    ),
    'access callback' => 'commerce_order_rules_access',
  );

  return $events;
}

/**
 * Implements hook_rules_condition_info().
 */
function commerce_restrict_area_rules_condition_info() {
  $conditions = array();

  $conditions['commerce_restrict_area_address_validate'] = array(
    'label' => t('Order has a valid address for restrict area validation'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order', array(), array('context' => 'a drupal commerce order')),
        'description' => t('The order containing the profile reference with the address in question.'),
      ),
    ),
    'group' => t('Commerce Restrict Area'),
    'callbacks' => array('execute' => 'commerce_restrict_area_address_validate'),
  );
  $conditions['commerce_restrict_area_user_has_bypass_permission'] = array(
    'label' => t('User has bypass permission for restrict area validation'),
    'parameter' => array(
      'account' => array('type' => 'user', 'label' => t('User')),
    ),
    'group' => t('Commerce Restrict Area'),
    'callbacks' => array('execute' => 'commerce_restrict_area_user_has_bypass_permission'),
  );

  return $conditions;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_restrict_area_rules_action_info() {
  $actions = array();

  $actions['commerce_restrict_area_remove_products'] = array(
    'label' => t('Remove products not available for that address'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order to update'),
      ),
    ),
    'group' => t('Commerce Restrict Area'),
    'callbacks' => array('execute' => 'commerce_restrict_area_remove_products'),
  );
  $actions['commerce_restrict_area_remove_products_message'] = array(
    'label' => t('Remove products not available for that address and notify it'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order to update'),
      ),
      'message' => array(
        'type' => 'text',
        'label' => t('Message'),
        'sanitize' => TRUE,
        'translatable' => TRUE,
      ),
      'type' => array(
        'type' => 'token',
        'label' => t('Message type'),
        'options list' => 'rules_action_drupal_message_types',
        'default value' => 'status',
        'optional' => TRUE,
      ),
      'repeat' => array(
        'type' => 'boolean',
        'label' => t('Repeat message'),
        'description' => t("If disabled and the message has been already shown, then the message won't be repeated."),
        'default value' => TRUE,
        'optional' => TRUE,
        'restriction' => 'input',
      ),
      'products' => array(
        'type' => 'boolean',
        'label' => t('Append products to message'),
        'description' => t("If disabled the removed products will be not append to the message."),
        'default value' => TRUE,
        'optional' => TRUE,
        'restriction' => 'input',
      ),
    ),
    'group' => t('Commerce Restrict Area'),
    'callbacks' => array('execute' => 'commerce_restrict_area_remove_products_message'),
  );
  $actions['commerce_restrict_area_message'] = array(
    'label' => t('Show a message on the site with the products not available for that address'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order to update'),
      ),
      'message' => array(
        'type' => 'text',
        'label' => t('Message'),
        'sanitize' => TRUE,
        'translatable' => TRUE,
      ),
      'type' => array(
        'type' => 'token',
        'label' => t('Message type'),
        'options list' => 'rules_action_drupal_message_types',
        'default value' => 'status',
        'optional' => TRUE,
      ),
      'repeat' => array(
        'type' => 'boolean',
        'label' => t('Repeat message'),
        'description' => t("If disabled and the message has been already shown, then the message won't be repeated."),
        'default value' => TRUE,
        'optional' => TRUE,
        'restriction' => 'input',
      ),
      'products' => array(
        'type' => 'boolean',
        'label' => t('Append products to message'),
        'description' => t("If disabled the removed products will be not append to the message."),
        'default value' => TRUE,
        'optional' => TRUE,
        'restriction' => 'input',
      ),
    ),
    'group' => t('Commerce Restrict Area'),
    'callbacks' => array('execute' => 'commerce_restrict_area_message'),
  );

  return $actions;
}

/**
 * Condition: Order has a valid address for restrict area validation.
 */
function commerce_restrict_area_address_validate($order) {
  return commerce_restrict_area_order_validate($order);
}

/**
 * Condition: User has bypass permission for restrict area validation.
 */
function commerce_restrict_area_user_has_bypass_permission($account) {
  return commerce_restrict_area_user_has_permission('bypass commerce restrict area', $account);
}

/**
 * Action: Remove products not available for that address.
 */
function commerce_restrict_area_remove_products($order) {
  commerce_restrict_area_order_validate($order, 'remove');
}

/**
 * Action: Remove products not available for that address and show a message.
 */
function commerce_restrict_area_remove_products_message($order, $message, $status, $repeat, $products) {
  if ($products) {
    $message = $message . commerce_restrict_area_products_to_message(commerce_restrict_area_order_validate($order, 'remove'));
  }
  drupal_set_message(filter_xss_admin($message), $status, $repeat);
}

/**
 * Action: Show a message on the site with products can not be sold.
 */
function commerce_restrict_area_message($order, $message, $status, $repeat, $products) {
  if ($products) {
    $message = $message . commerce_restrict_area_products_to_message(commerce_restrict_area_order_validate($order, 'validate_all'));
  }
  drupal_set_message(filter_xss_admin($message), $status, $repeat);
}

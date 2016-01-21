<?php
/**
 * @file
 * Rules definitions for Marketplace Orders.
 */

/**
 * Implements hook_rules_action_info().
 */
function mp_orders_rules_action_info() {
  // Send an e-mail regarding a coupon.
  $actions['mp_orders_seller_email'] = array(
    'label' => t('Send seller order notification'),
    'group' => t('Order'),
    'named parameters' => TRUE,
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'subject' => array(
        'type' => 'text',
        'label' => t('Subject'),
        'description' => t('Enter the subject line for the notifications.'),
        'restriction' => 'input',
      ),
      'message' => array(
        'type' => 'text',
        'label' => t('Message'),
        'description' => t('Enter the message body for the notifications.'),
        'restriction' => 'input',
      ),
      'format' => array(
        'type' => 'text',
        'label' => t('Message format'),
        'options list' => 'mp_orders_message_formats',
      ),
    ),
  );

  return $actions;
}

/**
 * Options list callback for message formats.
 */
function mp_orders_message_formats() {
  global $user;

  $options = array();
  $formats = filter_formats($user);
  foreach ($formats as $format) {
    $options[$format->format] = $format->name;
  }
  return $options;
}

/**
 * Action callback to send an e-mail notifying seller of an order.
 */
function mp_orders_seller_email($params) {
  $result = db_query("SELECT uid FROM {mp_seller_order_statuses} WHERE order_id = :order_id", array(':order_id' => $params['order']->order_id));
  foreach ($result as $seller) {
    mp_orders_send_seller_email($seller->uid, $order, $params);
  }
}

/**
 * Email helper function.
 */
function mp_orders_send_seller_email($uid, $order, $settings) {
  mp_orders_uc_order('load', $order, $uid);
  $seller = user_load($uid);
  $settings['uid'] = $uid;
  $settings['token_filters'] = array(
    'global' => NULL,
    'order' => $order,
    'user' => $seller,
  );
  $email_to = db_query("SELECT mail FROM {users} WHERE uid = :uid", array(':uid' => $uid))->fetchField();
  $language = language_default();
  $sent = drupal_mail('mp_orders', 'sellercheckout', $email_to, $language, $settings, uc_store_email_from());
  if ($sent) {
    $changes[] = t('Seller checkout message sent to @email.', array('@email' => $order->primary_email));
  }
  else {
    $changes[] = t('Seller checkout e-mail notification to @email failed.', array('@email' => $order->primary_email));
  }
  uc_order_log_changes($order->order_id, $changes);
}

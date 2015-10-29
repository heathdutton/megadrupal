<?php
/**
 * @file
 * Rules integration for uc_coupon_purchase
 */

/**
 * Implements hook_rules_event_info().
 */
function uc_coupon_purchase_rules_event_info() {
  $events = array();
  $events['uc_coupon_purchase'] = array(
    'label' => t('Customer purchases a coupon'),
    'group' => t('Coupon'),
    'variables' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),    
      ),
      'coupon' => array(
        'type' => 'uc_coupon',
        'label' => t('Coupon'),
      ),
    ),
  );
  return $events;
}


/**
 * Implements hook_rules_action_info().
 */
function uc_coupon_purchase_rules_action_info() {
  $actions = array();
  $actions['uc_coupon_purchase_email'] = array(
    'label' =>  t('Send an order email regarding coupon purchase.'),
    'group' => t('Coupon'),
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),    
        'optional' => TRUE,
      ),
      'coupon' => array(
        'type' => 'uc_coupon',
        'label' => t('Coupon'),
      ),
      'from' => array(
        'type' => 'text',
        'label' => t('Sender'),
      ),
      'addresses' => array(
        'type' => 'text',
        'label' => t('Recipients'),
        'description' => t('Enter the email addresses to receive the notifications, one on each line. You may use order tokens for dynamic email addresses.'),
      ),
      'subject' => array(
        'type' => 'text',
        'label' => t('Subject'),
      ),
      'message' => array(
        'type' => 'text',
        'label' => t('Message'),
      ),
      'format' => array(
        'type' => 'text',
        'label' => t('Message format'),
        'options list' => 'uc_coupon_purchase_message_formats',
      ),
      
    ),
  );
  $actions['uc_coupon_purchase_create_action'] = array(
    'label' =>  t('Clone coupon and assign to a user'),
    'group' => t('Coupon'),
    'provides' => array(
      'coupon' => array(
        'type' => 'uc_coupon',
        'label' => t('Coupon'),
      ),
    ),
    'parameter' => array(
      'cid' => array(
        'type' => 'integer',
        'label' => t('Coupon'),
        'description' => t('Select the base coupon to be assigned.'),
        'options list' => 'uc_coupon_purchase_cid_options',
        'restriction' => 'input',
      ),
      'user' => array(
        'type' => 'user',
        'label' => t('User to who we assing coupon'),
      ),
      'quantity' => array(
        'type' => 'integer',
        'label' => t('Quantity (multiplied by bulk coupon)'),
      ),
    ),
  );
  
  return $actions;
}

/**
 * Options list callback for message formats.
 */
function uc_coupon_purchase_message_formats() {
  global $user;

  $options = array();
  $formats = filter_formats($user);
  foreach ($formats as $format) {
    $options[$format->format] = $format->name;
  }
  return $options;
}


/**
 * Action callback to send an e-mail regarding coupon purchase.
 */
function uc_coupon_purchase_email($order, $coupon, $from, $addresses, $subject, $message, $format) {
  $settings = array(
    'from' => $from,
    'subject' => $subject,
    'message' => $message,
    'format' => $format,
    'replacements' => array(),  // Replacements are handled directly by Rules.
  );

  // Split up our recipient e-mail addresses.
  $recipients = array();
  foreach (explode("\n", $addresses) as $address) {
    $recipients[] = trim($address);
  }

  // Send the e-mails.
  foreach ($recipients as $email) {
    $sent = drupal_mail('uc_order', 'action-mail', $email, uc_store_mail_recipient_language($email), $settings, empty($settings['from']) ? uc_store_email_from() : $settings['from']);
    if (!$sent['result']) {
      watchdog('uc_order', 'Attempt to e-mail @email concerning order @order_id failed.', array('@email' => $email, '@order_id' => $order->order_id), WATCHDOG_ERROR);
    }
  }
}

/**
 * Action callback to give user a coupon
 */
function uc_coupon_purchase_create_action($cid, $user, $quantity) {
  $coupon = uc_coupon_load($cid);
  if ($coupon->cid) {
    $coupon = uc_coupon_purchase_create($coupon, $quantity, $user->uid);
    return array('coupon' => $coupon);
  }
  else {
    return array('coupon' => NULL);
  }
}

/**
 * Options list callback for list of coupons to assign.
 */
function uc_coupon_purchase_cid_options() {
  return db_query('SELECT cid, name FROM {uc_coupons} WHERE status = 0')->fetchAllKeyed();
}

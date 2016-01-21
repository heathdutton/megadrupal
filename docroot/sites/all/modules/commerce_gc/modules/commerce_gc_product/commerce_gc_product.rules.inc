<?php

/**
 * @file
 * Provides Rules module support for buying giftcards.
 */

/*
 * Implements hook_rules_action_info().
 */
function commerce_gc_product_rules_action_info() {
  $actions['commerce_gc_product_line_item_generate_giftcard'] = array(
    'label' => t('Generate giftcard coupons for a giftcard line item'),
    'group' => t('Commerce Giftcard'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
        'wrapped' => TRUE
      )
    ),
    'provides' => array(
      'giftcard_coupon' => array(
        'type' => 'commerce_coupon',
        'label' => t('Giftcard coupon'),
      )
    )
  );

  $actions['commerce_gc_product_giftcard_email_variables'] = array(
    'label' => t('Provide variables for a giftcard notification email.'),
    'group' => t('Commerce Giftcard'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
        'wrapped' => TRUE
      ),
    ),
    'provides' => array(
      'giftcard_mail' => array(
        'type' => 'token',
        'label' => t('Giftcard mail'),
      ),
    )
  );

  $actions['commerce_gc_product_attach_giftcard_to_user_recipient'] = array(
    'label' => t('Attach a giftcard to a user'),
    'group' => t('Commerce Giftcard'),
    'parameter' => array(
      'mail' => array(
        'type' => 'text',
        'label' => t('User mail'),
      ),
      'commerce_coupon' => array(
        'type' => 'commerce_coupon',
        'label' => t('Giftcard coupon'),
        'wrapped' => TRUE
      ),
      'email_type' => array(
        'type' => 'text',
        'label' => t('E-mail type'),
        'description' => t("Select the e-mail based on your site's account settings to send to the user."),
        'options list' => 'commerce_gc_product_account_email_options_list',
      ),
    ),
    'provides' => array(
      'user_recipient' => array(
        'type' => 'user',
        'label' => t('User recipient'),
      )
    )
  );

  return $actions;
}

/**
 * Returns the account e-mail types from the User module.
 *
 * @see _user_mail_notify()
 */
function commerce_gc_product_account_email_options_list() {
  return array(
    'register_admin_created' => t('Welcome (new user created by administrator)'),
    'register_no_approval_required' => t('Welcome (no approval required)'),
    'register_pending_approval' => t('Welcome (awaiting approval)'),
  );
}

/*
 * Implements hook_rules_event_info().
 */
function commerce_gc_product_rules_event_info() {
  $events['commerce_gc_product_process_giftcard_line_item'] = array(
    'label' => t('Giftcard needs to be generated for a line item'),
    'group' => t('Commerce Giftcard'),
    'variables' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
      )
    )
  );

  return $events;
}

/*
 * Rules action callback: generate giftcard coupons for a giftcard line item.
 */
function commerce_gc_product_line_item_generate_giftcard(EntityDrupalWrapper $line_item_wrapper) {
  $coupon = commerce_coupon_create('giftcard_coupon');
  $coupon_wrapper = entity_metadata_wrapper('commerce_coupon', $coupon);

  // Get the value of the giftcard product.
  $coupon_wrapper->commerce_gc_value = $line_item_wrapper->commerce_product->commerce_gc_value->value();
  // Use the product title as the giftcard name.
  $coupon_wrapper->commerce_gc_name = $line_item_wrapper->commerce_product->title->value();

  // Generate a code and save.
  $coupon->code = commerce_coupon_generate_coupon_code('giftcard_coupon');

  // Allow other modules to alter the coupon before it is saved as a giftcard.
  drupal_alter('commerce_gc_product_giftcard_coupon_presave', $coupon_wrapper, $line_item_wrapper);
  commerce_coupon_save($coupon);

  // Record a reference to this coupon on the line item.
  $line_item_wrapper->commerce_giftcards[] = $coupon;
  $line_item_wrapper->save();

  return array('giftcard_coupon' => $coupon);
}

/*
 * Rules action callback: Provides variables for giftcard notification message.
 */
function commerce_gc_product_giftcard_email_variables(EntityDrupalWrapper $line_item_wrapper) {
  $mail = '';
  // Find a suitable email.
  if (field_info_instance('commerce_line_item', 'commerce_gc_mail', $line_item_wrapper->type->value()) && $line_item_wrapper->commerce_gc_mail->value()) {
    $mail = $line_item_wrapper->commerce_gc_mail->value();
  }
  else if ($line_item_wrapper->order->value() && $line_item_wrapper->order->mail->value()) {
    $mail = $line_item_wrapper->order->mail->value();
  }
  else {
    global $user;
    if ($user->uid) {
      $mail = $user->mail;
    }
  }

  return array(
    'giftcard_mail' => $mail,
  );
}

/*
 * Rules action callback: Link a giftcard with a user.
 */
function commerce_gc_product_attach_giftcard_to_user_recipient($mail, $coupon_wrapper, $email_type) {
  $user = user_load_by_mail($mail);
  if (!$user && variable_get('commerce_gc_product_create_new_recipient_users', TRUE)) {
    // If the specified user does not yet exist, create it.
    $user = new stdClass();
    $user->mail = $mail;

    // Generate a unique non-email based username for the new user.
    $username = preg_replace('/[^\x{80}-\x{F7} a-z0-9@_.\'-]/i', '-', trim($mail));
    $username = preg_replace('/@.*$/', '', $username);
    $username = substr($username, 0, USERNAME_MAX_LENGTH);
    $user->name = commerce_order_unique_username($username);

    $user->pass = drupal_random_key();
    $user->status = 1;
    user_save($user);

    // TODO: let admins define what role(s) these users get.
    _user_mail_notify($email_type, $user);

    // Allow other modules to alter the user before it is saved.
    drupal_alter('commerce_gc_product_new_recipient_user', $user, $coupon);
  }

  if ($user) {
    // Save the coupon with a reference to this user.
    $coupon_wrapper->commerce_coupon_recipient = $user->uid;
    $coupon_wrapper->save();

    return array('user_recipient' => $user);
  }
}

<?php

/**
 * @file
 * Add "Your phone" field to popup with hooks, validate this field and finally
 * save the field to "field_phone" field of user billing profile.
 * Save "Your name" fileld to "field_name" of the billing profile.
 * Define value of [commerce-order:commerce-buy-one-click-phone] token.
 */

/**
 * Implements hook_commerce_buy_one_click_add_to_cart_form_prefix_alter().
 */
function hook_commerce_buy_one_click_add_to_cart_form_prefix_alter(&$prefix_html, $form, $form_state) {
  // @todo Add example code.
}

/**
 * Implements hook_commerce_buy_one_click_form_alter().
 */
function hook_commerce_buy_one_click_form_alter(&$form) {
  $form['form_wrapper']['commerce_buy_one_click_phone'] = array(
    '#title' => t('Your phone'),
    '#type' => 'textfield',
    '#default_value' => '',
    '#weight' => 15,
    '#required' => TRUE,
  );
}

/**
 * Implements hook_commerce_buy_one_click_admin_email_data_alter().
 */
function hook_commerce_buy_one_click_admin_email_data_alter(&$admin_email_data, $form, $form_state) {
  $admin_email_data['commerce_buy_one_click']['commerce-buy-one-click-phone'] = trim($form_state['values']['commerce_buy_one_click_phone']);
}

/**
 * Implements hook_commerce_buy_one_click_form_validate_alter().
 */
function hook_commerce_buy_one_click_form_validate_alter(&$errors, $form, $form_state) {
  if (empty($form_state['values']['commerce_buy_one_click_phone'])) {
    $errors = t('Error: phone must be specified.');
  }
}

/**
 * Implements hook_commerce_buy_one_click_form_submit_alter().
 */
function hook_commerce_buy_one_click_form_submit_alter($order, $order_user, $form, $form_state) {
  $phone = trim($form_state['values']['commerce_buy_one_click_phone']);
  $name = t('Buy one click');
  if (!empty($form_state['values']['commerce_buy_one_click_name'])) {
    $name = $form_state['values']['commerce_buy_one_click_name'];
  }
  if (!empty($phone) || !empty($name)) {
    $phone = trim($phone);

    // Update billing profile with user data.
    if (empty($order->commerce_customer_billing)) {
      $profile = commerce_customer_profile_new('billing', $order_user->uid);
      _commerce_buy_one_click_phone_update_billing_profile($profile, $phone, $name);
      $profile_object = array(
        'und' => array(
          array(
            'profile_id' => $profile->profile_id,
          ),
        ),
      );
      $order->commerce_customer_billing = $profile_object;
      commerce_order_save($order);
    }
    else {
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
      $profile = commerce_customer_profile_load($order_wrapper->commerce_customer_billing->profile_id->value());
      _commerce_buy_one_click_phone_update_billing_profile($profile, $phone, $name);
    }
  }
}

/**
 * Update billing profile function.
 */
function _commerce_buy_one_click_phone_update_billing_profile(&$profile, $phone, $name) {
  $profile_wrapper = entity_metadata_wrapper('commerce_customer_profile', $profile);
  $profile_wrapper->field_phone = $phone;
  $profile_wrapper->field_name = $name;
  commerce_customer_profile_save($profile);
}

/**
 * Implements hook_commerce_buy_one_click_order_completed_rules_args_alter().
 */
function hook_commerce_buy_one_click_order_completed_rules_args_alter(&$order_completed_rules_args) {
  // @todo Add example code.
}

/**
 * Implements hook_commerce_buy_one_click_order_completed_final_message_alter().
 */
function hook_commerce_buy_one_click_order_completed_final_message_alter(&$final_message, $order, $order_user, $form_state) {
  $final_message = 'My custom final message!';
}

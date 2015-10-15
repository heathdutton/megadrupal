<?php

/**
 * @file
 * Various payment related pages, forms and functions.
 *
 * Copyright 2014 Membersify.com
 */

/**
 * Page callback: Payment page where the user can purchase a plan.
 *
 * @param object $plan
 *   The plan object.
 *
 * @see membersify_menu()
 */
function membersify_purchase_plan($plan) {
  drupal_set_title(t('Payment'));
  $build = array();

  global $user;
  $account = FALSE;
  if ($user->uid) {
    $account = user_load($user->uid);
  }
  if (!empty($_SESSION['membersify_user_id'])) {
    $account = user_load($_SESSION['membersify_user_id']);
  }

  if ($account) {
    // Show the order summary table.
    $items = array();
    $items[] = array(
      'name' => $plan->name,
      'description' => $plan->description,
      'price_string' => membersify_get_price_string($plan->payment_plan),
      'payment_plan' => $plan->payment_plan,
    );

    $adjustment_ids = !empty($_SESSION['membersify_adjustments'][$plan->id]) ? $_SESSION['membersify_adjustments'][$plan->id] : array();
    $adjustments = array();
    foreach ($adjustment_ids as $adjustment_id => $active) {
      if ($active && $adjustment = membersify_adjustment_load($adjustment_id)) {
        $adjustments[] = $adjustment;
      }
    }
    $total = membersify_get_total($items, $adjustments);
    $coupon_widget = drupal_get_form('membersify_coupon_form', $plan, $account, $total);

    $use_coupons = FALSE;
    $result = _membersify_record_query('membersify_coupons');
    foreach ($result as $row) {
      $use_coupons = isset($row->adjustment_id);
      break;
    }

    $payment_button = drupal_get_form('membersify_payment_button_form', $plan, $account, $total);
    $build['checkout_page'] = array(
      '#theme' => 'membersify_checkout_page',
      '#summary' => theme('membersify_checkout_summary', array(
        'items' => $items,
        'adjustments' => $adjustments,
        'total' => membersify_format_money($total, $plan->payment_plan['currency']),
      )),
      '#use_coupons' => $use_coupons,
      '#coupon_widget' => drupal_render($coupon_widget),
      '#payment_button' => drupal_render($payment_button),
    );
  }
  else {
    drupal_goto('user/register');
  }

  return $build;
}

/**
 * Form constructor for the coupons form.
 */
function membersify_coupon_form($form, &$form_state, $plan, $account, $total) {
  $form['coupon_code'] = array(
    '#type' => 'textfield',
    '#title' => t("Promotion code"),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Apply"),
  );
  return $form;
}

/**
 * Form validation handler for membersify_coupon_form().
 */
function membersify_coupon_form_validate($form, &$form_state) {
  $plan = $form_state['build_info']['args'][0];
  $account = $form_state['build_info']['args'][1];
  $total = $form_state['build_info']['args'][2];
  $coupon = membersify_coupon_load_by_code($form_state['values']['coupon_code']);
  if ($coupon) {
    try {
      membersify_coupon_validate($coupon, $plan, $account, $total);
    }
    catch(Exception $e) {
      form_set_error('coupon_code', $e->getMessage());
    }
  }
  else {
    form_set_error('coupon_code', t("Incorrect code entered. Please check it and try again."));
  }
}

/**
 * Form submission handler for membersify_coupon_form().
 */
function membersify_coupon_form_submit($form, &$form_state) {
  // Add the adjustment to the array.
  $coupon = membersify_coupon_load_by_code($form_state['values']['coupon_code']);
  $plan = $form_state['build_info']['args'][0];
  $_SESSION['membersify_adjustments'][$plan->id][$coupon->id] = 'coupon';
}

/**
 * Build the payment button.
 */
function membersify_payment_button_form($form, $form_state, $plan, $account, $amount, $custom = '') {
  $form['#id'] = 'membersify-payment-button-form';
  $form['#plan'] = $plan;
  $form['#account'] = $account;
  $form['#custom_variable'] = $custom;
  $form['actions'] = array('#type' => 'actions');

  if ($plan->payment_plan['main_amount'] > 0) {
    // Show the payment button.
    // Include the js from stripe.com.
    $form['#attached']['js'][] = array(
      'data' => 'https://checkout.stripe.com/checkout.js',
      'type' => 'external'
    );

    $form['#attached']['js'][] = array(
      'data' => array('stripe' => array(
        'publicKey' => variable_get('membersify_stripe_account', ''),
        'orderTitle' => $plan->name,
        'orderDescription' => $plan->description,
        'orderEmail' => $account->mail,
        'orderTotal' => round($amount * 100),
        'orderCurrency' => strtolower($plan->payment_plan['currency']),
       )),
      'type' => 'setting'
    );
    $form['#attached']['js'][] = drupal_get_path('module', 'membersify') . '/js/membersify.stripe.js';
    $form['#attached']['css'][] = drupal_get_path('module', 'membersify') . '/css/membersify.stripe.css';
    $form['#attached']['css'][] = drupal_get_path('module', 'membersify') . '/css/membersify.membersify.payment.css';

    $form['token_field'] = array(
      '#type' => 'hidden',
      '#attributes' => array(
        'class' => array('membersify_payment_token_field'),
      ),
    );

    $payment_profiles = membersify_get_payment_profiles($account->uid);

    $i = 0;
    foreach ($payment_profiles as $payment_profile) {
      $form['actions']['saved_profile-' . $i] = array(
        '#type' => 'submit',
        '#profile_id' => $payment_profile->id,
        '#value' => t('Pay now with @card', array('@card' => $payment_profile->display)),
      );
      $i += 1;
    }

    $button_text = count($payment_profiles) > 0 ? t('Pay with new card') : t('Pay with card');
    $form['actions']['payment_button'] = array(
      '#type' => 'markup',
      '#markup' => "<a id='membersify_payment_button' href='#' class='stripe-connect'><span>" . $button_text . "</span></a>"
    );

    if ($amount == 0) {
      $form['free_explanation'] = array(
        '#type' => 'markup',
        '#markup' => "<div class='membersify_free_explanation'>"
          . t("Note: Your card won't be charged until your free trial is over. If you cancel before the free trial is over, you won't be charged.")
          . "</div>",
      );
    }

    // We need a button or else jQuery submit won't work. But it will be hidden.
    $form['submit'] = array(
      '#type' => 'submit',
      '#prefix' => "<div id='membersify-payment-submit-button-hidden'>",
      '#suffix' => '</div>',
      '#value' => t('Submit'),
    );
  }
  else {
    // Show the free button.
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#markup' => t('Complete'),
    );
  }

  return $form;
}

/**
 * Form validation handler for membersify_payment_button_form().
 */
function membersify_payment_button_form_validate($form, &$form_state) {
  try {
    $plan = $form['#plan'];
    $account = $form['#account'];
    $params = array(
      'user_id' => $account->uid,
      'display' => t('Subscription record for @username', array('@username' => $account->name)),
      'custom' => $form['#custom_variable'],
      'plan_id' => $plan->id,
      'email' => $account->mail,
      'adjustments' => !empty($_SESSION['membersify_adjustments'][$plan->id]) ? $_SESSION['membersify_adjustments'][$plan->id] : array(),
    );

    if (!empty($form_state['values']['token_field'])) {
      $params['token'] = $form_state['values']['token_field'];
    }

    if (!empty($form_state['triggering_element']['#profile_id'])) {
      $params['profile_id'] = $form_state['triggering_element']['#profile_id'];
    }

    Membersify::setKeys(variable_get('membersify_public_key', ''), variable_get('membersify_secret_key', ''));
    $subscription = Membersify_Subscription::create($params);

    membersify_change_roles($subscription->user_id, $plan->data['start_grant_roles'], $plan->data['start_remove_roles']);

    membersify_save_record('membersify_subscriptions', $subscription);
    $form_state['values']['subscription'] = $subscription;

    // Unblock the user if applicable.
    if (!$account->status
      && variable_get('user_register', USER_REGISTER_VISITORS_ADMINISTRATIVE_APPROVAL) == USER_REGISTER_VISITORS // Make sure the setting is set to allow users to register themselves.
      && !variable_get('user_email_verification', TRUE)) { // Also make sure that users aren't required to validate their email first (meaning they were able to enter their password during registration).
      db_update('users')
        ->fields(array(
          'status' => 1,
        ))
        ->condition('uid', $account->uid)
        ->execute();
      $form_state['values']['login_user'] = $account->uid;
    }
  }
  catch (Exception $e) {
    form_set_error('submit', t("There was an error. Please contact the site administrator. Error: @error", array('@error' => $e->getMessage())));
  }
}

/**
 * Form submission handler for membersify_payment_button_form().
 */
function membersify_payment_button_form_submit($form, &$form_state) {
  global $user;

  // Log the user back in if needed.
  if (!$user->uid && !empty($form_state['values']['login_user'])) {
    $account = user_load($form_state['values']['login_user']);

    if ($account->uid) {
      $user = $account;
      $login_array = array ('name' => $account->name);
      user_login_finalize($login_array);
    }
  }

  $subscription = $form_state['values']['subscription'];

  $form_state['redirect'] = variable_get('membersify_thankyou_page', 'membersify/thankyou/' . $subscription->id);
}

/**
 * Page callback for the thankyou page.
 */
function membersify_thankyou_page($subscription) {
  $account = user_load($subscription->user_id);
  $plan = membersify_plan_load($subscription->plan_id);

  return theme('membersify_thankyou_page', array(
    'account' => $account,
    'subscription' => $subscription,
    'plan' => $plan,
  ));
}

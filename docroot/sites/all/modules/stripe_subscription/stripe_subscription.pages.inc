<?php
/**
 * @file
 * Callbacks for the stripe_subscription module.
 */

/**
 * Cancel Subscription Form
 */
function stripe_subscription_subscription_cancel_form($form, &$form_state, $uid = NULL) {
  libraries_load('stripe');
  global $user;
  if (!isset($uid)) {
    $uid = $user->uid;
  }
  else {
    if ($user->uid != $uid && !user_access('administer stripe')) {
      drupal_access_denied();
      return;
    }
  }

  $account = user_load($uid);
  $back_path = str_replace('/cancel', '', current_path());

  // Test to see if there is a customer_id for this user already
  $livemode = stripe_subscription_live() ? 1 : 0;
  $customer = NULL;
  $subscription = NULL;
  $customer_id = db_select('stripe_customers', 'sc')
    ->fields('sc', array('customer_id'))
    ->condition('sc.uid', $account->uid)
    ->condition('sc.livemode', $livemode)
    ->execute()->fetchField();

  // If the customer ID exists, request the customer
  // object to modify the subscription form.
  if (!empty($customer_id)) {
    try {
      $customer = Stripe_Customer::retrieve($customer_id);
      if (isset($customer->subscriptions) && count($customer->subscriptions->data) > 0) {
        $subscription = $customer->subscriptions->data[0];
      }
    }
    catch(Exception $e) {

    }
  }

  if (!isset($subscription)) {
    drupal_goto($back_path);
    return;
  }

  $form['customer_id'] = array(
    '#type' => 'hidden',
    '#value' => $customer_id
  );

  $form['uid'] = array(
    '#type' => 'hidden',
    '#value' => $account->uid
  );

  $form['subscription'] = array(
    '#type' => 'fieldset',
    '#title' => 'Subscription Info'
  );

  if (is_object($subscription)) {

    // Build subscription status table.
    $header = array(t('Status'), t('Current Plan'), t('Next Invoice'));
    $status = isset($subscription->status) ? ucfirst($subscription->status) : t('Inactive');
    $current = isset($subscription->plan) ? stripe_subscription_plan_name($subscription->plan) : t('None');
    $next = format_date($subscription->current_period_end+1, 'custom', 'm/d/Y');
    $rows = array(array($status, $current, $next));

    $subscription_for = '<p><label>' . t('Subscriber:') . '</label>' . $account->name . ' (' . $account->mail . ')</p>';

    $form['subscription']['status'] = array(
      '#markup' => $subscription_for . theme('table', array('header' => $header, 'rows' => $rows))
    );

    if (isset($subscription->plan)) {
      $form['previous_plan_name'] = array(
        '#type' => 'hidden',
        '#value' => $subscription->plan->name
      );
    }
  }

  $form['subscription']['cancel_copy'] = array(
    '#markup' => '<p>' . t('Are you sure you would like to cancel this subscription?') . '</p>'
  );

  $form['subscription']['cancel_link'] = array(
    '#markup' => l('No, Go back ', $back_path)
  );

  $form['subscription']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Yes, Cancel Subscription')
  );

  return $form;
}

function stripe_subscription_subscription_cancel_form_submit($form, &$form_state) {
  libraries_load('stripe');
  try {
    $cu = Stripe_Customer::retrieve($form_state['values']['customer_id']);
    $cu->cancelSubscription();
    drupal_set_message('Your subscription has been cancelled.');
  }
  catch(Exception $e) {
    drupal_set_message('Error cancelling subscription.');
  }
}

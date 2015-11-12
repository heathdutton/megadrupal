<?php
/**
 * @file
 * Subscription page.
 */

/**
 * Main subscription page.
 */
function chargebee_subscription_page($user) {
  $page = '';
  $subscription = chargebee_subscription_load($user->uid);
  if (!$subscription) {
    $page .= chargebee_subscribe_table($user);
  }
  else {
    $variables = array(
      'user' => $user,
      'subscription' => $subscription,
      'customer' => chargebee_customer_load($user->uid),
      'card' => chargebee_card_load($user->uid),
    );
    $page .= theme('chargebee_subscription_info', $variables);
  }
  return $page;
}

/**
 * Subscription edit page.
 */
function chargebee_subscription_edit_page($user) {
  drupal_add_css(drupal_get_path('module', 'chargebee') . '/theme/css/chargebee.css');
  $page = '';
  $subscription = chargebee_subscription_load($user->uid);
  if ($subscription) {
    $page .= chargebee_subscribe_table($user, $subscription);
    $page .= l(t('Back'), 'user/' . $user->uid . '/chargebee');
    $page .= l(t('Cancel subscription'), 'user/' . $user->uid . '/chargebee/subscription/close', array('attributes' => array('class' => array('chargebee-cancel-link'))));
  }
  else {
    $page .= t('You do not have any subscription.');
    $page .= l(t('Back'), 'user/' . $user->uid . '/chargebee');
  }
  return $page;
}

/**
 * Update subscription.
 */
function chargebee_subscription_update_page($user, $plan) {
  $page = '';
  $subscription = chargebee_subscription_load($user->uid);
  if ($subscription) {
    if ($subscription->plan_id == $plan->id) {
      $page .= t('This is your current plan.');
      $page .= l(t('Back'), 'user/' . $user->uid . '/chargebee');
    }
    else {
      $form = drupal_get_form('chargebee_subscription_update_form', $user, $subscription, $plan);
      $page .= drupal_render($form);
    }
  }
  else {
    $page .= t('You do not have any subscription.');
    $page .= l(t('Back'), 'user/' . $user->uid . '/chargebee');
  }
  return $page;
}

/**
 * Subscription cancel page.
 */
function chargebee_subscription_cancel_page($user) {
  $page = '';
  $subscription = chargebee_subscription_load($user->uid);
  if ($subscription) {
    $form = drupal_get_form('chargebee_subscription_cancel_form', $user, $subscription);
    $page .= drupal_render($form);
  }
  else {
    $page .= t('You do not have any subscription.');
    $page .= l(t('Back'), 'user/' . $user->uid . '/chargebee/subscription');
  }
  return $page;
}

/**
 * Subscribe table.
 */
function chargebee_subscribe_table($user, $subscription = FALSE) {
  $plans = chargebee_plan_load_multiple();
  $table_title = array(
    t('Plan'),
    t('Price'),
    t('Action'),
  );
  $table_rows = array();
  $action = $subscription ? 'subscription/update' : 'subscribe';
  foreach ($plans as $plan) {
    $link = l(t('Subscribe'), 'user/' . $user->uid . '/chargebee/' . $action . '/' . $plan->plan_id);
    if ($subscription && $subscription->plan_id == $plan->id) {
      $link = t('Current plan');
    }
    $table_rows[] = array(
      l($plan->name, 'chargebee/plan/' . $plan->plan_id),
      '$ ' . money_format('%i', $plan->price / 100) . ' / ' . $plan->period_unit,
      $link,
    );
  }
  $table_vars = array(
    'header' => $table_title,
    'rows' => $table_rows,
  );
  return theme('table', $table_vars);
}

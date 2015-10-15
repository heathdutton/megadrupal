<?php
/**
 * @file
 * Module user pages.
 */

/**
 * Subscribe page.
 */
function chargebee_subscribe_page($user, $plan_id) {
  $page = '';
  $plan = chargebee_plan_load($plan_id);
  if ($plan) {
    $subscription = chargebee_subscription_load($user->uid);
    if ($subscription && $subscription->plan_id == $plan_id) {
      $page .= t('You are subscribed to this plan');
    }
    else {
      $hostedpage = chargebee_get_hostedpage_checkout($user, $plan_id);
      $page .= theme('chargebee_hostedpage', array('hostedpage' => $hostedpage));
    }
    $page .= l(t('Back'), 'user/' . $user->uid . '/chargebee');
  }
  return $page;
}

/**
 * Card edit page.
 */
function chargebee_card_edit_page($user) {
  $hostedpage = chargebee_get_hostedpage_card_update($user);
  $page = '';
  if ($hostedpage) {
    $page .= theme('chargebee_hostedpage', array('hostedpage' => $hostedpage));
  }
  else {
    $page .= t('You do not have active card.');
  }
  $page .= l(t('Back'), 'user/' . $user->uid . '/chargebee');
  return $page;
}

/**
 * Plan info page.
 */
function chargebee_plan_info_page($plan) {
  return theme('chargebee_plan_info', array('plan' => $plan));
}

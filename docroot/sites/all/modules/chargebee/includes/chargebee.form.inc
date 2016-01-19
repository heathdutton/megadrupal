<?php
/**
 * @file
 * Module forms.
 */

/**
 * Main subscription form.
 */
function chargebee_customer_information_form($form, &$form_state, $user) {
  $elements = array(
    'first_name' => 'First Name',
    'last_name' => 'Last Name',
    'address1' => 'Address Line 1',
    'address2' => 'Address Line 2',
    'city' => 'City',
    'state' => 'State',
    'zip' => 'Zip',
    'country' => 'Country',
    'phone' => 'Phone',
  );
  foreach ($elements as $key => $title) {
    $form[$key] = _chargebee_form_create_textfield($title);
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Back'), 'user/' . $user->uid . '/chargebee'),
  );
  return $form;
}

/**
 * Update subscription form.
 */
function chargebee_subscription_update_form($form, &$form_state, $user, $subscription, $plan) {
  $old_plan = chargebee_plan_load($subscription->plan_id);
  $message_vars = array(
    '@old_plan' => $old_plan->name,
    '@new_plan' => $plan->name,
  );
  $form['message'] = array(
    '#markup' => t('Are you sure you want to change your subscription plan "@old_plan" to "@new_plan"', $message_vars),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update plan'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Back'), 'user/' . $user->uid . '/chargebee'),
  );
  return $form;
}

/**
 * Update subscription form submit.
 */
function chargebee_subscription_update_form_submit($form, &$form_state) {
  $uid = isset($form_state['build_info']['args'][0]->uid) ? $form_state['build_info']['args'][0]->uid : NULL;
  $subscription = isset($form_state['build_info']['args'][1]) ? $form_state['build_info']['args'][1] : NULL;
  $plan_id = isset($form_state['build_info']['args'][2]->plan_id) ? $form_state['build_info']['args'][2]->plan_id : NULL;
  if ($subscription && $plan_id) {
    $result = chargebee_subscription_update_plan($subscription->subscription_id, $plan_id);
    if ($result) {
      chargebee_subscription_update($subscription, $result['subscription']);
      drupal_set_message('Successful update of subscription.');
    }
    else {
      drupal_set_message('Error update subscription', 'error');
    }
  }
  $form_state['redirect'] = $uid ? 'user/' . $uid . '/chargebee' : '<front>';
}

/**
 * Update subscription form.
 */
function chargebee_subscription_cancel_form($form, &$form_state, $user, $subscription) {
  $plan = chargebee_plan_load($subscription->plan_id);
  $form['message'] = array(
    '#markup' => t('You really want to close the subscription? (@plan)', array('@plan' => $plan->name)),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel subscription'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Back'), 'user/' . $user->uid . '/chargebee/subscription'),
  );
  return $form;
}

/**
 * Cancel subscription form submit.
 */
function chargebee_subscription_cancel_form_submit($form, &$form_state) {
  $uid = isset($form_state['build_info']['args'][0]->uid) ? $form_state['build_info']['args'][0]->uid : NULL;
  $subscription = isset($form_state['build_info']['args'][1]) ? $form_state['build_info']['args'][1] : NULL;
  if ($subscription) {
    $result = chargebee_subscription_cancel($subscription->subscription_id);
    if ($result) {
      chargebee_subscription_update($subscription, $result['subscription']);
      drupal_set_message('Successful cancel subscription.');
    }
    else {
      drupal_set_message('Error update subscription', 'error');
    }
  }
  $redirect_path = $uid ? 'user/' . $uid . '/chargebee' : '<front>';
  $form_state['redirect'] = $redirect_path;
}

/**
 * Create simple form textfield.
 *
 * @param string $title
 *   Textfield title.
 *
 * @return array
 *   Textfield array.
 */
function _chargebee_form_create_textfield($title) {
  $textfield = array(
    '#type' => 'textfield',
    '#title' => t('@title', array('@title' => $title)),
  );
  return $textfield;
}

<?php

/**
 * @file
 * Authorize.net - User Pages
 */

/**
 * User payment profiles list form.
 *
 * @param $user
 *   The user account to display payment profiles for.
 */
function authnet_user_payment_profiles_list_form($form, $form_state, $user) {

  // Set the page title.
  drupal_set_title(t('Saved cards'));

  // Start an empty form.
  $form = array();

  // Load the customer profile id.
  $customer_profile_id = authnet_cim_entity_profile_id_load('user', $user->uid);

  // Load the customer's payment profiles.
  $payment_profiles = authnet_cim_entity_customer_payment_profiles_load($customer_profile_id);

  // Save the user id in the form.
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $user->uid,
  );

  // Save the customre profile id to the form.
  $form['customer_profile_id'] = array(
    '#type' => 'value',
    '#value' => $customer_profile_id,
  );

  // Load the customer's default payment profile.
  $default_pid = authnet_cim_entity_customer_payment_profile_default($customer_profile_id);

  // Save the original default payment profile pid to the form for comparison later.
  $form['old_default'] = array(
    '#type' => 'value',
    '#value' => $default_pid,
  );

  // Create form elements for each payment profile.
  foreach ($payment_profiles as $payment_profile) {

    // Radio button for setting a payment profile as the default.
    $form['payment_profiles'][$payment_profile['pid']]['default'] = array(
      '#type' => 'radio',
      '#return_value' => $payment_profile['pid'],
      '#default_value' => $default_pid,
    );

    // Store the payment profile record in the form. It will be used in the theme function.
    $form['payment_profiles'][$payment_profile['pid']]['payment_profile'] = array(
      '#type' => 'value',
      '#value' => $payment_profile,
    );
  }

  // Submit button (if there are payment profiles).
  if (!empty($payment_profiles)) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }

  // Return the output.
  return $form;
}

/**
 * User payment profiles list form submit.
 */
function authnet_user_payment_profiles_list_form_submit($form, &$form_state) {

  // If the default payment profile has changed, update it in the database.
  if ($form_state['values']['old_default'] != $form_state['values']['default']) {

    // Get the customer profile id.
    $customer_profile_id = $form_state['values']['customer_profile_id'];

    // Set all the other payment profiles for this customer to NOT default.
    $query = db_update('authnet_cim_entity_payment_profile');
    $query->fields(array('is_default' => 0));
    $query->condition('is_default', 1);
    $query->condition('customer_profile_id', $customer_profile_id);
    $query->execute();

    // Set the selected payment profile as the default.
    $query = db_update('authnet_cim_entity_payment_profile');
    $query->fields(array('is_default' => 1));
    $query->condition('pid', $form_state['values']['default']);
    $query->execute();
  }
}

/**
 * User payment profile form.
 *
 * @param $user
 *   The user account object.
 * @param $payment_profile_id
 *   The id of the payment profile to edit.
 */
function authnet_user_payment_profile_form($form, &$form_state, $user, $payment_profile_id = NULL) {

  // Set the page title.
  if ($payment_profile_id) {
    drupal_set_title(t('Edit payment profile'));
  }
  else {
    drupal_set_title(t('Add a payment profile'));
  }

  // Load the user's customer profile id.
  $customer_profile_id = authnet_cim_entity_profile_id_load('user', $user->uid);

  // Start with an empty form.
  $form = array();

  // Save the user id value to the form. This will be used to generate
  // information for the Authorize.net customer profile, if one doesn't exist
  // already.
  // @see authnet_user_authnet_ui_form_customer_profile_new()
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $user->uid,
  );

  // Load the payment profile record, if it exists. Otherwise, just create a blank one.
  if ($payment_profile_id) {
    $payment_profile = authnet_cim_entity_payment_profile_load($payment_profile_id);
  }
  else {
    $payment_profile = authnet_cim_entity_payment_profile_new();
  }

  // Payment profile label.
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#description' => t('Use this field to give your payment profile a descriptive label.'),
    '#default_value' => $payment_profile['label'],
  );

  // Checkbox to make this payment profile the default one.
  $form['is_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make this payment profile the default.'),
    '#default_value' => $payment_profile['is_default'],
  );

  // Add the Authnet UI payment profile form.
  $form += authnet_ui_payment_profile_form($form, $form_state, $customer_profile_id, $payment_profile_id);

  // Add a submit function for redirecting.
  $form['submit']['#submit'][] = 'authnet_user_billing_redirect';

  // Return the payment profile form.
  return $form;
}

/**
 * Submit function for redirecting to the user's billing list page.
 */
function authnet_user_billing_redirect($form, &$form_state) {

  // Redirect to the user's billing list page.
  global $user;
  $form_state['redirect'] = 'user/' . $user->uid . '/billing';
}

/**
 * User payment profile delete form.
 *
 * @param $user
 *   The user account object.
 * @param $pid
 *   The id of the payment profile to delete.
 */
function authnet_user_payment_profile_delete_form($form, &$form_state, $user, $payment_profile_id) {

  // Start an empty form array.
  $form = array();

  // Store the payment profile id value in the form.
  $form['payment_profile_id'] = array(
    '#type' => 'value',
    '#value' => $payment_profile_id,
  );

  // If the user clicks "Cancel", return to the user's billing list.
  $cancel_path = 'user/' . $user->uid . '/billing';

  // Return a confirmation form.
  return confirm_form($form, 'Are you sure you want to delete this payment profile?', $cancel_path);
}

/**
 * User payment profile delete form submit.
 */
function authnet_user_payment_profile_delete_form_submit($form, &$form_state) {

  // Load the payment profile record.
  $payment_profile = authnet_cim_entity_payment_profile_load($form_state['values']['payment_profile_id']);

  // Delete the payment profile.
  authnet_cim_payment_profile_delete($payment_profile['customer_profile_id'], $payment_profile['payment_profile_id']);

  // Redirect the user back to the billing list page.
  global $user;
  $form_state['redirect'] = 'user/' . $user->uid . '/billing';
}

<?php

/**
 * Form constructor for the rebuild subscriptions form.
 *
 * @see membersify_sync_confirm_submit()
 * @see confirm_form()
 * @ingroup forms
 */
function membersify_sync_confirm($form, &$form_state) {
  return confirm_form($form, t('Are you sure you want to re-sync the subscriptions, plans, and settings?'), 'admin/config/membersify/settings', t('This will remove and add any roles that should have been removed or added to users based on their current subscriptions, by first removing roles from expired subscriptions, then adding roles from active subscriptions. This action cannot be undone.'), t('Re-sync'));
}

/**
 * Form submission handler for membersify_sync_confirm().
 */
function membersify_sync_confirm_submit($form, &$form_state) {
  try {
    membersify_sync_all();
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }

  $form_state['redirect'] = 'admin/config/membersify/settings';
}

/**
 * Syncs the settings.
 */
function membersify_sync_settings() {
  if (empty(Membersify::$secret_key)) {
    Membersify::setKeys(variable_get('membersify_public_key', ''), variable_get('membersify_secret_key', ''));
  }

  // Save the settings.
  $settings = Membersify::getSettings();
  foreach ($settings as $key => $value) {
    variable_set('membersify_' . $key, $value);
  }

  // Update the webhook URL.
  Membersify::saveWebhook(url('', array('absolute' => TRUE)), url('membersify/webhook', array('absolute' => TRUE)));

  drupal_set_message(format_plural(count($settings), t("Synced 1 setting."), t("Synced @count settings.")));
}

/**
 * Syncs the roles when a role is added, edited, or deleted.
 */
function membersify_sync_roles() {
  if (empty(Membersify::$secret_key)) {
    Membersify::setKeys(variable_get('membersify_public_key', ''), variable_get('membersify_secret_key', ''));
  }

  $roles = user_roles(TRUE);
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  Membersify::sendLevels($roles);
  drupal_set_message(format_plural(count($roles), t("Synced 1 role."), t("Synced @count roles.")));
}

/**
 * Syncs the plans.
 */
function membersify_sync_plans() {
  if (empty(Membersify::$secret_key)) {
    Membersify::setKeys(variable_get('membersify_public_key', ''), variable_get('membersify_secret_key', ''));
  }

  db_truncate('membersify_plans')->execute();

  // Grab the plans from Membersify.
  $plans = Membersify_Plan::all();

  foreach ($plans as $plan) {
    membersify_save_record('membersify_plans', $plan);
  }

  drupal_set_message(format_plural(count($plans), t("Synced 1 plan."), t("Synced @count plans.")));
}

/**
 * Syncs the subscriptions.
 */
function membersify_sync_subscriptions() {
  $plans = membersify_get_plans();
  if (empty(Membersify::$secret_key)) {
    Membersify::setKeys(variable_get('membersify_public_key', ''), variable_get('membersify_secret_key', ''));
  }

  db_truncate('membersify_subscriptions')->execute();

  // Grab all of the expired subscriptions and remove the roles from the user accounts.
  $filters = array(
    'status' => array('expired'),
  );
  $params = array('filters' => $filters);
  $subscriptions = Membersify_Subscription::all($params);

  foreach ($subscriptions as $subscription) {
    if (isset($plans[$subscription->plan_id])) {
      membersify_save_record('membersify_subscriptions', $subscription);
      membersify_change_roles($subscription->user_id, $plans[$subscription->plan_id]->data['expire_grant_roles'], $plans[$subscription->plan_id]->data['expire_remove_roles']);
    }
  }

  drupal_set_message(format_plural(count($subscriptions), t("Synced 1 expired subscription."), t("Synced @count expired subscriptions.")));

  // Grab all the rest of the subscriptions that are active and add the roles to
  // the user accounts.
  $filters = array(
    'status' => array('active', 'canceled', 'expiring', 'complete', 'pending_downgrade'),
  );
  $params = array('filters' => $filters);
  $subscriptions = Membersify_Subscription::all($params);

  foreach ($subscriptions as $subscription) {
    if (isset($plans[$subscription->plan_id])) {
      membersify_save_record('membersify_subscriptions', $subscription);
      membersify_change_roles($subscription->user_id, $plans[$subscription->plan_id]->data['start_grant_roles'], $plans[$subscription->plan_id]->data['start_remove_roles']);
    }
  }

  drupal_set_message(format_plural(count($subscriptions), t("Synced 1 active subscription."), t("Synced @count active subscriptions.")));
}

/**
 * Syncs the history items.
 */
function membersify_sync_history() {
  if (empty(Membersify::$secret_key)) {
    Membersify::setKeys(variable_get('membersify_public_key', ''), variable_get('membersify_secret_key', ''));
  }

  db_truncate('membersify_history')->execute();
  $result = _membersify_record_query('membersify_subscriptions');
  $count = 0;

  foreach ($result as $subscription) {
    $filters = array(
      'subscription_id' => $subscription->id,
    );
    $params = array('filters' => $filters);
    $history_items = Membersify_HistoryItem::all($params);

    foreach ($history_items as $history_item) {
      membersify_save_record('membersify_history', $history_item);
      $count += 1;
    }
  }

  drupal_set_message(format_plural($count, t("Synced 1 history item."), t("Synced @count history items.")));
}

/**
 * Syncs the profiles.
 */
function membersify_sync_profiles() {
  if (empty(Membersify::$secret_key)) {
    Membersify::setKeys(variable_get('membersify_public_key', ''), variable_get('membersify_secret_key', ''));
  }

  db_truncate('membersify_payment_profiles')->execute();

  $payment_profiles = Membersify_Profile::all();

  foreach ($payment_profiles as $payment_profile) {
    membersify_save_record('membersify_payment_profiles', $payment_profile);
  }

  drupal_set_message(format_plural(count($payment_profiles), t("Synced 1 payment profile."), t("Synced @count payment profiles.")));
}

/**
 * Syncs the coupons.
 */
function membersify_sync_coupons() {
  if (empty(Membersify::$secret_key)) {
    Membersify::setKeys(variable_get('membersify_public_key', ''), variable_get('membersify_secret_key', ''));
  }

  db_truncate('membersify_coupons')->execute();

  $coupons = Membersify_Coupon::all();

  foreach ($coupons as $coupon) {
    membersify_save_record('membersify_adjustments', $coupon);
    $coupon->adjustment_id = $coupon->id;
    membersify_save_record('membersify_coupons', $coupon, 'adjustment_id');
  }

  drupal_set_message(format_plural(count($coupons), t("Synced 1 coupon."), t("Synced @count coupons.")));
}

/**
 * Re-syncs everything.
 */
function membersify_sync_all() {
  membersify_sync_settings();
  membersify_sync_roles();
  membersify_sync_plans();
  membersify_sync_subscriptions();
  membersify_sync_profiles();
  membersify_sync_coupons();
  membersify_sync_history();
}

/**
 * Implements hook_user_role_update().
 */
function membersify_user_role_update($role) {
  try {
    membersify_sync_roles();
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }
}

/**
 * Implements hook_user_role_insert().
 */
function membersify_user_role_insert($role) {
  try {
    membersify_sync_roles();
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }
}

/**
 * Implements hook_user_role_delete().
 */
function membersify_user_role_delete($role) {
  try {
    membersify_sync_roles();
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }
}

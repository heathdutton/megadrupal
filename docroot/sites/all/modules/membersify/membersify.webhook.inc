<?php

/**
 * Access callback for the webhook.
 */
function membersify_webhook_access() {
  $headers = _membersify_get_headers();
  $sent_hash = $headers['X-Hash'];
  $content = file_get_contents('php://input');

  $hash = hash_hmac('sha256', $content, variable_get('membersify_secret_key', ''));

  return $hash == $sent_hash;
}

/**
 * Gets the headers from a request.
 */
function _membersify_get_headers() {
  $headers = array();
  foreach ($_SERVER as $k => $v) {
  if (substr($k, 0, 5) == "HTTP_") {
      $k = str_replace('_', ' ', substr($k, 5));
      $k = str_replace(' ', '-', ucwords(strtolower($k)));
      $headers[$k] = $v;
    }
  }
  return $headers;
}

/**
 * Webhook callback.
 */
function membersify_webhook() {
  try {
    // Get the event from Membersify to confirm it is legit.
    Membersify::setKeys(variable_get('membersify_public_key', ''), variable_get('membersify_secret_key', ''));
    $event = Membersify_Event::retrieve($_POST['id']);
    watchdog('membersify', 'Receiving a webhook. Data: !data', array('!data' => '<pre>' . print_r($event, TRUE) . '</pre>'));

    switch ($event->event_type) {
      case 'settings.sync_request':
        membersify_sync_settings();
        break;

      case 'plans.sync_request':
        membersify_sync_plans();
        break;

      case 'plan.saved':
        membersify_save_record('membersify_plans', $event->data['plan']);
        break;

      case 'plan.deleted':
        membersify_delete_record('membersify_plans', $event->data['plan']);
        break;

      case 'subscription.saved':
        membersify_save_record('membersify_subscriptions', $event->data['subscription']);
        break;

      case 'subscription.deleted':
        membersify_delete_record('membersify_subscriptions', $event->data['subscription']);
        break;

      case 'subscription.expiring':
      case 'subscription.expiring_soon':
      case 'subscription.changed_billing':
      case 'subscription.payment':
      case 'subscription.start':
      case 'subscription.cancel':
        $subscription = new Membersify_Subscription();
        $subscription->setValues($event->data['subscription']);

        // Save the subscription record.
        membersify_save_record('membersify_subscriptions', $subscription);

        // Save the history item if applicable.
        if (!empty($event->data['history_item'])) {
          membersify_save_record('membersify_history', $event->data['history_item']);
        }

        // Invoke the hook.
        module_invoke_all('membersify_' . str_replace('.', '_', $event->event_type), $subscription);

        // Send the email if applicable.
        _membersify_send_mail($event->data);
        break;

      case 'subscription.plan_changed':
        membersify_save_record('membersify_subscriptions', $event->data['subscription']);

        $plan = membersify_plan_load($event->data['plan_id']);
        $old_plan = membersify_plan_load($event->data['old_plan_id']);
        membersify_change_roles($event->data['subscription']->user_id, $old_plan->data['expire_grant_roles'], $old_plan->data['expire_remove_roles']);
        membersify_change_roles($event->data['subscription']->user_id, $plan->data['start_grant_roles'], $plan->data['start_remove_roles']);

        module_invoke_all('membersify_subscription_changed', $event->data['subscription']);
        _membersify_send_mail($event->data);
        break;

      case 'payment_profile.created':
        membersify_save_record('membersify_payment_profiles', $event->data['payment_profile']);
        break;

      case 'payment_profile.deleted':
        membersify_delete_record('membersify_payment_profiles', $event->data['payment_profile']);
        break;
    }
  }
  catch (Exception $e) {
    watchdog('membersify', 'Error receiving a webhook. Error: @error Data: !data',
      array('@error' => $e->getMessage(), '!data' => '<pre>' . print_r($_POST, TRUE) . '</pre>'));
  }
}

/**
 * Implements hook_membersify_subscription_expiring().
 */
function membersify_membersify_subscription_expiring(Membersify_Subscription $subscription) {
  $plan = membersify_plan_load($subscription->plan_id);
  membersify_change_roles($subscription->user_id, $plan->data['expire_grant_roles'], $plan->data['expire_remove_roles']);
}

/**
 * Sends an email as specified by a webhook event.
 */
function _membersify_send_mail($data) {
  if (!empty($data['email_body']) && !empty($data['email_subject']) && !empty($data['user_id'])) {
    $account = user_load($data['user_id']);
    if ($account->uid) {
      drupal_mail('membersify', 'webhook_send_mail', $account->mail, user_preferred_language($account), $data, variable_get('membersify_admin_mail', variable_get('site_mail', '')), TRUE);
    }
  }
}

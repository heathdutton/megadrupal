<?php

/**
 * @file
 * The user tab page callbacks and other related functions.
 */

/**
 * Page callback for the subscriptions tab page.
 */
function membersify_account_tab($account) {
  $subscriptions = membersify_get_subscriptions($account->uid);
  $subscription_html_array = array();
  $plans = membersify_get_plans();
  foreach ($subscriptions as $subscription) {
    $cancel_url = NULL;
    $change_url = NULL;
    $billing_url = NULL;
    $renew_url = NULL;
    $history_url = url('user/' . $account->uid . '/subscriptions/' . $subscription->id . '/history');
    $plan = membersify_plan_load($subscription->plan_id);
    $modify_options = membersify_get_modify_options($plan);

    if (in_array($subscription->status, array('active', 'pending_downgrade', 'expiring_soon'))) {
      $billing_url = url('user/' . $account->uid . '/subscriptions/' . $subscription->id . '/change_billing');
    }
    if (!empty($modify_options) && in_array($subscription->status, array('active', 'expiring_soon'))) {
      $change_url = url('user/' . $account->uid . '/subscriptions/' . $subscription->id . '/change_plan');
    }
    if (in_array($subscription->status, array('active'))) {
      $cancel_url = url('user/' . $account->uid . '/subscriptions/' . $subscription->id . '/cancel');
    }
    if (in_array($subscription->status, array('canceled'))) {
      $renew_url = url('user/' . $account->uid . '/subscriptions/' . $subscription->id . '/renew');
    }

    $subscription_html_array[] = theme('membersify_subscription', array(
      'subscription' => $subscription,
      'plan' => $plans[$subscription->plan_id],
      'change_billing_url' => $billing_url,
      'change_plan_url' => $change_url,
      'cancel_url' => $cancel_url,
      'renew_url' => $renew_url,
      'history_url' => $history_url,
    ));
  }

  $purchase_links_array = array();
  foreach ($plans as $plan) {
    $purchase_links_array[] = theme('membersify_purchase_link', array(
      'plan' => $plan,
      'url' => url('membersify/purchase/' . $plan->machine_name),
    ));
  }

  drupal_add_css(drupal_get_path('module', 'membersify') . '/css/membersify.user.css');

  return theme('membersify_user_page', array(
    'account' => $account,
    'subscriptions_html' => implode('', $subscription_html_array),
    'purchase_links' => implode('', $purchase_links_array),
  ));
}

/**
 * Form constructor for the cancel subscription form.
 */
function membersify_cancel_subscription_form($form, &$form_state, $account, Membersify_Subscription $subscription) {
  $form['question'] = array(
    '#markup' => t("Are you sure you want to cancel this subscription?"),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Yes, cancel my subscription'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Wait, I changed my mind'), 'user/' . $account->uid . '/subscription'),
  );

  return $form;
}

/**
 * Form validation handler for membersify_cancel_subscription_form().
 */
function membersify_cancel_subscription_form_validate($form, &$form_state) {
  try {
    $subscription = $form_state['build_info']['args'][1];
    Membersify::setKeys(variable_get('membersify_public_key', ''), variable_get('membersify_secret_key', ''));
    $subscription->cancel();
  }
  catch (Exception $e) {
    form_set_error('submit', $e->getMessage());
  }
}

/**
 * Form submission handler for membersify_cancel_subscription_form().
 */
function membersify_cancel_subscription_form_submit($form, &$form_state) {
  drupal_set_message("Your subscription has been cancelled.");
  $account = $form_state['build_info']['args'][0];
  $form_state['redirect'] = 'user/' . $account->uid . '/subscription';
}

/**
 * Form constructor for the renew subscription form.
 *
 * @see membersify_renew_subscription_form_validate()
 * @see membersify_renew_subscription_form_submit()
 * @see membersify_menu()
 */
function membersify_renew_subscription_form($form, &$form_state, $account, Membersify_Subscription $subscription) {
  $form['question'] = array(
    '#markup' => t("Are you sure you want to renew this subscription?"),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Yes, renew my subscription'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Wait, I changed my mind'), 'user/' . $account->uid . '/subscription'),
  );

  return $form;
}

/**
 * Form validation handler for membersify_renew_subscription_form().
 */
function membersify_renew_subscription_form_validate($form, &$form_state) {
  try {
    $subscription = $form_state['build_info']['args'][1];
    Membersify::setKeys(variable_get('membersify_public_key', ''), variable_get('membersify_secret_key', ''));
    $result = $subscription->renew();
    membersify_save_record('membersify_subscriptions', $result);
  }
  catch (Exception $e) {
    form_set_error('submit', $e->getMessage());
  }
}

/**
 * Form submission handler for membersify_renew_subscription_form().
 */
function membersify_renew_subscription_form_submit($form, &$form_state) {
  drupal_set_message("Your subscription has been renewed.");
  $account = $form_state['build_info']['args'][0];
  $form_state['redirect'] = 'user/' . $account->uid . '/subscription';
}

/**
 * Form constructor for the change plan form.
 *
 * @param Membersify_Subscription $subscription
 *   The subscription being modified.
 */
function membersify_change_plan_form($form, &$form_state, $account, Membersify_Subscription $subscription) {
  $current_plan = membersify_plan_load($subscription->plan_id);
  $form['plan_id'] = array(
    '#type' => 'radios',
    '#title' => t('New subscription level'),
    '#description' => t('Choose your new subscription level'),
    '#options' => membersify_get_modify_options($current_plan),
    '#required' => TRUE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Change plan'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Wait, I changed my mind.'), 'user/' . $account->uid . '/subscription'),
  );

  return $form;
}

/**
 * Form submission handler for membersify_change_plan_form().
 */
function membersify_change_plan_form_submit($form, &$form_state) {
  $subscription = $form_state['build_info']['args'][1];

  $plan = membersify_plan_load($form_state['values']['plan_id']);

  drupal_goto('user/' . $subscription->user_id . '/subscriptions/' . $subscription->id . '/change_plan/' . $plan->machine_name . '/confirm',
    array('query' => drupal_get_destination()));
}

/**
 * Form constructor for the change billing form.
 *
 * @param stdClass $account
 *   The user account.
 * @param Membersify_Subscription $subscription
 *   The subscription object.
 *
 * @see membersify_change_billing_form_validate()
 * @see membersify_change_billing_form_submit()
 * @ingroup forms
 */
function membersify_change_billing_form($form, &$form_state, $account, Membersify_Subscription $subscription) {
  $form['#id'] = 'membersify-add-card-button-form';
  // Also show a button to add a new payment profile.
  // Include the js from stripe.com.
  $form['#attached']['js'][] = array(
    'data' => 'https://checkout.stripe.com/checkout.js',
    'type' => 'external'
  );
  $form['#attached']['js'][] = array(
    'data' => array('stripe' => array(
      'publicKey' => variable_get('membersify_stripe_account', ''),
      'label' => t("Add new card"),
      'email' => $account->mail,
    )),
    'type' => 'setting'
  );
  $form['#attached']['js'][] = drupal_get_path('module', 'membersify') . '/js/membersify.stripe.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'membersify') . '/css/membersify.stripe.css';
  $form['#attached']['css'][] = drupal_get_path('module', 'membersify') . '/css/membersify.user.css';

  $form['token_field'] = array(
    '#type' => 'hidden',
    '#attributes' => array(
      'class' => array('membersify_add_card_token_field'),
    ),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['add_card_button'] = array(
    '#markup' => "<a id='membersify_add_card_button' href='#' class='stripe-connect'><span>" . t('Add new card') . "</span></a>"
  );

  $form['actions']['cancel'] = array(
    '#markup' => l(t('Back to subscriptions overview'), 'user/' . $account->uid . '/subscription'),
  );

  // We need a button or else jQuery submit won't work. But it will be hidden.
  $form['submit'] = array(
    '#type' => 'submit',
    '#prefix' => "<div id='membersify-billing-submit-button-hidden'>",
    '#suffix' => '</div>',
    '#value' => t('Pay with card'),
  );

  $profiles = membersify_get_payment_profiles($account->uid);
  $form['payment_profiles'] = array(
    '#type' => 'fieldset',
    '#title' => t("Cards"),
    '#prefix' => "<div class='membersify-profiles'>",
    '#suffix' => "</div>",
  );

  foreach ($profiles as $profile) {
    $classes = array('membersify-profile', 'membersify-profile-' . $profile->status);
    $actions = array();
    $statuses = array(membersify_get_payment_profile_status($profile->status));
    if ($profile->status == 'active' && $profile->id != $subscription->payment_profile_id) {
      $actions[] = l(t('Make default'), 'user/' . $account->uid . '/subscriptions/' . $subscription->id . '/change_billing/' . $profile->id . '/set_default',
        array('attributes' => array('class' => array('membersify_button'))));
    }
    $subscriptions = membersify_get_subscriptions_by_payment_profile_id($profile->id);
    if (empty($subscriptions)) {
      $actions[] = l(t('Delete'), 'user/' . $account->uid . '/subscriptions/' . $subscription->id . '/change_billing/' . $profile->id . '/delete',
          array('attributes' => array('class' => array('membersify_button'))));
    }
    else {
      if ($profile->id == $subscription->payment_profile_id) {
        $statuses[] =t("Default");
        $classes[] = 'membersify-profile-default';
      }
      else {
        $statuses[] =t("In use");
        $classes[] = 'membersify-profile-in-use';
      }
    }

    $form['payment_profiles'][$profile->id] = array(
      '#type' => 'markup',
      '#prefix' => "<div class='" . implode(' ', $classes) ."'>",
      '#suffix' => "<div class='membersify-clearfix'></div></div>",
      '#markup' => "<div class='membersify-profile-display'>" . $profile->display . "</div><div class='membersify-profile-status'>" . implode(', ', $statuses) . "</div><div class='membersify-profile-actions'>" . implode('', $actions)  . "</div>",
    );
  }

  return $form;
}

/**
 * Form validation handler for membersify_change_billing_form().
 */
function membersify_change_billing_form_validate($form, &$form_state) {
  try {
    Membersify::setKeys(variable_get('membersify_public_key', ''), variable_get('membersify_secret_key', ''));
    $account = $form_state['build_info']['args'][0];
    // If the user is adding a new card, let's save it.
    if (!empty($form_state['values']['token_field'])) {
      $params['token'] = $form_state['values']['token_field'];
      $params['user_id'] = $account->uid;

      $payment_profile = Membersify_Profile::create($params);

      // Save the payment profile.
      membersify_save_record('membersify_payment_profiles', $payment_profile);
    }
  }
  catch (Exception $e) {
    form_set_error('payment_profile_id', $e->getMessage());
  }
}

/**
 * Form submission handler for membersify_change_billing_form().
 */
function membersify_change_billing_form_submit($form, &$form_state) {
  drupal_set_message(t("Your card has been added successfully."));
}

/**
 * Form constructor for the set default profile form.
 *
 * @see membersify_set_default_profile_form_validate()
 * @see membersify_set_default_profile_form_submit()
 * @see membersify_menu()
 */
function membersify_set_default_profile_form($form, &$form_state, $account, Membersify_Subscription $subscription, Membersify_Profile $profile) {
  $form['question'] = array(
    '#markup' => t("Are you sure you want to set your default card to %display?", array('%display' => $profile->display)),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Yes, set default'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Wait, I changed my mind'), 'user/' . $account->uid . '/subscriptions/' . $subscription->id . '/change_billing'),
  );

  return $form;
}

/**
 * Form validation handler for membersify_set_default_profile_form().
 */
function membersify_set_default_profile_form_validate($form, &$form_state) {
  try {
    $subscription = $form_state['build_info']['args'][1];
    $profile = $form_state['build_info']['args'][2];
    Membersify::setKeys(variable_get('membersify_public_key', ''), variable_get('membersify_secret_key', ''));
    $subscription->changeBilling($profile->id);
  }
  catch (Exception $e) {
    form_set_error('submit', $e->getMessage());
  }
}

/**
 * Form submission handler for membersify_set_default_profile_form().
 */
function membersify_set_default_profile_form_submit($form, &$form_state) {
  drupal_set_message("The default card has been updated.");
  $account = $form_state['build_info']['args'][0];
  $subscription = $form_state['build_info']['args'][1];
  $form_state['redirect'] = 'user/' . $account->uid . '/subscriptions/' . $subscription->id . '/change_billing';
}

/**
 * Form constructor for the delete profile form.
 *
 * @see membersify_delete_profile_form_validate()
 * @see membersify_delete_profile_form_submit()
 * @see membersify_menu()
 */
function membersify_delete_profile_form($form, &$form_state, $account, Membersify_Profile $profile) {
  $form['question'] = array(
    '#markup' => t("Are you sure you want to delete the card %display?", array('%display' => $profile->display)),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Yes, delete this card'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Wait, I changed my mind'), 'user/' . $account->uid . '/payment_methods'),
  );

  return $form;
}

/**
 * Form validation handler for membersify_delete_profile_form().
 */
function membersify_delete_profile_form_validate($form, &$form_state) {
  try {
    $profile = $form_state['build_info']['args'][1];
    Membersify::setKeys(variable_get('membersify_public_key', ''), variable_get('membersify_secret_key', ''));
    $profile->delete();
  }
  catch (Exception $e) {
    form_set_error('submit', $e->getMessage());
  }
}

/**
 * Form submission handler for membersify_delete_profile_form().
 */
function membersify_delete_profile_form_submit($form, &$form_state) {
  drupal_set_message("The card has been deleted.");
  $account = $form_state['build_info']['args'][0];
  $form_state['redirect'] = 'user/' . $account->uid . '/payment_methods';
}

/**
 * Menu callback: The view subscription payments history page.
 *
 * @param stdClass $account
 *   The user account.
 * @param Membersify_Subscription $subscription
 *   The Membersify_Subscription object.
 *
 * @see membersify_menu()
 */
function membersify_payment_history_page($account, Membersify_Subscription $subscription) {
  $build = array();

  $header = array(
    'date' => array(
      'data' => t('Date'),
      'field' => 'created',
      'sort' => 'desc',
    ),
    'type' => array(
      'data' => t('Type'),
      'field' => 'type',
    ),
    'txn_id' => array(
      'data' => t('Transaction ID'),
      'field' => 'txn_id',
    ),
    'amount' => array(
      'data' => t('Amount'),
      'field' => 'amount',
    ),
    'actions' => array(
      'data' => t('Actions'),
    ),
  );
  $rows = array();

  $query = db_select('membersify_history', 'h');
  $query->fields('h');
  $query->condition('h.subscription_id', $subscription->id);
  $query = $query->extend('TableSort')->orderByHeader($header);
  $query = $query->extend('PagerDefault')->limit(20);
  $result = $query->execute();

  foreach ($result as $row) {
    $history_item = new Membersify_HistoryItem();
    $history_item->setValues($row);
    $actions = array();
    $actions[] = l(t('View details'), 'user/' . $subscription->user_id . '/subscriptions/' . $subscription->id . '/history/' . $history_item->id);

    $rows[] = array(
      format_date($history_item->created, 'small'),
      membersify_get_history_item_type($history_item->type),
      $history_item->txn_id,
      membersify_format_money($history_item->amount, $history_item->currency),
      implode(' | ', $actions),
    );
  }

  $build['payments_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'membersify-payments-history-table'),
    '#empty' => t('No history yet.')
  );
  $build['pager'] = array(
    '#theme' => 'pager',
    '#tags' => NULL,
  );

  return $build;
}

/**
 * Menu callback: The view subscription payments history page.
 *
 * @param stdClass $account
 *   The user account.
 * @param Membersify_Subscription $subscription
 *   The Membersify_Subscription object.
 * @param Membersify_HistoryItem $history_item
 *   The membershify history item to show the invoice for.
 *
 * @see membersify_menu()
 */
function membersify_payment_invoice_page($account, Membersify_Subscription $subscription, Membersify_HistoryItem $history_item) {
  $build = array();

  $build['invoice'] = array(
    '#theme' => 'membersify_invoice',
    '#account' => $account,
    '#subscription' => $subscription,
    '#history_item' => $history_item,
  );

  return $build;
}

/**
 * Page callback: The manage payment profiles page.
 *
 * @param $account
 *   The user account.
 *
 * @see membersify_menu().
 */
function membersify_payment_profiles_page($account) {
  $build = array();

  $header = array(
    'display' => array(
      'data' => t('Label'),
      'field' => 'display',
    ),
    'status' => array(
      'data' => t('Status'),
      'field' => 'status',
    ),
    'actions' => array(
      'data' => t('Actions'),
    ),
  );
  $rows = array();

  $query = db_select('membersify_payment_profiles', 'p');
  $query->fields('p');
  $query->condition('p.user_id', $account->uid);
  $result = $query->execute();

  foreach ($result as $row) {
    $profile = new Membersify_Profile();
    $profile->setValues($row);
    $actions = array();
    $actions[] = l(t('Delete'), 'user/' . $account->uid . '/payment_methods/' . $profile->id . '/delete');

    $rows[] = array(
      $profile->display,
      $profile->status,
      implode(' | ', $actions),
    );
  }

  $build['profiles_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'membersify-payment-profiles-table'),
    '#empty' => t('No payment methods yet.')
  );

  $build['add_card'] = drupal_get_form('membersify_add_payment_profile_form', $account);

  return $build;
}

/**
 * Form constructor for the add payment profile form.
 */
function membersify_add_payment_profile_form($form, $form_state, $account) {
  $form['#id'] = 'membersify-add-card-button-form';
  $form['#account'] = $account;
  $form['actions'] = array('#type' => 'actions');

  // Include the js from stripe.com.
  $form['#attached']['js'][] = array(
    'data' => 'https://checkout.stripe.com/checkout.js',
    'type' => 'external'
  );

  $form['#attached']['js'][] = array(
    'data' => array('stripe' => array(
      'publicKey' => variable_get('membersify_stripe_account', ''),
      'email' => $account->mail,
      'label' => t('Add card'),
    )),
    'type' => 'setting'
  );
  $form['#attached']['js'][] = drupal_get_path('module', 'membersify') . '/js/membersify.stripe.js';
  $form['#attached']['css'][] = drupal_get_path('module', 'membersify') . '/css/membersify.stripe.css';
  $form['#attached']['css'][] = drupal_get_path('module', 'membersify') . '/css/membersify.membersify.payment.css';

  $form['token_field'] = array(
    '#type' => 'hidden',
    '#attributes' => array(
      'class' => array('membersify_add_card_token_field'),
    ),
  );

  $form['actions']['payment_button'] = array(
    '#type' => 'markup',
    '#markup' => "<a id='membersify_add_card_button' href='#' class='stripe-connect'><span>" . t("Add new card") . "</span></a>"
  );

  // We need a button or else jQuery submit won't work. But it will be hidden.
  $form['submit'] = array(
    '#type' => 'submit',
    '#prefix' => "<div id='membersify-payment-submit-button-hidden'>",
    '#suffix' => '</div>',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Form validation handler for membersify_add_payment_profile_form().
 */
function membersify_add_payment_profile_form_validate($form, &$form_state) {
  try {
    $account = $form['#account'];
    $params = array(
      'user_id' => $account->uid,
      'email' => $account->mail,
    );

    if (!empty($form_state['values']['token_field'])) {
      $params['token'] = $form_state['values']['token_field'];
    }

    Membersify::setKeys(variable_get('membersify_public_key', ''), variable_get('membersify_secret_key', ''));
    $profile = Membersify_Profile::create($params);

    membersify_save_record('membersify_payment_profiles', $profile);
  }
  catch (Exception $e) {
    form_set_error('submit', t("There was an error. Please contact the site administrator. Error: @error", array('@error' => $e->getMessage())));
  }
}

/**
 * Form submission handler for membersify_add_payment_profile_form().
 */
function membersify_add_payment_profile_form_submit($form, &$form_state) {
  $account = $form['#account'];
  $form_state['redirect'] = 'user/' . $account->uid . '/payment_methods';
}

/**
 * Form constructor for the change plan confirm form.
 *
 * Confirm that the user wants to pay $X to change their plan, and confirm the details.
 *
 * @param $account
 *   The user account associated with the subscription.
 * @param Membersify_Subscription $subscription
 *   The subscription to modify.
 * @param Membersify_Plan $plan
 *   The new plan to modify to.
 */
function membersify_change_plan_confirm_form($form, $form_state, $account, Membersify_Subscription $subscription, Membersify_Plan $plan) {
  $message = t('Yes, change my plan to @plan.', array('@plan' => $plan->name));

  if ($subscription->plan_id == $plan->id) {
    drupal_set_message(t("You are already on this plan."));
    drupal_goto();
  }

  $old_plan = msc_plan_load($subscription->plan_id);

  if (in_array($plan->machine_name, array_filter($old_plan->data['modify_options']['upgrade']))) {
    // This is an upgrade, so we might need to change the subscription immediately.
    // First, calculate the credit from their current plan.
    if ($plan->payment_plan['main_amount'] > 0) {
      // Check to see if there is enough credit in the current subscription to
      // cover the cost of the new plan.
      $credit = membersify_calculate_credit($subscription);
      $outstanding = $plan->payment_plan['main_amount'] - $credit;

      if ($outstanding > 0) {
        // If the user doesn't have a card yet, and they need to pay $X immediately,
        // we ask them to add a card first.
        $payment_profiles = membersify_get_payment_profiles($subscription->user_id);
        if (empty($payment_profiles)) {
          // Show a message and redirect to the add card page.
          drupal_set_message(t("You must first add a valid payment method."));
          drupal_goto('user/' . $subscription->user_id . '/payment_methods');
        }

        // Show the card that is being charged.
        $form['payment_profile'] = array(
          '#type' => 'radios',
          '#title' => t("Payment method"),
          '#options' => membersify_list($payment_profiles, 'display'),
          '#required' => TRUE,
          '#default_value' => key($payment_profiles),
        );

        // We need to inform the user of the charge.
        $message = t('Yes, pay @amount and change my plan to @plan.',
          array('@amount' => membersify_format_money($outstanding), '@plan' => $plan->name));
      }
    }
  }
  else if (in_array($plan->machine_name, array_filter($old_plan->data['modify_options']['downgrade']))) {
    // We don't need to do anything special here for downgrades.
  }
  else {
    drupal_set_message(t("Invalid plan option."));
    drupal_goto();
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $message,
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Wait, I changed my mind.'), 'user/' . $account->uid . '/subscription'),
  );

  return $form;
}

/**
 * Form validation handler for membersify_change_plan_confirm_form().
 */
function membersify_change_plan_confirm_form_validate($form, $form_state) {
  try {
    $subscription = $form_state['build_info']['args'][1];
    $plan = $form_state['build_info']['args'][2];

    $profile_id = NULL;
    if (!empty($form_state['values']['payment_profile'])) {
      $profile_id = $form_state['values']['payment_profile'];
    }

    Membersify::setKeys(variable_get('membersify_public_key', ''), variable_get('membersify_secret_key', ''));
    $subscription->modify($plan->id, $profile_id);

    drupal_set_message(t("Your plan has been changed to @name.", array('@name' => $plan->name)));
  }
  catch (Exception $e) {
    form_set_error('submit', $e->getMessage());
  }
}
<?php
/**
 * @file
 * Menu callbacks for the Recurly.js module.
 */

/**
 * Form callback; Modifies the Recurly form at admin/config/services/recurly.
 */
function _recurlyjs_form_recurly_settings_form_alter(&$form, &$form_state) {
  $form['recurlyjs'] = array(
    '#title' => t('Recurly.js settings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
  );
  $form['recurlyjs']['recurlyjs_address_requirement'] = array(
    '#title' => t('Address requirement level'),
    '#type' => 'select',
    '#options' => array(
      'full' => t('Full'),
      'zipstreet' => t('Zip code and street'),
      'zip' => t('Zip code only'),
      'none' => t('None'),
    ),
    '#default_value' => variable_get('recurlyjs_address_requirement', 'full'),
    '#description' => t('Choose the level of address information required. Collecting more address information reduces the probability of fraudulent accounts. This setting should match the address requirements as configured on !link in "Site Settings".', array('!link' => variable_get('recurly_subdomain', '') ? l(t('your Recurly account'), recurly_hosted_url('configuration/edit')) : t('your Recurly account'))),
  );
  $form['recurlyjs']['recurlyjs_enable_add_ons'] = array(
    '#title' => t('Enable Add-ons'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('recurlyjs_enable_add_ons', 1),
    '#description' => t('Add-ons are options that may be displayed in addition to the base subscription. If a subscription does not have add-ons, nothing additional will be displayed.'),
  );
  $form['recurlyjs']['recurlyjs_enable_coupons'] = array(
    '#title' => t('Enable coupons'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('recurlyjs_enable_coupons', 0),
    '#description' => t('Display the <em>Coupon Code</em> field on Recurly.js subscription forms.'),
  );
  $form['recurlyjs']['recurlyjs_accept_paypal'] = array(
    '#title' => t('Enable PayPal'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('recurlyjs_accept_paypal', FALSE),
    '#description' => t('Show an option to pay with PayPal. This requires a PayPal Business account with Referential Transactions enabled. <a href="https://docs.recurly.com/payment-gateways/paypal-payments">See the Recurly Paypal documentation</a>.'),
  );
  $jquery_requirement = FALSE;
  if (module_exists('jquery_update')) {
    $jquery_version = variable_get('jquery_update_jquery_version', '1.10');
    if (version_compare('1.7', $jquery_version) < 0) {
      $jquery_requirement = TRUE;
    }
  }
  if (!$jquery_requirement) {
    $form['recurlyjs']['recurlyjs_accept_paypal']['#disabled'] = TRUE;
    $form['recurlyjs']['recurlyjs_accept_paypal']['#value'] = FALSE;
    $form['recurlyjs']['recurlyjs_accept_paypal']['#description'] .= ' <strong>' . t('To enable this feature please install <a href="http://drupal.org/project/jquery_update">jQuery Update</a> and use jQuery 1.7 or higher.') . '</strong>';
  }
}

/**
 * Menu callback; Display the page for starting a subscription.
 */
function recurlyjs_subscribe_page($entity_type, $entity, $plan_code, $currency = NULL) {
  // Initialize the Recurly client with the site-wide settings.
  if (!recurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  // Ensure the account does not already have this exact same plan. Recurly
  // does not support a single account having multiple of the same plan.
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $local_account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $id), TRUE);
  if ($local_account) {
    $current_subscriptions = recurly_account_get_subscriptions($local_account->account_code, 'active');

    // If the account is only allowed one subscription total, they shouldn't
    // ever see this signup page.
    if (variable_get('recurly_subscription_max', '1') === '1' && count($current_subscriptions) && empty($_POST)) {
      $current_subscription = reset($current_subscriptions);
      drupal_set_message(t('This account already has a @plan plan!', array('@plan' => $current_subscription->plan->name)));
      drupal_goto($entity_type . '/' . $id . '/subscription');
    }

    // Otherwise check if they already have one of this same plan.
    foreach ($current_subscriptions as $current_subscription) {
      if ($current_subscription->plan->plan_code === $plan_code && empty($_POST)) {
        drupal_set_message(t('This account already has a @plan plan!', array('@plan' => $current_subscription->plan->name)));
        drupal_goto($entity_type . '/' . $id . '/subscription/signup');
      }
    }
  }
  // Although this menu callback contains little else besides the subscription
  // form, it's a separate function because it's highly likely to need theming.
  $form = drupal_get_form('recurlyjs_subscribe_form', $entity_type, $entity, $plan_code, $currency);
  try {
    $plan = Recurly_Plan::get($plan_code);
  }
  catch (Recurly_NotFoundError $e) {
    return MENU_NOT_FOUND;
  }
  return theme('recurlyjs_subscribe_page', array('form' => $form));
}

/**
 * Form callback; Display the form for starting a subscription.
 */
function recurlyjs_subscribe_form($form, $form_state, $entity_type, $entity, $plan_code, $currency = NULL) {
  // Initialize the Recurly client with the site-wide settings.
  if (!recurly_client_initialize()) {
    $form['error'] = array(
      '#markup' => t('Could not initialize the Recurly client.'),
    );
    return $form;
  }

  $form['#entity_type'] = $entity_type;
  $form['#entity'] = $entity;

  $currency_list = array();
  $plan = Recurly_Plan::get($plan_code);
  foreach ($plan->unit_amount_in_cents as $unit_currency) {
    $currency_list[] = $unit_currency->currencyCode;
  }
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $id), TRUE);
  $form['update'] = array(
    '#type' => 'recurlyjs_subscribe',
    '#plan_code' => $plan_code,
    '#currency' => in_array($currency, $currency_list) ? $currency : variable_get('recurly_default_currency', 'USD'),
    '#account_code' => isset($account->account_code) ? $account->account_code : $entity_type . '-' . $id,
    '#username' => entity_label($entity_type, $entity),
    '#address_requirement' => variable_get('recurlyjs_address_requirement', 'full'),
    '#accept_paypal' => variable_get('recurlyjs_accept_paypal', FALSE),
    '#enable_coupons' => variable_get('recurlyjs_enable_coupons', 0),
    '#enable_add_ons' => variable_get('recurlyjs_enable_add_ons', 1),
  );

  // Prepopulate fields based on the entity's tokens.
  $mapping = recurly_token_mapping();
  foreach ($mapping as $recurly_field => $token) {
    $value = token_replace($token, array($entity_type => $entity), array('clear' => TRUE, 'sanitize' => FALSE));
    $properties = element_info('recurlyjs_subscribe');
    if ($value) {
      // Some properties are keyed #account_ and some #billing_.
      if (array_key_exists('#account_' . $recurly_field, $properties)) {
        $form['update']['#account_' . $recurly_field] = $value;
      }
      elseif (array_key_exists('#billing_' . $recurly_field, $properties)) {
        $form['update']['#billing_' . $recurly_field] = $value;
      }
    }
  }

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Purchase'),
  );

  return $form;
}

/**
 * Submit handler for recurlyjs_subscribe_form().
 */
function recurlyjs_subscribe_form_submit($form, &$form_state) {
  $subscription = $form_state['recurly_result'];
  $entity_type = $form['#entity_type'];
  $entity = $form['#entity'];

  drupal_set_message(t('Account upgraded to @plan!', array('@plan' => $subscription->plan->name)));
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  // Save the account locally immediately so that subscriber information may
  // be retrieved when the user is directed back to the /subscription tab.
  try {
    $account = $subscription->account->get();
    recurly_account_save($account, $entity_type, $id);
  }
  catch (Recurly_Error $e) {
    watchdog('recurly', 'New subscriber account could not be retreived from Recurly. Received the following error: @error', array('@error' => $e->getMessage()));
  }

  $form_state['redirect'] = $entity_type . '/' . $id . '/subscription';
}

/**
 * Form callback; Display the form for updating billing information.
 */
function recurlyjs_update_billing($form, $form_state, $entity_type, $entity) {
  // Initialize the Recurly client with the site-wide settings.
  if (!recurly_client_initialize()) {
    $form['error'] = array(
      '#markup' => t('Could not initialize the Recurly client.'),
    );
    return $form;
  }

  // See if we have a local mapping of entity ID to Recurly account code.
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $account = recurly_account_load(array('entity_type' => $entity_type, 'entity_id' => $id));

  try {
    $billing_info = Recurly_BillingInfo::get($account->account_code);
    $form['existing'] = array(
      '#theme' => 'recurly_credit_card_information',
      '#card_type' => check_plain($billing_info->card_type),
      '#first_name' => check_plain($billing_info->first_name),
      '#last_name' => check_plain($billing_info->last_name),
      '#year' => check_plain($billing_info->year),
      '#month' => check_plain($billing_info->month),
      '#mask_length' => strcasecmp($billing_info->card_type, 'American Express') === 0 ? 11 : 12,
      '#last_four' => check_plain($billing_info->last_four),
    );
  } catch (Recurly_NotFoundError $e) {

  }

  $form['update'] = array(
    '#type' => 'recurlyjs_billing',
    '#account_code' => $account->account_code,
    '#address_requirement' => variable_get('recurlyjs_address_requirement', 'full'),
    '#accept_paypal' => variable_get('recurlyjs_accept_paypal', FALSE),
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  return $form;
}

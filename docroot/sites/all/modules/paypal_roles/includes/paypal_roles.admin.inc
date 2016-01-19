<?php

/**
 * @file
 * Administrative page callbacks for the paypal_roles module.
 */

/**
 * Callback function to list all custom payments.
 */
function paypal_roles_custom_payments_list() {
  $css = drupal_get_path('module', 'paypal_roles') . '/css/paypal_roles.css';
  drupal_add_css($css);
  return views_embed_view('custom_payments', 'default');
}

/**
 * Callback function to list all payments.
 */
function paypal_roles_payments_list() {
  $css = drupal_get_path('module', 'paypal_roles') . '/css/paypal_roles.css';
  drupal_add_css($css);
  return views_embed_view('payments', 'default');
}

/**
 * Callback function to list all memberships.
 */
function paypal_roles_memberships_list() {
  $css = drupal_get_path('module', 'paypal_roles') . '/css/paypal_roles.css';
  drupal_add_css($css);
  return views_embed_view('memberships', 'default');
}

/**
 * Configuration form to manage custom payments.
 */
function paypal_roles_custom_payments_form($form, &$form_state, $payment = NULL) {
  $form = array();

  if (isset($payment->pid) && (int) $payment->pid > 0) {
    $form['pid'] = array(
      '#type' => 'hidden',
      '#value' => $payment->pid,
    );
  }

  if (isset($payment->item_id)) {
    $form['item_id'] = array(
      '#type' => 'hidden',
      '#value' => $payment->item_id,
    );
  }

  $form['uid'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#default_value' => isset($payment->uid) ? user_load($payment->uid)->name : '',
    '#autocomplete_path' => 'user/autocomplete',
    '#required' => TRUE,
    '#weight' => 1,
  );

  $roles = user_roles();
  // Removes anonymous user.
  unset($roles[1]);
  // Removes authenticated user.
  unset($roles[2]);

  $form['role'] = array(
    '#type' => 'select',
    '#title' => t('Grant access to this role'),
    '#options' => $roles,
    '#default_value' => isset($payment->role) ? $payment->role : 1,
    '#description' => t('Select a role, which would be provided for user.'),
    '#required' => TRUE,
    '#weight' => 2,
  );

  $form['date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Start date of custom subscription'),
    '#description' => t('Expiration date is calculated from this date.'),
    '#default_value' => isset($payment->date) ? date("Y-m-d", $payment->date) : date("Y-m-d"),
    '#date_format' => 'Y-m-d',
    '#date_label_position' => 'within',
    '#required' => TRUE,
    '#weight' => 3,
  );

  $form['length_container'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('container-inline', 'length-container'),
    ),
    '#weight' => 4,
  );

  $form['length_container']['length'] = array(
    '#type' => 'textfield',
    '#title' => t('Length of custom subscription'),
    '#default_value' => isset($payment->length) ? $payment->length : 1,
    '#size' => 5,
    '#required' => TRUE,
    '#element_validate' => array('element_validate_number'),
  );

  $form['length_container']['length_type'] = array(
    '#type' => 'select',
    '#description' => t('The length a user has access to the role can be set to a specified number of Days, Weeks, Months or Years.') . '<br>' . t('If you want to create a "No expires" access, set this value to 0.'),
    '#options' => array(
      'D' => t('day(s)'),
      'W' => t('week(s)'),
      'M' => t('month(s)'),
      'Y' => t('year(s)'),
    ),
    '#default_value' => isset($payment->length_type) ? $payment->length_type : 'M',
  );

  $form['amount_container'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('container-inline', 'amount-container'),
    ),
    '#weight' => 5,
  );

  $form['amount_container']['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount'),
    '#default_value' => isset($payment->amount) ? $payment->amount : 0,
    '#size' => 5,
    '#element_validate' => array('element_validate_number'),
  );

  $form['amount_container']['currency'] = array(
    '#type' => 'select',
    '#options' => paypal_roles_supported_currencies(),
    '#default_value' => isset($payment->currency) ? $payment->currency : 'USD',
    '#description' => t('The amount of payment.'),
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit_form'] = array(
    '#type' => 'submit',
    '#value' => t('Save Custom Payment'),
  );

  $form['#attributes']['class'][] = 'paypal-roles-custom-payments-form';

  $form['#prefix'] = '<div id="paypal-roles-custom-payments-form-wrapper">';
  $form['#suffix'] = '</div>';

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'paypal_roles') . '/css/paypal_roles.css',
  );

  return $form;
}

/**
 * Submit handler for Membership Level saving.
 */
function paypal_roles_custom_payments_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-submit-form') {
    $payment = _paypal_roles_convert_form_to_custom_payments($form_state);

    $exists = paypal_roles_custom_payments_load($payment->pid);
    if ($exists && empty($exists->in_code_only)) {
      if (paypal_roles_custom_payments_update($payment)) {
        drupal_set_message(t('The Custom Payment instance has been updated.'));
      }
    }
    else {
      if (paypal_roles_custom_payments_save($payment)) {
        drupal_set_message(t('The Custom Payment instance has been created.'));
      }
    }

    paypal_roles_manage_roles_by_users($payment->uid);

    $form_state['redirect'] = 'admin/paypal-roles/custom-payments';
  }
}

/**
 * Deletion of Membership Levels.
 */
function paypal_roles_custom_payments_delete_form($form, $form_state, $payment) {
  $form['pid'] = array(
    '#type' => 'hidden',
    '#value' => $payment->pid,
  );

  return confirm_form($form, t('Are you sure you want to delete this Custom Payment: %pid?', array('%pid' => $payment->pid)), 'admin/paypal-roles/custom-payments', '', t('Delete'), t('Cancel'));
}

/**
 * Submit handler for Membership Levels deletion.
 */
function paypal_roles_custom_payments_delete_form_submit($form, &$form_state) {
  paypal_roles_custom_payments_delete($form_state['values']['pid']);
  drupal_set_message(t('The Custom Payment instance (%pid) has been removed.', array('%pid' => $form_state['values']['pid'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/paypal-roles/custom-payments';
}

/**
 * Configuration form to manage Membership Levels.
 */
function paypal_roles_memberships_form($form, &$form_state, $ml = NULL) {
  $form = array();

  $form['name'] = array(
    '#title' => t('Name'),
    '#description' => t('This will appear as the name of Membership Package.'),
    '#type' => 'textfield',
    '#default_value' => isset($ml->name) ? $ml->name : '',
    '#weight' => 0,
    '#required' => TRUE,
  );

  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'paypal_roles_memberships_machine_name_exists',
      'source' => array('name'),
    ),
    '#description' => t('A unique machine-readable name for this Membership Package. It must only contain lowercase letters, numbers, and underscores. The machine name will be used internally by PayPal Role.'),
    '#weight' => 1,
  );

  if (isset($ml->machine_name) && !empty($ml->machine_name)) {
    $pmts = paypal_roles_payments_load_conditional(array(
      'item_id' => $ml->machine_name,
    ));

    $pmts_custom = paypal_roles_custom_payments_load_conditional(array(
      'item_id' => $ml->machine_name,
    ));

    if (count($pmts) > 0 || count($pmts_custom) > 0) {
      $disabled = TRUE;
    }
    else {
      $disabled = FALSE;
    }

    $form['machine_name']['#default_value'] = $ml->machine_name;
    $form['machine_name']['#disabled'] = TRUE;
    $form['machine_name']['#value'] = $ml->machine_name;
  }
  else {
    $disabled = FALSE;
  }

  // Initialise roles list. Add placeholder with zero value to force admins to select a role
  $roles[0] = t('-- Select Role --');
  $roles += user_roles();
  // Removes anonymous user.
  unset($roles[1]);
  // Removes authenticated user.
  unset($roles[2]);

  $form['role'] = array(
    '#type' => 'select',
    '#title' => t('Membership Role'),
    '#options' => $roles,
    '#default_value' => isset($ml->role) ? $ml->role : 1,
    '#disabled' => $disabled,
    '#description' => t('Select a role, which would be provided for subscribers.'),
    '#weight' => 2,
  );

  $form['button_type'] = array(
    '#type' => 'select',
    '#title' => t('Payment type'),
    '#options' => array(
      0 => t('One-time purchase'),
      1 => t('Recurring payment'),
    ),
    '#default_value' => isset($ml->button_type) ? $ml->button_type : 1,
    '#description' => t('Button type. Subscription or one-time purchase.'),
    '#disabled' => $disabled,
  );

  $form['length_container'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('container-inline', 'length-container'),
    ),
    '#weight' => 3,
  );

  $form['length_container']['length'] = array(
    '#type' => 'textfield',
    '#title' => t('Length of Subscription'),
    '#default_value' => isset($ml->length) ? $ml->length : 1,
    '#size' => 5,
    '#required' => TRUE,
    '#element_validate' => array('element_validate_number'),
    // '#disabled' => $disabled,
  );

  $form['length_container']['length_type'] = array(
    '#type' => 'select',
    '#options' => array(
      'D' => t('day(s)'),
      'W' => t('week(s)'),
      'M' => t('month(s)'),
      'Y' => t('year(s)'),
    ),
    '#default_value' => isset($ml->length_type) ? $ml->length_type : 'M',
    // '#disabled' => $disabled,
  );

  $form['length_container']['length_desc'] = array(
    '#type' => 'item',
    '#description' => t('The length a Member has access to the Membership can be set to a specified number of Days, Weeks, Months or Years.') . '<br>' . t('If you want to create a simple one-time purchase button with no-expiration time, set this value to 0.'),
  );

  $form['amount_container'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('container-inline', 'amount-container'),
    ),
    '#weight' => 5,
  );

  $form['amount_container']['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount'),
    '#default_value' => isset($ml->amount) ? $ml->amount : 1,
    '#size' => 5,
    '#required' => TRUE,
    '#element_validate' => array('element_validate_number'),
    // '#disabled' => $disabled,
  );

  $form['amount_container']['currency'] = array(
    '#type' => 'select',
    '#options' => paypal_roles_supported_currencies(),
    '#default_value' => isset($ml->currency) ? $ml->currency : 'USD',
    // '#disabled' => $disabled,
    '#description' => t('The amount of subscription / one-time purchase.') . '<br />' . t('<b>HUF, JPY, TWD:</b> These currencies do not support decimals. Passing a decimal amount will throw an error.'),
    '#ajax' => array(
      'callback' => 'paypal_roles_memberships_form_ajax_callback',
      'wrapper' => 'paypal-roles-memberships-form-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['discount_container'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('container-inline', 'discount-container'),
    ),
    '#weight' => 6,
    '#states' => array(
      'visible' => array(
        ':input[name="button_type"]' => array('value' => '0'),
      ),
    ),
  );

  $form['discount_container']['discount_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Discount rate'),
    '#default_value' => isset($ml->discount_rate) ? $ml->discount_rate : 0,
    '#size' => 5,
    '#element_validate' => array('element_validate_number'),
    // '#disabled' => $disabled,
    '#suffix' => '%',
  );

  $discount_roles = array(
    0 => t('- everyone -'),
  );
  $discount_roles += user_roles();
  // Removes anonymous user.
  unset($discount_roles[1]);
  // Removes authenticated user.
  unset($discount_roles[2]);

  $form['discount_container']['discount_role'] = array(
    '#type' => 'select',
    '#options' => $discount_roles,
    '#default_value' => isset($ml->discount_role) ? $ml->discount_role : 1,
    // '#disabled' => $disabled,
    '#weight' => 2,
    '#prefix' => ' ' . t('only for this role:'),
    '#description' => t('PayPal allows discount rate only for one-time purchase.'),
  );

  $form['trial_length_container'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('container-inline', 'trial-length-container'),
    ),
    '#weight' => 7,
    '#states' => array(
      'invisible' => array(
        ':input[name="button_type"]' => array('value' => '0'),
      ),
    ),
  );

  $curr = (isset($form_state['values']['currency']) ? $form_state['values']['currency'] : (isset($ml->currency) ? $ml->currency : 'USD'));

  $form['trial_length_container']['trial_amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount and length of Trial Period'),
    '#default_value' => isset($ml->trial_amount) ? $ml->trial_amount : 1,
    '#size' => 5,
    '#required' => TRUE,
    '#element_validate' => array('element_validate_number'),
    // '#disabled' => $disabled,
    '#suffix' => $curr . ' ' . t('for'),
  );

  $form['trial_length_container']['trial_length'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($ml->trial_length) ? $ml->trial_length : 0,
    '#size' => 5,
    '#required' => TRUE,
    '#element_validate' => array('element_validate_number'),
    // '#disabled' => $disabled,
  );

  $form['trial_length_container']['trial_length_type'] = array(
    '#type' => 'select',
    '#description' => t('The trial length a Member has access to the Membership can be set to a specified number of Days, Weeks, Months or Years.') . '<br>' . t('If you don\'t want to set a trial period, just set the value of length to 0.'),
    '#options' => array(
      'D' => t('day(s)'),
      'W' => t('week(s)'),
      'M' => t('month(s)'),
      'Y' => t('year(s)'),
    ),
    '#default_value' => isset($ml->trial_length_type) ? $ml->trial_length_type : 'M',
    // '#disabled' => $disabled,
  );

  $form['cycles'] = array(
    '#type' => 'textfield',
    '#title' => t('Billing cycles'),
    '#description' => t('Set recurring payments to stop after x billing cycles. 0 means that the subscription ends only when the merchant or subscriber cancel it.'),
    '#default_value' => isset($ml->cycles) ? $ml->cycles : 0,
    '#size' => 5,
    '#required' => TRUE,
    '#element_validate' => array('element_validate_number'),
    // '#disabled' => $disabled,
    '#weight' => 8,
    '#states' => array(
      'invisible' => array(
        ':input[name="button_type"]' => array('value' => '0'),
      ),
    ),
  );

  $form['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => array(
      1 => t('Active'),
      0 => t('Inactive'),
    ),
    '#default_value' => isset($ml->status) ? $ml->status : 1,
    '#weight' => 9,
    '#description' => t('Subscription button is active (visible on website) or not.'),
  );

  $form['description'] = array(
    '#type' => 'text_format',
    '#title' => t('Description'),
    '#default_value' => isset($ml->description) ? $ml->description : '',
    '#format' => (isset($ml->description_format) && !empty($ml->description_format)) ? $ml->description_format : 'filtered_html',
    '#weight' => 10,
    '#description' => t('Description of membership. This text will appear in the subscription block.'),
  );

  $form['language'] = array(
    '#type' => 'select',
    '#title' => t('PayPal login page language / locale'),
    '#options' => paypal_roles_languages(),
    '#default_value' => isset($ml->language) ? $ml->language : 'N/A',
    '#description' => t('The locale of the login or sign-up page, which may have the specific country\'s language available, depending on localization.'),
    '#weight' => 11,
  );

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit_form'] = array(
    '#type' => 'submit',
    '#value' => t('Save Membership'),
  );

  $form['#attributes']['class'][] = 'paypal-roles-memberships-form';

  $form['#prefix'] = '<div id="paypal-roles-memberships-form-wrapper">';
  $form['#suffix'] = '</div>';

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'paypal_roles') . '/css/paypal_roles.css',
  );

  return $form;
}

/**
 * Ajax callback to set currency code on trial amount field.
 */
function paypal_roles_memberships_form_ajax_callback($form, $form_state) {
  return $form;
}

/**
 * Validate handler for Membership Level saving.
 */
function paypal_roles_memberships_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  if (isset($values['cycles']) && (int) $values['cycles'] === 1) {
    form_set_error('cycles', t('Billing cycles cannot be 1. Set billing cycles to 0, or set a value, which is higher than 1.'));
  }

  if (isset($values['button_type']) && (int) $values['button_type'] === 1) {
    if (isset($values['length']) && (int) $values['length'] === 0) {
      form_set_error('length', t('Length of subscription cannot be 0. Value 0 is allowed only for one-time purchase.'));
    }
  }

  if (isset($values['amount'])) {
    if((strlen(strrchr($values['amount'], '.'))-1) > 2){
      form_set_error('amount', t('Please set the Amount value as a number rounded to 2 decimal places.'));
    }
  }

  if (isset($values['trial_amount'])) {
    if((strlen(strrchr($values['trial_amount'], '.'))-1) > 2){
      form_set_error('trial_amount', t('Please set the Amount of Trial Period value as a number rounded to 2 decimal places.'));
    }
  }

  if (isset($values['role']) && $values['role'] == 0) {
      form_set_error('role', t('Please select a role from the Membership Role list.'));
  }
}

/**
 * Submit handler for Membership Level saving.
 */
function paypal_roles_memberships_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-submit-form') {
    $ml = _paypal_roles_convert_form_to_membership($form_state);

    $exists = paypal_roles_memberships_load($ml->machine_name);
    if ($exists && empty($exists->in_code_only)) {
      if (paypal_roles_memberships_update($ml)) {
        drupal_set_message(t('The Membership instance has been updated.'));
      }
    }
    else {
      if (paypal_roles_memberships_save($ml)) {
        drupal_set_message(t('The Membership instance has been created.'));
      }
    }
    $form_state['redirect'] = 'admin/paypal-roles/memberships';
  }
}

/**
 * Cancel a payment.
 */
function paypal_roles_payments_cancel_form($form, $form_state, $pm) {
  $form['txn_id'] = array(
    '#type' => 'hidden',
    '#value' => $pm->txn_id,
  );

  return confirm_form($form, t('Are you sure you want to cancel this payment: %txn_id?', array('%txn_id' => $pm->txn_id)), 'admin/paypal-roles/payments', '', t('Yes'), t('No'));
}

/**
 * Submit handler for payment cancellation.
 */
function paypal_roles_payments_cancel_form_submit($form, &$form_state) {
  $txn_id = $form_state['values']['txn_id'];
  $pm = paypal_roles_payments_load($txn_id);

  if (isset($pm->cancelled)) {
    $pm->cancelled = 1;
    paypal_roles_payments_save($pm);

    // Update roles by user.
    paypal_roles_manage_roles_by_users($pm->uid);
  }

  drupal_set_message(t('The payment instance %txn_id has been cancelled.', array('%txn_id' => $txn_id)));

  $form_state['redirect'] = 'admin/paypal-roles/payments';
}

/**
 * Un-cancel a payment.
 */
function paypal_roles_payments_uncancel_form($form, $form_state, $pm) {
  $form['txn_id'] = array(
    '#type' => 'hidden',
    '#value' => $pm->txn_id,
  );

  return confirm_form($form, t('Are you sure you want to uncancel this payment: %txn_id?', array('%txn_id' => $pm->txn_id)), 'admin/paypal-roles/payments', '', t('Yes'), t('No'));
}

/**
 * Submit handler for payment un-cancellation.
 */
function paypal_roles_payments_uncancel_form_submit($form, &$form_state) {
  $txn_id = $form_state['values']['txn_id'];
  $pm = paypal_roles_payments_load($txn_id);

  if (isset($pm->cancelled)) {
    $pm->cancelled = 0;
    paypal_roles_payments_save($pm);

    // Update roles by user.
    paypal_roles_manage_roles_by_users($pm->uid);
  }

  drupal_set_message(t('The payment instance %txn_id has been uncancelled.', array('%txn_id' => $txn_id)));

  $form_state['redirect'] = 'admin/paypal-roles/payments';
}

/**
 * Deletion of Membership Levels.
 */
function paypal_roles_memberships_delete_form($form, $form_state, $ml) {
  $form['machine_name'] = array(
    '#type' => 'hidden',
    '#value' => $ml->machine_name,
  );

  $form['name'] = array(
    '#type' => 'hidden',
    '#value' => $ml->name,
  );

  return confirm_form($form, t('Are you sure you want to delete this Membership Level: %name?', array('%name' => $ml->name)), 'admin/paypal-roles/memberships', '', t('Delete'), t('Cancel'));
}

/**
 * Submit handler for Membership Levels deletion.
 */
function paypal_roles_memberships_delete_form_submit($form, &$form_state) {
  paypal_roles_memberships_delete($form_state['values']['machine_name']);
  drupal_set_message(t('The membership instance %name has been removed.', array('%name' => $form_state['values']['name'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/paypal-roles/memberships';
}

/**
 * Configuration form for controlling the paypal_roles behaviour.
 */
function paypal_roles_settings_form() {
  global $base_url, $base_path;

  $return = $base_url . $base_path;
  $return_cancel = $base_url . $base_path;

  $form = array();

  $form['paypal'] = array(
    '#type' => 'fieldset',
    '#title' => t('PayPal settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['paypal']['sandbox'] = array(
    '#type' => 'fieldset',
    '#title' => t('PayPal (Sandbox) settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['paypal']['sandbox']['paypal_roles_s_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Primary Email Address'),
    '#default_value' => variable_get('paypal_roles_s_email', ''),
  );

  $form['paypal']['sandbox']['paypal_roles_s_return'] = array(
    '#type' => 'textfield',
    '#title' => t('Return URL'),
    '#default_value' => variable_get('paypal_roles_s_return', $return),
  );

  $form['paypal']['sandbox']['paypal_roles_s_return_cancel'] = array(
    '#type' => 'textfield',
    '#title' => t('Return URL (cancel)'),
    '#default_value' => variable_get('paypal_roles_s_return_cancel', $return_cancel),
  );

  $form['paypal']['live'] = array(
    '#type' => 'fieldset',
    '#title' => t('PayPal (Live) settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['paypal']['live']['paypal_roles_l_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Primary Email Address'),
    '#default_value' => variable_get('paypal_roles_l_email', ''),
  );

  $form['paypal']['live']['paypal_roles_l_return'] = array(
    '#type' => 'textfield',
    '#title' => t('Return URL'),
    '#default_value' => variable_get('paypal_roles_l_return', $return),
  );

  $form['paypal']['live']['paypal_roles_l_return_cancel'] = array(
    '#type' => 'textfield',
    '#title' => t('Return URL (cancel)'),
    '#default_value' => variable_get('paypal_roles_l_return_cancel', $return_cancel),
  );

  $form['paypal']['paypal_roles_sandbox'] = array(
    '#type' => 'select',
    '#title' => t('Sandbox Mode'),
    '#description' => t('Switch between live and sandbox.'),
    '#options' => array(
      0 => t('Disabled (live)'),
      1 => t('Enabled'),
    ),
    '#default_value' => variable_get('paypal_roles_sandbox', 1),
  );

  $form['paypal']['paypal_roles_validate_primary_email'] = array(
    '#type' => 'select',
    '#title' => t('Validate Primary Email Address'),
    '#description' => t('Validate Primary Email Address on IPN messages.'),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
    '#default_value' => variable_get('paypal_roles_validate_primary_email', 1),
  );

  $form['paypal']['paypal_roles_log_failed_ipn'] = array(
    '#type' => 'select',
    '#title' => t('Logging failed IPNs'),
    '#description' => t('Logging failed IPNs using watchdog.'),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
    '#default_value' => variable_get('paypal_roles_log_failed_ipn', 1),
  );

  $form['paypal']['paypal_roles_payment_grace_period'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment grace period'),
    '#default_value' => variable_get('paypal_roles_payment_grace_period', 86400),
    '#description' => t('Payment grace period in seconds.'),
    '#size' => 5,
  );

  return system_settings_form($form);
}

/**
 * Configuration form for controlling the paypal_roles behaviour.
 */
function paypal_roles_account_selection_form($form) {
  $step_names = array(
    0 => t('Try to load account by using Payer or Subscriber ID (payer_id or subscr_id)'),
    1 => t('Try to load account by using the email of logged in (Drupal) user'),
    2 => t('Try to load account by using the email, which is provided by PayPal (payer_email)'),
  );

  $steps = variable_get('paypal_roles_account_selection', array(0, 1, 2));

  $form['#tree'] = TRUE;

  $weight = 0;
  foreach ($steps as $step) {
    $form[$step]['name'] = array(
      '#markup' => $step_names[$step],
    );

    $form[$step]['item'] = array(
      '#type' => 'hidden',
      '#value' => $step,
    );

    $form[$step]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#title_display' => 'invisible',
      '#default_value' => $weight,
      '#delta' => 3,
    );

    $weight++;
  }

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Changes'),
  );

  return $form;
}

/**
 * Submit handler for account selection overview. Updates changed ordering
 * weights.
 */
function paypal_roles_account_selection_form_submit($form, &$form_state) {
  $new_ordering = array();

  foreach ($form_state['values'] as $key => $value) {
    if (is_numeric($key)) {
      $item = $value['item'];
      $weight = $value['weight'];

      $new_ordering[$weight] = $item;
    }
  }

  ksort($new_ordering);
  $ordered_keys = array_values($new_ordering);

  variable_set('paypal_roles_account_selection', $ordered_keys);
}

/**
 * Helper function to convert the data on admin form into membership presentation.
 */
function _paypal_roles_convert_form_to_custom_payments($form_state) {
  $payment = new stdClass();

  if (isset($form_state['values']['pid'])) {
    $payment->pid = $form_state['values']['pid'];
  }
  else {
    $payment->pid = NULL;
  }

  $user = user_load_by_name($form_state['values']['uid']);

  $payment->uid = $user->uid;
  $payment->role = $form_state['values']['role'];
  $payment->date = strtotime($form_state['values']['date']);
  $payment->length = $form_state['values']['length'];
  $payment->length_type = $form_state['values']['length_type'];
  $payment->amount = $form_state['values']['amount'];
  $payment->currency = $form_state['values']['currency'];

  $payment->item_id = isset($form_state['values']['item_id']) ? $form_state['values']['item_id'] : '';

  return $payment;
}

/**
 * Helper function to convert the data on admin form into membership presentation.
 */
function _paypal_roles_convert_form_to_membership($form_state) {
  $ml = new stdClass();
  $ml->name = $form_state['values']['name'];
  $ml->description = $form_state['values']['description']['value'];
  $ml->description_format = $form_state['values']['description']['format'];
  $ml->role = $form_state['values']['role'];
  $ml->length = $form_state['values']['length'];
  $ml->length_type = $form_state['values']['length_type'];
  $ml->amount = $form_state['values']['amount'];
  $ml->discount_rate = $form_state['values']['discount_rate'];
  $ml->discount_role = $form_state['values']['discount_role'];
  $ml->currency = $form_state['values']['currency'];
  $ml->cycles = $form_state['values']['cycles'];
  $ml->trial_length = $form_state['values']['trial_length'];
  $ml->trial_length_type = $form_state['values']['trial_length_type'];
  $ml->trial_amount = $form_state['values']['trial_amount'];
  $ml->status = $form_state['values']['status'];
  $ml->language = $form_state['values']['language'];
  $ml->force_onetime = isset($form_state['values']['force_onetime']) ? (int) $form_state['values']['force_onetime'] : 0;
  $ml->button_type = $form_state['values']['button_type'];

  if (isset($form_state['values']['machine_name'])) {
    $ml->machine_name = $form_state['values']['machine_name'];
  }

  return $ml;
}

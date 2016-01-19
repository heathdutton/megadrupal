<?php

/**
 * @file
 * Contains the form functions for MS Core.
 *
 * Original author: Leighton Whiting - Released under GENERAL PUBLIC LICENSE 
 * Current maintenance by multiple MoneySuite users.
 */

/**
 * Form constructor for the order form.
 *
 * @param string $action
 *   The action, whether 'add' or 'edit'.
 * @param MsOrder $order
 *   (Optional) The order to edit.
 *
 * @ingroup forms
 * @see ms_core_order_form_validate()
 * @see ms_core_order_form_submit()
 */
function ms_core_order_form($form, &$form_state, $action = 'add', $order = NULL) {
  drupal_add_library('system', 'ui.datepicker');
  drupal_add_js('jQuery(document).ready(function(){jQuery( ".pickadate" ).datepicker({
      dateFormat: "mm/dd/yy",
      autoSize: true
    });});', 'inline');
  $is_edit = FALSE;
  $account = NULL;
  $form['order'] = array(
    '#type' => 'fieldset',
    '#title' => t('Order', array(), array('context' => 'ms_core')),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  if ($action == 'edit') {
    $is_edit = TRUE;
    $account = user_load($order->uid);

    // Show the products for the order.
    $form['products'] = array(
      '#type' => 'fieldset',
      '#title' => t('Products'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    // Show an add product link.
    $form['products']['add_product_link'] = array(
      '#type' => 'item',
      '#markup' => l(t('Add Product'), 'admin/moneysuite/products/add/' . $order->order_key, array('attributes' => array('class' => array('ms_core_edit_order_add_product_link')))),
    );

    foreach ($order->products as $prod_num => $product) {
      $form['products']['product_' . $prod_num . '_fieldset'] = array(
        '#type' => 'fieldset',
        '#title' => check_plain($product->name),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      // Show the Actions available for this payment.
      $product_actions = array();
      $product_actions[] = l(t('Remove'), 'admin/moneysuite/products/remove/' . $product->order_product_id);

      // Display details about the payment.
      $headers = array(array(
        'data' => t('Product Details'),
        'colspan' => 2,
      ));

      $product->recurring_schedule['qty'] = $product->qty;

      $rows = array();
      $rows[] = array(t('Actions'), implode(' | ', $product_actions));
      $rows[] = array(t('Module'), $product->module);
      $rows[] = array(t('Amount'), ms_core_get_recurring_string($product->recurring_schedule, ($product->type == 'recurring')));
      $rows[] = array(t('Type'), $product->type);

      $form['products']['product_' . $prod_num . '_fieldset']['product_' . $prod_num . '_details'] = array(
        '#type' => 'item',
        '#markup' => theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array())),
      );
    }

    // Show the payments for the order.
    $form['payments'] = array(
      '#type' => 'fieldset',
      '#title' => t('Payments'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    // Show an add payment link.
    $form['payments']['add_payment_link'] = array(
      '#type' => 'item',
      '#markup' => l(t('Add Payment'), 'admin/moneysuite/payments/add/' . $order->order_key, array('attributes' => array('class' => array('ms_core_edit_order_add_payment_link')))),
    );

    foreach ($order->payments as $pid => $payment) {
      $form['payments']['payment_' . $pid . '_fieldset'] = array(
        '#type' => 'fieldset',
        '#title' => check_plain(ms_core_get_payment_type($payment->type) . ' - ' . format_date($payment->created)),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      // Show the Actions available for this payment.
      $payment_actions = array();
      $payment_actions[] = l(t('Edit'), 'admin/moneysuite/payments/edit/' . $payment->pid);
      $payment_actions[] = l(t('Delete'), 'admin/moneysuite/payments/delete/' . $payment->pid);
      $payment_actions[] = l(t('Resubmit'), 'admin/moneysuite/payments/resubmit/' . $payment->pid);

      // Display details about the payment.
      $headers = array(array(
        'data' => t('Payment Details'),
        'colspan' => 2,
      ));
      $rows = array();
      $rows[] = array(t('Actions'), implode(' | ', $payment_actions));
      $rows[] = array(t('Amount'), ms_core_format_money($payment->amount, $payment->currency, 'payment_amount'));
      $rows[] = array(t('Type'), ms_core_get_payment_type($payment->type));
      $rows[] = array(t('Date'), format_date($payment->created));
      $rows[] = array(t('Gateway'), ms_core_get_payment_gateway_name($payment->gateway));
      if ($payment->transaction) {
        $rows[] = array(t('Transaction ID'), $payment->transaction);
      }
      if ($payment->recurring_id) {
        $rows[] = array(t('Recurring ID'), $payment->recurring_id);
      }

      $form['payments']['payment_' . $pid . '_fieldset']['payment_' . $pid . '_details'] = array(
        '#type' => 'item',
        '#markup' => theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array())),
      );
    }

    $form['order']['oid'] = array(
      '#type' => 'value',
      '#value' => $order->oid,
    );
    $form['order']['orderid'] = array(
      '#type' => 'textfield',
      '#title' => t('Order ID'),
      '#size' => 32,
      '#maxlength' => 64,
      '#disabled' => TRUE,
      '#description' => t("The ID of this Order"),
      '#value' => $order->order_number,
    );
  }

  $form['order']['created'] = array(
    '#type' => 'textfield',
    '#title' => t('Date Created'),
    '#default_value' => ($is_edit) ? date('m/d/Y', $order->created) : 0,
    '#description' => t("Enter any date in the MM/DD/YYYY format (or any other format supported by strtotime())."),
    '#attributes' => array('class' => array('pickadate')),
  );
  $form['order']['username'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#size' => 32,
    '#maxlength' => 64,
    '#description' => t("The User for this Order"),
    '#autocomplete_path' => 'admin/structure/ms_core/autocomplete',
    '#required' => TRUE,
    '#default_value' => ($is_edit) ? $account->name : '',
  );
  $form['order']['order_type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => array(
      'recurring' => t('Recurring'),
      'cart' => t('Cart'),
    ),
    '#description' => t("The type of order"),
    '#required' => TRUE,
    '#default_value' => ($is_edit) ? $order->order_type : 'cart',
  );
  $form['order']['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => ms_core_get_order_statuses(),
    '#description' => t("The status of this order"),
    '#required' => TRUE,
    '#default_value' => ($is_edit) ? $order->status : '',
  );
  $form['order']['gateway'] = array(
    '#type' => 'select',
    '#title' => t('Gateway'),
    '#options' => ms_core_get_payment_gateways_list(),
    '#description' => t("The gateway for this order"),
    '#required' => TRUE,
    '#default_value' => ($is_edit) ? $order->gateway : '',
  );
  $form['order']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#size' => 32,
    '#maxlength' => 64,
    '#description' => t("The first name of the Customer"),
    '#default_value' => ($is_edit) ? $order->first_name : '',
  );
  $form['order']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#size' => 32,
    '#maxlength' => 64,
    '#description' => t("The last name of the Customer"),
    '#default_value' => ($is_edit) ? $order->last_name : '',
  );
  $form['order']['email_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Address'),
    '#size' => 64,
    '#maxlength' => 255,
    '#default_value' => ($is_edit) ? $order->email_address : '',
  );
  $form['order']['currency'] = array(
    '#type' => 'select',
    '#title' => t('Currency'),
    '#options' => ms_core_get_currencies(),
    '#description' => t("The currency for this order"),
    '#required' => TRUE,
    '#default_value' => ($is_edit) ? $order->currency : '',
  );

  // Billing Address.
  $form['order']['billing_address'] = array(
    '#type' => 'fieldset',
    '#title' => t('Billing Address'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['order']['billing_address']['billing_street'] = array(
    '#type' => 'textfield',
    '#title' => variable_get('ms_checkout_form_billing_address1_title', t('Street')),
    '#size' => 32,
    '#maxlength' => 64,
    '#default_value' => ($is_edit) ? $order->billing_address['street'] : '',
  );
  $form['order']['billing_address']['billing_street2'] = array(
    '#type' => 'textfield',
    '#title' => variable_get('ms_checkout_form_billing_address2_title', t('Street 2')),
    '#size' => 32,
    '#maxlength' => 64,
    '#default_value' => ($is_edit) ? $order->billing_address['street2'] : '',
  );
  $form['order']['billing_address']['billing_city'] = array(
    '#type' => 'textfield',
    '#title' => variable_get('ms_checkout_form_billing_city_title', t('City')),
    '#size' => 32,
    '#maxlength' => 64,
    '#default_value' => ($is_edit) ? $order->billing_address['city'] : '',
  );
  $form['order']['billing_address']['billing_state'] = array(
    '#type' => 'textfield',
    '#title' => variable_get('ms_checkout_form_billing_state_title', t('State')),
    '#size' => 32,
    '#maxlength' => 15,
    '#default_value' => ($is_edit) ? $order->billing_address['state'] : '',
  );
  $form['order']['billing_address']['billing_zip'] = array(
    '#type' => 'textfield',
    '#title' => variable_get('ms_checkout_form_billing_zip_title', t('Zip Code')),
    '#size' => 32,
    '#maxlength' => 15,
    '#default_value' => ($is_edit) ? $order->billing_address['zip'] : '',
  );
  $form['order']['billing_address']['billing_country'] = array(
    '#type' => 'select',
    '#title' => variable_get('ms_checkout_form_billing_country_title', t('Country')),
    '#options' => ms_core_get_countries('iso'),
    '#default_value' => ($is_edit) ? $order->billing_address['country'] : '',
  );
  $form['order']['billing_address']['billing_phone'] = array(
    '#type' => 'textfield',
    '#title' => variable_get('ms_checkout_form_billing_phone_title', t('Phone Number')),
    '#size' => 32,
    '#maxlength' => 32,
    '#default_value' => ($is_edit) ? $order->billing_address['phone'] : '',
  );

  // Shipping Address.
  $form['order']['shipping_address'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shipping Address'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['order']['shipping_address']['shipping_street'] = array(
    '#type' => 'textfield',
    '#title' => variable_get('ms_checkout_form_shipping_address1_title', t('Street')),
    '#size' => 32,
    '#maxlength' => 64,
    '#default_value' => ($is_edit) ? $order->shipping_address['street'] : '',
  );
  $form['order']['shipping_address']['shipping_street2'] = array(
    '#type' => 'textfield',
    '#title' => variable_get('ms_checkout_form_shipping_address2_title', t('Street 2')),
    '#size' => 32,
    '#maxlength' => 64,
    '#default_value' => ($is_edit) ? $order->shipping_address['street2'] : '',
  );
  $form['order']['shipping_address']['shipping_city'] = array(
    '#type' => 'textfield',
    '#title' => variable_get('ms_checkout_form_shipping_city_title', t('City')),
    '#size' => 32,
    '#maxlength' => 64,
    '#default_value' => ($is_edit) ? $order->shipping_address['city'] : '',
  );
  $form['order']['shipping_address']['shipping_state'] = array(
    '#type' => 'textfield',
    '#title' => variable_get('ms_checkout_form_shipping_state_title', t('State')),
    '#size' => 32,
    '#maxlength' => 15,
    '#default_value' => ($is_edit) ? $order->shipping_address['state'] : '',
  );
  $form['order']['shipping_address']['shipping_zip'] = array(
    '#type' => 'textfield',
    '#title' => variable_get('ms_checkout_form_shipping_zip_title', t('Zip Code')),
    '#size' => 32,
    '#maxlength' => 15,
    '#default_value' => ($is_edit) ? $order->shipping_address['zip'] : '',
  );
  $form['order']['shipping_address']['shipping_country'] = array(
    '#type' => 'select',
    '#title' => variable_get('ms_checkout_form_shipping_country_title', t('Country')),
    '#options' => ms_core_get_countries('iso'),
    '#default_value' => ($is_edit) ? $order->shipping_address['country'] : '',
  );
  $form['order']['shipping_address']['shipping_phone'] = array(
    '#type' => 'textfield',
    '#title' => variable_get('ms_checkout_form_shipping_phone_title', t('Phone Number')),
    '#size' => 32,
    '#maxlength' => 32,
    '#default_value' => ($is_edit) ? $order->shipping_address['phone'] : '',
  );

  // Show the recurring schedule edit form if applicable.
  if ($is_edit && $order->order_type == 'recurring' && ($gateway = ms_core_get_payment_gateway($order->gateway)) && !empty($gateway['recurring_schedule_support']) && ($recurring_schedule = ms_core_load_recurring_schedule($order->oid))) {
    // TODO - Select all recurring schedules in the case of multi_recurring and show a form for each.
    $form['order']['recurring_schedule'] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => t('Recurring Schedule'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t("Here you can view && edit the recurring schedule for
        this order. Changing these values will change when the user is billed
        next, && for how much && how long. Be very careful about changing these
        values!")
    );
    $form['order']['recurring_schedule']['change_recurring_schedule'] = array(
      '#type' => 'checkbox',
      '#title' => t("Change the Recurring Schedule?"),
      '#description' => t("Click this in order to change the recurring schedule
        values below. Note that there is generally no reason to change the values
        in the recurring schedule."),
    );
    $form['order']['recurring_schedule']['status'] = array(
      '#type' => 'select',
      '#title' => t('Status'),
      '#options' => array(
        'active' => t('Active'),
        'cancelled' => t('Cancelled'),
        'expiring' => t('Expiring'),
        'pending_downgrade' => t('Pending Downgrade'),
      ),
      '#default_value' => $recurring_schedule->status,
    );
    $form['order']['recurring_schedule']['main_amount'] = array(
      '#type' => 'textfield',
      '#title' => t('Regular price'),
      '#default_value' => $recurring_schedule->main_amount,
    );
    $form['order']['recurring_schedule']['main_length'] = array(
      '#type' => 'textfield',
      '#title' => t('Regular period length'),
      '#default_value' => $recurring_schedule->main_length,
    );
    if (strlen($recurring_schedule->main_unit == 1)) {
      $form['order']['recurring_schedule']['main_unit'] = array(
        '#type' => 'select',
        '#title' => t('Regular period unit'),
        '#options' => array(
          'H' => t('Hours'),
          'D' => t('Days'),
          'W' => t('Weeks'),
          'M' => t('Months'),
          'Y' => t('Years'),
        ),
        '#default_value' => $recurring_schedule->main_unit,
      );
    } else {
      $form['order']['recurring_schedule']['main_unit'] = array(
        '#type' => 'textfield',
        '#title' => t('Regular period unit'),
        '#default_value' => $recurring_schedule->main_unit,
      );
    }
    $form['order']['recurring_schedule']['trial_amount'] = array(
      '#type' => 'textfield',
      '#title' => t('Trial price'),
      '#default_value' => $recurring_schedule->trial_amount,
    );
    $form['order']['recurring_schedule']['trial_length'] = array(
      '#type' => 'textfield',
      '#title' => t('Trial period length'),
      '#default_value' => $recurring_schedule->trial_length,
    );
    $form['order']['recurring_schedule']['trial_unit'] = array(
      '#type' => 'select',
      '#title' => t('Trial period unit'),
      '#options' => array(
        'H' => t('Hours'),
        'D' => t('Days'),
        'W' => t('Weeks'),
        'M' => t('Months'),
        'Y' => t('Years'),
      ),
      '#default_value' => $recurring_schedule->trial_unit,
    );
    $form['order']['recurring_schedule']['total_occurrences'] = array(
      '#type' => 'textfield',
      '#title' => t('Total Occurrences'),
      '#default_value' => $recurring_schedule->total_occurrences,
    );
    $form['order']['recurring_schedule']['next_payment'] = array(
      '#type' => 'textfield',
      '#title' => t('Next Payment'),
      '#default_value' => date('m/d/Y', $recurring_schedule->next_payment),
      '#description' => t("Enter any date in the MM/DD/YYYY format (or any other format supported by strtotime())."),
      '#attributes' => array('class' => array('pickadate')),
    );
    $form['order']['recurring_schedule']['current_payments'] = array(
      '#type' => 'textfield',
      '#title' => t('Current Payments'),
      '#default_value' => $recurring_schedule->current_payments,
    );
    $form['order']['recurring_schedule']['created'] = array(
      '#type' => 'textfield',
      '#title' => t('Created'),
      '#default_value' => date('m/d/Y', $recurring_schedule->created),
      '#description' => t("Enter any date in the MM/DD/YYYY format (or any other format supported by strtotime())."),
      '#attributes' => array('class' => array('pickadate')),
    );
    $form['order']['recurring_schedule']['expiration'] = array(
      '#type' => 'textfield',
      '#title' => t('Expiration'),
      '#default_value' => $recurring_schedule->expiration ? date('m/d/Y', $recurring_schedule->expiration) : 0,
      '#description' => t("Enter any date in the MM/DD/YYYY format (or any other
        format supported by strtotime()). Enter 0 for no expiration."),
      '#attributes' => array('class' => array('pickadate')),
    );
    $form['order']['recurring_schedule']['failed_payments'] = array(
      '#type' => 'textfield',
      '#title' => t('Failed Payments'),
      '#default_value' => $recurring_schedule->failed_payments,
    );
    $form['order']['recurring_schedule']['notified'] = array(
      '#type' => 'checkbox',
      '#title' => t('Notified'),
      '#default_value' => $recurring_schedule->notified,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form_state['redirect'] = 'admin/moneysuite/orders';
  return $form;
}

/**
 * Form validation handler for ms_core_order_form().
 *
 * @see ms_core_order_form_submit()
 */
function ms_core_order_form_validate($form, &$form_state) {
  $v = $form_state['values'];
  $account = user_load_by_name($v['username']);
  if (!$account->uid) {
    form_set_error('username', t('You must enter a valid user'));
  }
}

/**
 * Form submission handler for ms_core_order_form().
 *
 * @see ms_core_order_form_validate()
 */
function ms_core_order_form_submit($form, &$form_state) {
  // Handle the Updating or Inserting of an order.
  $v = $form_state['values'];

  if (isset($v['oid']) && ($order = ms_core_order_load($v['oid']))) {
    $account = user_load_by_name($v['username']);

    // Update the values.
    $order->created = strtotime($v['created']);
    $order->gateway = $v['gateway'];
    $order->order_type = $v['order_type'];
    $order->status = $v['status'];
    $order->currency = $v['currency'];
    $order->first_name = $v['first_name'];
    $order->last_name = $v['last_name'];
    $order->email_address = $v['email_address'];
    $order->uid = ($account) ? $account->uid : 0;
    $order->billing_address = array(
      'street' => trim($v['billing_street']),
      'street2' => trim($v['billing_street2']),
      'city' => trim($v['billing_city']),
      'state' => trim($v['billing_state']),
      'zip' => trim($v['billing_zip']),
      'country' => trim($v['billing_country']),
      'phone' => trim($v['billing_phone']),
    );
    $order->shipping_address = array(
      'street' => trim($v['shipping_street']),
      'street2' => trim($v['shipping_street2']),
      'city' => trim($v['shipping_city']),
      'state' => trim($v['shipping_state']),
      'zip' => trim($v['shipping_zip']),
      'country' => trim($v['shipping_country']),
      'phone' => trim($v['shipping_phone']),
    );

    // Save the recurring schedule.
    if (!empty($v['recurring_schedule']['change_recurring_schedule']) && ($recurring_schedule = ms_core_load_recurring_schedule($order->oid))) {
      // Merge in the values.
      $recurring_schedule = (object)array_merge((array)$recurring_schedule, $v['recurring_schedule']);
      $recurring_schedule->next_payment = strtotime($recurring_schedule->next_payment);
      $recurring_schedule->created = strtotime($recurring_schedule->created);
      $recurring_schedule->expiration = strtotime($recurring_schedule->expiration);
      ms_core_update_recurring_schedule($recurring_schedule);
    }

    // Save the order.
    if (ms_core_order_save($order)) {
      drupal_set_message(t('Order @num saved.', array('@num' => $order->order_number)));
    }
  } else {
    // Create a new order.
    $account = user_load_by_name($v['username']);
    $order = ms_core_order_new($v['order_type'], ($account) ? $account->uid : 0);

    // Set the values.
    $order->created = strtotime($v['created']);
    $order->gateway = $v['gateway'];
    $order->order_type = $v['order_type'];
    $order->status = $v['status'];
    $order->currency = $v['currency'];
    $order->first_name = $v['first_name'];
    $order->last_name = $v['last_name'];
    $order->email_address = $v['email_address'];
    $order->billing_address = array(
      'street' => trim($v['billing_street']),
      'street2' => trim($v['billing_street2']),
      'city' => trim($v['billing_city']),
      'state' => trim($v['billing_state']),
      'zip' => trim($v['billing_zip']),
      'country' => trim($v['billing_country']),
      'phone' => trim($v['billing_phone']),
    );
    $order->shipping_address = array(
      'street' => trim($v['shipping_street']),
      'street2' => trim($v['shipping_street2']),
      'city' => trim($v['shipping_city']),
      'state' => trim($v['shipping_state']),
      'zip' => trim($v['shipping_zip']),
      'country' => trim($v['shipping_country']),
      'phone' => trim($v['shipping_phone']),
    );

    // Save the order.
    ms_core_order_save($order);

    drupal_set_message(t('Order @num saved.', array('@num' => $order->order_number)));
  }

  $form_state['redirect'] = 'admin/moneysuite/orders';
}

/**
 * Form constructor for the Delete Checkout Order Confirmation form.
 *
 * @param MsOrder $order
 *   The order to delete.
 *
 * @ingroup forms
 * @see ms_core_delete_checkout_order_confirm_submit()
 */
function ms_core_delete_checkout_order_confirm($form, &$form_state, $order) {
  $form['#ms_core_oid'] = $order->oid;
  return confirm_form($form, t('Are you sure you want to delete this order?'), 'user', t('This action cannot be undone.'), t('Delete'));
}

/**
 * Form submission handler for ms_core_delete_checkout_order_confirm().
 */
function ms_core_delete_checkout_order_confirm_submit($form, &$form_state) {
  $order = ms_core_order_load($form['#ms_core_oid']);

  ms_core_order_delete($order->oid);

  drupal_set_message(t('Order %number has been deleted.', array('%number' => $order->order_number)));
  $form_state['redirect'] = 'user';
}

/**
 * Form constructor for the autobill optional checkbox.
 *
 * @param MsOrder $order
 *   The cart object.
 *
 * @ingroup forms
 */
function ms_core_autobill_option_form($form, &$form_state, $order, $is_cart = TRUE) {
  if (!empty($form_state['triggering_element']['#name']) && $form_state['triggering_element']['#name'] == 'autobill') {
    $new_type = $form_state['input']['autobill'] ? 'recurring' : 'cart';
    ms_core_change_order_type($order, $new_type, $is_cart);
  }
  $form['order'] = array(
    '#type' => 'value',
    '#value' => $order,
  );
  $form['autobill'] = array(
    '#type' => 'checkbox',
    '#title' => t("Autobill"),
    '#description' => t("If autobill is selected, payment will be automatically processed each term."),
    '#default_value' => $order->order_type == 'recurring',
    '#ajax' => array(
      'wrapper' => 'ms-core-checkout-page-wrapper',
      'callback' => 'ms_core_checkout_page_ajax_callback',
      'effect' => 'fade',
    ),
  );

  if (!$is_cart) {
    // Use a different callback for order.
    $form['autobill']['#ajax'] = array(
      'wrapper' => 'ms-core-checkout-payment-page-wrapper',
      'callback' => 'ms_core_checkout_payment_page_ajax_callback',
      'effect' => 'fade',
    );
  }

  return $form;
}

/**
 * AJAX Callback: Rebuilds the checkout payment page
 */
function ms_core_checkout_payment_page_ajax_callback($form, $form_state) {
  module_load_include('inc', 'ms_core', 'ms_core.pages');
  return ms_core_checkout_payment_page($form_state['values']['order']->gateway);
}

/**
 * AJAX Callback: Rebuilds the checkout page
 */
function ms_core_checkout_page_ajax_callback($form, $form_state) {
  module_load_include('inc', 'ms_core', 'ms_core.pages');
  return ms_core_checkout_page();
}

/**
 * Form constructor for the checkout form.
 *
 * @param MsOrder $cart
 *   The order or cart object.
 *
 * @ingroup forms
 * @see ms_core_checkout_form_validate()
 * @see ms_core_checkout_form_submit()
 */
function ms_core_checkout_form($form, &$form_state, $cart) {
  // Stop the caching.
  $GLOBALS['conf']['cache'] = FALSE;
  global $user;
  $form['#action'] = url('ms/checkout');

  // Add the User Registration form.
  if (!$user->uid && empty($cart->data['skip_registration'])) {
    $required = variable_get('ms_core_checkout_require_account_info', 'optional') == 'required';
    $form['outer_wrapper']['information'] = array(
      '#type' => 'fieldset',
      '#title' => t('Customer Information'),
      '#description' => t('Enter a valid email address for this order or !click to login with an existing account && return to checkout.',
        array('!click' => l(t('click here'), 'user/login', array('query' => drupal_get_destination())))),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $form['outer_wrapper']['information']['email1'] = array(
      '#type' => 'textfield',
      '#title' => t('E-mail Address'),
      '#size' => 32,
      '#description' => t('A valid e-mail address. All e-mails from the system will be sent to this address. The e-mail address is not made public && will only be used if you wish to receive a new password or wish to receive certain news or notifications by e-mail.'),
      '#required' => TRUE,
    );
    if (variable_get('ms_core_checkout_email_verify', TRUE)) {
      $form['outer_wrapper']['information']['email2'] = array(
        '#type' => 'textfield',
        '#title' => t('Confirm E-mail Address'),
        '#size' => 32,
        '#description' => t('Please re-enter your e-mail address.'),
        '#required' => TRUE,
      );
    }
    if (variable_get('ms_core_checkout_require_account_info', 'optional') != 'hidden') {
      $form['outer_wrapper']['information']['info'] = array(
        '#type' => 'fieldset',
        '#title' => t('New Account Details'),
        '#description' => $required ? '' : t('<em>Optional</em>. New customers may supply custom account details.
    We will create these for you if no values are entered.'),
        '#weight' => 99,
        '#collapsible' => !$required,
        '#collapsed' => !$required,
      );
      $form['outer_wrapper']['information']['info']['username'] = array(
        '#type' => 'textfield',
        '#title' => t('Username'),
        '#size' => 32,
        '#required' => $required,
      );
      $form['outer_wrapper']['information']['info']['password1'] = array(
        '#type' => 'password',
        '#title' => t('Password'),
        '#size' => 32,
        '#required' => $required,
      );
      $form['outer_wrapper']['information']['info']['password2'] = array(
        '#type' => 'password',
        '#title' => t('Confirm Password'),
        '#size' => 32,
        '#description' => t('Passwords must match to proceed'),
        '#required' => $required,
      );
    }
    // Add Profile 2 Fields if set.
    if (variable_get('ms_core_profile2_checkout_integration', FALSE) && module_exists('profile2')) {
      foreach (profile2_get_types() as $type_name => $profile_type) {
        if (!empty($profile_type->data['registration'])) {
          if (empty($form_state['profiles'][$type_name])) {
            $form_state['profiles'][$type_name] = profile2_create(array('type' => $type_name));
          }
          profile2_attach_form($form, $form_state);
          // Wrap each profile form in a fieldset.
          $form['profile_' . $type_name] += array(
            '#type' => 'fieldset',
            '#title' => check_plain($profile_type->getTranslation('label')),
          );
        }
      }
    }
    if (variable_get('ms_core_user_fields_checkout_integration', FALSE)) {
      // @todo - Figure out why this breaks if fields which aren't included on the registration form don't have default values set...
      $form['#user'] = drupal_anonymous_user();
      $form['#user_category'] = 'register';

      // Attach field widgets, and hide the ones where the 'user_register_form'
      // setting is not on.
      $langcode = entity_language('user', $form['#user']);
      field_attach_form('user', $form['#user'], $form, $form_state, $langcode);
      foreach (field_info_instances('user', 'user') as $field_name => $instance) {
        if (empty($instance['settings']['user_register_form'])) {
          $form[$field_name]['#access'] = FALSE;
        }
      }
    }
  }

  // If the order is shippable, collect the shipping address.
  if (!empty($cart->data['shippable'])) {
    $form['outer_wrapper']['shipping'] = array(
      '#type' => 'fieldset',
      '#title' => t('Shipping Address'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    $fields = ms_core_get_checkout_form_fields('shipping');

    $vars = FALSE;

    if (!empty($cart->oid)) {
      $varst = $cart->shipping_address;
      $vars = array(
        'shipping_first_name' => trim($varst['first_name']),
        'shipping_last_name' => trim($varst['last_name']),
        'shipping_address1' => trim($varst['street']),
        'shipping_address2' => trim($varst['street2']),
        'shipping_city' => trim($varst['city']),
        'shipping_state' => trim($varst['state']),
        'shipping_zip' => trim($varst['zip']),
        'shipping_country' => trim($varst['country']),
        'shipping_phone' => trim($varst['phone']),
      );
    } elseif ($cart->uid && ($account = user_load($cart->uid))) {
      // Load the account from the order.
      $vars = (array)$account;
    } elseif (!empty($cart->data['register_form'])) {
      // Get the address && email from the registration form.
      $vars = $cart->data['register_form'];
    } elseif (!empty($cart->data['form']['mail'])) {
      $vars = $cart->data['form'];
    } elseif (!empty($cart->data['form']['values']['mail'])) {
      $vars = $cart->data['form']['values'];
    }

    // Shipping Info.
    foreach ($fields as $field => $data) {
      if (variable_get('ms_checkout_form_' . $field . '_enabled', TRUE)) {
        $form['outer_wrapper']['shipping'][$field] = array(
          '#type' => $data['type'],
          '#title' => check_plain(variable_get('ms_checkout_form_' . $field . '_title', $data['name'])),
          '#description' => check_plain(variable_get('ms_checkout_form_' . $field . '_description', $data['description'])),
          '#size' => $data['size'],
          '#required' => variable_get('ms_checkout_form_' . $field . '_required', $data['required']),
          '#default_value' => isset($vars[$field]) ? $vars[$field] : ms_core_get_form_field_default(variable_get('ms_core_profile_' . $field, $data['profile']), $vars),
        );

        switch ($field) {
          case 'shipping_country':
            $form['outer_wrapper']['shipping'][$field]['#options'] = ms_core_get_countries();

            $form['outer_wrapper']['shipping'][$field]['#ajax'] = array(
              'wrapper' => 'ms-core-checkout-page-wrapper',
              'callback' => 'ms_core_checkout_page_ajax_callback',
              'effect' => 'fade',
            );
            if (empty($form['shipping'][$field]['#default_value'])) {
              $form['outer_wrapper']['shipping'][$field]['#default_value'] = variable_get('ms_core_default_country', 'US');
            }
            break;

          case 'shipping_state':
            if (empty($form['outer_wrapper']['shipping'][$field]['#default_value'])) {
              $form['outer_wrapper']['shipping'][$field]['#default_value'] = variable_get('ms_core_default_state', '');
            }
            $form['outer_wrapper']['shipping'][$field]['#prefix'] = "<div id='ms_core_checkout_shipping_state_wrapper'>";
            $form['outer_wrapper']['shipping'][$field]['#suffix'] = "</div>";
            $selected_country = ms_core_get_form_field_default(variable_get('ms_core_profile_' . $field, $data['profile']), $vars) ? ms_core_get_form_field_default(variable_get('ms_core_profile_' . $field, $data['profile']), $vars) : variable_get('ms_core_default_country', 'US');
            if (!empty($form_state['input']['shipping_country'])) {
              $selected_country = $form_state['input']['shipping_country'];
            }
            $numcode = ms_core_get_country_type($selected_country, 'iso', 'numcode');
            $options = ms_core_get_regions($numcode);
            if (!empty($options)) {
              $form['outer_wrapper']['shipping']['shipping_state']['#options'] = $options;
              $hook_check = module_implements('ms_core_checkout_region_changed');
              if (!empty($hook_check)) {
                $form['outer_wrapper']['shipping'][$field]['#ajax'] = array(
                  'wrapper' => 'ms-core-checkout-page-wrapper',
                  'callback' => 'ms_core_checkout_page_ajax_callback',
                  'effect' => 'fade',
                );
              }
            } else {
              // Hide the state field.
              $form['outer_wrapper']['shipping'][$field] = array(
                '#type' => 'value',
                '#value' => '',
                '#prefix' => "<div id='ms_core_checkout_shipping_state_wrapper'>",
                '#suffix' => "</div>",
              );
            }
            break;
        }
      } else {
        $form['outer_wrapper']['shipping'][$field] = array(
          '#type' => 'value',
          '#value' => ms_core_get_form_field_default(variable_get('ms_core_profile_' . $field, $data['profile']), $vars),
        );
      }
    }

    // Call the hooks if applicable.
    if (!empty($form_state['input']['shipping_country']) && !empty($form_state['input']['_triggering_element_name']) && $form_state['input']['_triggering_element_name'] == 'shipping_country') {
      module_invoke_all('ms_core_checkout_country_changed', $cart, $form_state['input']['shipping_country']);
    } elseif (!empty($form['outer_wrapper']['shipping']['shipping_country']['#default_value'])) {
      module_invoke_all('ms_core_checkout_country_changed', $cart, $form['outer_wrapper']['shipping']['shipping_country']['#default_value']);
    }
    if (!empty($form_state['input']['shipping_state'])) {
      if (!empty($form_state['input']['_triggering_element_name']) && $form_state['input']['_triggering_element_name'] == 'shipping_state') {
        module_invoke_all('ms_core_checkout_region_changed', $cart, $form_state['input']['shipping_state']);
      } else {
        module_invoke_all('ms_core_checkout_region_changed', $cart, key($form['outer_wrapper']['shipping']['shipping_state']['#options']));
      }
    } else {
      $state = !empty($form['outer_wrapper']['shipping']['shipping_state']['#default_value']) ? $form['outer_wrapper']['shipping']['shipping_state']['#default_value'] : key($form['outer_wrapper']['shipping']['shipping_state']['#options']);
      if ($state) {
        module_invoke_all('ms_core_checkout_region_changed', $cart, $state);
      }
    }
  }

  // Let other modules modify the form if needed, to add AJAX callbacks.
  drupal_alter('ms_core_checkout_form', $form, $form_state, $cart);

  if (!(variable_get('ms_core_terms_service_hide_for_authenticated', FALSE) && $user->uid)
    && variable_get('ms_core_terms_service_nid', '') && $node = node_load(variable_get('ms_core_terms_service_nid', ''))
  ) {
    $form['outer_wrapper']['terms_of_service'] = array(
      '#type' => 'fieldset',
    );
    $node_view = node_view($node);
    $form['outer_wrapper']['terms_of_service']['terms'] = array(
      '#type' => 'fieldset',
      '#collapsible' => variable_get('ms_core_terms_service_collapsed', FALSE),
      '#collapsed' => variable_get('ms_core_terms_service_collapsed', FALSE),
      '#title' => check_plain(variable_get('ms_core_terms_service_collapsed', FALSE) ? $node->title : ''),
    );
    $form['outer_wrapper']['terms_of_service']['terms']['terms_of_service_text'] = array(
      '#type' => 'item',
      '#markup' => drupal_render($node_view),
    );
    $form['outer_wrapper']['terms_of_service']['terms_of_service_agree'] = array(
      '#type' => 'checkbox',
      '#title' => variable_get('ms_core_terms_service_text', ''),
      '#return_value' => 1,
      '#required' => TRUE,
    );
  }

  $form['actions'] = array('#type' => 'actions');

  if ($cart->amount == 0 && !$cart->recurring_schedule['has_trial']) {
    // Put the Free Button here.
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Complete Order'),
      '#weight' => 999,
      '#prefix' => "<div class='ms_cart_right'>",
      '#suffix' => "</div>",
    );
  } else {
    // Get the Payment Gateways.
    $gateways = ms_core_get_checkout_payment_gateways($cart->order_type, $cart->data['blocked_gateways']);
    // Allow other modules to alter the gateways.
    drupal_alter('ms_payment_gateway', $gateways, $cart);
    if ($gateways) {
      // Show a form to let the user select which gateway.
      $gateway_options = array();
      foreach ($gateways as $gateway) {
        $gateway_options[$gateway['module']] = $gateway['description'];
      }
      if (!empty($gateway_options)) {
        if (count($gateway_options) > 1) {
          $form['gateways'] = array(
            '#type' => 'fieldset',
            '#title' => t('Choose Payment Method'),
            '#collapsible' => TRUE,
            '#collapsed' => FALSE,
          );
          $form['gateways']['gateway'] = array(
            '#type' => 'radios',
            '#title' => t('Payment Method'),
            '#options' => $gateway_options,
            '#description' => t("Choose your Payment Method."),
            '#default_value' => variable_get('ms_core_default_gateway', key($gateway_options)),
            '#required' => TRUE,
          );
        } else {
          $form['gateway'] = array(
            '#type' => 'value',
            '#value' => key($gateway_options),
            '#required' => TRUE,
          );
        }
      } else {
        $form['gateway'] = array(
          '#type' => 'item',
          '#markup' => t('No Payment Gateways are enabled which are compatible with this order. Please check your settings.'),
        );
      }

      $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Proceed to Payment'),
        '#weight' => 999,
        '#prefix' => "<div class='ms_cart_right'>",
        '#suffix' => "</div>",
      );
    } else {
      // There are no gateways installed.
      $form['gateways'] = array(
        '#type' => 'fieldset',
        '#title' => t('Choose Payment Method'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      $form['gateways']['gateways_html'] = array(
        '#type' => 'item',
        '#markup' => t('There are no payment gateways enabled which are compatible with this order.'),
      );
    }
  }

  if (isset($cart->oid)) {
    $form['oid'] = array(
      '#type' => 'value',
      '#value' => $cart->oid,
    );
  }

  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Back to Shopping'),
    '#submit' => array('ms_core_checkout_cancel_button'),
    '#limit_validation_errors' => array(),
    '#prefix' => "<div class='ms_cart_left'>",
    '#suffix' => "</div>",
  );

  return $form;
}

/**
 * Form submission handler for the cancel button of ms_core_checkout_form().
 */
function ms_core_checkout_cancel_button($form, &$form_state) {
  if (variable_get('ms_core_continue_shopping_path', '')) {
    drupal_goto(variable_get('ms_core_continue_shopping_path', ''));
  }
  drupal_goto();
}

/**
 * Form validation handler for ms_core_checkout_form().
 *
 * @see ms_core_checkout_form_submit()
 */
function ms_core_checkout_form_validate($form, &$form_state) {
  $v = $form_state['values'];

  if (isset($v['email1']) && $v['email1']) {
    // Verify Email Address is real.
    if (!valid_email_address($v['email1'])) {
      form_set_error('email1', t('Please enter a valid email address'));
    }
    if (variable_get('ms_core_checkout_email_verify', FALSE)) {
      // Verify Email Addresses Match.
      if ($v['email1'] != $v['email2']) {
        form_set_error('email2', t('The email addresses do not match'));
      }
    }
    // Verify Email Address is not being used.
    if ($email_account = user_load_by_mail($v['email1']) && !variable_get('ms_core_allow_anonymous_email_checkout', FALSE)) {
      form_set_error('email1', t('The email address you entered is already being used by an account on this site.
      Please !login to the account associated with this email address before proceeding.',
        array('!login' => l(t('log in'), 'user/login', array('query' => drupal_get_destination())))
      ));
    }
    if (variable_get('ms_core_checkout_require_account_info', 'optional') != 'hidden') {
      // Verify Username is not being used.
      if ($v['username'] && ($name_account = user_load_by_name($v['username']))) {
        form_set_error('username', t('The username you specified is already in use. Please choose a different username before proceeding.'));
      }
      // Verify Passwords Match.
      if ($v['password1'] != $v['password2']) {
        form_set_error('password2', t('The passwords do not match'));
      }
    }
  }
}

/**
 * Form submission handler for the ms_core_checkout_form().
 *
 * @see ms_core_checkout_form_validate()
 */
function ms_core_checkout_form_submit($form, &$form_state) {
  $v = $form_state['values'];
  global $user;

  $data = array(
    'register_form' => $v,
  );

  if (module_exists('rules')) {
    rules_invoke_event('ms_core_event_gateway_checkout');
  }

  if (isset($v['oid']) && ($order = ms_core_order_load($v['oid']))) {
    ms_core_set_order_session($order);
  } else {
    // Save the order.
    $order = ms_core_cart_to_order($data);
  }

  // Save the shipping address.
  if (isset($v['shipping_country'])) {
    $order->shipping_address = array(
      'first_name' => !empty($v['shipping_first_name']) ? trim($v['shipping_first_name']) : '',
      'last_name' => !empty($v['shipping_last_name']) ? trim($v['shipping_last_name']) : '',
      'street' => !empty($v['shipping_address1']) ? trim($v['shipping_address1']) : '',
      'street2' => !empty($v['shipping_address2']) ? trim($v['shipping_address2']) : '',
      'city' => !empty($v['shipping_city']) ? trim($v['shipping_city']) : '',
      'state' => !empty($v['shipping_state']) ? trim($v['shipping_state']) : '',
      'zip' => !empty($v['shipping_zip']) ? trim($v['shipping_zip']) : '',
      'country' => !empty($v['shipping_country']) ? trim($v['shipping_country']) : '',
      'phone' => !empty($v['shipping_phone']) ? trim($v['shipping_phone']) : '',
    );
    $order->save();
  }

  // Set the order user id if needed
  if (variable_get('ms_core_allow_anonymous_email_checkout', FALSE) && !empty($v['email1']) && ($email_account = user_load_by_mail($v['email1']))) {
    $order->uid = $email_account->uid;
    $order->data['disable_auto_login'] = TRUE;
    $order->save();
  }

  // If we used profile 2 fields on the checkout form, then the user needs to
  // be created first thing.
  if (!$order->uid && !$user->uid && empty($order->data['skip_registration']) && ((variable_get('ms_core_profile2_checkout_integration', FALSE) && module_exists('profile2')) || variable_get('ms_core_user_fields_checkout_integration', FALSE))) {
    // Register the User once they have paid.
    $pass = !empty($v['password1']) ? $v['password1'] : user_password();
    $name = !empty($v['username']) ? $v['username'] : ms_core_create_username($v['email1']);
    $roles = array();

    global $language;

    $account = user_save('', array_merge($v, array(
      'name' => $name,
      'mail' => $v['email1'],
      'init' => $v['email1'],
      'pass' => $pass,
      'roles' => $roles,
      'language' => $language->language,
      'status' => 1,
    )));

    $account->password = $pass;
    //Send the Default Email.
    if (variable_get('ms_core_send_registration_mail', TRUE)) {
      _user_mail_notify('register_no_approval_required', $account);
    }

    $form_state['user'] = $account;

    $order->uid = $account->uid;
    $order->save();
  }

  ms_core_set_order_session($order);

  if ($order->amount == 0 && !$order->recurring_schedule['has_trial']) {
    // Change recurring orders to non-recurring.
    $order->order_type = 'cart';

    ms_core_order_save($order);

    $payment = ms_core_new_payment($order->oid, 'ms_core', 'cart');
    // Generate a Unique Transaction ID.
    $payment->transaction = ms_core_new_transaction($order);
    $payment->currency = $order->currency;

    // Enter a payment for 0.
    ms_core_enter_payment($payment);

    // Redirect to the Gateway Payment Page.
    $form_state['redirect'] = 'ms/thankyou';
  } else {
    $gateway = ms_core_get_payment_gateway($v['gateway']);
    $_SESSION['ms_gateway'] = $v['gateway'];

    // Convert currency here if needed.
    if (!empty($gateway['currency']) && !in_array($order->currency, $gateway['currency'])) {
      $default_currency = ms_core_variable_get('ms_core_default_currency', 'USD', $order);
      if (!in_array($default_currency, $gateway['currency'])) {
        $default_currency = $gateway['currency'][0];
      }

      if (ms_core_convert_currency($order->currency, $default_currency, $order->amount)) {
        foreach ($order->products as $product) {
          $product->amount = ms_core_convert_currency($order->currency, $default_currency, $product->amount);
          $product->recurring_schedule['main_amount'] = ms_core_convert_currency($order->currency, $default_currency, $product->recurring_schedule['main_amount']);
          $product->recurring_schedule['trial_amount'] = ms_core_convert_currency($order->currency, $default_currency, $product->recurring_schedule['trial_amount']);
          $product->save();
        }

        $order->amount = ms_core_convert_currency($order->currency, $default_currency, $order->amount);
        $order->recurring_schedule['main_amount'] = ms_core_convert_currency($order->currency, $default_currency, $order->recurring_schedule['main_amount']);
        $order->recurring_schedule['trial_amount'] = ms_core_convert_currency($order->currency, $default_currency, $order->recurring_schedule['trial_amount']);
        $order->currency = $default_currency;
        $order->save();
      }
    }

    // Redirect to the Gateway Payment Page.
    if (!empty($gateway['checkout_path'])) {
      $form_state['redirect'] = $gateway['checkout_path'];
    } else {
      $form_state['redirect'] = 'ms/checkout/payment/' . $gateway['module'];
    }
  }
}

/**
 * AJAX Callback: Returns the checkout form after ajax element triggered.
 */
function ms_core_checkout_ajax_callback($form, $form_state) {
  return ms_core_checkout_page();
}

/**
 * Form constructor for the Delete Order form.
 *
 * @param MsOrder $order
 *   The order to delete.
 *
 * @ingroup forms
 * @see ms_core_delete_order_confirm_submit()
 */
function ms_core_delete_order_confirm($form, &$form_state, $order) {
  $form['#ms_core_oid'] = $order->oid;
  return confirm_form($form, t('Are you sure you want to delete this order?'), 'admin/moneysuite/orders', t('This action cannot be undone. Deleting this order will remove all records of payments received for this order as well.'), t('Delete'));
}

/**
 * Form submission handler for ms_core_delete_order_confirm().
 */
function ms_core_delete_order_confirm_submit($form, &$form_state) {
  $order = ms_core_order_load($form['#ms_core_oid']);

  ms_core_order_delete($order->oid);

  drupal_set_message(t('Order %number has been deleted.', array('%number' => $order->order_number)));
  $form_state['redirect'] = 'admin/moneysuite/orders';
}

/**
 * Form constructor for the Delete Payment form.
 *
 * @param MsPayment $payment
 *   The payment to delete.
 *
 * @ingroup forms
 * @see ms_core_delete_payment_confirm_submit()
 */
function ms_core_delete_payment_confirm($form, &$form_state, $payment) {
  $form['#ms_core_pid'] = $payment->pid;
  return confirm_form($form, t('Are you sure you want to delete this payment?'), 'admin/moneysuite/orders', t('This action cannot be undone. Deleting this payment will remove all records of it from the database.'), t('Delete'));
}

/**
 * Form submission handler for ms_core_delete_payment_confirm().
 */
function ms_core_delete_payment_confirm_submit($form, &$form_state) {
  $payment = ms_core_load_payment($form['#ms_core_pid']);

  db_delete('ms_payments')
    ->condition('pid', $payment->pid)
    ->execute();

  drupal_set_message(t('Payment %number has been deleted.', array('%number' => $payment->pid)));
  $form_state['redirect'] = 'admin/moneysuite/orders';
}

/**
 * Form constructor for the Remove Product form.
 *
 * @param int $order_product_id
 *   The order product id.
 *
 * @ingroup forms
 * @see ms_core_remove_product_confirm_submit()
 */
function ms_core_remove_product_confirm($form, &$form_state, $order_product_id) {
  $product = ms_core_load_order_product($order_product_id);
  $form['#ms_core_order_product_id'] = $product->order_product_id;
  return confirm_form($form, t('Are you sure you want to remove this product?'), 'admin/moneysuite/orders', t('This action cannot be undone.'), t('Remove'));
}

/**
 * Form submission handler for ms_core_remove_product_confirm().
 */
function ms_core_remove_product_confirm_submit($form, &$form_state) {
  $product = ms_core_load_order_product($form['#ms_core_order_product_id']);

  db_delete('ms_order_products')
    ->condition('order_product_id', $product->order_product_id)
    ->execute();

  drupal_set_message(t('Product %name has been removed.', array('%name' => $product->name)));
  $form_state['redirect'] = 'admin/moneysuite/orders';
}

/**
 * Form constructor for the Resubmit Payment form.
 *
 * @param MsPayment $payment
 *   The payment to resubmit.
 *
 * @ingroup forms
 * @see ms_core_resubmit_payment_confirm_submit()
 */
function ms_core_resubmit_payment_confirm($form, &$form_state, $payment) {
  $form['#ms_core_pid'] = $payment->pid;
  return confirm_form($form, t('Are you sure you want to resubmit this payment?'), 'admin/moneysuite/orders', t('This action cannot be undone. Resubmitting a payment will cause other modules to act on the payment as if it were a real payment.'), t('Resubmit'));
}

/**
 * Form submission handler for ms_core_resubmit_payment_confirm().
 */
function ms_core_resubmit_payment_confirm_submit($form, &$form_state) {
  $payment = ms_core_load_payment($form['#ms_core_pid']);

  // Process the payment, without inserting a new record.
  ms_core_enter_payment($payment, TRUE, FALSE);

  drupal_set_message(t('Payment %number has been resubmitted.', array('%number' => $payment->pid)));
  $form_state['redirect'] = 'admin/moneysuite/orders';
}

/**
 * Form constructor for the Resubmit Payment form.
 *
 * @param MsPayment $payment
 *   The payment to resubmit.
 *
 * @ingroup forms
 * @see ms_core_resubmit_payment_confirm_submit()
 */
function ms_core_refund_payment_confirm($form, &$form_state, $payment) {
  $form['#ms_core_pid'] = $payment->pid;
  return confirm_form($form, t('Are you sure you want to refund this payment?'), 'admin/moneysuite/orders', t('This action cannot be undone. Refunding a payment will cause most modules to remove the purchase from the user account.'), t('Refund'));
}

/**
 * Form submission handler for ms_core_refund_payment_confirm().
 */
function ms_core_resfund_payment_confirm_submit($form, &$form_state) {
  $payment = ms_core_load_payment($form['#ms_core_pid']);

  $gateway_info = ms_core_get_payment_gateway($payment->gateway);

  if (function_exists($gateway_info['refund_callback'])) {
    $params = array($payment);
    if (call_user_func_array($gateway_info['refund_callback'], $params)) {
      // Enter the payment.
      $refund_payment = ms_core_new_payment($payment->oid, $payment->gateway, 'refund');
      ms_core_enter_payment($refund_payment);
      drupal_set_message(t('Payment %number has been refunded.', array('%number' => $payment->pid)));
    } else {
      drupal_set_message(t("There was an error refunding the payment. Please see the logs for more details.", 'error'));
    }
  }

  $form_state['redirect'] = 'admin/moneysuite/orders';
}

/**
 * Form constructor for the add payment form.
 *
 * @param string $action
 *   (Optional) The action. Defaults to 'add'.
 * @param MsPayment $payment
 *   The payment if the action is 'edit'.
 *
 * @ingroup forms
 * @see ms_core_edit_payment_form_submit()
 */
function ms_core_edit_payment_form($form, &$form_state, $action = 'add', $payment = NULL) {
  ms_core_add_timeentry_js('edit-created-time');
  $form['payment'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payment'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  if ($action == 'edit') {
    $is_edit = TRUE;
    $order = ms_core_order_load($payment->oid);
    $form['payment']['pid'] = array(
      '#type' => 'value',
      '#value' => $payment->pid,
    );
    $form['payment']['paymentid'] = array(
      '#type' => 'textfield',
      '#title' => t('Payment ID'),
      '#size' => 32,
      '#maxlength' => 64,
      '#disabled' => TRUE,
      '#description' => t("The ID of this Payment"),
      '#value' => $payment->pid,
    );
    $form['payment']['oid'] = array(
      '#type' => 'value',
      '#value' => ($order) ? $order->oid : 0,
    );
    $form['payment']['orderid'] = array(
      '#type' => 'textfield',
      '#title' => t('Order ID'),
      '#size' => 32,
      '#maxlength' => 64,
      '#disabled' => TRUE,
      '#description' => t("The ID of the Order"),
      '#default_value' => ($order) ? $order->order_number : '',
    );
  } elseif (arg(3) == 'add' && ($oid = arg(4))) {
    $is_edit = FALSE;
    $order = ms_core_order_load_by_key($oid);
    $form['payment']['oid'] = array(
      '#type' => 'value',
      '#value' => ($order) ? $order->oid : 0,
    );
    $form['payment']['orderid'] = array(
      '#type' => 'textfield',
      '#title' => t('Order ID'),
      '#size' => 32,
      '#maxlength' => 64,
      '#disabled' => TRUE,
      '#description' => t("The ID of the Order"),
      '#default_value' => ($order) ? $order->order_number : '',
    );
  } else {
    $is_edit = FALSE;
    $order = FALSE;
    $form['payment']['oid'] = array(
      '#type' => 'textfield',
      '#title' => t('Order ID'),
      '#size' => 32,
      '#required' => TRUE,
    );
  }

  $today = REQUEST_TIME;
  $form['payment']['created'] = array(
    '#type' => 'date',
    '#title' => t('Date'),
    '#description' => t("Date the order was created"),
    '#default_value' => array(
      'year' => ($is_edit) ? date('Y', $payment->created) : date('Y', $today),
      'month' => ($is_edit) ? date('n', $payment->created) : date('n', $today),
      'day' => ($is_edit) ? date('j', $payment->created) : date('j', $today),
    ),
  );
  $form['payment']['created_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Time'),
    '#size' => 12,
    '#description' => t("Time the payment was created"),
    '#default_value' => ($is_edit) ? date('H:i:s', $payment->created) : date('H:i:s', $today),
  );
  $form['payment']['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => ($order) ? ms_core_get_payment_types($order->order_type) : ms_core_get_payment_types(),
    '#description' => t("The type of payment"),
    '#required' => TRUE,
    '#default_value' => ($is_edit) ? $payment->type : 'cart',
  );
  $form['payment']['gateway'] = array(
    '#type' => 'select',
    '#title' => t('Gateway'),
    '#options' => ms_core_get_payment_gateways_list(),
    '#description' => t("The gateway for this payment"),
    '#required' => TRUE,
    '#default_value' => ($is_edit) ? $payment->gateway : '',
  );
  $form['payment']['transaction'] = array(
    '#type' => 'textfield',
    '#title' => t('Transaction ID'),
    '#size' => 32,
    '#maxlength' => 64,
    '#description' => t("The transaction id"),
    '#default_value' => ($is_edit) ? $payment->transaction : '',
  );
  $form['payment']['recurring_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Recurring ID'),
    '#size' => 32,
    '#maxlength' => 64,
    '#description' => t("The recurring id"),
    '#default_value' => ($is_edit) ? $payment->recurring_id : '',
  );
  $form['payment']['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount'),
    '#size' => 32,
    '#maxlength' => 64,
    '#description' => t("The amount for this payment"),
    '#default_value' => ($is_edit) ? $payment->amount : '',
  );
  $form['payment']['currency'] = array(
    '#type' => 'select',
    '#title' => t('Currency'),
    '#options' => ms_core_get_currencies(),
    '#description' => t("The currency for this payment"),
    '#required' => TRUE,
    '#default_value' => ($is_edit) ? $payment->currency : ($order) ? $order->currency : variable_get('ms_core_default_currency', 'USD'),
  );
  $form['payment']['notify'] = array(
    '#type' => 'checkbox',
    '#title' => t('Process the Payment?'),
    '#description' => t("Check this to have modules act on the payment as if it were live."),
    '#default_value' => ($is_edit) ? 0 : 1,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form_state['redirect'] = 'admin/moneysuite/orders';
  return $form;
}

/**
 * Form submission handler for ms_core_edit_payment_form().
 */
function ms_core_edit_payment_form_submit($form, &$form_state) {
  $v = $form_state['values'];

  if (isset($v['orderid']) && ($order = ms_core_order_load($v['oid']))) {
    if (isset($v['pid']) && ($payment = ms_core_load_payment($v['pid']))) {
      $payment->oid = $order->oid;
      $payment->type = $v['type'];
      $payment->created = strtotime($v['created']['year'] . '-' . $v['created']['month'] . '-' . $v['created']['day'] . ' ' . $v['created_time']);
      $payment->gateway = $v['gateway'];
      $payment->transaction = $v['transaction'];
      $payment->amount = $v['amount'];
      $payment->currency = $v['currency'];
      $payment->recurring_id = $v['recurring_id'];

      $payment->save();
      drupal_set_message(t('Payment %number has been saved.', array('%number' => $payment->pid)));

      if ($v['notify']) {
        ms_core_enter_payment($payment, TRUE, FALSE);
      }
    } else {
      $payment = ms_core_new_payment($order->oid, $v['gateway'], $v['type']);
      $payment->created = strtotime($v['created']['year'] . '-' . $v['created']['month'] . '-' . $v['created']['day'] . ' ' . $v['created_time']);
      $payment->transaction = $v['transaction'];
      $payment->amount = $v['amount'];
      $payment->currency = $v['currency'];
      $payment->data = array('manual' => TRUE);
      $payment->recurring_id = $v['recurring_id'];
      $payment->billing_address = $order->billing_address;
      $payment->shipping_address = $order->shipping_address;

      // Add the Payment to the Order.
      ms_core_enter_payment($payment, $v['notify']);

      drupal_set_message(t('Added a new payment for Order %number.', array('%number' => $order->order_number)));
    }
  } else {
    drupal_set_message(t('Error loading the order.'));
  }

  $form_state['redirect'] = 'admin/moneysuite/orders';
}

/**
 * Form constructor for the add product form.
 *
 * @param MsOrder $order
 *   The order.
 *
 * @ingroup forms
 * @see ms_core_add_product_form_submit()
 */
function ms_core_add_product_form($form, &$form_state, $order) {
  $form['oid'] = array(
    '#type' => 'value',
    '#value' => $order->oid,
  );
  $form['orderid'] = array(
    '#type' => 'textfield',
    '#title' => t('Order ID'),
    '#size' => 32,
    '#maxlength' => 64,
    '#disabled' => TRUE,
    '#description' => t("The ID of the Order"),
    '#value' => $order->order_number,
  );
  $form['productid'] = array(
    '#type' => 'select',
    '#title' => t('Select Product'),
    '#options' => ms_core_get_module_products_list(),
    '#description' => t("Select a product from this list"),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Product'),
  );
  $form_state['redirect'] = 'admin/moneysuite/orders';

  return $form;
}

/**
 * Form submission handler for ms_core_add_product_form().
 */
function ms_core_add_product_form_submit($form, &$form_state) {
  $v = $form_state['values'];
  $order = ms_core_order_load($v['oid']);
  $product = ms_core_get_module_product($v['productid']);

  if ($product->id) {
    // Add the product to the order.
    ms_core_add_order_product($order, $product);

    drupal_set_message(t('Added product %product to Order %number.',
      array('%product' => $product->name, '%number' => $order->order_number)));
  }

  $form_state['redirect'] = 'admin/moneysuite/orders';
}

/**
 * Form constructor for the rebuild orders form.
 *
 * @ingroup forms
 * @see ms_core_rebuild_orders_confirm_submit()
 */
function ms_core_rebuild_orders_confirm($form, &$form_state) {
  return confirm_form($form, t('Are you sure you want to rebuild the orders?'), 'admin/moneysuite/orders',
    t('This will refresh the order data object to use the latest override settings from the product(s) in the order. This will also delete all orphaned records for non-existing orders. This action cannot be undone.'),
    t('Rebuild Orders'));
}

/**
 * Form submission handler for ms_core_rebuild_orders_confirm().
 */
function ms_core_rebuild_orders_confirm_submit($form, &$form_state) {
  // Load all of the orders from the database.
  $result = db_query("SELECT * FROM {ms_orders}");
  foreach ($result as $row) {
    // For each order, load the products && get the override settings && data
    // into the order object.
    $order = ms_core_order_load($row->oid);
    $data = $order->data;
    foreach ($order->products as $item) {
      if ($product = ms_core_load_module_product($item->module, $item->id)) {
        $data = array_merge($data, $product->data);
      }
    }

    ms_core_add_data($order, $data);
  }

  drupal_set_message(t('Successfully rebuilt the orders.'));

  ms_core_delete_orphaned_records();

  $form_state['redirect'] = 'admin/moneysuite/orders';
}

<?php

/**
 * @file
 * Contains code specific to recurring payments and gateway helper code.
 *
 * Original author: Leighton Whiting - Released under GENERAL PUBLIC LICENSE
 * Current maintenance by multiple MoneySuite users.
 * Re: new initiative: https://www.drupal.org/node/2315653 
 */

/**
 * Access callback: Checka if the user can modify their order.
 *
 * @param int $oid
 *   The order id.
 * @param int $id
 *   The id of the new product.
 *
 * @return bool
 *   TRUE if the user has access, FALSE otherwise.
 */
function ms_core_modify_test($oid, $id) {
  global $user;
  $order = ms_core_order_load($oid);
  if (user_access('administer moneysuite orders') OR ($user->uid == $order->uid)) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Access callback: Checks if the user can cancel their order.
 *
 * @param int $oid
 *   The order id.
 *
 * @return bool
 *   TRUE if the user has access, FALSE otherwise.
 */
function ms_core_cancel_test($oid) {
  global $user;
  $order = ms_core_order_load($oid);
  if ($user->uid == $order->uid OR user_access('administer moneysuite orders')) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Gets the Cancellation URL for a recurring order.
 *
 * @param MsOrder $order
 *   The order object.
 *
 * @return string
 *   The url path for the billing page.
 */
function ms_core_get_billing_helper_url($order) {
  return 'user/' . $order->uid . '/billing-info';
}

/**
 * Gets the Modification URL for a recurring order.
 *
 * @param MsOrder $order
 *   The order object.
 * @param int $new_product_id
 *   The new product id to modify to.
 * @param int $old_product_id
 *   (Optional) The old product id to modify from. Only applies to multi_recurring
 *   orders.
 *
 * @return string
 *   The url path for the modify order page.
 */
function ms_core_get_modify_helper_url($order, $new_product_id, $old_product_id = NULL) {
  $url = 'ms/modify/' . $order->oid . '/' . $new_product_id;
  if (!is_null($old_product_id)) {
    $url .= '/' . $old_product_id;
  }
  return $url;
}

/**
 * Gets the Cancellation URL for a recurring order.
 *
 * @param MsOrder $order
 *   The order object.
 * @param string $product_id
 *   (Optional) The product id to modify. Only applies to multi_recurring orders.
 *
 * @return string
 *   The url path for the cancel page.
 */
function ms_core_get_cancel_helper_url($order, $product_id = NULL) {
  $url = 'ms/cancel/' . $order->oid;

  if (!is_null($product_id)) {
    $url .= '/' . $product_id;
  }

  return $url;
}

/**
 * Form constructor for the cancellation form.
 *
 * @param int $oid
 *   The order id.
 *
 * @ingroup forms
 * @see ms_core_cancel_confirm_submit()
 */
function ms_core_cancel_confirm($form, &$form_state, $oid) {
  $order = ms_core_order_load($oid);
  $form['#ms_core_oid'] = $order->oid;
  $product_id = arg(3);
  if (!empty($product_id)) {
    $form['#ms_core_pid'] = $product_id;
  }
  return confirm_form($form, t('Are you sure you want to cancel your order?'), '', t('This action cannot be undone.'), t('Confirm'));
}

/**
 * Form submission handler for ms_core_cancel_confirm().
 */
function ms_core_cancel_confirm_submit($form, &$form_state) {
  $order = ms_core_order_load($form['#ms_core_oid']);

  // Enter the rec_cancel payment.
  $payment = ms_core_new_payment($order->oid, $order->gateway, 'rec_cancel');

  // Generate a Unique Transaction ID.
  $payment->transaction = ms_core_generate_transaction_id(10);
  $payment->currency = $order->currency;
  $payment->billing_address = $order->billing_address;
  $payment->shipping_address = $order->shipping_address;
  $payment->recurring_id = $order->oid;

  if (!empty($form['#ms_core_pid'])) {
    $payment->data['product_id'] = $form['#ms_core_pid'];
  }

  ms_core_enter_payment($payment);

  $form_state['redirect'] = '';
}

/**
 * Form constructor for the upgrade order form.
 *
 * @param int $oid
 *   The order id.
 * @param int $id
 *   The new product id.
 *
 * @ingroup forms
 * @see ms_core_modify_confirm_submit()
 */
function ms_core_modify_confirm($form, &$form_state, $oid, $id) {
  $order = ms_core_order_load($oid);
  $new_product = ms_core_load_module_product(ms_core_get_order_module($order), $id);

  // Show if there will be a charge.
  $details_text = t('Your order will be modified immediately. You will not be charged.');
  if ($amount = ms_core_get_outstanding_modification_amount($new_product, $order, arg(4))) {
    $details_text = t("You will be charged @amount if you proceed.",
      array('@amount' => ms_core_format_money($amount, $order->currency)));
  }

  $product_id = arg(4);
  if (!empty($product_id)) {
    $form['#ms_core_pid'] = $product_id; // Store the id of the product in the order to modify.
  }

  $form['#ms_core_oid'] = $order->oid;
  $form['#ms_core_id'] = $id;
  return confirm_form($form, t('Are you sure you want to change your order to %new_prod?',
    array('%new_prod' => $new_product->name)), '', $details_text, t('Confirm'));
}

/**
 * Form submission handler for ms_core_modify_confirm().
 */
function ms_core_modify_confirm_submit($form, &$form_state) {
  $approved = TRUE;

  // Load the order and products.
  $order = ms_core_order_load($form['#ms_core_oid']);

  $new_product = ms_core_load_module_product(ms_core_get_order_module($order), $form['#ms_core_id']);

  // Remove trial periods if any from modifications.
  $new_product->recurring_schedule['has_trial'] = FALSE;
  $new_product->recurring_schedule['trial_length'] = 0;

  // Remove any coupons from the order.
  ms_core_remove_order_adjustments($order);

  $product_id = !empty($form['#ms_core_id']) ? $form['#ms_core_id'] : NULL;

  if ($amount = ms_core_get_outstanding_modification_amount($new_product, $order, $product_id)) {
    // Ask the gateway to charge the order.
    $approved = module_invoke($order->gateway, 'ms_core_modification_charge', $amount, $order);
  }

  if ($approved) {
    $data = array();
    $data['new_product'] = $new_product;
    if (!empty($product_id)) {
      $data['product_id'] = $product_id;
    }

    $order = ms_core_add_data($order, $data);

    // Enter the rec_modify payment.
    $payment = ms_core_new_payment($order->oid, $order->gateway, 'rec_modify');

    // Generate a Unique Transaction ID.
    $payment->transaction = ms_core_generate_transaction_id(10);
    $payment->amount = $amount;
    $payment->currency = $order->currency;
    $payment->data = $data;
    $payment->billing_address = $order->billing_address;
    $payment->shipping_address = $order->shipping_address;
    $payment->recurring_id = $order->oid;

    ms_core_enter_payment($payment);

    $form_state['redirect'] = '';
  }
}

/**
 * Form constructor for the downgrade order form.
 *
 * @param int $oid
 *   The order id.
 * @param int $id
 *   The new product id.
 *
 * @ingroup forms
 * @see ms_core_downgrade_confirm_submit()
 */
function ms_core_downgrade_confirm($form, &$form_state, $oid, $id) {
  $order = ms_core_order_load($oid);
  $new_product = ms_core_load_module_product(ms_core_get_order_module($order), $id);

  $details_text = t("When your current plan expires, it will be changed to @new_prod.",
    array('@new_prod' => $new_product->name));

  $product_id = arg(4);
  if (!empty($product_id)) {
    $form['#ms_core_pid'] = $product_id; // Store the id of the product in the order to modify.
  }

  $form['#ms_core_oid'] = $order->oid;
  $form['#ms_core_id'] = $id;
  return confirm_form($form, t('Are you sure you want to change your order to %new_prod?',
    array('%new_prod' => $new_product->name)), '', $details_text, t('Confirm'));
}

/**
 * Form submission handler for ms_core_downgrade_confirm().
 */
function ms_core_downgrade_confirm_submit($form, &$form_state) {
  // Load the order and products.
  $order = ms_core_order_load($form['#ms_core_oid']);
  $new_product = ms_core_load_module_product(ms_core_get_order_module($order), $form['#ms_core_id']);

  // Remove trial periods if any from modifications.
  $new_product->recurring_schedule['has_trial'] = FALSE;
  $new_product->recurring_schedule['trial_length'] = 0;

  // Remove any coupons from the order.
  ms_core_remove_order_adjustments($order);

  $product_id = !empty($form['#ms_core_id']) ? $form['#ms_core_id'] : NULL;

  $data = array();
  $data['new_product'] = $new_product;
  if (!empty($product_id)) {
    $data['product_id'] = $product_id;
  }

  $order = ms_core_add_data($order, $data);

  $payment = ms_core_new_payment($order->oid, $order->gateway, 'rec_downgrade', 0, $order->currency);
  $payment->data = $data;
  ms_core_enter_payment($payment);

  // @todo - This might not be needed anymore because of multi-recurring. We should change everything that uses the order's recurring schedule to use the first product's recurring schedule instead, right?
  $order = ms_core_set_recurring_schedule($order, $new_product->recurring_schedule);

  // Create a recurring schedule if one doesn't exist.
  $recurring_schedule = ms_core_create_recurring_schedule($order->oid, $order->gateway, ms_core_get_order_module($order), $order->recurring_schedule, REQUEST_TIME, $product_id);
  $recurring_schedule = (object)array_merge((array)$recurring_schedule, $new_product->recurring_schedule);
  $recurring_schedule->modified = REQUEST_TIME;
  $recurring_schedule->status = 'pending_downgrade';
  ms_core_update_recurring_schedule($recurring_schedule);

  drupal_set_message(t("Your plan has been downgraded successfully. When your current plan expires, it will be changed to @new_prod.",
    array('@new_prod' => $new_product->name)));

  $form_state['redirect'] = '';
}

/**
 * Calculates the outstanding amount that should be paid for a modification.
 *
 * @param MsProduct $new_product
 *   The new product.
 * @param MsOrder $order
 *   The order object.
 * @param string $product_id
 *   (Optional) The product id. Only applies to multi_recurring orders.
 *
 * @return float
 *   The amount that should be charged in order to proceed with the
 *   modification.
 */
function ms_core_get_outstanding_modification_amount($new_product, $order, $product_id = NULL) {
  if ($amount = $new_product->recurring_schedule['main_amount']) {
    if (!is_null($product_id)) {
      if (isset($order->data['residual_credit_' . $product_id]) && ($credit = $order->data['residual_credit_' . $product_id])) {
        $amount -= $credit;
      }
    } else {
      if (isset($order->data['residual_credit']) && ($credit = $order->data['residual_credit'])) {
        $amount -= $credit;
      }
    }

    // Add in the adjustments as well, if there are new adjustments.
    if (!empty($new_product->data['adjustments'])) {
      foreach ($new_product->data['adjustments'] as $adjustment_info) {
        if ($adjustment_info['active']) {
          $amount += $adjustment_info['value'];
        }
      }
    }

    if ($amount > 0) {
      // Since most gateways don't support charges for under $1, let's set that
      // as a base amount.
      if ($amount < 1) {
        $amount = 1.00;
      }
      return $amount;
    }
  }

  return 0;
}

/**
 * Inserts a new recurring schedule.
 *
 * @param int $oid
 *   The order id.
 * @param string $gateway
 *   The gateway.
 * @param string $module
 *   The module.
 * @param array $recurring_schedule
 *   The recurring schedule of the order or product.
 * @param int $created
 *   (Optional) The timestamp that should be used for creation. Defaults to
 *   now.
 * @param int $product_id
 *   (Optional) The product id of the order product this recurring schedule is for.
 *
 * @return object
 *   The new recurring schedule object, or the old one if it already existed.
 */
function ms_core_create_recurring_schedule($oid, $gateway, $module, $recurring_schedule, $created = NULL, $product_id = NULL) {
  $order = ms_core_order_load($oid);

  // If a recurring schedule already exists, just return it.
  if ($existing = ms_core_load_recurring_schedule($oid, $product_id)) {
    return $existing;
  }
  // We also check for an existing recurring schedule for the order without the product id, which is useful in cases
  // of recurring type orders that are only supposed to have 1 recurring schedule record.
  else if ($order->order_type == 'recurring' && $existing = ms_core_load_recurring_schedule($oid)) {
    return $existing;
  }
  $created = ($created) ? $created : REQUEST_TIME;

  $last_payment = $created;

  if ($lp = ms_core_get_last_payment($order)) {
    $last_payment = $lp->created;
  }

  // Calculate when the next payment should be.
  if (!empty($recurring_schedule['fixed_date'])) {
    $next_payment = ms_core_calculate_next_fixed_date($recurring_schedule['fixed_date_string'], $recurring_schedule['fixed_date_type'], $last_payment);
  } else {
    if ($recurring_schedule['has_trial']) {
      // Add a payment for the trial to total_occurrences.
      if ($recurring_schedule['total_occurrences']) {
        $recurring_schedule['total_occurrences'] += 1;
      }
      $next_payment = ms_core_get_string_timestamp($recurring_schedule['trial_length'], $recurring_schedule['trial_unit'], $last_payment);
    } else {
      $next_payment = ms_core_get_string_timestamp($recurring_schedule['main_length'], $recurring_schedule['main_unit'], $last_payment);
    }
  }

  // Calculate the expiration based on the start date, trial period, etc.
  if ($recurring_schedule['total_occurrences']) {
    // Add time based on number of payments multiplied by period length and unit.
    $regular_time = ms_core_get_string_timestamp($recurring_schedule['main_length'], $recurring_schedule['main_unit'], 0);
    $date = $regular_time * $recurring_schedule['total_occurrences'];

    // Add Time for Trial Period 1.
    if ($recurring_schedule['trial_length']) {
      $date += ms_core_get_string_timestamp($recurring_schedule['trial_length'], $recurring_schedule['trial_unit'], 0);
    }

    $expiration = $created + $date;
  } else {
    $expiration = 0;
  }

  // Set the additional values.
  $recurring_schedule += array(
    'oid' => $oid,
    'status' => 'active',
    'gateway' => $gateway,
    'module' => $module,
    'next_payment' => $next_payment,
    'current_payments' => 0,
    'created' => $created,
    'expiration' => $expiration,
    'modified' => $created,
    'pid' => $product_id,
  );

  // Insert the recurring schedule.
  drupal_write_record('ms_recurring_schedules', $recurring_schedule);

  return (object)$recurring_schedule;
}

/**
 * Calculates the expiration based on the start date, trial period, etc.
 *
 * @param array $recurring_schedule
 *   The recurring schedule array.
 *
 * @return int
 *   The timestamp of the expiration string.
 */
function ms_core_calculate_expiration_date($recurring_schedule) {
  if (!empty($recurring_schedule['total_occurrences'])) {
    // Add time based on number of payments multiplied by period length and unit.
    $regular_time = ms_core_get_string_timestamp($recurring_schedule['main_length'], $recurring_schedule['main_unit'], 0);
    $date = $regular_time * $recurring_schedule['total_occurrences'];

    // Add Time for Trial Period 1.
    if ($recurring_schedule['trial_length']) {
      $date += ms_core_get_string_timestamp($recurring_schedule['trial_length'], $recurring_schedule['trial_unit'], 0);
    }

    return $date;
  }

  return 0;
}

/**
 * Calculates the date of the next payment from a recurring schedule.
 *
 * @param MsOrder $order
 *   The order object, passed by reference.
 * @param int $last_payment
 *   The timestamp of the last payment.
 * @param int $product_id
 *   (Optional) The product id.
 *
 * @return
 *   The timestamp of the next payment. If the recurring schedule has a trial
 *   period, then the total_occurrences is incremented by one if it is not
 *   unlimited.
 */
function ms_core_calculate_next_payment_date(&$order, $last_payment, $product_id = NULL) {
  $recurring_schedule = $order->recurring_schedule;
  if ($product_id) {
    foreach ($order->products as $product) {
      if ($product_id == $product->order_product_id) {
        $recurring_schedule = $product->recurring_schedule;
      }
    }
  }

  if (!empty($recurring_schedule['fixed_date'])) {
    return ms_core_calculate_next_fixed_date($recurring_schedule['fixed_date_string'], $recurring_schedule['fixed_date_type'], $last_payment);
  } else {
    // If this is the first payment, and
    // there is a trial period that is not free, then the next payment is after
    // the trial period.
    if (count($order->payments) == 1 && $recurring_schedule['has_trial']) {
      // Add a payment for the trial to total_occurrences if needed.
      if ($recurring_schedule['total_occurrences']) {
        $recurring_schedule['total_occurrences'] += 1;
      }
      return ms_core_get_string_timestamp($recurring_schedule['trial_length'], $recurring_schedule['trial_unit'], $last_payment);
    } else {
      return !empty($recurring_schedule['main_length']) ? ms_core_get_string_timestamp($recurring_schedule['main_length'], $recurring_schedule['main_unit'], $last_payment) : 0;
    }
  }
}

/**
 * Updates a recurring schedule object.
 *
 * @param object $recurring_schedule
 *   The recurring schedule object to update. Must have the id key set.
 */
function ms_core_update_recurring_schedule($recurring_schedule) {
  return drupal_write_record('ms_recurring_schedules', $recurring_schedule, 'id');
}

/**
 * Loads a recurring_schedule.
 *
 * @param int $oid
 *   The order id.
 * @param int $product_id
 *   (Optional) The product id if applicable.
 *
 * @return object
 *   The recurring schedule object.
 */
function ms_core_load_recurring_schedule($oid, $product_id = NULL, $active = TRUE) {
  $query = db_select('ms_recurring_schedules', 'rs')
    ->fields('rs')
    ->condition('oid', $oid);
  if (!is_null($product_id)) {
    $query->condition('pid', $product_id);
  }
  if ($active) {
    $query->condition('status', 'active');
  }
  $result = $query->execute();

  foreach ($result as $recurring_schedule) {
    return $recurring_schedule;
  }
  return FALSE;
}

/**
 * Increments the recurring schedule and resets the next payment.
 *
 * @param int $oid
 *   The order id.
 * @param int $time
 *   (Optional) The time used to calculate the next payment. Defaults to now.
 * @param int $product_id
 *   (Optional) The product id the payment was for. Defaults to NULL.
 */
function ms_core_increment_recurring_schedule($oid, $time = NULL, $product_id = NULL) {
  $time = !empty($time) ? $time : REQUEST_TIME;

  $order = ms_core_order_load($oid);
  // Attempt to load the recurring schedule. If not found, attempt to create one.
  if (!$recurring_schedule = ms_core_load_recurring_schedule($oid, $product_id)) {
    ms_core_create_recurring_schedule($order->oid, $order->gateway, ms_core_get_order_module($order), $order->recurring_schedule, REQUEST_TIME, $product_id);
    $recurring_schedule = ms_core_load_recurring_schedule($oid, $product_id);
  }
  if ($recurring_schedule) {
    // Calculate the next payment date.
    $next_payment = ms_core_calculate_next_payment_date($order, $time, $product_id);

    // Handle Failed Payments
    if ($recurring_schedule->failed_payments) {
      // Subtract 3 days for each failed payment.
      $next_payment -= 259200 * $recurring_schedule->failed_payments;

      // If the $next_payment is before $time, we need to keep some of the
      // failed payments.
      $failed_payments = 0;
      if (variable_get('ms_core_charge_back_payments', FALSE)) {
        if ($next_payment < $time) {
          $failed_payments = floor(($time - $next_payment) / 259200);
        }
      }

      // Clear the failed payments counter.
      db_update('ms_recurring_schedules')
        ->fields(array(
          'failed_payments' => $failed_payments,
        ))
        ->condition('id', $recurring_schedule->id)
        ->execute();
    }

    // Update into Database.
    db_update('ms_recurring_schedules')
      ->fields(array(
        'current_payments' => $recurring_schedule->current_payments + 1,
        'next_payment' => $next_payment,
        'notified' => 0,
      ))
      ->condition('id', $recurring_schedule->id)
      ->execute();

    return $next_payment;
  } else {
    ms_core_log_error('ms_core', 'Unable to load the recurring schedule for a rec_payment increment. Order #: !order',
      array('!order' => $oid));
    return FALSE;
  }
}

/**
 * Cancels the recurring schedule and sets next payment and expiration.
 *
 * @param int $oid
 *   The order id.
 * @param int $date
 *   (Optional) The date to use for the expiration date calculation. Defaults
 *   to now.
 */
function ms_core_cancel_recurring_schedule($oid, $product_id = NULL) {
  $expiration = 0;

  // Load the recurring schedule.
  if ($recurring_schedule = ms_core_load_recurring_schedule($oid, $product_id)) {
    // Calculate the Expiration Date.
    $now = REQUEST_TIME;
    $trial1 = 0;

    // Calculate time based on number of payments multiplied by period length and
    // unit.
    $regular_time = ms_core_get_string_timestamp($recurring_schedule->main_length, $recurring_schedule->main_unit, 0);

    if ($recurring_schedule->current_payments) {
      $regular_time = $regular_time * $recurring_schedule->current_payments;
    }

    // Calculate Time for Trial Period 1.
    if ($recurring_schedule->trial_unit) {
      $trial1 = ms_core_get_string_timestamp($recurring_schedule->trial_length, $recurring_schedule->trial_unit, 0);
    }

    if (($recurring_schedule->created + $trial1) > $now) {
      $expiration = $recurring_schedule->created + $trial1;
    } elseif (($recurring_schedule->created + $trial1 + $regular_time) > $now) {
      $expiration = $recurring_schedule->created + $trial1 + $regular_time;
    } else {
      $expiration = $now;
    }

    // Update into Database.
    $query = db_update('ms_recurring_schedules')
      ->fields(array(
        'total_occurrences' => $recurring_schedule->current_payments,
        'expiration' => $expiration,
        'status' => 'cancelled',
      ))
      ->condition('id', $recurring_schedule->id);
    $query->execute();
  }
  return $expiration;
}

/**
 * Marks a recurring schedule as expiring.
 *
 * @param int $oid
 *   The order id.
 * @param string $status
 *   The new status.
 */
function ms_core_change_recurring_schedule_status($oid, $status, $product_id = NULL) {
  // Update into Database.
  $query = db_update('ms_recurring_schedules')
    ->fields(array(
      'status' => $status,
    ))
    ->condition('oid', $oid);

  if (!empty($product_id)) {
    $query->condition('pid', $product_id);
  }
  $result = $query->execute();

  return $result;
}

/*
 * Calculates how much credit a user has left in a recurring product.
 *
 * @param int $oid
 *   The order id.
 * @param int $expiration
 *   (Optional) The expiration timestamp. Defaults to 0 (no expiration).
 * @param float $max
 *   (Optional) The maximum credit that can be given. Defaults 0.
 *
 * @return
 *   The credit that is given for an order.
 */
function ms_core_calculate_credit($oid, $expiration = 0, $max = 0) {
  // Load the order.
  if ($order = ms_core_order_load($oid)) {
    // Set some variables.
    $last_payment_time = $order->created;
    $price = $order->amount;

    /**
     * The problem with this is sometimes users can pay for multiple at
     * once, so we shouldn't use the payment amount, but rather the amount of
     * the product.
     *
     * This happens when upgrading from non-recurring orders with other products
     * bundled.
     *
     * This is why the new $max parameter was introduced.
     */

    // See if there is a payment entered for this order.
    if ($lp = ms_core_get_last_payment($order)) {
      switch ($lp->type) {
        case 'cart':
        case 'rec_signup':
        case 'rec_payment':
          $last_payment_time = $lp->created;
          $price = $lp->amount;
          break;
      }
    }

    if ($price > $max) {
      $price = $max;
    }

    $time_left = ($expiration) ? $expiration - REQUEST_TIME : 0;
    $total_time = ($expiration) ? $expiration - $last_payment_time : 0;

    // Change the values if it is recurring.
    if ($order->order_type == 'recurring') {
      if (($recurring_schedule = ms_core_load_recurring_schedule($order->oid)) && $recurring_schedule->next_payment) {
        $time_left = $recurring_schedule->next_payment - REQUEST_TIME;
        $total_time = $recurring_schedule->next_payment - $last_payment_time;
      }
    }
    if ($total_time > 0) {
      $credit = ($time_left / $total_time) * $price;
    } else {
      $credit = $price;
    }

    return ($credit > 0) ? round($credit, 2) : 0;
  } else {
    return 0;
  }
}

/**
 * Loads a payment_profile from the database by id.
 *
 * @param int $id
 *   The id of the payment profile.
 *
 * @return object
 *    The payment_profile object.
 */
function ms_core_payment_profile_load($id) {
  if (is_null($id)) {
    return FALSE;
  }

  $result = db_query("SELECT * FROM {ms_core_payment_profiles} WHERE id = :id", array(':id' => $id));
  foreach ($result as $profile) {
    return $profile;
  }
  return FALSE;
}

/**
 * Loads a payment_profile from the database.
 *
 * @param int $oid
 *   The order id or blank to load the session's order.
 *
 * @return object
 *    The payment_profile object.
 */
function ms_core_payment_profile_load_by_oid($oid) {
  if (is_null($oid) || $oid < 1) {
    return FALSE;
  }

  $result = db_query("SELECT * FROM {ms_core_payment_profiles} WHERE oid = :oid", array(':oid' => $oid));
  foreach ($result as $profile) {
    return $profile;
  }
  return FALSE;
}

/**
 * Loads the payment profiles for a user.
 *
 * @param int $uid
 *   The user id.
 * @param string $module
 *   (optional) The module associated with the payment profile.
 *
 * @return array
 *    Return an array of the payment_profile objects.
 */
function ms_core_payment_profiles_load_by_user($uid, $module = '') {
  if (is_null($uid) || $uid < 1) {
    return array();
  }

  if ($module) {
    $result = db_query("SELECT * FROM {ms_core_payment_profiles} WHERE uid = :uid AND module = :module",
      array(':uid' => $uid, ':module' => $module));
  } else {
    $result = db_query("SELECT * FROM {ms_core_payment_profiles} WHERE uid = :uid",
      array(':uid' => $uid));
  }
  $profiles = array();
  foreach ($result as $profile) {
    $profiles[] = $profile;
  }

  return $profiles;
}

/**
 * Loads the first payment profile for a user.
 *
 * @param int $uid
 *   The user id.
 * @param string $module
 *   (optional) The module associated with the payment profile.
 *
 * @return object
 *   The first payment profile.
 */
function ms_core_payment_profile_load_by_user($uid, $module = '') {
  $profiles = ms_core_payment_profiles_load_by_user($uid, $module);
  if (!empty($profiles)) {
    return $profiles[0];
  }
  return FALSE;
}

/**
 * Deletes a Payment Profile for an order.
 *
 * @param int $oid
 *   The order id.
 */
function ms_core_payment_profile_delete($oid) {
  db_delete('ms_core_payment_profiles')
    ->condition('oid', $oid)
    ->execute();
}

/**
 * Saves a payment_profile object.
 *
 * @param object $payment_profile
 *   The payment_profile object to save.
 *
 * @return bool
 *    Return the DB Result (boolean true) or FALSE.
 */
function ms_core_payment_profile_save($pp) {
  // Convert to object if it is an array.
  if (is_array($pp)) {
    $pp = (object)$pp;
  }

  $pp->cc_num = ms_core_mask_cc($pp->cc_num);

  if (isset($pp->id)) {
    // Update the record.
    drupal_write_record('ms_core_payment_profiles', $pp, 'id');
  } else {
    drupal_write_record('ms_core_payment_profiles', $pp);
  }

  return $pp;
}

/**
 * Gets the last payment string for an order.
 *
 * @param object $recurring_schedule
 *   The recurring_schedule object to get the last payment string for.
 *
 * @return string
 *   The date and amount of the last payment, or 'None'.
 */
function ms_core_get_last_payment_string($recurring_schedule) {
  $order = ms_core_order_load($recurring_schedule->oid);
  $lp = ms_core_get_last_payment($order, TRUE, $recurring_schedule->pid);
  if (isset($lp->created)) {
    return format_date($lp->created, 'small') . " - " . ms_core_format_money($lp->amount, $lp->currency);
  } else {
    return t('None');
  }
}

/**
 * Retrieves the recurring schedule for an order.
 *
 * @param int $oid
 *   The order id.
 * @param string $product_id
 *   (Optional) The product id or sku.
 * @param string $product_module
 *   (Optional) The product module.
 *
 * @return
 *   The recurring schedule, or FALSE if none found.
 */
function ms_core_get_recurring_schedule($oid, $product_id = NULL, $product_module = NULL) {
  if (is_null($product_id)) {
    return ms_core_load_recurring_schedule($oid);
  } else {
    $order = ms_core_order_load($oid);
    if ($order) {
      $order_product_id = NULL;
      if ($order->order_type == 'multi_recurring') {
        foreach ($order->products as $product) {
          if ($product->id == $product_id && $product->module == $product_module) {
            $order_product_id = $product->order_product_id;
          }
        }
      }
      return ms_core_load_recurring_schedule($order->oid, $order_product_id);
    }
  }
  return FALSE;
}

/**
 * Gets the next payment string for an order.
 *
 * @param object $recurring_schedule
 *   The recurring_schedule object to get the next payment string for.
 *
 * @return string
 *   The date and amount of the next payment, or 'None'.
 */
function ms_core_get_next_payment_string($recurring_schedule) {
  if (in_array($recurring_schedule->status, array('active', 'pending_downgrade')) && $recurring_schedule->next_payment > REQUEST_TIME) {
    return ms_core_get_next_payment_date($recurring_schedule) . " - " . ms_core_get_next_payment_amount($recurring_schedule);
  } else {
    return t('None');
  }
}

/**
 * Gets the next payment date.
 *
 * @param object $recurring_schedule
 *   The recurring_schedule object.
 *
 * @return string
 *   The next payment date.
 */
function ms_core_get_next_payment_date($recurring_schedule) {
  if (in_array($recurring_schedule->status, array('active', 'pending_downgrade')) && $recurring_schedule->next_payment > time()) {
    return format_date($recurring_schedule->next_payment, 'small');
  } else {
    return t('N/A');
  }
}

/**
 * Gets the next payment amount.
 *
 * @param object $recurring_schedule
 *   The recurring_schedule object.
 *
 * @return string
 *   The next payment amount.
 */
function ms_core_get_next_payment_amount($recurring_schedule) {
  if (in_array($recurring_schedule->status, array('active', 'pending_downgrade')) && $recurring_schedule->next_payment > time()) {
    $order = ms_core_order_load($recurring_schedule->oid);
    return ms_core_format_money($recurring_schedule->main_amount, $order->currency);
  } else {
    return t('N/A');
  }
}

/**
 * Form constructor for the edit payment profile form.
 *
 * @param object $account
 *   The user account.
 * @param int $id
 *   (Optional) The id of the payment profile.
 *
 * @ingroup forms
 */
function ms_core_payment_profile_edit_form($form, &$form_state, $account, $id = NULL) {
  $module = '';
  if ($id == 'new') {
    $module = arg(5);
    $payment_profile = FALSE;
  } else if ($payment_profile = ms_core_payment_profile_load($id)) {
    $form['profile_id'] = array(
      '#type' => 'value',
      '#value' => $id,
    );
    $module = $payment_profile->module;
  }

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );

  $form['card_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Credit Card Information'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('Please enter the new billing information you would like to use.'),
  );

  // Credit Card Info.
  $form['card_info']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => ms_core_get_checkout_field_title('cc_first_name'),
    '#size' => 80,
    '#maxlength' => 120,
    '#required' => TRUE,
    '#desription' => ms_core_get_checkout_field_description('cc_first_name'),
    '#default_value' => ($payment_profile) ? $payment_profile->first_name : '',
  );
  $form['card_info']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => ms_core_get_checkout_field_title('cc_last_name'),
    '#size' => 80,
    '#maxlength' => 120,
    '#required' => TRUE,
    '#desription' => ms_core_get_checkout_field_description('cc_last_name'),
    '#default_value' => ($payment_profile) ? $payment_profile->last_name : '',
  );
  $allowed_cards = array(
    'visa' => 'Visa',
    'mc' => 'MasterCard',
    'amex' => 'American Express',
    'discover' => 'Discover',
    'diners' => 'Diners',
    'maestro' => 'Maestro',
  );
  if (($cards = variable_get($module . '_allowed_cards', array())) && is_array($cards) && !empty($cards)) {
    $allowed_cards = array_intersect_key($allowed_cards, $cards);
  }
  $form['card_info']['cc_type'] = array(
    '#type' => 'select',
    '#title' => t('Card Type'),
    '#options' => $allowed_cards,
    '#description' => t("Select the Card Type."),
    '#required' => TRUE,
    '#default_value' => ($payment_profile) ? $payment_profile->cc_type : '',
  );
  $form['card_info']['cc_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Credit Card Number'),
    '#size' => 40,
    '#maxlength' => 20,
    '#description' => t("The Credit Card Number. You must enter the full number
      when changing any of the other information. Card on record: @card", array(
      '@card' => ($payment_profile) ? $payment_profile->cc_num : t('N/A'))),
    '#required' => TRUE,
    '#default_value' => '',
  );
  $form['card_info']['cc_cvv'] = array(
    '#type' => 'textfield',
    '#title' => t('Credit Card Verification Code'),
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t("Enter the Credit Card Verification Code."),
    '#required' => TRUE,
  );
  $form['card_info']['exp_month'] = array(
    '#type' => 'select',
    '#title' => t('Month'),
    '#options' => ms_core_get_months(),
    '#description' => t("The month this credit card expires."),
    '#default_value' => ($payment_profile) ? str_pad($payment_profile->exp_month, 2, 0, STR_PAD_LEFT) : '',
    '#required' => TRUE,
  );
  $form['card_info']['exp_year'] = array(
    '#type' => 'select',
    '#title' => t('Year'),
    '#options' => ms_core_get_years(),
    '#description' => t("The year this credit card expires."),
    '#default_value' => ($payment_profile) ? $payment_profile->exp_year : '',
    '#required' => TRUE,
  );

  $form['billing_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Billing Info'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // Billing Info.
  $form['billing_info']['address'] = array(
    '#type' => 'textfield',
    '#title' => ms_core_get_checkout_field_title('billing_address1'),
    '#size' => 80,
    '#required' => TRUE,
    '#description' => ms_core_get_checkout_field_description('billing_address1'),
    '#default_value' => ($payment_profile) ? $payment_profile->address : '',
  );
  $form['billing_info']['city'] = array(
    '#type' => 'textfield',
    '#title' => ms_core_get_checkout_field_title('billing_city'),
    '#size' => 40,
    '#required' => TRUE,
    '#description' => ms_core_get_checkout_field_description('billing_city'),
    '#default_value' => ($payment_profile) ? $payment_profile->city : '',
  );
  $form['billing_info']['state'] = array(
    '#type' => 'textfield',
    '#title' => ms_core_get_checkout_field_title('billing_state'),
    '#size' => 40,
    '#description' => ms_core_get_checkout_field_description('billing_state'),
    '#required' => TRUE,
    '#default_value' => ($payment_profile) ? $payment_profile->state : '',
  );
  $form['billing_info']['zip'] = array(
    '#type' => 'textfield',
    '#title' => ms_core_get_checkout_field_title('billing_zip'),
    '#size' => 20,
    '#required' => TRUE,
    '#description' => ms_core_get_checkout_field_description('billing_zip'),
    '#default_value' => ($payment_profile) ? $payment_profile->zip : '',
  );
  $form['billing_info']['country'] = array(
    '#type' => 'select',
    '#title' => ms_core_get_checkout_field_title('billing_country'),
    '#options' => ms_core_get_countries('iso'),
    '#default_value' => ($payment_profile) ? $payment_profile->country : '',
    '#description' => ms_core_get_checkout_field_description('billing_country'),
    '#required' => TRUE,
  );
  $form['billing_info']['phone'] = array(
    '#type' => 'textfield',
    '#title' => ms_core_get_checkout_field_title('billing_phone'),
    '#size' => 40,
    '#required' => FALSE,
    '#description' => ms_core_get_checkout_field_description('billing_phone'),
    '#default_value' => ($payment_profile) ? $payment_profile->phone : '',
  );
  $form['billing_info']['email'] = array(
    '#type' => 'textfield',
    '#title' => ms_core_get_checkout_field_title('billing_email'),
    '#size' => 80,
    '#required' => TRUE,
    '#description' => ms_core_get_checkout_field_description('billing_email'),
    '#default_value' => ($payment_profile) ? $payment_profile->email : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['submit']['#validate'][] = 'ms_core_cc_form_validate';
  if (function_exists($module . '_payment_profile_edit_form_validate')) {
    $form['submit']['#validate'][] = $module . '_payment_profile_edit_form_validate';
  }

  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  if (function_exists($module . '_payment_profile_delete_form_validate')) {
    $form['delete']['#validate'][] = $module . '_payment_profile_delete_form_validate';
  }

  $form['delete']['#submit'][] = 'ms_core_payment_profile_delete_form_submit';

  return $form;
}

/**
 * Form submission handler for ms_core_payment_profile_edit_form().
 *
 * This is called by the delete button. Deletes the payment profile from the
 * database.
 */
function ms_core_payment_profile_delete_form_submit($form, &$form_state) {
  db_delete('ms_core_payment_profiles')
    ->condition('id', $form_state['values']['profile_id'])
    ->execute();
  drupal_set_message(t("Successfully deleted the payment profile."));
  drupal_goto('user');
}

/**
 * Form constructor for the saved card checkout form.
 *
 * @param object $payment_profile
 *   The payment profile object.
 *
 * @ingroup forms
 */
function ms_core_saved_method_checkout_form($form, &$form_state, $payment_profile) {
  // Stop the caching.
  $GLOBALS['conf']['cache'] = FALSE;

  $order = ms_core_load_session_order();

  if (!$order) {
    // The order wasn't loaded: Error.
    drupal_set_message(t('There was an error with the checkout'), 'error');
    drupal_not_found();
  }

  if ($order->status != 'checkout') {
    // The order is not in checkout and shouldn't be here.
    drupal_set_message(t('This order has already been through Checkout.'), 'error');
    drupal_not_found();
  }

  global $user;
  if ($user->uid != $payment_profile->uid) {
    // This profile is invalid.
    drupal_set_message(t('Error, invalid profile.'), 'error');
    drupal_not_found();
  }

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'ms_core') . '/css/ms_core.checkout.css',
  );

  $form['ms_checkout_steps'] = array(
    '#markup' => theme('ms_core_checkout_steps', array('steps' => ms_core_get_checkout_steps($order), 'current' => 'payment')),
  );

  $form['ms_order_contents'] = array(
    '#type' => 'markup',
    '#markup' => ms_core_get_order_details_table($order),
  );

  $form['ms_payment_method'] = array(
    '#type' => 'item',
    '#title' => t("Payment Method"),
    '#markup' => t("Payment will be charged to your card: @card",
      array('@card' => $payment_profile->cc_num)),
  );

  $form['ms_oid'] = array(
    '#type' => 'value',
    '#value' => $order->oid,
  );
  $form['ms_payment_profile_id'] = array(
    '#type' => 'value',
    '#value' => $payment_profile->id,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Pay Now'),
    '#prefix' => "<div class='ms_cart_right'>",
    '#suffix' => "</div>",
  );

  return $form;
}

/**
 * Form validation handler for ms_core_saved_method_checkout_form().
 */
function ms_core_saved_method_checkout_form_validate($form, &$form_state) {
  $v = $form_state['values'];
  $order = ms_core_order_load($v['ms_oid']);
  if (!$payment_profile = ms_core_payment_profile_load($v['ms_payment_profile_id'])) {
    form_set_error('submit', t("Error: Invalid profile."));
    return;
  }

  if (function_exists($payment_profile->module . '_saved_method_checkout')) {
    if (call_user_func_array($payment_profile->module . '_saved_method_checkout', array($payment_profile, $order, $v))) {
      $form_state['redirect'] = 'ms/thankyou';
    }
  }
}

<?php

/**
 * @file
 * Contains functions pertaining to product plans.
 */

/**
 * Gets the text for a plan option on a form.
 *
 * @param MsProductsPlan $plan
 *   The product plan.
 *
 * @return string
 *   The text for the plan option.
 */
function ms_products_get_plan_option_text(MsProductsPlan $plan) {
  $display = array();

  $display[] = "<span class='ms_products_plan_name'>" . $plan->name . "</span>";

  $display[] = "<span class='ms_products_plan_price'>"
    . ms_core_get_recurring_string($plan->recurring_schedule, $plan->cart_type == 'recurring')
    . "</span>";
  if ($plan->description) {
    $display[] = "<span id='md" . $plan->sku . "' class='more_detailsDiv'>" . $plan->description . "</span>";
  }
  return implode(' - ', $display);
}

/**
 * Gets the upgrade options for a plan.
 *
 * @param MsProductsPlan $plan
 *   The plan object.
 *
 * @return array
 *   The upgrade options.
 */
function ms_products_get_modify_options(MsProductsPlan $plan) {
  $options = array(
    'upgrade' => array(),
    'downgrade' => array(),
  );
  if (isset($plan->modify_options['upgrade'])) {
    foreach (array_filter($plan->modify_options['upgrade']) as $sku => $name) {
      if ($t_plan = ms_products_plan_load($sku)) {
        $options['upgrade'][$t_plan->sku] = $t_plan;
      }
    }

    foreach (array_filter($plan->modify_options['downgrade']) as $sku => $name) {
      if ($t_plan = ms_products_plan_load($sku)) {
        $options['downgrade'][$t_plan->sku] = $t_plan;
      }
    }
  }

  if (!empty($options)) {
    return $options;
  }

  return FALSE;
}

/**
 * Returns a list of modify options for a plan.
 *
 * @param string $bundle
 *   The bundle.
 * @param string @sku
 *   (Optional) The sku of the product plan. Defaults to NULL.
 *
 * @return array
 *   An array of modification option names, keyed by sku.
 */
function ms_products_get_modify_options_plan_list($bundle, $sku = NULL) {
  $plans = ms_products_get_plans_list($bundle);
  $options = array();

  if ($sku) {
    unset($plans[$sku]);
  }

  foreach ($plans as $id => $name) {
    $plan = ms_products_plan_load($id);
    $options[$plan->sku] = $plan->name;
  }
  return $options;
}

/**
 * Checks if a product plan is free.
 *
 * @param MsProductsPlan $plan
 *   The product plan.
 *
 * @return bool
 *   TRUE if the plan is free, FALSE otherwise.
 */
function ms_products_plan_is_free(MsProductsPlan $plan) {
  if ($plan->recurring_schedule['main_amount'] == 0 && empty($plan->plan_options)) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Returns an array of Product Plans.
 *
 * @param string $bundle
 *   (optional) The bundle to get plans for. Leave blank to get all plans.
 *   Defaults to NULL.
 * @param bool $reset
 *   (optional) Whether or not to reset the cache. Defaults to FALSE.
 *
 * @return array
 *   An array of plans keyed by sku.
 */
function ms_products_get_plans($bundle = NULL, $reset = FALSE) {
  module_load_include('php', 'ms_products', 'ms_products.classes');
  ctools_include('export');
  $plans = array();
  $temp_plans = ctools_export_crud_load_all('ms_products_plans', $reset);

  if (!is_null($bundle)) {
    foreach ($temp_plans as $sku => $plan) {
      if ($plan->bundle != $bundle || !empty($plan->disabled)) {
        unset($temp_plans[$sku]);
      }
    }
  }

  // Sort the plans by weight.
  usort($temp_plans, '_ms_products_sort_weight');

  // Run the init function on the objects.
  foreach ($temp_plans as $plan) {
    $plan->initialize();
    $plans[$plan->sku] = $plan;
  }

  return $plans;
}

function _ms_products_sort_weight($a, $b) {
  return $a->weight - $b->weight;
}

/**
 * Returns a list of Product Plans.
 *
 * @param string $bundle
 *   (optional) The bundle to get plans for. Leave blank to get all plans.
 *   Defaults to NULL.
 * @param bool $reset
 *   (optional) Whether or not to reset the cache. Defaults to FALSE.
 *
 * @return array
 *   An array of plan names keyed by sku.
 */
function ms_products_get_plans_list($bundle = NULL, $reset = FALSE) {
  if (!is_null($bundle) && !is_string($bundle)) {
    $bundle = NULL;
  }
  $plans = ms_products_get_plans($bundle, $reset);
  $list = array();
  foreach ($plans as $plan) {
    $list[$plan->sku] = $plan->name;
  }
  return $list;
}

/**
 * Loads a saved product plan from the database.
 *
 * @param string $sku
 *   The sku for the plan.
 *
 * @return MsProductsPlan|false
 *   The plan object or FALSE.
 */
function ms_products_plan_load($sku) {
  ctools_include('export');
  $result = ctools_export_load_object('ms_products_plans', 'names', array($sku));

  if (isset($result[$sku]) AND $plan = $result[$sku]) {
    $plan->initialize();

    return $plan;
  }
  return FALSE;
}

/**
 * Saves a product plan.
 *
 * @param MsProductsPlan $plan
 *   The plan to save.
 */
function ms_products_plan_save(MsProductsPlan $plan) {
  return $plan->save();
}

/**
 * Deletes a product plan object from the database, and cleans up loose ends.
 *
 * @param object $plan
 *   The object to delete. This can also be the export key of the object.
 */
function ms_products_plan_delete(MsProductsPlan $plan) {
  // If we were sent the export key instead of the object, load the object.
  if (!is_object($plan)) {
    $plan = ms_products_plan_load($plan);
  }

  $bundle_info = ms_products_get_bundle($plan->bundle);

  // Also delete the purchases that are associated with the plan.
  db_delete('ms_products_purchases')
    ->condition('sku', $plan->sku)
    ->execute();

  db_delete('ms_products_plans')
    ->condition('sku', $plan->sku)
    ->execute();

  drupal_set_message(t('@plan_name %plan has been deleted',
    array('@plan_name' => $bundle_info['plan_name'], '%plan' => $plan->name)));

  // Call the hook to let other modules act on this deletion
  module_invoke_all('ms_products_plan_deleted', $plan);
}

/**
 * Calculates the Expiration Date from the Product Plan.
 *
 * @param string $sku
 *   The sku of the product plan.
 * @param int $start
 *   (Optional) The time to use as the start. Defaults to the current time.
 * @param bool $recurring
 *   (Optional) Whether or not autobill is in force. Defaults to TRUE.
 *
 * @return int
 *   The expiration date.
 */
function ms_products_get_expiration_date($sku, $start = 0, $recurring = TRUE) {
  $plan = ms_products_plan_load($sku);
  $now = ($start) ? $start : REQUEST_TIME;

  if (!$recurring) {
    $plan->recurring_schedule['total_occurrences'] = 1;
  }

  if ($plan->cart_type == 'recurring') {
    if ($plan->recurring_schedule['total_occurrences']) {
      if (!empty($plan->recurring_schedule['fixed_date'])) {
        // Calculate expiration date correctly for fixed date.
        $date = ms_core_calculate_next_fixed_date($plan->recurring_schedule['fixed_date_string'], $plan->recurring_schedule['fixed_date_type']);
      } else {
        // If the plan has a set number of occurrences, then multiply the length
        // by the occurrences to calculate expiration date.
        $date = ms_core_get_string_timestamp($plan->recurring_schedule['main_length'] * $plan->recurring_schedule['total_occurrences'], $plan->recurring_schedule['main_unit'], $now);

        // Add Time for Trial Period 1.
        if ($plan->recurring_schedule['trial_length']) {
          $date = ms_core_get_string_timestamp($plan->recurring_schedule['trial_length'], $plan->recurring_schedule['trial_unit'], $date);
        }
      }

      return $date;
    } else {
      // Otherwise, return 0 because the purchase shouldn't have an expiration
      // date yet.
      return 0;
    }
  } else {
    if ($plan->recurring_schedule['main_length']) {
      if (!empty($plan->recurring_schedule['fixed_date'])) {
        // Calculate expiration date correctly for fixed date.
        $date = ms_core_calculate_next_fixed_date($plan->recurring_schedule['fixed_date_string'], $plan->recurring_schedule['fixed_date_type']);
      } else {
        // For non recurring purchases.
        $date = ms_core_get_string_timestamp($plan->recurring_schedule['main_length'], $plan->recurring_schedule['main_unit'], $now);
      }

      return $date;
    } else {
      // If no expiration, return 0.
      return 0;
    }
  }
}

/**
 * Gets the purchase url for a plan.
 *
 * @param string $sku
 *   The sku of the product plan.
 * @param bool $checkout
 *   (Optional) Whether or not to redirect to checkout page.
 *
 * @return string
 *   The url of the purchase link.
 */
function ms_products_get_purchase_url($sku, $checkout = FALSE) {
  if ($checkout) {
    return url('ms_product/purchase/' . $sku . '/checkout');
  } else {
    return url('ms_product/purchase/' . $sku);
  }
}

/**
 * Page callback: Adds a product plan to the cart and redirects.
 *
 * @param MsProductsPlan $plan
 *   The product plan.
 * @param bool $checkout
 *   (Optional) If TRUE, redirect to checkout page. Defaults to FALSE.
 * @param array $data
 *   (Optional) An array of data to add to the product.
 * @param bool $redirect
 *   (Optional) Whether or not to redirect
 */
function ms_products_purchase_product(MsProductsPlan $plan, $checkout = FALSE, $data = array(), $redirect = TRUE) {
  if (!is_array($data)) {
    $data = array();
  }
  global $user;
  $renewing = FALSE;
  $bundle_info = ms_products_get_bundle($plan->bundle);

  if (arg(3) == 'renew') {
    $renewing = TRUE;
    // If the user is renewing, turn of the fixed date discount.
    $plan->recurring_schedule['fixed_date_discount'] = FALSE;
    $plan->primeRecurringSchedule();
    // If the user is not logged in, and they are renewing a purchase, redirect
    // them to the login screen.
    if (!$user->uid) {
      drupal_set_message(t('Please login below before renewing your purchase.'));
      drupal_goto('user/login', array('query' => drupal_get_destination()));
    }
  }
  if (arg(3) == 'checkout') {
    $checkout = TRUE;
  }

  $account = user_load($user->uid);

  // Make sure the user can purchase this plan.
  if (!ms_products_can_purchase($account, $plan) AND ms_products_user_has_product($account->uid, $plan->sku, $renewing) !== FALSE) {
    if (!$renewing AND ($id = ms_products_user_has_product($account->uid, $plan->sku, TRUE)) !== FALSE) {
      // Instead of just showing access denied, let's show alternatives. If
      // they can't purchase, see if they can renew, and vice versa.
      drupal_goto('ms_product/purchase/' . $plan->sku . '/renew/' . $id);
    } else {
      drupal_set_message(t('You are not able to purchase this @product.', array('@product' => $bundle_info['plan_name'])));
      drupal_access_denied();
    }
  }

  // Let other modules restrict access to purchasing products.
  module_invoke_all('ms_products_before_added_to_cart', $account, $plan);

  // If the user can upgrade to this plan, then do it.
  if ($account->uid AND isset($account->ms_products_purchases) AND is_array($account->ms_products_purchases)) {
    foreach ($account->ms_products_purchases as $info) {
      $purchase = ms_products_purchase_load($info['id']);
      $plan = ms_products_plan_load($purchase->sku);
      $order = ms_core_order_load($purchase->oid);

      // Get the product id of the purchase in the order.
      $product_id = NULL;
      foreach ($order->products as $product) {
        if ($product->id == $plan->sku) {
          $product_id = $product->order_product_id;
        }
      }

      $modify_options = ms_products_get_modify_options($plan);
      foreach ($modify_options['upgrade'] as $u_plan) {
        if ($u_plan->sku == $plan->sku) {
          if (!empty($plan->data['grant_credit'])) {
            if ($credit = ms_core_calculate_credit($purchase->oid, $purchase->expiration, $plan->recurring_schedule['main_amount'])) {
              $data['residual_credit'] = $credit;
              $order = ms_core_add_data($order, $data);
            }
          }
          // There is an upgrade available, do it.
          if ($product_id) {
            drupal_goto('ms/downgrade/' . $order->oid . '/' . $plan->sku . '/' . $product_id);
          } else {
            drupal_goto('ms/downgrade/' . $order->oid . '/' . $plan->sku);
          }
        }
      }
      foreach ($modify_options['downgrade'] as $u_plan) {
        if ($u_plan->sku == $plan->sku) {
          // There is a downgrade available, redirect to downgrade page.
          drupal_goto(ms_core_get_modify_url($order, $plan->sku, $product_id));
        }
      }
    }
  }

  $plan->data = array_merge($plan->data, $data);

  $account = user_load($user->uid);

  if ($plan->sku && ms_products_can_purchase($account, $plan)) {
    // If amount is 0, don't go to checkout, just grant the product.
    if ($account->uid AND ms_products_plan_is_free($plan)) {
      // Insert into Purchases database.
      $status = 'completed';
      $expiration = ms_products_get_expiration_date($plan->sku);
      $purchase = ms_products_insert_purchase(0, $account->uid, $plan->sku, $expiration, $status, 0, 1, 1);

      // Invoke hook to let other modules act on a new purchase.
      module_invoke_all('ms_products_signup', $account, $purchase, $plan);

      $payment_vars = array(
        'id' => $purchase->id,
      );

      // Send email.
      ms_products_send_mail('ms_products', 'signup', $account->mail, user_preferred_language($account), $payment_vars);
      drupal_set_message(t('The @purchase @product has been added to your account.',
        array('@purchase' => $plan->name, '@product' => $bundle_info['plan_name'])));

      // Redirect to Home Page.
      drupal_goto('');
    } else {
      // Create a new product and add it to the order.
      $product = ms_core_new_product($plan->name, $plan->bundle, $plan->sku, $plan->recurring_schedule['main_amount'], $plan->cart_type);

      // Override the title if set.
      if (!empty($data['product_title'])) {
        $product->name = $data['product_title'];
      }

      // Set the Recurring Schedule Settings.
      $product->recurring_schedule = $plan->recurring_schedule;

      // Store the Data.
      $product->data = $plan->data;

      // Add the variable for renewals.
      if ($renewing AND $purchase = ms_products_purchase_load(arg(4))) {
        // Store the Renew id data
        $product->data['renew_id'] = $purchase->id;
        $product->name .= ' - ' . t('Renewal');

        // Add a credit for users who are renewing recurring products.
        if ($credit = ms_core_calculate_credit($purchase->oid, $purchase->expiration, $plan->recurring_schedule['main_amount'])) {
          $product->data['residual_credit'] = $credit;
        }
      }

      $option_defaults = !empty($data['option_defaults']) ? $data['option_defaults'] : array();

      ms_products_add_plan_options($product, $plan, $option_defaults);

      ms_core_add_cart_product($product);

      // If option is set, go straight to checkout.
      $redirect = 'ms/cart';
      if ($checkout) {
        $redirect = 'ms/checkout';
      }

      drupal_goto($redirect);
    }
  } else {
    drupal_access_denied();
  }
}

/**
 * Adds a product plan to the cart.
 *
 * @param MsProductsPlan $plan
 *   The product plan.
 * @param array $data
 *   (Optional) An array of data to add to the product.
 * @param bool $show_message
 *   (Optional) Whether or not to show the add to cart message. Defaults to TRUE.
 */
function ms_products_add_product_to_cart(MsProductsPlan $plan, $data = array(), $show_message = TRUE, $alter = TRUE) {
  global $user;
  $renewing = FALSE;

  $account = user_load($user->uid);

  // Let other modules restrict access to purchasing products.
  if ($alter) {
    module_invoke_all('ms_products_before_added_to_cart', $account, $plan);
  }

  $plan->data = array_merge($plan->data, $data);

  $account = user_load($user->uid);

  if ($plan->sku && ms_products_can_purchase($account, $plan)) {
    // If amount is 0, don't go to checkout, just grant the product.
    if ($account->uid AND ms_products_plan_is_free($plan)) {
      // Insert into Purchases database.
      $status = 'completed';
      $expiration = ms_products_get_expiration_date($plan->sku);
      if (empty($data['renew_id']) || !$purchase = ms_products_purchase_load($data['renew_id'])) {
        $purchase = ms_products_insert_purchase(0, $account->uid, $plan->sku, $expiration, $status, 0, 1, 1);
      } else {
        ms_products_change_purchase_status($purchase->id, $status);
      }

      // Invoke hook to let other modules act on a new purchase.
      module_invoke_all('ms_products_signup', $account, $purchase, $plan);

      $payment_vars = array(
        'id' => $purchase->id,
      );

      // Send email.
      ms_products_send_mail('ms_products', 'signup', $account->mail, user_preferred_language($account), $payment_vars);
    } else {
      // Create a new product and add it to the order.
      $product = ms_core_new_product($plan->name, $plan->bundle, $plan->sku, $plan->recurring_schedule['main_amount'], $plan->cart_type);

      // Override the title if set.
      if (!empty($data['product_title'])) {
        $product->name = $data['product_title'];
      }

      // Set the Recurring Schedule Settings.
      $product->recurring_schedule = $plan->recurring_schedule;

      // Store the Data.
      $product->data = $plan->data;

      // Add the variable for renewals.
      if ($renewing AND $purchase = ms_products_purchase_load(arg(4))) {
        // Store the Renew id data
        $product->data['renew_id'] = $purchase->id;
        $product->name .= ' - ' . t('Renewal');

        // Add a credit for users who are renewing recurring products.
        if ($credit = ms_core_calculate_credit($purchase->oid, $purchase->expiration, $plan->recurring_schedule['main_amount'])) {
          $product->data['residual_credit'] = $credit;
        }
      }

      $option_defaults = !empty($data['option_defaults']) ? $data['option_defaults'] : array();

      ms_products_add_plan_options($product, $plan, $option_defaults);

      return ms_core_add_cart_product($product, FALSE, $show_message);
    }
  } else {
    drupal_set_message(t('Unable to add product to cart.'), 'error');
  }

  return FALSE;
}

/**
 * AJAX callback for the period settings checkboxes.
 *
 * Renders new options.
 */
function ms_products_plan_period_settings_callback($form, $form_state) {
  return $form['period_settings']['container'];
}

/**
 * Form constructor for the add/edit Product Plan form.
 *
 * @ingroup forms
 * @see ms_products_plan_form_validate()
 * @see ms_products_plan_form_submit()
 */
function ms_products_plan_form(&$form, &$form_state) {
  $plan = $form_state['item'];
  // If we are adding a new plan, get the product type from the url.
  if (arg(4) == 'add') {
    global $user;
    $plan = new MsProductsPlan(arg(3));
    $plan->initialize();
    $plan->export_type = $form_state['item']->export_type;
    $plan->type = $form_state['item']->type;
    $plan->uid = $user->uid;
    $form_state['item'] = $plan;
  }


  if ($form_state['form type'] == 'clone') {
    $old_plan = ms_products_plan_load($form_state['original name']);
    $plan->plan_options = $old_plan->plan_options;
  }

  $account = user_load($plan->uid);
  $bundle = $plan->bundle;
  $bundle_info = ms_products_get_bundle($bundle);

  if (arg(4) == 'add') {
    drupal_set_title(t('Add a new @plan_name', array('@plan_name' => $bundle_info['plan_name'])));
  } else if ($form_state['form type'] == 'clone') {
    drupal_set_title(t('Clone @plan_name', array('@plan_name' => $bundle_info['plan_name'])));
  } else {
    drupal_set_title(t('Edit @plan_name : @name', array('@plan_name' => $bundle_info['plan_name'], '@name' => $plan->name)));
  }

  $form['vertical_tabs'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['product'] = array(
    '#type' => 'fieldset',
    '#title' => t('@plan_name Info', array('@plan_name' => $bundle_info['plan_name'])),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#group' => 'vertical_tabs',
  );
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'vertical_tabs',
  );

  // Field Attach API
  field_attach_form('ms_products_plan', $plan, $form, $form_state);

  // Store the bundle name.
  $form['product']['bundle'] = array(
    '#type' => 'value',
    '#value' => $bundle,
  );

  $form['advanced']['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Owner'),
    '#size' => 32,
    '#maxlength' => 64,
    '#description' => t("The owner of this @plan_name. Used for tracking purposes.",
      array('@plan_name' => $bundle_info['plan_name'])),
    '#autocomplete_path' => 'admin/structure/ms_core/autocomplete',
    '#required' => TRUE,
    '#default_value' => $account->name,
  );

  $form['product']['main_amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Price'),
    '#size' => 8,
    '#maxlength' => 10,
    '#description' => t("How much the user must pay for the @purchase_name each time.
    Ex: '15.99', '27.95' etc", array('@purchase_name' => $bundle_info['purchase_name'])),
    '#default_value' => $plan->recurring_schedule['main_amount'],
    '#required' => TRUE,
  );
  $form['advanced']['shippable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Shippable'),
    '#default_value' => isset($plan->data['shippable']) ? $plan->data['shippable'] : FALSE,
    '#description' => t("If this product is shippable, then the shipping address will be collected on checkout, and the order will have a new status workflow."),
  );
  $form['advanced']['allow_multiple'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow Multiple'),
    '#default_value' => isset($plan->data['allow_multiple']) ? $plan->data['allow_multiple'] : FALSE,
    '#description' => t("Allow users to set the quantity desired when adding to
      the cart. Note that this only works for Non-Recurring products."),
  );
  $form['product']['weight'] = array(
    '#type' => 'select',
    '#title' => t('Weight'),
    '#default_value' => isset($plan->weight) ? $plan->weight : 0,
    '#options' => drupal_map_assoc(range(-100, 100)),
    '#description' => t("The weight determines the order in which plans appear in lists."),
  );

  $form['period_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Period Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'vertical_tabs',
  );
  $form['period_settings']['container'] = array(
    '#type' => 'container',
    '#attributes' => array('id' => 'ms_products_plan_period_settings'),
  );
  $form['period_settings']['container']['recurring'] = array(
    '#type' => 'checkbox',
    '#title' => t('Recurring Subscription?'),
    '#default_value' => ($plan->cart_type == 'recurring'),
    '#description' => t("Make the @purchase_name a Subscription with more than 1 Payment?",
      array('@purchase_name' => $bundle_info['purchase_name'])),
    '#ajax' => array(
      'callback' => 'ms_products_plan_period_settings_callback',
      'wrapper' => 'ms_products_plan_period_settings',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  $form['period_settings']['container']['fixed_date'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fixed Date?'),
    '#default_value' => $plan->recurring_schedule['fixed_date'],
    '#description' => t("If this is checked, you can enter an exact date string to
      determine when the plan should recur."),
    '#ajax' => array(
      'callback' => 'ms_products_plan_period_settings_callback',
      'wrapper' => 'ms_products_plan_period_settings',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  if (!empty($form_state['values']['fixed_date']) || (!isset($form_state['values']['fixed_date']) AND $plan->recurring_schedule['fixed_date'])) {
    $form['period_settings']['container']['fixed_date_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Expiration Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['period_settings']['container']['fixed_date_settings']['fixed_date_discount'] = array(
      '#type' => 'checkbox',
      '#title' => t('Prorate the Fee?'),
      '#return_value' => 1,
      '#default_value' => $plan->recurring_schedule['fixed_date_discount'],
      '#description' => t("If this is checked, the fee for the first period
        will be discounted by however much time has already passed in the current
        period. So, for example, if the user starts in the middle of the period, they will only pay
        for half the amount."),
    );
    $form['period_settings']['container']['fixed_date_settings']['fixed_date_type'] = array(
      '#type' => 'select',
      '#title' => t('Fixed Date Type'),
      '#options' => array(
        'Y' => t('Yearly'),
        'M' => t('Monthly'),
        'W' => t('Weekly'),
      ),
      '#default_value' => $plan->recurring_schedule['fixed_date_type'],
      '#description' => t("Select what type of fixed date you want.
        Yearly means the purchase will expire/renew on the same day every year.
        Monthly means the purchase will expire/renew on the same date every month.
        Weekly means the purchase will expire/renew on the same day every week."),
      '#required' => FALSE,
    );
    $form['period_settings']['container']['fixed_date_settings']['fixed_date_string'] = array(
      // @todo - Add ajax to replace the elements with the appropriate date/time elements so that users won't add wrong elements.
      // For example, if they choose Month, use a select list of dates 1-31. If they choose Week, then use select list Sunday - Saturday.
      // If they choose Yearly, we should just use a textbox...
      '#type' => 'textfield',
      '#title' => t('Expiration Date String'),
      '#description' => t("Enter a string representing when the purchase should
        expire. What you should enter depends on the type. For Yearly, enter the
        month and the date, like so: 'April 15'. For Monthly, simply entering the
        date will suffice, like so: '15'. For Weekly, enter the day of the week it
        should expire, like so 'Friday'."),
      '#default_value' => $plan->recurring_schedule['fixed_date_string'],
    );
    if (!empty($form_state['values']['recurring']) || (!isset($form_state['values']['recurring']) && $plan->cart_type == 'recurring')) {
      $form['period_settings']['container']['fixed_date_settings']['recurring_optional'] = array(
        '#type' => 'checkbox',
        '#title' => t('Make Auto-Bill Optional?'),
        '#default_value' => isset($plan->data['recurring_optional']) ? $plan->data['recurring_optional'] : FALSE,
        '#description' => t("Let users choose whether they want to be charged automatically."),
      );
      $tc_options = range(0, 90);
      $tc_options[0] = t('Recur Until Canceled');
      unset($tc_options[1]);
      $form['period_settings']['container']['fixed_date_settings']['total_occurrences'] = array(
        '#type' => 'select',
        '#title' => t('Number of Periods Until Expiration'),
        '#description' => t("How many times to bill the user when using Recurring Subscriptions."),
        '#required' => FALSE,
        '#options' => $tc_options,
        '#default_value' => $plan->recurring_schedule['total_occurrences'],
      );
    }
  }
  if ((isset($form_state['values']['fixed_date']) && empty($form_state['values']['fixed_date']) && empty($form_state['values']['recurring']))
    || (!isset($form_state['values']['fixed_date']) && $plan->cart_type == 'cart' && !$plan->recurring_schedule['fixed_date'])
  ) {
    $form['period_settings']['container']['expiration_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Expiration Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $el_options = range(0, 90);
    $el_options[0] = 'Never Expire';
    // @todo - Put these side by side. Use AJAX to make the main_unit hidden if never expire is selected.
    $form['period_settings']['container']['expiration_settings']['main_length'] = array(
      '#type' => 'select',
      '#title' => t('Length'),
      '#options' => $el_options,
      '#default_value' => $plan->recurring_schedule['main_length'],
      '#description' => t("How many units (below) before expiration.
        If set to never expire, the user will pay 1 time for a lifetime @purchase_name.",
        array('@purchase_name' => $bundle_info['purchase_name'])),
      '#required' => FALSE,
    );
    $form['period_settings']['container']['expiration_settings']['main_unit'] = array(
      '#type' => 'select',
      '#title' => t('Unit'),
      '#options' => array(
        'D' => t('Days'),
        'W' => t('Weeks'),
        'M' => t('Months'),
        'Y' => t('Years'),
      ),
      '#description' => t("What unit to use for the @purchase_name length",
        array('@purchase_name' => $bundle_info['purchase_name'])),
      '#default_value' => $plan->recurring_schedule['main_unit'],
      '#required' => FALSE,
    );
  }
  if ((!empty($form_state['values']['recurring']) && empty($form_state['values']['fixed_date']))
    || (!isset($form_state['values']['recurring']) && $plan->cart_type == 'recurring' && !$plan->recurring_schedule['fixed_date'])
  ) {
    $form['period_settings']['container']['recurring_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Recurring Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['period_settings']['container']['recurring_settings']['recurring_optional'] = array(
      '#type' => 'checkbox',
      '#title' => t('Make Auto-Bill Optional?'),
      '#default_value' => isset($plan->data['recurring_optional']) ? $plan->data['recurring_optional'] : FALSE,
      '#description' => t("Let users choose whether they want to be charged automatically."),
    );
    $tc_options = range(0, 90);
    $tc_options[0] = t('Recur Until Canceled');
    unset($tc_options[1]);
    $form['period_settings']['container']['recurring_settings']['total_occurrences'] = array(
      '#type' => 'select',
      '#title' => t('Number of Periods Until Expiration'),
      '#description' => t("How many times to bill the user when using Recurring Subscriptions."),
      '#required' => FALSE,
      '#options' => $tc_options,
      '#default_value' => $plan->recurring_schedule['total_occurrences'],
    );
    $ml_options = range(0, 90);
    unset($ml_options[0]);
    $form['period_settings']['container']['recurring_settings']['main_length'] = array(
      '#type' => 'select',
      '#title' => t('Period Length'),
      '#options' => $ml_options,
      '#description' => t("How many units (below) for each period."),
      '#default_value' => $plan->recurring_schedule['main_length'],
      '#required' => FALSE,
    );
    $form['period_settings']['container']['recurring_settings']['main_unit'] = array(
      '#type' => 'select',
      '#title' => t('Period Unit'),
      '#options' => array(
        'D' => t('Days'),
        'W' => t('Weeks'),
        'M' => t('Months'),
        'Y' => t('Years'),
      ),
      '#description' => t("What unit to use for the periods"),
      '#default_value' => $plan->recurring_schedule['main_unit'],
      '#required' => FALSE,
    );

    $form['period_settings']['container']['recurring_settings']['trial'] = array(
      '#type' => 'fieldset',
      '#title' => t('Trial Period Settings (Optional)'),
      '#collapsible' => TRUE,
      '#weight' => 50,
      '#collapsed' => ($plan->recurring_schedule['has_trial']) ? FALSE : TRUE,
    );
    $tl_options = range(0, 90);
    $tl_options[0] = 'No Trial Period';
    $form['period_settings']['container']['recurring_settings']['trial']['trial_length'] = array(
      '#type' => 'select',
      '#title' => t('Trial Period Length'),
      '#options' => $tl_options,
      '#description' => t("The number of units for the Trial Period.
        If No Trial Period is set, the Trial Period will not be used."),
      '#required' => FALSE,
      '#default_value' => $plan->recurring_schedule['trial_length'],
    );
    $form['period_settings']['container']['recurring_settings']['trial']['trial_unit'] = array(
      '#type' => 'select',
      '#title' => t('Trial Period Unit'),
      '#options' => array(
        'D' => t('Days'),
        'W' => t('Weeks'),
        'M' => t('Months'),
        'Y' => t('Years'),
      ),
      '#description' => t("The unit for the Trial Period."),
      '#default_value' => $plan->recurring_schedule['trial_unit'],
      '#required' => FALSE,
    );
    $form['period_settings']['container']['recurring_settings']['trial']['trial_amount'] = array(
      '#type' => 'textfield',
      '#title' => t('Trial Amount'),
      '#size' => 8,
      '#maxlength' => 10,
      '#description' => t("The Amount Charged for the Trial Period. Ex: '5.99', '7.95' etc. Put 0 for a free trial."),
      '#required' => FALSE,
      '#default_value' => $plan->recurring_schedule['trial_amount'],
    );
  }

  $form['modify'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upgrade & Downgrade Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'vertical_tabs',
    '#description' => '<p>' . t("Users can always upgrade from a non-recurring @plan to a recurring @plan.",
        array('@purchase' => $bundle_info['purchase_name'], '@plan' => $bundle_info['plan_name'])) . '</p>'
      . '<p>' . t("Users can upgrade or downgrade from one recurring @plan to another if the payment gateway support it.",
        array('@purchase' => $bundle_info['purchase_name'], '@plan' => $bundle_info['plan_name'])) . '</p>'
      . '<p>' . t("Some of the payment gateways which fully support upgrades and downgrades are Stripe, Authorize.net CIM, and Realex.") . '</p>',
  );
  $form['modify']['grant_credit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Apply credit when upgrading'),
    '#default_value' => isset($plan->data['grant_credit']) ? $plan->data['grant_credit'] : TRUE,
    '#description' => t("If this is active, then users will have any credit remaining in their purchase applied to the cost of upgrading."),
  );
  $form['modify']['modify_existing_term'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keep the existing term expiration date when upgrading/downgrading'),
    '#default_value' => isset($plan->data['modify_existing_term']) ? $plan->data['modify_existing_term'] : FALSE,
    '#description' => t("If this is active, then upgrading/downgrading to plans with the same main period will not change the expiration date of the purchase."),
  );
  $form['modify']['upgrade_options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Upgrade Options'),
    '#description' => '<p>' . t("Select which @plan_plural will be offered as an upgrade option from this @plan.",
        array('@plan_plural' => $bundle_info['plan_name_plural'], '@plan' => $bundle_info['plan_name'])) . '</p>'
      . '<p>' . t('When a user upgrades, they are charged the difference between their current @purchase and the new @plan, and their @plan is changed immediately.',
        array('@purchase' => $bundle_info['purchase_name'], '@plan' => $bundle_info['plan_name'])) . '</p>'
      . '<p>' . t("Some payment gateways don't support this very well. For example, PayPal WPS can't charge users when they upgrade, so it will grant access to the new @plan immediately and change the billing amount at the next scheduled payment time. Still other payment gateways don't support changing active subscriptions at all, so users will need to cancel their current @purchase and purchase a new one.",
        array('@purchase' => $bundle_info['purchase_name'], '@plan' => $bundle_info['plan_name'])) . '</p>'
      . '<p>' . t("Still other payment gateways don't support changing active subscriptions at all, so users will need to cancel their current @purchase and purchase a new one.",
        array('@purchase' => $bundle_info['purchase_name'], '@plan' => $bundle_info['plan_name'])) . '</p>',
    '#default_value' => $plan->modify_options['upgrade'],
    '#options' => ms_products_get_modify_options_plan_list($bundle, $plan->sku),
    '#empty' => t("You need to add more plans before you can set the upgrade options."),
  );
  $form['modify']['downgrade_options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Downgrade Options'),
    '#description' => '<p>' . t("Select which @plan_plural will be offered as a downgrade option from this @plan.",
        array('@plan_plural' => $bundle_info['plan_name_plural'], '@plan' => $bundle_info['plan_name'])) . '</p>'
      . '<p>' . t("When a user downgrades, their @purchase is put into a 'downgraded' state until the end of the term, at which point the @purchase will change to the new @plan automatically.",
        array('@purchase' => $bundle_info['purchase_name'], '@plan' => $bundle_info['plan_name'])) . '</p>'
      . '<p>' . t("Most payment gateways don't support downgrades at all, so users will need to cancel their current @purchase and purchase a new one.",
        array('@purchase' => $bundle_info['purchase_name'], '@plan' => $bundle_info['plan_name'])) . '</p>',
    '#default_value' => $plan->modify_options['downgrade'],
    '#options' => ms_products_get_modify_options_plan_list($bundle, $plan->sku),
    '#empty' => t("You need to add more plans before you can set the downgrade options."),
  );
  $form['advanced']['expire_when'] = array(
    '#type' => 'select',
    '#title' => t('When to Expire the @purchase?', array('@purchase' => $bundle_info['purchase_name'])),
    '#description' => t("When, if ever, should the @purchase be expired.", array('@purchase' => $bundle_info['purchase_name'])),
    '#default_value' => $plan->expire_when,
    '#options' => array(
      '' => t('Never'),
      'subscr_eot' => t('End of Term'),
      'subscr_cancel' => t('Upon Cancellation'),
    ),
  );
  $roles2 = (user_roles(FALSE));
  $form['advanced']['allow_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Allow Roles'),
    '#description' => t("Which roles will be allowed to purchase or upgrade to this plan? WARNING: This is an advanced setting for a reason, so only use it if you know you need it. If you select any roles and do not select the 'anonymous user' role, then this plan WILL NOT be available for guest users (because anonymous users won't have access to purchase it)."),
    '#default_value' => $plan->allow_roles,
    '#options' => $roles2,
  );
  $form['advanced']['deny_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Deny Roles'),
    '#description' => t("Which roles will be denied from purchasing this plan? Users with these roles will not be shown the plan or have access to purchase it."),
    '#default_value' => $plan->deny_roles,
    '#options' => $roles2,
  );

  $form['email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'vertical_tabs',
  );
  $token_types = array('ms_products_purchase', 'ms_products_plan', 'ms_core_order', 'ms_core_payment', 'user');

  $form['email']['signup_mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Purchase Email Subject'),
    '#required' => FALSE,
    '#default_value' => $plan->signup_mail_subject,
  );
  $form['email']['signup_mail_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Purchase Email Body'),
    '#description' => t("Leave blank to not send an email."),
    '#required' => FALSE,
    '#default_value' => $plan->signup_mail_body,
  );
  $form['email']['signup_mail_token_help']['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => $token_types,
    '#dialog' => TRUE,
  );
  // @todo - Only show if it has an expiration.
  $form['email']['expiring_mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Expiring Soon Email Subject'),
    '#required' => FALSE,
    '#default_value' => $plan->expiring_mail_subject,
  );
  $form['email']['expiring_mail_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Expiring Soon Email Body'),
    '#description' => t("Leave blank to not send an email."),
    '#required' => FALSE,
    '#default_value' => $plan->expiring_mail_body,
  );
  $days_array = range(0, 90);
  $form['email']['expiring_mail_days'] = array(
    '#type' => 'select',
    '#title' => t('When to Send Expiring Soon Email?'),
    '#description' => t("How many days before the @purchase_name expires should the Expiring Soon
    email be sent? Setting this to 0 will disable it.", array('@purchase_name' => $bundle_info['purchase_name'])),
    '#options' => $days_array,
    '#required' => FALSE,
    '#default_value' => $plan->expiring_mail_days,
  );
  $form['email']['expiring_mail_token_help']['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => $token_types,
    '#dialog' => TRUE,
  );
  $form['email']['eot_mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Expiration Email Subject'),
    '#required' => FALSE,
    '#default_value' => $plan->eot_mail_subject,
  );
  $form['email']['eot_mail_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Expiration Email Body'),
    '#description' => t("Leave blank to not send an email."),
    '#required' => FALSE,
    '#default_value' => $plan->eot_mail_body,
  );
  $form['email']['eot_mail_token_help']['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => $token_types,
    '#dialog' => TRUE,
  );
  // @todo - Only show if it is recurring.
  $form['email']['cancel_mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Cancellation Email Subject'),
    '#default_value' => $plan->cancel_mail_subject,
    '#required' => FALSE,
  );
  $form['email']['cancel_mail_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Cancellation Email Body'),
    '#default_value' => $plan->cancel_mail_body,
    '#description' => t("Leave blank to not send an email."),
    '#required' => FALSE,
  );
  $form['email']['cancel_mail_token_help']['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => $token_types,
    '#dialog' => TRUE,
  );
  $form['email']['modify_mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('@purchase_name Extension/Modification Email Subject',
      array('@purchase_name' => $bundle_info['purchase_name'])),
    '#default_value' => $plan->modify_mail_subject,
    '#required' => FALSE,
  );
  $form['email']['modify_mail_body'] = array(
    '#type' => 'textarea',
    '#title' => t('@purchase_name Extension/Modification Email Body',
      array('@purchase_name' => $bundle_info['purchase_name'])),
    '#default_value' => $plan->modify_mail_body,
    '#description' => t("This email will be sent when a user extends or modifies their @purchase_name.
    Recurring @purchase_plural can only be modified, and Non-Recurring @purchase_plural can only be extended.
    Leave blank to not send an email.",
      array(
        '@purchase_plural' => $bundle_info['purchase_name_plural'],
        '@purchase_name' => $bundle_info['purchase_name'],
      )),
    '#required' => FALSE,
  );
  $form['email']['modify_mail_token_help']['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => $token_types,
    '#dialog' => TRUE,
  );

  // Show the extra email fields.
  foreach ($bundle_info['emails'] as $email_name => $email_info) {
    if (!empty($email_info['extra']) AND !empty($plan->data['emails'][$email_name])) {
      $form['email'][$email_name . '_mail_subject'] = array(
        '#type' => 'textfield',
        '#title' => check_plain($email_info['subject_title']),
        '#default_value' => $plan->data['emails'][$email_name]['subject'],
        '#required' => FALSE,
      );
      $form['email'][$email_name . '_mail_body'] = array(
        '#type' => 'textarea',
        '#title' => check_plain($email_info['body_title']),
        '#default_value' => $plan->data['emails'][$email_name]['body'],
        '#description' => check_plain($email_info['body_description']),
        '#required' => FALSE,
      );
      $form['email'][$email_name . '_mail_token_help']['tokens'] = array(
        '#theme' => 'token_tree',
        '#token_types' => $token_types,
        '#dialog' => TRUE,
      );
    }
  }

  // Plan options.
  $form['plan_options'] = array(
    '#type' => 'container',
    '#prefix' => "<div id='ms_products_plan_options_wrapper'>",
    '#suffix' => "</div>",
    '#tree' => TRUE,
  );
  $form['plan_options']['change_plan_options'] = array(
    '#type' => 'checkbox',
    '#title' => t('Let users modify their @purchase_name options', array('@purchase_name' => $bundle_info['purchase_name'])),
    '#default_value' => isset($plan->data['change_plan_options']) ? $plan->data['change_plan_options'] : TRUE,
    '#description' => t("If set, users will be shown a link to 'Change Options' which, when clicked, will go to a page where they can select new options for their active @purchase_name. If they select more options, they will be charged the difference and given the new options immediately. If they select less options, the options will be removed at the end of the current billing period.",
      array('@purchase_name' => $bundle_info['purchase_name'])),
  );

  // Add a default plan option if the button was clicked.
  if (!empty($form_state['triggering_element']['#name'])) {
    switch ($form_state['triggering_element']['#name']) {
      case 'add_default_plan_option':
        $option_name = $form_state['triggering_element']['#option_name'];

        if (!empty($bundle_info['plan_options'][$option_name])) {
          $option_info = (object)$bundle_info['plan_options'][$option_name];
          $option_info->sku = $plan->sku;
          $option_info->name = $option_name;
          $plan->plan_options[$option_name] = $option_info;
          $form['plan_options']['#collapsed'] = FALSE;
        }
        break;

      case 'delete_plan_option':
        $option_name = $form_state['triggering_element']['#option_name'];
        unset($plan->plan_options[$option_name]);
        $form['plan_options']['#collapsed'] = FALSE;
        break;

      case 'add_custom_plan_option':
        $option_name = $form_state['values']['plan_options']['custom']['custom_option_name'];
        $option_info = (object)array(
          'title' => $form_state['values']['plan_options']['custom']['custom_option_title'],
          'description' => '',
          'optional' => TRUE,
          'widget' => 'checkbox',
        );
        $option_info->sku = $plan->sku;
        $option_info->name = $option_name;
        $plan->plan_options[$option_name] = $option_info;
        $form['plan_options']['#collapsed'] = FALSE;
        break;
    }
  }

  foreach ($bundle_info['plan_options'] as $option_name => $option_info) {
    if (!isset($plan->plan_options[$option_name])) {
      // Show a button that will add the default option.
      $form['plan_options'][$option_name]['add'] = array(
        '#type' => 'button',
        '#name' => 'add_default_plan_option',
        '#option_name' => $option_name,
        '#value' => t('Add @option_name option', array('@option_name' => $option_name)),
        '#ajax' => array(
          'callback' => 'ms_products_add_plan_option_callback',
          'wrapper' => 'ms_products_plan_options_wrapper',
          'method' => 'replace',
          'effect' => 'fade',
        ),
      );
    }
  }

  // Add a button to add a new option via ajax.
  $form['plan_options']['custom']['custom_option_title'] = array(
    '#type' => 'textfield',
    '#title' => t('New option'),
  );
  $form['plan_options']['custom']['custom_option_name'] = array(
    '#type' => 'machine_name',
    '#machine_name' => array(
      'exists' => 'ms_products_plan_option_exists',
      'source' => array('plan_options_container', 'plan_options', 'custom', 'custom_option_title'),
      'replace_pattern' => '[^a-z0-9-]+',
      'replace' => '-',
    ),
    '#required' => FALSE,
  );
  unset(
  $form_state['input']['plan_options']['custom']['custom_option_name'],
  $form_state['input']['plan_options']['custom']['custom_option_title']
  );
  $form['plan_options']['custom']['add'] = array(
    '#type' => 'button',
    '#name' => 'add_custom_plan_option',
    '#value' => t('Add'),
    '#ajax' => array(
      'callback' => 'ms_products_add_plan_option_callback',
      'wrapper' => 'ms_products_plan_options_wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  foreach ($plan->plan_options as $option_name => $option_info) {
    $form['plan_options']['#collapsed'] = !(isset($form_state['triggering_element']['#parents'][0]) && $form_state['triggering_element']['#parents'][0] == 'plan_options');
    $form['plan_options'][$option_name] = array(
      '#type' => 'fieldset',
      '#title' => filter_xss($option_name),
      '#collapsible' => TRUE,
      '#collapsed' => !(isset($form_state['triggering_element']['#parents'][1]) && $form_state['triggering_element']['#parents'][0] == 'plan_options' && $form_state['triggering_element']['#parents'][1] == $option_name),
      '#tree' => TRUE,
    );
    $form['plan_options'][$option_name]['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#description' => t('The display label of the option.'),
      '#default_value' => $option_info->title,
    );
    $form['plan_options'][$option_name]['description'] = array(
      '#type' => 'textfield',
      '#title' => t('Description'),
      '#description' => t('The display description of the option.'),
      '#default_value' => $option_info->description,
    );
    $form['plan_options'][$option_name]['amount'] = array(
      '#type' => 'textfield',
      '#title' => t('Fee'),
      '#description' => t('The cost for adding this option.'),
      '#default_value' => $option_info->amount,
    );
    $form['plan_options'][$option_name]['expiration'] = array(
      '#type' => 'textfield',
      '#title' => t('Expiration'),
      '#description' => t('How long the option will remain active for. You may enter any valid strtotime() string. Leave blank for no expiration. Example: 5 days'),
      '#default_value' => $option_info->expiration ? format_interval($option_info->expiration) : '',
    );
    $form['plan_options'][$option_name]['widget'] = array(
      '#type' => 'radios',
      '#title' => t('Widget type'),
      '#options' => array(
        'checkbox' => t('Checkbox'),
        'radios' => t('Radios'),
        'select' => t('Select'),
      ),
      '#default_value' => isset($option_info->widget) ? $option_info->widget : 'checkbox',
      '#ajax' => array(
        'callback' => 'ms_products_add_plan_option_callback',
        'wrapper' => 'ms_products_plan_options_wrapper',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );
    // If the product is not optional, hide default_value and make it be on by default.
    if ((!empty($form_state['values']['plan_options'][$option_name]['widget']) &&
        ($form_state['values']['plan_options'][$option_name]['widget'] == 'radios' || $form_state['values']['plan_options'][$option_name]['widget'] == 'select')) ||
      (!isset($form_state['values']['plan_options'][$option_name]['widget']) && ($option_info->widget == 'radios' || $option_info->widget == 'select'))
    ) {
      $form['plan_options'][$option_name]['sub_options'] = array(
        '#type' => 'textarea',
        '#title' => t('Options'),
        '#description' => t('Enter options available, one per line, in the format key_name|Value Name|extra_fee(Optional). Example: xl|Extra Large|5.00'),
        '#default_value' => $option_info->sub_options,
      );
      $form['plan_options'][$option_name]['default_value'] = array(
        '#type' => 'value',
        '#value' => TRUE,
      );
      $form['plan_options'][$option_name]['optional'] = array(
        '#type' => 'value',
        '#value' => FALSE,
      );
    } else {
      $form['plan_options'][$option_name]['sub_options'] = array(
        '#type' => 'value',
        '#value' => '',
      );
      $form['plan_options'][$option_name]['optional'] = array(
        '#type' => 'checkbox',
        '#title' => t('Optional?'),
        '#description' => t('Whether or not the option can be removed by the user.'),
        '#default_value' => $option_info->optional,
        '#ajax' => array(
          'callback' => 'ms_products_add_plan_option_callback',
          'wrapper' => 'ms_products_plan_options_wrapper',
          'method' => 'replace',
          'effect' => 'fade',
        ),
      );
      // If the product is not optional, hide default_value and make it be on by default.
      if (!empty($form_state['values']['plan_options'][$option_name]['optional']) ||
        (!isset($form_state['values']['plan_options'][$option_name]['optional']) && $option_info->optional)
      ) {
        $form['plan_options'][$option_name]['default_value'] = array(
          '#type' => 'checkbox',
          '#title' => t('On by default?'),
          '#description' => t('Whether or not the option is on by default.'),
          '#default_value' => $option_info->default_value,
        );
      } else {
        $form['plan_options'][$option_name]['default_value'] = array(
          '#type' => 'value',
          '#value' => TRUE,
        );
      }
    }

    // Show a delete button to remove the option.
    $form['plan_options'][$option_name]['remove'] = array(
      '#type' => 'button',
      '#value' => t('Delete option'),
      '#name' => 'delete_plan_option',
      '#option_name' => $option_name,
      '#ajax' => array(
        'callback' => 'ms_products_add_plan_option_callback',
        'wrapper' => 'ms_products_plan_options_wrapper',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );
  }

  $form['plan_options_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Plan options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'vertical_tabs',
  );
  $form['plan_options_container']['plan_options'] = $form['plan_options'];
  unset($form['plan_options']);

  foreach ($bundle_info['custom_elements'] as $key => $value) {
    $form[$key] = $value;
  }

  foreach ($bundle_info['plan_fields'] as $key => $value) {
    if (isset($plan->$key)) {
      $value['#default_value'] = $plan->$key;
    } else {
      $value['#default_value'] = $plan->data[$key];
    }

    $parents = !empty($value['#array_parents']) ? $value['#array_parents'] : array();
    $parents[] = $key;
    drupal_array_set_nested_value($form, $parents, $value);
  }

  // Add the override settings.
  ms_core_get_override_settings_form_elements($form, !empty($plan->sku) ? $plan : NULL);

  return $form;
}

/**
 * Checks if an option name is unique.
 *
 * @param string $option_name
 *   The option name.
 *
 * @return bool
 *   TRUE if the option exists, FALSE otherwise.
 */
function ms_products_plan_option_exists($option_name) {
  // Currently there is no way to do this, since the option names are only
  // required to be unique on a per-plan basis, and the plan isn't passed in as
  // an argument.
  return FALSE;
}

/**
 * Callback for adding a plan option.
 */
function ms_products_add_plan_option_callback($form, $form_state) {
  return $form['plan_options_container']['plan_options'];
}

/**
 * Form validation handler for ms_products_plan_form().
 *
 * @see ms_products_plan_form_submit()
 */
function ms_products_plan_form_validate($form, &$form_state) {
  $v = $form_state['values'];
  // Validate Trial Fee.
  if (isset($v['trial_amount']) && !is_numeric($v['trial_amount'])) {
    form_set_error('trial_amount', t('You must use a number for the Trial Amount.'));
  } elseif (isset($v['trial_amount']) && $v['trial_amount'] < 0) {
    form_set_error('trial_amount', t('The Trial Amount must be positive.'));
  }
  // Validate Regular Fee.
  if ($v['main_amount'] && !is_numeric($v['main_amount'])) {
    form_set_error('main_amount', t('You must use a number for the Price.'));
  } elseif ($v['main_amount'] && $v['main_amount'] < 0) {
    form_set_error('main_amount', t('The Price must be positive.'));
  }
  if (($v['main_amount'] == 0 OR $v['main_amount'] == 0.00) AND $v['recurring']) {
    form_set_error('recurring', t('Free Products cannot be recurring'));
  }
  // Validate that the SKU is unique and valid.
  if ($plan = ms_products_plan_load($v['sku'])) {
    if (!isset($v['sku']) OR $plan->sku != $v['sku']) {
      form_set_error('sku', t('The SKU is already being used by a different product.
        Please try a different SKU.'));
    }
  }

  // Let modules add their own validation.
  module_invoke_all('ms_products_api_plan_form_validate', $form_state, $form_state['values']['bundle']);

  $plan = $form_state['item'];

  field_attach_form_validate('ms_products_plan', $plan, $form, $form_state);
}

/**
 * Form submission handler for ms_products_plan_form().
 *
 * @see ms_products_plan_form_validate()
 */
function ms_products_plan_form_submit($form, &$form_state) {
  $form_values = (object)$form_state['values'];
  $plan = $form_state['item'];
  $account = user_load_by_name($form_values->username);

  // Set the values.
  $plan->name = $form_values->name;
  $plan->bundle = $form_values->bundle;
  $plan->uid = ($account) ? $account->uid : 0;
  $plan->sku = $form_values->sku;
  $plan->description = $form_values->description;

  $plan->recurring_schedule = array(
    'total_occurrences' => isset($form_values->total_occurrences) ? $form_values->total_occurrences : 1,
    'main_amount' => isset($form_values->main_amount) ? $form_values->main_amount : 0,
    'main_length' => isset($form_values->main_length) ? $form_values->main_length : '',
    'main_unit' => isset($form_values->main_unit) ? $form_values->main_unit : '',
    'has_trial' => !empty($form_values->trial_length) ? TRUE : FALSE,
    'trial_amount' => isset($form_values->trial_amount) ? $form_values->trial_amount : 0,
    'trial_length' => isset($form_values->trial_length) ? $form_values->trial_length : 0,
    'trial_unit' => isset($form_values->trial_unit) ? $form_values->trial_unit : '',
    'fixed_date' => isset($form_values->fixed_date) ? $form_values->fixed_date : 0,
    'fixed_date_discount' => isset($form_values->fixed_date_discount) ? $form_values->fixed_date_discount : 1,
    'fixed_date_type' => isset($form_values->fixed_date_type) ? $form_values->fixed_date_type : 'M',
    'fixed_date_string' => isset($form_values->fixed_date_string) ? $form_values->fixed_date_string : '',
  );

  $plan->cart_type = ($form_values->recurring) ? 'recurring' : 'cart';
  $plan->expiring_mail_days = $form_values->expiring_mail_days;
  if (!isset($plan->weight)) {
    $plan->weight = 0;
  }

  $plan->modify_options = array(
    'upgrade' => $form_values->upgrade_options,
    'downgrade' => $form_values->downgrade_options,
  );

  // Encode the Emails.
  $plan->signup_mail_subject = htmlspecialchars($form_values->signup_mail_subject);
  $plan->signup_mail_body = htmlspecialchars($form_values->signup_mail_body);
  $plan->expiring_mail_subject = htmlspecialchars($form_values->expiring_mail_subject);
  $plan->expiring_mail_body = htmlspecialchars($form_values->expiring_mail_body);
  $plan->eot_mail_subject = htmlspecialchars($form_values->eot_mail_subject);
  $plan->eot_mail_body = htmlspecialchars($form_values->eot_mail_body);
  $plan->cancel_mail_subject = htmlspecialchars($form_values->cancel_mail_subject);
  $plan->cancel_mail_body = htmlspecialchars($form_values->cancel_mail_body);
  $plan->modify_mail_subject = htmlspecialchars($form_values->modify_mail_subject);
  $plan->modify_mail_body = htmlspecialchars($form_values->modify_mail_body);

  if (empty($form_values->override_settings)) {
    $form_values->override_settings = array();
  }
  $plan->data['override_settings'] = ms_core_get_overridden_settings($form_values->override_settings);
  $plan->data['allow_multiple'] = $form_values->allow_multiple;
  $plan->data['shippable'] = $form_values->shippable;
  $plan->data['recurring_optional'] = isset($form_values->recurring_optional) ? $form_values->recurring_optional : FALSE;
  $plan->data['allow_roles'] = $form_values->allow_roles;
  $plan->data['deny_roles'] = $form_values->deny_roles;
  $plan->data['grant_credit'] = $form_values->grant_credit;
  $plan->data['change_plan_options'] = $form_values->plan_options['change_plan_options'];
  $plan->data['modify_existing_term'] = $form_values->modify_existing_term;

  $bundle_info = ms_products_get_bundle($plan->bundle);

  // Save all custom form values into the data array.
  foreach ($bundle_info['plan_fields'] as $custom_field => $custom_field_info) {
    $plan->data[$custom_field] = isset($form_values->$custom_field) ? $form_values->$custom_field : NULL;
  }

  // Save the plan options.
  if (!empty($form_values->plan_options)) {
    foreach ($form_values->plan_options as $option_name => $option_info) {
      if (isset($option_info['title'])) {
        if (!is_numeric($option_info['amount'])) {
          $option_info['amount'] = 0;
        }
        $plan->plan_options[$option_name] = (object)$option_info;
        $plan->plan_options[$option_name]->sku = $plan->sku;
        $plan->plan_options[$option_name]->expiration = strtotime($option_info['expiration'], 0);
        $plan->plan_options[$option_name]->name = $option_name;
      }
    }
  }

  // Save the extra email fields into the data element.
  foreach ($bundle_info['emails'] as $email_name => $email_info) {
    if (!empty($email_info['extra'])) {
      $plan->data['emails'][$email_name]['subject'] = $form_values->{$email_name . '_mail_subject'};
      $plan->data['emails'][$email_name]['body'] = $form_values->{$email_name . '_mail_body'};
    }
  }

  // Call a hook to let other modules save data or add data.
  drupal_alter('ms_products_plan_update', $plan, $form_state, $plan->bundle);

  drupal_set_message(t('@plan_name Saved: %sname.',
    array('@plan_name' => $bundle_info['plan_name'], '%sname' => $plan->name)));

  $form_state['saved_form'] = $form;

  $form_state['item'] = $plan;
}

/**
 * Page callback for the select plan page.
 *
 * @param string $bundle
 *   The bundle to show plans from.
 *
 * @see ms_products_menu()
 */
function ms_products_purchase_plan_page($bundle) {
  $plans = ms_products_get_plans_list($bundle);
  if (!empty($plans)) {
    return drupal_get_form('ms_products_user_purchase_options_form', $plans, $bundle, arg(3));
  } else {
    return t('There are no plans available.');
  }
}

/**
 * Tests if a user has access to purchase a product.
 *
 * @param object $account
 *   The user object.
 * @param MsProductsPlan $plan
 *   The product plan object.
 *
 * @return bool
 *   TRUE if the user can purchase the plan, FALSE otherwise.
 */
function ms_products_can_purchase($account, MsProductsPlan $plan) {
  $user_roles = array_flip($account->roles);

  $access = TRUE;

  // Check user's roles are allowed on this plan.
  $allowed_roles = array_filter($plan->allow_roles);

  // Only show for allowed roles.
  if (!empty($allowed_roles)) {
    $has_roles = array_intersect($user_roles, $allowed_roles);
    // As long as there is at least one item in the intersected array we are
    // still good.
    if (empty($has_roles)) {
      $access = FALSE;
    }
  }

  // Check user's roles are not denied on this plan.
  $denied_roles = $plan->deny_roles;
  if (!empty($denied_roles) && is_array($denied_roles)) {
    // Filter out empty values.
    $denied_roles = array_filter($denied_roles);
    if (!empty($denied_roles)) {
      $has_roles = array_intersect($user_roles, $denied_roles);
      // If there is a role in the intersect we should not continue.
      if (!empty($has_roles)) {
        $access = FALSE;
      }
    }
  }

  // Add an access callback to let other modules have a say in this.
  drupal_alter('ms_products_purchase_plan_access', $access, $account, $plan);

  return $access;
}

/**
 * Adds optional adjustments to the product based on the plugin settings.
 *
 * @param MsProduct $product
 *   The product object, passed by reference, to which the adjustments are
 *   added.
 * @param MsProductsPlan $plan
 *   The plan object.
 * @param array $defaults
 *   An array of default values for the options.
 */
function ms_products_add_plan_options(MsProduct &$product, MsProductsPlan $plan, array $defaults) {
  ms_products_add_product_options($product, $plan->plan_options, $defaults);
}

/**
 * Adds optional adjustments to the product based on the plugin settings.
 *
 * @param MsProduct $product
 *   The product object, passed by reference, to which the adjustments are
 *   added.
 * @param array $product_options
 *   An array of options.
 * @param array $defaults
 *   (Optional) An array of default values for the options.
 */
function ms_products_add_product_options(MsProduct &$product, $product_options, array $defaults = NULL) {
  if ($defaults == NULL) {
    $defaults = array();
  }
  foreach ($product_options as $option_name => $option_info) {
    $product->data['adjustments'][$option_name] = array(
      'value' => $option_info->amount,
      'display' => $option_info->title,
      'active' => !empty($defaults[$option_name]),
      'optional' => $option_info->optional,
      'option' => !empty($defaults[$option_name]) ? $defaults[$option_name] : '',
      'widget' => $option_info->widget,
    );

    if ($option_info->widget == 'radios' || $option_info->widget == 'select') {
      $sub_options = ms_core_map_select_options($option_info->sub_options, array('id', 'name', 'price'));
      $product->data['adjustments'][$option_name]['options'] = $sub_options;
      if (!empty($defaults[$option_name])) {
        $product->data['adjustments'][$option_name]['display'] .= ": " . $sub_options[$defaults[$option_name]]['name'];
        if (!empty($sub_options[$defaults[$option_name]]['price'])) {
          $product->data['adjustments'][$option_name]['value'] += $sub_options[$defaults[$option_name]]['price'];
        }
      }
    }
  }
}

/**
 * Builds the plan selector form element.
 *
 * This has built in support for AJAX options.
 *
 * @param array $options
 *   The options to choose from.
 *
 * @return array
 *   The element.
 */
function ms_products_plans_element($form_state, $title, $description, $options, $id, $required = FALSE, $default = NULL, $default_options = array()) {
  $element = array(
    '#type' => 'fieldset',
    '#prefix' => "<div class='ms_products_plan_field_wrapper' id='ms_products_plan_field_" . $id . "'>",
    '#suffix' => "</div>",
    '#tree' => TRUE,
  );
  if (is_null($default) && !empty($form_state['values'][$id]['sku'])) {
    $default = $form_state['values'][$id]['sku'];
  }
  if (is_null($default) && !empty($_REQUEST['plan'])) {
    $default = $_REQUEST['plan'];
  }
  $element['sku'] = array(
    '#type' => 'radios',
    '#title' => $title,
    '#options' => $options,
    '#required' => $required,
    '#default_value' => $default,
    '#description' => $description,
  );
  foreach ($options as $sku => $name) {
    $plan = ms_products_plan_load($sku);
    if (!empty($plan->plan_options)) {
      $element['sku']['#ajax'] = array(
        'callback' => 'ms_products_plan_field_choice',
        'wrapper' => 'ms_products_plan_field_' . $id,
        'method' => 'replace',
        'effect' => 'fade',
      );
      break;
    }
  }

  if (!empty($default) && ($selected_plan = ms_products_plan_load($default))) {
    // @todo - Add JS that will move the plan options underneath the selecting element.
    /*
    $element['#attached']['js']['alert("Hello!");'] = array(
      'type' => 'inline'
    );
    */
    if (!empty($selected_plan->plan_options)) {
      $element['options'] = array(
        '#prefix' => "<div class='ms_products_options_wrapper'>",
        '#suffix' => "</div>",
      );
      foreach ($selected_plan->plan_options as $option_name => $option_info) {
        switch ($option_info->widget) {
          case 'checkbox':
            $element['options'][$option_name] = array(
              '#type' => $option_info->widget,
              '#default_value' => !empty($default_options[$option_name]) ? $default_options[$option_name] : $option_info->default_value,
              '#disabled' => empty($option_info->optional),
              '#title' => $option_info->amount > 0 ? $option_info->title . " (" . ms_core_format_money($option_info->amount) . ")" : $option_info->title,
              '#description' => $option_info->description,
            );
            break;

          case 'select':
          case 'radios':
            $sub_options_array = ms_core_map_select_options($option_info->sub_options, array('id', 'name', 'price'));
            $sub_options = array();
            foreach ($sub_options_array as $option_key => $sub_option) {
              $sub_options[$option_key] = $sub_option['name'];
              if (!empty($sub_option['price'])) {
                $sub_options[$option_key] .= " (+" . ms_core_format_money($sub_option['price']) . ")";
              }
            }
            $element['options'][$option_name] = array(
              '#type' => $option_info->widget,
              '#default_value' => !empty($default_options[$option_name]) ? $default_options[$option_name] : key($sub_options),
              '#disabled' => FALSE,
              '#title' => $option_info->title,
              '#options' => $sub_options,
              '#description' => $option_info->description,
            );
            break;
        }
      }
    }
  }

  return $element;
}

/**
 * AJAX callback for the plan choices selector.
 *
 * Renders new options.
 */
function ms_products_plan_field_choice($form, $form_state) {
  $parents = $form_state['triggering_element']['#parents'];
  array_pop($parents);
  return drupal_array_get_nested_value($form, $parents);
}

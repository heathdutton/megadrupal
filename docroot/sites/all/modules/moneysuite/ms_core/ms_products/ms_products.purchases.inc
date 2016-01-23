<?php

/**
 * @file
 * Contains functions pertaining to product purchases.
 *
 * @todo - When modifying from a non recurring to a recurring plan, make sure to expire the old plan
 */

/**
 * Gets the edit path for a purchase.
 *
 * @param int $id
 *   The id of the purchase.
 *
 * @return string|false
 *   The path of the edit page or FALSE.
 */
function ms_products_get_purchase_edit_path($id) {
  $purchase = ms_products_purchase_load($id);
  if ($purchase) {
    $plan = ms_products_plan_load($purchase->sku);
    if ($plan) {
      return 'admin/structure/ms_products/' . $plan->bundle . '/purchases/edit/' . $id;
    }
  }

  return FALSE;
}

/**
 * Gets the delete path for a purchase.
 *
 * @param int $id
 *   The id of the purchase.
 *
 * @return string
 *   The path of the delete page.
 */
function ms_products_get_purchase_delete_path($id) {
  $purchase = ms_products_purchase_load($id);
  if ($purchase) {
    $plan = ms_products_plan_load($purchase->sku);
    if ($plan) {
      return 'admin/structure/ms_products/' . $plan->bundle . '/purchases/delete/' . $id;
    }
  }

  return FALSE;
}

/**
 * Checks if a user has a product.
 *
 * @param int $uid
 *   The user id.
 * @param string $sku
 *   The unique product id.
 * @param bool $renewing
 *   (optional) Whether or not the user is trying to renew an existing purchase.
 *   Defaults to FALSE.
 *
 * @return bool
 *   TRUE if the user has the product and it is active, FALSE if they don't.
 */
function ms_products_user_has_product($uid, $sku, $renewing = FALSE) {
  if ($uid) {
    $result = db_query("SELECT * FROM {ms_products_purchases} WHERE uid = :uid AND sku = :sku", array(':uid' => $uid, ':sku' => $sku));

    foreach ($result as $row) {
      if ($renewing) {
        switch ($row->status) {
          case 'completed':
          case 'payment_received':
          case 'delivered':
            return TRUE;
        }
      } else {
        switch ($row->status) {
          case 'completed':
          case 'canceled':
          case 'active':
          case 'payment_received':
          case 'pending_downgrade':
          case 'delivered':
          case 'expiring':
            return TRUE;
        }
      }
    }
  }

  return FALSE;
}

/**
 * Returns the text for the purchase status.
 *
 * @param string $status
 *   The status machine name.
 *
 * @return string
 *   The proper name for the status.
 */
function ms_products_get_purchase_status_text($status) {
  $statuses = ms_products_get_purchase_statuses();
  return !empty($statuses[$status]) ? $statuses[$status] : $status;
}

/**
 * Returns an array of statuses.
 *
 * @return array
 *   An array of possible statuses, keyed by machine name.
 */
function ms_products_get_purchase_statuses() {
  $statuses = array(
    'expired' => t('Expired / Inactive'),
    'active' => t('Active - Recurring'),
    'canceled' => t('Cancelled'),
    'completed' => t('Active - Payments Completed'),
    'payment_received' => t('Payment Received'),
    'delivered' => t('Delivered'),
    'expiring' => t('Expiring Soon'),
    'pending_downgrade' => t('Pending Downgrade'),
  );

  // Also get custom statuses from bundles.
  $bundles = ms_products_get_bundles();
  foreach ($bundles as $bundle_info) {
    foreach ($bundle_info['purchase_statuses'] as $status => $name) {
      $statuses[$status] = $name;
    }
  }

  return $statuses;
}

/**
 * Changes a purchase's status.
 *
 * @param int $id
 *   The purchase id.
 * @param string $status
 *   The new status.
 */
function ms_products_change_purchase_status($id, $status) {
  $purchase = ms_products_purchase_load($id);
  $plan = ms_products_plan_load($purchase->sku);
  $bundle_info = ms_products_get_bundle($plan->bundle);
  ms_core_log('ms_products', 'Changed the Status of @purchase_name !id to !status',
    array(
      '@purchase_name' => $bundle_info['purchase_name'],
      '!id' => $id,
      '!status' => ms_products_get_purchase_status_text($status),
    ));

  return db_update('ms_products_purchases')
    ->fields(array(
      'status' => $status,
    ))
    ->condition('id', $id)
    ->execute();
}

/**
 * Changes a purchase's order id.
 *
 * @param int $id
 *   The purchase id.
 * @param int $oid
 *   The new order id.
 */
function ms_products_change_purchase_oid($id, $oid) {
  $purchase = ms_products_purchase_load($id);
  $plan = ms_products_plan_load($purchase->sku);
  $bundle_info = ms_products_get_bundle($plan->bundle);

  ms_core_log('ms_products', 'Changed the Order ID of @purchase_name !id to !oid',
    array('@purchase_name' => $bundle_info['purchase_name'], '!id' => $id, '!oid' => $oid));

  return db_update('ms_products_purchases')
    ->fields(array(
      'oid' => $oid,
    ))
    ->condition('id', $id)
    ->execute();
}

/**
 * Changes a purchase's expiration.
 *
 * @param int $id
 *   The purchase id.
 * @param int $expiration
 *   The new expiration date.
 */
function ms_products_change_purchase_expiration($id, $expiration) {
  $purchase = ms_products_purchase_load($id);
  $plan = ms_products_plan_load($purchase->sku);
  $bundle_info = ms_products_get_bundle($plan->bundle);

  ms_core_log('ms_products', 'Changed the Expiration Date of @purchase_name !id to !expiration',
    array(
      '@purchase_name' => $bundle_info['purchase_name'],
      '!id' => $id,
      '!expiration' => format_date($expiration, 'short'),
    ));

  return db_update('ms_products_purchases')
    ->fields(array(
      'expiration' => $expiration,
    ))
    ->condition('id', $id)
    ->execute();
}

/**
 * Loads a Purchase.
 *
 * @param int $id
 *   The purchase id.
 *
 * @return MsProductsPurchase
 *   The purchase object.
 */
function ms_products_purchase_load($id) {
  $result = db_query("SELECT * FROM {ms_products_purchases} WHERE id = :id", array(':id' => $id));
  foreach ($result as $row) {
    // Invoke hook_ms_products_purchase_load().
    $plan = ms_products_plan_load($row->sku);
    $purchase = new MsProductsPurchase($row->bundle);
    $row->data = unserialize($row->data);

    $row_values = (array)$row;

    foreach ($row_values as $key => $value) {
      $purchase->$key = $value;
    }

    $purchase->initialize();

    module_invoke_all('ms_products_purchase_load', $purchase, $plan);
    return $purchase;
  }
  return FALSE;
}

/**
 * Loads a Purchase by Order ID.
 *
 * @param int $oid
 *   The order id.
 * @param string $sku
 *   The unique product id.
 *
 * @return bool|false
 *   The purchase object or FALSE.
 */
function ms_products_load_purchase_by_oid($oid, $sku) {
  $result = db_query("SELECT * FROM {ms_products_purchases} WHERE oid = :oid AND sku = :sku",
    array(':oid' => $oid, ':sku' => $sku));
  foreach ($result as $purchase) {
    return ms_products_purchase_load($purchase->id);
  }
  return FALSE;
}

/**
 * Returns purchases for a user.
 *
 * @param int $uid
 *   The user id.
 * @param string $bundle
 *   (optional) The bundle to get purchases for. Defaults to NULL, which returns
 *   all purchases for all bundles.
 * @param string $sku
 *   (optional) The sku of the product. Defaults to NULL.
 *
 * @return array
 *   An array of purchases.
 */
function ms_products_get_user_purchases($uid, $bundle = NULL, $sku = NULL) {
  $purchases = array();
  if (db_table_exists('ms_products_purchases')) {
    $query = db_select('ms_products_purchases', 'd');
    $query->fields('d');
    $query->condition('d.uid', $uid);

    if (!is_null($bundle)) {
      $query->condition('d.bundle', $bundle);
    }

    if (!is_null($sku)) {
      $query->condition('d.sku', $sku);
    }

    // execute the query
    $result = $query->execute();

    foreach ($result as $purchase) {
      $purchases[] = ms_products_purchase_load($purchase->id);
    }
  }

  return $purchases;
}

/**
 * Returns all purchases.
 *
 * @param string $bundle
 *   (optional) The bundle to get purchases for. Defaults to NULL, which returns
 *   all purchases for all bundles.
 * @param array $statuses
 *   (optional) The statuses that the purchases should be. Defaults to NULL,
 *   which returns all statuses.
 *
 * @return array
 *   An array of purchases.
 */
function ms_products_get_purchases($bundle = NULL, $statuses = NULL) {
  $query = db_select('ms_products_purchases', 'd');
  $query->fields('d');

  if (!is_null($bundle)) {
    $query->condition('d.bundle', $bundle);
  }

  if (!is_null($statuses)) {
    $query->condition('d.status', $statuses, 'IN');
  }

  // execute the query
  $result = $query->execute();

  $purchases = array();
  foreach ($result as $purchase) {
    $purchases[] = ms_products_purchase_load($purchase->id);
  }

  return $purchases;
}

/**
 * Gets a user purchase.
 *
 * @param int $uid
 *   The user id.
 * @param string $sku
 *   The unique product id.
 *
 * @return object
 *   The user purchase.
 */
function ms_products_get_user_purchase($uid, $sku) {
  $result = db_query("SELECT * FROM {ms_products_purchases} WHERE uid = :uid AND sku = :sku", array(':uid' => $uid, ':sku' => $sku));
  foreach ($result as $purchase) {
    return ms_products_purchase_load($purchase->id);
  }
  return FALSE;
}

/**
 * Loads a Purchase by Order ID.
 *
 * @param int $oid
 *   The order id.
 * @param string $sku
 *   (Optional) The sku of the product. Only applies to multi_recurring orders.
 *
 * @return MsProductsPurchase
 *   The purchase object or false.
 */
function ms_products_load_purchase_order_id($oid, $sku = NULL) {
  $query = db_select('ms_products_purchases', 'p')
    ->fields('p')
    ->condition('oid', $oid);

  if ($sku) {
    $query->condition('sku', $sku);
  }

  $result = $query->execute();
  foreach ($result as $purchase) {
    return ms_products_purchase_load($purchase->id);
  }
  return FALSE;
}

/**
 * Sets a purchase field.
 *
 * @param string $bundle
 *   The product type.
 * @param int $purchase_id
 *   The purchase id.
 * @param string $field_name
 *   The field name.
 * @param mixed $value
 *   The value.
 */
function ms_products_set_purchase_field($bundle, $purchase_id, $field_name, $value) {
  $wrapper = entity_metadata_wrapper('ms_products_purchase', $purchase_id);
  $name = $bundle . '_' . $field_name;
  $wrapper->$name->set($value);
  $wrapper->save();
}

/**
 * Gets a purchase field value.
 *
 * @param string $bundle
 *   The product type.
 * @param int $purchase_id
 *   The purchase id.
 * @param string $field_name
 *   The field name.
 */
function ms_products_get_purchase_field($bundle, $purchase_id, $field_name) {
  $wrapper = entity_metadata_wrapper('ms_products_purchase', $purchase_id);
  $name = $bundle . '_' . $field_name;
  return $wrapper->$name->value();
}

/**
 * Loads a purchase by a field value.
 *
 * @param string $bundle
 *   The product type.
 * @param string $field_name
 *   The name of the field.
 * @param mixed $field_value
 *   The value of the field.
 *
 * @return MsProductsPurchase|false
 *   The purchase object or FALSE.
 */
function ms_products_load_purchase_by_field($bundle, $field_name, $field_value) {
  foreach (ms_products_load_purchases_by_field($bundle, $field_name, $field_value) as $purchase) {
    return $purchase;
  }

  return FALSE;
}

/**
 * Loads purchases by a field value.
 *
 * @param string $bundle
 *   The product type.
 * @param string $field_name
 *   The name of the field.
 * @param mixed $field_value
 *   The value of the field.
 *
 * @return array
 *   An array of matching purchases.
 */
function ms_products_load_purchases_by_field($bundle, $field_name, $field_value) {
  $bundle_info = ms_products_get_bundle($bundle);

  if (!empty($bundle_info['purchase_fields'][$field_name]['field_value_column'])) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'ms_products_purchase')
      ->entityCondition('bundle', $bundle)
      ->fieldCondition($bundle . '_' . $field_name, $bundle_info['purchase_fields'][$field_name]['field_value_column'], $field_value);
    $results = $query->execute();
    if (!empty($results['ms_products_purchase'])) {
      $purchase_ids = array_keys($results['ms_products_purchase']);
      return entity_load('ms_products_purchase', $purchase_ids);
    }
  }

  return array();
}

/**
 * Gets the purchases matching the statuses.
 *
 * @param string $sku
 *   The unique plan id.
 * @param array $statuses
 *   (optional) An array of statuses to filter by. Defaults to all statuses.
 *
 * @return array
 *   The purchases matching the query.
 */
function ms_products_get_product_purchases($sku, $statuses = array()) {
  $query = db_select('ms_products_purchases', 'p');
  $query->fields('p');
  $query->condition('sku', $sku);

  if (!empty($statuses)) {
    $query->condition('p.status', $statuses, 'IN');
  }

  $result = $query->execute();

  $purchases = array();

  foreach ($result as $purchase) {
    $purchases[$purchase->id] = $purchase;
  }

  return $purchases;
}

/**
 * Gets the number of purchases matching the statuses.
 *
 * @param string $sku
 *   The unique plan id.
 * @param array $statuses
 *   An array of statuses to filter by.
 *
 * @return int
 *   The number of purchases matching the parameters.
 */
function ms_products_get_purchases_count($sku, $statuses = array()) {
  $purchases = ms_products_get_product_purchases($sku, $statuses);

  return count($purchases);
}

/**
 * Inserts a new purchase into the database.
 *
 * @param int $oid
 *   The order id.
 * @param int $uid
 *   The user id.
 * @param string $sku
 *   The plan sku.
 * @param int $expiration
 *   (Optional) The expiration timestamp. Defaults to 0 (No Expiration).
 * @param string $status
 *   (Optional) The purchase status. Defaults to 'active'.
 * @param int $start_date
 *   (Optional) The start date for the purchase. Defaults to current time.
 * @param int $current_payments
 *   (Optional) The number of payments currently made. Defaults to 0.
 * @param int $max_payments
 *   (Optional) The max number of payments. Defaults to 0.
 *
 * @return MsProductsPurchase
 */
function ms_products_insert_purchase($oid, $uid, $sku, $expiration = 0, $status = 'active', $start_date = 0, $current_payments = 0, $max_payments = 0) {
  if (!$start_date) {
    $start_date = REQUEST_TIME;
  }
  $plan = ms_products_plan_load($sku);
  $id = db_insert('ms_products_purchases')
    ->fields(array(
      'oid' => $oid,
      'uid' => $uid,
      'sku' => $sku,
      'pid' => $plan->pid,
      'expiration' => $expiration,
      'status' => $status,
      'start_date' => $start_date,
      'current_payments' => $current_payments,
      'max_payments' => $max_payments,
      'bundle' => $plan->bundle,
      'data' => serialize(array()),
    ))
    ->execute();

  // Call hook_ms_products_api_purchase_insert().
  $purchase = ms_products_purchase_load($id);
  module_invoke_all('ms_products_api_purchase_insert', $purchase, $plan);
  return $purchase;
}

/**
 * Updates the number of payments for an order.
 *
 * @param int $oid
 *   The order id.
 * @param int $num
 *   (Optional) The number of payments to increment by. Defaults to 1.
 */
function ms_products_increment_payment($oid, $num = 1) {
  db_update('ms_products_purchases')
    ->fields(array(
      'current_payments' => $num,
    ))
    ->condition('oid', $oid)
    ->execute();

  // Mark the order as completed if we have reached the max payments.
  $result = db_query("SELECT * FROM {ms_products_purchases} WHERE oid = :oid", array(':oid' => $oid));
  foreach ($result as $purchase) {
    if ($purchase->max_payments && ($purchase->current_payments == $purchase->max_payments)) {
      db_update('ms_products_purchases')
        ->fields(array(
          'status' => 'completed',
        ))
        ->condition('oid', $oid)
        ->execute();
    }
  }
}

/**
 * Gets a renewal link for a purchase.
 *
 * @param string $sku
 *   The plan sku.
 * @param int $id
 *   The purchase id.
 *
 * @return string
 *   The renew link as an absolute URL.
 */
function ms_products_get_renewal_link($sku, $id) {
  return url('ms_product/purchase/' . $sku . '/renew/' . $id, array('absolute' => TRUE));
}

/**
 * Page callback: Lets a user change the selected options for a purchase.
 *
 * @param MsProductsPurchase
 *   The purchase object.
 *
 * @see ms_products_menu()
 */
function ms_products_change_purchase_options_page($purchase) {
  $plan = ms_products_plan_load($purchase->sku);
  // Get the modification options for a product plan.
  return drupal_get_form('ms_products_change_purchase_options_form', $purchase->oid, $purchase->id, $plan);
}

/**
 * Form constructor for the modify purchase options form.
 *
 * @param int $oid
 *   The order id.
 * @param int $id .
 *   The purchase id.
 * @param MsProductsPlan $plan
 *   The plan the user currently has.
 *
 * @ingroup forms
 * @see ms_products_change_purchase_form_submit()
 */
function ms_products_change_purchase_options_form($form, &$form_state, $oid, $id, MsProductsPlan $plan) {
  $bundle_info = ms_products_get_bundle($plan->bundle);
  $purchase = ms_products_purchase_load($id);
  $form = array();
  $form['ms_products'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('id' => 'ms_products_fieldset'),
    '#title' => t('Change @purchase_name options.', array('@purchase_name' => check_plain($bundle_info['purchase_name']))),
    '#weight' => 0,
    '#collapsible' => FALSE,
  );
  $form['ms_products']['plan'] = array(
    '#type' => 'value',
    '#value' => $plan,
  );
  $form['ms_products']['oid'] = array(
    '#type' => 'value',
    '#value' => $oid,
  );
  $form['ms_products']['id'] = array(
    '#type' => 'value',
    '#value' => $id,
  );
  $form['ms_products']['radios'] = array(
    '#tree' => TRUE,
  );

  $options = array();
  foreach ($plan->plan_options as $option_name => $option_info) {
    switch ($option_info->widget) {
      case 'checkbox':
        $options[$option_name] = t("@option_name (@amount) <br /> !option_description", array(
          '@amount' => ms_core_format_money($option_info->amount),
          '@option_name' => $option_info->title,
          '!option_description' => $option_info->description,
        ));
        break;

      case 'radios':
        $sub_options = ms_core_map_select_options($option_info->sub_options, array('id', 'name', 'price'));

        $radio_options = array();
        foreach ($sub_options as $sub_option_name => $sub_option_info) {
          $radio_options[$sub_option_name] = $sub_option_info['name'];
          if (!empty($sub_option_info['price'])) {
            $radio_options[$sub_option_name] .= ' ' . $sub_option_info['price'];
          }
        }

        $selected = NULL;

        if (!empty($purchase->options[$option_name]->sub_option)) {
          $selected = $purchase->options[$option_name]->sub_option;
        }

        $form['ms_products']['radios'][$option_name] = array(
          '#type' => 'radios',
          '#title' => $option_info->title,
          '#description' => $option_info->description,
          '#options' => $radio_options,
          '#default_value' => $selected,
        );
        break;
    }
  }
  $selected = array();

  foreach ($purchase->options as $option_name => $option) {
    $selected[] = $option_name;
  }

  if (!empty($options)) {
    $form['ms_products']['purchase_options'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Options'),
      '#options' => $options,
      '#default_value' => $selected,
    );
  }
  $form['ms_products']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Form submission handler for ms_products_change_purchase_options_form().
 */
function ms_products_change_purchase_options_form_submit($form, &$form_state) {
  $v = $form_state['values'];

  $order = ms_core_order_load($v['oid']);

  $purchase = ms_products_purchase_load($v['id']);

  $plan = $v['plan'];

  // Get the current purchase options and add up the total.
  $old_amount = 0;
  foreach ($purchase->options as $option_name => $option_info) {
    if ($option_info->status == 'active') {
      $old_amount += $plan->plan_options[$option_name]->amount;
    }
  }

  // Add up the total of the selected plan options.
  $new_amount = 0;
  if (isset($v['purchase_options'])) {
    foreach (array_filter($v['purchase_options']) as $option_name) {
      $new_amount += $plan->plan_options[$option_name]->amount;
    }
  }
  foreach (array_filter($v['radios']) as $option_name => $sub_option_name) {
    $sub_options = ms_core_map_select_options($plan->plan_options[$option_name]->sub_options, array('id', 'name', 'price'));
    if (isset($sub_options[$sub_option_name]['price'])) {
      $new_amount += $sub_options[$sub_option_name]['price'];
    } else {
      $new_amount += $plan->plan_options[$option_name]->amount;
    }
  }

  // If the total for the new options is more, do an upgrade workflow: Add the
  // product to the cart for the modification, and redirect to checkout.
  if ($new_amount > $old_amount) {
    if (!empty($plan->data['grant_credit'])) {
      if ($credit = ms_core_calculate_credit($purchase->oid, $purchase->expiration, $plan->recurring_schedule['main_amount'])) {
        $data['residual_credit'] = $credit;
        $order = ms_core_add_data($order, $data);
      }
    }
    // Add the selected options to the SESSION.
    if (!empty($v['purchase_options'])) {
      $_SESSION['ms_products_options_' . $plan->sku] = $v['purchase_options'];
    }

    // Add the product id if multi_recurring.
    $order_product_id = NULL;
    if ($order->order_type == 'multi_recurring') {
      foreach ($order->products as $product) {
        if ($product->id == $purchase->sku && $product->module == $purchase->bundle) {
          $order_product_id = $product->order_product_id;
        }
      }
    }
    drupal_goto(ms_core_get_modify_url($order, $plan->sku, $order_product_id));
  } else if ($new_amount < $old_amount) {
    // Otherwise, do a downgrade workflow. Redirect to the downgrade page.
    // Add the selected options to the SESSION.
    if (!empty($v['purchase_options'])) {
      $_SESSION['ms_products_options_' . $plan->sku] = $v['purchase_options'];
    }
    drupal_goto('ms/downgrade/' . $order->oid . '/' . $plan->sku);
  } else {
    // Save the purchase options. Simply move them into the data object.
    foreach ($plan->plan_options as $option_name => $option_info) {
      if (isset($purchase->options[$option_name]->id)) {
        $option_info->pid = $purchase->id;
        $option_info->id = $purchase->options[$option_name]->id;
      } else {
        unset($option_info->pid);
        unset($option_info->id);
      }
      $purchase->options[$option_name] = $option_info;
      $purchase->options[$option_name]->name = $option_name;
      if ($purchase->options[$option_name]->expiration) {
        $purchase->options[$option_name]->expiration = strtotime($purchase->options[$option_name]->expiration);
      }

      if (isset($v['purchase_options'][$option_name])) {
        $purchase->options[$option_name]->status = $v['purchase_options'][$option_name] ? 'active' : 'inactive';
      }
      if (isset($v['radios'][$option_name])) {
        $purchase->options[$option_name]->status = 'active';
        $purchase->options[$option_name]->sub_option = $v['radios'][$option_name];
      }
    }

    $purchase->save();
    drupal_set_message(t("Options updated successfully."));
  }
}

/**
 * Page callback: Shows the Available Options for changing a purchase.
 *
 * @param MsProductsPurchase
 *   The purchase object.
 *
 * @see ms_products_menu()
 */
function ms_products_change_purchase_page($purchase) {
  $plan = ms_products_plan_load($purchase->sku);
  // Get the modification options for a product plan.
  $modify_options = ms_products_get_modify_options($plan);
  if ($modify_options) {
    return drupal_get_form('ms_products_change_purchase_form', $modify_options, $purchase->oid, $purchase->id, $plan);
  }
  drupal_access_denied();
}

/**
 * Form constructor for the modify purchase form.
 *
 * @param array $modify_options
 *   The options to show.
 * @param int $oid
 *   The order id.
 * @param int $id .
 *   The purchase id.
 * @param MsProductsPlan $old_plan
 *   The plan the user currently has.
 *
 * @ingroup forms
 * @see ms_products_change_purchase_form_submit()
 */
function ms_products_change_purchase_form($form, &$form_state, $modify_options, $oid, $id, $plan) {
  $plans = array();
  $bundle_info = ms_products_get_bundle($plan->bundle);

  foreach ($modify_options['upgrade'] as $option) {
    // Remove trial periods if any from modifications.
    $option->recurring_schedule['has_trial'] = FALSE;
    $option->recurring_schedule['trial_length'] = 0;
    $plans[$option->sku] = ms_products_get_plan_option_text($option);
  }
  foreach ($modify_options['downgrade'] as $option) {
    // Remove trial periods if any from modifications.
    $option->recurring_schedule['has_trial'] = FALSE;
    $option->recurring_schedule['trial_length'] = 0;
    $plans[$option->sku] = ms_products_get_plan_option_text($option);
  }

  $form = array();
  if (!empty($plans)) {
    $form['old_plan'] = array(
      '#type' => 'value',
      '#value' => $plan,
    );
    $form['oid'] = array(
      '#type' => 'value',
      '#value' => $oid,
    );
    $form['id'] = array(
      '#type' => 'value',
      '#value' => $id,
    );
    $form['ms_products'] = ms_products_plans_element($form_state, t('Choose your new @plan', array('@plan' => $bundle_info['plan_name'])), '', $plans, 'ms_products', TRUE);

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );
  }

  $form_state['redirect'] = drupal_get_destination();

  return $form;
}

/**
 * Form submission handler for ms_products_change_purchase_form().
 */
function ms_products_change_purchase_form_submit($form, &$form_state) {
  $v = $form_state['values'];

  $order = ms_core_order_load($v['oid']);

  $plan = ms_products_plan_load($v['ms_products']['sku']);
  $purchase = ms_products_purchase_load($v['id']);

  $old_plan = $v['old_plan'];
  $modify_options = ms_products_get_modify_options($old_plan);

  // @todo Maybe instead of splitting it into upgrade/downgrade, we should add up
  // the amount owed including plan options?
  if ($order && $order->order_type == 'recurring' && $order->gateway) {
    if (!empty($modify_options['upgrade'][$plan->sku])) {
      if (!empty($plan->data['grant_credit'])) {
        $credit = ms_core_calculate_credit($purchase->oid, $purchase->expiration, $old_plan->recurring_schedule['main_amount']);

        // Check if we need to prorate this even further.
        if (!empty($plan->data['modify_existing_term']) && $purchase->expiration) {
          // We need to prorate the new product price by taking only a percentage of it.
          // For example, if the old plan is being upgraded after 3 months and expires in 9 months,
          // then the new plan will only have 9 months or 75% of the actual time left.
          // So the new plan should only cost 75% of the regular price.
          $new_expiration = ms_core_calculate_expiration_date($plan->recurring_schedule);
          $percent_unused = ($new_expiration - $purchase->expiration) / ($new_expiration - REQUEST_TIME);
          $credit += round($plan->recurring_schedule['main_amount'] * $percent_unused, 2);
        }

        if ($credit) {
          $data['residual_credit'] = $credit;
          $order = ms_core_add_data($order, $data);
        }
      }
      // Add the selected options to the SESSION.
      if (!empty($form_state['values']['ms_products']['options'])) {
        $_SESSION['ms_products_options_' . $plan->sku] = $form_state['values']['ms_products']['options'];
      }

      // Add the product id for multi_recurring.
      // Add the product id if multi_recurring.
      $order_product_id = NULL;
      if ($order->order_type == 'multi_recurring') {
        foreach ($order->products as $product) {
          if ($product->id == $purchase->sku && $product->module == $purchase->bundle) {
            $order_product_id = $product->order_product_id;
          }
        }
      }
      drupal_goto(ms_core_get_modify_url($order, $plan->sku, $order_product_id));
    } elseif (!empty($modify_options['downgrade'][$plan->sku])) {
      // Add the selected options to the SESSION.
      if (!empty($form_state['values']['ms_products']['options'])) {
        $_SESSION['ms_products_options_' . $plan->sku] = $form_state['values']['ms_products']['options'];
      }
      drupal_goto('ms/downgrade/' . $order->oid . '/' . $plan->sku);
    }
  } else {
    // Add a product to the cart and go to checkout.
    if ($plan->sku) {
      $product = ms_core_new_product($plan->name, $plan->bundle, $plan->sku, $plan->recurring_schedule['main_amount'], $plan->cart_type);

      // Store the Data.
      $product->data = $plan->data;

      if ($plan->cart_type == 'recurring') {
        $product->type = 'recurring';
        // Set the Recurring Schedule Settings.
        $product->recurring_schedule = $plan->recurring_schedule;
      }

      // Mark that this purchase is modifying another purchase.
      $product->data['modify_id'] = $purchase->id;

      // Store the plan options.
      if (!empty($form_state['values']['ms_products']['options']) && is_array($form_state['values']['ms_products']['options'])) {
        $option_defaults = $form_state['values']['ms_products']['options'];
        ms_products_add_plan_options($product, $plan, $option_defaults);
      }

      if ($product->amount > 0) {
        $product = ms_core_add_cart_product($product);

        // Add the discount if applicable.
        if (!empty($plan->data['grant_credit'])) {
          if ($credit = ms_core_calculate_credit($purchase->oid, $purchase->expiration, $old_plan->recurring_schedule['main_amount'])) {
            $old_plan = ms_products_plan_load($purchase->sku);
            $adjustment = new MsAdjustment();
            $adjustment->id = 'ms_products_credit_' . $purchase->id;
            $adjustment->product_id = $product->cart_product_id;
            $adjustment->display = t('Credit: !name', array('!name' => $old_plan->name));
            $adjustment->type = 'fixed';
            $adjustment->scope = 'initial';
            $adjustment->value = -$credit;
            $adjustment->weight = -1;

            // Add the coupon to the order.
            ms_core_add_cart_adjustment($adjustment, TRUE);
          }
        }

        ms_core_checkout();
      } else {
        // It's a free product, so let's just change it.
        ms_products_delete_purchase($purchase->id);
        $new_purchase = ms_products_insert_purchase(0, $purchase->uid, $plan->sku, $purchase->expiration, $purchase->status, $purchase->start_date, 1, 1);
        ms_products_handle_purchase_options_payment('cart', $plan, $new_purchase, $product);
        $destination = drupal_get_destination();
        if (isset($destination['destination'])) {
          drupal_goto($destination['destination']);
        }
        drupal_goto();
      }
      exit;
    }
  }
}

/**
 * Page callback: Shows the Purchases Tab page.
 *
 * @param object $account
 *   The user account to show the purchases tab page for.
 * @param string $bundle
 *   The bundle to get purchases for.
 *
 * @see ms_membership_menu()
 */
function ms_products_user_purchases_page($account, $bundle) {
  $build = array();
  $bundle_info = ms_products_get_bundle($bundle);
  $purchases = ms_products_get_user_purchases($account->uid, $bundle);

  // Build the user purchases table.
  if (!empty($purchases)) {
    $headers = array(t('Name'), t('Status'), t('Next Payment'), t('Expiration'));

    // Provide a way for modules to add headers.
    $extra_headers = module_invoke_all('ms_products_purchases_headers', $bundle);
    foreach ($extra_headers as $header) {
      $headers[] = $header;
    }

    $headers[] = t('Actions');

    // Let other modules add headers.
    drupal_alter('ms_products_user_purchases_headers', $headers, $bundle);

    $rows = array();
    foreach ($purchases as $purchase) {
      $plan = ms_products_plan_load($purchase->sku);
      if ($purchase->oid && $purchase->status == 'active' && ($recurring_schedule = ms_core_get_recurring_schedule($purchase->oid, $plan->sku, $bundle))) {
        $np_string = ms_core_get_next_payment_string($recurring_schedule);
      } else {
        $np_string = t('N/A');
      }

      $display = array();

      $display[] = "<span class='ms_products_plan_name' title='"
        . $plan->description . "'>" . $plan->name . "</span>";

      $status = ms_products_get_purchase_status_text($purchase->status);
      $expiration = ($purchase->expiration) ? format_date($purchase->expiration, 'small') : t('None');
      $sub_row = array(
        'name' => implode(' - ', $display),
        'status' => $status,
        'payment' => $np_string,
        'expiration' => $expiration,
      );

      $actions = array();
      $order = ms_core_order_load($purchase->oid);

      switch ($purchase->status) {
        //case 'pending_downgrade': // Should pending downgrade really be able to cancel?
        /** @noinspection PhpMissingBreakStatementInspection */
        case 'active':
          // Cancel Link.
          if ($order && user_access('cancel own ' . $bundle . ' purchases')) {
            $recurring_schedule = ms_core_get_recurring_schedule($order->oid, $plan->sku, $bundle);
            if ($recurring_schedule) {
              $cancel_url = ms_core_get_cancel_url($order, $recurring_schedule->pid);
              if ($cancel_url) {
                $actions['cancel'] = l(t('Cancel'), $cancel_url,
                  array('query' => drupal_get_destination()));
              }
            }
          }
        // Fall through
        case 'canceled':
        case 'expiring':
          // Billing Settings Link.
          if ($order && user_access('edit own payment info') && ($billing_url = ms_core_get_billing_url($order))) {
            $actions['billing'] = l(t('Update Billing Information'), $billing_url,
              array('query' => drupal_get_destination()));
          }

          // Show the Modify Purchase link.
          if (user_access('modify own ' . $bundle . ' purchases')) {
            if ($modify_options = ms_products_get_modify_options($plan)) {
              foreach ($modify_options['upgrade'] as $modify_option) {
                if (!$order OR ($order && (!$order->gateway OR $order->gateway == 'ms_core' OR ($order->gateway && ms_core_get_modify_url($order, $purchase->sku))))) {
                  $actions['modify'] = l(t('Change @purchase', array('@purchase' => $bundle_info['purchase_name'])), 'ms_products/change/' . $purchase->id,
                    array('attributes' => array('class' => array('ms_modify_button'), 'rel' => $plan->sku)));
                }
              }
              foreach ($modify_options['downgrade'] as $modify_option) {
                if (!$order OR ($order && (!$order->gateway OR $order->gateway == 'ms_core' OR ($order->gateway && ms_core_get_modify_url($order, $purchase->sku))))) {
                  $actions['modify'] = l(t('Change @purchase', array('@purchase' => $bundle_info['purchase_name'])), 'ms_products/change/' . $purchase->id,
                    array('attributes' => array('class' => array('ms_modify_button'), 'rel' => $plan->sku)));
                }
              }
            }
            // Show the 'Change Options' link if needed.
            if (!empty($plan->data['change_plan_options']) && !empty($plan->plan_options)) {
              $actions['change_options'] = l(t('Change options'), 'ms_products/change_options/' . $purchase->id,
                array('attributes' => array('class' => array('ms_change_button'), 'rel' => $plan->sku)));
            }
          }
          break;

        case 'completed':
          // Show the Modify Purchase link.
          if (user_access('modify own ' . $bundle . ' purchases')) {
            if ($modify_options = ms_products_get_modify_options($plan)) {
              $options_combined = $modify_options['upgrade'] + $modify_options['downgrade'];
              foreach ($options_combined as $modify_option) {
                if (!$order OR ($order && (!$order->gateway OR $order->gateway == 'ms_core' OR ($order->gateway && ms_core_get_modify_url($order, $purchase->sku))))) {
                  $actions['modify'] = l(t('Change @purchase', array('@purchase' => $bundle_info['purchase_name'])), 'ms_products/change/' . $purchase->id,
                    array('attributes' => array('class' => array('ms_modify_button'), 'rel' => $plan->sku)));
                }
              }
            }
          }
          break;
      }

      // TODO - Change the display of the product so that it shows that it is downgraded to X.

      // Give other modules a chance to add links.
      $extra_actions = module_invoke_all('ms_products_purchase_actions', $account, $purchase, $plan);

      $actions = array_merge($actions, $extra_actions);

      // Show Renew/Extend Link.
      // Check to see if the user can purchase the product according to their
      // roles.
      if (user_access('renew own ' . $bundle . ' purchases') && ms_products_can_purchase($account, $plan) && !ms_products_plan_is_free($plan)) {
        if ($plan->cart_type == 'recurring') {
          if (in_array($purchase->status, array('expired', 'canceled'))) {
            $actions['renew'] = l(t('Renew'), ms_products_get_renewal_link($purchase->sku, $purchase->id));
          }
        } else {
          if (ms_products_can_purchase($account, $plan)) {
            if (in_array($purchase->status, array('expired'))) {
              $actions['renew'] = l(t('Renew'), ms_products_get_renewal_link($purchase->sku, $purchase->id));
            } else if ($purchase->expiration && empty($plan->recurring_schedule['fixed_date'])) {
              $actions['extend'] = l(t('Extend term'), ms_products_get_renewal_link($purchase->sku, $purchase->id));
            }
          }
        }
      }

      if (user_access('administer ' . $bundle . ' purchases')) {
        $actions['edit'] = l(t('Edit @purchase_name', array('@purchase_name' => $bundle_info['purchase_name'])), 'admin/structure/ms_products/' . $bundle . '/purchases/edit/' . $purchase->id);
      }

      // Provide a way for modules to add rows.
      $extra_rows = module_invoke_all('ms_products_purchases_row', $bundle, $purchase);
      foreach ($extra_rows as $extra_row) {
        $sub_row[] = $extra_row;
      }

      $sub_row[] = implode($actions, ' | ');

      $rows[] = $sub_row;
    }

    if (!empty($rows)) {
      $build['current_purchases'] = array(
        '#type' => 'fieldset',
        '#title' => t('Current @purchases', array('@purchases' => $bundle_info['purchase_name_plural'])),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
        '#weight' => 0,
      );
      $build['current_purchases']['list'] = array(
        '#type' => 'item',
        '#markup' => theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 'ms-user-purchases-table'), 'empty' => t('No @purchases yet.', array('@purchases' => $bundle_info['purchase_name_plural'])))),
      );
    }
  }

  // Show the Purchase Box in User Account.
  $purchase_options = array();
  if (user_access('purchase ' . $bundle . ' products')) {
    $has_active_sub = FALSE;
    foreach ($purchases as $purchase) {
      if (ms_products_user_has_product($account->uid, $purchase->sku)) {
        $has_active_sub = TRUE;
        break;
      }
    }

    if (!$has_active_sub OR user_access('have multiple ' . $bundle . ' purchases')) {
      $plans = ms_products_get_plans($bundle);
      $active_purchases = array();
      foreach ($purchases as $purchase) {
        if ($purchase->status == 'active') {
          $active_purchases[$purchase->sku] = 1;
        }
      }
      foreach ($plans as $plan) {
        if (!ms_products_can_purchase($account, $plan)) {
          continue;
        }

        if (empty($active_purchases[$plan->sku]) && !empty($plan->show_account)) {
          $purchase_options[$plan->sku] = $plan;
        }
      }
    }
  }

  if (!empty($purchase_options)) {
    $build['ms_products_purchase'] = drupal_get_form('ms_products_user_purchase_options_form', $purchase_options, $bundle);
  }

  // Let other modules add to or modify this page.
  drupal_alter('ms_products_user_purchases_page', $build);

  return $build;
}

/**
 * Form constructor for the purchase plan form.
 *
 * @param array $plans
 *   The plans to use as options.
 * @param string $bundle
 *   The bundle type.
 * @param string $default
 *   (Optional) The default sku to select. Defaults to none.
 *
 * @ingroup forms
 * @see ms_products_user_purchase_options_form_submit()
 */
function ms_products_user_purchase_options_form($form, &$form_state, $plans, $bundle, $default = NULL) {
  $bundle_info = ms_products_get_bundle($bundle);
  $purchase_options = array();

  foreach ($plans as $sku => $plan) {
    $purchase_options[$sku] = t('@plan - @amount<br />!description', array(
      '@plan' => $plan->name,
      '@amount' => ms_core_get_recurring_string($plan->recurring_schedule),
      '!description' => $plan->description,
    ));
  }

  $form['purchase_options'] = ms_products_plans_element($form_state, t('Select @plan', array('@plan' => $bundle_info['plan_name'])), '', $purchase_options, 'purchase_options', TRUE, $default);
  $form['purchase_options']['#title'] = t('Purchase @purchase', array('@purchase' => $bundle_info['purchase_name']));
  $form['purchase_options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Purchase'),
  );
  $form_state['redirect'] = drupal_get_destination();

  return $form;
}

/**
 * Form submission handler for ms_products_user_purchase_options_form().
 */
function ms_products_user_purchase_options_form_submit($form, &$form_state) {
  // Add the selected plan and options to the cart.
  if ($plan = ms_products_plan_load($form_state['values']['purchase_options']['sku'])) {
    $data = array();
    if (!empty($form_state['values']['purchase_options']['options']) && is_array($form_state['values']['purchase_options']['options'])) {
      $data['option_defaults'] = $form_state['values']['purchase_options']['options'];
    }
    ms_products_add_product_to_cart($plan, $data, FALSE);
    drupal_goto('ms/checkout');
  }
}

/**
 * Modifies a purchase to use a different product plan.
 *
 * @param MsOrder $order
 *   The order object.
 * @param MsProductsPurchase $old_purchase
 *   The purchase object.
 * @param MsProductsPlan $plan
 *   The new product plan.
 */
function ms_products_modify_purchase(MsOrder $order, MsProductsPurchase $old_purchase, MsProductsPlan $plan) {
  $account = user_load($order->uid);

  // Load the old product plan from the old_purchase.
  $old_plan = ms_products_plan_load($old_purchase->sku);
  $bundle_info = ms_products_get_bundle($plan->bundle);

  // Update the purchase info to the new product plan.
  db_update('ms_products_purchases')
    ->fields(array(
      'status' => $plan->cart_type == 'cart' ? 'complete' : 'active',
      'sku' => $plan->sku,
      'pid' => $plan->pid,
      'oid' => $order->oid,
      'current_payments' => 0,
      'max_payments' => $plan->recurring_schedule['total_occurrences'],
      'start_date' => REQUEST_TIME,
      'expiration' => !empty($old_plan->data['modify_existing_term']) ? $old_purchase->expiration : ms_products_get_expiration_date($plan->sku),
    ))
    ->condition('id', $old_purchase->id)
    ->execute();

  ms_core_log('ms_products', 'Modified @purchase for user %user from %old_mem to %new_mem',
    array(
      '@purchase' => $bundle_info['purchase_name'],
      '%user' => $account->name,
      '%old_mem' => $old_plan->name,
      '%new_mem' => $plan->name,
    ));

  $purchase = ms_products_purchase_load($old_purchase->id);

  $payment_vars = array(
    'id' => $purchase->id,
  );

  // Set Message.
  drupal_set_message(t('Your @purchase_name has been changed to @purchase.',
    array('@purchase_name' => $bundle_info['purchase_name'], '@purchase' => $plan->name)));

  // Send Mail.
  ms_products_send_mail('ms_products', 'modify', $account->mail, user_preferred_language($account), $payment_vars);

  // Invoke hook_ms_products_modification().
  module_invoke_all('ms_products_modification', $account, $purchase, $plan, $old_plan);

  return $purchase;
}

/**
 * Form constructor for the add new purchase form.
 *
 * @param string $bundle
 *   The bundle we are adding a purchase for.
 *
 * @ingroup forms
 * @see ms_products_purchase_add_form_validate()
 * @see ms_products_purchase_add_form_submit()
 */
function ms_products_purchase_add_form($form, &$form_state, $bundle) {
  $form['bundle'] = array(
    '#type' => 'value',
    '#value' => $bundle,
  );
  $bundle_info = ms_products_get_bundle($bundle);
  $form['purchase'] = array(
    '#type' => 'fieldset',
    '#title' => t('New @purchase_name', array('@purchase_name' => $bundle_info['purchase_name'])),
    '#weight' => -49,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['purchase']['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#size' => 32,
    '#maxlength' => 64,
    '#description' => t("The Name of the User"),
    '#autocomplete_path' => 'admin/structure/ms_core/autocomplete',
    '#required' => TRUE,
  );
  $plans = ms_products_get_plans_list($bundle);
  $form['purchase']['sku'] = array(
    '#type' => 'select',
    '#title' => 'Product Plans',
    '#options' => $plans,
    '#description' => t("The @plan_name to use as a template",
      array('@plan_name' => $bundle_info['plan_name'])),
    '#required' => TRUE,
  );

  $purchase = new MsProductsPurchase($bundle);

  // Display all of the purchase fields here as well. Also, save them in the submit function.
  field_attach_form('ms_products_purchase', $purchase, $form, $form_state);

  drupal_alter('ms_products_api_purchase_add_form', $form, $bundle);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form validation handler for ms_products_purchase_add_form().
 *
 * @see ms_products_purchase_add_form_submit()
 */
function ms_products_purchase_add_form_validate($form, &$form_state) {
  $v = $form_state['values'];
  if (!$account = user_load_by_name($v['username'])) {
    form_set_error('username', t('You must enter a valid user'));
  }
  $purchase = new MsProductsPurchase($form_state['values']['bundle']);
  field_attach_form_validate('ms_products_purchase', $purchase, $form, $form_state);
  module_invoke_all('ms_products_api_purchase_add_form_validate', $form_state, $form_state['values']['bundle']);
}

/**
 * Form submission handler for ms_products_purchase_add_form().
 *
 * @see ms_products_purchase_add_form_validate()
 */
function ms_products_purchase_add_form_submit($form, &$form_state) {
  $v = $form_state['values'];
  $account = user_load_by_name($v['username']);
  $plan = ms_products_plan_load($v['sku']);

  $purchase = ms_products_insert_new_purchase($plan, $account);
  field_attach_submit('ms_products_purchase', $purchase, $form, $form_state);
  field_attach_update('ms_products_purchase', $purchase);

  $form_state['redirect'] = 'admin/structure/ms_products/' . $v['bundle'] . '/purchases/list';
}

/**
 * Inserts a new purchase for a user.
 *
 * @param MsProductsPlan $plan
 *   The product plan.
 * @param object $account
 *   The user account.
 */
function ms_products_insert_new_purchase(MsProductsPlan $plan, $account) {
  $bundle_info = ms_products_get_bundle($plan->bundle);

  // Insert into Purchases database.
  $status = 'completed';
  $expiration = ms_products_get_expiration_date($plan->sku);
  $purchase = ms_products_insert_purchase(0, $account->uid, $plan->sku, $expiration, $status, 0, 1, 1);

  $payment_vars = array(
    'id' => $purchase->id,
  );

  // Invoke hook_ms_products_signup to let other modules act on a new purchase.
  module_invoke_all('ms_products_signup', $account, $purchase, $plan);

  ms_products_send_mail('ms_products', 'signup', $account->mail, user_preferred_language($account), $payment_vars);

  drupal_set_message(t('@purchase_name Added for user: %name.',
    array('@purchase_name' => $bundle_info['purchase_name'], '%name' => $account->name)));

  ms_core_log('ms_products', '@plan_name %product manually added for user %username',
    array(
      '@plan_name' => $bundle_info['plan_name'],
      '%product' => $plan->name,
      '%username' => $account->name,
    ));

  return $purchase;
}

/**
 * Form constructor for the edit purchase form.
 *
 * @param string $bundle
 *   The bundle we are working with.
 * @param MsProductsPurchase
 *   The purchase object we are editing.
 *
 * @ingroup forms
 * @see ms_products_purchase_edit_form_validate()
 * @see ms_products_purchase_edit_form_submit()
 */
function ms_products_purchase_edit_form($form, &$form_state, $bundle, $purchase) {
  $form['bundle'] = array(
    '#type' => 'value',
    '#value' => $bundle,
  );
  drupal_add_js(drupal_get_path('module', 'ms_products') . '/js/ms_products_edit_form.js');
  $plan = ms_products_plan_load($purchase->sku);
  $bundle_info = ms_products_get_bundle($plan->bundle);
  $form['purchase'] = array(
    '#type' => 'fieldset',
    '#title' => t('Purchase Settings'),
    '#weight' => -50,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['purchase']['id'] = array(
    '#type' => 'value',
    '#value' => $purchase->id,
  );
  $account = user_load($purchase->uid);
  $form['purchase']['username'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#size' => 32,
    '#maxlength' => 64,
    '#description' => t("The User Name"),
    '#disabled' => TRUE,
    '#value' => $account->name,
  );
  $form['purchase']['current_payments'] = array(
    '#type' => 'textfield',
    '#title' => t('Payments'),
    '#size' => 8,
    '#maxlength' => 10,
    '#description' => t("How many payments have been made?"),
    '#required' => FALSE,
    '#default_value' => $purchase->current_payments,
  );
  $form['purchase']['max_payments'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Payments'),
    '#size' => 8,
    '#maxlength' => 10,
    '#description' => t("How many payments are expected?"),
    '#required' => FALSE,
    '#default_value' => $purchase->max_payments,
  );
  $form['purchase']['start_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Start Date'),
    '#description' => t("When was this @purchase_name created?",
      array('@purchase_name' => $bundle_info['purchase_name'])),
    '#required' => TRUE,
    '#attributes' => array('class' => array('datepicker')),
    '#default_value' => date('m/d/Y', $purchase->start_date),
  );
  $form['purchase']['should_expire'] = array(
    '#type' => 'checkbox',
    '#title' => t('Expiration?'),
    '#description' => t("Check this to set an expiration date for the @purchase_name.
    Leaving this unchecked will cause the purchase to never expire.",
      array('@purchase_name' => $bundle_info['purchase_name'])),
    '#required' => FALSE,
    '#default_value' => ($purchase->expiration ? TRUE : FALSE),
  );
  $form['purchase']['expiration'] = array(
    '#type' => 'textfield',
    '#title' => t('Expiration Date'),
    '#description' => t("When does this @purchase_name Expire?",
      array('@purchase_name' => $bundle_info['purchase_name'])),
    '#attributes' => array('class' => array('datepicker')),
    '#default_value' => $purchase->expiration ? date('m/d/Y', $purchase->expiration) : '',
  );
  $form['purchase']['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => ms_products_get_purchase_statuses(),
    '#description' => t("What is the Status of the @purchase_name?",
      array('@purchase_name' => $bundle_info['purchase_name'])),
    '#required' => FALSE,
    '#default_value' => $purchase->status,
  );

  field_attach_form('ms_products_purchase', $purchase, $form, $form_state);

  // Show a 'purchase_options' fieldset with #tree and add the purchase_options
  // elements.
  if (!empty($plan->plan_options)) {
    $form['plan_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Options'),
      '#tree' => TRUE,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    foreach ($plan->plan_options as $option_name => $option_info) {
      $form['plan_options'][$option_name] = array(
        '#type' => 'fieldset',
        '#title' => !empty($option_info->title) ? filter_xss($option_info->title) : $option_info->name,
        '#description' => filter_xss($option_info->description),
        '#tree' => TRUE,
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
      );
      $form['plan_options'][$option_name]['status'] = array(
        '#type' => 'select',
        '#title' => t('Status'),
        '#options' => array(
          'inactive' => t('Inactive'),
          'active' => t('Active'),
          'expired' => t('Expired'),
        ),
        '#default_value' => !empty($purchase->options[$option_name]->status) ? $purchase->options[$option_name]->status : 'inactive',
      );
      $form['plan_options'][$option_name]['expiration'] = array(
        '#type' => 'textfield',
        '#title' => t('Expiration'),
        '#description' => t('Leave blank for no expiration.'),
        '#default_value' => !empty($purchase->options[$option_name]->expiration) ? date('m/d/Y', $purchase->options[$option_name]->expiration) : '',
        '#attributes' => array('class' => array('datepicker')),
      );
      if (isset($purchase->options[$option_name]->id)) {
        $form['plan_options'][$option_name]['id'] = array(
          '#type' => 'value',
          '#value' => $purchase->options[$option_name]->id,
        );
      }

      // Show suboptions if applicable.
      if (!empty($option_info->sub_options)) {
        $sub_options = ms_core_map_select_options($plan->plan_options[$option_name]->sub_options, array('id', 'name', 'price'));
        $radio_options = array();
        foreach ($sub_options as $sub_option_name => $sub_option_info) {
          $radio_options[$sub_option_name] = $sub_option_info['name'];
          if (!empty($sub_option_info['price'])) {
            $radio_options[$sub_option_name] .= ' ' . $sub_option_info['price'];
          }
        }
        $form['plan_options'][$option_name]['sub_option'] = array(
          '#type' => 'radios',
          '#title' => t('Sub-Options'),
          '#default_value' => $purchase->options[$option_name]->sub_option,
          '#options' => $radio_options,
        );
      }
    }
  }

  $form['#after_build'] = array('ms_products_purchase_form_uidatepicker');

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form_state['redirect'] = 'admin/structure/ms_products/' . $bundle . '/purchases/list';

  // Call the alter function to let other modules add elements to the form.
  $context = array(
    'purchase' => $purchase,
    'product_plan' => $plan,
  );
  drupal_alter('ms_products_api_purchase_edit_form', $form, $bundle, $context);

  return $form;
}

/**
 * Adds a datepicker ui to specified elements in the form.
 */
function ms_products_purchase_form_uidatepicker($form, $form_state) {
  // @todo - Add a date time picker in the future.
  // @see http://trentrichardson.com/examples/timepicker/
  drupal_add_library('system', 'ui.datepicker');
  drupal_add_js("(function ($) { $('.datepicker').datepicker(); })(jQuery);", array('type' => 'inline', 'scope' => 'footer', 'weight' => 5));
  return $form;
}

/**
 * Form validation handler for ms_products_purchase_edit_form()
 *
 * @see ms_products_purchase_edit_form_submit()
 */
function ms_products_purchase_edit_form_validate($form, &$form_state) {
  $v = $form_state['values'];
  $purchase = ms_products_purchase_load($v['id']);
  field_attach_form_validate('ms_products_purchase', $purchase, $form, $form_state);

  module_invoke_all('ms_products_api_purchase_edit_form_validate', $form_state, $form_state['values']['bundle']);
}

/**
 * Form submission handler for ms_products_purchase_edit_form()
 *
 * @see ms_products_purchase_edit_form_validate()
 */
function ms_products_purchase_edit_form_submit($form, &$form_state) {
  $v = $form_state['values'];
  $purchase = ms_products_purchase_load($v['id']);
  $account = user_load($purchase->uid);
  field_attach_submit('ms_products_purchase', $purchase, $form, $form_state);
  $plan = ms_products_plan_load($purchase->sku);

  $purchase->current_payments = $v['current_payments'];
  $purchase->max_payments = $v['max_payments'];
  $purchase->status = $v['status'];
  $purchase->start_date = strtotime($v['start_date']);
  if (!$v['should_expire']) {
    $purchase->expiration = 0;
  } else {
    $purchase->expiration = strtotime($v['expiration']);
  }

  // Save the purchase options. Simply move them into the data object.
  if (!empty($v['plan_options'])) {
    foreach ($v['plan_options'] as $option_name => $option_info) {
      $purchase->options[$option_name] = (object)$option_info;
      $purchase->options[$option_name]->pid = $purchase->id;
      $purchase->options[$option_name]->name = $option_name;
      if ($purchase->options[$option_name]->expiration) {
        $purchase->options[$option_name]->expiration = strtotime($purchase->options[$option_name]->expiration);
      }
    }
  }

  // Pass the values off to presave.
  module_invoke_all('ms_products_api_purchase_presave', $v, $purchase, $plan);

  $purchase->save();
  drupal_set_message(t('Purchase Saved.'));

  // Call hook_ms_products_expiring if it is expired.
  if ($purchase->status == 'expired') {
    $plan = ms_products_plan_load($purchase->sku);
    module_invoke_all('ms_products_expiring', $account, $purchase, $plan);
  }
  $form_state['redirect'] = 'admin/structure/ms_products/' . $v['bundle'] . '/purchases/list';
}

/**
 * Form constructor for the delete purchase form.
 *
 * @param string $bundle
 *   The product type.
 * @param object $purchase
 *   The purchase object.
 *
 * @ingroup forms
 * @see confirm_form()
 */
function ms_products_delete_purchase_confirm($form, &$form_state, $bundle, $purchase) {
  $form['#ms_id'] = $purchase->id;
  $form['#ms_bundle'] = $bundle;
  $bundle_info = ms_products_get_bundle($bundle);
  return confirm_form($form, t('Are you sure you want to delete this @purchase_name?',
      array('@purchase_name' => $bundle_info['purchase_name'])),
    'admin/structure/ms_products/' . $bundle . '/purchases/list',
    t('This action cannot be undone.'), t('Delete'));
}

/**
 * Submission handler for ms_products_delete_purchase_confirm().
 */
function ms_products_delete_purchase_confirm_submit($form, &$form_state) {
  $purchase = ms_products_purchase_load($form['#ms_id']);
  $account = user_load($purchase->uid);
  $plan = ms_products_plan_load($purchase->sku);
  $bundle_info = ms_products_get_bundle($plan->bundle);

  ms_products_delete_purchase($purchase->id);

  drupal_set_message(t('@purchase_name %product for user %name Deleted',
    array(
      '@purchase_name' => $bundle_info['purchase_name'],
      '%name' => $account->name,
      '%product' => $plan->name,
    )));

  $form_state['redirect'] = 'admin/structure/ms_products/' . $plan->bundle . '/purchases/list';
}

/**
 * Deletes a purchase.
 *
 * @param int $id
 *   The purchase id.
 */
function ms_products_delete_purchase($id) {
  $purchase = ms_products_purchase_load($id);
  $plan = ms_products_plan_load($purchase->sku);

  field_attach_delete('ms_products_purchase', $purchase);

  // Call hook_ms_products_api_purchase_delete().
  module_invoke_all('ms_products_api_purchase_delete', $purchase, $plan);

  // Delete the purchase.
  db_delete('ms_products_purchases')
    ->condition('id', $purchase->id)
    ->execute();
  db_delete('ms_products_purchase_options')
    ->condition('pid', $purchase->id)
    ->execute();
}

/**
 * Page callback: Shows a listing of all purchases.
 *
 * @param string $bundle
 *   (Optional) The bundle we are working with. Defaults to NULL which shows
 *   purchases from all bundles.
 *
 * @see ms_products_menu()
 */
function ms_products_show_purchases($bundle = NULL) {
  $headers = array(
    array(
      'data' => t('Start Date'),
      'class' => array('ms_products_purchase_start_date_header'),
      'field' => 'd.start_date',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Expiration'),
      'class' => array('ms_products_purchase_expiration_header'),
      'field' => 'd.expiration',
      'sort' => 'desc',
    ),
    array(
      'data' => t('User'),
      'class' => array('ms_products_purchase_user_header'),
      'field' => 'u.name',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Product'),
      'class' => array('ms_products_purchase_name_header'),
      'field' => 'p.name',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Order'),
      'class' => array('ms_products_purchase_order_header'),
    ),
    array(
      'data' => t('Status'),
      'class' => array('ms_products_purchase_status_header'),
      'field' => 'd.status',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Payments'),
      'class' => array('ms_products_purchase_current_payments_header'),
      'field' => 'd.current_payments',
      'sort' => 'desc',
    ),
  );

  // Provide a way for modules to add headers.
  $extra_headers = module_invoke_all('ms_products_purchases_headers', $bundle);
  foreach ($extra_headers as $header) {
    $headers[] = $header;
  }

  $headers[] = array(
    'data' => t('Actions'),
    'class' => array('ms_products_purchase_actions_header'),
    'sort' => 'desc',
  );

  $query = db_select('ms_products_purchases', 'd');
  $query->leftjoin('ms_products_plans', 'p', 'd.sku = p.sku');
  $query->leftjoin('users', 'u', 'd.uid = u.uid');
  $query->fields('d');
  $query->addField('p', 'name', 'product_name');
  $query->fields('u', array('name'));
  $query = $query->extend('TableSort')->orderByHeader($headers);
  $query = $query->extend('PagerDefault')->limit(20);

  if (!empty($bundle)) {
    $query->condition('d.bundle', $bundle);
  }

  $name = '';
  if (isset($_GET['name'])) {
    $name = $_GET['name'];
    $query->condition('u.name', '%' . $name . '%', 'LIKE');
  }

  // Let other modules alter the query.
  drupal_alter('ms_products_purchases_query', $query, $bundle);

  $result = $query->execute();

  $rows = array();
  $attr = array('id' => 'ms-purchases-table');
  foreach ($result as $row) {
    $purchase = ms_products_purchase_load($row->id);
    $order = ms_core_order_load($row->oid);
    $plan = ms_products_plan_load($purchase->sku);
    $actions = array(
      l(t('Edit'), 'admin/structure/ms_products/' . $bundle . '/purchases/edit/' . $purchase->id),
      l(t('Delete'), 'admin/structure/ms_products/' . $bundle . '/purchases/delete/' . $purchase->id, array('attributes' => array('class' => array('txn_del')))),
    );
    $row = array(
      array(
        'data' => format_date($purchase->start_date, 'small'),
        'class' => array('ms_products_purchase_start_date'),
      ),
      array(
        'data' => ($purchase->expiration ? format_date($purchase->expiration, 'small') : 'None'),
        'class' => array('ms_products_purchase_expiration'),
      ),
      array('data' => l($row->name, 'user/' . $purchase->uid), array('class' => 'ms_products_purchase_user')),
      array(
        'data' => $plan->name,
        'class' => array('ms_products_product_name'),
      ),
      array(
        'data' => $order ? l('#' . $order->order_number, 'admin/moneysuite/orders/view/' . $order->order_key) : t('N/A'),
        'class' => array('ms_products_product_status'),
      ),
      array(
        'data' => ms_products_get_purchase_status_text($purchase->status),
        'class' => array('ms_products_purchase_status'),
      ),
      array(
        'data' => $purchase->current_payments . '/' . ($purchase->max_payments ? $purchase->max_payments : 'Unlimited'),
        'class' => array('ms_products_purchase_current_payments'),
      ),
    );

    // Provide a way for modules to add headers.
    $extra_rows = module_invoke_all('ms_products_purchases_row', $bundle, $purchase);
    foreach ($extra_rows as $extra_row) {
      $row[] = $extra_row;
    }

    $row[] = array(
      'data' => implode($actions, ' | '),
      'class' => array('ms_products_purchase_actions'),
    );

    $rows[] = $row;
  }

  // Add the name filter.
  $filters = array('name' => $name);
  // Let other modules alter the filters.
  drupal_alter('ms_products_purchases_filters', $filters, $bundle);
  $filters_form = drupal_get_form('ms_core_get_filter_form', $filters);

  return array(
    'content' => array(
      'filters' => array(
        '#markup' => drupal_render($filters_form),
      ),
      'purchases_table' => array(
        '#markup' => theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => $attr, 'empty' => t('None found.'))),
      ),
    ),
    'pager' => array(
      '#theme' => 'pager',
      '#weight' => 5,
    )
  );
}

/**
 * Changes the expiration of a purchase option.
 *
 * @param int $pid
 *   The purchase id.
 * @param string $name
 *   The option name.
 * @param int $expiration
 *   The expiration timestamp.
 */
function ms_products_change_purchase_option_expiration($pid, $name, $expiration) {
  db_update('ms_products_purchase_options')
    ->fields(array(
      'expiration' => $expiration,
    ))
    ->condition('pid', $pid)
    ->condition('name', $name)
    ->execute();
}

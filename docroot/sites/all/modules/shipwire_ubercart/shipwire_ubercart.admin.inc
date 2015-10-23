<?php

/**
 * @file
 * Admin setting for Shipwire Ubercart module.
 */

/**
 * Admin form for shipwire ubercart settings.
 */
function shipwire_ubercart_admin_settings() {
  $form = array();

  if (!Shipwire::operational()) {
    $form['configure'] = array(
      '#markup' => t('Shipwire account settings must be configured first.') . ' '
      . l(t('Configure Shipwire'), 'admin/shipwire/settings'),
    );
    return system_settings_form($form);
  }
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
    '#collapsible' => TRUE,
  );
  $form['general']['shipwire_ubercart_order_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Order ID prefix'),
    '#description' => t('To ensure unique order numbers please specify an'
      . ' order prefix. This will be added to the beginning of the Ubercart'
      . ' order number e.g. Prefix "SU-" and Ubercart order number "11573" form'
      . ' Order id "SU-11573". The prefix must include a non numeric character.'),
    '#default_value' => variable_get('shipwire_ubercart_order_prefix', 'SU-'),
    '#size' => 5,
    '#required' => TRUE,
  );
  $form['general']['shipwire_ubercart_log_order'] = array(
    '#type' => 'radios',
    '#title' => t('Save shipment status messages to order log'),
    '#default_value' => variable_get('shipwire_ubercart_log_order', 1),
    '#options' => array(1 => t('Yes'), 0 => t('No')),
  );

  if (user_access('configure quotes')) {
    $currency_code = variable_get('uc_currency_code', 'USD');
    $currency_sign = variable_get('uc_currency_sign', '$');
    $currency_sign_after = variable_get('uc_sign_after_amount', FALSE);

    $field_prefix = '';
    $field_suffix = '';
    if ($currency_sign_after) {
      $field_suffix = $currency_sign . ' (' . $currency_code . ')';
    }
    else {
      $field_prefix = $currency_sign;
      $field_suffix = $currency_code;
    }

    $form['rate'] = array(
      '#type' => 'fieldset',
      '#title' => t('Shipping rate quote'),
      '#collapsible' => TRUE,
    );
    $form['rate']['shipwire_ubercart_padding_rate'] = array(
      '#type' => 'textfield',
      '#title' => t('Delivery rate padding'),
      '#description' => t('Additional handling fee towards added to the quote'
        . ' returned from Shipwire.'),
      '#default_value' => variable_get('shipwire_ubercart_padding_rate', 0),
      '#field_prefix' => $field_prefix,
      '#field_suffix' => $field_suffix,
      '#size' => 10,
    );
    $form['rate']['shipwire_ubercart_padding_days'] = array(
      '#type' => 'textfield',
      '#title' => t('Delivery days padding'),
      '#description' => t('Additional days added to the estimated shipping'
        . ' delivery date.'),
      '#default_value' => variable_get('shipwire_ubercart_padding_days', 0),
      '#field_suffix' => t('Days'),
      '#size' => 10,
    );
    $form['rate']['shipwire_ubercart_exclude'] = array(
      '#type' => 'textarea',
      '#title' => t('Exclude rates when order contains product SKUs'),
      '#description' => t('Enter a comma separated list of product SKUs if you'
        . ' wish Shipwire to exclude shipping rate quotes when the order contains'
        . ' the products. Only use this feature if you have another shipping rate'
        . ' module installed or customers will not be able to complete checkout.'),
      '#default_value' => variable_get('shipwire_ubercart_exclude', ''),
    );
  }

  $form['fulfill'] = array(
    '#type' => 'fieldset',
    '#title' => t('Order fulfillment'),
    '#collapsible' => TRUE,
  );
  $form['fulfill']['shipwire_ubercart_fulfill_type'] = array(
    '#type' => 'radios',
    '#title' => t('Order fulfillment method'),
    '#description' => t('Choose whether fulfillment should take place'
      . ' immediately using rules or using batch processing on CRON runs.'
      . ' Immediate execution requires communication with Shipwire during the'
      . ' checkout process which will slow down checkout and possibly fail'
      . ' causing checkout to stop. Batch processing on CRON runs reduces the'
      . ' number of requests to Shipwire, as many orders are submitted in one'
      . ' request, and ensures successful checkout even if communication is'
      . ' temporarily interrupted. Batch processing is recommended on sites that'
      . ' have more than 60 orders per hour.'),
    '#options' => array(
      'now' => t('Immediately during checkout using Rules'),
      'later' => t('Batch submission on CRON run'),
    ),
    '#default_value' => variable_get('shipwire_ubercart_fulfill_type', 'now'),
  );

  $stock_available = module_exists('uc_stock');
  $inventory_description = '';
  if (!$stock_available) {
    $inventory_description = t('Inventory stock updates require the Ubercart'
      . ' Stock module (uc_stock). Please enable the module if you want to use'
      . ' this feature.');
  }
  $form['inventory'] = array(
    '#type' => 'fieldset',
    '#title' => t('Inventory'),
    '#description' => $inventory_description,
    '#collapsible' => TRUE,
  );
  $form['inventory']['shipwire_ubercart_inventory_update'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update product stock levels'),
    '#description' => t('When enabled product stock quantity will be checked'
      . ' in Shipwire warehouses and will update this site. If this store'
      . ' maintains stock outside of Shipwire then do not enable this feature.'),
    '#default_value' => variable_get('shipwire_ubercart_inventory_update', 0),
    '#disabled' => !$stock_available,
  );

  $form['#validate'][] = 'shipwire_ubercart_admin_settings_validate';
  return system_settings_form($form);
}

/**
 * Validation callback for shipwire ubercart settings form.
 */
function shipwire_ubercart_admin_settings_validate($form, &$form_state) {
  $fsv = $form_state['values'];

  $prefix = preg_replace('/[0-9]/', '', $fsv['shipwire_ubercart_order_prefix']);
  if (empty($prefix)) {
    form_set_error('shipwire_ubercart_order_prefix', t('Order prefix must contain non numeric chracters.'));
  }

  if (preg_match('/[^0-9]/', $fsv['shipwire_ubercart_padding_days'])) {
    form_set_error('shipwire_ubercart_padding_days', t('Delivery days padding must be a whole number.'));
  }

  $dec = variable_get('uc_currency_dec', '.');
  if ($dec == '.') {
    $dec = '\\' . $dec;
  }
  $padding_rate = preg_replace("/[^0-9$dec]/", '', $fsv['shipwire_ubercart_padding_rate']);
  if (!is_numeric($padding_rate)) {
    form_set_error('shipwire_ubercart_padding_rate', t('Delivery rate padding must be a number.'));
  }
  elseif ((float) $padding_rate < 0) {
    form_set_error('shipwire_ubercart_padding_rate', t('Delivery rate padding must be 0 or greater.'));
  }
}

/**
 * Page callback displaying all Shipwire shipments made on an order.
 *
 * @param object $order
 *   The loaded ubercart order.
 *
 * @return array
 *   Drupal build array.
 */
function shipwire_ubercart_shipment_order($order) {
  $build = array();

  $result = db_query("SELECT shipwire_shipment_id
     FROM {shipwire_shipments}
     WHERE type = 'uc_order' AND order_id = :id",
      array(':id' => $order->order_id));
  $sids = array();
  while ($shipment = $result->fetchObject()) {
    $sids[] = $shipment->shipwire_shipment_id;
  }

  if (!empty($sids)) {
    $shipments = shipwire_shipment_load_multiple($sids);
    $rows = array();

    foreach ($shipments as $shipment) {
      $status_date = $shipment->date_delivered;
      $status = $shipment->status;
      if (empty($status_date)) {
        $status_date = $shipment->date_expected;
      }
      if (empty($status_date)) {
        $status_date = $shipment->date_shipped;
      }
      if (empty($status_date)) {
        $status_date = $shipment->date_submitted;
      }
      if (empty($status_date)) {
        $status_date = $shipment->created;
      }

      $tracking = $shipment->tracking_number;
      if (!empty($tracking) && !empty($shipment->tracking_uri)) {
        $tracking = l($tracking, $shipment->tracking_uri);
      }

      $row[] = $shipment->shipwire_shipment_id;
      $row[] = $shipment->shipwire_id;
      $row[] = Shipwire::getStatusLabel($status);
      $row[] = format_date($status_date);
      $row[] = $tracking;
      $row[] = uc_currency_format($shipment->cost_total);
      $row[] = l(t('View'), 'admin/store/orders/' . $order->order_id . '/shipwire/' . $shipment->shipwire_shipment_id . '/view');
      $row[] = l(t('Edit'), 'admin/store/orders/' . $order->order_id . '/shipwire/' . $shipment->shipwire_shipment_id . '/edit');
      $row[] = l(t('Delete'), 'admin/store/orders/' . $order->order_id . '/shipwire/' . $shipment->shipwire_shipment_id . '/delete');
      $rows[] = $row;
    }
  }

  if (empty($rows)) {
    drupal_set_message(t('No Shipwire shipments have been made for this order.'));
  }
  else {
    $header = array(
      t('Shipment ID'),
      t('Shipwire ID'),
      t('Status'),
      t('Status date'),
      t('Tracking'),
      t('Total'),
      array('data' => t('Actions'), 'colspan' => 3),
    );

    $build['shipments'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  }

  return $build;
}

/**
 * Shipwire ubercart shipment edit form.
 */
function shipwire_ubercart_shipment_edit($form, &$form_state, $order, $shipment) {
  module_load_include('inc', 'shipwire', 'shipwire.admin');
  $form = shipwire_shipment_edit($form, $form_state, $shipment);
  $form['actions']['submit']['#submit'][] = 'shipwire_ubercart_shipment_edit_submit';
  return $form;
}

/**
 * Submit handler for Shipwire ubercart shipment edit form.
 */
function shipwire_ubercart_shipment_edit_submit($form, &$form_state) {
  $shipment = $form_state['shipwire_shipment'];
  $form_state['redirect'] = 'admin/store/orders/' . $shipment->order_id . '/shipwire/' . $shipment->shipwire_shipment_id . '/view';
}

/**
 * Displays shipment details.
 */
function shipwire_ubercart_shipment_view($order, $shipment, $view_mode = 'full') {
  module_load_include('inc', 'shipwire', 'shipwire.admin');
  return shipwire_shipment_view($shipment, $view_mode);
}

/**
 * Form to create a new shipment for a ubercart order.
 */
function shipwire_ubercart_shipment_new($form, &$form_state, $order) {
  $form['order_id'] = array(
    '#type' => 'value',
    '#value' => $order->order_id,
  );

  $service = $order->quote['method'];
  $method = $order->quote['accessorials'];

  if (empty($service)) {
    $message = t('This order does not have a shipping service.'
      . ' If you wish to create a shipment please edit the order and add one.');
    drupal_set_message($message);
    return $form;
  }

  $form['shipping_method'] = array(
    '#type' => 'value',
    '#value' => $method,
  );

  $products = $order->products;

  // Check for previous shipments.
  $shipments = db_query("SELECT shipwire_shipment_id, data
     FROM {shipwire_shipments}
     WHERE order_id = :id
     AND type ='ubercart_order'", array(':id' => $order->order_id));

  $products_shipped = array();
  while ($shipment = $shipments->fetchObject()) {
    $data = unserialize($shipment->data);
    if (!empty($data['products'])) {
      foreach ($data['products'] as $product) {
        $products_shipped[] = $product;
      }
    }
  }

  // Remove products that have already been shipped.
  if (count($products_shipped) && count($products)) {
    foreach ($products_shipped as $ps) {
      foreach ($products as $key => $po) {
        if ($po->sku == $ps->sku) {
          $quantity = $po->quantity - $ps->quantity;
          if ($quantity) {
            $po->setQuantity();
          }
          else {
            unset($products[$key]);
          }
        }
      }
    }
  }

  if (empty($products)) {
    $message = t('All products in this order have already been submitted to'
      . ' Shipwire for fulfillment.');
    drupal_set_message($message, 'warning');
  }
  else {

    $options = array();
    $default = array();
    foreach ($products as $product) {
      $options[$product->order_product_id] = $product->model . ' x ' . $product->qty;
      $default[] = $product->model;
    }

    $form['products_select'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Products to ship'),
      '#options' => $options,
      '#required' => TRUE,
      '#default_value' => $default,
    );

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Fulfill order'),
      '#submit' => array('shipwire_ubercart_shipment_new_submit'),
    );
  }

  return $form;
}

/**
 * Form submit handler to create a new shipment for an order.
 */
function shipwire_ubercart_shipment_new_submit($form, &$form_state) {
  $fsv = $form_state['values'];
  $reload = uc_order_load_multiple(array(), array('order_id' => $fsv['order_id']), TRUE);
  $order = $reload[$fsv['order_id']];
  if ($order) {
    $skus = array_filter($fsv['products_select']);
    $products = array();
    foreach ($skus as $sku) {
      $products[] = $order->products[$sku];
    }
    if (!empty($products)) {
      shipwire_ubercart_fulfill_now($order, $products);
      $form_state['redirect'] = 'admin/store/orders/' . $form_state['values']['order_id'] . '/shipwire';
    }
  }
}

/**
 * Callback form to delete Shipwire Shipment.
 *
 * @see shipwire_ubercart_shipment_delete_confirm_submit()
 * @ingroup forms
 */
function shipwire_ubercart_shipment_delete_confirm($form, &$form_state, $order, $shipment) {
  $form['shipwire_shipment_id'] = array('#type' => 'value', '#value' => $shipment->shipwire_shipment_id);
  $question = t('Are you sure you want to delete this shipment?');
  $path_return = 'admin/store/orders/' . $order->order_id . '/shipwire';

  $description = t('This shipment has not been submitted to Shipwire. Deleting'
    . ' it here will cancel the shipment and products will not be shipped.');
  if (!empty($shipment->date_submitted)) {
    $description = t('This shipment was already submitted to Shipwire and'
      . ' cannot be canceled using the API. Deleting the shipment here will'
      . ' only remove information that may be needed. It is recommended that'
      . ' you first cancel the shipment using the Shipwire Merchant panel and'
      . ' then it is safe to delete it here.');
  }

  return confirm_form($form, $question, $path_return, $description, t('Delete'), t('Cancel'));
}

/**
 * Submit handler for shipwire_ubercart_shipment_delete_confirm().
 *
 * @see shipwire_ubercart_shipment_delete_confirm()
 */
function shipwire_ubercart_shipment_delete_confirm_submit($form, &$form_state) {
  $shipment = shipwire_shipment_load($form_state['values']['shipwire_shipment_id']);
  shipwire_shipment_delete($shipment);
}

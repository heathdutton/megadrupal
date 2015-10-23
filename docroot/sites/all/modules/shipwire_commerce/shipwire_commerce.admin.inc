<?php

/**
 * @file
 * Admin functions for Shipwire Commerce.
 */

/**
 * Admin form for shipwire commerce settings. 
 * 
 * @return array
 *   Drupal form array.
 */
function shipwire_commerce_admin_settings() {
  $form = array();

  if (!Shipwire::operational()) {
    $form['configure'] = array(
      '#markup' => t('Shipwire account settings must be configured first.') . ' '
      . l(t('Configure Shipwire'), 'admin/shipwire/settings'),
    );
    return system_settings_form($form);
  }

  $form['shipwire_commerce_order_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Order ID prefix'),
    '#description' => t('To ensure unique order numbers in the Shipwire system'
      . ' please specify an order prefix. This will be added to the beginning of'
      . ' the Commerce order number e.g. Prefix "SC-" and Commerce order number'
      . ' "11573" form Order id "SC-11573".'),
    '#default_value' => variable_get('shipwire_commerce_order_prefix', 'SC-'),
    '#size' => 5,
    '#required' => TRUE,
  );

  $form['fulfill'] = array(
    '#type' => 'fieldset',
    '#title' => t('Order fulfillment'),
    '#collapsible' => TRUE,
  );
  $form['fulfill']['shipwire_commerce_fulfill_type'] = array(
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
      'now' => t('Immediately during checkout'),
      'later' => t('Batch submission on CRON run'),
    ),
    '#default_value' => variable_get('shipwire_commerce_fulfill_type', 'now'),
  );

  $inventory_allow = FALSE;
  $inventory_description = t('Inventory stock updates require the Commerce'
    . ' Stock module (commerce_stock) 7.x-1.x. Please enable the module if you'
    . ' want to use this feature.');

  $cs_info = system_get_info('module', 'commerce_stock');
  if ($cs_info) {
    $inventory_description = t('Inventory stock updates require the Commerce'
      . ' Stock module (commerce_stock) 7.x-1.x');
    if (!empty($cs_info['version']) && strpos($cs_info['version'], '7.x-1.') === 0) {
      $inventory_allow = TRUE;
      $inventory_description = '';
    }
  }

  if (!$inventory_allow) {
    variable_set('shipwire_commerce_inventory_update', 0);
  }

  $form['inventory'] = array(
    '#type' => 'fieldset',
    '#title' => t('Inventory'),
    '#description' => $inventory_description,
    '#collapsible' => TRUE,
  );
  $form['inventory']['shipwire_commerce_inventory_update'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update product stock levels'),
    '#description' => t('When enabled product stock quantity will be checked'
      . ' in Shipwire warehouses and will update this site. If this store'
      . ' maintains stock outside of Shipwire then do not enable this feature.'),
    '#default_value' => variable_get('shipwire_commerce_inventory_update', 0),
    '#disabled' => !$inventory_allow,
  );

  $form['#validate'][] = 'shipwire_commerce_admin_settings_validate';
  return system_settings_form($form);
}

/**
 * Validation callback for shipwire commerce settings form.
 */
function shipwire_commerce_admin_settings_validate($form, &$form_state) {
  $fsv = $form_state['values'];

  $prefix = trim(preg_replace('/[0-9]/', '', $fsv['shipwire_commerce_order_prefix']));
  if (empty($prefix)) {
    form_set_error('shipwire_commerce_order_prefix', t('Order prefix must contain non numeric chracters.'));
  }
}

/**
 * Shipwire commerce shipment edit form.
 */
function shipwire_commerce_shipment_edit($form, &$form_state, $order, $shipment) {
  module_load_include('inc', 'shipwire', 'shipwire.admin');
  $form = shipwire_shipment_edit($form, $form_state, $shipment);
  $form['actions']['submit']['#submit'][] = 'shipwire_commerce_shipment_edit_submit';
  return $form;
}

/**
 * Submit handler for Shipwire commerce shipment edit form.
 */
function shipwire_commerce_shipment_edit_submit($form, &$form_state) {
  $shipment = $form_state['shipwire_shipment'];
  $form_state['redirect'] = 'admin/commerce/orders/' . $shipment->order_id . '/shipwire/' . $shipment->shipwire_shipment_id . '/view';
}

/**
 * Page callback displaying all Shipwire shipments made on an order.
 * 
 * @param object $order
 *   The loaded commerce order.
 * 
 * @return array
 *   Drupal build array.
 */
function shipwire_commerce_shipment_order($order) {
  $build = array();

  $result = db_query("SELECT shipwire_shipment_id
     FROM {shipwire_shipments}
     WHERE order_id = :id", array(':id' => $order->order_id));
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
      $row[] = commerce_currency_format($shipment->cost_total, $shipment->currency, NULL, FALSE);
      $row[] = l(t('View'), 'admin/commerce/orders/' . $order->order_id . '/shipwire/' . $shipment->shipwire_shipment_id . '/view');
      $row[] = l(t('Edit'), 'admin/commerce/orders/' . $order->order_id . '/shipwire/' . $shipment->shipwire_shipment_id . '/edit');
      $row[] = l(t('Delete'), 'admin/commerce/orders/' . $order->order_id . '/shipwire/' . $shipment->shipwire_shipment_id . '/delete');
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
 * Callback form to delete Shipwire Shipment.
 *
 * @see shipwire_commerce_shipment_delete_confirm_submit()
 * @ingroup forms
 */
function shipwire_commerce_shipment_delete_confirm($form, &$form_state, $order, $shipment) {
  $form['shipwire_shipment_id'] = array('#type' => 'value', '#value' => $shipment->shipwire_shipment_id);
  $question = t('Are you sure you want to delete this shipment?');
  $path_return = 'admin/commerce/orders/' . $order->order_id . '/shipwire';

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
 * Submit handler for shipwire_commerce_shipment_delete_confirm().
 *
 * @see shipwire_commerce_shipment_delete_confirm()
 */
function shipwire_commerce_shipment_delete_confirm_submit($form, &$form_state) {
  $shipment = shipwire_shipment_load($form_state['values']['shipwire_shipment_id']);
  shipwire_shipment_delete($shipment);
}

/**
 * Displays shipment details.
 */
function shipwire_commerce_shipment_view($order, $shipment, $view_mode = 'full') {
  module_load_include('inc', 'shipwire', 'shipwire.admin');
  return shipwire_shipment_view($shipment, $view_mode);
}

/**
 * Form to create a new shipment for a commerce order.
 */
function shipwire_commerce_shipment_new($form, &$form_state, $order) {
  $form['order_id'] = array(
    '#type' => 'value',
    '#value' => $order->order_id,
  );

  $service = shipwire_commerce_get_order_shipping_service($order);
  $method = shipwire_commerce_get_method($service);

  if (empty($method)) {
    $message = t('This order does not have a Shipwire shipping line item.'
      . ' If you wish to create a shipment please edit the order and add one.');
    drupal_set_message($message);
    return $form;
  }

  $form['shipping_method'] = array(
    '#type' => 'value',
    '#value' => $method,
  );

  $products = shipwire_commerce_get_products($order);

  // Check for previous shipments.
  $shipments = db_query("SELECT shipwire_shipment_id, data
     FROM {shipwire_shipments}
     WHERE order_id = :id
     AND type ='commerce_order'", array(':id' => $order->order_id));

  $products_shipped = array();
  while ($shipment = $shipments->fetchObject()) {
    $data = unserialize($shipment->data);
    if (isset($data['products'])) {
      if (count($products)) {
        foreach ($products as $product) {
          $products_shipped[] = $product;
        }
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
    $form['products'] = array(
      '#type' => 'value',
      '#value' => $product,
    );

    $options = array();
    $default = array();
    foreach ($products as $product) {
      $options[$product->sku] = $product->sku . ' x ' . $product->quantity;
      $default[] = $product->sku;
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
      '#submit' => array('shipwire_commerce_shipment_new_submit'),
    );
  }

  return $form;
}

/**
 * Form submit handler to create a new shipment for an order.
 */
function shipwire_commerce_shipment_new_submit($form, &$form_state) {
  $fsv = $form_state['values'];
  $reload = commerce_order_load_multiple(array(), array('order_number' => $fsv['order_id']), TRUE);
  $order = $reload[$fsv['order_id']];
  if ($order) {
    $skus = array_filter($fsv['products_select']);
    $products = array();
    foreach ($skus as $sku) {
      $data = $fsv['products_data'][$sku];
      $products[] = new ShipwireProduct($data['sku'], $data['quantity'], $data['currency'], $data['cost']);
    }
    if (!empty($products)) {
      shipwire_commerce_fulfill_now($order, $products);
      $form_state['redirect'] = 'admin/commerce/orders/' . $form_state['values']['order_id'] . '/shipwire';
    }
  }
}

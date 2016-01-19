<?php

/**
 * @file
 * Rules for shipwire ubercart shipments.
 */

/**
 * Implements hook_rules_action_info().
 */
function shipwire_ubercart_rules_action_info() {
  return array(
    'shipwire_ubercart_rules_action_fulfill' => array(
      'label' => t('Ship order using Shipwire'),
      'parameter' => array(
        'order' => array(
          'type' => 'uc_order',
          'label' => t('Order')),
      ),
      'group' => t('Shipwire Ubercart'),
    ),
    'shipwire_ubercart_rules_action_inventory_shipment' => array(
      'label' => t('Update inventory after order fulfillment'),
      'parameter' => array(
        'shipwire_shipment' => array('type' => 'shipwire_shipment', 'label' => t('Shipwire shipment')),
      ),
      'group' => t('Shipwire Ubercart'),
    ),
    'shipwire_ubercart_rules_action_modify_shipping' => array(
      'label' => t('Change shipping method'),
      'parameter' => array(
        'order' => array(
          'type' => 'uc_order',
          'label' => t('Order'),
          'save' => TRUE,
        ),
        'service' => array(
          'type' => 'text',
          'label' => t('Shipping service'),
          'options list' => 'shipwire_ubercart_rules_service_options',
          'restriction' => 'input',
          'description' => t('Warning: This updates order shipping service'
            . ' only and does not change order line items or totals. Customer'
            . ' will not be charged for additional shipping costs.'),
        ),
      ),
      'group' => t('Shipwire Ubercart'),
    ),
    'shipwire_ubercart_rules_action_modify_rate' => array(
      'label' => t('Modify shipping rate'),
      'group' => t('Shipwire Ubercart'),
      'parameter' => array(
        'rates' => array(
          'type' => 'shipwire_rates',
          'label' => t('Shipwire rates'),
        ),
        'request_order' => array(
          'type' => 'shipwire_request_order',
          'label' => t('Shipwire request order'),
        ),
        'rate_method' => array(
          'type' => 'text',
          'label' => t('Method'),
          'options list' => 'shipwire_ubercart_rules_service_options',
        ),
        'rate_text' => array(
          'type' => 'text',
          'label' => t('Rate text'),
        ),
        'rate_amount' => array(
          'type' => 'decimal',
          'label' => t('Rate amount'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_event_info().
 */
function shipwire_ubercart_rules_event_info() {
  return array(
    'shipwire_ubercart_rules_event_order_fulfilled' => array(
      'label' => t('Order submitted to Shipwire'),
      'group' => t('Shipwire Ubercart'),
      'variables' => array(
        'order' => array(
          'label' => t('Ubercart order'),
          'type' => 'uc_order'),
        'shipwire_shipment' => array(
          'label' => t('Shipwire shipment'),
          'type' => 'shipwire_shipment'),
      ),
    ),
    'shipwire_ubercart_rules_event_order_shipped' => array(
      'label' => t('Order shipped from Shipwire'),
      'group' => t('Shipwire Ubercart'),
      'variables' => array(
        'order_id' => array(
          'label' => t('Ubercart order'),
          'type' => 'uc_order'),
        'shipwire_shipment' => array(
          'label' => t('Shipwire shipment'),
          'type' => 'shipwire_shipment'),
      ),
    ),
    'shipwire_ubercart_rules_event_order_delivered' => array(
      'label' => t('Order shipment marked delivered'),
      'group' => t('Shipwire Ubercart'),
      'variables' => array(
        'order_id' => array(
          'label' => t('Ubercart order'),
          'type' => 'uc_order'),
        'shipwire_shipment' => array(
          'label' => t('Shipwire shipment'),
          'type' => 'shipwire_shipment'),
      ),
    ),
    'shipwire_ubercart_rules_quote_generated' => array(
      'label' => t('Shipping rate quote generated'),
      'group' => t('Shipwire Ubercart'),
      'variables' => array(
        'order' => array(
          'label' => t('Ubercart order ID'),
          'type' => 'uc_order'),
        'rates' => array(
          'label' => t('Shipping rates'),
          'type' => 'shipwire_rates'),
        'request_order' => array(
          'label' => t('Request order'),
          'type' => 'shipwire_request_order'),
      ),
    ),
  );
}

/**
 * Implements hook__data_type_info().
 */
function shipwire_ubercart_data_type_info() {
  return array(
    'shipwire_rates' => array(
      'label' => t('Shipwire rates'),
      'group' => t('Shipwire Ubercart'),
    ),
    'shipwire_request_order' => array(
      'label' => t('Shipwire request order'),
      'group' => t('Shipwire Ubercart'),
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 */
function shipwire_ubercart_rules_condition_info() {
  return array(
    'shipwire_ubercart_rules_order_first_paid_in_full' => array(
      'label' => t('When an order is first paid in full'),
      'group' => t('Shipwire Ubercart'),
      'parameter' => array(
        'order' => array(
          'type' => 'uc_order',
          'label' => t('Order'),
          'restriction' => 'selector',
        ),
      ),
    ),
  );
}

/**
 * Submit an ubercart order for fulfillment with Shipwire.
 *
 * @param object $order
 *   Fully loaded ubercart order.
 */
function shipwire_ubercart_rules_action_fulfill($order) {
  $fulfill = variable_get('shipwire_ubercart_fulfill_type', 'now');
  if ($fulfill == 'now') {
    shipwire_ubercart_fulfill_now($order);
  }
  else {
    shipwire_ubercart_fulfill_later($order);
  }
}

/**
 * Check inventory in Shipwire after shipment has been fulfilled.
 *
 * @param ShipwireShipment $shipment
 *   Fully loaded shipwire shipment.
 */
function shipwire_ubercart_rules_action_inventory_shipment(ShipwireShipment $shipment) {
  if (Shipwire::operational() && module_exists('uc_stock')) {
    try {
      $skus = array();
      foreach ($shipment->data['products'] as $product) {
        $skus[] = $product->sku;
      }

      $query = 'SELECT sku, nid
        FROM {uc_product_stock}
        WHERE active = 1
          AND SKU IN :skus"';

      $result = db_query($query, array(':skus' => $skus));

      $request = new ShipwireInventoryRequest();

      if ($result) {
        while ($product = $result->fetchObject()) {
          $sku_id_map[$product->sku] = $product->nid;
          $request->addProductSku($product->sku);
        }
      }

      if (count($sku_id_map)) {
        $shipwire = Shipwire::getInstance();
        $response = $shipwire->requestInventory($request);

        // Process products in response.
        if (count($response['data_xml']->Product)) {
          foreach ($response['data_xml']->Product as $sw) {
            $sw_attrib = $sw->attributes();
            $sku = (string) $sw_attrib->code;

            if (!empty($sku_id_map[$sku])) {
              $stock = new stdClass();
              $stock->sku = check_plain($sku);
              $stock->nid = $sku_id_map[$sku];
              $stock->stock = check_plain((int) $sw_attrib->quantity);
              drupal_write_record('uc_product_stock', $stock);
            }
          }
        }
      }
    }
    catch (Exception $e) {
      $message = 'Error updating inventory for shipment !shipment_id ubercart order !order_id.';
      $message_vars = array(
        '!shipment_id' => $shipment->shipwire_shipment_id,
        '!order_id' => $shipment->order_id,
      );
      Shipwire::logError($message, $message_vars, WATCHDOG_ERROR, $e);
    }
  }
}

/**
 * Check the order balance has been paid in full for first time.
 *
 * @param object $order
 *   Fully loaded ubercart order.
 *
 * @return bool
 *   Returns TRUE only if the last payment set the order balance below zero.
 */
function shipwire_ubercart_rules_order_first_paid_in_full($order) {
  $total = $order->order_total;
  $payments = uc_payment_load_payments($order->order_id);

  if ($payments === FALSE) {
    return FALSE;
  }

  $last_payment = count($payments) - 1;
  foreach ($payments as $key => $payment) {
    $total -= $payment->amount;
    if ($total <= 0.01 && $key != $last_payment) {
      return FALSE;
    }
  }

  return $total <= 0.01;
}

/**
 * Options list callback listing Shipwire shipping methods.
 *
 * @return array
 *  Keyed by shipping method code wioth value of shipping method label.
 *
 * @see shipwire_ubercart_rules_action_info()
 */
function shipwire_ubercart_rules_service_options() {
  $opts = Shipwire::getShippingMethods();
  return $opts;
}

/**
 * Rules action callback to modify shipping method.
 *
 * @param obj $order
 *  Fully loaded ubercart order.
 * @param string $service
 *  Shipping method code.
 */
function shipwire_ubercart_rules_action_modify_shipping($order, $service) {
  if (empty($order) || empty($service)) {
    return FALSE;
  }
  $order->quote['method'] = 'shipwire_ubercart';
  $order->quote['accessorials'] = $service;
}

/**
 * Rules action callback to modify shipping quote.
 *
 * @param array $rates
 * @param obj $request_order
 * @param string $rate_method
 * @param string $rate_text
 * @param float $rate_amount
 */
function shipwire_ubercart_rules_action_modify_rate($rates, $request_order, $rate_method, $rate_text, $rate_amount) {
  if (isset($rates[$rate_method])) {
    $changed = FALSE;
    if ($rate_amount != '') {
      $rates[$rate_method]['rate'] = $rate_amount;
      $rates[$rate_method]['format'] = uc_currency_format($rate_amount);
      $changed = TRUE;
    }
    if (!empty($rate_text)) {
      $rates[$rate_method]['option_label'] = $rate_text . ': ' . $rates[$rate_method]['option_label'];
      $changed = TRUE;
    }
    if ($changed) {
      shipwire_rates_cache_set($request_order, $rates);
    }
  }
}

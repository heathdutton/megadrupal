<?php

/**
 * @file
 * Rules for Shipwire Commerce.
 */

/**
 * Implements hook_rules_action_info().
 */
function shipwire_commerce_rules_action_info() {
  return array(
    'shipwire_commerce_rules_action_fulfill' => array(
      'label' => t('Ship order using Shipwire'),
      'parameter' => array(
        'commerce_order' => array('type' => 'commerce_order', 'label' => t('Commerce order')),
      ),
      'group' => t('Shipwire Commerce'),
    ),
    'shipwire_commerce_rules_action_inventory_shipment' => array(
      'label' => t('Update inventory after order fulfillment'),
      'parameter' => array(
        'shipwire_shipment' => array('type' => 'shipwire_shipment', 'label' => t('Shipwire shipment')),
      ),
      'group' => t('Shipwire Commerce'),
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 */
function shipwire_commerce_rules_condition_info() {
  return array(
    'shipwire_commerce_rules_condition_order_shipping_is_shipwire' => array(
      'label' => t('Order shipping service is Shipwire'),
      'parameter' => array(
        'commerce_order' => array('label' => t('Commerce order'), 'type' => 'commerce_order'),
      ),
      'group' => t('Shipwire Commerce'),
    ),
  );
}

/**
 * Implements hook_rules_event_info().
 */
function shipwire_commerce_rules_event_info() {
  $items = array(
    'shipwire_commerce_rules_event_order_fulfilled' => array(
      'label' => t('Order submitted to shipwire'),
      'group' => t('Shipwire Commerce'),
      'variables' => array(
        'commerce_order_id' => array(
          'label' => t('Commerce order ID'),
          'type' => 'integer'),
        'shipwire_shipment_id' => array(
          'label' => t('Shipwire shipment ID'),
          'type' => 'integer'),
      ),
    ),
    'shipwire_commerce_rules_event_order_shipped' => array(
      'label' => t('Order shipped from shipwire'),
      'group' => t('Shipwire Commerce'),
      'variables' => array(
        'commerce_order_id' => array(
          'label' => t('Commerce order ID'),
          'type' => 'integer'),
        'shipwire_shipment_id' => array(
          'label' => t('Shipwire shipment ID'),
          'type' => 'integer'),
      ),
    ),
    'shipwire_commerce_rules_event_order_delivered' => array(
      'label' => t('Order shipment marked delivered'),
      'group' => t('Shipwire Commerce'),
      'variables' => array(
        'commerce_order_id' => array(
          'label' => t('Commerce order ID'),
          'type' => 'integer'),
        'shipwire_shipment_id' => array(
          'label' => t('Shipwire shipment ID'),
          'type' => 'integer'),
      ),
    ),
  );
  return $items;
}

/**
 * Checks order to see is shipping is by Shipwire. 
 * 
 * @param object $order
 *   The loaded commerce order.
 * 
 * @return bool
 *   True on success.
 */
function shipwire_commerce_rules_condition_order_shipping_is_shipwire($order) {
  $services = shipwire_commerce_method_service_map();
  $orderwrapper = entity_metadata_wrapper('commerce_order', $order);
  foreach ($orderwrapper->commerce_line_items as $line_item) {
    if ($line_item->type->value() === 'shipping') {
      $shipping = $line_item->commerce_shipping_service->value();
      if (array_search($shipping, $services)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * Submits shipment for fulfillment.
 * 
 * @param object $order
 *   The loaded commerce order.
 */
function shipwire_commerce_rules_action_fulfill($order) {
  $fulfill = variable_get('shipwire_commerce_fulfill_type', 'now');
  if ($fulfill == 'now') {
    shipwire_commerce_fulfill_now($order);
  }
  else {
    shipwire_commerce_fulfill_later($order);
  }
}

/**
 * Check inventory in Shipwire after shipment has been fulfilled.
 * 
 * @param ShipwireShipment $shipment
 *   Fully loaded shipwire shipment.
 */
function shipwire_commerce_rules_action_inventory_shipment(ShipwireShipment $shipment) {
  if (Shipwire::operational() && module_exists('commerce_stock')) {
    try {
      $skus = array();
      foreach ($shipment->data['products'] as $product) {
        $skus[] = $product->sku;
      }

      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'commerce_product')
        ->fieldCondition('commerce_stock')
        ->propertyCondition('sku', $skus, 'IN')
        ->addMetaData('account', user_load(1));
      $result = $query->execute();

      if (isset($result['commerce_product'])) {
        $pids = array_keys($result['commerce_product']);
        $products = commerce_product_load_multiple($pids);

        $sku_id_map = array();
        $request = new ShipwireInventoryRequest();

        foreach ($products as $id => $product) {
          $sku_id_map[$product->sku] = $id;
          $request->addProductSku($product->sku);
        }

        $shipwire = Shipwire::getInstance();
        $response = $shipwire->requestInventory($request);

        // Process products in response.
        if (count($response['data_xml']->Product)) {
          foreach ($response['data_xml']->Product as $sw) {
            $sw_attrib = $sw->attributes();
            $sku = (string) $sw_attrib->code;
            $quantity = check_plain((int) $sw_attrib->quantity);

            if (!empty($sku_id_map[$sku])) {
              $pid = $sku_id_map[$sku];
              $product = $products[$pid];
              $pw = entity_metadata_wrapper('commerce_product', $product);
              $pw->commerce_stock->set($quantity);
              $pw->save();
              unset($sku_id_map[$sku]);
            }
          }
        }
      }
    }
    catch (Exception $e) {
      $message = 'Error updating inventory for shipment !shipment_id commerce order !order_id.';
      $message_vars = array(
        '!shipment_id' => $shipment->shipwire_shipment_id,
        '!order_id' => $shipment->order_id,
      );
      Shipwire::logError($message, $message_vars, WATCHDOG_ERROR, $e);
    }
  }
}
